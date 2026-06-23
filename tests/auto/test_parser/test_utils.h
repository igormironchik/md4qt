/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#pragma once

// md4qt include.
#include "doc.h"

// doctest include.
#include <doctest/doctest.h>

// C++ include.
#include <string>

inline void checkPositionsImpl(const char *file,
                               int line,
                               MD::Item *item,
                               qsizetype startColumn,
                               qsizetype startLine,
                               qsizetype endColumn,
                               qsizetype endLine)
{
    DOCTEST_INFO("Assert inside checkPositionsImpl() called from " << std::string(file) << ":" << line);

    REQUIRE(item->startColumn() == startColumn);
    REQUIRE(item->startLine() == startLine);
    REQUIRE(item->endColumn() == endColumn);
    REQUIRE(item->endLine() == endLine);
}

inline void checkLinesImpl(const char *file,
                           int line,
                           MD::Item *item,
                           qsizetype startLine,
                           qsizetype endLine)
{
    DOCTEST_INFO("Assert inside checkLinesImpl() called from " << std::string(file) << ":" << line);

    REQUIRE(item->startLine() == startLine);
    REQUIRE(item->endLine() == endLine);
}

#define CHECK_POSITIONS(item, startColumn, startLine, endColumn, endLine)                                              \
    checkPositionsImpl(__FILE__, __LINE__, (item), (startColumn), (startLine), (endColumn), (endLine))

#define CHECK_LINES(item, startLine, endLine) checkLinesImpl(__FILE__, __LINE__, (item), (startLine), (endLine))
