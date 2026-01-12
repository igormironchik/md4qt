/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "load.h"

// 6 Inlines

TEST_CASE("327")
{
    const auto doc = load_test(327);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("hi"));
    REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(1).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("lo`"));
}

// 6.1 Code spans

TEST_CASE("328")
{
    const auto doc = load_test(328);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("foo"));
}

TEST_CASE("329")
{
    const auto doc = load_test(329);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("foo ` bar"));
}

TEST_CASE("330")
{
    const auto doc = load_test(330);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("``"));
}

TEST_CASE("331")
{
    const auto doc = load_test(331);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral(" `` "));
}

TEST_CASE("332")
{
    const auto doc = load_test(332);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral(" a"));
}

TEST_CASE("333")
{
    const auto doc = load_test(333);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QString::fromUtf16(u"\x00A0\x0062\x00A0"));
}

TEST_CASE("334")
{
    const auto doc = load_test(334);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code *>(p->items().at(0).get());
        REQUIRE(c->isInline());
        REQUIRE(c->text() == QString::fromUtf16(u"\x00A0"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code *>(p->items().at(1).get());
        REQUIRE(c->isInline());
        REQUIRE(c->text() == QStringLiteral("  "));
    }
}

TEST_CASE("335")
{
    const auto doc = load_test(335);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("foo bar   baz"));
}

TEST_CASE("336")
{
    const auto doc = load_test(336);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("foo "));
}

TEST_CASE("337")
{
    const auto doc = load_test(337);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("foo   bar  baz"));
}

TEST_CASE("338")
{
    const auto doc = load_test(338);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("foo\\"));
    REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(1).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("bar`"));
}

TEST_CASE("339")
{
    const auto doc = load_test(339);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("foo`bar"));
}

TEST_CASE("340")
{
    const auto doc = load_test(340);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("foo `` bar"));
}

TEST_CASE("341")
{
    const auto doc = load_test(341);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("*foo"));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(1).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("*"));
}

TEST_CASE("342")
{
    const auto doc = load_test(342);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[not a "));
    }

    REQUIRE(p->items().at(1)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(1).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("link](/foo"));

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral(")"));
    }
}

TEST_CASE("343")
{
    const auto doc = load_test(343);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("<a href=\""));

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("\">`"));
    }
}

TEST_CASE("344")
{
    const auto doc = load_test(344);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(0).get());
    REQUIRE(h->text() == QStringLiteral("<a href=\"`\">"));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(1).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("`"));
}

TEST_CASE("345")
{
    const auto doc = load_test(345);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("<http://foo.bar."));

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("baz>`"));
    }
}

TEST_CASE("346") // Not strict to CommonMark as URL is not valid.
{
    MESSAGE("This test is not strict to CommonMark 0.30.");

    MESSAGE("Skip for now.");
}

TEST_CASE("347")
{
    const auto doc = load_test(347);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("```foo``"));
}

TEST_CASE("348")
{
    const auto doc = load_test(348);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("`foo"));
}

TEST_CASE("349")
{
    const auto doc = load_test(349);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("`foo"));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(1).get());
    REQUIRE(c->isInline());
    REQUIRE(c->text() == QStringLiteral("bar"));
}
