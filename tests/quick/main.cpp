/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// md4qt include.
#include <src/parser.h>

int main()
{
    MD::Parser parser;
    QString data = QStringLiteral("* ```cpp");
    QTextStream stream(&data);

    auto doc = parser.parse(stream, QStringLiteral("."), QStringLiteral("1.md"));
    // auto doc = parser.parse(QStringLiteral("./footnote3.md"));

    return 0;
}
