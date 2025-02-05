
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// 4.7 Link reference definitions

TEST_CASE("192")
{
    const auto doc = load_test(192);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::latin1ToString("/url"));
}

TEST_CASE("193")
{
    const auto doc = load_test(193);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::latin1ToString("/url"));
}

TEST_CASE("194")
{
    const auto doc = load_test(194);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == TRAIT::latin1ToString("Foo*bar]"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::latin1ToString("my_(url)"));
}

TEST_CASE("195")
{
    const auto doc = load_test(195);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == TRAIT::latin1ToString("Foo bar"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::latin1ToString("my url"));
}

TEST_CASE("196")
{
    const auto doc = load_test(196);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::latin1ToString("/url"));
}

TEST_CASE("197")
{
    const auto doc = load_test(197);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("[foo]: /url 'title"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("with blank line'"));
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(3).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("[foo]"));
    }
}

TEST_CASE("198")
{
    const auto doc = load_test(198);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::latin1ToString("/url"));
}

TEST_CASE("199")
{
    const auto doc = load_test(199);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("[foo]:"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("[foo]"));
    }
}

TEST_CASE("200")
{
    const auto doc = load_test(200);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::latin1ToString(""));
}

TEST_CASE("201")
{
    const auto doc = load_test(201);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 3);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("[foo]: "));
        }

        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml<TRAIT> *>(p->items().at(1).get());
        REQUIRE(h->text() == TRAIT::latin1ToString("<bar>"));

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("(baz)"));
        }
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("[foo]"));
    }
}

TEST_CASE("202")
{
    const auto doc = load_test(202);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::latin1ToString("/url\\bar*baz"));
}

TEST_CASE("203")
{
    const auto doc = load_test(203);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::latin1ToString("url"));
}

TEST_CASE("204")
{
    const auto doc = load_test(204);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::latin1ToString("first"));
}

TEST_CASE("205")
{
    const auto doc = load_test(205);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == TRAIT::latin1ToString("Foo"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::latin1ToString("/url"));
}

TEST_CASE("206")
{
    const auto doc = load_test(206);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == TRAIT::utf8ToString(u8"αγω"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::utf8ToString(u8"/φου"));
}

TEST_CASE("207")
{
    const auto doc = load_test(207);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 1);

    REQUIRE(doc->labeledLinks().size() == 1);
}

TEST_CASE("208")
{
    const auto doc = load_test(208);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::latin1ToString("bar"));
    REQUIRE(doc->labeledLinks().size() == 1);
}

TEST_CASE("209")
{
    const auto doc = load_test(209);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::latin1ToString("[foo]: /url \"title\" ok"));
}

TEST_CASE("210")
{
    const auto doc = load_test(210);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::latin1ToString("\"title\" ok"));

    REQUIRE(doc->labeledLinks().size() == 1);
}

TEST_CASE("211")
{
    const auto doc = load_test(211);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(!c->isInline());
    REQUIRE(c->text() == TRAIT::latin1ToString("[foo]: /url \"title\""));

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::latin1ToString("[foo]"));
}

TEST_CASE("212")
{
    const auto doc = load_test(212);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(!c->isInline());
    REQUIRE(c->text() == TRAIT::latin1ToString("[foo]: /url"));

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::latin1ToString("[foo]"));
}

TEST_CASE("213")
{
    const auto doc = load_test(213);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 2);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("Foo"));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("[bar]: /baz"));
        }
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("[bar]"));
    }
}

TEST_CASE("214")
{
    const auto doc = load_test(214);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(h->level() == 1);
        auto p = h->text().get();
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
        REQUIRE(l->text() == TRAIT::latin1ToString("Foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit->second->url() == TRAIT::latin1ToString("/url"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Blockquote);
        auto b = static_cast<MD::Blockquote<TRAIT> *>(doc->items().at(2).get());
        REQUIRE(b->items().size() == 1);
        REQUIRE(b->items().at(0)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(b->items().at(0).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("bar"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
    }
}

TEST_CASE("215")
{
    const auto doc = load_test(215);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(h->level() == 1);
        auto p = h->text().get();
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("bar"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
        REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit->second->url() == TRAIT::latin1ToString("/url"));
    }
}

TEST_CASE("216")
{
    const auto doc = load_test(216);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 2);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("==="));

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(1).get());
        REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit->second->url() == TRAIT::latin1ToString("/url"));
    }
}

TEST_CASE("217")
{
    const auto doc = load_test(217);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 5);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
        REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit->second->url() == TRAIT::latin1ToString("/foo-url"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString(","));
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(2).get());
        REQUIRE(l->text() == TRAIT::latin1ToString("bar"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit->second->url() == TRAIT::latin1ToString("/bar-url"));
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(3).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString(","));
    }

    {
        REQUIRE(p->items().at(4)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(4).get());
        REQUIRE(l->text() == TRAIT::latin1ToString("baz"));
        const auto lit = doc->labeledLinks().find(l->url());
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit->second->url() == TRAIT::latin1ToString("/baz-url"));
    }
}

TEST_CASE("218")
{
    const auto doc = load_test(218);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::latin1ToString("/url"));

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Blockquote);
    auto b = static_cast<MD::Blockquote<TRAIT> *>(doc->items().at(2).get());
    REQUIRE(b->isEmpty());
}
