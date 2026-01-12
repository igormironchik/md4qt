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
#include <QFile>

inline QString to_string(int i)
{
    return QString::number(i);
}

/*
<!DOCTYPE html>

*/
TEST_CASE("091")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/091.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<!DOCTYPE html>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 14);
    REQUIRE(h->endLine() == 0);
}

/*
Text <!DOCTYPE html>

*/
TEST_CASE("092")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/092.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 19);
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
    REQUIRE(h->text() == QStringLiteral("<!DOCTYPE html>"));
    REQUIRE(h->startColumn() == 5);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 19);
    REQUIRE(h->endLine() == 0);
}

/*
<!DOCTYPE html> Text

*/
TEST_CASE("093")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/093.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<!DOCTYPE html> Text"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 19);
    REQUIRE(h->endLine() == 0);
}

/*
<!DOCTYPE html>
Text

*/
TEST_CASE("094")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/094.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<!DOCTYPE html>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 14);
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
<!DOCTYPE html>

*/
TEST_CASE("095")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/095.md"));

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
    REQUIRE(h->text() == QStringLiteral("<!DOCTYPE html>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endColumn() == 14);
    REQUIRE(h->endLine() == 1);
}

/*
Text <!DOCTYPE html>
Text

*/
TEST_CASE("096")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/096.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 1);
    REQUIRE(p->items().size() == 3);

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
    REQUIRE(h->text() == QStringLiteral("<!DOCTYPE html>"));
    REQUIRE(h->startColumn() == 5);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 19);
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
<!DOCTYPE html>
Text

*/
TEST_CASE("097")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/097.md"));

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
    REQUIRE(h->text() == QStringLiteral("<!DOCTYPE html>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endColumn() == 14);
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
<![CDATA[
function matchwo(a,b)
{
  if (a < b && a < 0) then {
    return 1;

  } else {

    return 0;
  }
}
]]>

*/
TEST_CASE("098")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/098.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text()
            == QStringLiteral("<![CDATA[\n"
                              "function matchwo(a,b)\n"
                              "{\n"
                              "  if (a < b && a < 0) then {\n"
                              "    return 1;\n\n"
                              "  } else {\n\n"
                              "    return 0;\n"
                              "  }\n"
                              "}\n"
                              "]]>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 2);
    REQUIRE(h->endLine() == 11);
}

/*
Text <![CDATA[
function matchwo(a,b)
{
  if (a < b && a < 0) then {
    return 1;

  } else {

    return 0;
  }
}
]]>

*/
TEST_CASE("099")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/099.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 5);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 12);
        REQUIRE(p->endLine() == 4);
        REQUIRE(p->items().size() == 5);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Text <![CDATA["));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("function matchwo(a,b)"));
        }

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(2).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("{"));
        }

        {
            REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(3).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("if (a < b && a < 0) then {"));
        }

        {
            REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(4).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("return 1;"));
        }
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 6);
        REQUIRE(p->endColumn() == 9);
        REQUIRE(p->endLine() == 6);
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("} else {"));
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code *>(doc->items().at(3).get());
        REQUIRE(c->text() == QStringLiteral("return 0;"));
    }

    {
        REQUIRE(doc->items().at(4)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(4).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 9);
        REQUIRE(p->endColumn() == 2);
        REQUIRE(p->endLine() == 11);
        REQUIRE(p->items().size() == 3);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("}"));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("}"));
        }

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(2).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("]]>"));
        }
    }
}

/*
<![CDATA[
function matchwo(a,b)
{
  if (a < b && a < 0) then {
    return 1;

  } else {

    return 0;
  }
}
]]> Text

*/
TEST_CASE("100")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/100.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text()
            == QStringLiteral("<![CDATA[\n"
                              "function matchwo(a,b)\n"
                              "{\n"
                              "  if (a < b && a < 0) then {\n"
                              "    return 1;\n\n"
                              "  } else {\n\n"
                              "    return 0;\n"
                              "  }\n"
                              "}\n"
                              "]]> Text"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 7);
    REQUIRE(h->endLine() == 11);
}

/*
<![CDATA[
function matchwo(a,b)
{
  if (a < b && a < 0) then {
    return 1;

  } else {

    return 0;
  }
}
]]>
Text

*/
TEST_CASE("101")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/101.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text()
            == QStringLiteral("<![CDATA[\n"
                              "function matchwo(a,b)\n"
                              "{\n"
                              "  if (a < b && a < 0) then {\n"
                              "    return 1;\n\n"
                              "  } else {\n\n"
                              "    return 0;\n"
                              "  }\n"
                              "}\n"
                              "]]>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 2);
    REQUIRE(h->endLine() == 11);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 12);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 12);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    REQUIRE(t->startColumn() == 0);
    REQUIRE(t->startLine() == 12);
    REQUIRE(t->endColumn() == 3);
    REQUIRE(t->endLine() == 12);
}

/*
Text
<![CDATA[
function matchwo(a,b)
{
  if (a < b && a < 0) then {
    return 1;

  } else {

    return 0;
  }
}
]]>

*/
TEST_CASE("102")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/102.md"));

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
    REQUIRE(h->text()
            == QStringLiteral("<![CDATA[\n"
                              "function matchwo(a,b)\n"
                              "{\n"
                              "  if (a < b && a < 0) then {\n"
                              "    return 1;\n\n"
                              "  } else {\n\n"
                              "    return 0;\n"
                              "  }\n"
                              "}\n"
                              "]]>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endColumn() == 2);
    REQUIRE(h->endLine() == 12);
}

/*
Text <![CDATA[
function matchwo(a,b)
{
  if (a < b && a < 0) then {
    return 1;

  } else {

    return 0;
  }
}
]]>
Text

*/
TEST_CASE("103")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/103.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 5);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 12);
        REQUIRE(p->endLine() == 4);
        REQUIRE(p->items().size() == 5);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Text <![CDATA["));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("function matchwo(a,b)"));
        }

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(2).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("{"));
        }

        {
            REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(3).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("if (a < b && a < 0) then {"));
        }

        {
            REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(4).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("return 1;"));
        }
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 6);
        REQUIRE(p->endColumn() == 9);
        REQUIRE(p->endLine() == 6);
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("} else {"));
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code *>(doc->items().at(3).get());
        REQUIRE(c->text() == QStringLiteral("return 0;"));
    }

    {
        REQUIRE(doc->items().at(4)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(4).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 9);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 12);
        REQUIRE(p->items().size() == 4);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("}"));
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(1).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("}"));
        }

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(2).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("]]>"));
        }

        {
            REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(3).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Text"));
        }
    }
}

/*
Text
<![CDATA[
function matchwo(a,b)
{
  if (a < b && a < 0) then {
    return 1;

  } else {

    return 0;
  }
}
]]>
Text

*/
TEST_CASE("104")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/104.md"));

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
    REQUIRE(h->text()
            == QStringLiteral("<![CDATA[\n"
                              "function matchwo(a,b)\n"
                              "{\n"
                              "  if (a < b && a < 0) then {\n"
                              "    return 1;\n\n"
                              "  } else {\n\n"
                              "    return 0;\n"
                              "  }\n"
                              "}\n"
                              "]]>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endColumn() == 2);
    REQUIRE(h->endLine() == 12);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 13);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 13);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 13);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 13);
    }
}

/*
<form>

*/
TEST_CASE("105")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/105.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<form>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 5);
    REQUIRE(h->endLine() == 0);
}

/*
Text <form>

*/
TEST_CASE("106")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/106.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 10);
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
    REQUIRE(h->text() == QStringLiteral("<form>"));
    REQUIRE(h->startColumn() == 5);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 10);
    REQUIRE(h->endLine() == 0);
}

/*
<form> Text

*/
TEST_CASE("107")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/107.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<form> Text"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 10);
    REQUIRE(h->endLine() == 0);
}

/*
<form>
Text

*/
TEST_CASE("108")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/108.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<form>\nText"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 3);
    REQUIRE(h->endLine() == 1);
}

/*
Text
<form>

*/
TEST_CASE("109")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/109.md"));

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
    REQUIRE(h->text() == QStringLiteral("<form>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endColumn() == 5);
    REQUIRE(h->endLine() == 1);
}

/*
Text <form>
Text

*/
TEST_CASE("110")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/110.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 1);
    REQUIRE(p->items().size() == 3);

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
    REQUIRE(h->text() == QStringLiteral("<form>"));
    REQUIRE(h->startColumn() == 5);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 10);
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
<form>
Text

*/
TEST_CASE("111")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/111.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

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
    REQUIRE(h->text() == QStringLiteral("<form>\nText"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 1);
    REQUIRE(h->endColumn() == 3);
    REQUIRE(h->endLine() == 2);
}

/*
<a href="www.google.com">
Google
</a>

*/
TEST_CASE("112")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/112.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">\nGoogle\n</a>"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 3);
    REQUIRE(h->endLine() == 2);
}

/*
Text <a href="www.google.com">Google</a>

*/
TEST_CASE("113")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/113.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 39);
    REQUIRE(p->endLine() == 0);
    REQUIRE(p->items().size() == 4);

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

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">"));
        REQUIRE(h->startColumn() == 5);
        REQUIRE(h->startLine() == 0);
        REQUIRE(h->endColumn() == 29);
        REQUIRE(h->endLine() == 0);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Google"));
        REQUIRE(t->startColumn() == 30);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 35);
        REQUIRE(t->endLine() == 0);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(3).get());
        REQUIRE(h->text() == QStringLiteral("</a>"));
        REQUIRE(h->startColumn() == 36);
        REQUIRE(h->startLine() == 0);
        REQUIRE(h->endColumn() == 39);
        REQUIRE(h->endLine() == 0);
    }
}

/*
<a href="www.google.com">Google</a> Text

*/
TEST_CASE("114")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/114.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 39);
    REQUIRE(p->endLine() == 0);
    REQUIRE(p->items().size() == 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(0).get());
        REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">"));
        REQUIRE(h->startColumn() == 0);
        REQUIRE(h->startLine() == 0);
        REQUIRE(h->endColumn() == 24);
        REQUIRE(h->endLine() == 0);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Google"));
        REQUIRE(t->startColumn() == 25);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 30);
        REQUIRE(t->endLine() == 0);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(2).get());
        REQUIRE(h->text() == QStringLiteral("</a>"));
        REQUIRE(h->startColumn() == 31);
        REQUIRE(h->startLine() == 0);
        REQUIRE(h->endColumn() == 34);
        REQUIRE(h->endLine() == 0);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(3).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral(" Text"));
        REQUIRE(t->startColumn() == 35);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 39);
        REQUIRE(t->endLine() == 0);
    }
}

/*
<a href="www.google.com">
Google
</a>
Text

*/
TEST_CASE("115")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/115.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">\nGoogle\n</a>\nText"));
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 3);
    REQUIRE(h->endLine() == 3);
}

/*
Text
<a href="www.google.com">
Google
</a>

*/
TEST_CASE("116")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/116.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 3);
    REQUIRE(p->items().size() == 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 0);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">"));
        REQUIRE(h->startColumn() == 0);
        REQUIRE(h->startLine() == 1);
        REQUIRE(h->endColumn() == 24);
        REQUIRE(h->endLine() == 1);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Google"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 2);
        REQUIRE(t->endColumn() == 5);
        REQUIRE(t->endLine() == 2);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(3).get());
        REQUIRE(h->text() == QStringLiteral("</a>"));
        REQUIRE(h->startColumn() == 0);
        REQUIRE(h->startLine() == 3);
        REQUIRE(h->endColumn() == 3);
        REQUIRE(h->endLine() == 3);
    }
}

/*
Text <a href="www.google.com">Google</a>
Text

*/
TEST_CASE("117")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/117.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 5);
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

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">"));
        REQUIRE(h->startColumn() == 5);
        REQUIRE(h->startLine() == 0);
        REQUIRE(h->endColumn() == 29);
        REQUIRE(h->endLine() == 0);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Google"));
        REQUIRE(t->startColumn() == 30);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 35);
        REQUIRE(t->endLine() == 0);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(3).get());
        REQUIRE(h->text() == QStringLiteral("</a>"));
        REQUIRE(h->startColumn() == 36);
        REQUIRE(h->startLine() == 0);
        REQUIRE(h->endColumn() == 39);
        REQUIRE(h->endLine() == 0);
    }

    {
        REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(4).get());
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
<a href="www.google.com">
Google
</a>
Text

*/
TEST_CASE("118")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/118.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 5);
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 0);
    REQUIRE(p->endColumn() == 3);
    REQUIRE(p->endLine() == 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 0);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 0);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">"));
        REQUIRE(h->startColumn() == 0);
        REQUIRE(h->startLine() == 1);
        REQUIRE(h->endColumn() == 24);
        REQUIRE(h->endLine() == 1);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Google"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 2);
        REQUIRE(t->endColumn() == 5);
        REQUIRE(t->endLine() == 2);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(3).get());
        REQUIRE(h->text() == QStringLiteral("</a>"));
        REQUIRE(h->startColumn() == 0);
        REQUIRE(h->startLine() == 3);
        REQUIRE(h->endColumn() == 3);
        REQUIRE(h->endLine() == 3);
    }

    {
        REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(4).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->startLine() == 4);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->endLine() == 4);
    }
}

TEST_CASE("118-1")
{
    const auto fileName = QStringLiteral("tests/parser/data/118.md");

    QFile file(fileName);

    if (file.open(QIODeviceBase::ReadOnly)) {
        QTextStream stream(file.readAll());
        file.close();

        const auto wd = QDir().absolutePath();

        MD::Parser parser;

        auto doc = parser.parse(stream, wd, fileName);

        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 5);
        REQUIRE(p->items().size() == 5);
        REQUIRE(p->startColumn() == 0);
        REQUIRE(p->startLine() == 0);
        REQUIRE(p->endColumn() == 3);
        REQUIRE(p->endLine() == 4);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Text"));
            REQUIRE(t->startColumn() == 0);
            REQUIRE(t->startLine() == 0);
            REQUIRE(t->endColumn() == 3);
            REQUIRE(t->endLine() == 0);
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
            auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
            REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">"));
            REQUIRE(h->startColumn() == 0);
            REQUIRE(h->startLine() == 1);
            REQUIRE(h->endColumn() == 24);
            REQUIRE(h->endLine() == 1);
        }

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(2).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Google"));
            REQUIRE(t->startColumn() == 0);
            REQUIRE(t->startLine() == 2);
            REQUIRE(t->endColumn() == 5);
            REQUIRE(t->endLine() == 2);
        }

        {
            REQUIRE(p->items().at(3)->type() == MD::ItemType::RawHtml);
            auto h = static_cast<MD::RawHtml *>(p->items().at(3).get());
            REQUIRE(h->text() == QStringLiteral("</a>"));
            REQUIRE(h->startColumn() == 0);
            REQUIRE(h->startLine() == 3);
            REQUIRE(h->endColumn() == 3);
            REQUIRE(h->endLine() == 3);
        }

        {
            REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(4).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Text"));
            REQUIRE(t->startColumn() == 0);
            REQUIRE(t->startLine() == 4);
            REQUIRE(t->endColumn() == 3);
            REQUIRE(t->endLine() == 4);
        }
    } else
        REQUIRE(false);
}

/*
<a href="www.google.com">
Google
</a>
<pre>
</pre>

*/
TEST_CASE("119")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/119.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">\nGoogle\n</a>\n<pre>\n</pre>"));
        REQUIRE(h->startColumn() == 0);
        REQUIRE(h->startLine() == 0);
        REQUIRE(h->endColumn() == 5);
        REQUIRE(h->endLine() == 4);
    }
}

/*

$$
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
$$

*/
TEST_CASE("120")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/120.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->startColumn() == 0);
    REQUIRE(p->startLine() == 1);
    REQUIRE(p->endColumn() == 1);
    REQUIRE(p->endLine() == 11);

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
    REQUIRE(m->startColumn() == 0);
    REQUIRE(m->startLine() == 2);
    REQUIRE(m->endColumn() == 1);
    REQUIRE(m->endLine() == 10);
    REQUIRE(m->startDelim() == MD::WithPosition{0, 1, 1, 1});
    REQUIRE(m->endDelim() == MD::WithPosition{0, 11, 1, 11});
}
