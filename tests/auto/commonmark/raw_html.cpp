/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "load.h"

// 6.6 Raw HTML

TEST_CASE("612")
{
    const auto doc = load_test(612);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(0).get());
        REQUIRE(h->text() == QStringLiteral("<a>"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<bab>"));
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(2).get());
        REQUIRE(h->text() == QStringLiteral("<c2c>"));
    }
}

TEST_CASE("613")
{
    const auto doc = load_test(613);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(0).get());
        REQUIRE(h->text() == QStringLiteral("<a/>"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<b2/>"));
    }
}

TEST_CASE("614")
{
    const auto doc = load_test(614);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(0).get());
        REQUIRE(h->text() == QStringLiteral("<a  />"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<b2\ndata=\"foo\" >"));
    }
}

TEST_CASE("615")
{
    const auto doc = load_test(615);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(0).get());
    REQUIRE(h->text()
            == QStringLiteral("<a foo=\"bar\" bam = 'baz <em>\"</em>'\n"
                              "_boolean zoop:33=zoop:33 />"));
}

TEST_CASE("616")
{
    const auto doc = load_test(616);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Foo "));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<responsive-image src=\"foo.jpg\" />"));
    }
}

TEST_CASE("617")
{
    const auto doc = load_test(617);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("<33> <__>"));
}

TEST_CASE("618")
{
    const auto doc = load_test(618);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("<a h*#ref=\"hi\">"));
}

TEST_CASE("619")
{
    const auto doc = load_test(619);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("<a href=\"hi'> <a href=hi'>"));
}

TEST_CASE("620")
{
    const auto doc = load_test(620);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("< a><"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("foo><bar/ >"));
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(2).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("<foo bar=baz"));
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(3).get());
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("bim!bop />"));
    }
}

TEST_CASE("621")
{
    const auto doc = load_test(621);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("<a href='bar'title=title>"));
}

TEST_CASE("622")
{
    const auto doc = load_test(622);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());

    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(0).get());
        REQUIRE(h->text() == QStringLiteral("</a>"));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("</foo >"));
    }
}

TEST_CASE("623")
{
    const auto doc = load_test(623);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("</a href=\"foo\">"));
}

TEST_CASE("624")
{
    const auto doc = load_test(624);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("foo "));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<!-- this is a\ncomment - with hyphen -->"));
}

TEST_CASE("625")
{
    MESSAGE("Outdated. Changed in CommonMark 0.31.2");
}

TEST_CASE("626")
{
    MESSAGE("Outdated. Changed in CommonMark 0.31.2");
}

TEST_CASE("627")
{
    const auto doc = load_test(627);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("foo "));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<?php echo $a; ?>"));
}

TEST_CASE("628")
{
    const auto doc = load_test(628);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("foo "));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<!ELEMENT br EMPTY>"));
}

TEST_CASE("629")
{
    const auto doc = load_test(629);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("foo "));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<![CDATA[>&<]]>"));
}

TEST_CASE("630")
{
    const auto doc = load_test(630);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("foo "));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<a href=\"&ouml;\">"));
}

TEST_CASE("631")
{
    const auto doc = load_test(631);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("foo "));

    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<a href=\"\\*\">"));
}

TEST_CASE("632")
{
    const auto doc = load_test(632);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("<a href=\"\"\">"));
}
