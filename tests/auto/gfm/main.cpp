
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include "parser.h"

inline QSharedPointer<MD::Document> load_test(int n)
{
    auto fileName = std::to_string(n);

    if (fileName.size() < 3) {
        fileName.insert(0, 3 - fileName.size(), '0');
    }

    fileName.insert(0, "tests/gfm/data/");
    fileName.append(".md");

    MD::Parser p;

    return p.parse(QString::fromStdString(fileName), false);
}

TEST_CASE("198")
{
    const auto doc = load_test(198);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Table);
    const auto t = static_cast<MD::Table *>(doc->items().at(1).get());
    REQUIRE(t->columnsCount() == 2);
    REQUIRE(t->rows().size() == 2);

    REQUIRE(t->columnAlignment(0) == MD::Table::AlignLeft);
    REQUIRE(t->columnAlignment(1) == MD::Table::AlignLeft);

    {
        REQUIRE(t->rows().at(0)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(0).get());
        REQUIRE(r->cells().size() == 2);

        {
            REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("foo"));
        }

        {
            REQUIRE(r->cells().at(1)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(1).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("bar"));
        }
    }

    {
        REQUIRE(t->rows().at(1)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(1).get());
        REQUIRE(r->cells().size() == 2);

        {
            REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("baz"));
        }

        {
            REQUIRE(r->cells().at(1)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(1).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("bim"));
        }
    }
}

TEST_CASE("199")
{
    const auto doc = load_test(199);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Table);
    const auto t = static_cast<MD::Table *>(doc->items().at(1).get());
    REQUIRE(t->columnsCount() == 2);
    REQUIRE(t->rows().size() == 2);

    REQUIRE(t->columnAlignment(0) == MD::Table::AlignCenter);
    REQUIRE(t->columnAlignment(1) == MD::Table::AlignRight);

    {
        REQUIRE(t->rows().at(0)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(0).get());
        REQUIRE(r->cells().size() == 2);

        {
            REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("abc"));
        }

        {
            REQUIRE(r->cells().at(1)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(1).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("defghi"));
        }
    }

    {
        REQUIRE(t->rows().at(1)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(1).get());
        REQUIRE(r->cells().size() == 2);

        {
            REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("bar"));
        }

        {
            REQUIRE(r->cells().at(1)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(1).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("baz"));
        }
    }
}

TEST_CASE("200")
{
    const auto doc = load_test(200);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Table);
    const auto t = static_cast<MD::Table *>(doc->items().at(1).get());
    REQUIRE(t->columnsCount() == 1);
    REQUIRE(t->rows().size() == 3);

    REQUIRE(t->columnAlignment(0) == MD::Table::AlignLeft);

    {
        REQUIRE(t->rows().at(0)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(0).get());
        REQUIRE(r->cells().size() == 1);

        REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
        const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
        REQUIRE(c->items().size() == 1);
        REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
        const auto t = static_cast<MD::Text *>(c->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("f|oo"));
    }

    {
        REQUIRE(t->rows().at(1)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(1).get());
        REQUIRE(r->cells().size() == 1);

        REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
        const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
        REQUIRE(c->items().size() == 3);

        {
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("b "));
        }

        {
            REQUIRE(c->items().at(1)->type() == MD::ItemType::Code);
            const auto cc = static_cast<MD::Code *>(c->items().at(1).get());
            REQUIRE(cc->text() == QStringLiteral("|"));
        }

        {
            REQUIRE(c->items().at(2)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(2).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral(" az"));
        }
    }

    {
        REQUIRE(t->rows().at(2)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(2).get());
        REQUIRE(r->cells().size() == 1);

        REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
        const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
        REQUIRE(c->items().size() == 3);

        {
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("b "));
        }

        {
            REQUIRE(c->items().at(1)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(1).get());
            REQUIRE(t->opts() == MD::BoldText);
            REQUIRE(t->text() == QStringLiteral("|"));
        }

        {
            REQUIRE(c->items().at(2)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(2).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral(" im"));
        }
    }
}

TEST_CASE("201")
{
    const auto doc = load_test(201);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Table);
    const auto t = static_cast<MD::Table *>(doc->items().at(1).get());
    REQUIRE(t->columnsCount() == 2);
    REQUIRE(t->rows().size() == 2);

    REQUIRE(t->columnAlignment(0) == MD::Table::AlignLeft);
    REQUIRE(t->columnAlignment(1) == MD::Table::AlignLeft);

    {
        REQUIRE(t->rows().at(0)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(0).get());
        REQUIRE(r->cells().size() == 2);

        {
            REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("abc"));
        }

        {
            REQUIRE(r->cells().at(1)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(1).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("def"));
        }
    }

    {
        REQUIRE(t->rows().at(1)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(1).get());
        REQUIRE(r->cells().size() == 2);

        {
            REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("bar"));
        }

        {
            REQUIRE(r->cells().at(1)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(1).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("baz"));
        }
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Blockquote);
    const auto b = static_cast<MD::Blockquote *>(doc->items().at(2).get());
    REQUIRE(b->items().size() == 1);
    REQUIRE(b->items().at(0)->type() == MD::ItemType::Paragraph);
    const auto p = static_cast<MD::Paragraph *>(b->items().at(0).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        const auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("bar"));
    }
}

TEST_CASE("202")
{
    const auto doc = load_test(202);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Table);
    const auto t = static_cast<MD::Table *>(doc->items().at(1).get());
    REQUIRE(t->columnsCount() == 2);
    REQUIRE(t->rows().size() == 3);

    REQUIRE(t->columnAlignment(0) == MD::Table::AlignLeft);
    REQUIRE(t->columnAlignment(1) == MD::Table::AlignLeft);

    {
        REQUIRE(t->rows().at(0)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(0).get());
        REQUIRE(r->cells().size() == 2);

        {
            REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("abc"));
        }

        {
            REQUIRE(r->cells().at(1)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(1).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("def"));
        }
    }

    {
        REQUIRE(t->rows().at(1)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(1).get());
        REQUIRE(r->cells().size() == 2);

        {
            REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("bar"));
        }

        {
            REQUIRE(r->cells().at(1)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(1).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("baz"));
        }
    }

    {
        REQUIRE(t->rows().at(2)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(2).get());
        REQUIRE(r->cells().size() == 1);

        {
            REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("bar"));
        }
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    const auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    REQUIRE(p->items().size() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        const auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("bar"));
    }
}

TEST_CASE("203")
{
    const auto doc = load_test(203);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    const auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        const auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("| abc | def |"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        const auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("| --- |"));
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        const auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("| bar |"));
    }
}

TEST_CASE("204")
{
    const auto doc = load_test(204);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Table);
    const auto t = static_cast<MD::Table *>(doc->items().at(1).get());
    REQUIRE(t->columnsCount() == 2);
    REQUIRE(t->rows().size() == 3);

    REQUIRE(t->columnAlignment(0) == MD::Table::AlignLeft);
    REQUIRE(t->columnAlignment(1) == MD::Table::AlignLeft);

    {
        REQUIRE(t->rows().at(0)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(0).get());
        REQUIRE(r->cells().size() == 2);

        {
            REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("abc"));
        }

        {
            REQUIRE(r->cells().at(1)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(1).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("def"));
        }
    }

    {
        REQUIRE(t->rows().at(1)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(1).get());
        REQUIRE(r->cells().size() == 1);

        {
            REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("bar"));
        }
    }

    {
        REQUIRE(t->rows().at(2)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(2).get());
        REQUIRE(r->cells().size() == 2);

        {
            REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("bar"));
        }

        {
            REQUIRE(r->cells().at(1)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(1).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("baz"));
        }
    }
}

TEST_CASE("205")
{
    const auto doc = load_test(205);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Table);
    const auto t = static_cast<MD::Table *>(doc->items().at(1).get());
    REQUIRE(t->columnsCount() == 2);
    REQUIRE(t->rows().size() == 1);

    REQUIRE(t->columnAlignment(0) == MD::Table::AlignLeft);
    REQUIRE(t->columnAlignment(1) == MD::Table::AlignLeft);

    {
        REQUIRE(t->rows().at(0)->type() == MD::ItemType::TableRow);
        const auto r = static_cast<MD::TableRow *>(t->rows().at(0).get());
        REQUIRE(r->cells().size() == 2);

        {
            REQUIRE(r->cells().at(0)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(0).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("abc"));
        }

        {
            REQUIRE(r->cells().at(1)->type() == MD::ItemType::TableCell);
            const auto c = static_cast<MD::TableCell *>(r->cells().at(1).get());
            REQUIRE(c->items().size() == 1);
            REQUIRE(c->items().at(0)->type() == MD::ItemType::Text);
            const auto t = static_cast<MD::Text *>(c->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("def"));
        }
    }
}

TEST_CASE("279")
{
    const auto doc = load_test(279);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    const auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 2);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        const auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(li->isTaskList());
        REQUIRE(!li->isChecked());
        REQUIRE(li->items().size() == 1);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        const auto p = static_cast<MD::Paragraph *>(li->items().at(0).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        const auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("foo"));
    }

    {
        REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);
        const auto li = static_cast<MD::ListItem *>(l->items().at(1).get());
        REQUIRE(li->isTaskList());
        REQUIRE(li->isChecked());
        REQUIRE(li->items().size() == 1);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        const auto p = static_cast<MD::Paragraph *>(li->items().at(0).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        const auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("bar"));
    }
}

TEST_CASE("280")
{
    const auto doc = load_test(280);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    const auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 2);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        const auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(li->isTaskList());
        REQUIRE(li->isChecked());
        REQUIRE(li->items().size() == 2);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        const auto p = static_cast<MD::Paragraph *>(li->items().at(0).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        const auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("foo"));

        {
            REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
            const auto l = static_cast<MD::List *>(li->items().at(1).get());
            REQUIRE(l->items().size() == 2);

            {
                REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
                const auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
                REQUIRE(li->isTaskList());
                REQUIRE(!li->isChecked());
                REQUIRE(li->items().size() == 1);
                REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
                const auto p = static_cast<MD::Paragraph *>(li->items().at(0).get());
                REQUIRE(p->items().size() == 1);
                REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
                const auto t = static_cast<MD::Text *>(p->items().at(0).get());
                REQUIRE(t->opts() == MD::TextWithoutFormat);
                REQUIRE(t->text() == QStringLiteral("bar"));
            }

            {
                REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);
                const auto li = static_cast<MD::ListItem *>(l->items().at(1).get());
                REQUIRE(li->isTaskList());
                REQUIRE(li->isChecked());
                REQUIRE(li->items().size() == 1);
                REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
                const auto p = static_cast<MD::Paragraph *>(li->items().at(0).get());
                REQUIRE(p->items().size() == 1);
                REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
                const auto t = static_cast<MD::Text *>(p->items().at(0).get());
                REQUIRE(t->opts() == MD::TextWithoutFormat);
                REQUIRE(t->text() == QStringLiteral("baz"));
            }
        }
    }

    {
        REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);
        const auto li = static_cast<MD::ListItem *>(l->items().at(1).get());
        REQUIRE(li->isTaskList());
        REQUIRE(!li->isChecked());
        REQUIRE(li->items().size() == 1);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        const auto p = static_cast<MD::Paragraph *>(li->items().at(0).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        const auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("bim"));
    }
}

TEST_CASE("491")
{
    const auto doc = load_test(491);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    const auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        const auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::StrikethroughText);
        REQUIRE(t->text() == QStringLiteral("Hi"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        const auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral(" Hello, world!"));
    }
}

TEST_CASE("492")
{
    const auto doc = load_test(492);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        const auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        const auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("This ~~has a"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        const auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        const auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("new paragraph~~."));
    }
}
