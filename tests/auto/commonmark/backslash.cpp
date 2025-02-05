
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// 2.4 Backslash escapes

TEST_CASE("012")
{
    const auto doc = load_test(12);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::latin1ToString("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"));
}

TEST_CASE("013")
{
    const auto doc = load_test(13);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::utf8ToString(u8"\\\t\\A\\a\\ \\3\\φ\\«"));
}

TEST_CASE("014")
{
    const auto doc = load_test(14);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 9);

    std::vector<typename TRAIT::String> data = {TRAIT::latin1ToString("*not emphasized*"),
                                                TRAIT::latin1ToString("<br/> not a tag"),
                                                TRAIT::latin1ToString("[not a link](/foo)"),
                                                TRAIT::latin1ToString("`not code`"),
                                                TRAIT::latin1ToString("1. not a list"),
                                                TRAIT::latin1ToString("* not a list"),
                                                TRAIT::latin1ToString("# not a heading"),
                                                TRAIT::latin1ToString("[foo]: /url \"not a reference\""),
                                                TRAIT::latin1ToString("&ouml; not a character entity")};

    for (long long int i = 0; i < static_cast<long long int>(p->items().size()); ++i) {
        REQUIRE(p->items().at(i)->type() == MD::ItemType::Text);
        auto t1 = static_cast<MD::Text<TRAIT> *>(p->items().at(i).get());
        REQUIRE(t1->opts() == MD::TextWithoutFormat);
        REQUIRE(t1->text() == data.at(i));
    }
}

TEST_CASE("015")
{
    const auto doc = load_test(15);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t1 = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t1->opts() == MD::TextWithoutFormat);
    REQUIRE(t1->text() == TRAIT::latin1ToString("\\"));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
    auto t2 = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
    REQUIRE(t2->opts() == MD::ItalicText);
    REQUIRE(t2->text() == TRAIT::latin1ToString("emphasis"));
}

TEST_CASE("016")
{
    const auto doc = load_test(16);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 3);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t1 = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t1->text() == TRAIT::latin1ToString("foo"));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::LineBreak);

    REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
    auto t2 = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
    REQUIRE(t2->text() == TRAIT::latin1ToString("bar"));
}

TEST_CASE("017")
{
    const auto doc = load_test(17);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code<TRAIT> *>(p->items().at(0).get());
    REQUIRE(c->isInline() == true);
    REQUIRE(c->syntax().size() == 0);
    REQUIRE(c->text() == TRAIT::latin1ToString("\\[\\`"));
}

TEST_CASE("018")
{
    const auto doc = load_test(18);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(c->isInline() == false);
    REQUIRE(c->syntax().size() == 0);
    REQUIRE(c->text() == TRAIT::latin1ToString("\\[\\]"));
}

TEST_CASE("019")
{
    const auto doc = load_test(19);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(c->isInline() == false);
    REQUIRE(c->syntax().size() == 0);
    REQUIRE(c->text() == TRAIT::latin1ToString("\\[\\]"));
}

TEST_CASE("020")
{
    MESSAGE("This test is not strict to CommonMark 0.30.");

    const auto doc = load_test(20);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->text() == TRAIT::latin1ToString("<http://example.com?find=*>"));
}

TEST_CASE("021")
{
    const auto doc = load_test(21);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(h->text() == TRAIT::latin1ToString("<a href=\"/bar\\/)\">"));
}

TEST_CASE("022")
{
    const auto doc = load_test(22);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
    REQUIRE(l->url() == TRAIT::latin1ToString("/bar*"));
}

TEST_CASE("023")
{
    const auto doc = load_test(23);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->text() == TRAIT::latin1ToString("foo"));
    REQUIRE(doc->labeledLinks().size() == 1);
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit->second->url() == TRAIT::latin1ToString("/bar*"));
}

TEST_CASE("024")
{
    const auto doc = load_test(24);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code<TRAIT> *>(doc->items().at(1).get());

    REQUIRE(c->isInline() == false);
    REQUIRE(c->syntax() == TRAIT::latin1ToString("foo+bar"));
    REQUIRE(c->text() == TRAIT::latin1ToString("foo"));
}
