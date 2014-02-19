// Copyright 2013-2014 Antony Polukhin

// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying file LICENSE_1_0.txt
// or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//[type_index_userdefined_usertypes
/*`
    The following example shows how a user defined type_info can be created and used.
    Example works with and without RTTI.

    Consider situation when user uses only those types in `typeid()`:
*/

#include <vector>
#include <string>

namespace my_namespace {

class my_class;
struct my_struct;

typedef std::vector<my_class> my_classes;
typedef std::string my_string;

} // namespace my_namespace

//] [/type_index_userdefined_usertypes]


//[type_index_userdefined_enum
/*`
    In that case user may wish to save space in binary and create it's own type system.
    For that case `detail::typenum<>` meta function is added. Depending on the input type T
    this function will return different numeric values.
*/
#include <boost/type_index/type_index_facade.hpp>

namespace my_namespace { namespace detail {
    template <class T> struct typenum;
    template <> struct typenum<void>{       enum {value = 0}; };
    template <> struct typenum<my_class>{   enum {value = 1}; };
    template <> struct typenum<my_struct>{  enum {value = 2}; };
    template <> struct typenum<my_classes>{ enum {value = 3}; };
    template <> struct typenum<my_string>{  enum {value = 4}; };

    // my_typeinfo structure is used to save type number
    struct my_typeinfo {
        // type_[0] will hold a type number
        // type_[1] will be '\0', to have a zero terminated raw type name
        char type_[2];
    };

    template <class T>
    inline const my_typeinfo& my_typeinfo_construct() {
        static const my_typeinfo ret = {{ static_cast<char>(typenum<T>::value), '\0' }};
        return ret;
    }
}} // my_namespace::detail

//] [/type_index_userdefined_usertypes]


//[type_index_my_type_index
/*`
    `my_type_index` is a user created type_index class. If in doubt during this phase, you can always 
    take a look at the `<boost/type_index/ctti_type_index.hpp>` or `<boost/type_index/stl_type_index.hpp>`
    files. Documentation for `type_index_facade` could be also useful.

    See implementation of `my_type_index`:
*/
namespace my_namespace {

class my_type_index: public boost::typeind::type_index_facade<my_type_index, detail::my_typeinfo> {
    const detail::my_typeinfo* data_;

public:
    typedef detail::my_typeinfo type_info_t;

    inline my_type_index() BOOST_NOEXCEPT
        : data_(&detail::my_typeinfo_construct<void>())
    {}

    inline my_type_index(const type_info_t& data) BOOST_NOEXCEPT
        : data_(&data)
    {}

    inline const type_info_t&  type_info() const BOOST_NOEXCEPT {
        return *data_;
    }

    inline const char*  raw_name() const BOOST_NOEXCEPT {
        return data_->type_;
    }

    inline std::string  pretty_name() const {
        // Must be in sync with detail::typenum<T>::value
        static const char* names[] = {
            "void", "my_class", "my_struct", "my_classes", "my_string"
        };

        const std::size_t indx = static_cast<std::size_t>(data_->type_[0]);
        return names[indx];
    }

    template <class T>
    inline static my_type_index type_id() BOOST_NOEXCEPT {
        return detail::my_typeinfo_construct<T>();
    }
};

} // namespace my_namespace

/*`
    Note that we have used the boost::typeind::type_index_facade class as base.
    That class took care about all the helper function and operators (comparison, hashing, ostreaming and others).
*/

//] [/type_index_my_type_index]

namespace my_namespace {

class my_class{};
struct my_struct{};

} // namespace my_namespace

//[type_index_my_type_index_usage
/*`
    Finally we can use the my_type_index class for getting type indexes:
*/

using namespace my_namespace;
#include <cassert>

int main() {
    my_type_index
        cl1 = my_type_index::type_id<my_class>(),
        st1 = my_type_index::type_id<my_struct>(),
        st2 = my_type_index::type_id<my_struct>(),
        vec = my_type_index::type_id<my_classes>()
    ;

    assert(cl1 != st1);
    assert(st2 == st1);
    assert(vec.pretty_name() == "my_classes");
    assert(cl1.pretty_name() == "my_class");
}

//] [/type_index_my_type_index_usage]

