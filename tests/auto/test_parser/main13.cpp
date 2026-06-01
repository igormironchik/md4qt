/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include "parser.h"

// Qt include.
#include <QTextStream>

/*
[foo](/url "the title")
*/
TEST_CASE("link_title_inline_link_dquote")
{
    QString md = QStringLiteral("[foo](/url \"the title\")");
    QTextStream stream(&md);
    MD::Parser parser;
    auto doc = parser.parse(stream, QString(), QString());

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->url() == QStringLiteral("/url"));
    REQUIRE(l->title() == QStringLiteral("the title"));
}

/*
[foo](/url 'the title')
*/
TEST_CASE("link_title_inline_link_squote")
{
    QString md = QStringLiteral("[foo](/url 'the title')");
    QTextStream stream(&md);
    MD::Parser parser;
    auto doc = parser.parse(stream, QString(), QString());

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->url() == QStringLiteral("/url"));
    REQUIRE(l->title() == QStringLiteral("the title"));
}

/*
[foo](/url (the title))
*/
TEST_CASE("link_title_inline_link_parens")
{
    QString md = QStringLiteral("[foo](/url (the title))");
    QTextStream stream(&md);
    MD::Parser parser;
    auto doc = parser.parse(stream, QString(), QString());

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->url() == QStringLiteral("/url"));
    REQUIRE(l->title() == QStringLiteral("the title"));
}

/*
[foo](/url)
*/
TEST_CASE("link_title_inline_link_none")
{
    QString md = QStringLiteral("[foo](/url)");
    QTextStream stream(&md);
    MD::Parser parser;
    auto doc = parser.parse(stream, QString(), QString());

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->title().isEmpty());
}

/*
![foo](/url "img title")
*/
TEST_CASE("link_title_inline_image")
{
    QString md = QStringLiteral("![foo](/url \"img title\")");
    QTextStream stream(&md);
    MD::Parser parser;
    auto doc = parser.parse(stream, QString(), QString());

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto img = static_cast<MD::Image *>(p->items().at(0).get());
    REQUIRE(img->url() == QStringLiteral("/url"));
    REQUIRE(img->title() == QStringLiteral("img title"));
}

/*
[foo]

[foo]: /url "my title"
*/
TEST_CASE("link_title_shortcut_link")
{
    QString md = QStringLiteral("[foo]\n\n[foo]: /url \"my title\"");
    QTextStream stream(&md);
    MD::Parser parser;
    auto doc = parser.parse(stream, QString(), QStringLiteral("test.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->title() == QStringLiteral("my title"));
}

/*
![foo]

[foo]: /url "my title"
*/
TEST_CASE("link_title_shortcut_image")
{
    QString md = QStringLiteral("![foo]\n\n[foo]: /url \"my title\"");
    QTextStream stream(&md);
    MD::Parser parser;
    auto doc = parser.parse(stream, QString(), QStringLiteral("test.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto img = static_cast<MD::Image *>(p->items().at(0).get());
    REQUIRE(img->url() == QStringLiteral("/url"));
    REQUIRE(img->title() == QStringLiteral("my title"));
}

/*
[text][foo]

[foo]: /url "my title"
*/
TEST_CASE("link_title_full_ref_link")
{
    QString md = QStringLiteral("[text][foo]\n\n[foo]: /url \"my title\"");
    QTextStream stream(&md);
    MD::Parser parser;
    auto doc = parser.parse(stream, QString(), QStringLiteral("test.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->title() == QStringLiteral("my title"));
}

/*
![alt][foo]

[foo]: /url "my title"
*/
TEST_CASE("link_title_full_ref_image")
{
    QString md = QStringLiteral("![alt][foo]\n\n[foo]: /url \"my title\"");
    QTextStream stream(&md);
    MD::Parser parser;
    auto doc = parser.parse(stream, QString(), QStringLiteral("test.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
    auto img = static_cast<MD::Image *>(p->items().at(0).get());
    REQUIRE(img->url() == QStringLiteral("/url"));
    REQUIRE(img->title() == QStringLiteral("my title"));
}

/*
[foo]: /url "definition title"
*/
TEST_CASE("link_title_labeled_definition")
{
    QString md = QStringLiteral("[foo]: /url \"definition title\"");
    QTextStream stream(&md);
    MD::Parser parser;
    auto doc = parser.parse(stream, QString(), QStringLiteral("test.md"));

    REQUIRE(doc->labeledLinks().size() == 1);
    const auto it = doc->labeledLinks().cbegin();
    REQUIRE(it.value()->url() == QStringLiteral("/url"));
    REQUIRE(it.value()->title() == QStringLiteral("definition title"));
}
