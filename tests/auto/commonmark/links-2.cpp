/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "load.h"

// 6.3 Links

TEST_CASE("526")
{
    const auto doc = load_test(526);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == QStringLiteral("foo"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit.value()->url() == QStringLiteral("/url"));
}

TEST_CASE("527")
{
    const auto doc = load_test(527);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == QStringLiteral("link [foo [bar]]"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit.value()->url() == QStringLiteral("/uri"));
}

TEST_CASE("528")
{
    const auto doc = load_test(528);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == QStringLiteral("link [bar"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit.value()->url() == QStringLiteral("/uri"));
}

TEST_CASE("529")
{
    const auto doc = load_test(529);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == QStringLiteral("link *foo **bar** `#`*"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit.value()->url() == QStringLiteral("/uri"));
}

TEST_CASE("530")
{
    const auto doc = load_test(530);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == QStringLiteral("![moon](moon.jpg)"));
    REQUIRE(!l->img()->isEmpty());
    REQUIRE(l->img()->text() == QStringLiteral("moon"));
    REQUIRE(l->img()->url() == QStringLiteral("moon.jpg"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit.value()->url() == QStringLiteral("/uri"));
}

TEST_CASE("531")
{
    const auto doc = load_test(531);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[foo "));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(1).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("bar"));
        REQUIRE(l->url() == QStringLiteral("/uri"));
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("]"));
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(3).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("ref"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/uri"));
    }
}

TEST_CASE("532")
{
    const auto doc = load_test(532);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 5);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[foo "));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::ItalicText);
        REQUIRE(t->text() == QStringLiteral("bar "));
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(2).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::ItalicText);
        REQUIRE(l->text() == QStringLiteral("baz"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/uri"));
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(3).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("]"));
    }

    {
        REQUIRE(p->items().at(4)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(4).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("ref"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/uri"));
    }
}

TEST_CASE("533")
{
    const auto doc = load_test(533);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("*"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(1).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("foo*"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/uri"));
    }
}

TEST_CASE("534")
{
    const auto doc = load_test(534);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("foo *bar"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/uri"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("*"));
    }
}

TEST_CASE("535")
{
    const auto doc = load_test(535);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("[foo "));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<bar attr=\"][ref]\">"));
}

TEST_CASE("536")
{
    const auto doc = load_test(536);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[foo"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code *>(p->items().at(1).get());
        REQUIRE(c->text() == QStringLiteral("][ref]"));
    }
}

TEST_CASE("537")
{
    const auto doc = load_test(537);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[foo"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(1).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text().isEmpty());
        REQUIRE(l->url() == QStringLiteral("http://example.com/?search=][ref]"));
    }
}

TEST_CASE("538")
{
    const auto doc = load_test(538);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == QStringLiteral("foo"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit.value()->url() == QStringLiteral("/url"));
}

TEST_CASE("539")
{
    const auto doc = load_test(539);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == QString::fromUtf8(u8"ẞ"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit.value()->url() == QStringLiteral("/url"));
}

TEST_CASE("540")
{
    const auto doc = load_test(540);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == QStringLiteral("Baz"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit.value()->url() == QStringLiteral("/url"));
}

TEST_CASE("541")
{
    const auto doc = load_test(541);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[foo] "));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(1).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("bar"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url"));
    }
}

TEST_CASE("542")
{
    const auto doc = load_test(542);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[foo]"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(1).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("bar"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url"));
    }
}

TEST_CASE("543")
{
    const auto doc = load_test(543);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("bar"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url1"));
    }
}

TEST_CASE("544")
{
    const auto doc = load_test(544);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[bar][foo!]"));
    }
}

TEST_CASE("545")
{
    const auto doc = load_test(545);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[foo][ref[]"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[ref[]: /uri"));
    }
}

TEST_CASE("546")
{
    const auto doc = load_test(546);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[foo][ref[bar]]"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[ref[bar]]: /uri"));
    }
}

TEST_CASE("547")
{
    const auto doc = load_test(547);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[[[foo]]]"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[[[foo]]]: /url"));
    }
}

TEST_CASE("548")
{
    const auto doc = load_test(548);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/uri"));
    }
}

TEST_CASE("549")
{
    const auto doc = load_test(549);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("bar\\"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/uri"));
    }
}

TEST_CASE("550")
{
    const auto doc = load_test(550);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[]"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[]: /uri"));
    }
}

TEST_CASE("551")
{
    const auto doc = load_test(551);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 2);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("["));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("]"));
        }
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 2);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("["));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("]: /uri"));
        }
    }
}

TEST_CASE("552")
{
    const auto doc = load_test(552);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("foo"));
        REQUIRE(l->startColumn() == 0);
        REQUIRE(l->startLine() == 0);
        REQUIRE(l->endColumn() == 6);
        REQUIRE(l->endLine() == 0);
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url"));
    }
}

TEST_CASE("553")
{
    const auto doc = load_test(553);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("*foo* bar"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url"));
    }
}

TEST_CASE("554")
{
    const auto doc = load_test(554);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("Foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url"));
    }
}

TEST_CASE("555")
{
    const auto doc = load_test(555);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[]"));
    }
}

TEST_CASE("556")
{
    const auto doc = load_test(556);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url"));
    }
}

TEST_CASE("557")
{
    const auto doc = load_test(557);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("*foo* bar"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url"));
    }
}

TEST_CASE("558")
{
    const auto doc = load_test(558);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("["));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(1).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("*foo* bar"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url"));
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("]"));
    }
}

TEST_CASE("559")
{
    const auto doc = load_test(559);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[[bar "));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(1).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url"));
    }
}

TEST_CASE("560")
{
    const auto doc = load_test(560);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("Foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url"));
    }
}

TEST_CASE("561")
{
    const auto doc = load_test(561);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral(" bar"));
    }
}

TEST_CASE("562")
{
    const auto doc = load_test(562);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[foo]"));
    }
}

TEST_CASE("563")
{
    const auto doc = load_test(563);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("*"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(1).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("foo*"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url"));
    }
}

TEST_CASE("564")
{
    const auto doc = load_test(564);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url2"));
    }
}

TEST_CASE("565")
{
    const auto doc = load_test(565);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url1"));
    }
}

TEST_CASE("566")
{
    const auto doc = load_test(566);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("foo"));
        REQUIRE(l->url().isEmpty());
    }
}

TEST_CASE("567")
{
    const auto doc = load_test(567);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url1"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("(not a link)"));
    }
}

TEST_CASE("568")
{
    const auto doc = load_test(568);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[foo]"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(1).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("bar"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url"));
    }
}

TEST_CASE("569")
{
    const auto doc = load_test(569);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url2"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(1).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("baz"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url1"));
    }
}

TEST_CASE("570")
{
    const auto doc = load_test(570);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[foo]"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(1).get());
        REQUIRE(l->img()->isEmpty());
        REQUIRE(l->opts() == MD::TextWithoutFormat);
        REQUIRE(l->text() == QStringLiteral("bar"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("/url1"));
    }
}
