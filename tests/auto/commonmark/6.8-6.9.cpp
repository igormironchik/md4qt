
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// 6.8 Soft line breaks

TEST_CASE("648")
{
    const auto doc = load_test(648);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("foo"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("baz"));
    }
}

TEST_CASE("649")
{
    const auto doc = load_test(649);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("foo"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("baz"));
    }
}

// 6.9 Textual content

TEST_CASE("650")
{
    const auto doc = load_test(650);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::latin1ToString("hello $.;'there"));
}

TEST_CASE("651")
{
    const auto doc = load_test(651);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::utf8ToString(u8"Foo χρῆν"));
}

TEST_CASE("652")
{
    const auto doc = load_test(652);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::latin1ToString("Multiple     spaces"));
}
