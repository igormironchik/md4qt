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

#include "test_utils.h"

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
    CHECK_POSITIONS(h, 0, 0, 14, 0);
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
    CHECK_POSITIONS(p, 0, 0, 19, 0);
    REQUIRE(p->items().size() == 2);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text "));
    CHECK_POSITIONS(t, 0, 0, 4, 0);

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<!DOCTYPE html>"));
    CHECK_POSITIONS(h, 5, 0, 19, 0);
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
    CHECK_POSITIONS(h, 0, 0, 19, 0);
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
    CHECK_POSITIONS(h, 0, 0, 14, 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    REQUIRE(p->items().size() == 1);
    CHECK_POSITIONS(p, 0, 1, 3, 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(t, 0, 1, 3, 1);
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
    CHECK_POSITIONS(p, 0, 0, 3, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(t, 0, 0, 3, 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->text() == QStringLiteral("<!DOCTYPE html>"));
    CHECK_POSITIONS(h, 0, 1, 14, 1);
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
    CHECK_POSITIONS(p, 0, 0, 3, 1);
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text "));
        CHECK_POSITIONS(t, 0, 0, 4, 0);
    }

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<!DOCTYPE html>"));
    CHECK_POSITIONS(h, 5, 0, 19, 0);

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
    REQUIRE(h->text() == QStringLiteral("<!DOCTYPE html>"));
    CHECK_POSITIONS(h, 0, 1, 14, 1);

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
    CHECK_POSITIONS(h, 0, 0, 2, 11);
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
        CHECK_POSITIONS(p, 0, 0, 12, 4);
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
        CHECK_POSITIONS(p, 0, 6, 9, 6);
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
        CHECK_POSITIONS(p, 0, 9, 2, 11);
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
    CHECK_POSITIONS(h, 0, 0, 7, 11);
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
    CHECK_POSITIONS(h, 0, 0, 2, 11);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    CHECK_POSITIONS(p, 0, 12, 3, 12);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(t, 0, 12, 3, 12);
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
    CHECK_POSITIONS(p, 0, 0, 3, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(t, 0, 0, 3, 0);

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
    CHECK_POSITIONS(h, 0, 1, 2, 12);
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
        CHECK_POSITIONS(p, 0, 0, 12, 4);
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
        CHECK_POSITIONS(p, 0, 6, 9, 6);
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
        CHECK_POSITIONS(p, 0, 9, 3, 12);
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
    CHECK_POSITIONS(h, 0, 1, 2, 12);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(3).get());
        CHECK_POSITIONS(p, 0, 13, 3, 13);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(t, 0, 13, 3, 13);
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
    CHECK_POSITIONS(h, 0, 0, 5, 0);
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
    CHECK_POSITIONS(p, 0, 0, 10, 0);
    REQUIRE(p->items().size() == 2);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text "));
    CHECK_POSITIONS(t, 0, 0, 4, 0);

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<form>"));
    CHECK_POSITIONS(h, 5, 0, 10, 0);
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
    CHECK_POSITIONS(h, 0, 0, 10, 0);
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
    CHECK_POSITIONS(h, 0, 0, 3, 1);
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
    CHECK_POSITIONS(p, 0, 0, 3, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Text"));
    CHECK_POSITIONS(t, 0, 0, 3, 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
    REQUIRE(h->text() == QStringLiteral("<form>"));
    CHECK_POSITIONS(h, 0, 1, 5, 1);
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
    CHECK_POSITIONS(p, 0, 0, 3, 1);
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text "));
        CHECK_POSITIONS(t, 0, 0, 4, 0);
    }

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<form>"));
    CHECK_POSITIONS(h, 5, 0, 10, 0);

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
    REQUIRE(h->text() == QStringLiteral("<form>\nText"));
    CHECK_POSITIONS(h, 0, 1, 3, 2);
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
    CHECK_POSITIONS(h, 0, 0, 3, 2);
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
    CHECK_POSITIONS(p, 0, 0, 39, 0);
    REQUIRE(p->items().size() == 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text "));
        CHECK_POSITIONS(t, 0, 0, 4, 0);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">"));
        CHECK_POSITIONS(h, 5, 0, 29, 0);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Google"));
        CHECK_POSITIONS(t, 30, 0, 35, 0);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(3).get());
        REQUIRE(h->text() == QStringLiteral("</a>"));
        CHECK_POSITIONS(h, 36, 0, 39, 0);
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
    CHECK_POSITIONS(p, 0, 0, 39, 0);
    REQUIRE(p->items().size() == 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(0).get());
        REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">"));
        CHECK_POSITIONS(h, 0, 0, 24, 0);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Google"));
        CHECK_POSITIONS(t, 25, 0, 30, 0);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(2).get());
        REQUIRE(h->text() == QStringLiteral("</a>"));
        CHECK_POSITIONS(h, 31, 0, 34, 0);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(3).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral(" Text"));
        CHECK_POSITIONS(t, 35, 0, 39, 0);
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
    CHECK_POSITIONS(h, 0, 0, 3, 3);
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
    CHECK_POSITIONS(p, 0, 0, 3, 3);
    REQUIRE(p->items().size() == 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(t, 0, 0, 3, 0);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">"));
        CHECK_POSITIONS(h, 0, 1, 24, 1);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Google"));
        CHECK_POSITIONS(t, 0, 2, 5, 2);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(3).get());
        REQUIRE(h->text() == QStringLiteral("</a>"));
        CHECK_POSITIONS(h, 0, 3, 3, 3);
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
    CHECK_POSITIONS(p, 0, 0, 3, 1);
    REQUIRE(p->items().size() == 5);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text "));
        CHECK_POSITIONS(t, 0, 0, 4, 0);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">"));
        CHECK_POSITIONS(h, 5, 0, 29, 0);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Google"));
        CHECK_POSITIONS(t, 30, 0, 35, 0);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(3).get());
        REQUIRE(h->text() == QStringLiteral("</a>"));
        CHECK_POSITIONS(h, 36, 0, 39, 0);
    }

    {
        REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(4).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(t, 0, 1, 3, 1);
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
    CHECK_POSITIONS(p, 0, 0, 3, 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(t, 0, 0, 3, 0);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">"));
        CHECK_POSITIONS(h, 0, 1, 24, 1);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Google"));
        CHECK_POSITIONS(t, 0, 2, 5, 2);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(3).get());
        REQUIRE(h->text() == QStringLiteral("</a>"));
        CHECK_POSITIONS(h, 0, 3, 3, 3);
    }

    {
        REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(4).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));
        CHECK_POSITIONS(t, 0, 4, 3, 4);
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
        CHECK_POSITIONS(p, 0, 0, 3, 4);

        {
            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(0).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Text"));
            CHECK_POSITIONS(t, 0, 0, 3, 0);
        }

        {
            REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
            auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
            REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">"));
            CHECK_POSITIONS(h, 0, 1, 24, 1);
        }

        {
            REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(2).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Google"));
            CHECK_POSITIONS(t, 0, 2, 5, 2);
        }

        {
            REQUIRE(p->items().at(3)->type() == MD::ItemType::RawHtml);
            auto h = static_cast<MD::RawHtml *>(p->items().at(3).get());
            REQUIRE(h->text() == QStringLiteral("</a>"));
            CHECK_POSITIONS(h, 0, 3, 3, 3);
        }

        {
            REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
            auto t = static_cast<MD::Text *>(p->items().at(4).get());
            REQUIRE(t->opts() == MD::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Text"));
            CHECK_POSITIONS(t, 0, 4, 3, 4);
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
        CHECK_POSITIONS(h, 0, 0, 5, 4);
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
    CHECK_POSITIONS(p, 0, 1, 1, 11);

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
    CHECK_POSITIONS(m, 0, 2, 1, 10);
    REQUIRE(m->startDelim() == MD::WithPosition{0, 1, 1, 1});
    REQUIRE(m->endDelim() == MD::WithPosition{0, 11, 1, 11});
}
