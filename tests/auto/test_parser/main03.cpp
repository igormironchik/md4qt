/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include "parser.h"

inline QString to_string(int i)
{
    return QString::number(i);
}

/*
*   A list item with a code block:

        <code goes here>

*/
TEST_CASE("061")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/061.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->startColumn() == 0);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 23);
    REQUIRE(l->endLine() == 2);

    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->startColumn() == 0);
    REQUIRE(li->startLine() == 0);
    REQUIRE(li->endColumn() == 23);
    REQUIRE(li->endLine() == 2);
    REQUIRE(li->delim() == MD::WithPosition{0, 0, 0, 0});

    REQUIRE(li->items().size() == 2);

    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(li->items().at(0).get());
    REQUIRE(p->startColumn() == 4);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 33);
    REQUIRE(p->endLine() == 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->startColumn() == 4);
    REQUIRE(t->startLine() == 0);
    REQUIRE(t->endColumn() == 33);
    REQUIRE(t->endLine() == 0);
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("A list item with a code block:"));

    REQUIRE(li->items().at(1)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(li->items().at(1).get());
    REQUIRE(c->startColumn() == 8);
    REQUIRE(c->startLine() == 2);
    REQUIRE(c->endColumn() == 23);
    REQUIRE(c->endLine() == 2);
    REQUIRE(c->isInline() == false);
    REQUIRE(c->syntax().isEmpty());
    REQUIRE(c->text() == QStringLiteral("<code goes here>"));
}

/*
**`**`*

**``**``*

**``**`**``*

**`*`**`*

*/
TEST_CASE("062")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/062.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 5);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 6);
        REQUIRE(p->endLine() == 0);

        REQUIRE(p->items().size() == 2);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t1 = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t1->startColumn() == 0);
        REQUIRE(t1->startLine() == 0);
        REQUIRE(t1->endColumn() == 0);
        REQUIRE(t1->endLine() == 0);
        REQUIRE(t1->text() == QStringLiteral("*"));

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Code);
        auto c1 = static_cast<MD::Code *>(p->items().at(1).get());
        REQUIRE(c1->startColumn() == 3);
        REQUIRE(c1->startLine() == 0);
        REQUIRE(c1->endColumn() == 4);
        REQUIRE(c1->endLine() == 0);
        REQUIRE(c1->startDelim() == MD::WithPosition{2, 0, 2, 0});
        REQUIRE(c1->endDelim() == MD::WithPosition{5, 0, 5, 0});
        REQUIRE(c1->text() == QStringLiteral("**"));
        REQUIRE(c1->openStyles().size() == 1);
        REQUIRE(c1->openStyles().at(0) == MD::StyleDelim{MD::ItalicText, 1, 0, 1, 0});
        REQUIRE(c1->closeStyles().size() == 1);
        REQUIRE(c1->closeStyles().at(0) == MD::StyleDelim{MD::ItalicText, 6, 0, 6, 0});
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 2);
        REQUIRE(p->endColumn() == 8);
        REQUIRE(p->endLine() == 2);

        REQUIRE(p->items().size() == 2);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t1 = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t1->startColumn() == 0);
        REQUIRE(t1->startLine() == 2);
        REQUIRE(t1->endColumn() == 0);
        REQUIRE(t1->endLine() == 2);
        REQUIRE(t1->text() == QStringLiteral("*"));

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Code);
        auto c1 = static_cast<MD::Code *>(p->items().at(1).get());
        REQUIRE(c1->startColumn() == 4);
        REQUIRE(c1->startLine() == 2);
        REQUIRE(c1->endColumn() == 5);
        REQUIRE(c1->endLine() == 2);
        REQUIRE(c1->startDelim() == MD::WithPosition{2, 2, 3, 2});
        REQUIRE(c1->endDelim() == MD::WithPosition{6, 2, 7, 2});
        REQUIRE(c1->text() == QStringLiteral("**"));
        REQUIRE(c1->openStyles().size() == 1);
        REQUIRE(c1->openStyles().at(0) == MD::StyleDelim{MD::ItalicText, 1, 2, 1, 2});
        REQUIRE(c1->closeStyles().size() == 1);
        REQUIRE(c1->closeStyles().at(0) == MD::StyleDelim{MD::ItalicText, 8, 2, 8, 2});
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 4);
        REQUIRE(p->endColumn() == 11);
        REQUIRE(p->endLine() == 4);

        REQUIRE(p->items().size() == 2);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t1 = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t1->startColumn() == 0);
        REQUIRE(t1->startLine() == 4);
        REQUIRE(t1->endColumn() == 0);
        REQUIRE(t1->endLine() == 4);
        REQUIRE(t1->text() == QStringLiteral("*"));

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Code);
        auto c1 = static_cast<MD::Code *>(p->items().at(1).get());
        REQUIRE(c1->startColumn() == 4);
        REQUIRE(c1->startLine() == 4);
        REQUIRE(c1->endColumn() == 8);
        REQUIRE(c1->endLine() == 4);
        REQUIRE(c1->startDelim() == MD::WithPosition{2, 4, 3, 4});
        REQUIRE(c1->endDelim() == MD::WithPosition{9, 4, 10, 4});
        REQUIRE(c1->text() == QStringLiteral("**`**"));
        REQUIRE(c1->openStyles().size() == 1);
        REQUIRE(c1->openStyles().at(0) == MD::StyleDelim{MD::ItalicText, 1, 4, 1, 4});
        REQUIRE(c1->closeStyles().size() == 1);
        REQUIRE(c1->closeStyles().at(0) == MD::StyleDelim{MD::ItalicText, 11, 4, 11, 4});
    }

    {
        REQUIRE(doc->items().at(4)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(4).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 6);
        REQUIRE(p->endColumn() == 8);
        REQUIRE(p->endLine() == 6);

        REQUIRE(p->items().size() == 2);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
        auto c1 = static_cast<MD::Code *>(p->items().at(0).get());
        REQUIRE(c1->startColumn() == 3);
        REQUIRE(c1->startLine() == 6);
        REQUIRE(c1->endColumn() == 3);
        REQUIRE(c1->endLine() == 6);
        REQUIRE(c1->startDelim() == MD::WithPosition{2, 6, 2, 6});
        REQUIRE(c1->endDelim() == MD::WithPosition{4, 6, 4, 6});
        REQUIRE(c1->text() == QStringLiteral("*"));
        REQUIRE(c1->openStyles().size() == 1);
        REQUIRE(c1->openStyles().at(0) == MD::StyleDelim{MD::BoldText, 0, 6, 1, 6});
        REQUIRE(c1->closeStyles().size() == 1);
        REQUIRE(c1->closeStyles().at(0) == MD::StyleDelim{MD::BoldText, 5, 6, 6, 6});

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t1 = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t1->startColumn() == 7);
        REQUIRE(t1->startLine() == 6);
        REQUIRE(t1->endColumn() == 8);
        REQUIRE(t1->endLine() == 6);
        REQUIRE(t1->text() == QStringLiteral("`*"));
        REQUIRE(t1->openStyles().empty());
        REQUIRE(t1->closeStyles().empty());
    }
}

/*
_**Lorem.**_ Aenean

*/
TEST_CASE("063")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/063.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 18);
    REQUIRE(p->endLine() == 0);
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 3);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 8);
        REQUIRE(t->endLine() == 0);
        REQUIRE(t->opts() == (MD::ItalicText | MD::BoldText));
        REQUIRE(t->text() == QStringLiteral("Lorem."));
        REQUIRE(t->openStyles().size() == 2);
        REQUIRE(t->openStyles().at(0) == MD::StyleDelim{MD::ItalicText, 0, 0, 0, 0});
        REQUIRE(t->openStyles().at(1) == MD::StyleDelim{MD::BoldText, 1, 0, 2, 0});
        REQUIRE(t->closeStyles().size() == 2);
        REQUIRE(t->closeStyles().at(0) == MD::StyleDelim{MD::BoldText, 9, 0, 10, 0});
        REQUIRE(t->closeStyles().at(1) == MD::StyleDelim{MD::ItalicText, 11, 0, 11, 0});
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->startColumn() == 12);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 18);
        REQUIRE(t->endLine() == 0);
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral(" Aenean"));
    }
}

/*
\\`

*/
TEST_CASE("064")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/064.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 2);
    REQUIRE(p->endLine() == 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->startColumn() == 0);
    REQUIRE(t->startLine() == 0);
    REQUIRE(t->endColumn() == 2);
    REQUIRE(t->endLine() == 0);
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("\\`"));
}

/*
When $a \ne 0$, there are two solutions to $(ax^2 + bx + c = 0)$ and they are
$$ x = {-b \pm \sqrt{b^2-4ac} \over 2a} $$

*/
TEST_CASE("065")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/065.md"));

    auto checkDoc = [](QSharedPointer<MD::Document> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 41);
        REQUIRE(p->endLine() == 1);

        REQUIRE(p->items().size() == 6);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->startColumn() == 0);
            REQUIRE(t->startLine() == 0);
            REQUIRE(t->endColumn() == 4);
            REQUIRE(t->endLine() == 0);
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("When "));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Math);
            auto m = static_cast<MD::Math *>(p->items().at(1).get());
            REQUIRE(m->startColumn() == 6);
            REQUIRE(m->startLine() == 0);
            REQUIRE(m->endColumn() == 12);
            REQUIRE(m->endLine() == 0);
            REQUIRE(m->expr() == QStringLiteral("a \\ne 0"));
            REQUIRE(m->isInline());
            REQUIRE(m->startDelim() == MD::WithPosition{5, 0, 5, 0});
            REQUIRE(m->endDelim() == MD::WithPosition{13, 0, 13, 0});
        }

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(2).get());
            REQUIRE(t->startColumn() == 14);
            REQUIRE(t->startLine() == 0);
            REQUIRE(t->endColumn() == 42);
            REQUIRE(t->endLine() == 0);
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral(", there are two solutions to "));
        }

        {
            REQUIRE(p->items().at(3)->type() == MD::ItemType::Math);
            auto m = static_cast<MD::Math *>(p->items().at(3).get());
            REQUIRE(m->startColumn() == 44);
            REQUIRE(m->startLine() == 0);
            REQUIRE(m->endColumn() == 62);
            REQUIRE(m->endLine() == 0);
            REQUIRE(m->expr() == QStringLiteral("(ax^2 + bx + c = 0)"));
            REQUIRE(m->isInline());
            REQUIRE(m->startDelim() == MD::WithPosition{43, 0, 43, 0});
            REQUIRE(m->endDelim() == MD::WithPosition{63, 0, 63, 0});
        }

        {
            REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(4).get());
            REQUIRE(t->startColumn() == 64);
            REQUIRE(t->startLine() == 0);
            REQUIRE(t->endColumn() == 76);
            REQUIRE(t->endLine() == 0);
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral(" and they are"));
        }

        {
            REQUIRE(p->items().at(5)->type() == MD::ItemType::Math);
            auto m = static_cast<MD::Math *>(p->items().at(5).get());
            REQUIRE(m->startColumn() == 2);
            REQUIRE(m->startLine() == 1);
            REQUIRE(m->endColumn() == 39);
            REQUIRE(m->endLine() == 1);
            REQUIRE(m->expr() == QStringLiteral(" x = {-b \\pm \\sqrt{b^2-4ac} \\over 2a} "));
            REQUIRE(!m->isInline());
            REQUIRE(m->startDelim() == MD::WithPosition{0, 1, 1, 1});
            REQUIRE(m->endDelim() == MD::WithPosition{40, 1, 41, 1});
        }
    };

    checkDoc(doc);

    checkDoc(doc->clone().staticCast<MD::Document>());
}

/*
```math
x = {-b \pm \sqrt{b^2-4ac} \over 2a}
```

*/
TEST_CASE("066")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/066.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 2);
    REQUIRE(p->endLine() == 2);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Math);
    auto m = static_cast<MD::Math *>(p->items().at(0).get());
    REQUIRE(m->startColumn() == 0);
    REQUIRE(m->startLine() == 1);
    REQUIRE(m->endColumn() == 35);
    REQUIRE(m->endLine() == 1);
    REQUIRE(m->expr() == QStringLiteral("x = {-b \\pm \\sqrt{b^2-4ac} \\over 2a}"));
    REQUIRE(!m->isInline());
    REQUIRE(m->startDelim() == MD::WithPosition{0, 0, 2, 0});
    REQUIRE(m->endDelim() == MD::WithPosition{0, 2, 2, 2});
    REQUIRE(m->syntaxPos() == MD::WithPosition{3, 0, 6, 0});
}

/*
Paragraph

 * List
item

   ```
   Code
   ```

*/
TEST_CASE("067")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/067.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 8);
        REQUIRE(p->endLine() == 0);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 8);
        REQUIRE(t->endLine() == 0);

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Paragraph"));
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(2).get());
    REQUIRE(l->startColumn() == 1);
    REQUIRE(l->startLine() == 2);
    REQUIRE(l->endColumn() == 5);
    REQUIRE(l->endLine() == 7);

    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto item = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(item->startColumn() == 1);
    REQUIRE(item->startLine() == 2);
    REQUIRE(item->endColumn() == 5);
    REQUIRE(item->endLine() == 7);
    REQUIRE(item->delim() == MD::WithPosition{1, 2, 1, 2});

    REQUIRE(item->listType() == MD::ListItem::Unordered);

    REQUIRE(item->items().size() == 2);

    {
        REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
        REQUIRE(p->startColumn() == 3);
        REQUIRE(p->startLine() == 2);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 3);

        REQUIRE(p->items().size() == 2);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->startColumn() == 3);
            REQUIRE(t->startLine() == 2);
            REQUIRE(t->endColumn() == 6);
            REQUIRE(t->endLine() == 2);

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("List"));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(1).get());
            REQUIRE(t->startColumn() == 0);
            REQUIRE(t->startLine() == 3);
            REQUIRE(t->endColumn() == 3);
            REQUIRE(t->endLine() == 3);

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("item"));
        }
    }

    {
        REQUIRE(item->items().at(1)->type() == MD::ItemType::Code);

        auto c = static_cast<MD::Code *>(item->items().at(1).get());
        REQUIRE(c->startColumn() == 3);
        REQUIRE(c->startLine() == 6);
        REQUIRE(c->endColumn() == 6);
        REQUIRE(c->endLine() == 6);
        REQUIRE(c->startDelim() == MD::WithPosition{3, 5, 5, 5});
        REQUIRE(c->endDelim() == MD::WithPosition{3, 7, 5, 7});
        REQUIRE(c->syntaxPos() == MD::WithPosition{-1, -1, -1, -1});

        REQUIRE(c->text() == QStringLiteral("Code"));
    }
}

/*
<!-- Not
finished
HTML
comment

*/
TEST_CASE("068")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/068.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 0);
    REQUIRE(h->endLine() == 4);
    REQUIRE(h->text() == QStringLiteral("<!-- Not\nfinished\nHTML\ncomment\n"));
}

/*
<pre>

data

</pre>

*/
TEST_CASE("069")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/069.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<pre>\n\ndata\n\n</pre>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 5);
    REQUIRE(h->endLine() == 4);
}

/*
Text <pre>

data

</pre>

*/
TEST_CASE("070")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/070.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 9);
        REQUIRE(p->endLine() == 0);
        REQUIRE(p->items().size() == 2);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text "));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 4);
        REQUIRE(t->endLine() == 0);

        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<pre>"));
        REQUIRE(h->startColumn() == 5);
        REQUIRE(h->startLine() == 0);
        REQUIRE(h->endColumn() == 9);
        REQUIRE(h->endLine() == 0);
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 2);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 2);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("data"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 2);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 2);
    }

    REQUIRE(doc->items().at(3)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(3).get());
    REQUIRE(h->text() == QStringLiteral("</pre>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 4);
    REQUIRE(h->endColumn() == 5);
    REQUIRE(h->endLine() == 4);
}

/*
<pre>

data

</pre> Text

*/
TEST_CASE("071")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/071.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<pre>\n\ndata\n\n</pre> Text"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 10);
    REQUIRE(h->endLine() == 4);
}

/*
<pre>

data

</pre>
Text

*/
TEST_CASE("072")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/072.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<pre>\n\ndata\n\n</pre>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 5);
    REQUIRE(h->endLine() == 4);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 5);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 5);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    REQUIRE(t->startColumn() == 0);
    REQUIRE(t->startLine() == 5);
    REQUIRE(t->endColumn() == 3);
    REQUIRE(t->endLine() == 5);
}

/*
<pre>abc</pre>
Text

*/
TEST_CASE("073")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/073.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<pre>abc</pre>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 13);
    REQUIRE(h->endLine() == 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 1);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 1);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->startColumn() == 0);
    REQUIRE(t->startLine() == 1);
    REQUIRE(t->endColumn() == 3);
    REQUIRE(t->endLine() == 1);
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
}

/*
Text
<pre>abc</pre>

*/
TEST_CASE("074")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/074.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    REQUIRE(t->startColumn() == 0);
    REQUIRE(t->startLine() == 0);
    REQUIRE(t->endColumn() == 3);
    REQUIRE(t->endLine() == 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->text() == QStringLiteral("<pre>abc</pre>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endColumn() == 13);
    REQUIRE(h->endLine() == 1);
}

/*
Text <pre>
</pre>
Text

*/
TEST_CASE("075")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/075.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 2);
    REQUIRE(p->items().size() == 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 4);
        REQUIRE(t->endLine() == 0);
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text "));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->startColumn() == 5);
        REQUIRE(h->startLine() == 0);
        REQUIRE(h->endColumn() == 9);
        REQUIRE(h->endLine() == 0);
        REQUIRE(h->text() == QStringLiteral("<pre>"));
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(2).get());
        REQUIRE(h->startColumn() == 0);
        REQUIRE(h->startLine() == 1);
        REQUIRE(h->endColumn() == 5);
        REQUIRE(h->endLine() == 1);
        REQUIRE(h->text() == QStringLiteral("</pre>"));
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(3).get());
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 2);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 2);
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
    }
}

/*
Text
<pre>
</pre>
Text

*/
TEST_CASE("076")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/076.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 0);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 0);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->text() == QStringLiteral("<pre>\n</pre>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endColumn() == 5);
    REQUIRE(h->endLine() == 2);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 3);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 3);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 3);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 3);
    }
}

/*
<!-- Comment -->

*/
TEST_CASE("077")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/077.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<!-- Comment -->"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 15);
    REQUIRE(h->endLine() == 0);
}

/*
Text <!-- Comment -->

*/
TEST_CASE("078")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/078.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 20);
    REQUIRE(p->endLine() == 0);
    REQUIRE(p->items().size() == 2);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text "));
    REQUIRE(t->startColumn() == 0);
    REQUIRE(t->startLine() == 0);
    REQUIRE(t->endColumn() == 4);
    REQUIRE(t->endLine() == 0);

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<!-- Comment -->"));
    REQUIRE(h->startColumn() == 5);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 20);
    REQUIRE(h->endLine() == 0);
}

/*
<!-- Comment --> Text

*/
TEST_CASE("079")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/079.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<!-- Comment --> Text"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 20);
    REQUIRE(h->endLine() == 0);
}

/*
<!-- Comment -->
Text

*/
TEST_CASE("080")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/080.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<!-- Comment -->"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 15);
    REQUIRE(h->endLine() == 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 1);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 1);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    REQUIRE(t->startColumn() == 0);
    REQUIRE(t->startLine() == 1);
    REQUIRE(t->endColumn() == 3);
    REQUIRE(t->endLine() == 1);
}

/*
Text
<!-- Comment -->

*/
TEST_CASE("081")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/081.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    REQUIRE(t->startColumn() == 0);
    REQUIRE(t->startLine() == 0);
    REQUIRE(t->endColumn() == 3);
    REQUIRE(t->endLine() == 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->text() == QStringLiteral("<!-- Comment -->"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endColumn() == 15);
    REQUIRE(h->endLine() == 1);
}

/*
Text <!-- Comment -->
Text

*/
TEST_CASE("082")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/082.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text "));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 4);
        REQUIRE(t->endLine() == 0);
    }

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<!-- Comment -->"));
    REQUIRE(h->startColumn() == 5);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 20);
    REQUIRE(h->endLine() == 0);

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 1);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 1);
    }
}

/*
Text
<!-- Comment -->
Text

*/
TEST_CASE("083")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/083.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 0);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 0);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->text() == QStringLiteral("<!-- Comment -->"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endColumn() == 15);
    REQUIRE(h->endLine() == 1);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 2);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 2);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 2);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 2);
    }
}

/*
<?php

echo '>';

?>

*/
TEST_CASE("084")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/084.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<?php\n\necho '>';\n\n?>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 1);
    REQUIRE(h->endLine() == 4);
}

/*
Text <?php

echo '>';

?>

*/
TEST_CASE("085")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/085.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 9);
        REQUIRE(p->endLine() == 0);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text <?php"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 9);
        REQUIRE(t->endLine() == 0);
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 2);
        REQUIRE(p->endColumn() == 8);
        REQUIRE(p->endLine() == 2);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("echo '>';"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 2);
        REQUIRE(t->endColumn() == 8);
        REQUIRE(t->endLine() == 2);
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 4);
        REQUIRE(p->endColumn() == 1);
        REQUIRE(p->endLine() == 4);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("?>"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 4);
        REQUIRE(t->endColumn() == 1);
        REQUIRE(t->endLine() == 4);
    }
}

/*
<?php

echo '>';

?> Text

*/
TEST_CASE("086")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/086.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<?php\n\necho '>';\n\n?> Text"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 6);
    REQUIRE(h->endLine() == 4);
}

/*
<?php

echo '>';

?>
Text

*/
TEST_CASE("087")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/087.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<?php\n\necho '>';\n\n?>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 1);
    REQUIRE(h->endLine() == 4);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 5);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 5);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    REQUIRE(t->startColumn() == 0);
    REQUIRE(t->startLine() == 5);
    REQUIRE(t->endColumn() == 3);
    REQUIRE(t->endLine() == 5);
}

/*
Text
<?php

echo '>';

?>

*/
TEST_CASE("088")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/088.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    REQUIRE(t->startColumn() == 0);
    REQUIRE(t->startLine() == 0);
    REQUIRE(t->endColumn() == 3);
    REQUIRE(t->endLine() == 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->text() == QStringLiteral("<?php\n\necho '>';\n\n?>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endColumn() == 1);
    REQUIRE(h->endLine() == 5);
}

/*
Text <?php

echo '>';

?>
Text

*/
TEST_CASE("089")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/089.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 9);
        REQUIRE(p->endLine() == 0);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text <?php"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 9);
        REQUIRE(t->endLine() == 0);
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 2);
        REQUIRE(p->endColumn() == 8);
        REQUIRE(p->endLine() == 2);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("echo '>';"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 2);
        REQUIRE(t->endColumn() == 8);
        REQUIRE(t->endLine() == 2);
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 4);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 5);
        REQUIRE(p->items().size() == 2);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        {
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("?>"));
            REQUIRE(t->startColumn() == 0);
            REQUIRE(t->startLine() == 4);
            REQUIRE(t->endColumn() == 1);
            REQUIRE(t->endLine() == 4);
        }

        {
            auto t = static_cast<MD::Text *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Text"));
            REQUIRE(t->startColumn() == 0);
            REQUIRE(t->startLine() == 5);
            REQUIRE(t->endColumn() == 3);
            REQUIRE(t->endLine() == 5);
        }
    }
}

/*
Text
<?php

echo '>';

?>
Text

*/
TEST_CASE("090")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/090.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 0);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 0);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->text() == QStringLiteral("<?php\n\necho '>';\n\n?>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endColumn() == 1);
    REQUIRE(h->endLine() == 5);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 6);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 6);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 6);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 6);
    }
}
