/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include "parser.h"
#include "yaml_parser.h"

QSharedPointer<MD::Document> loadTest(const QString &name)
{
    MD::Parser p;
    return p.parse(QStringLiteral("tests/plugins/yaml/data/") + name + QStringLiteral(".md"));
}

TEST_CASE("001")
{
    auto doc = loadTest(QStringLiteral("001"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 1);
    REQUIRE(doc->items().at(0)->type() == MD::ItemType::Anchor);
}

/*


---
id: 1
...
text

*/
TEST_CASE("002")
{
    auto doc = loadTest(QStringLiteral("002"));

    const auto checkDoc = [](QSharedPointer<MD::Document> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 3);

        REQUIRE(doc->items().at(1)->type()
                == static_cast<MD::ItemType>(static_cast<int>(MD::ItemType::UserDefined) + 1));
        auto h = static_cast<MD::YAMLHeader *>(doc->items().at(1).get());
        REQUIRE(h->startColumn() == 0);
        REQUIRE(h->startLine() == 2);
        REQUIRE(h->endColumn() == 3);
        REQUIRE(h->endLine() == 4);
        REQUIRE(h->yaml() == QStringLiteral("id: 1"));
        REQUIRE(h->startDelim() == MD::WithPosition{0, 2, 2, 2});
        REQUIRE(h->endDelim() == MD::WithPosition{0, 4, 3, 4});

        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 5);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 5);
        REQUIRE(t->text() == QStringLiteral("text"));
    };

    checkDoc(doc);
    checkDoc(doc->clone().staticCast<MD::Document>());
}

/*
---
id: 1
...
text

*/
TEST_CASE("003")
{
    auto doc = loadTest(QStringLiteral("003"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == static_cast<MD::ItemType>(static_cast<int>(MD::ItemType::UserDefined) + 1));
    auto h = static_cast<MD::YAMLHeader *>(doc->items().at(1).get());
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 3);
    REQUIRE(h->endLine() == 2);
    REQUIRE(h->yaml() == QStringLiteral("id: 1"));
    REQUIRE(h->startDelim() == MD::WithPosition{0, 0, 2, 0});
    REQUIRE(h->endDelim() == MD::WithPosition{0, 2, 3, 2});

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->startColumn() == 0);
    REQUIRE(t->startLine() == 3);
    REQUIRE(t->endColumn() == 3);
    REQUIRE(t->endLine() == 3);
    REQUIRE(t->text() == QStringLiteral("text"));
}

/*
text
---
id: 1
...
text

*/
TEST_CASE("004")
{
    auto doc = loadTest(QStringLiteral("004"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
}

/*
> ---
> id: 1
> ...
> text

*/
TEST_CASE("005")
{
    auto doc = loadTest(QStringLiteral("005"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);
    auto b = static_cast<MD::Blockquote *>(doc->items().at(1).get());
    REQUIRE(b->items().size() == 2);
    REQUIRE(b->items().at(0)->type() == MD::ItemType::HorizontalLine);
    REQUIRE(b->items().at(1)->type() == MD::ItemType::Paragraph);
}
