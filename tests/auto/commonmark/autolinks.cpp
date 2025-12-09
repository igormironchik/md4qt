/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "load.h"

// 6.5 Autolinks

TEST_CASE("593")
{
    const auto doc = load_test(593);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text().size() == 0);
    REQUIRE(l->url() == QStringLiteral("http://foo.bar.baz"));
}

TEST_CASE("594")
{
    const auto doc = load_test(594);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text().size() == 0);
    REQUIRE(l->url() == QStringLiteral("http://foo.bar.baz/test?q=hello&id=22&boolean"));
}

TEST_CASE("595")
{
    const auto doc = load_test(595);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text().size() == 0);
    REQUIRE(l->url() == QStringLiteral("irc://foo.bar:2233/baz"));
}

TEST_CASE("596")
{
    const auto doc = load_test(596);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text().size() == 0);
    REQUIRE(l->url() == QStringLiteral("MAILTO:FOO@BAR.BAZ"));
}

TEST_CASE("597")
{
    const auto doc = load_test(597);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text().size() == 0);
    REQUIRE(l->url() == QStringLiteral("a+b+c:d"));
}

TEST_CASE("598")
{
    MESSAGE("This test is not strict to CommonMark 0.30.");
    MESSAGE("Skip for now.");
}

TEST_CASE("599")
{
    MESSAGE("This test is not strict to CommonMark 0.30.");
    MESSAGE("Skip for now.");
}

TEST_CASE("600")
{
    const auto doc = load_test(600);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text().size() == 0);
    REQUIRE(l->url() == QStringLiteral("localhost:5001/foo"));
}

TEST_CASE("601")
{
    MESSAGE("This test is not strict to CommonMark due to GitHub's autolinks extension.");
    MESSAGE("Skip for now.");

    // const auto doc = load_test( 601 );

    // REQUIRE( doc->isEmpty() == false );
    // REQUIRE( doc->items().size() == 2 );

    // REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
    // auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
    // REQUIRE( p->items().size() == 1 );
    // REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
    // auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
    // REQUIRE( t->opts() == MD::TextWithoutFormat );
    // REQUIRE( t->text() == u8"<http://foo.bar/baz bim>" );
}

TEST_CASE("602")
{
    MESSAGE("This test is not strict to CommonMark 0.30.");
    MESSAGE("Skip for now.");
}

TEST_CASE("603")
{
    const auto doc = load_test(603);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text().size() == 0);
    REQUIRE(l->url() == QStringLiteral("foo@bar.example.com"));
}

TEST_CASE("604")
{
    const auto doc = load_test(604);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->img()->isEmpty());
    REQUIRE(l->opts() == MD::TextWithoutFormat);
    REQUIRE(l->text().size() == 0);
    REQUIRE(l->url() == QStringLiteral("foo+special@Bar.baz-bar0.com"));
}

TEST_CASE("605")
{
    const auto doc = load_test(605);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("<foo+@bar.example.com>"));
}

TEST_CASE("606")
{
    const auto doc = load_test(606);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("<>"));
}

TEST_CASE("607")
{
    MESSAGE("This test is not strict to CommonMark due to GitHub's autolink extension.");
    MESSAGE("Skip for now.");

    // const auto doc = load_test( 607 );

    // REQUIRE( doc->isEmpty() == false );
    // REQUIRE( doc->items().size() == 2 );

    // REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
    // auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
    // REQUIRE( p->items().size() == 1 );
    // REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
    // auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
    // REQUIRE( t->opts() == MD::TextWithoutFormat );
    // REQUIRE( t->text() == u8"< http://foo.bar >" );
}

TEST_CASE("608")
{
    MESSAGE("This test is not strict to CommonMark 0.30.");
    MESSAGE("Skip for now.");
}

TEST_CASE("609")
{
    const auto doc = load_test(609);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("<foo.bar.baz>"));
}

TEST_CASE("610")
{
    MESSAGE("This test is not strict to CommonMark due to GitHub's autolink extension.");
    MESSAGE("Skip for now.");

    // const auto doc = load_test( 610 );

    // REQUIRE( doc->isEmpty() == false );
    // REQUIRE( doc->items().size() == 2 );

    // REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
    // auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
    // REQUIRE( p->items().size() == 1 );
    // REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
    // auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
    // REQUIRE( t->opts() == MD::TextWithoutFormat );
    // REQUIRE( t->text() == u8"http://example.com" );
}

TEST_CASE("611")
{
    MESSAGE("This test is not strict to CommonMark due to GitHub's autolink extension.");
    MESSAGE("Skip for now.");

    // const auto doc = load_test( 611 );

    // REQUIRE( doc->isEmpty() == false );
    // REQUIRE( doc->items().size() == 2 );

    // REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
    // auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
    // REQUIRE( p->items().size() == 1 );
    // REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
    // auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
    // REQUIRE( t->opts() == MD::TextWithoutFormat );
    // REQUIRE( t->text() == u8"foo@bar.example.com" );
}
