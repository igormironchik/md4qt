
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include <md4qt/parser.h>
#include <md4qt/plugins.h>

template<class Trait>
std::shared_ptr<MD::Document<Trait>> loadTest(const typename Trait::String &name)
{
    MD::Parser<TRAIT> p;
    p.addTextPlugin(MD::TextPlugin::UserDefined,
                    MD::EmphasisPlugin::emphasisTemplatePlugin<TRAIT>,
                    true,
                    typename Trait::StringList() << Trait::latin1ToString("^") << Trait::latin1ToString("8"));
    return p.parse(Trait::latin1ToString("tests/plugins/emphasis/data/") + name + Trait::latin1ToString(".md"));
}

TEST_CASE("001")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("001"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 1);
    REQUIRE(doc->items().at(0)->type() == MD::ItemType::Anchor);
}

/*
text

*/
TEST_CASE("002")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("002"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->text() == TRAIT::latin1ToString("text"));
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->openStyles().empty());
    REQUIRE(t->closeStyles().empty());
}

/*
*text*

*/
TEST_CASE("003")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("003"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->text() == TRAIT::latin1ToString("text"));
    REQUIRE(t->opts() == MD::ItalicText);
    REQUIRE(t->openStyles().size() == 1);
    REQUIRE(t->openStyles().front().style() == MD::ItalicText);
    REQUIRE(t->closeStyles().size() == 1);
    REQUIRE(t->closeStyles().front().style() == MD::ItalicText);
}

/*
^text^

*/
TEST_CASE("004")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("004"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->text() == TRAIT::latin1ToString("text"));
    REQUIRE(t->opts() == 8);
    REQUIRE(t->startColumn() == 1);
    REQUIRE(t->endColumn() == 4);
    REQUIRE(t->openStyles().size() == 1);
    const auto &d1 = t->openStyles().front();
    REQUIRE(d1.style() == 8);
    REQUIRE(d1.startColumn() == 0);
    REQUIRE(d1.startLine() == 0);
    REQUIRE(d1.endColumn() == 0);
    REQUIRE(d1.endLine() == 0);
    REQUIRE(t->closeStyles().size() == 1);
    const auto &d2 = t->closeStyles().front();
    REQUIRE(d2.style() == 8);
    REQUIRE(d2.startColumn() == 5);
    REQUIRE(d2.startLine() == 0);
    REQUIRE(d2.endColumn() == 5);
    REQUIRE(d2.endLine() == 0);
}

/*
*^text^*

*/
TEST_CASE("005")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("005"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->text() == TRAIT::latin1ToString("text"));
    REQUIRE(t->opts() == (8 | MD::ItalicText));
    REQUIRE(t->startColumn() == 2);
    REQUIRE(t->endColumn() == 5);

    {
        REQUIRE(t->openStyles().size() == 2);
        const auto &d1 = t->openStyles()[0];
        REQUIRE(d1.style() == MD::ItalicText);
        REQUIRE(d1.startColumn() == 0);
        REQUIRE(d1.startLine() == 0);
        REQUIRE(d1.endColumn() == 0);
        REQUIRE(d1.endLine() == 0);

        const auto &d2 = t->openStyles()[1];
        REQUIRE(d2.style() == 8);
        REQUIRE(d2.startColumn() == 1);
        REQUIRE(d2.startLine() == 0);
        REQUIRE(d2.endColumn() == 1);
        REQUIRE(d2.endLine() == 0);
    }

    {
        REQUIRE(t->closeStyles().size() == 2);
        const auto &d1 = t->closeStyles()[0];
        REQUIRE(d1.style() == 8);
        REQUIRE(d1.startColumn() == 6);
        REQUIRE(d1.startLine() == 0);
        REQUIRE(d1.endColumn() == 6);
        REQUIRE(d1.endLine() == 0);

        const auto &d2 = t->closeStyles()[1];
        REQUIRE(d2.style() == MD::ItalicText);
        REQUIRE(d2.startColumn() == 7);
        REQUIRE(d2.startLine() == 0);
        REQUIRE(d2.endColumn() == 7);
        REQUIRE(d2.endLine() == 0);
    }
}

/*
^*text*^

*/
TEST_CASE("006")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("006"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->text() == TRAIT::latin1ToString("text"));
    REQUIRE(t->opts() == (8 | MD::ItalicText));
    REQUIRE(t->startColumn() == 2);
    REQUIRE(t->endColumn() == 5);

    {
        REQUIRE(t->openStyles().size() == 2);
        const auto &d1 = t->openStyles()[0];
        REQUIRE(d1.style() == 8);
        REQUIRE(d1.startColumn() == 0);
        REQUIRE(d1.startLine() == 0);
        REQUIRE(d1.endColumn() == 0);
        REQUIRE(d1.endLine() == 0);

        const auto &d2 = t->openStyles()[1];
        REQUIRE(d2.style() == MD::ItalicText);
        REQUIRE(d2.startColumn() == 1);
        REQUIRE(d2.startLine() == 0);
        REQUIRE(d2.endColumn() == 1);
        REQUIRE(d2.endLine() == 0);
    }

    {
        REQUIRE(t->closeStyles().size() == 2);
        const auto &d1 = t->closeStyles()[0];
        REQUIRE(d1.style() == MD::ItalicText);
        REQUIRE(d1.startColumn() == 6);
        REQUIRE(d1.startLine() == 0);
        REQUIRE(d1.endColumn() == 6);
        REQUIRE(d1.endLine() == 0);

        const auto &d2 = t->closeStyles()[1];
        REQUIRE(d2.style() == 8);
        REQUIRE(d2.startColumn() == 7);
        REQUIRE(d2.startLine() == 0);
        REQUIRE(d2.endColumn() == 7);
        REQUIRE(d2.endLine() == 0);
    }
}

/*
^*text^*

*/
TEST_CASE("007")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("007"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("*text"));
        REQUIRE(t->opts() == 8);
        REQUIRE(t->startColumn() == 1);
        REQUIRE(t->endColumn() == 5);

        {
            REQUIRE(t->openStyles().size() == 1);
            const auto &d1 = t->openStyles()[0];
            REQUIRE(d1.style() == 8);
            REQUIRE(d1.startColumn() == 0);
            REQUIRE(d1.startLine() == 0);
            REQUIRE(d1.endColumn() == 0);
            REQUIRE(d1.endLine() == 0);
        }

        {
            REQUIRE(t->closeStyles().size() == 1);
            const auto &d1 = t->closeStyles()[0];
            REQUIRE(d1.style() == 8);
            REQUIRE(d1.startColumn() == 6);
            REQUIRE(d1.startLine() == 0);
            REQUIRE(d1.endColumn() == 6);
            REQUIRE(d1.endLine() == 0);
        }
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("*"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 7);
        REQUIRE(t->endColumn() == 7);
    }
}

/*
^*text*

*/
TEST_CASE("008")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("008"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("^"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->endColumn() == 0);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text"));
        REQUIRE(t->opts() == MD::ItalicText);
        REQUIRE(t->startColumn() == 2);
        REQUIRE(t->endColumn() == 5);
    }
}

/*
^^text^

*/
TEST_CASE("009")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("009"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("^"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->endColumn() == 0);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text"));
        REQUIRE(t->opts() == 8);
        REQUIRE(t->startColumn() == 2);
        REQUIRE(t->endColumn() == 5);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
    }
}

/*
^^text^^

*/
TEST_CASE("010")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("010"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
    REQUIRE(t->text() == TRAIT::latin1ToString("text"));
    REQUIRE(t->opts() == 8);
    REQUIRE(t->startColumn() == 2);
    REQUIRE(t->endColumn() == 5);
    REQUIRE(t->openStyles().size() == 2);
    REQUIRE(t->openStyles()[0].startColumn() == 0);
    REQUIRE(t->openStyles()[1].startColumn() == 1);
    REQUIRE(t->closeStyles().size() == 2);
    REQUIRE(t->closeStyles()[0].startColumn() == 6);
    REQUIRE(t->closeStyles()[1].startColumn() == 7);
}

/*
^text^^

*/
TEST_CASE("011")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("011"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text"));
        REQUIRE(t->opts() == 8);
        REQUIRE(t->startColumn() == 1);
        REQUIRE(t->endColumn() == 4);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("^"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 6);
        REQUIRE(t->endColumn() == 6);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }
}

/*
*^text*^

*/
TEST_CASE("012")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("012"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("^text"));
        REQUIRE(t->opts() == MD::ItalicText);
        REQUIRE(t->startColumn() == 1);
        REQUIRE(t->endColumn() == 5);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("^"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 7);
        REQUIRE(t->endColumn() == 7);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }
}

/*
^text
^

*/
TEST_CASE("013")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("013"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("^text"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->endColumn() == 4);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("^"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->endColumn() == 0);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }
}

/*
^text
text^

*/
TEST_CASE("014")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("014"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text"));
        REQUIRE(t->opts() == 8);
        REQUIRE(t->startColumn() == 1);
        REQUIRE(t->endColumn() == 4);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text"));
        REQUIRE(t->opts() == 8);
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->endColumn() == 3);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().size() == 1);
    }
}

/*
^^
^

*/
TEST_CASE("015")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("015"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("^^"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->endColumn() == 1);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("^"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 0);
        REQUIRE(t->endColumn() == 0);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }
}

/*
**text1 ^text2 *text3^***

*/
TEST_CASE("016")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("016"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text1 "));
        REQUIRE(t->opts() == MD::BoldText);
        REQUIRE(t->startColumn() == 2);
        REQUIRE(t->endColumn() == 7);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text2 *text3"));
        REQUIRE(t->opts() == (MD::BoldText | 8));
        REQUIRE(t->startColumn() == 9);
        REQUIRE(t->endColumn() == 20);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 2);
        REQUIRE(t->closeStyles().back().startColumn() == 22);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("*"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 24);
        REQUIRE(t->endColumn() == 24);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }
}

/*
**text1 ^text2 _text3^_**

*/
TEST_CASE("017")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("017"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text1 "));
        REQUIRE(t->opts() == MD::BoldText);
        REQUIRE(t->startColumn() == 2);
        REQUIRE(t->endColumn() == 7);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text2 _text3"));
        REQUIRE(t->opts() == (MD::BoldText | 8));
        REQUIRE(t->startColumn() == 9);
        REQUIRE(t->endColumn() == 20);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("_"));
        REQUIRE(t->opts() == MD::BoldText);
        REQUIRE(t->startColumn() == 22);
        REQUIRE(t->endColumn() == 22);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().size() == 1);
        REQUIRE(t->closeStyles().front().startColumn() == 23);
    }
}

/*
**text1 ^text2 _text3^**_

*/
TEST_CASE("018")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("018"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text1 "));
        REQUIRE(t->opts() == MD::BoldText);
        REQUIRE(t->startColumn() == 2);
        REQUIRE(t->endColumn() == 7);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text2 _text3"));
        REQUIRE(t->opts() == (MD::BoldText | 8));
        REQUIRE(t->startColumn() == 9);
        REQUIRE(t->endColumn() == 20);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 2);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("_"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 24);
        REQUIRE(t->endColumn() == 24);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }
}

/*
*text1 ^**text2**^ text3*

*/
TEST_CASE("019")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("019"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text1 "));
        REQUIRE(t->opts() == MD::ItalicText);
        REQUIRE(t->startColumn() == 1);
        REQUIRE(t->endColumn() == 6);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text2"));
        REQUIRE(t->opts() == (MD::BoldText | MD::ItalicText | 8));
        REQUIRE(t->startColumn() == 10);
        REQUIRE(t->endColumn() == 14);
        REQUIRE(t->openStyles().size() == 2);
        REQUIRE(t->closeStyles().size() == 2);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString(" text3"));
        REQUIRE(t->opts() == MD::ItalicText);
        REQUIRE(t->startColumn() == 18);
        REQUIRE(t->endColumn() == 23);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().size() == 1);
    }
}

/*
^*text1^ **text2***

*/
TEST_CASE("020")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("020"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("*text1"));
        REQUIRE(t->opts() == 8);
        REQUIRE(t->startColumn() == 1);
        REQUIRE(t->endColumn() == 6);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString(" "));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 8);
        REQUIRE(t->endColumn() == 8);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text2"));
        REQUIRE(t->opts() == MD::BoldText);
        REQUIRE(t->startColumn() == 11);
        REQUIRE(t->endColumn() == 15);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(3).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("*"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 18);
        REQUIRE(t->endColumn() == 18);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }
}

/*
^*text1^ _text2_ **text3***

*/
TEST_CASE("021")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("021"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 6);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("*text1"));
        REQUIRE(t->opts() == 8);
        REQUIRE(t->startColumn() == 1);
        REQUIRE(t->endColumn() == 6);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString(" "));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 8);
        REQUIRE(t->endColumn() == 8);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text2"));
        REQUIRE(t->opts() == MD::ItalicText);
        REQUIRE(t->startColumn() == 10);
        REQUIRE(t->endColumn() == 14);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(3).get());
        REQUIRE(t->text() == TRAIT::latin1ToString(" "));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 16);
        REQUIRE(t->endColumn() == 16);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(4).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text3"));
        REQUIRE(t->opts() == MD::BoldText);
        REQUIRE(t->startColumn() == 19);
        REQUIRE(t->endColumn() == 23);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
    }

    {
        REQUIRE(p->items().at(5)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(5).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("*"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 26);
        REQUIRE(t->endColumn() == 26);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }
}

/*
^*text1 **text2 ~text3^~***

*/
TEST_CASE("022")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("022"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("*text1 **text2 ~text3"));
        REQUIRE(t->opts() == 8);
        REQUIRE(t->startColumn() == 1);
        REQUIRE(t->endColumn() == 21);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("~***"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 23);
        REQUIRE(t->endColumn() == 26);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }
}

/*
*text1 **text2 ^~text3^~***

*/
TEST_CASE("023")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("023"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text1 "));
        REQUIRE(t->opts() == MD::ItalicText);
        REQUIRE(t->startColumn() == 1);
        REQUIRE(t->endColumn() == 6);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text2 "));
        REQUIRE(t->opts() == (MD::ItalicText | MD::BoldText));
        REQUIRE(t->startColumn() == 9);
        REQUIRE(t->endColumn() == 14);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("~text3"));
        REQUIRE(t->opts() == (MD::ItalicText | MD::BoldText | 8));
        REQUIRE(t->startColumn() == 16);
        REQUIRE(t->endColumn() == 21);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(3).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("~"));
        REQUIRE(t->opts() == (MD::ItalicText | MD::BoldText));
        REQUIRE(t->startColumn() == 23);
        REQUIRE(t->endColumn() == 23);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().size() == 2);
    }
}

/*
^**[link](/url)^**

*/
TEST_CASE("024")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("024"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("**"));
        REQUIRE(t->opts() == 8);
        REQUIRE(t->startColumn() == 1);
        REQUIRE(t->endColumn() == 2);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(1).get());
        REQUIRE(l->opts() == 8);
        REQUIRE(l->startColumn() == 3);
        REQUIRE(l->endColumn() == 14);
        REQUIRE(l->openStyles().empty());
        REQUIRE(l->closeStyles().size() == 1);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("**"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 16);
        REQUIRE(t->endColumn() == 17);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }
}

/*
^__[link](/url)__^

*/
TEST_CASE("025")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("025"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);

    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT> *>(p->items().at(0).get());
    REQUIRE(l->opts() == (MD::BoldText | 8));
    REQUIRE(l->startColumn() == 3);
    REQUIRE(l->endColumn() == 14);
    REQUIRE(l->openStyles().size() == 2);
    REQUIRE(l->closeStyles().size() == 2);
}

/*
^text1 *text2* __text3^__ ~text4~

*/
TEST_CASE("026")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("026"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 5);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text1 "));
        REQUIRE(t->opts() == 8);
        REQUIRE(t->startColumn() == 1);
        REQUIRE(t->endColumn() == 6);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text2"));
        REQUIRE(t->opts() == (MD::ItalicText | 8));
        REQUIRE(t->startColumn() == 8);
        REQUIRE(t->endColumn() == 12);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString(" __text3"));
        REQUIRE(t->opts() == 8);
        REQUIRE(t->startColumn() == 14);
        REQUIRE(t->endColumn() == 21);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().size() == 1);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(3).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("__ "));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 23);
        REQUIRE(t->endColumn() == 25);
        REQUIRE(t->openStyles().empty());
        REQUIRE(t->closeStyles().empty());
    }

    {
        REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(4).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text4"));
        REQUIRE(t->opts() == MD::StrikethroughText);
        REQUIRE(t->startColumn() == 27);
        REQUIRE(t->endColumn() == 31);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
    }
}

/*
~*text ^text^* text

*/
TEST_CASE("027")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("027"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("~"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text "));
        REQUIRE(t->opts() == MD::ItalicText);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text"));
        REQUIRE(t->opts() == (MD::ItalicText | 8));
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(3).get());
        REQUIRE(t->text() == TRAIT::latin1ToString(" text"));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
    }
}

/*
~*text ^text^* text~

*/
TEST_CASE("028")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("028"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text "));
        REQUIRE(t->opts() == (MD::ItalicText | MD::StrikethroughText));
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text"));
        REQUIRE(t->opts() == (MD::ItalicText | 8 | MD::StrikethroughText));
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString(" text"));
        REQUIRE(t->opts() == MD::StrikethroughText);
    }
}

/*
^text^ text ^text^

*/
TEST_CASE("029")
{
    auto doc = loadTest<TRAIT>(TRAIT::latin1ToString("029"));
    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text"));
        REQUIRE(t->opts() == 8);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString(" text "));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text"));
        REQUIRE(t->opts() == 8);
    }
}

/*
text ^=text@text@=^

*/
TEST_CASE("030")
{
    MD::Parser<TRAIT> parser;
    parser.addTextPlugin(MD::TextPlugin::UserDefined,
                         MD::EmphasisPlugin::emphasisTemplatePlugin<TRAIT>,
                         true,
                         typename TRAIT::StringList() << TRAIT::latin1ToString("^") << TRAIT::latin1ToString("8"));
    parser.addTextPlugin(static_cast<MD::TextPlugin>(static_cast<int>(MD::TextPlugin::UserDefined) + 1),
                         MD::EmphasisPlugin::emphasisTemplatePlugin<TRAIT>,
                         true,
                         typename TRAIT::StringList() << TRAIT::latin1ToString("@") << TRAIT::latin1ToString("16"));
    parser.addTextPlugin(static_cast<MD::TextPlugin>(static_cast<int>(MD::TextPlugin::UserDefined) + 2),
                         MD::EmphasisPlugin::emphasisTemplatePlugin<TRAIT>,
                         true,
                         typename TRAIT::StringList() << TRAIT::latin1ToString("=") << TRAIT::latin1ToString("32"));
    const auto doc = parser.parse(TRAIT::latin1ToString("tests/plugins/emphasis/data/030.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 3);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text "));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text"));
        REQUIRE(t->opts() == (8 | 32));
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text"));
        REQUIRE(t->opts() == (8 | 16 | 32));
        REQUIRE(t->closeStyles().size() == 3);
    }
}

/*
-*text*- ^**text*^

*/
TEST_CASE("031")
{
    MD::Parser<TRAIT> parser;
    parser.addTextPlugin(MD::TextPlugin::UserDefined,
                         MD::EmphasisPlugin::emphasisTemplatePlugin<TRAIT>,
                         true,
                         typename TRAIT::StringList() << TRAIT::latin1ToString("^") << TRAIT::latin1ToString("8"));
    parser.addTextPlugin(static_cast<MD::TextPlugin>(static_cast<int>(MD::TextPlugin::UserDefined) + 1),
                         MD::EmphasisPlugin::emphasisTemplatePlugin<TRAIT>,
                         true,
                         typename TRAIT::StringList() << TRAIT::latin1ToString("-") << TRAIT::latin1ToString("16"));
    parser.addTextPlugin(static_cast<MD::TextPlugin>(static_cast<int>(MD::TextPlugin::UserDefined) + 2),
                         MD::EmphasisPlugin::emphasisTemplatePlugin<TRAIT>,
                         true,
                         typename TRAIT::StringList() << TRAIT::latin1ToString("=") << TRAIT::latin1ToString("32"));
    const auto doc = parser.parse(TRAIT::latin1ToString("tests/plugins/emphasis/data/031.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 4);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text"));
        REQUIRE(t->opts() == (16 | MD::ItalicText));
        REQUIRE(t->startColumn() == 2);
        REQUIRE(t->endColumn() == 5);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString(" "));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->startColumn() == 8);
        REQUIRE(t->endColumn() == 8);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("*"));
        REQUIRE(t->opts() == 8);
        REQUIRE(t->startColumn() == 10);
        REQUIRE(t->endColumn() == 10);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(3).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("text"));
        REQUIRE(t->opts() == (8 | MD::ItalicText));
        REQUIRE(t->closeStyles().size() == 2);
        REQUIRE(t->startColumn() == 12);
        REQUIRE(t->endColumn() == 15);
    }
}

/*
H ^2 ^3^^ -2 -3--

*/
TEST_CASE("032")
{
    MD::Parser<TRAIT> parser;
    parser.addTextPlugin(MD::TextPlugin::UserDefined,
                         MD::EmphasisPlugin::emphasisTemplatePlugin<TRAIT>,
                         true,
                         typename TRAIT::StringList() << TRAIT::latin1ToString("^") << TRAIT::latin1ToString("8"));
    parser.addTextPlugin(static_cast<MD::TextPlugin>(static_cast<int>(MD::TextPlugin::UserDefined) + 1),
                         MD::EmphasisPlugin::emphasisTemplatePlugin<TRAIT>,
                         true,
                         typename TRAIT::StringList() << TRAIT::latin1ToString("-") << TRAIT::latin1ToString("16"));
    parser.addTextPlugin(static_cast<MD::TextPlugin>(static_cast<int>(MD::TextPlugin::UserDefined) + 2),
                         MD::EmphasisPlugin::emphasisTemplatePlugin<TRAIT>,
                         true,
                         typename TRAIT::StringList() << TRAIT::latin1ToString("=") << TRAIT::latin1ToString("32"));
    const auto doc = parser.parse(TRAIT::latin1ToString("tests/plugins/emphasis/data/032.md"),
                                  false,
                                  {TRAIT::latin1ToString("md")},
                                  false);

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 6);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(0).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("H "));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->openStyles().size() == 0);
        REQUIRE(t->closeStyles().size() == 0);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(1).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("2 "));
        REQUIRE(t->opts() == 8);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 0);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(2).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("3"));
        REQUIRE(t->opts() == 8);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 2);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(3).get());
        REQUIRE(t->text() == TRAIT::latin1ToString(" "));
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->openStyles().size() == 0);
        REQUIRE(t->closeStyles().size() == 0);
    }

    {
        REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(4).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("2 "));
        REQUIRE(t->opts() == 16);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 0);
    }

    {
        REQUIRE(p->items().at(5)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(p->items().at(5).get());
        REQUIRE(t->text() == TRAIT::latin1ToString("3"));
        REQUIRE(t->opts() == 16);
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 2);
    }
}
