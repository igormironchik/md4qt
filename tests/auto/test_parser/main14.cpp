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
#include <QTextStream>

//
// isCommonMarkAutolinkUri unit tests
//

TEST_CASE("commonmark_autolink_uri_valid_http")
{
    REQUIRE(MD::isCommonMarkAutolinkUri(QStringLiteral("http://example.com")));
}

TEST_CASE("commonmark_autolink_uri_valid_made_up_scheme")
{
    // Key case: accepted by CommonMark grammar, rejected by QUrl
    REQUIRE(MD::isCommonMarkAutolinkUri(QStringLiteral("made-up-scheme://foo,bar")));
}

TEST_CASE("commonmark_autolink_uri_valid_min_scheme_length")
{
    // Minimum valid scheme is 2 characters
    REQUIRE(MD::isCommonMarkAutolinkUri(QStringLiteral("ab:anything")));
}

TEST_CASE("commonmark_autolink_uri_valid_max_scheme_length")
{
    // Maximum valid scheme is 32 characters (1 letter + 31 more)
    const QString scheme = QStringLiteral("a") + QString(31, QLatin1Char('b'));
    REQUIRE(MD::isCommonMarkAutolinkUri(scheme + QStringLiteral(":path")));
}

TEST_CASE("commonmark_autolink_uri_invalid_single_letter_scheme")
{
    // {1,31} requires at least one character after the opening letter
    REQUIRE(!MD::isCommonMarkAutolinkUri(QStringLiteral("a:path")));
}

TEST_CASE("commonmark_autolink_uri_invalid_scheme_too_long")
{
    // 33-character scheme exceeds the {1,31} limit
    const QString scheme = QStringLiteral("a") + QString(32, QLatin1Char('b'));
    REQUIRE(!MD::isCommonMarkAutolinkUri(scheme + QStringLiteral(":path")));
}

TEST_CASE("commonmark_autolink_uri_invalid_starts_with_digit")
{
    REQUIRE(!MD::isCommonMarkAutolinkUri(QStringLiteral("1http://foo")));
}

TEST_CASE("commonmark_autolink_uri_invalid_space_in_path")
{
    // Space (0x20) is excluded by the path character class
    REQUIRE(!MD::isCommonMarkAutolinkUri(QStringLiteral("http://foo bar")));
}

TEST_CASE("commonmark_autolink_uri_invalid_gt_in_path")
{
    REQUIRE(!MD::isCommonMarkAutolinkUri(QStringLiteral("http://foo>bar")));
}

TEST_CASE("commonmark_autolink_uri_invalid_lt_in_path")
{
    REQUIRE(!MD::isCommonMarkAutolinkUri(QStringLiteral("http://foo<bar")));
}

TEST_CASE("commonmark_autolink_uri_invalid_control_char_in_path")
{
    // Control characters (0x00–0x1F) are excluded
    REQUIRE(!MD::isCommonMarkAutolinkUri(QStringLiteral("http://foo") + QChar(0x01) + QStringLiteral("bar")));
}

TEST_CASE("commonmark_autolink_uri_invalid_del_in_path")
{
    // DEL (0x7F) is excluded
    REQUIRE(!MD::isCommonMarkAutolinkUri(QStringLiteral("http://foo") + QChar(0x7F) + QStringLiteral("bar")));
}

TEST_CASE("commonmark_autolink_uri_invalid_empty")
{
    REQUIRE(!MD::isCommonMarkAutolinkUri(QString()));
}

TEST_CASE("commonmark_autolink_uri_invalid_no_colon")
{
    REQUIRE(!MD::isCommonMarkAutolinkUri(QStringLiteral("httpnopath")));
}

//
// GFM autolink integration tests with CommonMark validation mode
// These exercise isValidHttpAutolink() with CommonMark mode in
// gfm_autolink_parser.cpp.
//

TEST_CASE("gfm_autolink_www_commonmark_mode")
{
    QString md = QStringLiteral("www.example.com");
    QTextStream stream(&md);
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
    REQUIRE(l->url() == QStringLiteral("http://www.example.com"));
}

TEST_CASE("gfm_autolink_http_commonmark_mode")
{
    QString md = QStringLiteral("http://example.com");
    QTextStream stream(&md);
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
    REQUIRE(l->url() == QStringLiteral("http://example.com"));
}

TEST_CASE("gfm_autolink_https_commonmark_mode")
{
    QString md = QStringLiteral("https://example.com");
    QTextStream stream(&md);
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
    REQUIRE(l->url() == QStringLiteral("https://example.com"));
}

TEST_CASE("gfm_autolink_http_qurl_mode")
{
    // Confirm default QUrl mode still produces a link for a valid http URL,
    // and that the two modes agree on well-formed URLs.
    QString md = QStringLiteral("http://example.com");
    QTextStream stream(&md);
    MD::Parser parser; // default: AutolinkUriValidation::QUrl
    auto doc = parser.parse(stream, QString(), QString());

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->url() == QStringLiteral("http://example.com"));
}
