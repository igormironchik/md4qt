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

TEST_CASE("001")
{
    MD::Parser p;
    auto doc = p.parse(QStringLiteral("tests/parser/data/001.md"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 1);
    REQUIRE(doc->items().at(0)->type() == MD::ItemType::Anchor);
}

/*
This is just a text!

*/
TEST_CASE("002")
{
    MD::Parser p;
    auto doc = p.parse(QStringLiteral("tests/parser/data/002.md"));

    auto checkDoc = [](QSharedPointer<MD::Document> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        CHECK_POSITIONS(dp, 0, 0, 19, 0);

        REQUIRE(dp->items().size() == 1);
        REQUIRE(dp->items().front()->type() == MD::ItemType::Text);

        auto dt = static_cast<MD::Text *>(dp->items().front().get());

        REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(dt->text() == QStringLiteral("This is just a text!"));
        CHECK_POSITIONS(dt, 0, 0, 19, 0);
    };

    checkDoc(doc);

    checkDoc(doc->clone().staticCast<MD::Document>());
}

/*

Paragraph 1.

Paragraph 2.

*/
TEST_CASE("003")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/003.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        CHECK_POSITIONS(dp, 0, 1, 11, 1);

        REQUIRE(dp->items().size() == 1);
        REQUIRE(dp->items().front()->type() == MD::ItemType::Text);

        auto dt = static_cast<MD::Text *>(dp->items().front().get());

        REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(dt->text() == QStringLiteral("Paragraph 1."));
        CHECK_POSITIONS(dt, 0, 1, 11, 1);
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);

        auto dp = static_cast<MD::Paragraph *>(doc->items().at(2).get());
        CHECK_POSITIONS(dp, 0, 3, 11, 3);

        REQUIRE(dp->items().size() == 1);
        REQUIRE(dp->items().front()->type() == MD::ItemType::Text);

        auto dt = static_cast<MD::Text *>(dp->items().front().get());

        REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(dt->text() == QStringLiteral("Paragraph 2."));
        CHECK_POSITIONS(dt, 0, 3, 11, 3);
    }
}

/*
Line 1...
Line 2...
Line 3...

*/
TEST_CASE("004")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/004.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(dp, 0, 0, 8, 2);

    REQUIRE(dp->items().size() == 3);

    {
        REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);
        auto dt = static_cast<MD::Text *>(dp->getItemAt(0).get());
        REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(dt->text() == QStringLiteral("Line 1..."));
        CHECK_POSITIONS(dt, 0, 0, 8, 0);
    }

    {
        REQUIRE(dp->items().at(1)->type() == MD::ItemType::Text);
        auto dt = static_cast<MD::Text *>(dp->items().at(1).get());
        REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(dt->text() == QStringLiteral("Line 2..."));
        CHECK_POSITIONS(dt, 0, 1, 8, 1);
    }

    {
        REQUIRE(dp->items().at(2)->type() == MD::ItemType::Text);
        auto dt = static_cast<MD::Text *>(dp->items().at(2).get());
        REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(dt->text() == QStringLiteral("Line 3..."));
        CHECK_POSITIONS(dt, 0, 2, 8, 2);
    }
}

/*
Line 1...<space><space>
Line 2...
Line 3...

*/
TEST_CASE("005")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/005.md"));

    auto checkDoc = [](QSharedPointer<MD::Document> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        CHECK_POSITIONS(dp, 0, 0, 8, 2);

        REQUIRE(dp->items().size() == 4);

        {
            REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);

            auto dt = static_cast<MD::Text *>(dp->items().at(0).get());
            CHECK_POSITIONS(dt, 0, 0, 8, 0);

            REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(dt->text() == QStringLiteral("Line 1..."));
        }

        REQUIRE(dp->items().at(1)->type() == MD::ItemType::LineBreak);
        auto lb = static_cast<MD::LineBreak *>(dp->items().at(1).get());
        CHECK_POSITIONS(lb, 9, 0, 10, 0);

        {
            REQUIRE(dp->items().at(2)->type() == MD::ItemType::Text);

            auto dt = static_cast<MD::Text *>(dp->items().at(2).get());

            REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(dt->text() == QStringLiteral("Line 2..."));
            CHECK_POSITIONS(dt, 0, 1, 8, 1);
        }

        {
            REQUIRE(dp->items().at(3)->type() == MD::ItemType::Text);

            auto dt = static_cast<MD::Text *>(dp->items().at(3).get());

            REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(dt->text() == QStringLiteral("Line 3..."));
            CHECK_POSITIONS(dt, 0, 2, 8, 2);
        }
    };

    checkDoc(doc);

    checkDoc(doc->clone().staticCast<MD::Document>());
}

/*
*Line 1...*
__Line 2...__
~~Line 3...~~

*/
TEST_CASE("006")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/006.md"));

    auto checkDoc = [](QSharedPointer<MD::Document> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        CHECK_POSITIONS(dp, 0, 0, 12, 2);

        REQUIRE(dp->items().size() == 3);

        {
            REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);

            auto dt = static_cast<MD::Text *>(dp->items().at(0).get());

            REQUIRE(dt->opts() == MD::TextOption::ItalicText);
            REQUIRE(dt->text() == QStringLiteral("Line 1..."));
            CHECK_POSITIONS(dt, 1, 0, 9, 0);
            REQUIRE(dt->openStyles().size() == 1);
            REQUIRE(dt->closeStyles().size() == 1);
            REQUIRE(dt->openStyles().at(0) == MD::StyleDelim{MD::ItalicText, 0, 0, 0, 0});
            REQUIRE(dt->closeStyles().at(0) == MD::StyleDelim{MD::ItalicText, 10, 0, 10, 0});
        }

        {
            REQUIRE(dp->items().at(1)->type() == MD::ItemType::Text);

            auto dt = static_cast<MD::Text *>(dp->items().at(1).get());

            REQUIRE(dt->opts() == MD::TextOption::BoldText);
            REQUIRE(dt->text() == QStringLiteral("Line 2..."));
            CHECK_POSITIONS(dt, 2, 1, 10, 1);
            REQUIRE(dt->openStyles().size() == 1);
            REQUIRE(dt->openStyles().at(0) == MD::StyleDelim{MD::BoldText, 0, 1, 1, 1});
            REQUIRE(dt->closeStyles().size() == 1);
            REQUIRE(dt->closeStyles().at(0) == MD::StyleDelim{MD::BoldText, 11, 1, 12, 1});
        }

        {
            REQUIRE(dp->items().at(2)->type() == MD::ItemType::Text);

            auto dt = static_cast<MD::Text *>(dp->items().at(2).get());

            REQUIRE(dt->opts() == MD::TextOption::StrikethroughText);
            REQUIRE(dt->text() == QStringLiteral("Line 3..."));
            CHECK_POSITIONS(dt, 2, 2, 10, 2);
            REQUIRE(dt->openStyles().size() == 1);
            REQUIRE(dt->openStyles().at(0) == MD::StyleDelim{MD::StrikethroughText, 0, 2, 1, 2});
            REQUIRE(dt->closeStyles().size() == 1);
            REQUIRE(dt->closeStyles().at(0) == MD::StyleDelim{MD::StrikethroughText, 11, 2, 12, 2});
        }
    };

    checkDoc(doc);

    checkDoc(doc->clone().staticCast<MD::Document>());
}

/*
__*Line 1...
Line 2...
Line 3...*__

*/
TEST_CASE("007")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/007.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(dp, 0, 0, 11, 2);

    REQUIRE(dp->items().size() == 3);

    {
        REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);
        auto dt = static_cast<MD::Text *>(dp->items().at(0).get());
        REQUIRE(dt->opts() == (MD::TextOption::ItalicText | MD::TextOption::BoldText));
        REQUIRE(dt->text() == QStringLiteral("Line 1..."));
        CHECK_POSITIONS(dt, 3, 0, 11, 0);
        REQUIRE(dt->openStyles().size() == 2);
        REQUIRE(dt->openStyles().at(0) == MD::StyleDelim{MD::BoldText, 0, 0, 1, 0});
        REQUIRE(dt->openStyles().at(1) == MD::StyleDelim{MD::ItalicText, 2, 0, 2, 0});
        REQUIRE(dt->closeStyles().empty());
    }

    {
        REQUIRE(dp->items().at(1)->type() == MD::ItemType::Text);
        auto dt = static_cast<MD::Text *>(dp->items().at(1).get());
        REQUIRE(dt->opts() == (MD::TextOption::ItalicText | MD::TextOption::BoldText));
        REQUIRE(dt->text() == QStringLiteral("Line 2..."));
        CHECK_POSITIONS(dt, 0, 1, 8, 1);
        REQUIRE(dt->openStyles().empty());
        REQUIRE(dt->closeStyles().empty());
    }

    {
        REQUIRE(dp->items().at(2)->type() == MD::ItemType::Text);
        auto dt = static_cast<MD::Text *>(dp->items().at(2).get());
        REQUIRE(dt->opts() == (MD::TextOption::ItalicText | MD::TextOption::BoldText));
        REQUIRE(dt->text() == QStringLiteral("Line 3..."));
        CHECK_POSITIONS(dt, 0, 2, 8, 2);
        REQUIRE(dt->openStyles().empty());
        REQUIRE(dt->closeStyles().size() == 2);
        REQUIRE(dt->closeStyles().at(0) == MD::StyleDelim{MD::ItalicText, 9, 2, 9, 2});
        REQUIRE(dt->closeStyles().at(1) == MD::StyleDelim{MD::BoldText, 10, 2, 11, 2});
    }
}

/*
~~__*Line 1...
Line 2...
Line 3...*__~~

*/
TEST_CASE("008")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/008.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(dp, 0, 0, 13, 2);

    REQUIRE(dp->items().size() == 3);

    {
        REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);

        auto dt = static_cast<MD::Text *>(dp->items().at(0).get());

        REQUIRE(dt->opts()
                == (MD::TextOption::ItalicText | MD::TextOption::BoldText | MD::TextOption::StrikethroughText));
        REQUIRE(dt->text() == QStringLiteral("Line 1..."));
        CHECK_POSITIONS(dt, 5, 0, 13, 0);
        REQUIRE(dt->openStyles().size() == 3);
        REQUIRE(dt->openStyles().at(0) == MD::StyleDelim{MD::StrikethroughText, 0, 0, 1, 0});
        REQUIRE(dt->openStyles().at(1) == MD::StyleDelim{MD::BoldText, 2, 0, 3, 0});
        REQUIRE(dt->openStyles().at(2) == MD::StyleDelim{MD::ItalicText, 4, 0, 4, 0});
        REQUIRE(dt->closeStyles().empty());
    }

    {
        REQUIRE(dp->items().at(1)->type() == MD::ItemType::Text);

        auto dt = static_cast<MD::Text *>(dp->items().at(1).get());

        REQUIRE(dt->opts()
                == (MD::TextOption::ItalicText | MD::TextOption::BoldText | MD::TextOption::StrikethroughText));
        REQUIRE(dt->text() == QStringLiteral("Line 2..."));
        CHECK_POSITIONS(dt, 0, 1, 8, 1);
        REQUIRE(dt->openStyles().empty());
        REQUIRE(dt->closeStyles().empty());
    }

    {
        REQUIRE(dp->items().at(2)->type() == MD::ItemType::Text);

        auto dt = static_cast<MD::Text *>(dp->items().at(2).get());

        REQUIRE(dt->opts()
                == (MD::TextOption::ItalicText | MD::TextOption::BoldText | MD::TextOption::StrikethroughText));
        REQUIRE(dt->text() == QStringLiteral("Line 3..."));
        CHECK_POSITIONS(dt, 0, 2, 8, 2);
        REQUIRE(dt->closeStyles().size() == 3);
        REQUIRE(dt->closeStyles().at(0) == MD::StyleDelim{MD::ItalicText, 9, 2, 9, 2});
        REQUIRE(dt->closeStyles().at(1) == MD::StyleDelim{MD::BoldText, 10, 2, 11, 2});
        REQUIRE(dt->closeStyles().at(2) == MD::StyleDelim{MD::StrikethroughText, 12, 2, 13, 2});
        REQUIRE(dt->openStyles().empty());
    }
}

/*
~~__*Line 1...
Line 2...~~
Line 3...*__

*/
TEST_CASE("009")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/009.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(dp, 0, 0, 11, 2);

    REQUIRE(dp->items().size() == 3);

    {
        REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);

        auto dt = static_cast<MD::Text *>(dp->items().at(0).get());

        REQUIRE(dt->opts() == MD::TextOption::StrikethroughText);
        REQUIRE(dt->text() == QStringLiteral("__*Line 1..."));
        CHECK_POSITIONS(dt, 2, 0, 13, 0);
        REQUIRE(dt->openStyles().size() == 1);
        REQUIRE(dt->openStyles().at(0) == MD::StyleDelim{MD::StrikethroughText, 0, 0, 1, 0});
        REQUIRE(dt->closeStyles().empty());
    }

    {
        REQUIRE(dp->items().at(1)->type() == MD::ItemType::Text);

        auto dt = static_cast<MD::Text *>(dp->items().at(1).get());

        REQUIRE(dt->opts() == MD::TextOption::StrikethroughText);
        REQUIRE(dt->text() == QStringLiteral("Line 2..."));
        CHECK_POSITIONS(dt, 0, 1, 8, 1);
        REQUIRE(dt->openStyles().empty());
        REQUIRE(dt->closeStyles().size() == 1);
        REQUIRE(dt->closeStyles().at(0) == MD::StyleDelim{MD::StrikethroughText, 9, 1, 10, 1});
    }

    {
        REQUIRE(dp->items().at(2)->type() == MD::ItemType::Text);

        auto dt = static_cast<MD::Text *>(dp->items().at(2).get());

        REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(dt->text() == QStringLiteral("Line 3...*__"));
        CHECK_POSITIONS(dt, 0, 2, 11, 2);
        REQUIRE(dt->openStyles().empty());
        REQUIRE(dt->closeStyles().empty());
    }
}

/*
\~\~\_\_\*Line 1...
Line 2...\~\~
Line 3...\*\_\_

*/
TEST_CASE("010")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/010.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(dp, 0, 0, 14, 2);

    REQUIRE(dp->items().size() == 3);

    {
        REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);

        auto dt = static_cast<MD::Text *>(dp->items().at(0).get());

        REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(dt->text() == QStringLiteral("~~__*Line 1..."));
        CHECK_POSITIONS(dt, 0, 0, 18, 0);
        REQUIRE(dt->openStyles().empty());
        REQUIRE(dt->closeStyles().empty());
    }

    {
        REQUIRE(dp->items().at(1)->type() == MD::ItemType::Text);

        auto dt = static_cast<MD::Text *>(dp->items().at(1).get());

        REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(dt->text() == QStringLiteral("Line 2...~~"));
        CHECK_POSITIONS(dt, 0, 1, 12, 1);
        REQUIRE(dt->openStyles().empty());
        REQUIRE(dt->closeStyles().empty());
    }

    {
        REQUIRE(dp->items().at(2)->type() == MD::ItemType::Text);

        auto dt = static_cast<MD::Text *>(dp->items().at(2).get());

        REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(dt->text() == QStringLiteral("Line 3...*__"));
        CHECK_POSITIONS(dt, 0, 2, 14, 2);
        REQUIRE(dt->openStyles().empty());
        REQUIRE(dt->closeStyles().empty());
    }
}

/*
`code`

*/
TEST_CASE("011")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/011.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(dp, 0, 0, 5, 0);

    REQUIRE(dp->items().size() == 1);

    REQUIRE(dp->items().at(0)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(dp->items().at(0).get());

    REQUIRE(c->isInline() == true);
    REQUIRE(c->text() == QStringLiteral("code"));
    CHECK_POSITIONS(c, 1, 0, 4, 0);
    REQUIRE(c->startDelim() == MD::WithPosition{0, 0, 0, 0});
    REQUIRE(c->endDelim() == MD::WithPosition{5, 0, 5, 0});
}

/*
Code in the `text`.

*/
TEST_CASE("012")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/012.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(dp, 0, 0, 18, 0);

    REQUIRE(dp->items().size() == 3);

    REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);

    auto t1 = static_cast<MD::Text *>(dp->items().at(0).get());
    CHECK_POSITIONS(t1, 0, 0, 11, 0);

    REQUIRE(t1->text() == QStringLiteral("Code in the "));

    REQUIRE(dp->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(dp->items().at(1).get());
    CHECK_POSITIONS(c, 13, 0, 16, 0);
    REQUIRE(c->startDelim() == MD::WithPosition{12, 0, 12, 0});
    REQUIRE(c->endDelim() == MD::WithPosition{17, 0, 17, 0});

    REQUIRE(c->isInline() == true);
    REQUIRE(c->text() == QStringLiteral("text"));

    REQUIRE(dp->items().at(2)->type() == MD::ItemType::Text);

    auto t2 = static_cast<MD::Text *>(dp->items().at(2).get());
    CHECK_POSITIONS(t2, 18, 0, 18, 0);

    REQUIRE(t2->text() == QStringLiteral("."));
}

/*
``Use this `code`
in the code``

*/
TEST_CASE("013")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/013.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

    auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(dp, 0, 0, 12, 1);

    REQUIRE(dp->items().size() == 1);

    REQUIRE(dp->items().at(0)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(dp->items().at(0).get());

    REQUIRE(c->isInline() == true);
    REQUIRE(c->text() == QStringLiteral("Use this `code` in the code"));
    CHECK_POSITIONS(c, 2, 0, 10, 1);
    REQUIRE(c->startDelim() == MD::WithPosition{0, 0, 1, 0});
    REQUIRE(c->endDelim() == MD::WithPosition{11, 1, 12, 1});
}

TEST_CASE("014")
{
    MD::Parser parser;

    std::ofstream file("tests/parser/data/014.md", std::ios::out | std::ios::trunc | std::ios::binary);

    if (file.good()) {
        const char *str = "Line 1...\rLine 2...\r\nLine 3...\n";
        file.write(str, strlen(str));
        file.close();

        auto doc = parser.parse(QStringLiteral("tests/parser/data/014.md"));

        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        CHECK_POSITIONS(dp, 0, 0, 8, 2);

        REQUIRE(dp->items().size() == 3);

        {
            REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);

            auto dt = static_cast<MD::Text *>(dp->items().at(0).get());

            REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(dt->text() == QStringLiteral("Line 1..."));
            CHECK_POSITIONS(dt, 0, 0, 8, 0);
        }

        {
            REQUIRE(dp->items().at(1)->type() == MD::ItemType::Text);

            auto dt = static_cast<MD::Text *>(dp->items().at(1).get());

            REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(dt->text() == QStringLiteral("Line 2..."));
            CHECK_POSITIONS(dt, 0, 1, 8, 1);
        }

        {
            REQUIRE(dp->items().at(2)->type() == MD::ItemType::Text);

            auto dt = static_cast<MD::Text *>(dp->items().at(2).get());

            REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(dt->text() == QStringLiteral("Line 3..."));
            CHECK_POSITIONS(dt, 0, 2, 8, 2);
        }
    } else
        REQUIRE(true == false);
}

TEST_CASE("015")
{
    MD::Parser parser;

    std::ofstream file("tests/parser/data/015.md", std::ios::out | std::ios::trunc | std::ios::binary);

    if (file.good()) {
        const char *str = "Line 1...\r\rLine 2...\r\rLine 3...\r";
        file.write(str, strlen(str));
        file.close();

        auto doc = parser.parse(QStringLiteral("tests/parser/data/015.md"));

        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 4);

        {
            REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

            auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
            CHECK_POSITIONS(dp, 0, 0, 8, 0);

            REQUIRE(dp->items().size() == 1);

            REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(dp->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Line 1..."));
            CHECK_POSITIONS(t, 0, 0, 8, 0);
        }

        {
            REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);

            auto dp = static_cast<MD::Paragraph *>(doc->items().at(2).get());
            CHECK_POSITIONS(dp, 0, 2, 8, 2);

            REQUIRE(dp->items().size() == 1);

            REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(dp->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Line 2..."));
            CHECK_POSITIONS(t, 0, 2, 8, 2);
        }

        {
            REQUIRE(doc->items().at(3)->type() == MD::ItemType::Paragraph);

            auto dp = static_cast<MD::Paragraph *>(doc->items().at(3).get());
            CHECK_POSITIONS(dp, 0, 4, 8, 4);

            REQUIRE(dp->items().size() == 1);

            REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(dp->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Line 3..."));
            CHECK_POSITIONS(t, 0, 4, 8, 4);
        }
    } else
        REQUIRE(true == false);
}

TEST_CASE("016")
{
    MD::Parser parser;

    std::ofstream file("tests/parser/data/016.md", std::ios::out | std::ios::trunc | std::ios::binary);

    if (file.good()) {
        const char *str = "Line 1...\r\nLine 2...\r\nLine 3...\r\n";
        file.write(str, strlen(str));
        file.close();

        auto doc = parser.parse(QStringLiteral("tests/parser/data/016.md"));

        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto dp = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        CHECK_POSITIONS(dp, 0, 0, 8, 2);

        REQUIRE(dp->items().size() == 3);

        {
            REQUIRE(dp->items().at(0)->type() == MD::ItemType::Text);

            auto dt = static_cast<MD::Text *>(dp->items().at(0).get());

            REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(dt->text() == QStringLiteral("Line 1..."));
            CHECK_POSITIONS(dt, 0, 0, 8, 0);
        }

        {
            REQUIRE(dp->items().at(1)->type() == MD::ItemType::Text);

            auto dt = static_cast<MD::Text *>(dp->items().at(1).get());

            REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(dt->text() == QStringLiteral("Line 2..."));
            CHECK_POSITIONS(dt, 0, 1, 8, 1);
        }

        {
            REQUIRE(dp->items().at(2)->type() == MD::ItemType::Text);

            auto dt = static_cast<MD::Text *>(dp->items().at(2).get());

            REQUIRE(dt->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(dt->text() == QStringLiteral("Line 3..."));
            CHECK_POSITIONS(dt, 0, 2, 8, 2);
        }
    } else
        REQUIRE(true == false);
}

/*
> Quote paragraph 1.
>
> Quote paragraph 2.
>
>> Nested quote

*/
TEST_CASE("017")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/017.md"));

    auto checkDoc = [](QSharedPointer<MD::Document> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);

        auto bq = static_cast<MD::Blockquote *>(doc->items().at(1).get());
        CHECK_POSITIONS(bq, 0, 0, 14, 4);
        REQUIRE(bq->delims()
                == MD::Blockquote::Delims{{0, 0, 0, 0}, {0, 1, 0, 1}, {0, 2, 0, 2}, {0, 3, 0, 3}, {0, 4, 0, 4}});

        REQUIRE(!bq->isEmpty());
        REQUIRE(bq->items().size() == 3);

        {
            REQUIRE(bq->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(bq->items().at(0).get());
            CHECK_POSITIONS(p, 2, 0, 19, 0);

            REQUIRE(!p->isEmpty());
            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Quote paragraph 1."));
            CHECK_POSITIONS(t, 2, 0, 19, 0);
        }

        {
            REQUIRE(bq->items().at(1)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(bq->items().at(1).get());
            CHECK_POSITIONS(p, 2, 2, 19, 2);

            REQUIRE(!p->isEmpty());
            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Quote paragraph 2."));
            CHECK_POSITIONS(t, 2, 2, 19, 2);
        }

        REQUIRE(bq->items().at(2)->type() == MD::ItemType::Blockquote);

        auto nbq = static_cast<MD::Blockquote *>(bq->items().at(2).get());
        CHECK_POSITIONS(nbq, 1, 4, 14, 4);
        REQUIRE(nbq->delims() == MD::Blockquote::Delims{{1, 4, 1, 4}});

        REQUIRE(!nbq->isEmpty());
        REQUIRE(nbq->items().size() == 1);

        REQUIRE(nbq->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(nbq->items().at(0).get());
        CHECK_POSITIONS(p, 3, 4, 14, 4);

        REQUIRE(!p->isEmpty());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Nested quote"));
        CHECK_POSITIONS(t, 3, 4, 14, 4);
    };

    checkDoc(doc);

    checkDoc(doc->clone().staticCast<MD::Document>());
}

/*
> Quote paragraph 1.
>
> Quote paragraph 2.
>
> > Nested quote

*/
TEST_CASE("018")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/018.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);

    auto bq = static_cast<MD::Blockquote *>(doc->items().at(1).get());
    CHECK_POSITIONS(bq, 0, 0, 15, 4);
    REQUIRE(bq->delims()
            == MD::Blockquote::Delims{{0, 0, 0, 0}, {0, 1, 0, 1}, {0, 2, 0, 2}, {0, 3, 0, 3}, {0, 4, 0, 4}});

    REQUIRE(!bq->isEmpty());
    REQUIRE(bq->items().size() == 3);

    {
        REQUIRE(bq->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(bq->items().at(0).get());
        CHECK_POSITIONS(p, 2, 0, 19, 0);

        REQUIRE(!p->isEmpty());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Quote paragraph 1."));
        CHECK_POSITIONS(t, 2, 0, 19, 0);
    }

    {
        REQUIRE(bq->items().at(1)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(bq->items().at(1).get());
        CHECK_POSITIONS(p, 2, 2, 19, 2);

        REQUIRE(!p->isEmpty());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Quote paragraph 2."));
        CHECK_POSITIONS(t, 2, 2, 19, 2);
    }

    REQUIRE(bq->items().at(2)->type() == MD::ItemType::Blockquote);

    auto nbq = static_cast<MD::Blockquote *>(bq->items().at(2).get());
    CHECK_POSITIONS(nbq, 2, 4, 15, 4);
    REQUIRE(nbq->delims() == MD::Blockquote::Delims{{2, 4, 2, 4}});

    REQUIRE(!nbq->isEmpty());
    REQUIRE(nbq->items().size() == 1);

    REQUIRE(nbq->items().at(0)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(nbq->items().at(0).get());
    CHECK_POSITIONS(p, 4, 4, 15, 4);

    REQUIRE(!p->isEmpty());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

    auto t = static_cast<MD::Text *>(p->items().at(0).get());

    REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Nested quote"));
    CHECK_POSITIONS(t, 4, 4, 15, 4);
}

/*
> Quote paragraph 1.
>
> Quote paragraph 2.
>
> > Nested quote

> Quote paragraph 1.
>
> Quote paragraph 2.
>
> > Nested quote

*/
TEST_CASE("019")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/019.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    for (int i = 1; i < 3; ++i) {
        REQUIRE(doc->items().at(i)->type() == MD::ItemType::Blockquote);

        auto bq = static_cast<MD::Blockquote *>(doc->items().at(i).get());
        CHECK_POSITIONS(bq, 0, 0 + 6 * (i - 1), 15, 4 + 6 * (i - 1));
        REQUIRE(bq->delims()
                == MD::Blockquote::Delims{{0, 0 + (i - 1) * 6, 0, 0 + (i - 1) * 6},
                                          {0, 1 + (i - 1) * 6, 0, 1 + (i - 1) * 6},
                                          {0, 2 + (i - 1) * 6, 0, 2 + (i - 1) * 6},
                                          {0, 3 + (i - 1) * 6, 0, 3 + (i - 1) * 6},
                                          {0, 4 + (i - 1) * 6, 0, 4 + (i - 1) * 6}});

        REQUIRE(!bq->isEmpty());
        REQUIRE(bq->items().size() == 3);

        {
            REQUIRE(bq->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(bq->items().at(0).get());
            CHECK_POSITIONS(p, 2, 0 + 6 * (i - 1), 19, 0 + 6 * (i - 1));

            REQUIRE(!p->isEmpty());
            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Quote paragraph 1."));
            CHECK_POSITIONS(t, 2, 0 + 6 * (i - 1), 19, 0 + 6 * (i - 1));
        }

        {
            REQUIRE(bq->items().at(1)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(bq->items().at(1).get());
            CHECK_POSITIONS(p, 2, 2 + 6 * (i - 1), 19, 2 + 6 * (i - 1));

            REQUIRE(!p->isEmpty());
            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Quote paragraph 2."));
            CHECK_POSITIONS(t, 2, 2 + 6 * (i - 1), 19, 2 + 6 * (i - 1));
        }

        REQUIRE(bq->items().at(2)->type() == MD::ItemType::Blockquote);

        auto nbq = static_cast<MD::Blockquote *>(bq->items().at(2).get());
        CHECK_POSITIONS(nbq, 2, 4 + 6 * (i - 1), 15, 4 + 6 * (i - 1));
        REQUIRE(nbq->delims() == MD::Blockquote::Delims{{2, 4 + (i - 1) * 6, 2, 4 + (i - 1) * 6}});

        REQUIRE(!nbq->isEmpty());
        REQUIRE(nbq->items().size() == 1);

        REQUIRE(nbq->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(nbq->items().at(0).get());
        CHECK_POSITIONS(p, 4, 4 + 6 * (i - 1), 15, 4 + 6 * (i - 1));

        REQUIRE(!p->isEmpty());
        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Nested quote"));
        CHECK_POSITIONS(t, 4, 4 + 6 * (i - 1), 15, 4 + 6 * (i - 1));
    }
}

/*
```cpp
if( a > b )
  do_something();
else
  dont_do_anything();
```

*/
TEST_CASE("020")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/020.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(doc->items().at(1).get());
    CHECK_POSITIONS(c, 0, 1, 20, 4);
    REQUIRE(c->startDelim() == MD::WithPosition{0, 0, 2, 0});
    REQUIRE(c->endDelim() == MD::WithPosition{0, 5, 2, 5});
    REQUIRE(c->syntaxPos() == MD::WithPosition{3, 0, 5, 0});

    REQUIRE(c->isInline() == false);
    REQUIRE(c->text() == QStringLiteral("if( a > b )\n  do_something();\nelse\n  dont_do_anything();"));
    REQUIRE(c->syntax() == QStringLiteral("cpp"));
}

/*
    if( a > b )
      do_something();
    else
      dont_do_anything();
*/
TEST_CASE("021")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/021.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(doc->items().at(1).get());
    CHECK_POSITIONS(c, 4, 0, 24, 3);
    REQUIRE(c->startDelim() == MD::WithPosition{-1, -1, -1, -1});
    REQUIRE(c->endDelim() == MD::WithPosition{-1, -1, -1, -1});
    REQUIRE(c->syntaxPos() == MD::WithPosition{-1, -1, -1, -1});

    REQUIRE(c->isInline() == false);
    REQUIRE(c->text() == QStringLiteral("if( a > b )\n  do_something();\nelse\n  dont_do_anything();"));
}

/*
    if( a > b )
      do_something();
    else
      dont_do_anything();

*/
TEST_CASE("022")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/022.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);

    auto c = static_cast<MD::Code *>(doc->items().at(1).get());
    CHECK_POSITIONS(c, 1, 0, 21, 3);

    REQUIRE(c->isInline() == false);
    REQUIRE(c->text() == QStringLiteral("if( a > b )\n  do_something();\nelse\n  dont_do_anything();"));
}

/*
* Item 1
* Item 2
* Item 3

*/
TEST_CASE("023")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/023.md"));

    auto checkDoc = [](QSharedPointer<MD::Document> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

        auto l = static_cast<MD::List *>(doc->items().at(1).get());
        CHECK_POSITIONS(l, 0, 0, 0, 3);

        REQUIRE(l->items().size() == 3);

        for (int i = 0; i < 3; ++i) {
            REQUIRE(l->items().at(i)->type() == MD::ItemType::ListItem);

            auto item = static_cast<MD::ListItem *>(l->items().at(i).get());
            CHECK_POSITIONS(item, 0, i, (i < 2 ? 7 : 0), (i < 2 ? i : i + 1));
            REQUIRE(item->delim() == MD::WithPosition{0, i, 0, i});

            REQUIRE(item->listType() == MD::ListItem::Unordered);

            REQUIRE(item->items().size() == 1);

            REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
            CHECK_POSITIONS(p, 2, i, 7, i);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Item ") + to_string(i + 1));
            CHECK_POSITIONS(t, 2, i, 7, i);
        }
    };

    checkDoc(doc);

    checkDoc(doc->clone().staticCast<MD::Document>());
}

/*
* Item 1
  * Item 1
  * Item 2
* Item 2
  * Item 1
  * Item 2
* Item 3
  * Item 1
  * Item 2

*/
TEST_CASE("024")
{
    auto checkItem = [](MD::ListItem *item, int i, int indent, int line) {
        REQUIRE(item->delim() == MD::WithPosition{indent, line, indent, line});
        REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
        CHECK_POSITIONS(p, 2 + indent, line, 7 + indent, line);

        REQUIRE(p->items().size() == 1);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t = static_cast<MD::Text *>(p->items().at(0).get());

        REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Item ") + to_string(i + 1));
        CHECK_POSITIONS(t, 2 + indent, line, 7 + indent, line);
    };

    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/024.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 9);

    REQUIRE(l->items().size() == 3);

    for (int i = 0; i < 3; ++i) {
        REQUIRE(l->items().at(i)->type() == MD::ItemType::ListItem);

        auto item = static_cast<MD::ListItem *>(l->items().at(i).get());
        CHECK_POSITIONS(item, 0, i * 3, (i < 2 ? 9 : 0), 2 + i * 3 + (i < 2 ? 0 : 1));

        REQUIRE(item->listType() == MD::ListItem::Unordered);

        REQUIRE(item->items().size() == 2);

        checkItem(item, i, 0, i * 3);

        REQUIRE(item->items().at(1)->type() == MD::ItemType::List);

        auto nl = static_cast<MD::List *>(item->items().at(1).get());

        REQUIRE(nl->items().size() == 2);

        for (int j = 0; j < 2; ++j) {
            REQUIRE(nl->items().at(j)->type() == MD::ItemType::ListItem);

            auto nitem = static_cast<MD::ListItem *>(nl->items().at(j).get());

            checkItem(nitem, j, 2, j + i * 3 + 1);
        }
    }
}

/*
* Item 1

  Paragraph in list

* Item 2

  Paragraph in list

* Item 3

  Paragraph in list

*/
TEST_CASE("025")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/025.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 11);

    REQUIRE(l->items().size() == 3);

    for (int i = 0; i < 3; ++i) {
        REQUIRE(l->items().at(i)->type() == MD::ItemType::ListItem);

        auto item = static_cast<MD::ListItem *>(l->items().at(i).get());
        CHECK_POSITIONS(item, 0, i * 4, (i < 2 ? 3 : 0), 3 + i * 4);
        REQUIRE(item->delim() == MD::WithPosition{0, i * 4, 0, i * 4});

        REQUIRE(item->listType() == MD::ListItem::Unordered);

        REQUIRE(item->items().size() == 2);

        {
            REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
            CHECK_POSITIONS(p, 2, i * 4, 7, i * 4);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == (QStringLiteral("Item ") + to_string(i + 1)));
            CHECK_POSITIONS(t, 2, i * 4, 7, i * 4);
        }

        {
            REQUIRE(item->items().at(1)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(item->items().at(1).get());
            CHECK_POSITIONS(p, 2, 2 + i * 4, 18, 2 + i * 4);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Paragraph in list"));
            CHECK_POSITIONS(t, 2, 2 + i * 4, 18, 2 + i * 4);
        }
    }
}

/*
* Item 1

  Paragraph in list

  * Nested

    Paragraph in list

* Item 2

  Paragraph in list

  * Nested

    Paragraph in list

* Item 3

  Paragraph in list

  * Nested

    Paragraph in list

*/
TEST_CASE("026")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/026.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 23);

    REQUIRE(l->items().size() == 3);

    for (int i = 0; i < 3; ++i) {
        REQUIRE(l->items().at(i)->type() == MD::ItemType::ListItem);

        auto item = static_cast<MD::ListItem *>(l->items().at(i).get());
        CHECK_POSITIONS(item, 0, 8 * i, 0, 7 + 8 * i);
        REQUIRE(item->delim() == MD::WithPosition{0, i * 8, 0, i * 8});

        REQUIRE(item->listType() == MD::ListItem::Unordered);

        REQUIRE(item->items().size() == 3);

        {
            REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
            CHECK_POSITIONS(p, 2, 8 * i, 7, 8 * i);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == (QStringLiteral("Item ") + to_string(i + 1)));
            CHECK_POSITIONS(t, 2, 8 * i, 7, 8 * i);
        }

        {
            REQUIRE(item->items().at(1)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(item->items().at(1).get());
            CHECK_POSITIONS(p, 2, 2 + 8 * i, 18, 2 + 8 * i);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Paragraph in list"));
            CHECK_POSITIONS(t, 2, 2 + 8 * i, 18, 2 + 8 * i);
        }

        {
            REQUIRE(item->items().at(2)->type() == MD::ItemType::List);

            auto nl = static_cast<MD::List *>(item->items().at(2).get());
            CHECK_POSITIONS(nl, 2, 4 + 8 * i, 0, 7 + 8 * i);

            REQUIRE(nl->items().at(0)->type() == MD::ItemType::ListItem);

            auto item = static_cast<MD::ListItem *>(nl->items().at(0).get());
            CHECK_POSITIONS(item, 2, 4 + 8 * i, 0, 7 + 8 * i);
            REQUIRE(item->delim() == MD::WithPosition{2, 4 + 8 * i, 2, 4 + 8 * i});

            REQUIRE(item->listType() == MD::ListItem::Unordered);

            REQUIRE(item->items().size() == 2);

            {
                REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

                auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
                CHECK_POSITIONS(p, 4, 4 + 8 * i, 9, 4 + 8 * i);

                REQUIRE(p->items().size() == 1);

                REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

                auto t = static_cast<MD::Text *>(p->items().at(0).get());

                REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
                REQUIRE(t->text() == QStringLiteral("Nested"));
                CHECK_POSITIONS(t, 4, 4 + 8 * i, 9, 4 + 8 * i);
            }

            {
                REQUIRE(item->items().at(1)->type() == MD::ItemType::Paragraph);

                auto p = static_cast<MD::Paragraph *>(item->items().at(1).get());
                CHECK_POSITIONS(p, 4, 6 + 8 * i, 20, 6 + 8 * i);

                REQUIRE(p->items().size() == 1);

                REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

                auto t = static_cast<MD::Text *>(p->items().at(0).get());

                REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
                REQUIRE(t->text() == QStringLiteral("Paragraph in list"));
                CHECK_POSITIONS(t, 4, 6 + 8 * i, 20, 6 + 8 * i);
            }
        }
    }
}

/*
* Item 1

      code

* Item 2

      code

* Item 3

      code

*/
TEST_CASE("027")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/027.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 11);

    REQUIRE(l->items().size() == 3);

    for (int i = 0; i < 3; ++i) {
        REQUIRE(l->items().at(i)->type() == MD::ItemType::ListItem);

        auto item = static_cast<MD::ListItem *>(l->items().at(i).get());
        CHECK_POSITIONS(item, 0, 4 * i, (i < 2 ? 3 : 0), 3 + 4 * i);
        REQUIRE(item->delim() == MD::WithPosition{0, i * 4, 0, i * 4});

        REQUIRE(item->listType() == MD::ListItem::Unordered);

        REQUIRE(item->items().size() == 2);

        {
            REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
            CHECK_POSITIONS(p, 2, 4 * i, 7, 4 * i);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Item ") + to_string(i + 1));
            CHECK_POSITIONS(t, 2, 4 * i, 7, 4 * i);
        }

        {
            REQUIRE(item->items().at(1)->type() == MD::ItemType::Code);

            auto c = static_cast<MD::Code *>(item->items().at(1).get());
            CHECK_POSITIONS(c, 6, 2 + 4 * i, 9, 2 + 4 * i);

            REQUIRE(c->isInline() == false);
            REQUIRE(c->text() == QStringLiteral("code"));
        }
    }
}

/*
* Item 1

    ```
    code
    ```

* Item 2

    ```
    code
    ```

* Item 3

    ```
    code
    ```

*/
TEST_CASE("028")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/028.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 17);

    REQUIRE(l->items().size() == 3);

    for (int i = 0; i < 3; ++i) {
        REQUIRE(l->items().at(i)->type() == MD::ItemType::ListItem);

        auto item = static_cast<MD::ListItem *>(l->items().at(i).get());
        CHECK_POSITIONS(item, 0, 6 * i, (i < 2 ? 3 : 0), 5 + 6 * i);
        REQUIRE(item->delim() == MD::WithPosition{0, i * 6, 0, i * 6});

        REQUIRE(item->listType() == MD::ListItem::Unordered);

        REQUIRE(item->items().size() == 2);

        {
            REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
            CHECK_POSITIONS(p, 2, 6 * i, 7, 6 * i);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Item ") + to_string(i + 1));
            CHECK_POSITIONS(t, 2, 6 * i, 7, 6 * i);
        }

        {
            REQUIRE(item->items().at(1)->type() == MD::ItemType::Code);

            auto c = static_cast<MD::Code *>(item->items().at(1).get());
            CHECK_POSITIONS(c, 4, 3 + 6 * i, 7, 3 + 6 * i);
            REQUIRE(c->startDelim() == MD::WithPosition{4, 2 + 6 * i, 6, 2 + 6 * i});
            REQUIRE(c->endDelim() == MD::WithPosition{4, 4 + 6 * i, 6, 4 + 6 * i});
            REQUIRE(c->syntaxPos() == MD::WithPosition{-1, -1, -1, -1});

            REQUIRE(c->isInline() == false);
            REQUIRE(c->text() == QStringLiteral("code"));
        }
    }
}

/*
* Item 1

    Paragraph in list

    * Nested

        Paragraph in list

* Item 2

    Paragraph in list

    * Nested

        Paragraph in list

* Item 3

    Paragraph in list

    * Nested

        Paragraph in list

Standalone paragraph

*/
TEST_CASE("029")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/029.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);

    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    CHECK_POSITIONS(l, 0, 0, 0, 23);

    REQUIRE(l->items().size() == 3);

    for (int i = 0; i < 3; ++i) {
        REQUIRE(l->items().at(i)->type() == MD::ItemType::ListItem);

        auto item = static_cast<MD::ListItem *>(l->items().at(i).get());
        CHECK_POSITIONS(item, 0, 8 * i, (i < 2 ? 3 : 0), 7 + 8 * i);
        REQUIRE(item->delim() == MD::WithPosition{0, 8 * i, 0, 8 * i});

        REQUIRE(item->listType() == MD::ListItem::Unordered);

        REQUIRE(item->items().size() == 3);

        {
            REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
            CHECK_POSITIONS(p, 2, 8 * i, 7, 8 * i);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Item ") + to_string(i + 1));
            CHECK_POSITIONS(t, 2, 8 * i, 7, 8 * i);
        }

        {
            REQUIRE(item->items().at(1)->type() == MD::ItemType::Paragraph);

            auto p = static_cast<MD::Paragraph *>(item->items().at(1).get());
            CHECK_POSITIONS(p, 4, 2 + 8 * i, 20, 2 + 8 * i);

            REQUIRE(p->items().size() == 1);

            REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

            auto t = static_cast<MD::Text *>(p->items().at(0).get());

            REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
            REQUIRE(t->text() == QStringLiteral("Paragraph in list"));
            CHECK_POSITIONS(t, 4, 2 + 8 * i, 20, 2 + 8 * i);
        }

        {
            REQUIRE(item->items().at(2)->type() == MD::ItemType::List);

            auto nl = static_cast<MD::List *>(item->items().at(2).get());
            CHECK_POSITIONS(nl, 4, 4 + 8 * i, (i < 2 ? 3 : 0), 7 + 8 * i);

            REQUIRE(nl->items().at(0)->type() == MD::ItemType::ListItem);

            auto item = static_cast<MD::ListItem *>(nl->items().at(0).get());
            CHECK_POSITIONS(item, 4, 4 + 8 * i, (i < 2 ? 3 : 0), 7 + 8 * i);
            REQUIRE(item->delim() == MD::WithPosition{4, 4 + 8 * i, 4, 4 + 8 * i});

            REQUIRE(item->listType() == MD::ListItem::Unordered);

            REQUIRE(item->items().size() == 2);

            {
                REQUIRE(item->items().at(0)->type() == MD::ItemType::Paragraph);

                auto p = static_cast<MD::Paragraph *>(item->items().at(0).get());
                CHECK_POSITIONS(p, 6, 4 + 8 * i, 11, 4 + 8 * i);

                REQUIRE(p->items().size() == 1);

                REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

                auto t = static_cast<MD::Text *>(p->items().at(0).get());

                REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
                REQUIRE(t->text() == QStringLiteral("Nested"));
                CHECK_POSITIONS(t, 6, 4 + 8 * i, 11, 4 + 8 * i);
            }

            {
                REQUIRE(item->items().at(1)->type() == MD::ItemType::Paragraph);

                auto p = static_cast<MD::Paragraph *>(item->items().at(1).get());
                CHECK_POSITIONS(p, 8, 6 + 8 * i, 24, 6 + 8 * i);

                REQUIRE(p->items().size() == 1);

                REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

                auto t = static_cast<MD::Text *>(p->items().at(0).get());

                REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
                REQUIRE(t->text() == QStringLiteral("Paragraph in list"));
                CHECK_POSITIONS(t, 8, 6 + 8 * i, 24, 6 + 8 * i);
            }
        }
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);

    auto p = static_cast<MD::Paragraph *>(doc->items().at(2).get());
    CHECK_POSITIONS(p, 0, 24, 19, 24);

    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

    auto t = static_cast<MD::Text *>(p->items().at(0).get());

    REQUIRE(t->opts() == MD::TextOption::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Standalone paragraph"));
    CHECK_POSITIONS(t, 0, 24, 19, 24);
}

/*
Text ![Image 1](a.jpg) continue ![ Image 2 ](b.png) and ![ Image 3]( http://www.where.com/c.jpeg "description" )

*/
TEST_CASE("030")
{
    MD::Parser parser;

    QString wd = QDir().absolutePath() + QStringLiteral("/tests/parser/data/");

    auto doc = parser.parse(QStringLiteral("tests/parser/data/030.md"));

    auto checkDoc = [&wd](QSharedPointer<MD::Document> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 2);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);

        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        CHECK_POSITIONS(p, 0, 0, 111, 0);

        REQUIRE(p->items().size() == 6);

        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);

        auto t1 = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t1, 0, 0, 4, 0);

        REQUIRE(t1->text() == QStringLiteral("Text "));

        REQUIRE(p->items().at(1)->type() == MD::ItemType::Image);

        auto i1 = static_cast<MD::Image *>(p->items().at(1).get());
        CHECK_POSITIONS(i1, 5, 0, 21, 0);
        REQUIRE(i1->textPos() == MD::WithPosition{7, 0, 13, 0});
        REQUIRE(i1->urlPos() == MD::WithPosition{16, 0, 20, 0});

        REQUIRE(i1->text() == QStringLiteral("Image 1"));
        REQUIRE(i1->url() == wd + QStringLiteral("a.jpg"));

        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);

        auto t2 = static_cast<MD::Text *>(p->items().at(2).get());
        CHECK_POSITIONS(t2, 22, 0, 31, 0);

        REQUIRE(t2->text() == QStringLiteral(" continue "));

        REQUIRE(p->items().at(3)->type() == MD::ItemType::Image);

        auto i2 = static_cast<MD::Image *>(p->items().at(3).get());
        CHECK_POSITIONS(i2, 32, 0, 50, 0);
        REQUIRE(i2->textPos() == MD::WithPosition{34, 0, 42, 0});
        REQUIRE(i2->urlPos() == MD::WithPosition{45, 0, 49, 0});

        REQUIRE(i2->text() == QStringLiteral("Image 2"));
        REQUIRE(i2->url() == wd + QStringLiteral("b.png"));

        REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);

        auto t3 = static_cast<MD::Text *>(p->items().at(4).get());
        CHECK_POSITIONS(t3, 51, 0, 55, 0);

        REQUIRE(t3->text() == QStringLiteral(" and "));

        REQUIRE(p->items().at(5)->type() == MD::ItemType::Image);

        auto i3 = static_cast<MD::Image *>(p->items().at(5).get());
        CHECK_POSITIONS(i3, 56, 0, 111, 0);
        REQUIRE(i3->textPos() == MD::WithPosition{58, 0, 65, 0});
        REQUIRE(i3->urlPos() == MD::WithPosition{69, 0, 95, 0});

        REQUIRE(i3->text() == QStringLiteral("Image 3"));
        REQUIRE(i3->url() == QStringLiteral("http://www.where.com/c.jpeg"));
    };

    checkDoc(doc);

    checkDoc(doc->clone().staticCast<MD::Document>());
}
