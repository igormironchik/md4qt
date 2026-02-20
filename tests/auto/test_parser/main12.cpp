/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include "parser.h"
#include "utils.h"

// Qt include.
#include <QDir>

/*
[](<a<>) []((a ))

*/
TEST_CASE("344")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/344.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
}

/*
[1]: /url (title())

[2]: /url ((title))

[3]: /url (title))

*/
TEST_CASE("345")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/345.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
}

TEST_CASE("346")
{
    REQUIRE(!MD::isEmail(QStringLiteral("mail@a#.net")));
}

/*
<a attr val>

<a attr / val>

<a 0a>

<a/

*/
TEST_CASE("347")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/347.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 5);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    }

    {
        REQUIRE(doc->items().at(4)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(4).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    }
}

/*
<!-- -> -->

*/
TEST_CASE("348")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/348.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
}

/*
<? ? ?>

*/
TEST_CASE("349")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/349.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
}

/*
<![CDATA[ ]> ]]>

*/
TEST_CASE("350")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/350.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
}

/*

$$\[\mathrm{\mathbf{M}}(\alpha) =
   \left(
      \begin{matrix}
         \cos(\alpha)+n_x^2\cdot (1-\cos(\alpha))  &  n_x\cdot n_y\cdot (1-\cos(\alpha))-n_z\cdot \sin(\alpha) &
n_x\cdot n_z\cdot (1-\cos(\alpha))+n_y\cdot \sin(\alpha)\\
         n_x\cdot n_y\cdot (1-\cos(\alpha))+n_z\cdot \sin(\alpha) & \cos(\alpha)+n_y^2\cdot (1-\cos(\alpha))  & n_y\cdot
n_z\cdot (1-\cos(\alpha))-n_x\cdot \sin(\alpha)\\ n_z\cdot n_x\cdot (1-\cos(\alpha))-n_y\cdot \sin(\alpha) & n_z\cdot
n_y\cdot (1-\cos(\alpha))+n_x\cdot \sin(\alpha)  & \cos(\alpha)+n_z^2\cdot (1-\cos(\alpha))
      \end{matrix}
   \right)
\]$$

*/
TEST_CASE("351")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/351.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 1);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 9);

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
    REQUIRE(m->startColumn() == 2);
    REQUIRE(m->startLine() == 1);
    REQUIRE(m->endColumn() == 1);
    REQUIRE(m->endLine() == 9);
    REQUIRE(m->startDelim() == MD::WithPosition{0, 1, 1, 1});
    REQUIRE(m->endDelim() == MD::WithPosition{2, 9, 3, 9});
}

/*
- list item
  - nested list item
    > quoteblock

*/
TEST_CASE("352")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/352.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(li->items().size() == 2);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List *>(li->items().at(1).get());
        REQUIRE(l->items().size() == 1);
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

        {
            auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
            REQUIRE(li->items().size() == 2);
            REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
            REQUIRE(li->items().at(1)->type() == MD::ItemType::Blockquote);
        }
    }
}

/*
- list item
  - nested list item
  > quoteblock

*/
TEST_CASE("353")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/353.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->items().size() == 3);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
    REQUIRE(li->items().at(2)->type() == MD::ItemType::Blockquote);
}

/*
- list item
  - nested list item
> quote block
*/
TEST_CASE("354")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/354.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->items().size() == 2);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::List);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Blockquote);
}

/*
# tool
# tool
# tool
*/
TEST_CASE("355")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/355.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->labeledHeadings().size() == 3);

    const QString path = QDir().absolutePath() + QStringLiteral("/tests/parser/data/355.md");

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(1).get());
        const QString label = QStringLiteral("#tool/") + path;
        REQUIRE(h->label() == label);
        REQUIRE(doc->labeledHeadings().contains(label));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(2).get());
        const QString label = QStringLiteral("#tool-1/") + path;
        REQUIRE(h->label() == label);
        REQUIRE(doc->labeledHeadings().contains(label));
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(3).get());
        const QString label = QStringLiteral("#tool-2/") + path;
        REQUIRE(h->label() == label);
        REQUIRE(doc->labeledHeadings().contains(label));
    }
}

/*
# tool
# tool
# tool 1

*/
TEST_CASE("356")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/356.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->labeledHeadings().size() == 3);

    const QString path = QDir().absolutePath() + QStringLiteral("/tests/parser/data/356.md");

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(1).get());
        const QString label = QStringLiteral("#tool/") + path;
        REQUIRE(h->label() == label);
        REQUIRE(doc->labeledHeadings().contains(label));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(2).get());
        const QString label = QStringLiteral("#tool-1/") + path;
        REQUIRE(h->label() == label);
        REQUIRE(doc->labeledHeadings().contains(label));
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(3).get());
        const QString label = QStringLiteral("#tool-1-1/") + path;
        REQUIRE(h->label() == label);
        REQUIRE(doc->labeledHeadings().contains(label));
    }
}
