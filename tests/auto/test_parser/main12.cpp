/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include "parser.h"

/*
[](<a<>) []((a ))

*/
TEST_CASE("344")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/344.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
}

/*
[1]: /url (title())

[2]: /url ((title))

[3]: /url (title))

*/
TEST_CASE("345")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/345.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
}
