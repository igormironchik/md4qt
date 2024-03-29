/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef MD4QT_MD_UTILS_HPP_INCLUDED
#define MD4QT_MD_UTILS_HPP_INCLUDED

namespace MD {

//
// DISABLE_COPY
//

//! Macro for disabling copy.
#define DISABLE_COPY( Class ) \
	Class( const Class & ) = delete; \
	Class & operator= ( const Class & ) = delete;

} /* namespace MD */

#endif // MD4QT_MD_UTILS_HPP_INCLUDED
