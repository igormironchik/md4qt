
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#pragma once

#include <md4qt/parser.h>

#include <string>

#ifdef MD4QT_QT_SUPPORT
#include <QString>
#endif

inline std::shared_ptr<MD::Document<TRAIT>> load_test(int n, const std::string &folder = "0.30")
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

    MD::Parser<TRAIT> p;

    return p.parse(
#ifdef MD4QT_QT_SUPPORT
        QString::fromStdString(path),
#else
        path,
#endif
        false);
}
