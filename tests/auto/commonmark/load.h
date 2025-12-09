/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#pragma once

// md4qt include.
#include "parser.h"

// C++ include.
#include <string>

// Qt include.
#include <QString>

inline QSharedPointer<MD::Document> load_test(int n,
                                              const std::string &folder = "0.30")
{
    auto fileName = std::to_string(n);

    if (fileName.size() < 3) {
        fileName.insert(0, 3 - fileName.size(), '0');
    }

    std::string path = "tests/commonmark/";
    path.append(folder);
    path.append("/");
    path.append(fileName);
    path.append(".md");

    MD::Parser p;
    p.setBlockParsers(MD::Parser::makeCommonMarkBlockParsersPipeline(&p));
    p.setInlineParsers(MD::Parser::makeCommonMarkInlineParsersPipeline());

    return p.parse(QString::fromStdString(path), false);
}
