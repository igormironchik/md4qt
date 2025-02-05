
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

/*
 - 1
  - 2
   - 3
    - 4
     - 5
      - 6
       - 7
      - 6
     -
    - 4
   - 3
  - 2
 - 1

*/
TEST_CASE("281")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/281.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 3);
    REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem<TRAIT>*>(l->items().at(2).get());
        REQUIRE(li->items().size() == 2);

        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(li->items().at(1).get());
        REQUIRE(l->items().size() == 5);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(3).get());
        REQUIRE(l->items().size() == 3);
    }
}

/*
 - 1
  - 2
   - 3
    - 4
     - 5
      - 6
       - 7
      - 6
     -     code
    - 4
   - 3
  - 2
 - 1

*/
TEST_CASE("282")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/282.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 3);
    REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem<TRAIT>*>(l->items().at(2).get());
        REQUIRE(li->items().size() == 2);

        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(li->items().at(1).get());
        REQUIRE(l->items().size() == 5);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(3).get());
        REQUIRE(l->items().size() == 3);
    }
}

/*
 - 1
  - 2
   - 3
    - 4
     - 5
      - 6
       - 7
      - 6
     - ```
    - 4
   - 3
  - 2
 - 1

*/
TEST_CASE("283")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/283.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 3);
    REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem<TRAIT>*>(l->items().at(2).get());
        REQUIRE(li->items().size() == 2);

        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(li->items().at(1).get());
        REQUIRE(l->items().size() == 5);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(3).get());
        REQUIRE(l->items().size() == 3);
    }
}

/*
 - 1
  - 2
   - 3
    - 4
     - 5
      - 6
       - 7
      - 6
     - > ```
    - 4
   - 3
  - 2
 - 1

*/
TEST_CASE("284")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/284.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 3);
    REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem<TRAIT>*>(l->items().at(2).get());
        REQUIRE(li->items().size() == 2);

        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(li->items().at(1).get());
        REQUIRE(l->items().size() == 5);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(3).get());
        REQUIRE(l->items().size() == 3);
    }
}

/*
 - 1
  - 2
   - 3
    - 4
     - 5
      - 6
       - 7
      - 6
     -     code

           code
    - 4
   - 3
  - 2
 - 1

*/
TEST_CASE("285")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/285.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 3);
    REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem<TRAIT>*>(l->items().at(2).get());
        REQUIRE(li->items().size() == 2);

        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(li->items().at(1).get());
        REQUIRE(l->items().size() == 5);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code<TRAIT>*>(doc->items().at(2).get());
    REQUIRE(c->text() == TRAIT::latin1ToString("- 4"));
    REQUIRE(c->startColumn() == 4);
    REQUIRE(c->startLine() == 11);
    REQUIRE(c->endColumn() == 6);
    REQUIRE(c->endLine() == 11);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(3).get());
        REQUIRE(l->items().size() == 3);
    }
}

/*

* <!--
-->

In CommonMark will be.

*/
TEST_CASE("286")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/286.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
}

/*
* <div>

text

*/
TEST_CASE("287")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/287.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
}

/*
* <?
?>

text

*/
TEST_CASE("288")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/288.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
}
