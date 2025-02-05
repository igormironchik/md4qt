
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// 4.8 Paragraphs

TEST_CASE("219")
{
    const auto doc = load_test(219);

    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("aaa"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("bbb"));
    }
}

TEST_CASE("220")
{
    const auto doc = load_test(220);

    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 2);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("aaa"));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("bbb"));
        }
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 2);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("ccc"));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("ddd"));
        }
    }
}

TEST_CASE("221")
{
    const auto doc = load_test(221);

    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("aaa"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("bbb"));
    }
}

TEST_CASE("222")
{
    const auto doc = load_test(222);

    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 2);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("aaa"));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("bbb"));
        }
    }
}

TEST_CASE("223")
{
    const auto doc = load_test(223);

    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 3);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("aaa"));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("bbb"));
        }

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("ccc"));
        }
    }
}

TEST_CASE("224")
{
    const auto doc = load_test(224);

    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 2);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("aaa"));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("bbb"));
        }
    }
}

TEST_CASE("225")
{
    const auto doc = load_test(225);

    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(c->text() == TRAIT::latin1ToString("aaa"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("bbb"));
    }
}

TEST_CASE("226")
{
    const auto doc = load_test(226);

    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 3);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("aaa"));
        }

        REQUIRE(p->items().at(1)->type() == MD::ItemType::LineBreak);

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("bbb"));
        }
    }
}

// 4.9 Blank lines

TEST_CASE("227")
{
    const auto doc = load_test(227);

    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("aaa"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading<TRAIT> *>(doc->items().at(2).get());
        REQUIRE(h->text().get());
        auto p = h->text().get();
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(h->level() == 1);
        REQUIRE(t->text() == TRAIT::latin1ToString("aaa"));
    }
}
