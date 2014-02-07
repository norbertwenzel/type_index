//
// Copyright (c) Antony Polukhin, 2012-2014.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_HPP
#define BOOST_TYPE_INDEX_HPP

/// \file boost/type_index.hpp
/// \brief Includes minimal set of headers required to use the Boost.TypeIndex library.
///
/// By inclusion of this file most optimal type index classes will be included and used 
/// as a boost::typeind::type_index and boost::typeind::type_info.

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/config.hpp>

#if defined(BOOST_TYPE_INDEX_USER_TYPEINDEX) && defined(BOOST_TYPE_INDEX_USER_TYPEINDEX_NAME) 
#   include BOOST_TYPE_INDEX_USER_TYPEINDEX
#elif (!defined(BOOST_NO_RTTI) && !defined(BOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY)) || defined(BOOST_MSVC)
#   include <boost/type_index/stl_type_index.hpp>
#else 
#   include <boost/type_index/ctti_type_index.hpp>
#endif

namespace boost { namespace typeind {

#if defined(BOOST_TYPE_INDEX_DOXYGEN_INVOKED)
    /// Depending on a compiler flags, optimal implementation of type_index will be used 
    /// as a default boost::typeind::type_index.
    ///
    /// Could be a boost::typeind::stl_type_index, boost::typeind::ctti_type_index or 
    /// user defined type_index class.
    typedef platform-specific type_index;
#elif defined(BOOST_TYPE_INDEX_USER_TYPEINDEX) && defined(BOOST_TYPE_INDEX_USER_TYPEINDEX_NAME) 
    typedef BOOST_TYPE_INDEX_USER_TYPEINDEX_NAME type_index;
#elif (!defined(BOOST_NO_RTTI) && !defined(BOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY)) || defined(BOOST_MSVC)
    typedef boost::typeind::stl_type_index type_index;
#else 
    typedef boost::typeind::ctti_type_index type_index;
#endif

/// Depending on a compiler flags, optimal implementation of type_info will be used 
/// as a default boost::typeind::type_info.
///
/// Could be a std::type_info, boost::typeind::detail::ctti_data or 
/// some user defined class.
typedef type_index::type_info_t type_info;

// TODO:
// inline bool is_equal(const type_info&, const type_info&) BOOST_NOEXCEPT;
// inline bool is_before(const type_info&, const type_info&) BOOST_NOEXCEPT;

#if defined(BOOST_TYPE_INDEX_DOXYGEN_INVOKED)

/// \def BOOST_TYPE_INDEX_USER_TYPEINFO
/// BOOST_TYPE_INDEX_USER_TYPEINFO can be defined to the path to header file
/// with user provided implementation of type_index.
///
/// BOOST_TYPE_INDEX_USER_TYPEINDEX_NAME must be also defined!
#define BOOST_TYPE_INDEX_USER_TYPEINDEX <full/absolute/path/to/header/with/type_index>


/// \def BOOST_TYPE_INDEX_USER_TYPEINDEX_NAME
/// BOOST_TYPE_INDEX_USER_TYPEINDEX_NAME can be defined to the fullty 
/// qualified name of the user's type_index implementation.
///
/// BOOST_TYPE_INDEX_USER_TYPEINDEX must be also defined!
#define BOOST_TYPE_INDEX_USER_TYPEINDEX_NAME my_namespace::my_type_index

#endif // defined(BOOST_TYPE_INDEX_DOXYGEN_INVOKED)


/// Function to get boost::type_index for a type T.
/// Removes const, volatile && and & modifiers from T.
///
/// \b Example:
/// \code
/// type_index ti = type_id<int&>();
/// std::cout << ti.pretty_name();  // Outputs 'int'
/// \endcode
///
/// \throw Nothing.
/// \return boost::typeind::type_index with information about the specified type T.
template <class T>
inline type_index type_id() BOOST_NOEXCEPT {
    return type_index::construct<T>();
}

/// Function for constructing boost::type_index instance for type T. 
/// Does not remove const, volatile, & and && modifiers from T.
///
/// If T has no const, volatile, & and && modifiers, then returns exactly 
/// the same result as in case of calling `type_id<T>()`.
///
/// \b Example:
/// \code
/// type_index ti = type_id_with_cvr<int&>();
/// std::cout << ti.pretty_name();  // Outputs 'int&'
/// \endcode
///
/// \throw Nothing.
/// \return boost::typeind::type_index with information about the specified type T.
template <class T>
inline type_index type_id_with_cvr() BOOST_NOEXCEPT {
    return type_index::construct_with_cvr<T>();
}

/// Function that works exactly like C++ typeid(rtti_val) call, but returns boost::type_index.
///
/// Retunrs runtime information about specified type.
///
/// \b Requirements: RTTI available or specially designed user type_info class must be provided
/// via BOOST_TYPE_INDEX_USER_TYPEINDEX and BOOST_TYPE_INDEX_USER_TYPEINDEX_NAME macro.
///
/// \b Example:
/// \code
/// struct Base { virtual ~Base(){} };
/// struct Derived: public Base  {};
/// ...
/// Derived d;
/// Base& b = d;
/// type_index ti = type_id_runtime(b);
/// std::cout << ti.pretty_name();  // Outputs 'Derived'
/// \endcode
///
/// \param runtime_val Varaible which runtime type must be returned.
/// \throw Nothing.
/// \return boost::typeind::type_index with information about the specified variable.
template <class T>
inline type_index type_id_runtime(const T& runtime_val) BOOST_NOEXCEPT {
    return type_index::construct_runtime(runtime_val);
}

/// Function that works exactly like C++ typeid(rtti_val) call, but returns boost::type_index.
/// Function that works exactly like C++ typeid(rtti_val) call, but returns boost::type_index.
///
/// Retunrs runtime information about specified type.
///
/// \b Requirements: RTTI available or specially designed user type_info class must be provided
/// via BOOST_TYPE_INDEX_USER_TYPEINDEX and BOOST_TYPE_INDEX_USER_TYPEINDEX_NAME macro.
///
/// \b Example:
/// \code
/// struct Base { virtual ~Base(){} };
/// struct Derived: public Base  {};
/// ...
/// Base* b = new Derived();
/// type_index ti = type_id_runtime(b);
/// std::cout << ti.pretty_name();  // Outputs 'Derived*'
/// \endcode
///
/// \param runtime_val Varaible which runtime type must be returned.
/// \throw Nothing.
/// \return boost::typeind::type_index with information about the specified variable.
template <class T>
inline type_index type_id_runtime(const T* runtime_val) {
    return type_index::construct_runtime(runtime_val);
}

}} // namespace boost::typeind



#endif // BOOST_TYPE_INDEX_HPP
