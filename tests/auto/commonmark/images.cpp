
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// 6.4 Images

TEST_CASE("571")
{
    const auto doc = load_test(571);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("foo"));
    REQUIRE(i->url() == TRAIT::latin1ToString("/url"));

    REQUIRE(i->p().get());
    REQUIRE(i->p()->items().size() == 1);
    REQUIRE(i->p()->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(i->p()->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::latin1ToString("foo"));
}

TEST_CASE("572")
{
    const auto doc = load_test(572);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("foo *bar*"));
    REQUIRE(i->url() == TRAIT::latin1ToString("train.jpg"));
}

TEST_CASE("573")
{
    const auto doc = load_test(573);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("foo ![bar](/url)"));
    REQUIRE(i->url() == TRAIT::latin1ToString("/url2"));
}

TEST_CASE("574")
{
    const auto doc = load_test(574);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("foo [bar](/url)"));
    REQUIRE(i->url() == TRAIT::latin1ToString("/url2"));
}

TEST_CASE("575")
{
    const auto doc = load_test(575);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("foo *bar*"));
    REQUIRE(i->url() == TRAIT::latin1ToString("train.jpg"));
}

TEST_CASE("576")
{
    const auto doc = load_test(576);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("foo *bar*"));
    REQUIRE(i->url() == TRAIT::latin1ToString("train.jpg"));
}

TEST_CASE("577")
{
    const auto doc = load_test(577);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("foo"));
    REQUIRE(i->url() == TRAIT::latin1ToString("train.jpg"));
}

TEST_CASE("578")
{
    const auto doc = load_test(578);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::latin1ToString("My "));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(1).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("foo bar"));
    REQUIRE(i->url() == TRAIT::latin1ToString("/path/to/train.jpg"));
}

TEST_CASE("579")
{
    const auto doc = load_test(579);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("foo"));
    REQUIRE(i->url() == TRAIT::latin1ToString("url"));
}

TEST_CASE("580")
{
    const auto doc = load_test(580);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text().isEmpty());
    REQUIRE(i->url() == TRAIT::latin1ToString("/url"));
}

TEST_CASE("581")
{
    const auto doc = load_test(581);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("foo"));
    REQUIRE(i->url() == TRAIT::latin1ToString("/url"));
}

TEST_CASE("582")
{
    const auto doc = load_test(582);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("foo"));
    REQUIRE(i->url() == TRAIT::latin1ToString("/url"));
}

TEST_CASE("583")
{
    const auto doc = load_test(583);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("foo"));
    REQUIRE(i->url() == TRAIT::latin1ToString("/url"));
}

TEST_CASE("584")
{
    const auto doc = load_test(584);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("*foo* bar"));
    REQUIRE(i->url() == TRAIT::latin1ToString("/url"));
}

TEST_CASE("585")
{
    const auto doc = load_test(585);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("Foo"));
    REQUIRE(i->url() == TRAIT::latin1ToString("/url"));
}

TEST_CASE("586")
{
    const auto doc = load_test(586);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("foo"));
    REQUIRE(i->url() == TRAIT::latin1ToString("/url"));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::latin1ToString("[]"));
}

TEST_CASE("587")
{
    const auto doc = load_test(587);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("foo"));
    REQUIRE(i->url() == TRAIT::latin1ToString("/url"));
}

TEST_CASE("588")
{
    const auto doc = load_test(588);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("*foo* bar"));
    REQUIRE(i->url() == TRAIT::latin1ToString("/url"));
}

TEST_CASE("589")
{
    const auto doc = load_test(589);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("![[foo]]"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("[[foo]]: /url \"title\""));
    }
}

TEST_CASE("590")
{
    const auto doc = load_test(590);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(0).get());
    REQUIRE(!i->isEmpty());
    REQUIRE(i->text() == TRAIT::latin1ToString("Foo"));
    REQUIRE(i->url() == TRAIT::latin1ToString("/url"));
}

TEST_CASE("591")
{
    const auto doc = load_test(591);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::latin1ToString("![foo]"));
}

TEST_CASE("592")
{
    const auto doc = load_test(592);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::latin1ToString("!"));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(1).get());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::latin1ToString("/url"));
}
