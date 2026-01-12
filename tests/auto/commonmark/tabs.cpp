/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// Qt include.
#include <QDir>

#include "load.h"

// 2.2 Tabs

TEST_CASE("001")
{
    const auto doc = load_test(1);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(doc->items().at(1).get());
    REQUIRE(c->isInline() == false);
    REQUIRE(c->syntax().isEmpty());
    REQUIRE(c->text() == QStringLiteral("foo\tbaz\t\tbim"));
}

TEST_CASE("002")
{
    const auto doc = load_test(2);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(doc->items().at(1).get());
    REQUIRE(c->isInline() == false);
    REQUIRE(c->syntax().isEmpty());
    REQUIRE(c->text() == QStringLiteral("foo\tbaz\t\tbim"));
}

TEST_CASE("003")
{
    const auto doc = load_test(3);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(doc->items().at(1).get());
    REQUIRE(c->isInline() == false);
    REQUIRE(c->syntax().isEmpty());
    REQUIRE(c->text() == QString::fromUtf8(u8"a\ta\nὐ\ta"));
}

TEST_CASE("004")
{
    const auto doc = load_test(4);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);

    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->listType() == MD::ListItem::Unordered);
    REQUIRE(li->items().size() == 2);

    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    auto p1 = static_cast<MD::Paragraph *>(li->items().at(0).get());
    REQUIRE(p1->items().size() == 1);
    REQUIRE(p1->items().at(0)->type() == MD::ItemType::Text);
    auto t1 = static_cast<MD::Text *>(p1->items().at(0).get());
    REQUIRE(t1->opts() == MD::TextWithoutFormat);
    REQUIRE(t1->text() == QStringLiteral("foo"));

    REQUIRE(li->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p2 = static_cast<MD::Paragraph *>(li->items().at(1).get());
    REQUIRE(p2->items().size() == 1);
    REQUIRE(p2->items().at(0)->type() == MD::ItemType::Text);
    auto t2 = static_cast<MD::Text *>(p2->items().at(0).get());
    REQUIRE(t2->opts() == MD::TextWithoutFormat);
    REQUIRE(t2->text() == QStringLiteral("bar"));
}

TEST_CASE("005")
{
    const auto doc = load_test(5);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);

    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->listType() == MD::ListItem::Unordered);
    REQUIRE(li->items().size() == 2);

    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    auto p1 = static_cast<MD::Paragraph *>(li->items().at(0).get());
    REQUIRE(p1->items().size() == 1);
    REQUIRE(p1->items().at(0)->type() == MD::ItemType::Text);
    auto t1 = static_cast<MD::Text *>(p1->items().at(0).get());
    REQUIRE(t1->opts() == MD::TextWithoutFormat);
    REQUIRE(t1->text() == QStringLiteral("foo"));

    REQUIRE(li->items().at(1)->type() == MD::ItemType::Code);
    auto c2 = static_cast<MD::Code *>(li->items().at(1).get());
    REQUIRE(c2->isInline() == false);
    REQUIRE(c2->syntax().isEmpty());
    REQUIRE(c2->text() == QStringLiteral("  bar"));
}

TEST_CASE("006")
{
    const auto doc = load_test(6);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);

    auto q = static_cast<MD::Blockquote *>(doc->items().at(1).get());
    REQUIRE(q->items().size() == 1);

    REQUIRE(q->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(q->items().at(0).get());
    REQUIRE(c->isInline() == false);
    REQUIRE(c->syntax().isEmpty());
    REQUIRE(c->text() == QStringLiteral("  foo"));
}

TEST_CASE("007")
{
    const auto doc = load_test(7);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);

    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->listType() == MD::ListItem::Unordered);
    REQUIRE(li->items().size() == 1);

    REQUIRE(li->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(li->items().at(0).get());
    REQUIRE(c->isInline() == false);
    REQUIRE(c->syntax().isEmpty());
    REQUIRE(c->text() == QStringLiteral("  foo"));
}

TEST_CASE("008")
{
    const auto doc = load_test(8);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(doc->items().at(1).get());
    REQUIRE(c->isInline() == false);
    REQUIRE(c->syntax().isEmpty());
    REQUIRE(c->text() == QStringLiteral("foo\nbar"));
}

TEST_CASE("009")
{
    const auto doc = load_test(9);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l1 = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l1->items().size() == 1);
    REQUIRE(l1->items().at(0)->type() == MD::ItemType::ListItem);
    auto li1 = static_cast<MD::ListItem *>(l1->items().at(0).get());
    REQUIRE(li1->items().size() == 2);
    REQUIRE(li1->items().at(0)->type() == MD::ItemType::Paragraph);
    auto p1 = static_cast<MD::Paragraph *>(li1->items().at(0).get());
    REQUIRE(p1->items().size() == 1);
    REQUIRE(p1->items().at(0)->type() == MD::ItemType::Text);
    auto t1 = static_cast<MD::Text *>(p1->items().at(0).get());
    REQUIRE(t1->opts() == MD::TextWithoutFormat);
    REQUIRE(t1->text() == QStringLiteral("foo"));

    REQUIRE(li1->items().at(1)->type() == MD::ItemType::List);
    auto l2 = static_cast<MD::List *>(li1->items().at(1).get());
    REQUIRE(l2->items().size() == 1);
    REQUIRE(l2->items().at(0)->type() == MD::ItemType::ListItem);
    auto li2 = static_cast<MD::ListItem *>(l2->items().at(0).get());
    REQUIRE(li2->items().size() == 2);
    REQUIRE(li2->items().at(0)->type() == MD::ItemType::Paragraph);
    auto p2 = static_cast<MD::Paragraph *>(li2->items().at(0).get());
    REQUIRE(p2->items().size() == 1);
    REQUIRE(p2->items().at(0)->type() == MD::ItemType::Text);
    auto t2 = static_cast<MD::Text *>(p2->items().at(0).get());
    REQUIRE(t2->opts() == MD::TextWithoutFormat);
    REQUIRE(t2->text() == QStringLiteral("bar"));

    REQUIRE(li2->items().at(1)->type() == MD::ItemType::List);
    auto l3 = static_cast<MD::List *>(li2->items().at(1).get());
    REQUIRE(l3->items().size() == 1);
    REQUIRE(l3->items().at(0)->type() == MD::ItemType::ListItem);
    auto li3 = static_cast<MD::ListItem *>(l3->items().at(0).get());
    REQUIRE(li3->items().size() == 1);
    REQUIRE(li3->items().at(0)->type() == MD::ItemType::Paragraph);
    auto p3 = static_cast<MD::Paragraph *>(li3->items().at(0).get());
    REQUIRE(p3->items().size() == 1);
    REQUIRE(p3->items().at(0)->type() == MD::ItemType::Text);
    auto t3 = static_cast<MD::Text *>(p3->items().at(0).get());
    REQUIRE(t3->opts() == MD::TextWithoutFormat);
    REQUIRE(t3->text() == QStringLiteral("baz"));
}

TEST_CASE("010")
{
    const auto doc = load_test(10);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);

    auto h = static_cast<MD::Heading *>(doc->items().at(1).get());
    REQUIRE(h->isLabeled());
    const QString fn = QStringLiteral("/") + QDir().absolutePath() + QStringLiteral("/tests/commonmark/0.30/010.md");

    REQUIRE(h->text().get());
    auto p = h->text().get();
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(h->label() == QStringLiteral("#") + t->text() + fn);
    REQUIRE(h->level() == 1);
    REQUIRE(t->text() == QStringLiteral("Foo"));
}

TEST_CASE("011")
{
    const auto doc = load_test(11);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::HorizontalLine);
}
