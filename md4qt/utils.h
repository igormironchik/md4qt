/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef MD4QT_MD_UTILS_HPP_INCLUDED
#define MD4QT_MD_UTILS_HPP_INCLUDED

namespace MD
{

//
// DISABLE_COPY
//

//! Macro for disabling copy.
#define MD_DISABLE_COPY(Class)                                                                                                                                 \
    Class(const Class &) = delete;                                                                                                                             \
    Class &operator=(const Class &) = delete;

//
// UNUSED
//

//! Avoid "unused parameter" warnings.
#define MD_UNUSED(x) (void)x;

} /* namespace MD */

#endif // MD4QT_MD_UTILS_HPP_INCLUDED
