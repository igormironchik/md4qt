/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "load.h"

// 2.4 Backslash escapes

TEST_CASE("012")
{
    const auto doc = load_test(12);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"));
}

TEST_CASE("013")
{
    const auto doc = load_test(13);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QString::fromUtf8(u8"\\\t\\A\\a\\ \\3\\φ\\«"));
}

TEST_CASE("014")
{
    const auto doc = load_test(14);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 9);

    std::vector<QString> data = {QStringLiteral("*not emphasized*"),
                                 QStringLiteral("<br/> not a tag"),
                                 QStringLiteral("[not a link](/foo)"),
                                 QStringLiteral("`not code`"),
                                 QStringLiteral("1. not a list"),
                                 QStringLiteral("* not a list"),
                                 QStringLiteral("# not a heading"),
                                 QStringLiteral("[foo]: /url \"not a reference\""),
                                 QStringLiteral("&ouml; not a character entity")};

    for (long long int i = 0; i < static_cast<long long int>(p->items().size()); ++i) {
        REQUIRE(p->items().at(i)->type() == MD::ItemType::Text);
        auto t1 = static_cast<MD::Text *>(p->items().at(i).get());
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

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t1 = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t1->opts() == MD::TextWithoutFormat);
    REQUIRE(t1->text() == QStringLiteral("\\"));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
    auto t2 = static_cast<MD::Text *>(p->items().at(1).get());
    REQUIRE(t2->opts() == MD::ItalicText);
    REQUIRE(t2->text() == QStringLiteral("emphasis"));
}

TEST_CASE("016")
{
    const auto doc = load_test(16);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 3);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t1 = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t1->text() == QStringLiteral("foo"));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::LineBreak);

    REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
    auto t2 = static_cast<MD::Text *>(p->items().at(2).get());
    REQUIRE(t2->text() == QStringLiteral("bar"));
}

TEST_CASE("017")
{
    const auto doc = load_test(17);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(p->items().at(0).get());
    REQUIRE(c->isInline() == true);
    REQUIRE(c->syntax().size() == 0);
    REQUIRE(c->text() == QStringLiteral("\\[\\`"));
}

TEST_CASE("018")
{
    const auto doc = load_test(18);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(doc->items().at(1).get());
    REQUIRE(c->isInline() == false);
    REQUIRE(c->syntax().size() == 0);
    REQUIRE(c->text() == QStringLiteral("\\[\\]"));
}

TEST_CASE("019")
{
    const auto doc = load_test(19);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(doc->items().at(1).get());
    REQUIRE(c->isInline() == false);
    REQUIRE(c->syntax().size() == 0);
    REQUIRE(c->text() == QStringLiteral("\\[\\]"));
}

TEST_CASE("020")
{
    MESSAGE("This test is not strict to CommonMark 0.30.");

    const auto doc = load_test(20);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->text() == QStringLiteral("<http://example.com?find=*>"));
}

TEST_CASE("021")
{
    const auto doc = load_test(21);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<a href=\"/bar\\/)\">"));
}

TEST_CASE("022")
{
    const auto doc = load_test(22);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->text() == QStringLiteral("foo"));
    REQUIRE(l->url() == QStringLiteral("/bar*"));
}

TEST_CASE("023")
{
    const auto doc = load_test(23);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->text() == QStringLiteral("foo"));
    REQUIRE(doc->labeledLinks().size() == 1);
    const auto lit = doc->labeledLinks().find(l->url());
    REQUIRE(lit != doc->labeledLinks().cend());
    REQUIRE(lit.value()->url() == QStringLiteral("/bar*"));
}

TEST_CASE("024")
{
    const auto doc = load_test(24);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(doc->items().at(1).get());

    REQUIRE(c->isInline() == false);
    REQUIRE(c->syntax() == QStringLiteral("foo+bar"));
    REQUIRE(c->text() == QStringLiteral("foo"));
}
