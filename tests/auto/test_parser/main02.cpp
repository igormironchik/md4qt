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
#include <QDir>

#include "test_utils.h"

inline QString to_string(int i)
{
    return QString::number(i);
}

/*
[link 0][wrong-label] [link 1](a.md) [![image 1](a.png)](b.md) [link 3][label] [^ref]

[label]: http://www.where.com/a.md (caption)

[^ref] text

[1]: a.md 'title'

[link 4](#label)

*/
TEST_CASE("031")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/031.md"));

    REQUIRE(doc->labeledLinks().size() == 2);
    auto it = doc->labeledLinks().cbegin();

    {
        auto l = static_cast<MD::Link *>(it.value().get());
        CHECK_POSITIONS(l, 0, 6, 16, 6);
    }

    ++it;

    {
        auto l = static_cast<MD::Link *>(it.value().get());
        CHECK_POSITIONS(l, 0, 2, 43, 2);
    }

    const auto wd = QDir().absolutePath() + QStringLiteral("/tests/parser/data");

    auto checkDoc = [&wd](QSharedPointer<MD::Document> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 8);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        CHECK_POSITIONS(p, 0, 0, 84, 0);

        REQUIRE(p->items().size() == 7);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t0 = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t0, 0, 0, 21, 0);

        REQUIRE(t0->text() == QStringLiteral("[link 0][wrong-label] "));
        REQUIRE(t0->opts() == MD::TextWithoutFormat);

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);

        auto l1 = static_cast<MD::Link *>(p->items().at(1).get());
        CHECK_POSITIONS(l1, 22, 0, 35, 0);

        REQUIRE(l1->text() == QStringLiteral("link 1"));
        REQUIRE(l1->textPos() == MD::WithPosition{23, 0, 28, 0});
        REQUIRE(l1->url() == wd + QStringLiteral("/a.md"));
        REQUIRE(l1->urlPos() == MD::WithPosition{31, 0, 34, 0});

        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t1 = static_cast<MD::Text *>(p->items().at(2).get());
        CHECK_POSITIONS(t1, 36, 0, 36, 0);
        REQUIRE(t1->text() == QStringLiteral(" "));

        REQUIRE(p->items().at(3)->type() == MD::ItemType::Link);

        auto l2 = static_cast<MD::Link *>(p->items().at(3).get());
        CHECK_POSITIONS(l2, 37, 0, 61, 0);

        REQUIRE(l2->url() == wd + QStringLiteral("/b.md"));
        REQUIRE(l2->opts() == MD::TextOption::TextWithoutFormat);

        REQUIRE(l2->img().get());
        REQUIRE(l2->img()->text() == QStringLiteral("image 1"));
        REQUIRE(l2->img()->url() == wd + QStringLiteral("/") + QStringLiteral("a.png"));
        REQUIRE(l2->textPos() == MD::WithPosition{38, 0, 54, 0});
        REQUIRE(l2->urlPos() == MD::WithPosition{57, 0, 60, 0});

        REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
        auto t2 = static_cast<MD::Text *>(p->items().at(4).get());
        CHECK_POSITIONS(t2, 62, 0, 62, 0);
        REQUIRE(t2->text() == QStringLiteral(" "));

        REQUIRE(p->items().at(5)->type() == MD::ItemType::Link);

        auto l3 = static_cast<MD::Link *>(p->items().at(5).get());
        CHECK_POSITIONS(l3, 63, 0, 77, 0);

        REQUIRE(l3->text() == QStringLiteral("link 3"));

        const QString label = QStringLiteral("#LABEL/") + wd + QStringLiteral("/031.md");

        REQUIRE(l3->url() == label);
        REQUIRE(l3->textPos() == MD::WithPosition{64, 0, 69, 0});
        REQUIRE(l3->urlPos() == MD::WithPosition{72, 0, 76, 0});

        REQUIRE(p->items().at(6)->type() == MD::ItemType::Text);
        auto t3 = static_cast<MD::Text *>(p->items().at(6).get());
        CHECK_POSITIONS(t3, 78, 0, 84, 0);
        REQUIRE(t3->text() == QStringLiteral(" [^ref]"));

        REQUIRE(!doc->labeledLinks().empty());
        const auto lit = doc->labeledLinks().find(label);
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit.value()->url() == QStringLiteral("http://www.where.com/a.md"));

        {
            REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);

            p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
            CHECK_POSITIONS(p, 0, 4, 10, 4);

            REQUIRE(p->items().size() == 1);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            CHECK_POSITIONS(t, 0, 4, 10, 4);

            REQUIRE(t->text() == QStringLiteral("[^ref] text"));

            REQUIRE(doc->labeledLinks().size() == 2);

            const auto lit = doc->labeledLinks().find(QStringLiteral("#1/") + wd + QStringLiteral("/031.md"));
            REQUIRE(lit != doc->labeledLinks().cend());

            REQUIRE(lit.value()->url() == wd + QStringLiteral("/a.md"));
        }

        {
            REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);

            p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
            CHECK_POSITIONS(p, 0, 8, 15, 8);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);

            auto l = static_cast<MD::Link *>(p->items().at(0).get());
            CHECK_POSITIONS(l, 0, 8, 15, 8);
            REQUIRE(l->textPos() == MD::WithPosition{1, 8, 6, 8});
            REQUIRE(l->urlPos() == MD::WithPosition{9, 8, 14, 8});

            REQUIRE(l->url() == QStringLiteral("#label/") + wd + QStringLiteral("/031.md"));
        }

        REQUIRE(doc->items().at(4)->type() == MD::ItemType::PageBreak);
    };

    checkDoc(doc);

    checkDoc(doc->clone().staticCast<MD::Document>());
}

/*
> ```cpp
> if( a < b )
>   do_something();
> ```

*/
TEST_CASE("032")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/032.md"));

    auto checkDoc = [](QSharedPointer<MD::Document> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);

        auto q = static_cast<MD::Blockquote *>(doc->items().at(1).get());
        CHECK_POSITIONS(q, 0, 0, 4, 3);
        REQUIRE(q->delims() == MD::Blockquote::Delims{{0, 0, 0, 0}, {0, 1, 0, 1}, {0, 2, 0, 2}, {0, 3, 0, 3}});

        REQUIRE(q->items().size() == 1);

        REQUIRE(q->items().at(0)->type() == MD::ItemType::Code);

        auto c = static_cast<MD::Code *>(q->items().at(0).get());
        CHECK_POSITIONS(c, 2, 1, 18, 2);
        REQUIRE(c->startDelim() == MD::WithPosition{2, 0, 4, 0});
        REQUIRE(c->endDelim() == MD::WithPosition{2, 3, 4, 3});
        REQUIRE(c->syntaxPos() == MD::WithPosition{5, 0, 7, 0});

        REQUIRE(c->isInline() == false);
        REQUIRE(c->text() == QStringLiteral("if( a < b )\n  do_something();"));
    };

    checkDoc(doc);

    checkDoc(doc->clone().staticCast<MD::Document>());
}

/*
<http://www.google.com>

*/
TEST_CASE("033")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/033.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 22, 0);

    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);

    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    CHECK_POSITIONS(l, 0, 0, 22, 0);

    REQUIRE(l->url() == QStringLiteral("http://www.google.com"));
    REQUIRE(l->textPos() == MD::WithPosition{1, 0, 21, 0});
    REQUIRE(l->urlPos() == l->textPos());
    REQUIRE(l->text().isEmpty());
}

/*
**[Google](https://www.google.com)**

*/
TEST_CASE("034")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/034.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 35, 0);

    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);

    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    CHECK_POSITIONS(l, 2, 0, 33, 0);
    REQUIRE(l->textPos() == MD::WithPosition{3, 0, 8, 0});
    REQUIRE(l->urlPos() == MD::WithPosition{11, 0, 32, 0});

    REQUIRE(l->url() == QStringLiteral("https://www.google.com"));
    REQUIRE(l->text() == QStringLiteral("Google"));
    REQUIRE(l->opts() == MD::TextOption::BoldText);
    REQUIRE(l->openStyles().size() == 1);
    REQUIRE(l->openStyles().at(0) == MD::StyleDelim{MD::BoldText, 0, 0, 1, 0});
    REQUIRE(l->closeStyles().size() == 1);
    REQUIRE(l->closeStyles().at(0) == MD::StyleDelim{MD::BoldText, 34, 0, 35, 0});
}

/*
1. 1
2. 2
   1. 1
   2. 2
3. 3

*/
TEST_CASE("035")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/035.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 5);

    REQUIRE(l->items().size() == 3);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

        auto i1 = static_cast<MD::ListItem *>(l->items().at(0).get());
        CHECK_POSITIONS(i1, 0, 0, 3, 0);
        REQUIRE(i1->delim() == MD::WithPosition{0, 0, 1, 0});

        REQUIRE(i1->listType() == MD::ListItem::Ordered);
        REQUIRE(i1->orderedListPreState() == MD::ListItem::Start);
        REQUIRE(i1->items().size() == 1);
        REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(i1->items().at(0).get());
        CHECK_POSITIONS(p, 3, 0, 3, 0);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(static_cast<MD::Text *>(p->items().at(0).get())->text() == QStringLiteral("1"));
    }

    REQUIRE(l->items().size() == 3);

    {
        REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);

        auto i1 = static_cast<MD::ListItem *>(l->items().at(1).get());
        CHECK_POSITIONS(i1, 0, 1, 6, 3);
        REQUIRE(i1->delim() == MD::WithPosition{0, 1, 1, 1});

        REQUIRE(i1->listType() == MD::ListItem::Ordered);
        REQUIRE(i1->orderedListPreState() == MD::ListItem::Continue);
        REQUIRE(i1->items().size() == 2);
        REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(i1->items().at(0).get());
        CHECK_POSITIONS(p, 3, 1, 3, 1);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(static_cast<MD::Text *>(p->items().at(0).get())->text() == QStringLiteral("2"));

        REQUIRE(i1->items().at(1)->type() == MD::ItemType::List);

        auto nl = static_cast<MD::List *>(i1->items().at(1).get());
        CHECK_POSITIONS(nl, 3, 2, 6, 3);

        REQUIRE(nl->items().size() == 2);

        {
            REQUIRE(nl->items().at(0)->type() == MD::ItemType::ListItem);

            auto i1 = static_cast<MD::ListItem *>(nl->items().at(0).get());
            CHECK_POSITIONS(i1, 3, 2, 6, 2);
            REQUIRE(i1->delim() == MD::WithPosition{3, 2, 4, 2});

            REQUIRE(i1->listType() == MD::ListItem::Ordered);
            REQUIRE(i1->orderedListPreState() == MD::ListItem::Start);
            REQUIRE(i1->items().size() == 1);
            REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(i1->items().at(0).get());
            CHECK_POSITIONS(p, 6, 2, 6, 2);

            REQUIRE(p->items().size() == 1);
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            REQUIRE(static_cast<MD::Text *>(p->items().at(0).get())->text() == QStringLiteral("1"));
        }

        {
            REQUIRE(nl->items().at(1)->type() == MD::ItemType::ListItem);

            auto i1 = static_cast<MD::ListItem *>(nl->items().at(1).get());
            CHECK_POSITIONS(i1, 3, 3, 6, 3);
            REQUIRE(i1->delim() == MD::WithPosition{3, 3, 4, 3});

            REQUIRE(i1->listType() == MD::ListItem::Ordered);
            REQUIRE(i1->orderedListPreState() == MD::ListItem::Continue);
            REQUIRE(i1->items().size() == 1);
            REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(i1->items().at(0).get());
            CHECK_POSITIONS(p, 6, 3, 6, 3);

            REQUIRE(p->items().size() == 1);
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            REQUIRE(static_cast<MD::Text *>(p->items().at(0).get())->text() == QStringLiteral("2"));
        }
    }

    {
        REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

        auto i1 = static_cast<MD::ListItem *>(l->items().at(2).get());
        CHECK_POSITIONS(i1, 0, 4, 0, 5);
        REQUIRE(i1->delim() == MD::WithPosition{0, 4, 1, 4});

        REQUIRE(i1->listType() == MD::ListItem::Ordered);
        REQUIRE(i1->orderedListPreState() == MD::ListItem::Continue);
        REQUIRE(i1->items().size() == 1);
        REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(i1->items().at(0).get());
        CHECK_POSITIONS(p, 3, 4, 3, 4);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(static_cast<MD::Text *>(p->items().at(0).get())->text() == QStringLiteral("3"));
    }
}

/*
[Google]( www.google.com "Google Shmoogle..." )

*/
TEST_CASE("036")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/036.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 46, 0);

    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);

    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    CHECK_POSITIONS(l, 0, 0, 46, 0);
    REQUIRE(l->textPos() == MD::WithPosition{1, 0, 6, 0});
    REQUIRE(l->urlPos() == MD::WithPosition{10, 0, 23, 0});

    REQUIRE(l->url() == QStringLiteral("www.google.com"));
    REQUIRE(l->text() == QStringLiteral("Google"));
}

/*
[Google] ( www.google.com Google Shmoogle...

[Google] (

[Google

[Google]

[![Google](

![Google](

[Google] ( www.google.com "Google Shmoogle..."

[![Google](https://www.google.com/logo.png)

text [^ref]:

[^ref

[

[1]:

[1]:

[text][link

[text]#

<www.google.com

*/
TEST_CASE("037")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/037.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 17);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        CHECK_POSITIONS(p, 0, 0, 43, 0);

        REQUIRE(p->items().size() == 3);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            CHECK_POSITIONS(t, 0, 0, 10, 0);

            REQUIRE(t->text() == QStringLiteral("[Google] ( "));
        }

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(1).get());
        CHECK_POSITIONS(l, 11, 0, 24, 0);
        REQUIRE(l->textPos() == MD::WithPosition{11, 0, 24, 0});
        REQUIRE(l->urlPos() == l->textPos());
        REQUIRE(l->url() == QStringLiteral("http://www.google.com"));

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(2).get());
            CHECK_POSITIONS(t, 25, 0, 43, 0);

            REQUIRE(t->text() == QStringLiteral(" Google Shmoogle..."));
        }
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        CHECK_POSITIONS(p, 0, 2, 9, 2);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 2, 9, 2);

        REQUIRE(t->text() == QStringLiteral("[Google] ("));
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        CHECK_POSITIONS(p, 0, 4, 6, 4);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 4, 6, 4);

        REQUIRE(t->text() == QStringLiteral("[Google"));
    }

    {
        REQUIRE(doc->items().at(4)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(4).get());
        CHECK_POSITIONS(p, 0, 6, 7, 6);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->text() == QStringLiteral("[Google]"));
        CHECK_POSITIONS(t, 0, 6, 7, 6);
    }

    {
        REQUIRE(doc->items().at(5)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(5).get());
        CHECK_POSITIONS(p, 0, 8, 10, 8);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->text() == QStringLiteral("[![Google]("));
        CHECK_POSITIONS(t, 0, 8, 10, 8);
    }

    {
        REQUIRE(doc->items().at(6)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(6).get());
        CHECK_POSITIONS(p, 0, 10, 9, 10);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->text() == QStringLiteral("![Google]("));
        CHECK_POSITIONS(t, 0, 10, 9, 10);
    }

    {
        REQUIRE(doc->items().at(7)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(7).get());
        CHECK_POSITIONS(p, 0, 12, 45, 12);

        REQUIRE(p->items().size() == 3);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            CHECK_POSITIONS(t, 0, 12, 10, 12);

            REQUIRE(t->text() == QStringLiteral("[Google] ( "));
        }

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(1).get());
        CHECK_POSITIONS(l, 11, 12, 24, 12);
        REQUIRE(l->url() == QStringLiteral("http://www.google.com"));
        REQUIRE(l->textPos() == MD::WithPosition{11, 12, 24, 12});
        REQUIRE(l->urlPos() == l->textPos());

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(2).get());
            CHECK_POSITIONS(t, 25, 12, 45, 12);

            REQUIRE(t->text() == QStringLiteral(" \"Google Shmoogle...\""));
        }
    }

    {
        REQUIRE(doc->items().at(8)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(8).get());
        CHECK_POSITIONS(p, 0, 14, 42, 14);

        REQUIRE(p->items().size() == 2);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 14, 0, 14);

        REQUIRE(t->text() == QStringLiteral("["));

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Image);

        auto i = static_cast<MD::Image *>(p->items().at(1).get());
        CHECK_POSITIONS(i, 1, 14, 42, 14);
        REQUIRE(i->textPos() == MD::WithPosition{3, 14, 8, 14});
        REQUIRE(i->urlPos() == MD::WithPosition{11, 14, 41, 14});

        REQUIRE(i->url() == QStringLiteral("https://www.google.com/logo.png"));
        REQUIRE(i->text() == QStringLiteral("Google"));
    }

    {
        REQUIRE(doc->items().at(9)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(9).get());
        CHECK_POSITIONS(p, 0, 16, 11, 16);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 16, 11, 16);
        REQUIRE(t->text() == QStringLiteral("text [^ref]:"));
    }

    {
        REQUIRE(doc->items().at(10)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(10).get());
        CHECK_POSITIONS(p, 0, 18, 4, 18);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 18, 4, 18);

        REQUIRE(t->text() == QStringLiteral("[^ref"));
    }

    {
        REQUIRE(doc->items().at(11)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(11).get());
        CHECK_POSITIONS(p, 0, 20, 1, 20);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 20, 1, 20);

        REQUIRE(t->text() == QStringLiteral("["));
    }

    {
        REQUIRE(doc->items().at(12)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(12).get());
        CHECK_POSITIONS(p, 0, 22, 3, 22);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 22, 3, 22);

        REQUIRE(t->text() == QStringLiteral("[1]:"));
    }

    {
        REQUIRE(doc->items().at(13)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(13).get());
        CHECK_POSITIONS(p, 0, 24, 4, 24);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 24, 4, 24);

        REQUIRE(t->text() == QStringLiteral("[1]:"));
    }

    {
        REQUIRE(doc->items().at(14)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(14).get());
        CHECK_POSITIONS(p, 0, 26, 10, 26);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 26, 10, 26);

        REQUIRE(t->text() == QStringLiteral("[text][link"));
    }

    {
        REQUIRE(doc->items().at(15)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(15).get());
        CHECK_POSITIONS(p, 0, 28, 6, 28);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 28, 6, 28);

        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("[text]#"));
    }

    {
        REQUIRE(doc->items().at(16)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(16).get());
        CHECK_POSITIONS(p, 0, 30, 14, 30);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 30, 14, 30);

        REQUIRE(t->text() == QStringLiteral("<www.google.com"));
    }
}
/*
1. 1
2. 2
    1. 1
    2. 2
3. 3

*/
TEST_CASE("038")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/038.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 5);

    REQUIRE(l->items().size() == 3);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

        auto i1 = static_cast<MD::ListItem *>(l->items().at(0).get());
        CHECK_POSITIONS(i1, 0, 0, 3, 0);
        REQUIRE(i1->delim() == MD::WithPosition{0, 0, 1, 0});

        REQUIRE(i1->listType() == MD::ListItem::Ordered);
        REQUIRE(i1->orderedListPreState() == MD::ListItem::Start);
        REQUIRE(i1->items().size() == 1);
        REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(i1->items().at(0).get());
        CHECK_POSITIONS(p, 3, 0, 3, 0);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(static_cast<MD::Text *>(p->items().at(0).get())->text() == QStringLiteral("1"));
    }

    REQUIRE(l->items().size() == 3);

    {
        REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);

        auto i1 = static_cast<MD::ListItem *>(l->items().at(1).get());
        CHECK_POSITIONS(i1, 0, 1, 4, 3);
        REQUIRE(i1->delim() == MD::WithPosition{0, 1, 1, 1});

        REQUIRE(i1->listType() == MD::ListItem::Ordered);
        REQUIRE(i1->orderedListPreState() == MD::ListItem::Continue);
        REQUIRE(i1->items().size() == 2);
        REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(i1->items().at(0).get());
        CHECK_POSITIONS(p, 3, 1, 3, 1);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(static_cast<MD::Text *>(p->items().at(0).get())->text() == QStringLiteral("2"));

        REQUIRE(i1->items().at(1)->type() == MD::ItemType::List);

        auto nl = static_cast<MD::List *>(i1->items().at(1).get());
        CHECK_POSITIONS(nl, 1, 2, 4, 3);

        REQUIRE(nl->items().size() == 2);

        {
            REQUIRE(nl->items().at(0)->type() == MD::ItemType::ListItem);

            auto i1 = static_cast<MD::ListItem *>(nl->items().at(0).get());
            CHECK_POSITIONS(i1, 1, 2, 4, 2);
            REQUIRE(i1->delim() == MD::WithPosition{1, 2, 2, 2});

            REQUIRE(i1->listType() == MD::ListItem::Ordered);
            REQUIRE(i1->orderedListPreState() == MD::ListItem::Start);
            REQUIRE(i1->items().size() == 1);
            REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(i1->items().at(0).get());
            CHECK_POSITIONS(p, 4, 2, 4, 2);

            REQUIRE(p->items().size() == 1);
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            REQUIRE(static_cast<MD::Text *>(p->items().at(0).get())->text() == QStringLiteral("1"));
        }

        {
            REQUIRE(nl->items().at(1)->type() == MD::ItemType::ListItem);

            auto i1 = static_cast<MD::ListItem *>(nl->items().at(1).get());
            CHECK_POSITIONS(i1, 1, 3, 4, 3);
            REQUIRE(i1->delim() == MD::WithPosition{1, 3, 2, 3});

            REQUIRE(i1->listType() == MD::ListItem::Ordered);
            REQUIRE(i1->orderedListPreState() == MD::ListItem::Continue);
            REQUIRE(i1->items().size() == 1);
            REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(i1->items().at(0).get());
            CHECK_POSITIONS(p, 4, 3, 4, 3);

            REQUIRE(p->items().size() == 1);
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            REQUIRE(static_cast<MD::Text *>(p->items().at(0).get())->text() == QStringLiteral("2"));
        }
    }

    {
        REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

        auto i1 = static_cast<MD::ListItem *>(l->items().at(2).get());
        CHECK_POSITIONS(i1, 0, 4, 0, 5);
        REQUIRE(i1->delim() == MD::WithPosition{0, 4, 1, 4});

        REQUIRE(i1->listType() == MD::ListItem::Ordered);
        REQUIRE(i1->orderedListPreState() == MD::ListItem::Continue);
        REQUIRE(i1->items().size() == 1);
        REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(i1->items().at(0).get());
        CHECK_POSITIONS(p, 3, 4, 3, 4);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(static_cast<MD::Text *>(p->items().at(0).get())->text() == QStringLiteral("3"));
    }
}

/*
****text****

*/
TEST_CASE("039")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/039.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 11, 0);

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    CHECK_POSITIONS(t, 4, 0, 7, 0);

    REQUIRE(t->opts() == MD::TextOption::BoldText);
    REQUIRE(t->text() == QStringLiteral("text"));
    REQUIRE(t->openStyles().size() == 2);
    REQUIRE(t->openStyles().at(0) == MD::StyleDelim{MD::BoldText, 0, 0, 1, 0});
    REQUIRE(t->openStyles().at(1) == MD::StyleDelim{MD::BoldText, 2, 0, 3, 0});
    REQUIRE(t->closeStyles().size() == 2);
    REQUIRE(t->closeStyles().at(0) == MD::StyleDelim{MD::BoldText, 8, 0, 9, 0});
    REQUIRE(t->closeStyles().at(1) == MD::StyleDelim{MD::BoldText, 10, 0, 11, 0});
}

/*
``Use this `code`
in the code

*/
TEST_CASE("040")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/040.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(dp, 0, 0, 10, 1);

    REQUIRE(dp->items().size() == 3);

    REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);

    auto t1 = static_cast<MD::Text *>(dp->items().at(0).get());
    REQUIRE(t1->text() == QStringLiteral("``Use this "));
    CHECK_POSITIONS(t1, 0, 0, 10, 0);

    REQUIRE(dp->items().at(1)->type() == MD::ItemType::Code);

    auto c1 = static_cast<MD::Code *>(dp->items().at(1).get());
    CHECK_POSITIONS(c1, 12, 0, 15, 0);
    REQUIRE(c1->startDelim() == MD::WithPosition{11, 0, 11, 0});
    REQUIRE(c1->endDelim() == MD::WithPosition{16, 0, 16, 0});

    REQUIRE(c1->text() == QStringLiteral("code"));

    REQUIRE(dp->items().at(2)->type() == MD::ItemType::Text);

    auto t2 = static_cast<MD::Text *>(dp->items().at(2).get());
    CHECK_POSITIONS(t2, 0, 1, 10, 1);

    REQUIRE(t2->text() == QStringLiteral("in the code"));
}

/*
* Item 1

    Paragraph in list

* Item 2

    Paragraph in list

* Item 3

    Paragraph in list

*/
TEST_CASE("041")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/041.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 11);

    REQUIRE(l->items().size() == 3);

    for (int i = 0; i < 3; ++i) {
        REQUIRE(l->items().at(i)->type() == MD::ItemType::ListItem);

        auto item = static_cast<MD::ListItem *>(l->items().at(i).get());
        CHECK_POSITIONS(item, 0, 4 * i, (i < 2 ? 3 : 0), 3 + 4 * i);
        REQUIRE(item->delim() == MD::WithPosition{0, i * 4, 0, i * 4});

        REQUIRE(item->listType() == MD::ListItem::Unordered);

        REQUIRE(item->items().size() == 2);

        {
            REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
            CHECK_POSITIONS(p, 2, 4 * i, 7, 4 * i);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            CHECK_POSITIONS(t, 2, 4 * i, 7, 4 * i);

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Item ") + to_string(i + 1));
        }

        {
            REQUIRE(item->items().at(1)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(item->items().at(1).get());
            CHECK_POSITIONS(p, 1, 2 + 4 * i, 17, 2 + 4 * i);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            CHECK_POSITIONS(t, 1, 2 + 4 * i, 17, 2 + 4 * i);

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Paragraph in list"));
        }
    }
}

/*
* [Chapter 1](042-1.md)
* [Chapter 1](042-1.md)

*/
TEST_CASE("042")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/042.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 5);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 2);

    REQUIRE(l->items().size() == 2);

    const QString wd = QDir().absolutePath() + QStringLiteral("/tests/parser/data");

    for (int i = 0; i < 2; ++i) {
        REQUIRE(l->items().at(i)->type() == MD::ItemType::ListItem);

        auto li = static_cast<MD::ListItem *>(l->items().at(i).get());
        CHECK_POSITIONS(li, 0, i, (i < 1 ? 22 : 0), (i < 1 ? i : i + 1));
        REQUIRE(li->delim() == MD::WithPosition{0, i, 0, i});

        REQUIRE(li->items().size() == 1);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(li->items().at(0).get());
        CHECK_POSITIONS(p, 2, i, 22, i);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);

        auto lnk = static_cast<MD::Link *>(p->items().at(0).get());
        CHECK_POSITIONS(lnk, 2, i, 22, i);
        REQUIRE(lnk->textPos() == MD::WithPosition{3, i, 11, i});
        REQUIRE(lnk->urlPos() == MD::WithPosition{14, i, 21, i});

        REQUIRE(lnk->text() == QStringLiteral("Chapter 1"));
        REQUIRE(lnk->url() == wd + QStringLiteral("/042-1.md"));
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::PageBreak);

    REQUIRE(doc->items().at(3)->type() == MD::ItemType::Anchor);

    REQUIRE(static_cast<MD::Anchor *>(doc->items().at(3).get())->label() == wd + QStringLiteral("/042-1.md"));

    REQUIRE(doc->items().at(4)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(4).get());
    CHECK_POSITIONS(p, 0, 0, 10, 0);

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    CHECK_POSITIONS(t, 0, 0, 10, 0);

    REQUIRE(t->text() == QStringLiteral("Paragraph 1"));
}

TEST_CASE("042-1")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/042.md"), false);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 2);

    REQUIRE(l->items().size() == 2);

    const QString wd = QDir().absolutePath() + QStringLiteral("/tests/parser/data");

    for (int i = 0; i < 2; ++i) {
        REQUIRE(l->items().at(i)->type() == MD::ItemType::ListItem);

        auto li = static_cast<MD::ListItem *>(l->items().at(i).get());
        CHECK_POSITIONS(li, 0, i, (i < 1 ? 22 : 0), (i < 1 ? i : i + 1));

        REQUIRE(li->items().size() == 1);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(li->items().at(0).get());
        CHECK_POSITIONS(p, 2, i, 22, i);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);

        auto lnk = static_cast<MD::Link *>(p->items().at(0).get());
        CHECK_POSITIONS(lnk, 2, i, 22, i);

        REQUIRE(lnk->text() == QStringLiteral("Chapter 1"));
        REQUIRE(lnk->url() == wd + QStringLiteral("/042-1.md"));
    }
}

/*
* Item

    > Quote


*/
TEST_CASE("044")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/044.md"), false);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 4);

    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    CHECK_POSITIONS(li, 0, 0, 0, 4);
    REQUIRE(li->delim() == MD::WithPosition{0, 0, 0, 0});

    REQUIRE(li->items().size() == 2);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(li->items().at(0).get());
    CHECK_POSITIONS(p, 2, 0, 5, 0);

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    CHECK_POSITIONS(t, 2, 0, 5, 0);

    REQUIRE(t->text() == QStringLiteral("Item"));

    REQUIRE(li->items().at(1)->type() == MD::ItemType::Blockquote);

    auto bq = static_cast<MD::Blockquote *>(li->items().at(1).get());
    CHECK_POSITIONS(bq, 4, 2, 10, 2);
    REQUIRE(bq->delims() == MD::Blockquote::Delims{{4, 2, 4, 2}});

    REQUIRE(bq->items().size() == 1);

    REQUIRE(bq->items().at(0)->type() == MD::ItemType::Paragraph);

    p = static_cast<MD::Paragraph *>(bq->items().at(0).get());
    CHECK_POSITIONS(p, 6, 2, 10, 2);

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

    t = static_cast<MD::Text *>(p->items().at(0).get());
    CHECK_POSITIONS(t, 6, 2, 10, 2);

    REQUIRE(t->text() == QStringLiteral("Quote"));
}

/*
  [^footnote]: Paragraph in footnote

    Paragraph in footnote

    Paragraph in footnote

*/
TEST_CASE("045")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/045.md"), false);

    REQUIRE(!doc->isEmpty());
    REQUIRE(doc->items().size() == 1);
    REQUIRE(doc->items().at(0)->type() == MD::ItemType::Anchor);

    REQUIRE(doc->footnotesMap().size() == 1);

    const QString wd = QDir().absolutePath() + QStringLiteral("/tests/parser/data/");

    const QString label = QStringLiteral("#^FOOTNOTE/") + wd + QStringLiteral("045.md");

    const auto fit = doc->footnotesMap().find(label);

    REQUIRE(fit != doc->footnotesMap().cend());

    auto f = fit.value();

    REQUIRE(f->items().size() == 3);
    CHECK_POSITIONS(f.get(), 0, 0, 21, 4);
    REQUIRE(f->idPos() == MD::WithPosition{1, 0, 12, 0});

    static const std::vector<long long int> start = {14, 4, 1};

    for (int i = 0; i < 3; ++i) {
        REQUIRE(f->items().at(i)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(f->items().at(i).get());
        CHECK_POSITIONS(p, start.at(i), 2 * i, start.at(i) + 20, 2 * i);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, start.at(i), 2 * i, start.at(i) + 20, 2 * i);

        REQUIRE(t->text() == QStringLiteral("Paragraph in footnote"));
    }
}

/*
Heading 1
=========
Paragraph 1

Heading 2
---------
Paragraph 2

  # Heading 1

Paragraph 1

## Heading 2

Paragraph 2

Heading 1
=========

Paragraph 1

Heading 2
---------

Paragraph 2

### Heading 3 {#heading-3}

*/
TEST_CASE("046")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/046.md"));

    REQUIRE(!doc->isEmpty());

    const QString wd = QDir().absolutePath() + QStringLiteral("/tests/parser/data/");

    auto checkDoc = [&wd](QSharedPointer<MD::Document> doc) {
        REQUIRE(doc->items().size() == 14);

        static const std::vector<long long int> hcolumn = {0, 0, 2, 0, 0, 0};
        static const std::vector<long long int> hecolumn = {8, 8, 12, 11, 8, 8};
        static const std::vector<long long int> sline = {0, 4, 8, 12, 16, 21};
        static const std::vector<long long int> eline = {1, 5, 8, 12, 17, 22};
        static const std::vector<long long int> pcolumn = {0, 0, 4, 3, 0, 0};
        static const std::vector<long long int> pline = {2, 6, 10, 14, 19, 24};
        static const std::vector<MD::WithPosition> delims =
            {{0, 1, 8, 1}, {0, 5, 8, 5}, {2, 8, 2, 8}, {0, 12, 1, 12}, {0, 17, 8, 17}, {0, 22, 8, 22}};

        int didx = 1;

        for (int i = 1; i < 4; ++i) {
            for (int j = 1; j < 3; ++j) {
                REQUIRE(doc->items().at(didx)->type() == MD::ItemType::Heading);

                auto h = static_cast<MD::Heading *>(doc->items().at(didx).get());
                const size_t idx = ((i - 1) * 2) + j - 1;
                CHECK_POSITIONS(h, hcolumn.at(idx), sline.at(idx), hecolumn.at(idx), eline.at(idx));
                REQUIRE(h->delims().size() == 1);
                REQUIRE(h->delims().front() == delims.at(idx));

                {
                    REQUIRE(h->level() == j);
                    REQUIRE(h->text().get());
                    auto p = h->text().get();
                    CHECK_POSITIONS(p, pcolumn.at(idx), sline.at(idx), pcolumn.at(idx) + 8, p->startLine());
                    REQUIRE(p->items().size() == 1);
                    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
                    auto t = static_cast<MD::Text *>(p->items().at(0).get());
                    REQUIRE(t->text() == QStringLiteral("Heading ") + to_string(j));
                    CHECK_POSITIONS(t, pcolumn.at(idx), sline.at(idx), pcolumn.at(idx) + 8, t->startLine());
                }

                ++didx;

                REQUIRE(doc->items().at(didx)->type() == MD::ItemType::Paragraph);

                auto p = static_cast<MD::Paragraph *>(doc->items().at(didx).get());
                CHECK_POSITIONS(p, 0, pline.at(idx), 10, p->startLine());

                REQUIRE(p->items().size() == 1);
                REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
                REQUIRE(static_cast<MD::Text *>(p->items().at(0).get())->text()
                        == QStringLiteral("Paragraph ") + to_string(j));

                ++didx;
            }
        }

        REQUIRE(doc->items().at(didx)->type() == MD::ItemType::Heading);

        auto h = static_cast<MD::Heading *>(doc->items().at(didx).get());
        CHECK_POSITIONS(h, 0, 26, 25, 26);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 26, 2, 26});

        REQUIRE(h->level() == 3);
        REQUIRE(h->text().get());
        auto p = h->text().get();
        CHECK_POSITIONS(p, 4, 26, 25, 26);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 4, 26, 13, 26);
        REQUIRE(t->text() == QStringLiteral("Heading 3 "));
        REQUIRE(h->isLabeled());

        const QString label = QStringLiteral("#heading-3/") + wd + QStringLiteral("046.md");

        REQUIRE(h->label() == label);

        REQUIRE(doc->labeledHeadings().size() == 13);
        const auto hit = doc->labeledHeadings().find(label);
        REQUIRE(hit != doc->labeledHeadings().cend());
        REQUIRE(hit.value().get() == h);
    };

    checkDoc(doc);

    checkDoc(doc->clone().staticCast<MD::Document>());
}

/*

Column 1 | Column 2
---------|---------
Cell 1   | Cell 2

| Column 1 | Column 2 |
|:--------:|---------:|
| Cell 1   | Cell 2   |

*/
TEST_CASE("047")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/047.md"));

    auto checkDoc = [](QSharedPointer<MD::Document> doc) {
        REQUIRE(!doc->isEmpty());
        REQUIRE(doc->items().size() == 3);

        {
            REQUIRE(doc->items().at(1)->type() == MD::ItemType::Table);

            auto t = static_cast<MD::Table *>(doc->items().at(1).get());
            CHECK_POSITIONS(t, 0, 1, 16, 3);

            REQUIRE(t->columnsCount() == 2);
            REQUIRE(t->rows().size() == 2);

            auto r0 = t->rows().at(0);
            CHECK_POSITIONS(r0.get(), 0, 1, 18, r0->startLine());

            REQUIRE(r0->type() == MD::ItemType::TableRow);

            REQUIRE(r0->cells().size() == 2);

            {
                REQUIRE(r0->cells().at(0)->type() == MD::ItemType::TableCell);
                auto c0 = static_cast<MD::TableCell *>(r0->cells().at(0).get());
                CHECK_POSITIONS(c0, 0, r0->startLine(), 7, c0->startLine());

                REQUIRE(c0->items().size() == 1);
                REQUIRE(c0->items().at(0)->type() == MD::ItemType::Text);

                auto t0 = static_cast<MD::Text *>(c0->items().at(0).get());
                CHECK_POSITIONS(t0, 0, c0->startLine(), 7, t0->startLine());

                REQUIRE(t0->text() == QStringLiteral("Column 1"));
            }

            {
                auto c1 = static_cast<MD::TableCell *>(r0->cells().at(1).get());
                CHECK_POSITIONS(c1, 11, r0->startLine(), 18, c1->startLine());

                REQUIRE(c1->items().size() == 1);
                REQUIRE(c1->items().at(0)->type() == MD::ItemType::Text);

                auto t1 = static_cast<MD::Text *>(c1->items().at(0).get());
                CHECK_POSITIONS(t1, 11, r0->startLine(), 18, c1->startLine());

                REQUIRE(t1->text() == QStringLiteral("Column 2"));
            }

            auto r1 = t->rows().at(1);
            CHECK_POSITIONS(r1.get(), 0, 3, 16, r1->startLine());

            REQUIRE(r1->cells().size() == 2);

            {
                auto c0 = static_cast<MD::TableCell *>(r1->cells().at(0).get());
                CHECK_POSITIONS(c0, 0, r1->startLine(), 5, c0->startLine());

                REQUIRE(c0->items().size() == 1);
                REQUIRE(c0->items().at(0)->type() == MD::ItemType::Text);

                auto t0 = static_cast<MD::Text *>(c0->items().at(0).get());
                CHECK_POSITIONS(t0, 0, r1->startLine(), 5, t0->startLine());

                REQUIRE(t0->text() == QStringLiteral("Cell 1"));
            }

            {
                auto c1 = static_cast<MD::TableCell *>(r1->cells().at(1).get());
                CHECK_POSITIONS(c1, 11, r1->startLine(), 16, c1->startLine());

                REQUIRE(c1->items().size() == 1);
                REQUIRE(c1->items().at(0)->type() == MD::ItemType::Text);

                auto t1 = static_cast<MD::Text *>(c1->items().at(0).get());
                CHECK_POSITIONS(t1, 11, r1->startLine(), 16, t1->startLine());

                REQUIRE(t1->text() == QStringLiteral("Cell 2"));
            }
        }

        {
            REQUIRE(doc->items().at(2)->type() == MD::ItemType::Table);

            auto t = static_cast<MD::Table *>(doc->items().at(2).get());
            CHECK_POSITIONS(t, 0, 5, 22, 7);

            REQUIRE(t->columnsCount() == 2);
            REQUIRE(t->rows().size() == 2);

            auto r0 = t->rows().at(0);
            CHECK_POSITIONS(r0.get(), 0, 5, 22, r0->startLine());

            REQUIRE(r0->type() == MD::ItemType::TableRow);

            REQUIRE(r0->cells().size() == 2);

            {
                REQUIRE(r0->cells().at(0)->type() == MD::ItemType::TableCell);
                auto c0 = static_cast<MD::TableCell *>(r0->cells().at(0).get());
                CHECK_POSITIONS(c0, 2, r0->startLine(), 9, c0->startLine());

                REQUIRE(c0->items().size() == 1);
                REQUIRE(c0->items().at(0)->type() == MD::ItemType::Text);

                auto t0 = static_cast<MD::Text *>(c0->items().at(0).get());
                CHECK_POSITIONS(t0, 2, c0->startLine(), 9, t0->startLine());

                REQUIRE(t0->text() == QStringLiteral("Column 1"));
            }

            {
                auto c1 = static_cast<MD::TableCell *>(r0->cells().at(1).get());
                CHECK_POSITIONS(c1, 13, r0->startLine(), 20, c1->startLine());

                REQUIRE(c1->items().size() == 1);
                REQUIRE(c1->items().at(0)->type() == MD::ItemType::Text);

                auto t1 = static_cast<MD::Text *>(c1->items().at(0).get());
                CHECK_POSITIONS(t1, 13, r0->startLine(), 20, c1->startLine());

                REQUIRE(t1->text() == QStringLiteral("Column 2"));
            }

            auto r1 = t->rows().at(1);
            CHECK_POSITIONS(r1.get(), 0, 7, 22, r1->startLine());

            REQUIRE(r1->cells().size() == 2);

            {
                auto c0 = static_cast<MD::TableCell *>(r1->cells().at(0).get());
                CHECK_POSITIONS(c0, 2, r1->startLine(), 7, c0->startLine());

                REQUIRE(c0->items().size() == 1);
                REQUIRE(c0->items().at(0)->type() == MD::ItemType::Text);

                auto t0 = static_cast<MD::Text *>(c0->items().at(0).get());
                CHECK_POSITIONS(t0, 2, r1->startLine(), 7, t0->startLine());

                REQUIRE(t0->text() == QStringLiteral("Cell 1"));
            }

            {
                auto c1 = static_cast<MD::TableCell *>(r1->cells().at(1).get());
                CHECK_POSITIONS(c1, 13, r1->startLine(), 18, c1->startLine());

                REQUIRE(c1->items().size() == 1);
                REQUIRE(c1->items().at(0)->type() == MD::ItemType::Text);

                auto t1 = static_cast<MD::Text *>(c1->items().at(0).get());
                CHECK_POSITIONS(t1, 13, r1->startLine(), 18, t1->startLine());

                REQUIRE(t1->text() == QStringLiteral("Cell 2"));
            }
        }

        auto table = static_cast<MD::Table *>(doc->items().at(1).get());

        REQUIRE(table->columnAlignment(0) == MD::Table::AlignLeft);
        REQUIRE(table->columnAlignment(1) == MD::Table::AlignLeft);

        table = static_cast<MD::Table *>(doc->items().at(2).get());

        REQUIRE(table->columnAlignment(0) == MD::Table::AlignCenter);
        REQUIRE(table->columnAlignment(1) == MD::Table::AlignRight);
    };

    checkDoc(doc);

    checkDoc(doc->clone().staticCast<MD::Document>());
}

/*
# Heading
Paragraph
# Heading

*/
TEST_CASE("048")
{
    MD::Parser parser;
    auto doc = parser.parse(QStringLiteral("tests/parser/data/048.md"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(0)->type() == MD::ItemType::Anchor);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(1).get());
        CHECK_POSITIONS(h, 0, 0, 8, 0);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 0, 0, 0});

        REQUIRE(h->text().get());
        auto p = h->text().get();
        CHECK_POSITIONS(p, 2, 0, 8, 0);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->text() == QStringLiteral("Heading"));
        CHECK_POSITIONS(t, 2, 0, 8, 0);
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto *p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        CHECK_POSITIONS(p, 0, 1, 8, 1);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(static_cast<MD::Text *>(p->items().at(0).get())->text() == QStringLiteral("Paragraph"));
    }

    REQUIRE(doc->items().at(3)->type() == MD::ItemType::Heading);
    auto h = static_cast<MD::Heading *>(doc->items().at(3).get());
    CHECK_POSITIONS(h, 0, 2, 8, 2);
    REQUIRE(h->delims().size() == 1);
    REQUIRE(h->delims().front() == MD::WithPosition{0, 2, 0, 2});

    REQUIRE(h->text().get());
    auto p = h->text().get();
    CHECK_POSITIONS(p, 2, 2, 8, 2);

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    CHECK_POSITIONS(t, 2, 2, 8, 2);
    REQUIRE(t->text() == QStringLiteral("Heading"));
}

/*
<!-- --> 1<!-- -->

*/
TEST_CASE("049")
{
    MD::Parser parser;
    auto doc = parser.parse(QStringLiteral("tests/parser/data/049.md"));

    auto checkDoc = [](QSharedPointer<MD::Document> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);

        REQUIRE(doc->items().at(0)->type() == MD::ItemType::Anchor);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<!-- --> 1<!-- -->"));
        CHECK_POSITIONS(h, 0, 0, 17, 0);
    };

    checkDoc(doc);

    checkDoc(doc->clone().staticCast<MD::Document>());
}

/*
[a\]](#1)
[b\\](#2)
[c\-d](#3)
[\\](#4)

*/
TEST_CASE("050")
{
    MD::Parser parser;
    auto doc = parser.parse(QStringLiteral("tests/parser/data/050.md"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto *p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 4);
    CHECK_POSITIONS(p, 0, 0, 7, 3);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l0 = static_cast<MD::Link *>(p->items().at(0).get());
    CHECK_POSITIONS(l0, 0, 0, 8, 0);
    REQUIRE(l0->text() == QStringLiteral("a]"));
    REQUIRE(l0->textPos() == MD::WithPosition{1, 0, 3, 0});
    REQUIRE(l0->urlPos() == MD::WithPosition{6, 0, 7, 0});

    REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
    auto l1 = static_cast<MD::Link *>(p->items().at(1).get());
    CHECK_POSITIONS(l1, 0, 1, 8, 1);
    REQUIRE(l1->text() == QStringLiteral("b\\"));
    REQUIRE(l1->textPos() == MD::WithPosition{1, 1, 3, 1});
    REQUIRE(l1->urlPos() == MD::WithPosition{6, 1, 7, 1});

    REQUIRE(p->items().at(2)->type() == MD::ItemType::Link);
    auto l2 = static_cast<MD::Link *>(p->items().at(2).get());
    CHECK_POSITIONS(l2, 0, 2, 9, 2);
    REQUIRE(l2->text() == QStringLiteral("c-d"));
    REQUIRE(l2->textPos() == MD::WithPosition{1, 2, 4, 2});
    REQUIRE(l2->urlPos() == MD::WithPosition{7, 2, 8, 2});

    REQUIRE(p->items().at(3)->type() == MD::ItemType::Link);
    auto l3 = static_cast<MD::Link *>(p->items().at(3).get());
    CHECK_POSITIONS(l3, 0, 3, 7, 3);
    REQUIRE(l3->text() == QStringLiteral("\\"));
    REQUIRE(l3->textPos() == MD::WithPosition{1, 3, 2, 3});
    REQUIRE(l3->urlPos() == MD::WithPosition{5, 3, 6, 3});
}

/*
[link1](051-1.md)
[link2](051-2.md)

*/
TEST_CASE("051")
{
    MD::Parser parser;
    auto doc = parser.parse(QStringLiteral("tests/parser/data/051.md"));

    const QString wd = QDir().absolutePath() + QStringLiteral("/tests/parser/data");

    REQUIRE(doc->items().size() == 8);

    REQUIRE(static_cast<MD::Anchor *>(doc->items().at(0).get())->label() == wd + QStringLiteral("/051.md"));

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::PageBreak);

    REQUIRE(static_cast<MD::Anchor *>(doc->items().at(3).get())->label() == wd + QStringLiteral("/051-1.md"));

    REQUIRE(doc->items().at(4)->type() == MD::ItemType::Paragraph);
    REQUIRE(doc->items().at(5)->type() == MD::ItemType::PageBreak);

    REQUIRE(static_cast<MD::Anchor *>(doc->items().at(6).get())->label() == wd + QStringLiteral("/051-2.md"));

    REQUIRE(doc->items().at(7)->type() == MD::ItemType::Paragraph);
}

/*
```code *bold _italic
_```` *bold* _italic
```

*/
TEST_CASE("052")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/052.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

        auto c = static_cast<MD::Code *>(doc->items().at(1).get());
        CHECK_POSITIONS(c, 0, 1, 19, 1);
        REQUIRE(c->startDelim() == MD::WithPosition{0, 0, 2, 0});
        REQUIRE(c->endDelim() == MD::WithPosition{0, 2, 2, 2});
        REQUIRE(c->syntaxPos() == MD::WithPosition{3, 0, 6, 0});

        REQUIRE(c->isInline() == false);
        REQUIRE(c->text() == QStringLiteral("_```` *bold* _italic"));
        REQUIRE(c->syntax() == QStringLiteral("code"));
    }
}

/*
```cpp
```

*/
TEST_CASE("053")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/053.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code *>(doc->items().at(1).get());
        CHECK_POSITIONS(c, 6, 0, 6, 0);
        REQUIRE(!c->isInline());
        REQUIRE(c->text().isEmpty());
        REQUIRE(c->startDelim() == MD::WithPosition{0, 0, 2, 0});
        REQUIRE(c->endDelim() == MD::WithPosition{0, 1, 2, 1});
        REQUIRE(c->syntaxPos() == MD::WithPosition{3, 0, 5, 0});
    }
}

/*
```java

*/
TEST_CASE("054")
{
    MD::Parser parser;

    const auto doc = parser.parse(QStringLiteral("tests/parser/data/054.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code *>(doc->items().at(1).get());
        REQUIRE(c->syntax() == QStringLiteral("java"));
        REQUIRE(!c->isInline());
        REQUIRE(c->text() == QStringLiteral("\n"));
        CHECK_POSITIONS(c, 0, 1, 0, 1);
        REQUIRE(c->startDelim() == MD::WithPosition{0, 0, 2, 0});
        REQUIRE(c->endDelim() == MD::WithPosition{-1, -1, -1, -1});
        REQUIRE(c->syntaxPos() == MD::WithPosition{3, 0, 6, 0});
    }
}

/*
  [^footnote]: Paragraph in footnote

    Paragraph in footnote

    Paragraph in footnote

Text

*/
TEST_CASE("055")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/055.md"), false);

    const QString wd = QDir().absolutePath() + QStringLiteral("/tests/parser/data/");

    auto checkDoc = [&wd](QSharedPointer<MD::Document> doc) {
        REQUIRE(doc->type() == MD::ItemType::Document);

        REQUIRE(!doc->isEmpty());
        REQUIRE(doc->items().size() == 2);
        REQUIRE(doc->items().at(0)->type() == MD::ItemType::Anchor);

        REQUIRE(doc->footnotesMap().size() == 1);

        const QString label = QStringLiteral("#^FOOTNOTE/") + wd + QStringLiteral("055.md");

        const auto fit = doc->footnotesMap().find(label);
        REQUIRE(fit != doc->footnotesMap().cend());

        auto f = fit.value();

        REQUIRE(f->type() == MD::ItemType::Footnote);

        REQUIRE(f->items().size() == 3);
        CHECK_POSITIONS(f.get(), 0, 0, 21, 4);
        REQUIRE(f->idPos() == MD::WithPosition{1, 0, 12, 0});

        static const std::vector<long long int> start = {14, 4, 1};

        for (int i = 0; i < 3; ++i) {
            REQUIRE(f->items().at(i)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(f->items().at(i).get());
            CHECK_POSITIONS(p, start.at(i), 2 * i, start.at(i) + 20, 2 * i);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            CHECK_POSITIONS(t, start.at(i), 2 * i, start.at(i) + 20, 2 * i);

            REQUIRE(t->text() == QStringLiteral("Paragraph in footnote"));
        }

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        CHECK_POSITIONS(p, 0, 6, 3, 6);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 6, 3, 6);

        REQUIRE(t->text() == QStringLiteral("Text"));
    };

    checkDoc(doc);

    checkDoc(doc->clone().staticCast<MD::Document>());
}

/*
    if( a > b )

      do_something();

    else

      dont_do_anything();

*/
TEST_CASE("056")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/056.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(doc->items().at(1).get());
    CHECK_POSITIONS(c, 4, 0, 24, 6);

    REQUIRE(c->isInline() == false);
    REQUIRE(c->text() == QStringLiteral("if( a > b )\n\n  do_something();\n\nelse\n\n  dont_do_anything();"));
}

/*
```
code

code
```

*/
TEST_CASE("057")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/057.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(doc->items().at(1).get());
    CHECK_POSITIONS(c, 0, 1, 3, 3);
    REQUIRE(c->startDelim() == MD::WithPosition{0, 0, 2, 0});
    REQUIRE(c->endDelim() == MD::WithPosition{0, 4, 2, 4});
    REQUIRE(c->syntaxPos() == MD::WithPosition{-1, -1, -1, -1});

    REQUIRE(c->isInline() == false);
    REQUIRE(c->text() == QStringLiteral("code\n\ncode"));
}

/*
 * List 1

   Paragraph

 *  List 2

    Paragraph

*/
TEST_CASE("058")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/058.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 1, 0, 0, 7);

    REQUIRE(l->items().size() == 2);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);

    auto li = static_cast<MD::ListItem *>(l->items().at(1).get());
    CHECK_POSITIONS(li, 1, 4, 0, 7);
    REQUIRE(li->delim() == MD::WithPosition{1, 4, 1, 4});

    REQUIRE(li->items().size() == 2);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::Paragraph);
}

/*
> ## This is a header.
>
> 1.   This is the first list item.
> 2.   This is the second list item.
>
> Here's some example code:
>
>     return shell_exec("echo $input | $markdown_script");

*/
TEST_CASE("059")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/059.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);

    auto b = static_cast<MD::Blockquote *>(doc->items().at(1).get());
    CHECK_POSITIONS(b, 0, 0, 57, 7);
    REQUIRE(b->delims()
            == MD::Blockquote::Delims{{0, 0, 0, 0},
                                      {0, 1, 0, 1},
                                      {0, 2, 0, 2},
                                      {0, 3, 0, 3},
                                      {0, 4, 0, 4},
                                      {0, 5, 0, 5},
                                      {0, 6, 0, 6},
                                      {0, 7, 0, 7}});

    REQUIRE(b->items().size() == 4);

    REQUIRE(b->items().at(0)->type() == MD::ItemType::Heading);
    {
        auto i = static_cast<MD::Heading *>(b->items().at(0).get());
        CHECK_POSITIONS(i, 2, 0, 21, 0);
        REQUIRE(i->delims().size() == 1);
        REQUIRE(i->delims().front() == MD::WithPosition{2, 0, 3, 0});
    }
    REQUIRE(b->items().at(1)->type() == MD::ItemType::List);
    {
        auto i = static_cast<MD::List *>(b->items().at(1).get());
        CHECK_POSITIONS(i, 2, 2, 0, 4);

        REQUIRE(i->items().size() == 2);

        for (int j = 0; j < 2; ++j) {
            REQUIRE(i->items().at(j)->type() == MD::ItemType::ListItem);
            auto li = static_cast<MD::ListItem *>(i->items().at(j).get());
            REQUIRE(li->delim() == MD::WithPosition{2, 2 + j, 3, 2 + j});
        }
    }
    REQUIRE(b->items().at(2)->type() == MD::ItemType::Paragraph);
    {
        auto i = static_cast<MD::Paragraph *>(b->items().at(2).get());
        CHECK_POSITIONS(i, 2, 5, 26, 5);
    }
    REQUIRE(b->items().at(3)->type() == MD::ItemType::Code);
    {
        auto i = static_cast<MD::Code *>(b->items().at(3).get());
        CHECK_POSITIONS(i, 6, 7, 57, 7);
    }
}

/*
 * List
   * List

*/
TEST_CASE("060")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/060.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 1, 0, 0, 2);

    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    CHECK_POSITIONS(li, 1, 0, 0, 2);
    REQUIRE(li->delim() == MD::WithPosition{1, 0, 1, 0});

    REQUIRE(li->items().size() == 2);

    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::List);

    {
        auto l = static_cast<MD::List *>(li->items().at(1).get());
        REQUIRE(l->items().size() == 1);
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto nli = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(nli->delim() == MD::WithPosition{3, 1, 3, 1});
    }
}
