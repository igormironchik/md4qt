/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include "asterisk_emphasis_parser.h"
#include "atx_heading_parser.h"
#include "inline_context.h"
#include "parser.h"
#include "reverse_solidus.h"
#include "text_stream.h"
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

/*
# tool
# tool

[](357-1.md)

*/
TEST_CASE("357")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/357.md"), true);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 8);
    REQUIRE(doc->labeledHeadings().size() == 4);

    const QString path357 = QDir().absolutePath() + QStringLiteral("/tests/parser/data/357.md");

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(1).get());
        const QString label = QStringLiteral("#tool/") + path357;
        REQUIRE(h->label() == label);
        REQUIRE(doc->labeledHeadings().contains(label));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(2).get());
        const QString label = QStringLiteral("#tool-1/") + path357;
        REQUIRE(h->label() == label);
        REQUIRE(doc->labeledHeadings().contains(label));
    }

    REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
    REQUIRE(doc->items().at(4)->type() == MD::ItemType::PageBreak);
    REQUIRE(doc->items().at(5)->type() == MD::ItemType::Anchor);

    const QString path357_1 = QDir().absolutePath() + QStringLiteral("/tests/parser/data/357-1.md");

    {
        REQUIRE(doc->items().at(6)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(6).get());
        const QString label = QStringLiteral("#tool/") + path357_1;
        REQUIRE(h->label() == label);
        REQUIRE(doc->labeledHeadings().contains(label));
    }

    {
        REQUIRE(doc->items().at(7)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(7).get());
        const QString label = QStringLiteral("#tool-1/") + path357_1;
        REQUIRE(h->label() == label);
        REQUIRE(doc->labeledHeadings().contains(label));
    }

    {
        MD::ATXHeadingParser atx(nullptr);

        MD::Line line;
        QString str;
        QTextStream qstream(&str);
        MD::TextStream stream(qstream);
        QSharedPointer<MD::Document> doc(new MD::Document);
        MD::Context ctx;

        REQUIRE(atx.continueCheck(line, stream, doc, ctx, {}, {}) == MD::BlockState::Stop);
    }
}

/*
# heading


> quote

*/
TEST_CASE("358")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/358.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Blockquote);
}

TEST_CASE("doc")
{
    {
        MD::StyleDelim style{MD::TextOption::BoldText, 0, 0, 0, 0};
        REQUIRE(style.style() == MD::TextOption::BoldText);
        style.setStyle(MD::TextOption::ItalicText);
        REQUIRE(style.style() == MD::TextOption::ItalicText);

        MD::Text text;
        text.openStyles() = {style};
        text.closeStyles() = {style};

        const auto &textConstRef = text;
        REQUIRE(textConstRef.openStyles().size() == 1);
        REQUIRE(textConstRef.closeStyles().size() == 1);
    }

    {
        MD::Blockquote quote;
        quote.delims().append(MD::WithPosition{});
        quote.delims().append(MD::WithPosition{});
        quote.appendItem(QSharedPointer<MD::Text>::create());
        quote.appendItem(QSharedPointer<MD::Text>::create());

        REQUIRE(quote.items().size() == 2);
        REQUIRE(quote.delims().size() == 2);

        quote.insertItem(1, QSharedPointer<MD::Code>::create(QString(), false, true));

        REQUIRE(quote.items().size() == 3);
        REQUIRE(quote.items().at(1)->type() == MD::ItemType::Code);

        const auto &quoteConstRef = quote;

        REQUIRE(quoteConstRef.delims().size() == 2);
    }

    {
        MD::Heading heading;
        heading.setLabelVariants(MD::Heading::LabelsVector() << QStringLiteral("1") << QStringLiteral("2"));

        REQUIRE(heading.labelVariants().size() == 2);

        const auto &headingConstRef = heading;

        REQUIRE(headingConstRef.labelVariants().size() == 2);
    }

    {
        const auto delim = MD::WithPosition{0, 0, 0, 0};
        MD::ListItem item;
        item.setTaskDelim(delim);
        REQUIRE(item.taskDelim() == delim);
    }

    {
        MD::TableRow row;
        REQUIRE(row.isEmpty());
    }

    {
        MD::Table table;
        table.setColumnAlignment(0, MD::Table::AlignLeft);

        REQUIRE(table.columnAlignment(0) == MD::Table::AlignLeft);

        table.setColumnAlignment(0, MD::Table::AlignRight);

        REQUIRE(table.columnAlignment(0) == MD::Table::AlignRight);
    }

    {
        MD::FootnoteRef ref(QStringLiteral("1"));
        const auto idPos = MD::WithPosition{0, 0, 0, 0};
        ref.setIdPos(idPos);

        REQUIRE(ref.idPos() == idPos);

        const auto clone = ref.clone().staticCast<MD::FootnoteRef>();

        REQUIRE(clone->idPos() == idPos);
    }
}

TEST_CASE("whitespace")
{
    MD::AsteriskEmphasisParser e;
    MD::ReverseSolidusHandler rs;
    QString data = QStringLiteral("*a");
    data.prepend(QLatin1Char(0x0C));
    MD::Line line(data, 0);
    rs.process(line.currentChar());
    line.nextChar();
    rs.next();

    MD::ParagraphStream::HashedLines lines;
    lines.insert(0, line);
    MD::ParagraphStream stream(lines, 0, 0);

    MD::InlineContext ctx;

    auto doc = QSharedPointer<MD::Document>::create();

    QStringList linksToParse;
    MD::Parser parser;

    REQUIRE(e.check(line, stream, ctx, doc, QString(), QString(), linksToParse, parser, rs));
}
