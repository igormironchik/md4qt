/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#include "paragraph_parser.h"
#include "utils.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest/doctest.h>

// TEST_CASE("is_footnote")
// {
//     REQUIRE(!MD::isFootnote(QStringLiteral("[^]:")));
//     REQUIRE(!MD::isFootnote(QStringLiteral("[^ a]:")));
//     REQUIRE(!MD::isFootnote(QStringLiteral("[^  a]:")));
//     REQUIRE(!MD::isFootnote(QStringLiteral("[^ a a]:")));
//     REQUIRE(!MD::isFootnote(QStringLiteral("[^a a]:")));
// }

TEST_CASE("paragraph_to_label")
{
    {
        auto p = QSharedPointer<MD::Paragraph>::create();
        auto c = QSharedPointer<MD::Code>::create(QStringLiteral("ICU"), false, true);
        c->setStartColumn(0);
        c->setStartLine(0);
        c->setEndColumn(3);
        c->setEndLine(0);
        p->appendItem(c);
        auto t = QSharedPointer<MD::Text>::create();
        t->setText(QStringLiteral("?"));
        t->setStartColumn(5);
        t->setStartLine(0);
        t->setEndColumn(5);
        t->setEndLine(0);
        p->appendItem(t);

        REQUIRE(MD::ParagraphParser::paragraphToLabel(p) == QStringLiteral("ICU"));
    }

    {
        auto p = QSharedPointer<MD::Paragraph>::create();
        auto c = QSharedPointer<MD::Code>::create(QStringLiteral("ICU"), false, true);
        c->setStartColumn(0);
        c->setStartLine(0);
        c->setEndColumn(3);
        c->setEndLine(0);
        p->appendItem(c);

        {
            auto t = QSharedPointer<MD::Text>::create();
            t->setText(QStringLiteral(","));
            t->setStartColumn(5);
            t->setStartLine(0);
            t->setEndColumn(5);
            t->setEndLine(0);
            p->appendItem(t);
        }

        auto t = QSharedPointer<MD::Text>::create();
        t->setText(QStringLiteral("text"));
        t->setStartColumn(0);
        t->setStartLine(1);
        t->setEndColumn(3);
        t->setEndLine(1);
        p->appendItem(t);

        REQUIRE(MD::ParagraphParser::paragraphToLabel(p) == QStringLiteral("ICUtext"));
    }

    {
        auto p = QSharedPointer<MD::Paragraph>::create();
        auto c = QSharedPointer<MD::Code>::create(QStringLiteral("text"), false, true);
        c->setStartColumn(0);
        c->setStartLine(0);
        c->setEndColumn(3);
        c->setEndLine(0);
        p->appendItem(c);

        {
            auto t = QSharedPointer<MD::Text>::create();
            t->setText(QStringLiteral("? text?"));
            t->setStartColumn(5);
            t->setStartLine(0);
            t->setEndColumn(11);
            t->setEndLine(0);
            p->appendItem(t);
        }

        REQUIRE(MD::ParagraphParser::paragraphToLabel(p) == QStringLiteral("text-text"));
    }
}

TEST_CASE("is_email")
{
    REQUIRE(MD::isEmail(QStringLiteral("igor@gmail.com")));
    REQUIRE(!MD::isEmail(QStringLiteral("igor@gmail-.com")));
    REQUIRE(!MD::isEmail(QStringLiteral("igor@-gmail.com")));

    static const auto i63 = QString(63, QLatin1Char('i'));
    static const auto i64 = QString(64, QLatin1Char('i'));

    static const QString okEmail = QStringLiteral("igor@") + i63 + QString(1, QLatin1Char('.')) + i63;
    REQUIRE(MD::isEmail(okEmail));

    static const QString wrongEmail = QStringLiteral("igor@") + i64 + QString(1, QLatin1Char('.')) + i63;
    REQUIRE(!MD::isEmail(wrongEmail));

    REQUIRE(!MD::isEmail(QStringLiteral("i[]gor@gmail.com")));

    REQUIRE(MD::isEmail(QStringLiteral("igor@gmail-gmail.com")));

    REQUIRE(!MD::isEmail(QStringLiteral("igor@gmail-gmail.")));
    REQUIRE(!MD::isEmail(QStringLiteral("igor@gmail-gmail")));
    REQUIRE(!MD::isEmail(QStringLiteral("igor@.")));
    REQUIRE(MD::isEmail(QStringLiteral("a@a.a")));
    REQUIRE(!MD::isEmail(QStringLiteral("@a.a")));
    REQUIRE(!MD::isEmail(QStringLiteral("@.a")));
    REQUIRE(!MD::isEmail(QStringLiteral("@.")));
}
