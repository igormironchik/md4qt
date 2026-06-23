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
> foo
bar
--

*/
TEST_CASE("121")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/121.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);
        auto b = static_cast<MD::Blockquote *>(doc->items().at(1).get());
        CHECK_POSITIONS(b, 0, 0, 1, 2);
        REQUIRE(b->delims() == MD::Blockquote::Delims{{0, 0, 0, 0}});
        REQUIRE(b->items().size() == 1);
        REQUIRE(b->items().at(0)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(b->items().at(0).get());
        REQUIRE(p->items().size() == 3);
        CHECK_POSITIONS(p, 2, 0, 1, 2);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("foo"));
            CHECK_POSITIONS(t, 2, 0, 4, 0);
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("bar"));
            CHECK_POSITIONS(t, 0, 1, 2, 1);
        }

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(2).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("--"));
            CHECK_POSITIONS(t, 0, 2, 1, 2);
        }
    }
}

/*
```math
\[\mathrm{\mathbf{M}}(\alpha) =
   \left(
      \begin{matrix}
         \cos(\alpha)+n_x^2\cdot (1-\cos(\alpha))  &  n_x\cdot n_y\cdot (1-\cos(\alpha))-n_z\cdot \sin(\alpha) &
n_x\cdot n_z\cdot (1-\cos(\alpha))+n_y\cdot
\sin(\alpha)\\
         n_x\cdot n_y\cdot (1-\cos(\alpha))+n_z\cdot \sin(\alpha) & \cos(\alpha)+n_y^2\cdot (1-\cos(\alpha))  & n_y\cdot
n_z\cdot (1-\cos(\alpha))-n_x\cdot
\sin(\alpha)\\ n_z\cdot n_x\cdot (1-\cos(\alpha))-n_y\cdot \sin(\alpha) & n_z\cdot n_y\cdot (1-\cos(\alpha))+n_x\cdot
\sin(\alpha)  & \cos(\alpha)+n_z^2\cdot (1-\cos(\alpha)) \end{matrix} \right)
\]
```

*/
TEST_CASE("122")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/122.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    CHECK_POSITIONS(p, 0, 0, 2, 10);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Math);
    auto m = static_cast<MD::Math *>(p->items().at(0).get());
    REQUIRE(!m->isInline());
    REQUIRE(m->expr()
            == QStringLiteral("\\[\\mathrm{\\mathbf{M}}(\\alpha) =\n"
                              "   \\left(\n"
                              "      \\begin{matrix}\n"
                              "         \\cos(\\alpha)+n_x^2\\cdot (1-\\cos(\\alpha))  &  "
                              "n_x\\cdot n_y\\cdot (1-\\cos(\\alpha))-n_z\\cdot \\sin(\\alpha) &  "
                              "n_x\\cdot n_z\\cdot (1-\\cos(\\alpha))+n_y\\cdot \\sin(\\alpha)\\\\\n"
                              "         n_x\\cdot n_y\\cdot (1-\\cos(\\alpha))+n_z\\cdot \\sin(\\alpha) & "
                              "\\cos(\\alpha)+n_y^2\\cdot (1-\\cos(\\alpha))  &   "
                              "n_y\\cdot n_z\\cdot (1-\\cos(\\alpha))-n_x\\cdot \\sin(\\alpha)\\\\\n"
                              "         n_z\\cdot n_x\\cdot (1-\\cos(\\alpha))-n_y\\cdot \\sin(\\alpha) & "
                              "n_z\\cdot n_y\\cdot (1-\\cos(\\alpha))+n_x\\cdot \\sin(\\alpha)  & "
                              "\\cos(\\alpha)+n_z^2\\cdot (1-\\cos(\\alpha))\n"
                              "      \\end{matrix}\n"
                              "   \\right)\n"
                              "\\]"));
    CHECK_POSITIONS(m, 0, 1, 1, 9);
    REQUIRE(m->startDelim() == MD::WithPosition{0, 0, 2, 0});
    REQUIRE(m->endDelim() == MD::WithPosition{0, 10, 2, 10});
    REQUIRE(m->syntaxPos() == MD::WithPosition{3, 0, 6, 0});
}

/*
Text

# Heading

 * List

*/
TEST_CASE("123")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/123.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    CHECK_POSITIONS(p, 0, 0, 3, 0);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(t, 0, 0, 3, 0);

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(2).get());
        CHECK_POSITIONS(h, 0, 2, 8, 2);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 2, 0, 2});

        auto p = h->text().get();
        REQUIRE(p->items().size() == 1);
        CHECK_POSITIONS(p, 2, 2, 8, 2);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->text() == QStringLiteral("Heading"));
        CHECK_POSITIONS(t, 2, 2, 8, 2);
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List *>(doc->items().at(3).get());
        CHECK_POSITIONS(l, 1, 4, 0, 5);

        REQUIRE(l->items().size() == 1);

        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto item = static_cast<MD::ListItem *>(l->items().at(0).get());
        CHECK_POSITIONS(item, 1, 4, 0, 5);
        REQUIRE(item->delim() == MD::WithPosition{1, 4, 1, 4});
        REQUIRE(item->listType() == MD::ListItem::Unordered);
        REQUIRE(item->items().size() == 1);
        REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
        CHECK_POSITIONS(p, 3, 4, 6, 4);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("List"));
        CHECK_POSITIONS(t, 3, 4, 6, 4);
    }
}

/*
**Why is implementation of the following method so terrible?**

 *
   ```cpp
   template< class Trait >
   inline void
   Parser< Trait >::parse( StringListStream< Trait > & stream,
       std::shared_ptr< Block< Trait > > parent,
       std::shared_ptr< Document< Trait > > doc,
       typename Trait::StringList & linksToParse,
       const typename Trait::String & workingPath,
       const typename Trait::String & fileName,
       bool collectRefLinks, bool top );
   ```

   Implementation of the above method is so ugly because

*/
TEST_CASE("124")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/124.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    CHECK_POSITIONS(p, 0, 0, 61, 0);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->text() == QStringLiteral("Why is implementation of the following method so terrible?"));
    CHECK_POSITIONS(t, 2, 0, 59, 0);
    REQUIRE(t->openStyles().size() == 1);
    REQUIRE(t->openStyles().at(0) == MD::StyleDelim{MD::BoldText, 0, 0, 1, 0});
    REQUIRE(t->closeStyles().size() == 1);
    REQUIRE(t->closeStyles().at(0) == MD::StyleDelim{MD::BoldText, 60, 0, 61, 0});

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(2).get());
    CHECK_POSITIONS(l, 1, 2, 2, 16);

    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto item = static_cast<MD::ListItem *>(l->items().at(0).get());
    CHECK_POSITIONS(item, 1, 2, 2, 16);
    REQUIRE(item->delim() == MD::WithPosition{1, 2, 1, 2});

    REQUIRE(item->listType() == MD::ListItem::Unordered);

    REQUIRE(item->items().size() == 2);

    REQUIRE(item->items().at(0)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(item->items().at(0).get());
    CHECK_POSITIONS(c, 3, 4, 39, 12);
    REQUIRE(c->syntax() == QStringLiteral("cpp"));
    REQUIRE(!c->isInline());
    REQUIRE(c->startDelim() == MD::WithPosition{3, 3, 5, 3});
    REQUIRE(c->endDelim() == MD::WithPosition{3, 13, 5, 13});
    REQUIRE(c->syntaxPos() == MD::WithPosition{6, 3, 8, 3});
    REQUIRE(c->text()
            == QStringLiteral("template< class Trait >\n"
                              "inline void\n"
                              "Parser< Trait >::parse( StringListStream< Trait > & stream,\n"
                              "    std::shared_ptr< Block< Trait > > parent,\n"
                              "    std::shared_ptr< Document< Trait > > doc,\n"
                              "    typename Trait::StringList & linksToParse,\n"
                              "    const typename Trait::String & workingPath,\n"
                              "    const typename Trait::String & fileName,\n"
                              "    bool collectRefLinks, bool top );"));

    REQUIRE(item->items().at(1)->type() == MD::ItemType::Paragraph);
    {
        auto p = static_cast<MD::Paragraph *>(item->items().at(1).get());
        REQUIRE(p->items().size() == 1);
        CHECK_POSITIONS(p, 3, 15, 55, 15);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->text() == QStringLiteral("Implementation of the above method is so ugly because"));
        CHECK_POSITIONS(t, 3, 15, 55, 15);
    }
}

/*
* [Issues](#issues)<!-- endToc -->

*/
TEST_CASE("125")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/125.md"));

    const auto wd = QDir().absolutePath() + QStringLiteral("/tests/parser/data");

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 1);

    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto item = static_cast<MD::ListItem *>(l->items().at(0).get());
    CHECK_POSITIONS(item, 0, 0, 0, 1);
    REQUIRE(item->delim() == MD::WithPosition{0, 0, 0, 0});

    REQUIRE(item->listType() == MD::ListItem::Unordered);

    REQUIRE(item->items().size() == 1);

    REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
    CHECK_POSITIONS(p, 2, 0, 33, 0);

    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);

    {
        auto l = static_cast<MD::Link *>(p->items().at(0).get());

        REQUIRE(l->text() == QStringLiteral("Issues"));
        REQUIRE(l->url() == QStringLiteral("#issues/") + wd + QStringLiteral("/125.md"));
        CHECK_POSITIONS(l, 2, 0, 18, 0);
        REQUIRE(l->textPos() == MD::WithPosition{3, 0, 8, 0});
        REQUIRE(l->urlPos() == MD::WithPosition{11, 0, 17, 0});
    }

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);

    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());

    REQUIRE(h->text() == QStringLiteral("<!-- endToc -->"));
    CHECK_POSITIONS(h, 19, 0, 33, 0);
}

/*
# Radiated 1G-6G
Measured in GTEM using 12 Faces Method.
<foo>
## Results
![Measurement Result](MeasurementResult)

<Peaklist>
<bar>

## Observation
No relevant emissions other than radio transmissions.

## Images

*/
TEST_CASE("126")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/126.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 9);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);

        auto h = static_cast<MD::Heading *>(doc->items().at(1).get());
        CHECK_POSITIONS(h, 0, 0, 15, 0);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 0, 0, 0});

        {
            REQUIRE(h->level() == 1);
            REQUIRE(h->text().get());
            auto p = h->text().get();
            CHECK_POSITIONS(p, 2, 0, 15, p->startLine());
            REQUIRE(p->items().size() == 1);
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->text() == QStringLiteral("Radiated 1G-6G"));
            CHECK_POSITIONS(t, 2, 0, 15, t->startLine());
        }
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        CHECK_POSITIONS(p, 0, 1, 4, 2);

        REQUIRE(p->items().size() == 2);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Measured in GTEM using 12 Faces Method."));
        CHECK_POSITIONS(t, 0, 1, 38, 1);

        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);

        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());

        REQUIRE(h->text() == QStringLiteral("<foo>"));
        CHECK_POSITIONS(h, 0, 2, 4, 2);
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Heading);

        auto h = static_cast<MD::Heading *>(doc->items().at(3).get());
        CHECK_POSITIONS(h, 0, 3, 9, 3);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 3, 1, 3});

        {
            REQUIRE(h->level() == 2);
            REQUIRE(h->text().get());
            auto p = h->text().get();
            CHECK_POSITIONS(p, 3, 3, 9, p->startLine());
            REQUIRE(p->items().size() == 1);
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->text() == QStringLiteral("Results"));
            CHECK_POSITIONS(t, 3, 3, 9, t->startLine());
        }
    }

    {
        REQUIRE(doc->items().at(4)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(4).get());
        CHECK_POSITIONS(p, 0, 4, 39, 4);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);

        auto i = static_cast<MD::Image *>(p->items().at(0).get());

        REQUIRE(i->text() == QStringLiteral("Measurement Result"));
        REQUIRE(i->url() == QStringLiteral("MeasurementResult"));
        CHECK_POSITIONS(i, 0, 4, 39, 4);
        REQUIRE(i->textPos() == MD::WithPosition{2, 4, 19, 4});
        REQUIRE(i->urlPos() == MD::WithPosition{22, 4, 38, 4});
    }

    {
        REQUIRE(doc->items().at(5)->type() == MD::ItemType::RawHtml);

        auto h = static_cast<MD::RawHtml *>(doc->items().at(5).get());

        REQUIRE(h->text() == QStringLiteral("<Peaklist>\n<bar>"));
        CHECK_POSITIONS(h, 0, 6, 4, 7);
    }

    {
        REQUIRE(doc->items().at(6)->type() == MD::ItemType::Heading);

        auto h = static_cast<MD::Heading *>(doc->items().at(6).get());
        CHECK_POSITIONS(h, 0, 9, 13, 9);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 9, 1, 9});

        {
            REQUIRE(h->level() == 2);
            REQUIRE(h->text().get());
            auto p = h->text().get();
            CHECK_POSITIONS(p, 3, 9, 13, p->startLine());
            REQUIRE(p->items().size() == 1);
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->text() == QStringLiteral("Observation"));
            CHECK_POSITIONS(t, 3, 9, 13, t->startLine());
        }
    }

    {
        REQUIRE(doc->items().at(7)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(7).get());
        CHECK_POSITIONS(p, 0, 10, 52, 10);

        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("No relevant emissions other than radio transmissions."));
        CHECK_POSITIONS(t, 0, 10, 52, 10);
    }

    {
        REQUIRE(doc->items().at(8)->type() == MD::ItemType::Heading);

        auto h = static_cast<MD::Heading *>(doc->items().at(8).get());
        CHECK_POSITIONS(h, 0, 12, 8, 12);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 12, 1, 12});

        {
            REQUIRE(h->level() == 2);
            REQUIRE(h->text().get());
            auto p = h->text().get();
            CHECK_POSITIONS(p, 3, 12, 8, p->startLine());
            REQUIRE(p->items().size() == 1);
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->text() == QStringLiteral("Images"));
            CHECK_POSITIONS(t, 3, 12, 8, t->startLine());
        }
    }
}

/*
<?php

echo ''

echo ''

echo ''

*/
TEST_CASE("127")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/127.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);

    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());

    REQUIRE(h->text() == QStringLiteral("<?php\n\necho ''\n\necho ''\n\necho ''\n"));
    CHECK_POSITIONS(h, 0, 0, 0, 7);
}

/*
- full example
```C++
#include <iostream>

int main() { return 0; }
```

# Heading

*/
TEST_CASE("128")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/128.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

        auto l = static_cast<MD::List *>(doc->items().at(1).get());
        CHECK_POSITIONS(l, 0, 0, 13, 0);

        REQUIRE(l->items().size() == 1);

        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

        auto item = static_cast<MD::ListItem *>(l->items().at(0).get());
        CHECK_POSITIONS(item, 0, 0, 13, 0);
        REQUIRE(item->delim() == MD::WithPosition{0, 0, 0, 0});

        REQUIRE(item->listType() == MD::ListItem::Unordered);

        REQUIRE(item->items().size() == 1);

        REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
        CHECK_POSITIONS(p, 2, 0, 13, 0);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("full example"));
        CHECK_POSITIONS(t, 2, 0, 13, 0);
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);

        auto c = static_cast<MD::Code *>(doc->items().at(2).get());

        REQUIRE(!c->isInline());
        REQUIRE(c->text() == QStringLiteral("#include <iostream>\n\nint main() { return 0; }"));
        CHECK_POSITIONS(c, 0, 2, 23, 4);
        REQUIRE(c->startDelim() == MD::WithPosition{0, 1, 2, 1});
        REQUIRE(c->endDelim() == MD::WithPosition{0, 5, 2, 5});
        REQUIRE(c->syntaxPos() == MD::WithPosition{3, 1, 5, 1});
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Heading);

        auto h = static_cast<MD::Heading *>(doc->items().at(3).get());
        CHECK_POSITIONS(h, 0, 7, 8, 7);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 7, 0, 7});

        REQUIRE(h->level() == 1);
        REQUIRE(h->text().get());
        auto p = h->text().get();
        CHECK_POSITIONS(p, 2, 7, 8, p->startLine());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->text() == QStringLiteral("Heading"));
        CHECK_POSITIONS(t, 2, 7, 8, t->startLine());
    }
}

/*
<!--[![Downloads](https://pepy.tech/badge/ludwig)](https://pepy.tech/project/ludwig)-->

*/
TEST_CASE("129")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/129.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(
        h->text()
        == QStringLiteral("<!--[![Downloads](https://pepy.tech/badge/ludwig)](https://pepy.tech/project/ludwig)-->"));
    CHECK_POSITIONS(h, 0, 0, 86, h->startLine());
}

/*
<!--[![Downloads](https://pepy.tech/badge/ludwig)](https://pepy.tech/project/ludwig)
text

text

*/
TEST_CASE("130")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/130.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(
        h->text()
        == QStringLiteral(
            "<!--[![Downloads](https://pepy.tech/badge/ludwig)](https://pepy.tech/project/ludwig)\ntext\n\ntext\n"));
    CHECK_POSITIONS(h, 0, 0, 0, 4);
}

/*
> <!--
> text
>

text

*/
TEST_CASE("131")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/131.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);
    auto b = static_cast<MD::Blockquote *>(doc->items().at(1).get());
    REQUIRE(b->items().size() == 1);
    CHECK_POSITIONS(b, 0, 0, 1, 2);
    REQUIRE(b->delims() == MD::Blockquote::Delims{{0, 0, 0, 0}, {0, 1, 0, 1}, {0, 2, 0, 2}});
    REQUIRE(b->items().at(0)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(b->items().at(0).get());
    REQUIRE(h->text() == QStringLiteral("<!--\ntext\n"));
    CHECK_POSITIONS(h, 2, 0, 1, 2);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    CHECK_POSITIONS(p, 0, 4, 3, 4);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->text() == QStringLiteral("text"));
    CHECK_POSITIONS(t, 0, 4, 3, 4);
}

/*
* <!--
* text

text

*/
TEST_CASE("132")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/132.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 2);
    CHECK_POSITIONS(l, 0, 0, 0, 2);
    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto i = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(i->items().size() == 1);
    CHECK_POSITIONS(i, 0, 0, 5, 0);
    REQUIRE(i->delim() == MD::WithPosition{0, 0, 0, 0});
    REQUIRE(i->items().at(0)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(i->items().at(0).get());
    REQUIRE(h->text() == QStringLiteral("<!--"));
    CHECK_POSITIONS(h, 2, 0, 5, 0);
    REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    CHECK_POSITIONS(p, 0, 3, 3, 3);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->text() == QStringLiteral("text"));
    CHECK_POSITIONS(t, 0, 3, 3, 3);
}

/*
[Standalone GPU-Ready solution](#standalone-gpu-ready-solution)
[Files in `work/data` too](#files-in-workdata-too)

# Standalone GPU-Ready solution

# Files in `work/data` too

*/
TEST_CASE("133")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/133.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 49, 1);
    REQUIRE(p->items().size() == 2);

    const auto wd = QDir().absolutePath() + QStringLiteral("/tests/parser/data");

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->url() == QStringLiteral("#standalone-gpu-ready-solution/") + wd + QStringLiteral("/133.md"));
        REQUIRE(l->textPos() == MD::WithPosition{1, 0, 29, 0});
        REQUIRE(l->urlPos() == MD::WithPosition{32, 0, 61, 0});
        REQUIRE(doc->labeledHeadings().find(l->url()) != doc->labeledHeadings().cend());
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(1).get());
        REQUIRE(l->url() == QStringLiteral("#files-in-workdata-too/") + wd + QStringLiteral("/133.md"));
        REQUIRE(l->textPos() == MD::WithPosition{1, 1, 24, 1});
        REQUIRE(l->urlPos() == MD::WithPosition{27, 1, 48, 1});
        REQUIRE(doc->labeledHeadings().find(l->url()) != doc->labeledHeadings().cend());
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(2).get());
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 3, 0, 3});
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(3).get());
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 5, 0, 5});
    }
}

/*
Text
| Column 1 |
| -------- |
Text

*/
TEST_CASE("134")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/134.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(dp, 0, 0, 3, 0);

    REQUIRE(dp->items().size() == 1);
    REQUIRE(dp->items().front()->type() == MD::ItemType::Text);

    auto dt = static_cast<MD::Text *>(dp->items().front().get());

    REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
    REQUIRE(dt->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(dt, 0, 0, 3, 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Table);

    auto t = static_cast<MD::Table *>(doc->items().at(2).get());
    CHECK_POSITIONS(t, 0, 1, 3, 3);

    REQUIRE(t->columnsCount() == 1);
    REQUIRE(t->rows().size() == 2);

    auto r0 = t->rows().at(0);
    CHECK_POSITIONS(r0.get(), 0, 1, 11, r0->startLine());

    REQUIRE(r0->type() == MD::ItemType::TableRow);

    REQUIRE(r0->cells().size() == 1);

    REQUIRE(r0->cells().at(0)->type() == MD::ItemType::TableCell);
    auto c0 = static_cast<MD::TableCell *>(r0->cells().at(0).get());
    CHECK_POSITIONS(c0, 2, r0->startLine(), 9, c0->startLine());

    REQUIRE(c0->items().size() == 1);
    REQUIRE(c0->items().at(0)->type() == MD::ItemType::Text);

    auto t0 = static_cast<MD::Text *>(c0->items().at(0).get());
    CHECK_POSITIONS(t0, 2, c0->startLine(), 9, t0->startLine());

    REQUIRE(t0->text() == QStringLiteral("Column 1"));

    auto r1 = t->rows().at(1);
    CHECK_POSITIONS(r1.get(), 0, 3, 3, r1->startLine());

    REQUIRE(r1->type() == MD::ItemType::TableRow);

    REQUIRE(r1->cells().size() == 1);

    REQUIRE(r1->cells().at(0)->type() == MD::ItemType::TableCell);
    auto c1 = static_cast<MD::TableCell *>(r1->cells().at(0).get());
    CHECK_POSITIONS(c1, 0, r1->startLine(), 3, c1->startLine());

    REQUIRE(c1->items().size() == 1);
    REQUIRE(c1->items().at(0)->type() == MD::ItemType::Text);

    auto t1 = static_cast<MD::Text *>(c1->items().at(0).get());
    CHECK_POSITIONS(t1, 0, c1->startLine(), 3, t1->startLine());

    REQUIRE(t1->text() == QStringLiteral("Text"));
}

/*
`Text
| Column 1 |`
| -------- |
Text

*/
TEST_CASE("135")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/135.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(dp, 0, 0, 3, 3);

    REQUIRE(dp->items().size() == 3);
    REQUIRE(dp->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(dp->items().at(0).get());
    REQUIRE(c->text() == QStringLiteral("Text | Column 1 |"));
    CHECK_POSITIONS(c, 1, 0, 11, 1);
    REQUIRE(c->startDelim() == MD::WithPosition{0, 0, 0, 0});
    REQUIRE(c->endDelim() == MD::WithPosition{12, 1, 12, 1});

    {
        REQUIRE(dp->items().at(1)->type() == MD::ItemType::Text);
        auto dt = static_cast<MD::Text *>(dp->items().at(1).get());
        REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(dt->text() == QStringLiteral("| -------- |"));
        CHECK_POSITIONS(dt, 0, 2, 11, 2);
    }

    {
        REQUIRE(dp->items().at(2)->type() == MD::ItemType::Text);
        auto dt = static_cast<MD::Text *>(dp->items().at(2).get());
        REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(dt->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(dt, 0, 3, 3, 3);
    }
}

/*
Text
| Column 1 |
| -------- |
===

*/
TEST_CASE("136")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/136.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(dp, 0, 0, 3, 0);

    REQUIRE(dp->items().size() == 1);
    REQUIRE(dp->items().front()->type() == MD::ItemType::Text);

    auto dt = static_cast<MD::Text *>(dp->items().front().get());

    REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
    REQUIRE(dt->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(dt, 0, 0, 3, 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Table);

    auto t = static_cast<MD::Table *>(doc->items().at(2).get());
    CHECK_POSITIONS(t, 0, 1, 2, 3);

    REQUIRE(t->columnsCount() == 1);
    REQUIRE(t->rows().size() == 2);

    auto r0 = t->rows().at(0);
    CHECK_POSITIONS(r0.get(), 0, 1, 11, r0->startLine());

    REQUIRE(r0->type() == MD::ItemType::TableRow);

    REQUIRE(r0->cells().size() == 1);

    REQUIRE(r0->cells().at(0)->type() == MD::ItemType::TableCell);
    auto c0 = static_cast<MD::TableCell *>(r0->cells().at(0).get());
    CHECK_POSITIONS(c0, 2, r0->startLine(), 9, c0->startLine());

    REQUIRE(c0->items().size() == 1);
    REQUIRE(c0->items().at(0)->type() == MD::ItemType::Text);

    auto t0 = static_cast<MD::Text *>(c0->items().at(0).get());
    CHECK_POSITIONS(t0, 2, c0->startLine(), 9, t0->startLine());

    REQUIRE(t0->text() == QStringLiteral("Column 1"));

    auto r1 = t->rows().at(1);
    CHECK_POSITIONS(r1.get(), 0, 3, 2, r1->startLine());

    REQUIRE(r1->type() == MD::ItemType::TableRow);

    REQUIRE(r1->cells().size() == 1);

    REQUIRE(r1->cells().at(0)->type() == MD::ItemType::TableCell);
    auto c1 = static_cast<MD::TableCell *>(r1->cells().at(0).get());
    CHECK_POSITIONS(c1, 0, r1->startLine(), r1->endColumn(), c1->startLine());

    REQUIRE(c1->items().size() == 1);
    REQUIRE(c1->items().at(0)->type() == MD::ItemType::Text);

    auto t1 = static_cast<MD::Text *>(c1->items().at(0).get());
    CHECK_POSITIONS(t1, 0, c1->startLine(), 2, t1->startLine());

    REQUIRE(t1->text() == QStringLiteral("==="));
}

/*
`Text
| Column 1 |`
| -------- |
===

*/
TEST_CASE("137")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/137.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
    auto h = static_cast<MD::Heading *>(doc->items().at(1).get());
    CHECK_POSITIONS(h, 0, 0, 2, 3);
    REQUIRE(h->delims().size() == 1);
    REQUIRE(h->delims().front() == MD::WithPosition{0, 3, 2, 3});

    auto dp = h->text().get();
    CHECK_POSITIONS(dp, 0, 0, 11, 2);

    REQUIRE(dp->items().size() == 2);
    REQUIRE(dp->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(dp->items().at(0).get());
    REQUIRE(c->text() == QStringLiteral("Text | Column 1 |"));
    CHECK_POSITIONS(c, 1, 0, 11, 1);
    REQUIRE(c->startDelim() == MD::WithPosition{0, 0, 0, 0});
    REQUIRE(c->endDelim() == MD::WithPosition{12, 1, 12, 1});

    REQUIRE(dp->items().at(1)->type() == MD::ItemType::Text);
    auto dt = static_cast<MD::Text *>(dp->items().at(1).get());
    REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
    REQUIRE(dt->text() == QStringLiteral("| -------- |"));
    CHECK_POSITIONS(dt, 0, 2, 11, 2);
}

/*
1.
   Monday

   fgh

   ```
   code
   ```
2.
Tuesday
3.
Wednesday

*/
TEST_CASE("138")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/138.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 1, 8);

    REQUIRE(l->items().size() == 2);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto i1 = static_cast<MD::ListItem *>(l->items().at(0).get());
    CHECK_POSITIONS(i1, 0, 0, 5, 7);
    REQUIRE(i1->delim() == MD::WithPosition{0, 0, 1, 0});

    REQUIRE(i1->listType() == MD::ListItem::Ordered);

    REQUIRE(i1->items().size() == 3);

    {
        REQUIRE(i1->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(i1->items().at(0).get());
        CHECK_POSITIONS(p, 3, 1, 8, 1);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Monday"));
        CHECK_POSITIONS(t, 3, 1, 8, 1);
    }

    {
        REQUIRE(i1->items().at(1)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(i1->items().at(1).get());
        CHECK_POSITIONS(p, 3, 3, 5, 3);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("fgh"));
        CHECK_POSITIONS(t, 3, 3, 5, 3);
    }

    {
        REQUIRE(i1->items().at(2)->type() == MD::ItemType::Code);

        auto c = static_cast<MD::Code *>(i1->items().at(2).get());
        CHECK_POSITIONS(c, 3, 6, 6, 6);
        REQUIRE(c->startDelim() == MD::WithPosition{3, 5, 5, 5});
        REQUIRE(c->endDelim() == MD::WithPosition{3, 7, 5, 7});
        REQUIRE(c->syntaxPos() == MD::WithPosition{-1, -1, -1, -1});
        REQUIRE(c->text() == QStringLiteral("code"));
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    CHECK_POSITIONS(p, 0, 9, 8, 11);

    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Tuesday"));
        CHECK_POSITIONS(t, 0, 9, 6, 9);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(1).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("3."));
        CHECK_POSITIONS(t, 0, 10, 1, 10);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(2).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Wednesday"));
        CHECK_POSITIONS(t, 0, 11, 8, 11);
    }

    REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);

    auto i2 = static_cast<MD::ListItem *>(l->items().at(1).get());
    REQUIRE(i2->isEmpty());
}

/*
* Text
    -

*/
TEST_CASE("139")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/139.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 2);

    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto item = static_cast<MD::ListItem *>(l->items().at(0).get());
    CHECK_POSITIONS(item, 0, 0, 0, 2);
    REQUIRE(item->delim() == MD::WithPosition{0, 0, 0, 0});

    REQUIRE(item->listType() == MD::ListItem::Unordered);

    REQUIRE(item->items().size() == 1);

    REQUIRE(item->items().at(0)->type() == MD::ItemType::Heading);

    auto h = static_cast<MD::Heading *>(item->items().at(0).get());
    CHECK_POSITIONS(h, 2, 0, 4, 1);

    REQUIRE(h->level() == 2);
    REQUIRE(!h->text()->isEmpty());

    auto p = h->text().get();
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

    auto t = static_cast<MD::Text *>(p->items().at(0).get());

    REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(t, 2, 0, 5, 0);
}

/*
Text

*   Text
    Text

    Text
    Text

*/
TEST_CASE("140")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/140.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        CHECK_POSITIONS(p, 0, 0, 3, 0);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(t, 0, 0, 3, 0);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(2).get());
    CHECK_POSITIONS(l, 0, 2, 0, 7);

    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto item = static_cast<MD::ListItem *>(l->items().at(0).get());
    CHECK_POSITIONS(item, 0, 2, 0, 7);
    REQUIRE(item->delim() == MD::WithPosition{0, 2, 0, 2});

    REQUIRE(item->listType() == MD::ListItem::Unordered);

    REQUIRE(item->items().size() == 2);

    {
        REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
        CHECK_POSITIONS(p, 4, 2, 7, 3);

        REQUIRE(p->items().size() == 2);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Text"));
            CHECK_POSITIONS(t, 4, 2, 7, 2);
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(1).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Text"));
            CHECK_POSITIONS(t, 4, 3, 7, 3);
        }
    }

    {
        REQUIRE(item->items().at(1)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(item->items().at(1).get());
        CHECK_POSITIONS(p, 4, 5, 7, 6);

        REQUIRE(p->items().size() == 2);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Text"));
            CHECK_POSITIONS(t, 4, 5, 7, 5);
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(1).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Text"));
            CHECK_POSITIONS(t, 4, 6, 7, 6);
        }
    }
}

/*
![sample code output](https://raw.githubusercontent.com/wiki/ocornut/imgui/web/v160/code_sample_02_jp.png)
<br>_(settings: Dark style (left), Light style (right) / Font: NotoSansCJKjp-Medium, 20px / Rounding: 5)_

*/
TEST_CASE("141")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/141.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 104, 1);

    REQUIRE(p->items().size() == 3);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);

    auto i = static_cast<MD::Image *>(p->items().at(0).get());
    CHECK_POSITIONS(i, 0, 0, 105, 0);
    REQUIRE(i->textPos() == MD::WithPosition{2, 0, 19, 0});
    REQUIRE(i->urlPos() == MD::WithPosition{22, 0, 104, 0});

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);

    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    CHECK_POSITIONS(h, 0, 1, 3, 1);

    REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);

    auto t = static_cast<MD::Text *>(p->items().at(2).get());

    REQUIRE(t->opts() == MD::TextOption::ItalicText);
    REQUIRE(t->text()
            == QStringLiteral("(settings: Dark style (left), Light style (right) "
                              "/ Font: NotoSansCJKjp-Medium, 20px / Rounding: 5)"));
    CHECK_POSITIONS(t, 5, 1, 103, 1);
}

/*
- Text

      Code
        Code
          Code
        Code
      Code

*/
TEST_CASE("142")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/142.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 7);

    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto item = static_cast<MD::ListItem *>(l->items().at(0).get());
    CHECK_POSITIONS(item, 0, 0, 0, 7);
    REQUIRE(item->delim() == MD::WithPosition{0, 0, 0, 0});

    REQUIRE(item->listType() == MD::ListItem::Unordered);

    REQUIRE(item->items().size() == 2);

    {
        REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
        CHECK_POSITIONS(p, 2, 0, 5, 0);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(t, 2, 0, 5, 0);
    }

    {
        REQUIRE(item->items().at(1)->type() == MD::ItemType::Code);

        auto c = static_cast<MD::Code *>(item->items().at(1).get());
        CHECK_POSITIONS(c, 6, 2, 9, 6);

        REQUIRE(c->text() == QStringLiteral("Code\n  Code\n    Code\n  Code\nCode"));
    }
}

/*
```
|**
 * Code
 *
 * Code
 -/
```

*/

TEST_CASE("143")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/143.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(doc->items().at(1).get());
    CHECK_POSITIONS(c, 0, 1, 2, 5);
    REQUIRE(c->startDelim() == MD::WithPosition{0, 0, 2, 0});
    REQUIRE(c->endDelim() == MD::WithPosition{0, 6, 2, 6});
    REQUIRE(c->syntaxPos() == MD::WithPosition{-1, -1, -1, -1});

    REQUIRE(c->text() == QStringLiteral("/**\n * Code\n *\n * Code\n */"));
}

/*
**text (*).**

*/
TEST_CASE("144")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/144.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 12, 0);

    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    CHECK_POSITIONS(t, 2, 0, 10, 0);

    REQUIRE(t->opts() == MD::BoldText);
    REQUIRE(t->text() == QStringLiteral("text (*)."));
    REQUIRE(t->openStyles().size() == 1);
    REQUIRE(t->openStyles().at(0) == MD::StyleDelim{MD::BoldText, 0, 0, 1, 0});
    REQUIRE(t->closeStyles().size() == 1);
    REQUIRE(t->closeStyles().at(0) == MD::StyleDelim{MD::BoldText, 11, 0, 12, 0});
}

/*
text******text***

*/
TEST_CASE("145")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/145.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 16, 0);

    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 0, 6, 0);

        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("text***"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        CHECK_POSITIONS(t, 10, 0, 13, 0);

        REQUIRE(t->opts() == (MD::BoldText | MD::ItalicText));
        REQUIRE(t->text() == QStringLiteral("text"));
        REQUIRE(t->openStyles().size() == 2);
        REQUIRE(t->openStyles().at(0) == MD::StyleDelim{MD::ItalicText, 7, 0, 7, 0});
        REQUIRE(t->openStyles().at(1) == MD::StyleDelim{MD::BoldText, 8, 0, 9, 0});
        REQUIRE(t->closeStyles().size() == 2);
        REQUIRE(t->closeStyles().at(0) == MD::StyleDelim{MD::BoldText, 14, 0, 15, 0});
        REQUIRE(t->closeStyles().at(1) == MD::StyleDelim{MD::ItalicText, 16, 0, 16, 0});
    }
}

/*
******text***

*/
TEST_CASE("146")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/146.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 12, 0);

    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 0, 2, 0);

        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("***"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        CHECK_POSITIONS(t, 6, 0, 9, 0);

        REQUIRE(t->opts() == (MD::BoldText | MD::ItalicText));
        REQUIRE(t->text() == QStringLiteral("text"));
        REQUIRE(t->openStyles().size() == 2);
        REQUIRE(t->openStyles().at(0) == MD::StyleDelim{MD::ItalicText, 3, 0, 3, 0});
        REQUIRE(t->openStyles().at(1) == MD::StyleDelim{MD::BoldText, 4, 0, 5, 0});
        REQUIRE(t->closeStyles().size() == 2);
        REQUIRE(t->closeStyles().at(0) == MD::StyleDelim{MD::BoldText, 10, 0, 11, 0});
        REQUIRE(t->closeStyles().at(1) == MD::StyleDelim{MD::ItalicText, 12, 0, 12, 0});
    }
}

/*
***text******

*/
TEST_CASE("147")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/147.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 12, 0);

    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 3, 0, 6, 0);

        REQUIRE(t->opts() == (MD::BoldText | MD::ItalicText));
        REQUIRE(t->text() == QStringLiteral("text"));
        REQUIRE(t->openStyles().size() == 2);
        REQUIRE(t->openStyles().at(0) == MD::StyleDelim{MD::ItalicText, 0, 0, 0, 0});
        REQUIRE(t->openStyles().at(1) == MD::StyleDelim{MD::BoldText, 1, 0, 2, 0});
        REQUIRE(t->closeStyles().size() == 2);
        REQUIRE(t->closeStyles().at(0) == MD::StyleDelim{MD::BoldText, 7, 0, 8, 0});
        REQUIRE(t->closeStyles().at(1) == MD::StyleDelim{MD::ItalicText, 9, 0, 9, 0});
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        CHECK_POSITIONS(t, 10, 0, 12, 0);

        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("***"));
    }
}

/*
  - [link](#link) **text**

    - `item 1`
    - `item 2`

    <br />

    ```
    code
    ```

*/
TEST_CASE("148")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/148.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 2, 0, 0, 10);

    REQUIRE(l->items().size() == 1);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());

    REQUIRE(li->items().size() == 4);
    REQUIRE(li->delim() == MD::WithPosition{2, 0, 2, 0});

    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);

    {
        auto p = static_cast<MD::Paragraph *>(li->items().at(0).get());
        CHECK_POSITIONS(p, 4, 0, 25, 0);

        REQUIRE(p->items().size() == 3);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        CHECK_POSITIONS(l, 4, 0, 16, 0);
        REQUIRE(l->textPos() == MD::WithPosition{5, 0, 8, 0});
        REQUIRE(l->urlPos() == MD::WithPosition{11, 0, 15, 0});

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto s = static_cast<MD::Text *>(p->items().at(1).get());
        CHECK_POSITIONS(s, 17, 0, 17, 0);
        REQUIRE(s->opts() == MD::TextWithoutFormat);
        REQUIRE(s->text() == QStringLiteral(" "));
        REQUIRE(s->openStyles().size() == 0);
        REQUIRE(s->closeStyles().size() == 0);

        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        CHECK_POSITIONS(t, 20, 0, 23, 0);
        REQUIRE(t->opts() == MD::BoldText);
        REQUIRE(t->text() == QStringLiteral("text"));
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->openStyles().at(0) == MD::StyleDelim{MD::BoldText, 18, 0, 19, 0});
        REQUIRE(t->closeStyles().size() == 1);
        REQUIRE(t->closeStyles().at(0) == MD::StyleDelim{MD::BoldText, 24, 0, 25, 0});
    }

    REQUIRE(li->items().at(1)->type() == MD::ItemType::List);

    {
        auto l = static_cast<MD::List *>(li->items().at(1).get());
        CHECK_POSITIONS(l, 4, 2, 0, 4);
    }

    REQUIRE(li->items().at(2)->type() == MD::ItemType::RawHtml);

    REQUIRE(li->items().at(3)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(li->items().at(3).get());
    CHECK_POSITIONS(c, 4, 8, 7, 8);
    REQUIRE(c->startDelim() == MD::WithPosition{4, 7, 6, 7});
    REQUIRE(c->endDelim() == MD::WithPosition{4, 9, 6, 9});
    REQUIRE(c->syntaxPos() == MD::WithPosition{-1, -1, -1, -1});
    REQUIRE(c->text() == QStringLiteral("code"));
}

/*
* list
> quote

*/
TEST_CASE("149")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/149.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Blockquote);
    auto b = static_cast<MD::Blockquote *>(doc->items().at(2).get());
    REQUIRE(b->delims() == MD::Blockquote::Delims{{0, 1, 0, 1}});
}

/*
* list
  > quote

*/
TEST_CASE("150")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/150.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
}

/*
* list
> quote
* list
# head
* list
  > quote
* list
  # head

*/
TEST_CASE("151")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/151.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 6);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Blockquote);
    auto b = static_cast<MD::Blockquote *>(doc->items().at(2).get());
    REQUIRE(b->delims() == MD::Blockquote::Delims{{0, 1, 0, 1}});
    REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);
    REQUIRE(doc->items().at(4)->type() == MD::ItemType::Heading);

    {
        auto h = static_cast<MD::Heading *>(doc->items().at(4).get());
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 3, 0, 3});
    }

    REQUIRE(doc->items().at(5)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(5).get());
    REQUIRE(l->items().size() == 2);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(li->items().size() == 2);
        REQUIRE(li->items().at(1)->type() == MD::ItemType::Blockquote);
        auto b = static_cast<MD::Blockquote *>(li->items().at(1).get());
        REQUIRE(b->delims() == MD::Blockquote::Delims{{2, 5, 2, 5}});
    }

    {
        REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(l->items().at(1).get());
        REQUIRE(li->items().size() == 2);
        REQUIRE(li->items().at(1)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(li->items().at(1).get());
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{2, 7, 2, 7});
    }
}

/*
* list

> quote

* list

# head

* list

  > quote

* list

  # head

*/
TEST_CASE("152")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/152.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 6);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Blockquote);

    {
        auto b = static_cast<MD::Blockquote *>(doc->items().at(2).get());
        REQUIRE(b->delims() == MD::Blockquote::Delims{{0, 2, 0, 2}});
    }

    REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);

    REQUIRE(doc->items().at(4)->type() == MD::ItemType::Heading);
    auto h = static_cast<MD::Heading *>(doc->items().at(4).get());
    REQUIRE(h->delims().size() == 1);
    REQUIRE(h->delims().front() == MD::WithPosition{0, 6, 0, 6});
    REQUIRE(doc->items().at(5)->type() == MD::ItemType::List);

    {
        auto l = static_cast<MD::List *>(doc->items().at(5).get());
        REQUIRE(l->items().size() == 2);

        {
            REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
            auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
            REQUIRE(li->items().size() == 2);
            REQUIRE(li->items().at(1)->type() == MD::ItemType::Blockquote);
            auto b = static_cast<MD::Blockquote *>(li->items().at(1).get());
            REQUIRE(b->delims() == MD::Blockquote::Delims{{2, 10, 2, 10}});
        }

        {
            REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);
            auto li = static_cast<MD::ListItem *>(l->items().at(1).get());
            REQUIRE(li->items().size() == 2);
            REQUIRE(li->items().at(1)->type() == MD::ItemType::Heading);
            auto h = static_cast<MD::Heading *>(li->items().at(1).get());
            REQUIRE(h->delims().size() == 1);
            REQUIRE(h->delims().front() == MD::WithPosition{2, 14, 2, 14});
        }
    }
}
