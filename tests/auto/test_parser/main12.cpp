/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include "algo.h"
#include "asterisk_emphasis_parser.h"
#include "atx_heading_parser.h"
#include "constants.h"
#include "inline_context.h"
#include "paragraph_parser.h"
#include "parser.h"
#include "reverse_solidus.h"
#include "setext_heading_parser.h"
#include "text_stream.h"
#include "thematic_break_parser.h"
#include "utils.h"

// Qt include.
#include <QDir>
#include <QTextStream>

#include "test_utils.h"

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

TEST_CASE("autolink_uri_validation_config")
{
    QString markdown = QStringLiteral("<made-up-scheme://foo,bar>");

    {
        QTextStream stream(&markdown);
        MD::Parser parser;
        auto doc = parser.parse(stream, QString(), QString());

        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    }

    {
        QTextStream stream(&markdown);
        MD::Parser parser;
        parser.setAutolinkUriValidation(MD::Parser::AutolinkUriValidation::CommonMark);
        auto doc = parser.parse(stream, QString(), QString());

        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(0).get());
        REQUIRE(l->url() == markdown.sliced(1, markdown.length() - 2));
    }
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
    CHECK_POSITIONS(p, 0, 1, 3, 9);

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
    CHECK_POSITIONS(m, 2, 1, 1, 9);
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

/*
```math
*/
TEST_CASE("359")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/359.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Math);
    auto m = static_cast<MD::Math *>(p->items().at(0).get());
    CHECK_POSITIONS(m, 7, 0, 7, 0);
}

/*
```math
```

*/
TEST_CASE("360")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/360.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Math);
    auto m = static_cast<MD::Math *>(p->items().at(0).get());
    CHECK_POSITIONS(m, 7, 0, 7, 0);
}

/*
text[^1]



[^1]: note

*/
TEST_CASE("361")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/361.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    REQUIRE(doc->footnotesMap().size() == 1);
}

/*
[ref]: <<>

[ref]:
> quote

*/
TEST_CASE("362")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/362.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    REQUIRE(doc->items().at(3)->type() == MD::ItemType::Blockquote);
}

/*
*[](/url)to_skip*

*/
TEST_CASE("test_to_skip")
{
    const QString data = QStringLiteral("*[](/url)to_skip*");
    MD::Line line(data, 0);
    MD::ParagraphStream::HashedLines lines;
    lines.insert(0, line);
    MD::ParagraphStream stream(lines, 0, 0);
    MD::InlineContext ctx;
    ctx.openStyles().append(MD::StyleDelim(MD::TextOption::ItalicText, 0, 0, 0, 0));
    auto link = QSharedPointer<MD::Link>::create();
    link->setStartColumn(1);
    link->setStartLine(0);
    link->setEndColumn(8);
    link->setEndLine(0);
    ctx.inlines().append(link);
    ctx.closeStyles().append(MD::StyleDelim(MD::TextOption::ItalicText, 16, 0, 16, 0));
    auto p = QSharedPointer<MD::Paragraph>::create();
    MD::ParagraphParser::makeTextObjects(ctx, stream, p, MD::WithPosition(9, 0, 15, 0));

    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->openStyles().size() == 1);
    REQUIRE(l->closeStyles().size() == 1);
}

TEST_CASE("paragraph_to_label")
{
    REQUIRE(MD::ParagraphParser::paragraphToLabel(nullptr).isEmpty());

    QSharedPointer<MD::Paragraph> p(new MD::Paragraph);

    QSharedPointer<MD::Image> img1(new MD::Image);

    QSharedPointer<MD::Text> text1(new MD::Text);
    text1->setText(QStringLiteral("text1"));

    img1->p()->appendItem(text1);

    QSharedPointer<MD::Image> img2(new MD::Image);
    img2->setText(QStringLiteral("img2"));

    QSharedPointer<MD::Link> link1(new MD::Link);
    link1->setText(QStringLiteral("link1"));

    p->appendItem(img1);
    p->appendItem(img2);
    p->appendItem(link1);

    REQUIRE(MD::ParagraphParser::paragraphToLabel(p) == QStringLiteral("text1img2link1"));
}

TEST_CASE("test_stream")
{
    {
        QString data = QStringLiteral("str");
        QTextStream sstream(&data);
        MD::TextStream stream(sstream);

        REQUIRE(stream.currentLine().lineNumber() == -1);

        REQUIRE(stream.moveTo(0).lineNumber() == -1);

        REQUIRE(stream.readLine().lineNumber() == 0);
    }

    {
        QString data = QStringLiteral("");
        QTextStream sstream(&data);
        MD::TextStream stream(sstream);

        REQUIRE(stream.currentLine().lineNumber() == -1);

        REQUIRE(stream.moveTo(0).lineNumber() == -1);

        REQUIRE(stream.readLine().lineNumber() == -1);
    }
}

TEST_CASE("line")
{
    QString data(QChar(0x00));
    MD::Line line(data, 0);

    REQUIRE(line.currentChar() == MD::s_replaceChar);
    REQUIRE(line.sliced(0).currentChar() == MD::s_replaceChar);
}

TEST_CASE("functions")
{
    {
        const QString data = QStringLiteral("a\nb");
        MD::Line line(data, 0);
        REQUIRE(MD::readLinkDestination(line).isEmpty());
    }

    {
        const QString data = QStringLiteral("a");
        MD::Line line(data, 0);
        line.saveState();
        REQUIRE(MD::readLinkDestination(line) == QStringLiteral("a"));
        line.restoreState();
        line.nextChar();
        REQUIRE(MD::readLinkDestination(line).isEmpty());
    }

    {
        QString en = QStringLiteral("&#x0;");
        MD::replaceEntity(en);
        REQUIRE(en == MD::s_replaceChar);
    }

    {
        const QString data = QStringLiteral("");
        MD::Line line(data, 0);
        QChar startChar;
        int startParenthesisCount;
        qsizetype startPos;
        bool endStarted;
        REQUIRE(MD::readLinkTitle(line, startChar, startParenthesisCount, startPos, endStarted).isEmpty());
    }

    {
        const QString data = QStringLiteral("");
        MD::Line line(data, 0);
        REQUIRE(MD::readEscapedSequence(line).isEmpty());
    }

    {
        const MD::StyleDelim s1(MD::TextOption::ItalicText, -1, -1, -1, -1);
        const MD::StyleDelim s2(MD::TextOption::ItalicText, 0, 0, 0, 0);

        REQUIRE(!(s1 < s2));
    }

    {
        const QString data = QStringLiteral("");
        MD::Line line(data, 0);

        REQUIRE(!MD::isClosed(line, 8, true));
    }

    {
        const QString data = QStringLiteral("aaa");
        REQUIRE(MD::skipIfBackward(3,
                                   data,
                                   [](const QChar &ch) {
                                       return ch == QLatin1Char('a');
                                   })
                == -1);
        REQUIRE(MD::skipIfBackward(-2,
                                   data,
                                   [](const QChar &ch) {
                                       return ch == QLatin1Char('a');
                                   })
                == -1);
    }
}

TEST_CASE("some_things")
{
    MD::Line line;
    QString str;
    QTextStream qstream(&str);
    MD::TextStream stream(qstream);
    QSharedPointer<MD::Document> doc(new MD::Document);
    MD::Context ctx;

    {
        MD::ThematicBreakParser tb(nullptr);

        REQUIRE(tb.continueCheck(line, stream, doc, ctx, {}, {}) == MD::BlockState::Stop);
    }

    {
        MD::SetextHeadingParser sh(nullptr);

        REQUIRE(sh.continueCheck(line, stream, doc, ctx, {}, {}) == MD::BlockState::Stop);
    }
}

MD::ItemWithOpts::Styles makeStylesByTwo(int style,
                                         qsizetype startPos,
                                         qsizetype lineNumber,
                                         qsizetype length)
{
    MD::ItemWithOpts::Styles styles;

    for (auto i = 0; i < length; i += 2) {
        styles.append(MD::StyleDelim(style, startPos + i, lineNumber, startPos + i + 1, lineNumber));
    }

    return styles;
}

class SubEmphasisParser : public MD::EmphasisParser
{
public:
    SubEmphasisParser()
        : m_symbol(QLatin1Char('-'))
    {
    }

    ~SubEmphasisParser() override = default;

    const QChar &symbol() const override
    {
        return m_symbol;
    }

    bool isEmphasis(int length) const override
    {
        return (length % 2 == 0);
    }

    bool isLengthCorrespond() const override
    {
        return false;
    }

    MD::ItemWithOpts::Styles openStyles(qsizetype startPos,
                                        qsizetype lineNumber,
                                        qsizetype length) const override
    {
        return makeStylesByTwo(8, startPos, lineNumber, length);
    }

    MD::ItemWithOpts::Styles closeStyles(qsizetype startPos,
                                         qsizetype lineNumber,
                                         qsizetype length) const override
    {
        return makeStylesByTwo(8, startPos, lineNumber, length);
    }

private:
    const QChar m_symbol;
}; // class SubEmphasisParser

class HighlightEmphasisParser : public MD::EmphasisParser
{
public:
    HighlightEmphasisParser()
        : m_symbol(QLatin1Char('='))
    {
    }
    ~HighlightEmphasisParser() override = default;

    const QChar &symbol() const override
    {
        return m_symbol;
    }

    bool isEmphasis(int length) const override
    {
        return (length % 2 == 0);
    }

    bool isLengthCorrespond() const override
    {
        return false;
    }

    MD::ItemWithOpts::Styles openStyles(qsizetype startPos,
                                        qsizetype lineNumber,
                                        qsizetype length) const override
    {
        return makeStylesByTwo(16, startPos, lineNumber, length);
    }

    MD::ItemWithOpts::Styles closeStyles(qsizetype startPos,
                                         qsizetype lineNumber,
                                         qsizetype length) const override
    {
        return makeStylesByTwo(16, startPos, lineNumber, length);
    }

private:
    const QChar m_symbol;
}; // class HighlightEmphasisParser

/*
--===--==--==--==

*/
TEST_CASE("363")
{
    MD::Parser parser;

    MD::Parser::InlineParsers inlineParsers;

    MD::Parser::appendInlineParser<SubEmphasisParser>(inlineParsers);
    MD::Parser::appendInlineParser<HighlightEmphasisParser>(inlineParsers);

    parser.setInlineParsers(inlineParsers);

    auto doc = parser.parse(QStringLiteral("tests/parser/data/363.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t1 = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t1->openStyles().size() == 1);
    REQUIRE(t1->closeStyles().size() == 1);
    REQUIRE(t1->text() == QStringLiteral("==="));
    REQUIRE(t1->opts() == 8);

    REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
    auto t2 = static_cast<MD::Text *>(p->items().at(1).get());
    REQUIRE(t2->openStyles().size() == 1);
    REQUIRE(t2->closeStyles().size() == 1);
    REQUIRE(t2->text() == QStringLiteral("--"));
    REQUIRE(t2->opts() == 16);

    REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
    auto t3 = static_cast<MD::Text *>(p->items().at(2).get());
    REQUIRE(t3->openStyles().size() == 0);
    REQUIRE(t3->closeStyles().size() == 0);
    REQUIRE(t3->text() == QStringLiteral("--=="));
    REQUIRE(t3->opts() == MD::TextOption::TextWithoutFormat);
}

/*
* ```
*

*/
TEST_CASE("364")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/364.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 2);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->items().size() == 1);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(li->items().at(0).get());
    REQUIRE(c->startDelim() == MD::WithPosition{2, 0, 4, 0});
    CHECK_POSITIONS(c, 5, 0, 5, 0);
    REQUIRE(c->endDelim().isNullPositions());
}

/*
* [x] text
  ---

*/
TEST_CASE("365")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/365.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->items().size() == 1);
    REQUIRE(!li->isTaskList());
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Heading);
    auto h = static_cast<MD::Heading *>(li->items().at(0).get());
    REQUIRE(h->text()->items().size() == 1);
    REQUIRE(h->text()->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(h->text()->items().at(0).get());
    REQUIRE(t->text() == QStringLiteral("[x] text"));
}

/*
* ```yaml
*

*/
TEST_CASE("366")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/366.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 2);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->items().size() == 1);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(li->items().at(0).get());
    REQUIRE(c->startDelim() == MD::WithPosition{2, 0, 4, 0});
    REQUIRE(c->syntaxPos() == MD::WithPosition{5, 0, 8, 0});
    CHECK_POSITIONS(c, 10, 0, 10, 0);
    REQUIRE(c->endDelim().isNullPositions());
}

/*
```cpp

*/
TEST_CASE("367")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/367.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(doc->items().at(1).get());
    REQUIRE(c->startDelim() == MD::WithPosition{0, 0, 2, 0});
    REQUIRE(c->syntaxPos() == MD::WithPosition{3, 0, 5, 0});
    REQUIRE(c->endDelim().isNullPositions());
    CHECK_POSITIONS(c, 9, 0, 9, 0);
}

/*
* ```cpp

* ```cpp

*/
TEST_CASE("368")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/368.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 2);

    {
        auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(li->items().size() == 1);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code *>(li->items().at(0).get());
        REQUIRE(c->startDelim() == MD::WithPosition{2, 0, 4, 0});
        REQUIRE(c->endDelim().isNullPositions());
    }

    {
        auto li = static_cast<MD::ListItem *>(l->items().at(1).get());
        REQUIRE(li->items().size() == 1);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code *>(li->items().at(0).get());
        REQUIRE(c->startDelim() == MD::WithPosition{2, 2, 4, 2});
        REQUIRE(c->endDelim().isNullPositions());
    }
}

TEST_CASE("ascii_control_in_autolink")
{
    QString markdown = QStringLiteral("<https://www.");
    markdown.append(QLatin1Char(0x02));
    markdown.append(QStringLiteral("google.com"));

    QTextStream stream(&markdown);
    MD::Parser parser;
    auto doc = parser.parse(stream, QString(), QString());

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->text() == markdown);
}

/*
text[^1] text[^2]

[^1]: foot1
[^2]: foot2

text[^1] text[^2]

*/
TEST_CASE("369")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/369.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 4);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(p->items().at(1)->type() == MD::ItemType::FootnoteRef);
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        REQUIRE(p->items().at(3)->type() == MD::ItemType::FootnoteRef);
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 4);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(p->items().at(1)->type() == MD::ItemType::FootnoteRef);
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        REQUIRE(p->items().at(3)->type() == MD::ItemType::FootnoteRef);
    }

    REQUIRE(doc->footnotesMap().size() == 2);

    for (auto it = doc->footnotesMap().cbegin(), last = doc->footnotesMap().cend(); it != last; ++it) {
        REQUIRE((*it)->items().size() == 1);
    }
}

/*
text[^1] text[^2]

[^1]: foot1
[^2]: foot2
*/
TEST_CASE("370")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/370.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 4);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(p->items().at(1)->type() == MD::ItemType::FootnoteRef);
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        REQUIRE(p->items().at(3)->type() == MD::ItemType::FootnoteRef);
    }

    REQUIRE(doc->footnotesMap().size() == 2);

    for (auto it = doc->footnotesMap().cbegin(), last = doc->footnotesMap().cend(); it != last; ++it) {
        REQUIRE((*it)->items().size() == 1);
    }
}

TEST_CASE("block_parser")
{
    MD::Parser parser;
    MD::ParagraphParser p(&parser);
    MD::Context ctx;
    REQUIRE(p.currentBlock(ctx) == nullptr);
}

/*
text[^1] text[^2]

[^1]: foot1
[^2]: foot2
*/
TEST_CASE("370-1")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/370.md"), QStringLiteral("/"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 4);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        REQUIRE(p->items().at(1)->type() == MD::ItemType::FootnoteRef);
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        REQUIRE(p->items().at(3)->type() == MD::ItemType::FootnoteRef);
    }

    REQUIRE(doc->footnotesMap().size() == 2);

    for (auto it = doc->footnotesMap().cbegin(), last = doc->footnotesMap().cend(); it != last; ++it) {
        REQUIRE((*it)->items().size() == 1);
    }
}

TEST_CASE("text_stream")
{
    struct My : public MD::Item {
        MD::ItemType type() const override
        {
            return MD::ItemType::UserDefined;
        }

        QSharedPointer<MD::Item> clone(MD::Document *doc = nullptr) const override
        {
            Q_UNUSED(doc)

            return {};
        }
    };

    auto doc = QSharedPointer<MD::Document>::create();

    auto my = QSharedPointer<My>::create();

    auto b = QSharedPointer<MD::Blockquote>::create();
    b->appendItem(my);

    auto l = QSharedPointer<MD::List>::create();
    auto li = QSharedPointer<MD::ListItem>::create();
    li->appendItem(my);
    l->appendItem(li);

    auto t = QSharedPointer<MD::Table>::create();
    auto r = QSharedPointer<MD::TableRow>::create();
    t->appendRow(r);
    auto c = QSharedPointer<MD::TableCell>::create();
    c->appendItem(my);
    r->appendCell(c);
    t->setColumnAlignment(0, MD::Table::AlignLeft);

    auto f = QSharedPointer<MD::Footnote>::create();
    f->appendItem(my);

    doc->insertFootnote(QStringLiteral("f1"), f);

    doc->appendItem(b);
    doc->appendItem(l);
    doc->appendItem(t);

    int count = 0;

    MD::forEach(QVector<MD::ItemType>() << MD::ItemType::UserDefined, doc, [&count](MD::Item *) {
        ++count;
    });

    REQUIRE(count == 4);
}

/*
text <?a?> <!a> <![CDATA[a]]>

*/
TEST_CASE("371")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/371.md"), QStringLiteral("/"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 6);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<?a?>"));
    }

    REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(3).get());
        REQUIRE(h->text() == QStringLiteral("<!a>"));
    }

    REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);

    {
        REQUIRE(p->items().at(5)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(5).get());
        REQUIRE(h->text() == QStringLiteral("<![CDATA[a]]>"));
    }
}

/*
```
    ```<lang>
        code goes here
    ```
```
Text

Text

*/
TEST_CASE("372")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/372.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code *>(doc->items().at(1).get());
        REQUIRE(c->startDelim() == MD::WithPosition{0, 0, 2, 0});
        REQUIRE(c->endDelim() == MD::WithPosition{0, 4, 2, 4});
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        CHECK_POSITIONS(p, 0, 5, 3, 5);
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        CHECK_POSITIONS(p, 0, 7, 3, 7);
    }
}

/*
1. Ordered item
   > - Bullet inside quote
   > - Another bullet

*/
TEST_CASE("373")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/373.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->listType() == MD::ListItem::Ordered);
    REQUIRE(li->items().size() == 2);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::Blockquote);
    auto b = static_cast<MD::Blockquote *>(li->items().at(1).get());
    REQUIRE(b->items().size() == 1);
    REQUIRE(b->items().at(0)->type() == MD::ItemType::List);
    auto l1 = static_cast<MD::List *>(b->items().at(0).get());
    REQUIRE(l1->items().size() == 2);

    {
        REQUIRE(l1->items().at(0)->type() == MD::ItemType::ListItem);
        auto li1 = static_cast<MD::ListItem *>(l1->items().at(0).get());
        REQUIRE(li1->items().size() == 1);
        REQUIRE(li1->items().at(0)->type() == MD::ItemType::Paragraph);
    }

    {
        REQUIRE(l1->items().at(1)->type() == MD::ItemType::ListItem);
        auto li1 = static_cast<MD::ListItem *>(l1->items().at(1).get());
        REQUIRE(li1->items().size() == 1);
        REQUIRE(li1->items().at(0)->type() == MD::ItemType::Paragraph);
    }
}

/*
1. Ordered item
   > - Bullet inside quote
   > - Another bullet
   >
   > ```cpp
   > // this is some code
   > ```

*/
TEST_CASE("374")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/374.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->listType() == MD::ListItem::Ordered);
    REQUIRE(li->items().size() == 2);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::Blockquote);
    auto b = static_cast<MD::Blockquote *>(li->items().at(1).get());
    REQUIRE(b->items().size() == 2);
    REQUIRE(b->items().at(0)->type() == MD::ItemType::List);
    auto l1 = static_cast<MD::List *>(b->items().at(0).get());
    REQUIRE(l1->items().size() == 2);

    {
        REQUIRE(l1->items().at(0)->type() == MD::ItemType::ListItem);
        auto li1 = static_cast<MD::ListItem *>(l1->items().at(0).get());
        REQUIRE(li1->items().size() == 1);
        REQUIRE(li1->items().at(0)->type() == MD::ItemType::Paragraph);
    }

    {
        REQUIRE(l1->items().at(1)->type() == MD::ItemType::ListItem);
        auto li1 = static_cast<MD::ListItem *>(l1->items().at(1).get());
        REQUIRE(li1->items().size() == 1);
        REQUIRE(li1->items().at(0)->type() == MD::ItemType::Paragraph);
    }

    REQUIRE(b->items().at(1)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(b->items().at(1).get());
    REQUIRE(c->isFensedCode());
}

/*
1. Ordered item
   > - Bullet inside quote
   > - Another bullet
   > ```cpp
   > // this is some code
   > ```

*/
TEST_CASE("375")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/375.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 6);
    REQUIRE(l->items().size() == 1);
    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    CHECK_POSITIONS(li, 0, 0, 0, 6);
    REQUIRE(li->listType() == MD::ListItem::Ordered);
    REQUIRE(li->items().size() == 2);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::Blockquote);
    auto b = static_cast<MD::Blockquote *>(li->items().at(1).get());
    CHECK_POSITIONS(b, 3, 1, 7, 5);
    REQUIRE(b->items().size() == 2);
    REQUIRE(b->items().at(0)->type() == MD::ItemType::List);
    auto l1 = static_cast<MD::List *>(b->items().at(0).get());
    CHECK_POSITIONS(l1, 5, 1, 20, 2);
    REQUIRE(l1->items().size() == 2);

    {
        REQUIRE(l1->items().at(0)->type() == MD::ItemType::ListItem);
        auto li1 = static_cast<MD::ListItem *>(l1->items().at(0).get());
        CHECK_POSITIONS(li1, 5, 1, 25, 1);
        REQUIRE(li1->items().size() == 1);
        REQUIRE(li1->items().at(0)->type() == MD::ItemType::Paragraph);
    }

    {
        REQUIRE(l1->items().at(1)->type() == MD::ItemType::ListItem);
        auto li1 = static_cast<MD::ListItem *>(l1->items().at(1).get());
        CHECK_POSITIONS(li1, 5, 2, 20, 2);
        REQUIRE(li1->items().size() == 1);
        REQUIRE(li1->items().at(0)->type() == MD::ItemType::Paragraph);
    }

    REQUIRE(b->items().at(1)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(b->items().at(1).get());
    CHECK_POSITIONS(c, 5, 4, 24, 4);
    REQUIRE(c->isFensedCode());
}
