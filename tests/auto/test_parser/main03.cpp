/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include "parser.h"

#include "test_utils.h"

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
    CHECK_POSITIONS(l, 0, 0, 0, 3);

    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    CHECK_POSITIONS(li, 0, 0, 0, 3);
    REQUIRE(li->delim() == MD::WithPosition{0, 0, 0, 0});

    REQUIRE(li->items().size() == 2);

    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(li->items().at(0).get());
    CHECK_POSITIONS(p, 4, 0, 33, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    CHECK_POSITIONS(t, 4, 0, 33, 0);
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("A list item with a code block:"));

    REQUIRE(li->items().at(1)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(li->items().at(1).get());
    CHECK_POSITIONS(c, 8, 2, 23, 2);
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
        CHECK_POSITIONS(p, 0, 0, 6, 0);

        REQUIRE(p->items().size() == 2);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t1 = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t1, 0, 0, 0, 0);
        REQUIRE(t1->text() == QStringLiteral("*"));

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Code);
        auto c1 = static_cast<MD::Code *>(p->items().at(1).get());
        CHECK_POSITIONS(c1, 3, 0, 4, 0);
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
        CHECK_POSITIONS(p, 0, 2, 8, 2);

        REQUIRE(p->items().size() == 2);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t1 = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t1, 0, 2, 0, 2);
        REQUIRE(t1->text() == QStringLiteral("*"));

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Code);
        auto c1 = static_cast<MD::Code *>(p->items().at(1).get());
        CHECK_POSITIONS(c1, 4, 2, 5, 2);
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
        CHECK_POSITIONS(p, 0, 4, 11, 4);

        REQUIRE(p->items().size() == 2);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t1 = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t1, 0, 4, 0, 4);
        REQUIRE(t1->text() == QStringLiteral("*"));

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Code);
        auto c1 = static_cast<MD::Code *>(p->items().at(1).get());
        CHECK_POSITIONS(c1, 4, 4, 8, 4);
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
        CHECK_POSITIONS(p, 0, 6, 8, 6);

        REQUIRE(p->items().size() == 2);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Code);
        auto c1 = static_cast<MD::Code *>(p->items().at(0).get());
        CHECK_POSITIONS(c1, 3, 6, 3, 6);
        REQUIRE(c1->startDelim() == MD::WithPosition{2, 6, 2, 6});
        REQUIRE(c1->endDelim() == MD::WithPosition{4, 6, 4, 6});
        REQUIRE(c1->text() == QStringLiteral("*"));
        REQUIRE(c1->openStyles().size() == 1);
        REQUIRE(c1->openStyles().at(0) == MD::StyleDelim{MD::BoldText, 0, 6, 1, 6});
        REQUIRE(c1->closeStyles().size() == 1);
        REQUIRE(c1->closeStyles().at(0) == MD::StyleDelim{MD::BoldText, 5, 6, 6, 6});

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t1 = static_cast<MD::Text *>(p->items().at(1).get());
        CHECK_POSITIONS(t1, 7, 6, 8, 6);
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
    CHECK_POSITIONS(p, 0, 0, 18, 0);
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 3, 0, 8, 0);
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
        CHECK_POSITIONS(t, 12, 0, 18, 0);
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
    CHECK_POSITIONS(p, 0, 0, 2, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    CHECK_POSITIONS(t, 0, 0, 2, 0);
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
        CHECK_POSITIONS(p, 0, 0, 41, 1);

        REQUIRE(p->items().size() == 6);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            CHECK_POSITIONS(t, 0, 0, 4, 0);
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("When "));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Math);
            auto m = static_cast<MD::Math *>(p->items().at(1).get());
            CHECK_POSITIONS(m, 6, 0, 12, 0);
            REQUIRE(m->expr() == QStringLiteral("a \\ne 0"));
            REQUIRE(m->isInline());
            REQUIRE(m->startDelim() == MD::WithPosition{5, 0, 5, 0});
            REQUIRE(m->endDelim() == MD::WithPosition{13, 0, 13, 0});
        }

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(2).get());
            CHECK_POSITIONS(t, 14, 0, 42, 0);
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral(", there are two solutions to "));
        }

        {
            REQUIRE(p->items().at(3)->type() == MD::ItemType::Math);
            auto m = static_cast<MD::Math *>(p->items().at(3).get());
            CHECK_POSITIONS(m, 44, 0, 62, 0);
            REQUIRE(m->expr() == QStringLiteral("(ax^2 + bx + c = 0)"));
            REQUIRE(m->isInline());
            REQUIRE(m->startDelim() == MD::WithPosition{43, 0, 43, 0});
            REQUIRE(m->endDelim() == MD::WithPosition{63, 0, 63, 0});
        }

        {
            REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(4).get());
            CHECK_POSITIONS(t, 64, 0, 76, 0);
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral(" and they are"));
        }

        {
            REQUIRE(p->items().at(5)->type() == MD::ItemType::Math);
            auto m = static_cast<MD::Math *>(p->items().at(5).get());
            CHECK_POSITIONS(m, 2, 1, 39, 1);
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
    CHECK_POSITIONS(p, 0, 0, 2, 2);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Math);
    auto m = static_cast<MD::Math *>(p->items().at(0).get());
    CHECK_POSITIONS(m, 0, 1, 35, 1);
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
        CHECK_POSITIONS(p, 0, 0, 8, 0);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 0, 8, 0);

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Paragraph"));
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(2).get());
    CHECK_POSITIONS(l, 1, 2, 0, 8);

    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto item = static_cast<MD::ListItem *>(l->items().at(0).get());
    CHECK_POSITIONS(item, 1, 2, 0, 8);
    REQUIRE(item->delim() == MD::WithPosition{1, 2, 1, 2});

    REQUIRE(item->listType() == MD::ListItem::Unordered);

    REQUIRE(item->items().size() == 2);

    {
        REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
        CHECK_POSITIONS(p, 3, 2, 3, 3);

        REQUIRE(p->items().size() == 2);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            CHECK_POSITIONS(t, 3, 2, 6, 2);

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("List"));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(1).get());
            CHECK_POSITIONS(t, 0, 3, 3, 3);

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("item"));
        }
    }

    {
        REQUIRE(item->items().at(1)->type() == MD::ItemType::Code);

        auto c = static_cast<MD::Code *>(item->items().at(1).get());
        CHECK_POSITIONS(c, 3, 6, 6, 6);
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
    CHECK_POSITIONS(h, 0, 0, 0, 4);
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
    CHECK_POSITIONS(h, 0, 0, 5, 4);
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
        CHECK_POSITIONS(p, 0, 0, 9, 0);
        REQUIRE(p->items().size() == 2);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text "));
        CHECK_POSITIONS(t, 0, 0, 4, 0);

        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<pre>"));
        CHECK_POSITIONS(h, 5, 0, 9, 0);
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        CHECK_POSITIONS(p, 0, 2, 3, 2);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("data"));
        CHECK_POSITIONS(t, 0, 2, 3, 2);
    }

    REQUIRE(doc->items().at(3)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(3).get());
    REQUIRE(h->text() == QStringLiteral("</pre>"));
    CHECK_POSITIONS(h, 0, 4, 5, 4);
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
    CHECK_POSITIONS(h, 0, 0, 10, 4);
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
    CHECK_POSITIONS(h, 0, 0, 5, 4);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    CHECK_POSITIONS(p, 0, 5, 3, 5);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(t, 0, 5, 3, 5);
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
    CHECK_POSITIONS(h, 0, 0, 13, 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    CHECK_POSITIONS(p, 0, 1, 3, 1);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    CHECK_POSITIONS(t, 0, 1, 3, 1);
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
    CHECK_POSITIONS(p, 0, 0, 3, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(t, 0, 0, 3, 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->text() == QStringLiteral("<pre>abc</pre>"));
    CHECK_POSITIONS(h, 0, 1, 13, 1);
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
    CHECK_POSITIONS(p, 0, 0, 3, 2);
    REQUIRE(p->items().size() == 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 0, 4, 0);
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text "));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        CHECK_POSITIONS(h, 5, 0, 9, 0);
        REQUIRE(h->text() == QStringLiteral("<pre>"));
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(2).get());
        CHECK_POSITIONS(h, 0, 1, 5, 1);
        REQUIRE(h->text() == QStringLiteral("</pre>"));
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(3).get());
        CHECK_POSITIONS(t, 0, 2, 3, 2);
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
        CHECK_POSITIONS(p, 0, 0, 3, 0);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(t, 0, 0, 3, 0);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->text() == QStringLiteral("<pre>\n</pre>"));
    CHECK_POSITIONS(h, 0, 1, 5, 2);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        CHECK_POSITIONS(p, 0, 3, 3, 3);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(t, 0, 3, 3, 3);
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
    CHECK_POSITIONS(h, 0, 0, 15, 0);
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
    CHECK_POSITIONS(p, 0, 0, 20, 0);
    REQUIRE(p->items().size() == 2);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text "));
    CHECK_POSITIONS(t, 0, 0, 4, 0);

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<!-- Comment -->"));
    CHECK_POSITIONS(h, 5, 0, 20, 0);
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
    CHECK_POSITIONS(h, 0, 0, 20, 0);
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
    CHECK_POSITIONS(h, 0, 0, 15, 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    CHECK_POSITIONS(p, 0, 1, 3, 1);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(t, 0, 1, 3, 1);
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
    CHECK_POSITIONS(p, 0, 0, 3, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(t, 0, 0, 3, 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->text() == QStringLiteral("<!-- Comment -->"));
    CHECK_POSITIONS(h, 0, 1, 15, 1);
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
    CHECK_POSITIONS(p, 0, 0, 3, 1);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text "));
        CHECK_POSITIONS(t, 0, 0, 4, 0);
    }

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<!-- Comment -->"));
    CHECK_POSITIONS(h, 5, 0, 20, 0);

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(t, 0, 1, 3, 1);
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
        CHECK_POSITIONS(p, 0, 0, 3, 0);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(t, 0, 0, 3, 0);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->text() == QStringLiteral("<!-- Comment -->"));
    CHECK_POSITIONS(h, 0, 1, 15, 1);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        CHECK_POSITIONS(p, 0, 2, 3, 2);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(t, 0, 2, 3, 2);
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
    CHECK_POSITIONS(h, 0, 0, 1, 4);
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
        CHECK_POSITIONS(p, 0, 0, 9, 0);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text <?php"));
        CHECK_POSITIONS(t, 0, 0, 9, 0);
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        CHECK_POSITIONS(p, 0, 2, 8, 2);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("echo '>';"));
        CHECK_POSITIONS(t, 0, 2, 8, 2);
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        CHECK_POSITIONS(p, 0, 4, 1, 4);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("?>"));
        CHECK_POSITIONS(t, 0, 4, 1, 4);
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
    CHECK_POSITIONS(h, 0, 0, 6, 4);
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
    CHECK_POSITIONS(h, 0, 0, 1, 4);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    CHECK_POSITIONS(p, 0, 5, 3, 5);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(t, 0, 5, 3, 5);
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
    CHECK_POSITIONS(p, 0, 0, 3, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(t, 0, 0, 3, 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->text() == QStringLiteral("<?php\n\necho '>';\n\n?>"));
    CHECK_POSITIONS(h, 0, 1, 1, 5);
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
        CHECK_POSITIONS(p, 0, 0, 9, 0);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text <?php"));
        CHECK_POSITIONS(t, 0, 0, 9, 0);
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        CHECK_POSITIONS(p, 0, 2, 8, 2);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("echo '>';"));
        CHECK_POSITIONS(t, 0, 2, 8, 2);
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        CHECK_POSITIONS(p, 0, 4, 3, 5);
        REQUIRE(p->items().size() == 2);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        {
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("?>"));
            CHECK_POSITIONS(t, 0, 4, 1, 4);
        }

        {
            auto t = static_cast<MD::Text *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Text"));
            CHECK_POSITIONS(t, 0, 5, 3, 5);
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
        CHECK_POSITIONS(p, 0, 0, 3, 0);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(t, 0, 0, 3, 0);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->text() == QStringLiteral("<?php\n\necho '>';\n\n?>"));
    CHECK_POSITIONS(h, 0, 1, 1, 5);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        CHECK_POSITIONS(p, 0, 6, 3, 6);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(t, 0, 6, 3, 6);
    }
}
