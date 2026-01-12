/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include "gfm_autolink_parser.h"
#include "parser.h"

// Qt include.
#include <QDir>
#include <QFile>

/*
*<a>text</a>*

*/
TEST_CASE("249")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/249.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(0).get());
        REQUIRE(h->opts() == MD::ItalicText);
        REQUIRE(h->text() == QStringLiteral("<a>"));
        REQUIRE(h->openStyles().size() == 1);
        REQUIRE(h->openStyles().at(0) == MD::StyleDelim{MD::ItalicText, 0, 0, 0, 0});
        REQUIRE(h->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::ItalicText);
        REQUIRE(t->text() == QStringLiteral("text"));
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(2).get());
        REQUIRE(h->opts() == MD::ItalicText);
        REQUIRE(h->text() == QStringLiteral("</a>"));
        REQUIRE(h->openStyles().empty());
        REQUIRE(h->closeStyles().size() == 1);
        REQUIRE(h->closeStyles().at(0) == MD::StyleDelim{MD::ItalicText, 12, 0, 12, 0});
    }
}

/*
## headig {#heading} ##

*/
TEST_CASE("251")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/251.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
    auto h = static_cast<MD::Heading *>(doc->items().at(1).get());
    REQUIRE(h->label().startsWith(QStringLiteral("#heading")));
    REQUIRE(h->text()->items().size() == 1);
    REQUIRE(h->text()->startColumn() == 3);
    REQUIRE(h->text()->startLine() == 0);
    REQUIRE(h->text()->endColumn() == 20);
    REQUIRE(h->text()->endLine() == 0);
    REQUIRE(h->labelPos() == MD::WithPosition{10, 0, 19, 0});
    REQUIRE(h->delims().size() == 2);
    REQUIRE(h->delims().front() == MD::WithPosition{0, 0, 1, 0});
    REQUIRE(h->delims().back() == MD::WithPosition{21, 0, 22, 0});
    auto t = static_cast<MD::Text *>(h->text()->items().at(0).get());
    REQUIRE(t->startColumn() == 3);
    REQUIRE(t->startLine() == 0);
    REQUIRE(t->endColumn() == 9);
    REQUIRE(t->endLine() == 0);
    REQUIRE(t->text() == QStringLiteral("headig "));
}

/*
`code` \
-

*/
TEST_CASE("252")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/252.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
    auto h = static_cast<MD::Heading *>(doc->items().at(1).get());
    REQUIRE(h->text()->items().size() == 2);
    REQUIRE(h->text()->items().at(1)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(h->text()->items().at(1).get());
    REQUIRE(t->text() == QStringLiteral(" \\"));
    REQUIRE(t->startColumn() == 6);
    REQUIRE(t->endColumn() == 7);
}

/*
www.google.com

*/
TEST_CASE("231-1")
{
    MD::Parser parser;
    auto inlines = MD::Parser::makeDefaultInlineParsersPipeline();
    inlines.removeIf([](const auto &i) {
        return dynamic_cast<MD::GfmAutolinkParser *>(i.get());
    });
    parser.setInlineParsers(inlines);

    auto doc = parser.parse(QStringLiteral("tests/parser/data/231.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->startColumn() == 0);
    REQUIRE(t->startLine() == 0);
    REQUIRE(t->endColumn() == 13);
    REQUIRE(t->endLine() == 0);
    REQUIRE(t->text() == QStringLiteral("www.google.com"));
}

/*
</

<//

*/
TEST_CASE("255")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/255.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->text() == QStringLiteral("</"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->text() == QStringLiteral("<//"));
    }
}

/*
**Some* text**

*/
TEST_CASE("256")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/256.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->text() == QStringLiteral("Some"));
        REQUIRE(t->opts() == MD::ItalicText);
        REQUIRE(t->openStyles().size() == 2);
        REQUIRE(t->openStyles().at(0) == MD::WithPosition{0, 0, 0, 0});
        REQUIRE(t->openStyles().at(1) == MD::WithPosition{1, 0, 1, 0});
        REQUIRE(t->closeStyles().size() == 1);
        REQUIRE(t->closeStyles().at(0) == MD::WithPosition{6, 0, 6, 0});
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->text() == QStringLiteral(" text"));
        REQUIRE(t->opts() == MD::ItalicText);
        REQUIRE(t->openStyles().size() == 0);
        REQUIRE(t->closeStyles().size() == 1);
        REQUIRE(t->closeStyles().at(0) == MD::WithPosition{12, 0, 12, 0});
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->text() == QStringLiteral("*"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 13);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 13);
        REQUIRE(t->endLine() == 0);
    }
}

/*
**_some_* text*

*/
TEST_CASE("257")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/257.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->text() == QStringLiteral("some"));
        REQUIRE(t->opts() == MD::ItalicText);
        REQUIRE(t->openStyles().size() == 3);
        REQUIRE(t->openStyles().at(0) == MD::WithPosition{0, 0, 0, 0});
        REQUIRE(t->openStyles().at(1) == MD::WithPosition{1, 0, 1, 0});
        REQUIRE(t->openStyles().at(2) == MD::WithPosition{2, 0, 2, 0});
        REQUIRE(t->closeStyles().size() == 2);
        REQUIRE(t->closeStyles().at(0) == MD::WithPosition{7, 0, 7, 0});
        REQUIRE(t->closeStyles().at(1) == MD::WithPosition{8, 0, 8, 0});
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->text() == QStringLiteral(" text"));
        REQUIRE(t->opts() == MD::ItalicText);
        REQUIRE(t->openStyles().size() == 0);
        REQUIRE(t->closeStyles().size() == 1);
        REQUIRE(t->closeStyles().at(0) == MD::WithPosition{14, 0, 14, 0});
    }
}

/*
==

==

*/
TEST_CASE("258")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/258.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    for (int i = 1; i < 3; ++i) {
        REQUIRE(doc->items().at(i)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(i).get());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->text() == QStringLiteral("=="));
    }
}

/*
*$a \ne 0$*
*`code`*
*[google](https://www.google.com)*
*![](https://www.google.com)*
*[^1]*

[^1]: foot


*/
TEST_CASE("259")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/259.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->footnotesMap().size() == 1);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 5);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Math);
    REQUIRE(p->items().at(1)->type() == MD::ItemType::Code);
    REQUIRE(p->items().at(2)->type() == MD::ItemType::Link);
    REQUIRE(p->items().at(3)->type() == MD::ItemType::Image);
    REQUIRE(p->items().at(4)->type() == MD::ItemType::FootnoteRef);

    for (int i = 0; i < 2; ++i)
        REQUIRE(static_cast<MD::ItemWithOpts *>(p->items().at(i).get())->startColumn() == 2);

    for (int i = 2; i < 5; ++i)
        REQUIRE(static_cast<MD::ItemWithOpts *>(p->items().at(i).get())->startColumn() == 1);
}

/*
*[^1]*

*/
TEST_CASE("260")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/260.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->startColumn() == 1);
    REQUIRE(t->endColumn() == 4);
    REQUIRE(t->openStyles().size() == 1);
    REQUIRE(t->openStyles().at(0) == MD::StyleDelim{MD::ItalicText, 0, 0, 0, 0});
    REQUIRE(t->closeStyles().size() == 1);
    REQUIRE(t->closeStyles().at(0) == MD::StyleDelim{MD::ItalicText, 5, 0, 5, 0});
}

void check257WithSemiOpt(QSharedPointer<MD::Document> doc)
{
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->text() == QStringLiteral("some"));
        REQUIRE(t->opts() == MD::ItalicText);
        REQUIRE(t->openStyles().size() == 3);
        REQUIRE(t->openStyles().at(0) == MD::WithPosition{0, 0, 0, 0});
        REQUIRE(t->openStyles().at(1) == MD::WithPosition{1, 0, 1, 0});
        REQUIRE(t->openStyles().at(2) == MD::WithPosition{2, 0, 2, 0});
        REQUIRE(t->closeStyles().size() == 2);
        REQUIRE(t->closeStyles().at(0) == MD::WithPosition{7, 0, 7, 0});
        REQUIRE(t->closeStyles().at(1) == MD::WithPosition{8, 0, 8, 0});
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->text() == QStringLiteral(" text"));
        REQUIRE(t->opts() == MD::ItalicText);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().size() == 1);
        REQUIRE(t->closeStyles().at(0) == MD::WithPosition{14, 0, 14, 0});
    }
}

/*
**_some_* text*

*/
TEST_CASE("257-1")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/257.md"),
                            false,
                            {QStringLiteral("md"), QStringLiteral("markdown")});

    check257WithSemiOpt(doc);
}

/*
**_some_* text*

*/
TEST_CASE("257-2")
{
    const auto fileName = QStringLiteral("tests/parser/data/257.md");

    QFile file(fileName);

    if (file.open(QIODeviceBase::ReadOnly)) {
        QTextStream stream(file.readAll());
        file.close();

        const auto wd = QDir().absolutePath();

        MD::Parser parser;

        auto doc = parser.parse(stream, wd, fileName);

        check257WithSemiOpt(doc);
    } else
        REQUIRE(false);
}

/*
+ Create a list by starting a line with `+`, `-`, or `*`
+ Sub-lists are made by indenting 2 spaces:
  - Marker character change forces new list start:
    * Ac tristique libero volutpat at
    + Facilisis in pretium nisl aliquet
    - Nulla volutpat aliquam velit
+ Very easy!

*/
TEST_CASE("261")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/261.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 3);
    REQUIRE(l->startColumn() == 0);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 11);
    REQUIRE(l->endLine() == 6);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(li->startColumn() == 0);
        REQUIRE(li->startLine() == 0);
        REQUIRE(li->endColumn() == 55);
        REQUIRE(li->endLine() == 0);
        REQUIRE(li->items().size() == 1);
    }

    {
        REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(l->items().at(1).get());
        REQUIRE(li->startColumn() == 0);
        REQUIRE(li->startLine() == 1);
        REQUIRE(li->endColumn() == 33);
        REQUIRE(li->endLine() == 5);
        REQUIRE(li->items().size() == 2);

        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List *>(li->items().at(1).get());
        REQUIRE(l->startColumn() == 2);
        REQUIRE(l->startLine() == 2);
        REQUIRE(l->endColumn() == 33);
        REQUIRE(l->endLine() == 5);

        {
            REQUIRE(l->items().size() == 1);
            REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
            auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
            REQUIRE(li->startColumn() == 2);
            REQUIRE(li->startLine() == 2);
            REQUIRE(li->endColumn() == 33);
            REQUIRE(li->endLine() == 5);
            REQUIRE(li->items().size() == 4);
            REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);

            {
                REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
                auto l = static_cast<MD::List *>(li->items().at(1).get());
                REQUIRE(l->startColumn() == 4);
                REQUIRE(l->startLine() == 3);
                REQUIRE(l->endColumn() == 36);
                REQUIRE(l->endLine() == 3);
            }

            {
                REQUIRE(li->items().at(2)->type() == MD::ItemType::List);
                auto l = static_cast<MD::List *>(li->items().at(2).get());
                REQUIRE(l->startColumn() == 4);
                REQUIRE(l->startLine() == 4);
                REQUIRE(l->endColumn() == 38);
                REQUIRE(l->endLine() == 4);
            }

            {
                REQUIRE(li->items().at(3)->type() == MD::ItemType::List);
                auto l = static_cast<MD::List *>(li->items().at(3).get());
                REQUIRE(l->startColumn() == 4);
                REQUIRE(l->startLine() == 5);
                REQUIRE(l->endColumn() == 33);
                REQUIRE(l->endLine() == 5);
            }
        }
    }

    {
        REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(l->items().at(2).get());
        REQUIRE(li->startColumn() == 0);
        REQUIRE(li->startLine() == 6);
        REQUIRE(li->endColumn() == 11);
        REQUIRE(li->endLine() == 6);
        REQUIRE(li->items().size() == 1);
    }
}

/*
- list item
- list item
<div class="some-class">

Content

</div>
- list item

*/
TEST_CASE("262")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/262.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 5);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endLine() == 1);

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
        REQUIRE(h->startLine() == 2);
        REQUIRE(h->endLine() == 2);
    }

    REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);

    {
        REQUIRE(doc->items().at(4)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(doc->items().at(4).get());
        REQUIRE(h->startLine() == 6);
        REQUIRE(h->endLine() == 7);
    }
}

/*
- list
<div>
  - list

*/
TEST_CASE("263")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/263.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endLine() == 0);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endLine() == 2);
}

/*
- list
  <div>
  - list

*/
TEST_CASE("264")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/264.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endLine() == 2);
    REQUIRE(l->items().size() == 1);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->items().size() == 2);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(li->items().at(0).get());
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endLine() == 0);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(li->items().at(1).get());
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endLine() == 2);
}

/*
- list
<div>

  - list

*/
TEST_CASE("265")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/265.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List *>(doc->items().at(1).get());
        REQUIRE(l->startLine() == 0);
        REQUIRE(l->endLine() == 0);
        REQUIRE(l->items().size() == 1);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endLine() == 1);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List *>(doc->items().at(3).get());
        REQUIRE(l->startLine() == 3);
        REQUIRE(l->endLine() == 3);
        REQUIRE(l->items().size() == 1);
    }
}

/*
- list
  - list
    - list
  <div>
- list

*/
TEST_CASE("266")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/266.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endLine() == 4);
    REQUIRE(l->items().size() == 2);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->startLine() == 0);
    REQUIRE(li->endLine() == 3);
    REQUIRE(li->items().size() == 3);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
    REQUIRE(li->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(li->items().at(2).get());
    REQUIRE(h->startLine() == 3);
    REQUIRE(h->endLine() == 3);
}

/*
- <!--
-->
- list

*/
TEST_CASE("267")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/267.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endLine() == 0);
    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->startLine() == 0);
    REQUIRE(li->endLine() == 0);
    REQUIRE(li->items().size() == 1);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::RawHtml);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);

    REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);
}

/*
- list
  <div>
  ---

*/
TEST_CASE("268")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/268.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endLine() == 2);
    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->startLine() == 0);
    REQUIRE(li->endLine() == 2);
    REQUIRE(li->items().size() == 2);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::RawHtml);
}

/*
- list
  <div>

  ---

*/
TEST_CASE("269")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/269.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endLine() == 3);
    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->startLine() == 0);
    REQUIRE(li->endLine() == 3);
    REQUIRE(li->items().size() == 3);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::RawHtml);
    REQUIRE(li->items().at(2)->type() == MD::ItemType::HorizontalLine);
}

/*
- list
  <div>
---

*/
TEST_CASE("270")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/270.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endLine() == 1);
    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->startLine() == 0);
    REQUIRE(li->endLine() == 1);
    REQUIRE(li->items().size() == 2);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::RawHtml);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::HorizontalLine);
}

/*
- list
  - list

    <div>

    text
  <div>
<div>

*/
TEST_CASE("271")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/271.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endLine() == 6);
    REQUIRE(l->items().size() == 1);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(li->startLine() == 0);
        REQUIRE(li->endLine() == 6);
        REQUIRE(li->items().size() == 3);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        REQUIRE(li->items().at(2)->type() == MD::ItemType::RawHtml);

        {
            auto l = static_cast<MD::List *>(li->items().at(1).get());
            REQUIRE(l->startLine() == 1);
            REQUIRE(l->endLine() == 5);
            REQUIRE(l->items().size() == 1);
            REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
            auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
            REQUIRE(li->items().size() == 3);
            REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
            REQUIRE(li->items().at(1)->type() == MD::ItemType::RawHtml);
            REQUIRE(li->items().at(2)->type() == MD::ItemType::Paragraph);
        }
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
}

/*
- list
  <div>

  - list

*/
TEST_CASE("272")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/272.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endLine() == 3);
    REQUIRE(l->items().size() == 1);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(li->startLine() == 0);
        REQUIRE(li->endLine() == 3);
        REQUIRE(li->items().size() == 3);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(li->items().at(1)->type() == MD::ItemType::RawHtml);
        REQUIRE(li->items().at(2)->type() == MD::ItemType::List);

        {
            auto l = static_cast<MD::List *>(li->items().at(2).get());
            REQUIRE(l->startLine() == 3);
            REQUIRE(l->endLine() == 3);
            REQUIRE(l->items().size() == 1);
            REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
            auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
            REQUIRE(li->items().size() == 1);
            REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        }
    }
}

/*
- list
  <div>

  - list
  <div>

  - list
  <div>

*/
TEST_CASE("273")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/273.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endLine() == 7);
    REQUIRE(l->items().size() == 1);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(li->startLine() == 0);
        REQUIRE(li->endLine() == 7);
        REQUIRE(li->items().size() == 6);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(li->items().at(1)->type() == MD::ItemType::RawHtml);
        REQUIRE(li->items().at(2)->type() == MD::ItemType::List);
        REQUIRE(li->items().at(3)->type() == MD::ItemType::RawHtml);
        REQUIRE(li->items().at(4)->type() == MD::ItemType::List);
        REQUIRE(li->items().at(5)->type() == MD::ItemType::RawHtml);
    }
}

/*
* text

    | Column 1 |
    | -------- |
    | Cell 1 |

*/
TEST_CASE("274")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/274.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(li->items().size() == 2);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(li->items().at(1)->type() == MD::ItemType::Table);
    }
}

/*
- list
<!--
- a
-->

More text

*/
TEST_CASE("275")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/275.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->startColumn() == 0);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 5);
    REQUIRE(l->endLine() == 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endColumn() == 2);
    REQUIRE(h->endLine() == 3);

    REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 5);
    REQUIRE(p->endColumn() == 8);
    REQUIRE(p->endLine() == 5);
}

/*
 - 1
  - 2
   - 3
    - 4
     - 5
      - 6
       - 7
      - 6
     - 5
    - 4
   - 3
  - 2
 - 1

*/
TEST_CASE("276")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/276.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 6);
    REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem *>(l->items().at(2).get());
        REQUIRE(li->items().size() == 2);

        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List *>(li->items().at(1).get());
        REQUIRE(l->items().size() == 5);
        REQUIRE(l->items().at(4)->type() == MD::ItemType::ListItem);

        {
            auto li = static_cast<MD::ListItem *>(l->items().at(4).get());
            REQUIRE(li->items().size() == 1);
            REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(li->items().at(0).get());
            REQUIRE(p->items().size() == 2);
            REQUIRE(p->startColumn() == 7);
            REQUIRE(p->startLine() == 8);
            REQUIRE(p->endColumn() == 6);
            REQUIRE(p->endLine() == 9);
        }
    }
}

/*
[*[*[*[*[foo]*]*]*]*]: bar
[*[*[*[foo]*]*]*]: bar
[*[*[foo]*]*]: bar
[*[foo]*]: bar
[foo]: bar

*/
TEST_CASE("277")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/277.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 9);
    REQUIRE(p->endLine() == 4);

    REQUIRE(doc->labeledLinks().empty());
}

/*
\
\\
\\\
\\\\
\\\\\

*/
TEST_CASE("278")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/278.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 6);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::LineBreak);
    REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
    REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
    REQUIRE(p->items().at(3)->type() == MD::ItemType::LineBreak);
    REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
    REQUIRE(p->items().at(5)->type() == MD::ItemType::Text);
}

/*
**text **text*text

*/
TEST_CASE("279")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/279.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 17);
    REQUIRE(p->endLine() == 0);
    REQUIRE(p->items().size() == 1);
}

/*
~~text~~text~~

*/
TEST_CASE("280")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/280.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 13);
    REQUIRE(p->endLine() == 0);
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 2);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 5);
        REQUIRE(t->endLine() == 0);
        REQUIRE(t->opts() == MD::StrikethroughText);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->startColumn() == 8);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 13);
        REQUIRE(t->endLine() == 0);
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }
}
