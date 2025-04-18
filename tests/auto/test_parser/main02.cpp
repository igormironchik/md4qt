
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

inline typename TRAIT::String to_string(int i)
{
#ifdef MD4QT_QT_SUPPORT
    return QString::number(i);
#else
    return std::to_string(i);
#endif
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
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/031.md"));

    REQUIRE(doc->labeledLinks().size() == 2);
    auto it = doc->labeledLinks().cbegin();

    {
        auto l = static_cast<MD::Link<TRAIT> *>(it->second.get());
        REQUIRE(l->startColumn() == 0);
        REQUIRE(l->startLine() == 6);
        REQUIRE(l->endColumn() == 16);
        REQUIRE(l->endLine() == 6);
    }

    ++it;

    {
        auto l = static_cast<MD::Link<TRAIT> *>(it->second.get());
        REQUIRE(l->startColumn() == 0);
        REQUIRE(l->startLine() == 2);
        REQUIRE(l->endColumn() == 43);
        REQUIRE(l->endLine() == 2);
    }

    typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
        QDir().absolutePath()
#else
        typename TRAIT::String(std::filesystem::canonical(std::filesystem::current_path()).u8string())
#endif
        + TRAIT::latin1ToString("/tests/parser/data");

#ifndef MD4QT_QT_SUPPORT
    std::string tmp;
    wd.toUTF8String(tmp);
    std::replace(tmp.begin(), tmp.end(), '\\', '/');
    wd = icu::UnicodeString::fromUTF8(tmp);
#endif

    auto checkDoc = [&wd](std::shared_ptr<MD::Document<TRAIT>> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 8);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 84);
        REQUIRE(p->endLine() == 0);

        REQUIRE(p->items().size() == 8);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t0 = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t0->startColumn() == 0);
        REQUIRE(t0->startLine() == 0);
        REQUIRE(t0->endColumn() == 21);
        REQUIRE(t0->endLine() == 0);

        REQUIRE(t0->text() == TRAIT::latin1ToString("[link 0][wrong-label] "));
        REQUIRE(t0->opts() == MD::TextWithoutFormat);

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);

        auto l1 = static_cast<MD::Link<TRAIT> *>(p->items().at(1).get());
        REQUIRE(l1->startColumn() == 22);
        REQUIRE(l1->startLine() == 0);
        REQUIRE(l1->endColumn() == 35);
        REQUIRE(l1->endLine() == 0);

        REQUIRE(l1->text() == TRAIT::latin1ToString("link 1"));
        REQUIRE(l1->textPos() == MD::WithPosition{23, 0, 28, 0});
        REQUIRE(l1->url() == wd + TRAIT::latin1ToString("/a.md"));
        REQUIRE(l1->urlPos() == MD::WithPosition{31, 0, 34, 0});

        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t1 = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t1->startColumn() == 36);
        REQUIRE(t1->startLine() == 0);
        REQUIRE(t1->endColumn() == 36);
        REQUIRE(t1->endLine() == 0);
        REQUIRE(t1->text() == TRAIT::latin1ToString(" "));

        REQUIRE(p->items().at(3)->type() == MD::ItemType::Link);

        auto l2 = static_cast<MD::Link<TRAIT> *>(p->items().at(3).get());
        REQUIRE(l2->startColumn() == 37);
        REQUIRE(l2->startLine() == 0);
        REQUIRE(l2->endColumn() == 61);
        REQUIRE(l2->endLine() == 0);

        REQUIRE(l2->url() == wd + TRAIT::latin1ToString("/b.md"));
        REQUIRE(l2->opts() == MD::TextOption::TextWithoutFormat);

        REQUIRE(l2->img().get());
        REQUIRE(l2->img()->text() == TRAIT::latin1ToString("image 1"));
        REQUIRE(l2->img()->url() == wd + TRAIT::latin1ToString("/") + TRAIT::latin1ToString("a.png"));
        REQUIRE(l2->textPos() == MD::WithPosition{38, 0, 54, 0});
        REQUIRE(l2->urlPos() == MD::WithPosition{57, 0, 60, 0});

        REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
        auto t2 = static_cast<MD::Text<TRAIT> *>(p->items().at(4).get());
        REQUIRE(t2->startColumn() == 62);
        REQUIRE(t2->startLine() == 0);
        REQUIRE(t2->endColumn() == 62);
        REQUIRE(t2->endLine() == 0);
        REQUIRE(t2->text() == TRAIT::latin1ToString(" "));

        REQUIRE(p->items().at(5)->type() == MD::ItemType::Link);

        auto l3 = static_cast<MD::Link<TRAIT> *>(p->items().at(5).get());
        REQUIRE(l3->startColumn() == 63);
        REQUIRE(l3->startLine() == 0);
        REQUIRE(l3->endColumn() == 77);
        REQUIRE(l3->endLine() == 0);

        REQUIRE(l3->text() == TRAIT::latin1ToString("link 3"));

        const TRAIT::String label = TRAIT::latin1ToString("#LABEL/") + wd + TRAIT::latin1ToString("/031.md");

        REQUIRE(l3->url() == label);
        REQUIRE(l3->textPos() == MD::WithPosition{64, 0, 69, 0});
        REQUIRE(l3->urlPos() == MD::WithPosition{72, 0, 76, 0});

        REQUIRE(p->items().at(6)->type() == MD::ItemType::Text);
        auto t3 = static_cast<MD::Text<TRAIT> *>(p->items().at(6).get());
        REQUIRE(t3->startColumn() == 78);
        REQUIRE(t3->startLine() == 0);
        REQUIRE(t3->endColumn() == 78);
        REQUIRE(t3->endLine() == 0);
        REQUIRE(t3->text() == TRAIT::latin1ToString(" "));

        REQUIRE(p->items().at(7)->type() == MD::ItemType::FootnoteRef);

        auto f1 = static_cast<MD::FootnoteRef<TRAIT> *>(p->items().at(7).get());
        REQUIRE(f1->startColumn() == 79);
        REQUIRE(f1->startLine() == 0);
        REQUIRE(f1->endColumn() == 84);
        REQUIRE(f1->endLine() == 0);

        REQUIRE(f1->id() == TRAIT::latin1ToString("#^REF/") + wd + TRAIT::latin1ToString("/031.md"));
        REQUIRE(f1->idPos() == MD::WithPosition{80, 0, 83, 0});

        REQUIRE(!doc->labeledLinks().empty());
        const auto lit = doc->labeledLinks().find(label);
        REQUIRE(lit != doc->labeledLinks().cend());
        REQUIRE(lit->second->url() == TRAIT::latin1ToString("http://www.where.com/a.md"));

        {
            REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);

            p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
            REQUIRE(p->startColumn() == 0);
            REQUIRE(p->startLine() == 4);
            REQUIRE(p->endColumn() == 10);
            REQUIRE(p->endLine() == 4);

            REQUIRE(p->items().size() == 2);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::FootnoteRef);

            f1 = static_cast<MD::FootnoteRef<TRAIT> *>(p->items().at(0).get());
            REQUIRE(f1->startColumn() == 0);
            REQUIRE(f1->startLine() == 4);
            REQUIRE(f1->endColumn() == 5);
            REQUIRE(f1->endLine() == 4);
            REQUIRE(f1->idPos() == MD::WithPosition{1, 4, 4, 4});

            REQUIRE(f1->id() == TRAIT::latin1ToString("#^REF/") + wd + TRAIT::latin1ToString("/031.md"));

            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
            REQUIRE(t->startColumn() == 6);
            REQUIRE(t->startLine() == 4);
            REQUIRE(t->endColumn() == 10);
            REQUIRE(t->endLine() == 4);

            REQUIRE(t->text() == TRAIT::latin1ToString(" text"));

            REQUIRE(doc->labeledLinks().size() == 2);

            const auto lit = doc->labeledLinks().find(TRAIT::latin1ToString("#1/") + wd + TRAIT::latin1ToString("/031.md"));
            REQUIRE(lit != doc->labeledLinks().cend());

            REQUIRE(lit->second->url() == wd + TRAIT::latin1ToString("/a.md"));
        }

        {
            REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);

            p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(3).get());
            REQUIRE(p->startColumn() == 0);
            REQUIRE(p->startLine() == 8);
            REQUIRE(p->endColumn() == 15);
            REQUIRE(p->endLine() == 8);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);

            auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
            REQUIRE(l->startColumn() == 0);
            REQUIRE(l->startLine() == 8);
            REQUIRE(l->endColumn() == 15);
            REQUIRE(l->endLine() == 8);
            REQUIRE(l->textPos() == MD::WithPosition{1, 8, 6, 8});
            REQUIRE(l->urlPos() == MD::WithPosition{9, 8, 14, 8});

            REQUIRE(l->url() == TRAIT::latin1ToString("#label/") + wd + TRAIT::latin1ToString("/031.md"));
        }

        REQUIRE(doc->items().at(4)->type() == MD::ItemType::PageBreak);
    };

    checkDoc(doc);

    checkDoc(std::static_pointer_cast<MD::Document<TRAIT>>(doc->clone()));
}

/*
> ```cpp
> if( a < b )
>   do_something();
> ```

*/
TEST_CASE("032")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/032.md"));

    auto checkDoc = [](std::shared_ptr<MD::Document<TRAIT>> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);

        auto q = static_cast<MD::Blockquote<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(q->startColumn() == 0);
        REQUIRE(q->startLine() == 0);
        REQUIRE(q->endColumn() == 4);
        REQUIRE(q->endLine() == 3);
        REQUIRE(q->delims() == MD::Blockquote<TRAIT>::Delims{{0, 0, 0, 0}, {0, 1, 0, 1}, {0, 2, 0, 2}, {0, 3, 0, 3}});

        REQUIRE(q->items().size() == 1);

        REQUIRE(q->items().at(0)->type() == MD::ItemType::Code);

        auto c = static_cast<MD::Code<TRAIT> *>(q->items().at(0).get());
        REQUIRE(c->startColumn() == 2);
        REQUIRE(c->startLine() == 1);
        REQUIRE(c->endColumn() == 18);
        REQUIRE(c->endLine() == 2);
        REQUIRE(c->startDelim() == MD::WithPosition{2, 0, 4, 0});
        REQUIRE(c->endDelim() == MD::WithPosition{2, 3, 4, 3});
        REQUIRE(c->syntaxPos() == MD::WithPosition{5, 0, 7, 0});

        REQUIRE(c->isInline() == false);
        REQUIRE(c->text() == TRAIT::latin1ToString("if( a < b )\n  do_something();"));
    };

    checkDoc(doc);

    checkDoc(std::static_pointer_cast<MD::Document<TRAIT>>(doc->clone()));
}

/*
<http://www.google.com>

*/
TEST_CASE("033")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/033.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 22);
    REQUIRE(p->endLine() == 0);

    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);

    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->startColumn() == 0);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 22);
    REQUIRE(l->endLine() == 0);

    REQUIRE(l->url() == TRAIT::latin1ToString("http://www.google.com"));
    REQUIRE(l->textPos() == MD::WithPosition{1, 0, 21, 0});
    REQUIRE(l->urlPos() == l->textPos());
    REQUIRE(l->text().isEmpty());
}

/*
**[Google](https://www.google.com)**

*/
TEST_CASE("034")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/034.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 35);
    REQUIRE(p->endLine() == 0);

    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);

    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->startColumn() == 2);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 33);
    REQUIRE(l->endLine() == 0);
    REQUIRE(l->textPos() == MD::WithPosition{3, 0, 8, 0});
    REQUIRE(l->urlPos() == MD::WithPosition{11, 0, 32, 0});

    REQUIRE(l->url() == TRAIT::latin1ToString("https://www.google.com"));
    REQUIRE(l->text() == TRAIT::latin1ToString("Google"));
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
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/035.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(l->startColumn() == 0);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 3);
    REQUIRE(l->endLine() == 4);

    REQUIRE(l->items().size() == 3);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

        auto i1 = static_cast<MD::ListItem<TRAIT> *>(l->items().at(0).get());
        REQUIRE(i1->startColumn() == 0);
        REQUIRE(i1->startLine() == 0);
        REQUIRE(i1->endColumn() == 3);
        REQUIRE(i1->endLine() == 0);
        REQUIRE(i1->delim() == MD::WithPosition{0, 0, 1, 0});

        REQUIRE(i1->listType() == MD::ListItem<TRAIT>::Ordered);
        REQUIRE(i1->orderedListPreState() == MD::ListItem<TRAIT>::Start);
        REQUIRE(i1->items().size() == 1);
        REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(i1->items().at(0).get());
        REQUIRE(p->startColumn() == 3);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 0);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(static_cast<MD::Text<TRAIT> *>(p->items().at(0).get())->text() == TRAIT::latin1ToString("1"));
    }

    REQUIRE(l->items().size() == 3);

    {
        REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);

        auto i1 = static_cast<MD::ListItem<TRAIT> *>(l->items().at(1).get());
        REQUIRE(i1->startColumn() == 0);
        REQUIRE(i1->startLine() == 1);
        REQUIRE(i1->endColumn() == 6);
        REQUIRE(i1->endLine() == 3);
        REQUIRE(i1->delim() == MD::WithPosition{0, 1, 1, 1});

        REQUIRE(i1->listType() == MD::ListItem<TRAIT>::Ordered);
        REQUIRE(i1->orderedListPreState() == MD::ListItem<TRAIT>::Continue);
        REQUIRE(i1->items().size() == 2);
        REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(i1->items().at(0).get());
        REQUIRE(p->startColumn() == 3);
        REQUIRE(p->startLine() == 1);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 1);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(static_cast<MD::Text<TRAIT> *>(p->items().at(0).get())->text() == TRAIT::latin1ToString("2"));

        REQUIRE(i1->items().at(1)->type() == MD::ItemType::List);

        auto nl = static_cast<MD::List<TRAIT> *>(i1->items().at(1).get());
        REQUIRE(nl->startColumn() == 3);
        REQUIRE(nl->startLine() == 2);
        REQUIRE(nl->endColumn() == 6);
        REQUIRE(nl->endLine() == 3);

        REQUIRE(nl->items().size() == 2);

        {
            REQUIRE(nl->items().at(0)->type() == MD::ItemType::ListItem);

            auto i1 = static_cast<MD::ListItem<TRAIT> *>(nl->items().at(0).get());
            REQUIRE(i1->startColumn() == 3);
            REQUIRE(i1->startLine() == 2);
            REQUIRE(i1->endColumn() == 6);
            REQUIRE(i1->endLine() == 2);
            REQUIRE(i1->delim() == MD::WithPosition{3, 2, 4, 2});

            REQUIRE(i1->listType() == MD::ListItem<TRAIT>::Ordered);
            REQUIRE(i1->orderedListPreState() == MD::ListItem<TRAIT>::Start);
            REQUIRE(i1->items().size() == 1);
            REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph<TRAIT> *>(i1->items().at(0).get());
            REQUIRE(p->startColumn() == 6);
            REQUIRE(p->startLine() == 2);
            REQUIRE(p->endColumn() == 6);
            REQUIRE(p->endLine() == 2);

            REQUIRE(p->items().size() == 1);
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            REQUIRE(static_cast<MD::Text<TRAIT> *>(p->items().at(0).get())->text() == TRAIT::latin1ToString("1"));
        }

        {
            REQUIRE(nl->items().at(1)->type() == MD::ItemType::ListItem);

            auto i1 = static_cast<MD::ListItem<TRAIT> *>(nl->items().at(1).get());
            REQUIRE(i1->startColumn() == 3);
            REQUIRE(i1->startLine() == 3);
            REQUIRE(i1->endColumn() == 6);
            REQUIRE(i1->endLine() == 3);
            REQUIRE(i1->delim() == MD::WithPosition{3, 3, 4, 3});

            REQUIRE(i1->listType() == MD::ListItem<TRAIT>::Ordered);
            REQUIRE(i1->orderedListPreState() == MD::ListItem<TRAIT>::Continue);
            REQUIRE(i1->items().size() == 1);
            REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph<TRAIT> *>(i1->items().at(0).get());
            REQUIRE(p->startColumn() == 6);
            REQUIRE(p->startLine() == 3);
            REQUIRE(p->endColumn() == 6);
            REQUIRE(p->endLine() == 3);

            REQUIRE(p->items().size() == 1);
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            REQUIRE(static_cast<MD::Text<TRAIT> *>(p->items().at(0).get())->text() == TRAIT::latin1ToString("2"));
        }
    }

    {
        REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

        auto i1 = static_cast<MD::ListItem<TRAIT> *>(l->items().at(2).get());
        REQUIRE(i1->startColumn() == 0);
        REQUIRE(i1->startLine() == 4);
        REQUIRE(i1->endColumn() == 3);
        REQUIRE(i1->endLine() == 4);
        REQUIRE(i1->delim() == MD::WithPosition{0, 4, 1, 4});

        REQUIRE(i1->listType() == MD::ListItem<TRAIT>::Ordered);
        REQUIRE(i1->orderedListPreState() == MD::ListItem<TRAIT>::Continue);
        REQUIRE(i1->items().size() == 1);
        REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(i1->items().at(0).get());
        REQUIRE(p->startColumn() == 3);
        REQUIRE(p->startLine() == 4);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 4);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(static_cast<MD::Text<TRAIT> *>(p->items().at(0).get())->text() == TRAIT::latin1ToString("3"));
    }
}

/*
[Google]( www.google.com "Google Shmoogle..." )

*/
TEST_CASE("036")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/036.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 46);
    REQUIRE(p->endLine() == 0);

    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);

    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->startColumn() == 0);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 46);
    REQUIRE(l->endLine() == 0);
    REQUIRE(l->textPos() == MD::WithPosition{1, 0, 6, 0});
    REQUIRE(l->urlPos() == MD::WithPosition{10, 0, 23, 0});

    REQUIRE(l->url() == TRAIT::latin1ToString("www.google.com"));
    REQUIRE(l->text() == TRAIT::latin1ToString("Google"));
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
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/037.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 17);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 43);
        REQUIRE(p->endLine() == 0);

        REQUIRE(p->items().size() == 3);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
            REQUIRE(t->startColumn() == 0);
            REQUIRE(t->startLine() == 0);
            REQUIRE(t->endColumn() == 10);
            REQUIRE(t->endLine() == 0);

            REQUIRE(t->text() == TRAIT::latin1ToString("[Google] ( "));
        }

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(1).get());
        REQUIRE(l->startColumn() == 11);
        REQUIRE(l->startLine() == 0);
        REQUIRE(l->endColumn() == 24);
        REQUIRE(l->endLine() == 0);
        REQUIRE(l->textPos() == MD::WithPosition{11, 0, 24, 0});
        REQUIRE(l->urlPos() == l->textPos());
        REQUIRE(l->url() == TRAIT::latin1ToString("http://www.google.com"));

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
            REQUIRE(t->startColumn() == 25);
            REQUIRE(t->startLine() == 0);
            REQUIRE(t->endColumn() == 43);
            REQUIRE(t->endLine() == 0);

            REQUIRE(t->text() == TRAIT::latin1ToString(" Google Shmoogle..."));
        }
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 2);
        REQUIRE(p->endColumn() == 9);
        REQUIRE(p->endLine() == 2);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 2);
        REQUIRE(t->endColumn() == 9);
        REQUIRE(t->endLine() == 2);

        REQUIRE(t->text() == TRAIT::latin1ToString("[Google] ("));
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(3).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 4);
        REQUIRE(p->endColumn() == 6);
        REQUIRE(p->endLine() == 4);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 4);
        REQUIRE(t->endColumn() == 6);
        REQUIRE(t->endLine() == 4);

        REQUIRE(t->text() == TRAIT::latin1ToString("[Google"));
    }

    {
        REQUIRE(doc->items().at(4)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(4).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 6);
        REQUIRE(p->endColumn() == 7);
        REQUIRE(p->endLine() == 6);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());

        REQUIRE(t->text() == TRAIT::latin1ToString("[Google]"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 6);
        REQUIRE(t->endColumn() == 7);
        REQUIRE(t->endLine() == 6);
    }

    {
        REQUIRE(doc->items().at(5)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(5).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 8);
        REQUIRE(p->endColumn() == 10);
        REQUIRE(p->endLine() == 8);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());

        REQUIRE(t->text() == TRAIT::latin1ToString("[![Google]("));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 8);
        REQUIRE(t->endColumn() == 10);
        REQUIRE(t->endLine() == 8);
    }

    {
        REQUIRE(doc->items().at(6)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(6).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 10);
        REQUIRE(p->endColumn() == 9);
        REQUIRE(p->endLine() == 10);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());

        REQUIRE(t->text() == TRAIT::latin1ToString("![Google]("));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 10);
        REQUIRE(t->endColumn() == 9);
        REQUIRE(t->endLine() == 10);
    }

    {
        REQUIRE(doc->items().at(7)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(7).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 12);
        REQUIRE(p->endColumn() == 45);
        REQUIRE(p->endLine() == 12);

        REQUIRE(p->items().size() == 3);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
            REQUIRE(t->startColumn() == 0);
            REQUIRE(t->startLine() == 12);
            REQUIRE(t->endColumn() == 10);
            REQUIRE(t->endLine() == 12);

            REQUIRE(t->text() == TRAIT::latin1ToString("[Google] ( "));
        }

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(1).get());
        REQUIRE(l->startColumn() == 11);
        REQUIRE(l->startLine() == 12);
        REQUIRE(l->endColumn() == 24);
        REQUIRE(l->endLine() == 12);
        REQUIRE(l->url() == TRAIT::latin1ToString("http://www.google.com"));
        REQUIRE(l->textPos() == MD::WithPosition{11, 12, 24, 12});
        REQUIRE(l->urlPos() == l->textPos());

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
            REQUIRE(t->startColumn() == 25);
            REQUIRE(t->startLine() == 12);
            REQUIRE(t->endColumn() == 45);
            REQUIRE(t->endLine() == 12);

            REQUIRE(t->text() == TRAIT::latin1ToString(" \"Google Shmoogle...\""));
        }
    }

    {
        REQUIRE(doc->items().at(8)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(8).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 14);
        REQUIRE(p->endColumn() == 42);
        REQUIRE(p->endLine() == 14);

        REQUIRE(p->items().size() == 2);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 14);
        REQUIRE(t->endColumn() == 0);
        REQUIRE(t->endLine() == 14);

        REQUIRE(t->text() == TRAIT::latin1ToString("["));

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Image);

        auto i = static_cast<MD::Image<TRAIT> *>(p->items().at(1).get());
        REQUIRE(i->startColumn() == 1);
        REQUIRE(i->startLine() == 14);
        REQUIRE(i->endColumn() == 42);
        REQUIRE(i->endLine() == 14);
        REQUIRE(i->textPos() == MD::WithPosition{3, 14, 8, 14});
        REQUIRE(i->urlPos() == MD::WithPosition{11, 14, 41, 14});

        REQUIRE(i->url() == TRAIT::latin1ToString("https://www.google.com/logo.png"));
        REQUIRE(i->text() == TRAIT::latin1ToString("Google"));
    }

    {
        REQUIRE(doc->items().at(9)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(9).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 16);
        REQUIRE(p->endColumn() == 11);
        REQUIRE(p->endLine() == 16);

        REQUIRE(p->items().size() == 3);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 16);
        REQUIRE(t->endColumn() == 4);
        REQUIRE(t->endLine() == 16);

        REQUIRE(t->text() == TRAIT::latin1ToString("text "));

        REQUIRE(p->items().at(1)->type() == MD::ItemType::FootnoteRef);

        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);

        t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->startColumn() == 11);
        REQUIRE(t->startLine() == 16);
        REQUIRE(t->endColumn() == 11);
        REQUIRE(t->endLine() == 16);

        REQUIRE(t->text() == TRAIT::latin1ToString(":"));
    }

    {
        REQUIRE(doc->items().at(10)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(10).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 18);
        REQUIRE(p->endColumn() == 4);
        REQUIRE(p->endLine() == 18);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 18);
        REQUIRE(t->endColumn() == 4);
        REQUIRE(t->endLine() == 18);

        REQUIRE(t->text() == TRAIT::latin1ToString("[^ref"));
    }

    {
        REQUIRE(doc->items().at(11)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(11).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 20);
        REQUIRE(p->endColumn() == 1);
        REQUIRE(p->endLine() == 20);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 20);
        REQUIRE(t->endColumn() == 1);
        REQUIRE(t->endLine() == 20);

        REQUIRE(t->text() == TRAIT::latin1ToString("["));
    }

    {
        REQUIRE(doc->items().at(12)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(12).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 22);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 22);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 22);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 22);

        REQUIRE(t->text() == TRAIT::latin1ToString("[1]:"));
    }

    {
        REQUIRE(doc->items().at(13)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(13).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 24);
        REQUIRE(p->endColumn() == 4);
        REQUIRE(p->endLine() == 24);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 24);
        REQUIRE(t->endColumn() == 4);
        REQUIRE(t->endLine() == 24);

        REQUIRE(t->text() == TRAIT::latin1ToString("[1]:"));
    }

    {
        REQUIRE(doc->items().at(14)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(14).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 26);
        REQUIRE(p->endColumn() == 10);
        REQUIRE(p->endLine() == 26);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 26);
        REQUIRE(t->endColumn() == 10);
        REQUIRE(t->endLine() == 26);

        REQUIRE(t->text() == TRAIT::latin1ToString("[text][link"));
    }

    {
        REQUIRE(doc->items().at(15)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(15).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 28);
        REQUIRE(p->endColumn() == 6);
        REQUIRE(p->endLine() == 28);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 28);
        REQUIRE(t->endColumn() == 6);
        REQUIRE(t->endLine() == 28);

        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == TRAIT::latin1ToString("[text]#"));
    }

    {
        REQUIRE(doc->items().at(16)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(16).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 30);
        REQUIRE(p->endColumn() == 14);
        REQUIRE(p->endLine() == 30);

        REQUIRE(p->items().size() == 2);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 30);
        REQUIRE(t->endColumn() == 0);
        REQUIRE(t->endLine() == 30);

        REQUIRE(t->text() == TRAIT::latin1ToString("<"));

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(1).get());
        REQUIRE(l->startColumn() == 1);
        REQUIRE(l->startLine() == 30);
        REQUIRE(l->endColumn() == 14);
        REQUIRE(l->endLine() == 30);
        REQUIRE(l->url() == TRAIT::latin1ToString("http://www.google.com"));
        REQUIRE(l->textPos() == MD::WithPosition{1, 30, 14, 30});
        REQUIRE(l->urlPos() == l->textPos());
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
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/038.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(l->startColumn() == 0);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 3);
    REQUIRE(l->endLine() == 4);

    REQUIRE(l->items().size() == 3);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

        auto i1 = static_cast<MD::ListItem<TRAIT> *>(l->items().at(0).get());
        REQUIRE(i1->startColumn() == 0);
        REQUIRE(i1->startLine() == 0);
        REQUIRE(i1->endColumn() == 3);
        REQUIRE(i1->endLine() == 0);
        REQUIRE(i1->delim() == MD::WithPosition{0, 0, 1, 0});

        REQUIRE(i1->listType() == MD::ListItem<TRAIT>::Ordered);
        REQUIRE(i1->orderedListPreState() == MD::ListItem<TRAIT>::Start);
        REQUIRE(i1->items().size() == 1);
        REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(i1->items().at(0).get());
        REQUIRE(p->startColumn() == 3);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 0);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(static_cast<MD::Text<TRAIT> *>(p->items().at(0).get())->text() == TRAIT::latin1ToString("1"));
    }

    REQUIRE(l->items().size() == 3);

    {
        REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);

        auto i1 = static_cast<MD::ListItem<TRAIT> *>(l->items().at(1).get());
        REQUIRE(i1->startColumn() == 0);
        REQUIRE(i1->startLine() == 1);
        REQUIRE(i1->endColumn() == 4);
        REQUIRE(i1->endLine() == 3);
        REQUIRE(i1->delim() == MD::WithPosition{0, 1, 1, 1});

        REQUIRE(i1->listType() == MD::ListItem<TRAIT>::Ordered);
        REQUIRE(i1->orderedListPreState() == MD::ListItem<TRAIT>::Continue);
        REQUIRE(i1->items().size() == 2);
        REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(i1->items().at(0).get());
        REQUIRE(p->startColumn() == 3);
        REQUIRE(p->startLine() == 1);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 1);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(static_cast<MD::Text<TRAIT> *>(p->items().at(0).get())->text() == TRAIT::latin1ToString("2"));

        REQUIRE(i1->items().at(1)->type() == MD::ItemType::List);

        auto nl = static_cast<MD::List<TRAIT> *>(i1->items().at(1).get());
        REQUIRE(nl->startColumn() == 1);
        REQUIRE(nl->startLine() == 2);
        REQUIRE(nl->endColumn() == 4);
        REQUIRE(nl->endLine() == 3);

        REQUIRE(nl->items().size() == 2);

        {
            REQUIRE(nl->items().at(0)->type() == MD::ItemType::ListItem);

            auto i1 = static_cast<MD::ListItem<TRAIT> *>(nl->items().at(0).get());
            REQUIRE(i1->startColumn() == 1);
            REQUIRE(i1->startLine() == 2);
            REQUIRE(i1->endColumn() == 4);
            REQUIRE(i1->endLine() == 2);
            REQUIRE(i1->delim() == MD::WithPosition{1, 2, 2, 2});

            REQUIRE(i1->listType() == MD::ListItem<TRAIT>::Ordered);
            REQUIRE(i1->orderedListPreState() == MD::ListItem<TRAIT>::Start);
            REQUIRE(i1->items().size() == 1);
            REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph<TRAIT> *>(i1->items().at(0).get());
            REQUIRE(p->startColumn() == 4);
            REQUIRE(p->startLine() == 2);
            REQUIRE(p->endColumn() == 4);
            REQUIRE(p->endLine() == 2);

            REQUIRE(p->items().size() == 1);
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            REQUIRE(static_cast<MD::Text<TRAIT> *>(p->items().at(0).get())->text() == TRAIT::latin1ToString("1"));
        }

        {
            REQUIRE(nl->items().at(1)->type() == MD::ItemType::ListItem);

            auto i1 = static_cast<MD::ListItem<TRAIT> *>(nl->items().at(1).get());
            REQUIRE(i1->startColumn() == 1);
            REQUIRE(i1->startLine() == 3);
            REQUIRE(i1->endColumn() == 4);
            REQUIRE(i1->endLine() == 3);
            REQUIRE(i1->delim() == MD::WithPosition{1, 3, 2, 3});

            REQUIRE(i1->listType() == MD::ListItem<TRAIT>::Ordered);
            REQUIRE(i1->orderedListPreState() == MD::ListItem<TRAIT>::Continue);
            REQUIRE(i1->items().size() == 1);
            REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph<TRAIT> *>(i1->items().at(0).get());
            REQUIRE(p->startColumn() == 4);
            REQUIRE(p->startLine() == 3);
            REQUIRE(p->endColumn() == 4);
            REQUIRE(p->endLine() == 3);

            REQUIRE(p->items().size() == 1);
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            REQUIRE(static_cast<MD::Text<TRAIT> *>(p->items().at(0).get())->text() == TRAIT::latin1ToString("2"));
        }
    }

    {
        REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

        auto i1 = static_cast<MD::ListItem<TRAIT> *>(l->items().at(2).get());
        REQUIRE(i1->startColumn() == 0);
        REQUIRE(i1->startLine() == 4);
        REQUIRE(i1->endColumn() == 3);
        REQUIRE(i1->endLine() == 4);
        REQUIRE(i1->delim() == MD::WithPosition{0, 4, 1, 4});

        REQUIRE(i1->listType() == MD::ListItem<TRAIT>::Ordered);
        REQUIRE(i1->orderedListPreState() == MD::ListItem<TRAIT>::Continue);
        REQUIRE(i1->items().size() == 1);
        REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(i1->items().at(0).get());
        REQUIRE(p->startColumn() == 3);
        REQUIRE(p->startLine() == 4);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 4);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(static_cast<MD::Text<TRAIT> *>(p->items().at(0).get())->text() == TRAIT::latin1ToString("3"));
    }
}

/*
****text****

*/
TEST_CASE("039")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/039.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 11);
    REQUIRE(p->endLine() == 0);

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->startColumn() == 4);
    REQUIRE(t->startLine() == 0);
    REQUIRE(t->endColumn() == 7);
    REQUIRE(t->endLine() == 0);

    REQUIRE(t->opts() == MD::TextOption::BoldText);
    REQUIRE(t->text() == TRAIT::latin1ToString("text"));
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
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/040.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto dp = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(dp->startColumn() == 0);
    REQUIRE(dp->startLine() == 0);
    REQUIRE(dp->endColumn() == 10);
    REQUIRE(dp->endLine() == 1);

    REQUIRE(dp->items().size() == 3);

    REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);

    auto t1 = static_cast<MD::Text<TRAIT> *>(dp->items().at(0).get());

    REQUIRE(t1->text() == TRAIT::latin1ToString("``Use this "));
    REQUIRE(t1->startColumn() == 0);
    REQUIRE(t1->startLine() == 0);
    REQUIRE(t1->endColumn() == 10);
    REQUIRE(t1->endLine() == 0);

    REQUIRE(dp->items().at(1)->type() == MD::ItemType::Code);

    auto c1 = static_cast<MD::Code<TRAIT> *>(dp->items().at(1).get());
    REQUIRE(c1->startColumn() == 12);
    REQUIRE(c1->startLine() == 0);
    REQUIRE(c1->endColumn() == 15);
    REQUIRE(c1->endLine() == 0);
    REQUIRE(c1->startDelim() == MD::WithPosition{11, 0, 11, 0});
    REQUIRE(c1->endDelim() == MD::WithPosition{16, 0, 16, 0});

    REQUIRE(c1->text() == TRAIT::latin1ToString("code"));

    REQUIRE(dp->items().at(2)->type() == MD::ItemType::Text);

    auto t2 = static_cast<MD::Text<TRAIT> *>(dp->items().at(2).get());
    REQUIRE(t2->startColumn() == 0);
    REQUIRE(t2->startLine() == 1);
    REQUIRE(t2->endColumn() == 10);
    REQUIRE(t2->endLine() == 1);

    REQUIRE(t2->text() == TRAIT::latin1ToString("in the code"));
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
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/041.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(l->startColumn() == 0);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 17);
    REQUIRE(l->endLine() == 10);

    REQUIRE(l->items().size() == 3);

    for (int i = 0; i < 3; ++i) {
        REQUIRE(l->items().at(i)->type() == MD::ItemType::ListItem);

        auto item = static_cast<MD::ListItem<TRAIT> *>(l->items().at(i).get());
        REQUIRE(item->startColumn() == 0);
        REQUIRE(item->startLine() == 4 * i);
        REQUIRE(item->endColumn() == 17);
        REQUIRE(item->endLine() == 2 + 4 * i);
        REQUIRE(item->delim() == MD::WithPosition{0, i * 4, 0, i * 4});

        REQUIRE(item->listType() == MD::ListItem<TRAIT>::Unordered);

        REQUIRE(item->items().size() == 2);

        {
            REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph<TRAIT> *>(item->items().at(0).get());
            REQUIRE(p->startColumn() == 2);
            REQUIRE(p->startLine() == 4 * i);
            REQUIRE(p->endColumn() == 7);
            REQUIRE(p->endLine() == 4 * i);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
            REQUIRE(t->startColumn() == 2);
            REQUIRE(t->startLine() == 4 * i);
            REQUIRE(t->endColumn() == 7);
            REQUIRE(t->endLine() == 4 * i);

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("Item ") + to_string(i + 1));
        }

        {
            REQUIRE(item->items().at(1)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph<TRAIT> *>(item->items().at(1).get());
            REQUIRE(p->startColumn() == 0);
            REQUIRE(p->startLine() == 2 + 4 * i);
            REQUIRE(p->endColumn() == 17);
            REQUIRE(p->endLine() == 2 + 4 * i);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
            REQUIRE(t->startColumn() == 0);
            REQUIRE(t->startLine() == 2 + 4 * i);
            REQUIRE(t->endColumn() == 17);
            REQUIRE(t->endLine() == 2 + 4 * i);

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == TRAIT::latin1ToString("Paragraph in list"));
        }
    }
}

/*
* [Chapter 1](042-1.md)
* [Chapter 1](042-1.md)

*/
TEST_CASE("042")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/042.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 5);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(l->startColumn() == 0);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 22);
    REQUIRE(l->endLine() == 1);

    REQUIRE(l->items().size() == 2);

    typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
        QDir().absolutePath()
#else
        typename TRAIT::String(std::filesystem::canonical(std::filesystem::current_path()).u8string())
#endif
        + TRAIT::latin1ToString("/tests/parser/data");

#ifndef MD4QT_QT_SUPPORT
    std::string tmp;
    wd.toUTF8String(tmp);
    std::replace(tmp.begin(), tmp.end(), '\\', '/');
    wd = icu::UnicodeString::fromUTF8(tmp);
#endif

    for (int i = 0; i < 2; ++i) {
        REQUIRE(l->items().at(i)->type() == MD::ItemType::ListItem);

        auto li = static_cast<MD::ListItem<TRAIT> *>(l->items().at(i).get());
        REQUIRE(li->startColumn() == 0);
        REQUIRE(li->startLine() == i);
        REQUIRE(li->endColumn() == 22);
        REQUIRE(li->endLine() == i);
        REQUIRE(li->delim() == MD::WithPosition{0, i, 0, i});

        REQUIRE(li->items().size() == 1);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(li->items().at(0).get());
        REQUIRE(p->startColumn() == 2);
        REQUIRE(p->startLine() == i);
        REQUIRE(p->endColumn() == 22);
        REQUIRE(p->endLine() == i);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);

        auto lnk = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
        REQUIRE(lnk->startColumn() == 2);
        REQUIRE(lnk->startLine() == i);
        REQUIRE(lnk->endColumn() == 22);
        REQUIRE(lnk->endLine() == i);
        REQUIRE(lnk->textPos() == MD::WithPosition{3, i, 11, i});
        REQUIRE(lnk->urlPos() == MD::WithPosition{14, i, 21, i});

        REQUIRE(lnk->text() == TRAIT::latin1ToString("Chapter 1"));
        REQUIRE(lnk->url() == wd + TRAIT::latin1ToString("/042-1.md"));
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::PageBreak);

    REQUIRE(doc->items().at(3)->type() == MD::ItemType::Anchor);

    REQUIRE(static_cast<MD::Anchor<TRAIT> *>(doc->items().at(3).get())->label() == wd + TRAIT::latin1ToString("/042-1.md"));

    REQUIRE(doc->items().at(4)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(4).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 10);
    REQUIRE(p->endLine() == 0);

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->startColumn() == 0);
    REQUIRE(t->startLine() == 0);
    REQUIRE(t->endColumn() == 10);
    REQUIRE(t->endLine() == 0);

    REQUIRE(t->text() == TRAIT::latin1ToString("Paragraph 1"));
}

TEST_CASE("042-1")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/042.md"), false);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(l->startColumn() == 0);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 22);
    REQUIRE(l->endLine() == 1);

    REQUIRE(l->items().size() == 2);

    typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
        QDir().absolutePath()
#else
        typename TRAIT::String(std::filesystem::canonical(std::filesystem::current_path()).u8string())
#endif
        + TRAIT::latin1ToString("/tests/parser/data");

#ifndef MD4QT_QT_SUPPORT
    std::string tmp;
    wd.toUTF8String(tmp);
    std::replace(tmp.begin(), tmp.end(), '\\', '/');
    wd = icu::UnicodeString::fromUTF8(tmp);
#endif

    for (int i = 0; i < 2; ++i) {
        REQUIRE(l->items().at(i)->type() == MD::ItemType::ListItem);

        auto li = static_cast<MD::ListItem<TRAIT> *>(l->items().at(i).get());
        REQUIRE(li->startColumn() == 0);
        REQUIRE(li->startLine() == i);
        REQUIRE(li->endColumn() == 22);
        REQUIRE(li->endLine() == i);

        REQUIRE(li->items().size() == 1);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(li->items().at(0).get());
        REQUIRE(p->startColumn() == 2);
        REQUIRE(p->startLine() == i);
        REQUIRE(p->endColumn() == 22);
        REQUIRE(p->endLine() == i);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);

        auto lnk = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
        REQUIRE(lnk->startColumn() == 2);
        REQUIRE(lnk->startLine() == i);
        REQUIRE(lnk->endColumn() == 22);
        REQUIRE(lnk->endLine() == i);

        REQUIRE(lnk->text() == TRAIT::latin1ToString("Chapter 1"));
        REQUIRE(lnk->url() == wd + TRAIT::latin1ToString("/042-1.md"));
    }
}

/*
* Item

    > Quote


*/
TEST_CASE("044")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/044.md"), false);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(l->startColumn() == 0);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 10);
    REQUIRE(l->endLine() == 2);

    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto li = static_cast<MD::ListItem<TRAIT> *>(l->items().at(0).get());
    REQUIRE(li->startColumn() == 0);
    REQUIRE(li->startLine() == 0);
    REQUIRE(li->endColumn() == 10);
    REQUIRE(li->endLine() == 2);
    REQUIRE(li->delim() == MD::WithPosition{0, 0, 0, 0});

    REQUIRE(li->items().size() == 2);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph<TRAIT> *>(li->items().at(0).get());
    REQUIRE(p->startColumn() == 2);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 5);
    REQUIRE(p->endLine() == 0);

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->startColumn() == 2);
    REQUIRE(t->startLine() == 0);
    REQUIRE(t->endColumn() == 5);
    REQUIRE(t->endLine() == 0);

    REQUIRE(t->text() == TRAIT::latin1ToString("Item"));

    REQUIRE(li->items().at(1)->type() == MD::ItemType::Blockquote);

    auto bq = static_cast<MD::Blockquote<TRAIT> *>(li->items().at(1).get());
    REQUIRE(bq->startColumn() == 2);
    REQUIRE(bq->startLine() == 2);
    REQUIRE(bq->endColumn() == 10);
    REQUIRE(bq->endLine() == 2);
    REQUIRE(bq->delims() == MD::Blockquote<TRAIT>::Delims{{4, 2, 4, 2}});

    REQUIRE(bq->items().size() == 1);

    REQUIRE(bq->items().at(0)->type() == MD::ItemType::Paragraph);

    p = static_cast<MD::Paragraph<TRAIT> *>(bq->items().at(0).get());
    REQUIRE(p->startColumn() == 6);
    REQUIRE(p->startLine() == 2);
    REQUIRE(p->endColumn() == 10);
    REQUIRE(p->endLine() == 2);

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

    t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->startColumn() == 6);
    REQUIRE(t->startLine() == 2);
    REQUIRE(t->endColumn() == 10);
    REQUIRE(t->endLine() == 2);

    REQUIRE(t->text() == TRAIT::latin1ToString("Quote"));
}

/*
  [^footnote]: Paragraph in footnote

    Paragraph in footnote

    Paragraph in footnote

*/
TEST_CASE("045")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/045.md"), false);

    REQUIRE(!doc->isEmpty());
    REQUIRE(doc->items().size() == 1);
    REQUIRE(doc->items().at(0)->type() == MD::ItemType::Anchor);

    REQUIRE(doc->footnotesMap().size() == 1);

    typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
        QDir().absolutePath()
#else
        typename TRAIT::String(std::filesystem::canonical(std::filesystem::current_path()).u8string())
#endif
        + TRAIT::latin1ToString("/tests/parser/data/");

#ifndef MD4QT_QT_SUPPORT
    std::string tmp;
    wd.toUTF8String(tmp);
    std::replace(tmp.begin(), tmp.end(), '\\', '/');
    wd = icu::UnicodeString::fromUTF8(tmp);
#endif

    const typename TRAIT::String label = TRAIT::latin1ToString("#^footnote/") + wd + TRAIT::latin1ToString("045.md");

    const auto fit = doc->footnotesMap().find(label);

    REQUIRE(fit != doc->footnotesMap().cend());

    auto f = fit->second;

    REQUIRE(f->items().size() == 3);
    REQUIRE(f->startColumn() == 0);
    REQUIRE(f->startLine() == 0);
    REQUIRE(f->endColumn() == 21);
    REQUIRE(f->endLine() == 4);
    REQUIRE(f->idPos() == MD::WithPosition{1, 0, 12, 0});

    static const std::vector<long long int> start = {14, 4, 1};

    for (int i = 0; i < 3; ++i) {
        REQUIRE(f->items().at(i)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(f->items().at(i).get());
        REQUIRE(p->startColumn() == start.at(i));
        REQUIRE(p->startLine() == 2 * i);
        REQUIRE(p->endColumn() == start.at(i) + 20);
        REQUIRE(p->endLine() == 2 * i);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == start.at(i));
        REQUIRE(t->startLine() == 2 * i);
        REQUIRE(t->endColumn() == start.at(i) + 20);
        REQUIRE(t->endLine() == 2 * i);

        REQUIRE(t->text() == TRAIT::latin1ToString("Paragraph in footnote"));
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
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/046.md"));

    REQUIRE(!doc->isEmpty());

    typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
        QDir().absolutePath()
#else
        typename TRAIT::String(std::filesystem::canonical(std::filesystem::current_path()).u8string())
#endif
        + TRAIT::latin1ToString("/tests/parser/data/");

#ifndef MD4QT_QT_SUPPORT
    std::string tmp;
    wd.toUTF8String(tmp);
    std::replace(tmp.begin(), tmp.end(), '\\', '/');
    wd = icu::UnicodeString::fromUTF8(tmp);
#endif

    auto checkDoc = [&wd](std::shared_ptr<MD::Document<TRAIT>> doc) {
        REQUIRE(doc->items().size() == 14);

        static const std::vector<long long int> hcolumn = {0, 0, 2, 0, 0, 0};
        static const std::vector<long long int> hecolumn = {8, 8, 12, 11, 8, 8};
        static const std::vector<long long int> sline = {0, 4, 8, 12, 16, 21};
        static const std::vector<long long int> eline = {1, 5, 8, 12, 17, 22};
        static const std::vector<long long int> pcolumn = {0, 0, 4, 3, 0, 0};
        static const std::vector<long long int> pline = {2, 6, 10, 14, 19, 24};
        static const std::vector<MD::WithPosition> delims = {{0, 1, 8, 1}, {0, 5, 8, 5}, {2, 8, 2, 8}, {0, 12, 1, 12}, {0, 17, 8, 17}, {0, 22, 8, 22}};

        int didx = 1;

        for (int i = 1; i < 4; ++i) {
            for (int j = 1; j < 3; ++j) {
                REQUIRE(doc->items().at(didx)->type() == MD::ItemType::Heading);

                auto h = static_cast<MD::Heading<TRAIT> *>(doc->items().at(didx).get());
                const size_t idx = ((i - 1) * 2) + j - 1;
                REQUIRE(h->startColumn() == hcolumn.at(idx));
                REQUIRE(h->startLine() == sline.at(idx));
                REQUIRE(h->endColumn() == hecolumn.at(idx));
                REQUIRE(h->endLine() == eline.at(idx));
                REQUIRE(h->delims().size() == 1);
                REQUIRE(h->delims().front() == delims.at(idx));

                {
                    REQUIRE(h->level() == j);
                    REQUIRE(h->text().get());
                    auto p = h->text().get();
                    REQUIRE(p->startColumn() == pcolumn.at(idx));
                    REQUIRE(p->startLine() == sline.at(idx));
                    REQUIRE(p->endColumn() == pcolumn.at(idx) + 8);
                    REQUIRE(p->endLine() == p->startLine());
                    REQUIRE(p->items().size() == 1);
                    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
                    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
                    REQUIRE(t->text() == TRAIT::latin1ToString("Heading ") + to_string(j));
                    REQUIRE(t->startColumn() == pcolumn.at(idx));
                    REQUIRE(t->startLine() == sline.at(idx));
                    REQUIRE(t->endColumn() == pcolumn.at(idx) + 8);
                    REQUIRE(t->endLine() == t->startLine());
                }

                ++didx;

                REQUIRE(doc->items().at(didx)->type() == MD::ItemType::Paragraph);

                auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(didx).get());
                REQUIRE(p->startColumn() == 0);
                REQUIRE(p->startLine() == pline.at(idx));
                REQUIRE(p->endColumn() == 10);
                REQUIRE(p->endLine() == p->startLine());

                REQUIRE(p->items().size() == 1);
                REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
                REQUIRE(static_cast<MD::Text<TRAIT> *>(p->items().at(0).get())->text() == TRAIT::latin1ToString("Paragraph ") + to_string(j));

                ++didx;
            }
        }

        REQUIRE(doc->items().at(didx)->type() == MD::ItemType::Heading);

        auto h = static_cast<MD::Heading<TRAIT> *>(doc->items().at(didx).get());
        REQUIRE(h->startColumn() == 0);
        REQUIRE(h->startLine() == 26);
        REQUIRE(h->endColumn() == 25);
        REQUIRE(h->endLine() == 26);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 26, 2, 26});

        REQUIRE(h->level() == 3);
        REQUIRE(h->text().get());
        auto p = h->text().get();
        REQUIRE(p->startColumn() == 4);
        REQUIRE(p->startLine() == 26);
        REQUIRE(p->endColumn() == 12);
        REQUIRE(p->endLine() == 26);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 4);
        REQUIRE(t->startLine() == 26);
        REQUIRE(t->endColumn() == 12);
        REQUIRE(t->endLine() == 26);
        REQUIRE(t->text() == TRAIT::latin1ToString("Heading 3"));
        REQUIRE(h->isLabeled());

        const typename TRAIT::String label = TRAIT::latin1ToString("#heading-3/") + wd + TRAIT::latin1ToString("046.md");

        REQUIRE(h->label() == label);

        REQUIRE(doc->labeledHeadings().size() == 5);
        const auto hit = doc->labeledHeadings().find(label);
        REQUIRE(hit != doc->labeledHeadings().cend());
        REQUIRE(hit->second.get() == h);
    };

    checkDoc(doc);

    checkDoc(std::static_pointer_cast<MD::Document<TRAIT>>(doc->clone()));
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
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/047.md"));

    auto checkDoc = [](std::shared_ptr<MD::Document<TRAIT>> doc) {
        REQUIRE(!doc->isEmpty());
        REQUIRE(doc->items().size() == 3);

        {
            REQUIRE(doc->items().at(1)->type() == MD::ItemType::Table);

            auto t = static_cast<MD::Table<TRAIT> *>(doc->items().at(1).get());
            REQUIRE(t->startColumn() == 0);
            REQUIRE(t->startLine() == 1);
            REQUIRE(t->endColumn() == 16);
            REQUIRE(t->endLine() == 3);

            REQUIRE(t->columnsCount() == 2);
            REQUIRE(t->rows().size() == 2);

            auto r0 = t->rows().at(0);
            REQUIRE(r0->startColumn() == 0);
            REQUIRE(r0->startLine() == 1);
            REQUIRE(r0->endColumn() == 18);
            REQUIRE(r0->endLine() == r0->startLine());

            REQUIRE(r0->type() == MD::ItemType::TableRow);

            REQUIRE(r0->cells().size() == 2);

            {
                REQUIRE(r0->cells().at(0)->type() == MD::ItemType::TableCell);
                auto c0 = static_cast<MD::TableCell<TRAIT> *>(r0->cells().at(0).get());
                REQUIRE(c0->startColumn() == 0);
                REQUIRE(c0->startLine() == r0->startLine());
                REQUIRE(c0->endColumn() == 9);
                REQUIRE(c0->endLine() == c0->startLine());

                REQUIRE(c0->items().size() == 1);
                REQUIRE(c0->items().at(0)->type() == MD::ItemType::Text);

                auto t0 = static_cast<MD::Text<TRAIT> *>(c0->items().at(0).get());
                REQUIRE(t0->startColumn() == 0);
                REQUIRE(t0->startLine() == c0->startLine());
                REQUIRE(t0->endColumn() == 8);
                REQUIRE(t0->endLine() == t0->startLine());

                REQUIRE(t0->text() == TRAIT::latin1ToString("Column 1"));
            }

            {
                auto c1 = static_cast<MD::TableCell<TRAIT> *>(r0->cells().at(1).get());
                REQUIRE(c1->startColumn() == 9);
                REQUIRE(c1->startLine() == r0->startLine());
                REQUIRE(c1->endColumn() == 18);
                REQUIRE(c1->endLine() == c1->startLine());

                REQUIRE(c1->items().size() == 1);
                REQUIRE(c1->items().at(0)->type() == MD::ItemType::Text);

                auto t1 = static_cast<MD::Text<TRAIT> *>(c1->items().at(0).get());
                REQUIRE(t1->startColumn() == 10);
                REQUIRE(t1->startLine() == r0->startLine());
                REQUIRE(t1->endColumn() == 18);
                REQUIRE(t1->endLine() == c1->startLine());

                REQUIRE(t1->text() == TRAIT::latin1ToString("Column 2"));
            }

            auto r1 = t->rows().at(1);
            REQUIRE(r1->startColumn() == 0);
            REQUIRE(r1->startLine() == 3);
            REQUIRE(r1->endColumn() == 16);
            REQUIRE(r1->endLine() == r1->startLine());

            REQUIRE(r1->cells().size() == 2);

            {
                auto c0 = static_cast<MD::TableCell<TRAIT> *>(r1->cells().at(0).get());
                REQUIRE(c0->startColumn() == 0);
                REQUIRE(c0->startLine() == r1->startLine());
                REQUIRE(c0->endColumn() == 9);
                REQUIRE(c0->endLine() == c0->startLine());

                REQUIRE(c0->items().size() == 1);
                REQUIRE(c0->items().at(0)->type() == MD::ItemType::Text);

                auto t0 = static_cast<MD::Text<TRAIT> *>(c0->items().at(0).get());
                REQUIRE(t0->startColumn() == 0);
                REQUIRE(t0->startLine() == r1->startLine());
                REQUIRE(t0->endColumn() == 8);
                REQUIRE(t0->endLine() == t0->startLine());

                REQUIRE(t0->text() == TRAIT::latin1ToString("Cell 1"));
            }

            {
                auto c1 = static_cast<MD::TableCell<TRAIT> *>(r1->cells().at(1).get());
                REQUIRE(c1->startColumn() == 9);
                REQUIRE(c1->startLine() == r1->startLine());
                REQUIRE(c1->endColumn() == 16);
                REQUIRE(c1->endLine() == c1->startLine());

                REQUIRE(c1->items().size() == 1);
                REQUIRE(c1->items().at(0)->type() == MD::ItemType::Text);

                auto t1 = static_cast<MD::Text<TRAIT> *>(c1->items().at(0).get());
                REQUIRE(t1->startColumn() == 10);
                REQUIRE(t1->startLine() == r1->startLine());
                REQUIRE(t1->endColumn() == 16);
                REQUIRE(t1->endLine() == t1->startLine());

                REQUIRE(t1->text() == TRAIT::latin1ToString("Cell 2"));
            }
        }

        {
            REQUIRE(doc->items().at(2)->type() == MD::ItemType::Table);

            auto t = static_cast<MD::Table<TRAIT> *>(doc->items().at(2).get());
            REQUIRE(t->startColumn() == 0);
            REQUIRE(t->startLine() == 5);
            REQUIRE(t->endColumn() == 22);
            REQUIRE(t->endLine() == 7);

            REQUIRE(t->columnsCount() == 2);
            REQUIRE(t->rows().size() == 2);

            auto r0 = t->rows().at(0);
            REQUIRE(r0->startColumn() == 0);
            REQUIRE(r0->startLine() == 5);
            REQUIRE(r0->endColumn() == 22);
            REQUIRE(r0->endLine() == r0->startLine());

            REQUIRE(r0->type() == MD::ItemType::TableRow);

            REQUIRE(r0->cells().size() == 2);

            {
                REQUIRE(r0->cells().at(0)->type() == MD::ItemType::TableCell);
                auto c0 = static_cast<MD::TableCell<TRAIT> *>(r0->cells().at(0).get());
                REQUIRE(c0->startColumn() == 0);
                REQUIRE(c0->startLine() == r0->startLine());
                REQUIRE(c0->endColumn() == 11);
                REQUIRE(c0->endLine() == c0->startLine());

                REQUIRE(c0->items().size() == 1);
                REQUIRE(c0->items().at(0)->type() == MD::ItemType::Text);

                auto t0 = static_cast<MD::Text<TRAIT> *>(c0->items().at(0).get());
                REQUIRE(t0->startColumn() == 1);
                REQUIRE(t0->startLine() == c0->startLine());
                REQUIRE(t0->endColumn() == 10);
                REQUIRE(t0->endLine() == t0->startLine());

                REQUIRE(t0->text() == TRAIT::latin1ToString("Column 1"));
            }

            {
                auto c1 = static_cast<MD::TableCell<TRAIT> *>(r0->cells().at(1).get());
                REQUIRE(c1->startColumn() == 11);
                REQUIRE(c1->startLine() == r0->startLine());
                REQUIRE(c1->endColumn() == 22);
                REQUIRE(c1->endLine() == c1->startLine());

                REQUIRE(c1->items().size() == 1);
                REQUIRE(c1->items().at(0)->type() == MD::ItemType::Text);

                auto t1 = static_cast<MD::Text<TRAIT> *>(c1->items().at(0).get());
                REQUIRE(t1->startColumn() == 12);
                REQUIRE(t1->startLine() == r0->startLine());
                REQUIRE(t1->endColumn() == 21);
                REQUIRE(t1->endLine() == c1->startLine());

                REQUIRE(t1->text() == TRAIT::latin1ToString("Column 2"));
            }

            auto r1 = t->rows().at(1);
            REQUIRE(r1->startColumn() == 0);
            REQUIRE(r1->startLine() == 7);
            REQUIRE(r1->endColumn() == 22);
            REQUIRE(r1->endLine() == r1->startLine());

            REQUIRE(r1->cells().size() == 2);

            {
                auto c0 = static_cast<MD::TableCell<TRAIT> *>(r1->cells().at(0).get());
                REQUIRE(c0->startColumn() == 0);
                REQUIRE(c0->startLine() == r1->startLine());
                REQUIRE(c0->endColumn() == 11);
                REQUIRE(c0->endLine() == c0->startLine());

                REQUIRE(c0->items().size() == 1);
                REQUIRE(c0->items().at(0)->type() == MD::ItemType::Text);

                auto t0 = static_cast<MD::Text<TRAIT> *>(c0->items().at(0).get());
                REQUIRE(t0->startColumn() == 1);
                REQUIRE(t0->startLine() == r1->startLine());
                REQUIRE(t0->endColumn() == 10);
                REQUIRE(t0->endLine() == t0->startLine());

                REQUIRE(t0->text() == TRAIT::latin1ToString("Cell 1"));
            }

            {
                auto c1 = static_cast<MD::TableCell<TRAIT> *>(r1->cells().at(1).get());
                REQUIRE(c1->startColumn() == 11);
                REQUIRE(c1->startLine() == r1->startLine());
                REQUIRE(c1->endColumn() == 22);
                REQUIRE(c1->endLine() == c1->startLine());

                REQUIRE(c1->items().size() == 1);
                REQUIRE(c1->items().at(0)->type() == MD::ItemType::Text);

                auto t1 = static_cast<MD::Text<TRAIT> *>(c1->items().at(0).get());
                REQUIRE(t1->startColumn() == 12);
                REQUIRE(t1->startLine() == r1->startLine());
                REQUIRE(t1->endColumn() == 21);
                REQUIRE(t1->endLine() == t1->startLine());

                REQUIRE(t1->text() == TRAIT::latin1ToString("Cell 2"));
            }
        }

        auto table = static_cast<MD::Table<TRAIT> *>(doc->items().at(1).get());

        REQUIRE(table->columnAlignment(0) == MD::Table<TRAIT>::AlignLeft);
        REQUIRE(table->columnAlignment(1) == MD::Table<TRAIT>::AlignLeft);

        table = static_cast<MD::Table<TRAIT> *>(doc->items().at(2).get());

        REQUIRE(table->columnAlignment(0) == MD::Table<TRAIT>::AlignCenter);
        REQUIRE(table->columnAlignment(1) == MD::Table<TRAIT>::AlignRight);
    };

    checkDoc(doc);

    checkDoc(std::static_pointer_cast<MD::Document<TRAIT>>(doc->clone()));
}

/*
# Heading
Paragraph
# Heading

*/
TEST_CASE("048")
{
    MD::Parser<TRAIT> parser;
    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/048.md"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(0)->type() == MD::ItemType::Anchor);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(h->startColumn() == 0);
        REQUIRE(h->startLine() == 0);
        REQUIRE(h->endColumn() == 8);
        REQUIRE(h->endLine() == 0);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 0, 0, 0});

        REQUIRE(h->text().get());
        auto p = h->text().get();
        REQUIRE(p->startColumn() == 2);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 8);
        REQUIRE(p->endLine() == 0);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("Heading"));
        REQUIRE(t->startColumn() == 2);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 8);
        REQUIRE(t->endLine() == 0);
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto *p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(2).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 1);
        REQUIRE(p->endColumn() == 8);
        REQUIRE(p->endLine() == 1);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(static_cast<MD::Text<TRAIT> *>(p->items().at(0).get())->text() == TRAIT::latin1ToString("Paragraph"));
    }

    REQUIRE(doc->items().at(3)->type() == MD::ItemType::Heading);
    auto h = static_cast<MD::Heading<TRAIT> *>(doc->items().at(3).get());
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 2);
    REQUIRE(h->endColumn() == 8);
    REQUIRE(h->endLine() == 2);
    REQUIRE(h->delims().size() == 1);
    REQUIRE(h->delims().front() == MD::WithPosition{0, 2, 0, 2});

    REQUIRE(h->text().get());
    auto p = h->text().get();
    REQUIRE(p->startColumn() == 2);
    REQUIRE(p->startLine() == 2);
    REQUIRE(p->endColumn() == 8);
    REQUIRE(p->endLine() == 2);

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->startColumn() == 2);
    REQUIRE(t->startLine() == 2);
    REQUIRE(t->endColumn() == 8);
    REQUIRE(t->endLine() == 2);
    REQUIRE(t->text() == TRAIT::latin1ToString("Heading"));
}

/*
<!-- --> 1<!-- -->

*/
TEST_CASE("049")
{
    MD::Parser<TRAIT> parser;
    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/049.md"));

    auto checkDoc = [](std::shared_ptr<MD::Document<TRAIT>> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);

        REQUIRE(doc->items().at(0)->type() == MD::ItemType::Anchor);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(h->text() == TRAIT::latin1ToString("<!-- --> 1<!-- -->"));
        REQUIRE(h->startColumn() == 0);
        REQUIRE(h->startLine() == 0);
        REQUIRE(h->endColumn() == 17);
        REQUIRE(h->endLine() == 0);
    };

    checkDoc(doc);

    checkDoc(std::static_pointer_cast<MD::Document<TRAIT>>(doc->clone()));
}

/*
[a\]](#1)
[b\\](#2)
[c\-d](#3)
[\\](#4)

*/
TEST_CASE("050")
{
    MD::Parser<TRAIT> parser;
    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/050.md"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto *p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 4);
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 7);
    REQUIRE(p->endLine() == 3);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l0 = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l0->startColumn() == 0);
    REQUIRE(l0->startLine() == 0);
    REQUIRE(l0->endColumn() == 8);
    REQUIRE(l0->endLine() == 0);
    REQUIRE(l0->text() == TRAIT::latin1ToString("a]"));
    REQUIRE(l0->textPos() == MD::WithPosition{1, 0, 3, 0});
    REQUIRE(l0->urlPos() == MD::WithPosition{6, 0, 7, 0});

    REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
    auto l1 = static_cast<MD::Link<TRAIT> *>(p->items().at(1).get());
    REQUIRE(l1->startColumn() == 0);
    REQUIRE(l1->startLine() == 1);
    REQUIRE(l1->endColumn() == 8);
    REQUIRE(l1->endLine() == 1);
    REQUIRE(l1->text() == TRAIT::latin1ToString("b\\"));
    REQUIRE(l1->textPos() == MD::WithPosition{1, 1, 3, 1});
    REQUIRE(l1->urlPos() == MD::WithPosition{6, 1, 7, 1});

    REQUIRE(p->items().at(2)->type() == MD::ItemType::Link);
    auto l2 = static_cast<MD::Link<TRAIT> *>(p->items().at(2).get());
    REQUIRE(l2->startColumn() == 0);
    REQUIRE(l2->startLine() == 2);
    REQUIRE(l2->endColumn() == 9);
    REQUIRE(l2->endLine() == 2);
    REQUIRE(l2->text() == TRAIT::latin1ToString("c-d"));
    REQUIRE(l2->textPos() == MD::WithPosition{1, 2, 4, 2});
    REQUIRE(l2->urlPos() == MD::WithPosition{7, 2, 8, 2});

    REQUIRE(p->items().at(3)->type() == MD::ItemType::Link);
    auto l3 = static_cast<MD::Link<TRAIT> *>(p->items().at(3).get());
    REQUIRE(l3->startColumn() == 0);
    REQUIRE(l3->startLine() == 3);
    REQUIRE(l3->endColumn() == 7);
    REQUIRE(l3->endLine() == 3);
    REQUIRE(l3->text() == TRAIT::latin1ToString("\\"));
    REQUIRE(l3->textPos() == MD::WithPosition{1, 3, 2, 3});
    REQUIRE(l3->urlPos() == MD::WithPosition{5, 3, 6, 3});
}

/*
[link1](051-1.md)
[link2](051-2.md)

*/
TEST_CASE("051")
{
    MD::Parser<TRAIT> parser;
    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/051.md"));

    typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
        QDir().absolutePath()
#else
        typename TRAIT::String(std::filesystem::canonical(std::filesystem::current_path()).u8string())
#endif
        + TRAIT::latin1ToString("/tests/parser/data");

#ifndef MD4QT_QT_SUPPORT
    std::string tmp;
    wd.toUTF8String(tmp);
    std::replace(tmp.begin(), tmp.end(), '\\', '/');
    wd = icu::UnicodeString::fromUTF8(tmp);
#endif

    REQUIRE(doc->items().size() == 8);

    REQUIRE(static_cast<MD::Anchor<TRAIT> *>(doc->items().at(0).get())->label() == wd + TRAIT::latin1ToString("/051.md"));

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::PageBreak);

    REQUIRE(static_cast<MD::Anchor<TRAIT> *>(doc->items().at(3).get())->label() == wd + TRAIT::latin1ToString("/051-1.md"));

    REQUIRE(doc->items().at(4)->type() == MD::ItemType::Paragraph);
    REQUIRE(doc->items().at(5)->type() == MD::ItemType::PageBreak);

    REQUIRE(static_cast<MD::Anchor<TRAIT> *>(doc->items().at(6).get())->label() == wd + TRAIT::latin1ToString("/051-2.md"));

    REQUIRE(doc->items().at(7)->type() == MD::ItemType::Paragraph);
}

/*
```code *bold _italic
_```` *bold* _italic
```

*/
TEST_CASE("052")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/052.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

        auto c = static_cast<MD::Code<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(c->startColumn() == 0);
        REQUIRE(c->startLine() == 1);
        REQUIRE(c->endColumn() == 19);
        REQUIRE(c->endLine() == 1);
        REQUIRE(c->startDelim() == MD::WithPosition{0, 0, 2, 0});
        REQUIRE(c->endDelim() == MD::WithPosition{0, 2, 2, 2});
        REQUIRE(c->syntaxPos() == MD::WithPosition{3, 0, 6, 0});

        REQUIRE(c->isInline() == false);
        REQUIRE(c->text() == TRAIT::latin1ToString("_```` *bold* _italic"));
        REQUIRE(c->syntax() == TRAIT::latin1ToString("code"));
    }
}

/*
```cpp
```

*/
TEST_CASE("053")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/053.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(c->startColumn() == 6);
        REQUIRE(c->startLine() == 0);
        REQUIRE(c->endColumn() == 6);
        REQUIRE(c->endLine() == 0);
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
    MD::Parser<TRAIT> parser;

    const auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/054.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(c->syntax() == TRAIT::latin1ToString("java"));
        REQUIRE(!c->isInline());
        REQUIRE(c->text().isEmpty());
        REQUIRE(c->startColumn() == 7);
        REQUIRE(c->startLine() == 0);
        REQUIRE(c->endColumn() == 7);
        REQUIRE(c->endLine() == 0);
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
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/055.md"), false);

    typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
        QDir().absolutePath()
#else
        typename TRAIT::String(std::filesystem::canonical(std::filesystem::current_path()).u8string())
#endif
        + TRAIT::latin1ToString("/tests/parser/data/");

#ifndef MD4QT_QT_SUPPORT
    std::string tmp;
    wd.toUTF8String(tmp);
    std::replace(tmp.begin(), tmp.end(), '\\', '/');
    wd = icu::UnicodeString::fromUTF8(tmp);
#endif

    auto checkDoc = [&wd](std::shared_ptr<MD::Document<TRAIT>> doc) {
        REQUIRE(doc->type() == MD::ItemType::Document);

        REQUIRE(!doc->isEmpty());
        REQUIRE(doc->items().size() == 2);
        REQUIRE(doc->items().at(0)->type() == MD::ItemType::Anchor);

        REQUIRE(doc->footnotesMap().size() == 1);

        const typename TRAIT::String label = TRAIT::latin1ToString("#^footnote/") + wd + TRAIT::latin1ToString("055.md");

        const auto fit = doc->footnotesMap().find(label);
        REQUIRE(fit != doc->footnotesMap().cend());

        auto f = fit->second;

        REQUIRE(f->type() == MD::ItemType::Footnote);

        REQUIRE(f->items().size() == 3);
        REQUIRE(f->startColumn() == 0);
        REQUIRE(f->startLine() == 0);
        REQUIRE(f->endColumn() == 21);
        REQUIRE(f->endLine() == 4);
        REQUIRE(f->idPos() == MD::WithPosition{1, 0, 12, 0});

        static const std::vector<long long int> start = {14, 4, 1};

        for (int i = 0; i < 3; ++i) {
            REQUIRE(f->items().at(i)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph<TRAIT> *>(f->items().at(i).get());
            REQUIRE(p->startColumn() == start.at(i));
            REQUIRE(p->startLine() == 2 * i);
            REQUIRE(p->endColumn() == start.at(i) + 20);
            REQUIRE(p->endLine() == 2 * i);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());

            REQUIRE(t->startColumn() == start.at(i));
            REQUIRE(t->startLine() == 2 * i);
            REQUIRE(t->endColumn() == start.at(i) + 20);
            REQUIRE(t->endLine() == 2 * i);

            REQUIRE(t->text() == TRAIT::latin1ToString("Paragraph in footnote"));
        }

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 6);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 6);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 6);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 6);

        REQUIRE(t->text() == TRAIT::latin1ToString("Text"));
    };

    checkDoc(doc);

    checkDoc(std::static_pointer_cast<MD::Document<TRAIT>>(doc->clone()));
}

/*
    if( a > b )

      do_something();

    else

      dont_do_anything();

*/
TEST_CASE("056")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/056.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(c->startColumn() == 4);
    REQUIRE(c->startLine() == 0);
    REQUIRE(c->endColumn() == 24);
    REQUIRE(c->endLine() == 6);

    REQUIRE(c->isInline() == false);
    REQUIRE(c->text() == TRAIT::latin1ToString("if( a > b )\n\n  do_something();\n\nelse\n\n  dont_do_anything();"));
}

/*
```
code

code
```

*/
TEST_CASE("057")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/057.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(c->startColumn() == 0);
    REQUIRE(c->startLine() == 1);
    REQUIRE(c->endColumn() == 3);
    REQUIRE(c->endLine() == 3);
    REQUIRE(c->startDelim() == MD::WithPosition{0, 0, 2, 0});
    REQUIRE(c->endDelim() == MD::WithPosition{0, 4, 2, 4});
    REQUIRE(c->syntaxPos() == MD::WithPosition{-1, -1, -1, -1});

    REQUIRE(c->isInline() == false);
    REQUIRE(c->text() == TRAIT::latin1ToString("code\n\ncode"));
}

/*
 * List 1

   Paragraph

 *  List 2

    Paragraph

*/
TEST_CASE("058")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/058.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(l->startColumn() == 1);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 12);
    REQUIRE(l->endLine() == 6);

    REQUIRE(l->items().size() == 2);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);

    auto li = static_cast<MD::ListItem<TRAIT> *>(l->items().at(1).get());
    REQUIRE(li->startColumn() == 1);
    REQUIRE(li->startLine() == 4);
    REQUIRE(li->endColumn() == 12);
    REQUIRE(li->endLine() == 6);
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
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/059.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);

    auto b = static_cast<MD::Blockquote<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(b->startColumn() == 0);
    REQUIRE(b->startLine() == 0);
    REQUIRE(b->endColumn() == 57);
    REQUIRE(b->endLine() == 7);
    REQUIRE(b->delims()
            == MD::Blockquote<TRAIT>::Delims{{0, 0, 0, 0}, {0, 1, 0, 1}, {0, 2, 0, 2}, {0, 3, 0, 3}, {0, 4, 0, 4}, {0, 5, 0, 5}, {0, 6, 0, 6}, {0, 7, 0, 7}});

    REQUIRE(b->items().size() == 4);

    REQUIRE(b->items().at(0)->type() == MD::ItemType::Heading);
    {
        auto i = static_cast<MD::Heading<TRAIT> *>(b->items().at(0).get());
        REQUIRE(i->startColumn() == 2);
        REQUIRE(i->startLine() == 0);
        REQUIRE(i->endColumn() == 21);
        REQUIRE(i->endLine() == 0);
        REQUIRE(i->delims().size() == 1);
        REQUIRE(i->delims().front() == MD::WithPosition{2, 0, 3, 0});
    }
    REQUIRE(b->items().at(1)->type() == MD::ItemType::List);
    {
        auto i = static_cast<MD::List<TRAIT> *>(b->items().at(1).get());
        REQUIRE(i->startColumn() == 2);
        REQUIRE(i->startLine() == 2);
        REQUIRE(i->endColumn() == 35);
        REQUIRE(i->endLine() == 3);

        REQUIRE(i->items().size() == 2);

        for (int j = 0; j < 2; ++j) {
            REQUIRE(i->items().at(j)->type() == MD::ItemType::ListItem);
            auto li = static_cast<MD::ListItem<TRAIT> *>(i->items().at(j).get());
            REQUIRE(li->delim() == MD::WithPosition{2, 2 + j, 3, 2 + j});
        }
    }
    REQUIRE(b->items().at(2)->type() == MD::ItemType::Paragraph);
    {
        auto i = static_cast<MD::Paragraph<TRAIT> *>(b->items().at(2).get());
        REQUIRE(i->startColumn() == 2);
        REQUIRE(i->startLine() == 5);
        REQUIRE(i->endColumn() == 26);
        REQUIRE(i->endLine() == 5);
    }
    REQUIRE(b->items().at(3)->type() == MD::ItemType::Code);
    {
        auto i = static_cast<MD::Code<TRAIT> *>(b->items().at(3).get());
        REQUIRE(i->startColumn() == 6);
        REQUIRE(i->startLine() == 7);
        REQUIRE(i->endColumn() == 57);
        REQUIRE(i->endLine() == 7);
    }
}

/*
 * List
   * List

*/
TEST_CASE("060")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/060.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(l->startColumn() == 1);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 8);
    REQUIRE(l->endLine() == 1);

    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto li = static_cast<MD::ListItem<TRAIT> *>(l->items().at(0).get());
    REQUIRE(li->startColumn() == 1);
    REQUIRE(li->startLine() == 0);
    REQUIRE(li->endColumn() == 8);
    REQUIRE(li->endLine() == 1);
    REQUIRE(li->delim() == MD::WithPosition{1, 0, 1, 0});

    REQUIRE(li->items().size() == 2);

    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::List);

    {
        auto l = static_cast<MD::List<TRAIT> *>(li->items().at(1).get());
        REQUIRE(l->items().size() == 1);
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto nli = static_cast<MD::ListItem<TRAIT> *>(l->items().at(0).get());
        REQUIRE(nli->delim() == MD::WithPosition{3, 1, 3, 1});
    }
}
