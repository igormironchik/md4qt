
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#include <md4qt/parser.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest/doctest.h>

TEST_CASE("replace_remove_1")
{
    TRAIT::InternalString s(TRAIT::latin1ToString("abcde"));

    REQUIRE(s.virginPos(-1) == -1);
    REQUIRE(s.virginPos(0) == 0);
    REQUIRE(s.virginPos(1) == 1);
    REQUIRE(s.virginPos(2) == 2);
    REQUIRE(s.virginPos(3) == 3);
    REQUIRE(s.virginPos(4) == 4);
    REQUIRE(s.virginPos(5) == 5);

    s.replace(TRAIT::latin1ToString("b"), TRAIT::latin1ToString("bb"));

    REQUIRE(s.asString() == TRAIT::latin1ToString("abbcde"));

    REQUIRE(s.virginPos(0) == 0);
    REQUIRE(s.virginPos(1) == 1);
    REQUIRE(s.virginPos(2) == 1);
    REQUIRE(s.virginPos(3) == 2);
    REQUIRE(s.virginPos(4) == 3);
    REQUIRE(s.virginPos(5) == 4);
    REQUIRE(s.virginPos(6) == 5);

    s.replace(TRAIT::latin1ToString("bb"), TRAIT::latin1ToString("b"));

    REQUIRE(s.asString() == TRAIT::latin1ToString("abcde"));

    REQUIRE(s.virginPos(0) == 0);
    REQUIRE(s.virginPos(1) == 1);
    REQUIRE(s.virginPos(2) == 2);
    REQUIRE(s.virginPos(3) == 3);
    REQUIRE(s.virginPos(4) == 4);

    s.replace(TRAIT::latin1ToString("b"), TRAIT::latin1ToString(""));

    REQUIRE(s.asString() == TRAIT::latin1ToString("acde"));

    REQUIRE(s.virginPos(0) == 0);
    REQUIRE(s.virginPos(1) == 2);
    REQUIRE(s.virginPos(2) == 3);
    REQUIRE(s.virginPos(3) == 4);

    s.remove(0, 1);

    REQUIRE(s.asString() == TRAIT::latin1ToString("cde"));

    REQUIRE(s.virginPos(0) == 2);
    REQUIRE(s.virginPos(1) == 3);
    REQUIRE(s.virginPos(2) == 4);
}

TEST_CASE("replace_remove_2")
{
    TRAIT::InternalString s(TRAIT::latin1ToString("xxxxxx"));

    REQUIRE(s.virginPos(1) == 1);

    s.replace(TRAIT::latin1ToString("xx"), TRAIT::latin1ToString("x"));

    REQUIRE(s.asString() == TRAIT::latin1ToString("xxx"));

    REQUIRE(s.virginPos(0) == 0);
    REQUIRE(s.virginPos(1) == 2);
    REQUIRE(s.virginPos(2) == 4);

    s.remove(1, 1);

    REQUIRE(s.asString() == TRAIT::latin1ToString("xx"));

    REQUIRE(s.virginPos(0) == 0);
    REQUIRE(s.virginPos(1) == 4);

    s.remove(0, 1);

    REQUIRE(s.asString() == TRAIT::latin1ToString("x"));

    REQUIRE(s.virginPos(0) == 4);
}

TEST_CASE("replace_remove_3")
{
    TRAIT::InternalString s(TRAIT::latin1ToString("xxxxxx"));

    REQUIRE(s.virginPos(1) == 1);

    s.remove(0, 3);

    REQUIRE(s.asString() == TRAIT::latin1ToString("xxx"));

    REQUIRE(s.virginPos(0) == 3);
    REQUIRE(s.virginPos(1) == 4);
    REQUIRE(s.virginPos(2) == 5);

    s.replace(TRAIT::latin1ToString("xx"), TRAIT::latin1ToString("x"));

    REQUIRE(s.asString() == TRAIT::latin1ToString("xx"));

    REQUIRE(s.virginPos(0) == 3);
    REQUIRE(s.virginPos(1) == 5);

    s.remove(0, 1);

    REQUIRE(s.asString() == TRAIT::latin1ToString("x"));

    REQUIRE(s.virginPos(0) == 5);
}

TEST_CASE("replace_remove_4")
{
    TRAIT::InternalString s(TRAIT::latin1ToString("xxxxxx"));

    REQUIRE(s.virginPos(1) == 1);

    s.replace(TRAIT::latin1ToString("xxx"), TRAIT::latin1ToString("bbbb"));

    REQUIRE(s.asString() == TRAIT::latin1ToString("bbbbbbbb"));

    REQUIRE(s.virginPos(0) == 0);
    REQUIRE(s.virginPos(1) == 1);
    REQUIRE(s.virginPos(2) == 2);
    REQUIRE(s.virginPos(3) == 2);

    REQUIRE(s.virginPos(4) == 3);
    REQUIRE(s.virginPos(5) == 4);
    REQUIRE(s.virginPos(6) == 5);
    REQUIRE(s.virginPos(7) == 5);
}

TEST_CASE("replace_remove_5")
{
    TRAIT::InternalString s(TRAIT::latin1ToString("xxxxxx"));

    REQUIRE(s.virginPos(1) == 1);

    s.replace(TRAIT::latin1ToString("xxx"), TRAIT::latin1ToString("bbb"));

    REQUIRE(s.asString() == TRAIT::latin1ToString("bbbbbb"));

    REQUIRE(s.virginPos(0) == 0);
    REQUIRE(s.virginPos(1) == 1);
    REQUIRE(s.virginPos(2) == 2);
    REQUIRE(s.virginPos(3) == 3);
    REQUIRE(s.virginPos(4) == 4);
    REQUIRE(s.virginPos(5) == 5);
}

TEST_CASE("replace_remove_6")
{
    TRAIT::InternalString s(TRAIT::latin1ToString("\tParagraph\t"));

    REQUIRE(s.virginPos(1) == 1);

    s.replace(TRAIT::latin1ToString("\t"), TRAIT::latin1ToString("    "));

    REQUIRE(s.asString() == TRAIT::latin1ToString("    Paragraph    "));

    REQUIRE(s.virginPos(0) == 0);
    REQUIRE(s.virginPos(1) == 0);
    REQUIRE(s.virginPos(2) == 0);
    REQUIRE(s.virginPos(3) == 0);
    REQUIRE(s.virginPos(4) == 1);
    REQUIRE(s.virginPos(5) == 2);
    REQUIRE(s.virginPos(13) == 10);
    REQUIRE(s.virginPos(14) == 10);
    REQUIRE(s.virginPos(15) == 10);
    REQUIRE(s.virginPos(16) == 10);
    REQUIRE(s.virginPos(17) == 11);
    REQUIRE(s.virginPos(18) == 11);

    s.remove(0, 4);

    REQUIRE(s.asString() == TRAIT::latin1ToString("Paragraph    "));

    REQUIRE(s.virginPos(0) == 1);
    REQUIRE(s.virginPos(1) == 2);
    REQUIRE(s.virginPos(2) == 3);
    REQUIRE(s.virginPos(9) == 10);
    REQUIRE(s.virginPos(10) == 10);
    REQUIRE(s.virginPos(11) == 10);
    REQUIRE(s.virginPos(12) == 10);
    REQUIRE(s.virginPos(13) == 11);
    REQUIRE(s.virginPos(14) == 11);
}

TEST_CASE("simplified")
{
    TRAIT::InternalString s(TRAIT::latin1ToString("   a   b   c   "));

    s = s.simplified();

    REQUIRE(s.asString() == TRAIT::latin1ToString("a b c"));

    REQUIRE(s.virginPos(0) == 3);
    REQUIRE(s.virginPos(1) == 4);
    REQUIRE(s.virginPos(2) == 7);
    REQUIRE(s.virginPos(3) == 8);
    REQUIRE(s.virginPos(4) == 11);

    s = TRAIT::InternalString(TRAIT::latin1ToString("   a b c   "));

    s = s.simplified();

    REQUIRE(s.asString() == TRAIT::latin1ToString("a b c"));

    REQUIRE(s.virginPos(0) == 3);
    REQUIRE(s.virginPos(1) == 4);
    REQUIRE(s.virginPos(2) == 5);
    REQUIRE(s.virginPos(3) == 6);
    REQUIRE(s.virginPos(4) == 7);

    s = TRAIT::InternalString(TRAIT::latin1ToString("a b c"));

    s = s.simplified();

    REQUIRE(s.asString() == TRAIT::latin1ToString("a b c"));

    REQUIRE(s.virginPos(0) == 0);
    REQUIRE(s.virginPos(1) == 1);
    REQUIRE(s.virginPos(2) == 2);
    REQUIRE(s.virginPos(3) == 3);
    REQUIRE(s.virginPos(4) == 4);

    s = TRAIT::InternalString(TRAIT::latin1ToString("a b  c"));

    s = s.simplified();

    REQUIRE(s.asString() == TRAIT::latin1ToString("a b c"));

    REQUIRE(s.virginPos(0) == 0);
    REQUIRE(s.virginPos(1) == 1);
    REQUIRE(s.virginPos(2) == 2);
    REQUIRE(s.virginPos(3) == 3);
    REQUIRE(s.virginPos(4) == 5);

    s = TRAIT::InternalString(TRAIT::latin1ToString(""));

    s = s.simplified();

    REQUIRE(s.asString() == TRAIT::latin1ToString(""));
    REQUIRE(s.isEmpty());

    s = TRAIT::InternalString(TRAIT::latin1ToString("   "));

    s = s.simplified();

    REQUIRE(s.asString() == TRAIT::latin1ToString(""));
    REQUIRE(s.isEmpty());
}

TEST_CASE("split")
{
    TRAIT::InternalString s(TRAIT::latin1ToString("|a|b|c|"));

    auto r = s.split(TRAIT::InternalString(TRAIT::latin1ToString("|")));

    REQUIRE(r.size() == 3);

    REQUIRE(r.at(0).asString() == TRAIT::latin1ToString("a"));
    REQUIRE(r.at(0).virginPos(0) == 1);

    REQUIRE(r.at(1).asString() == TRAIT::latin1ToString("b"));
    REQUIRE(r.at(1).virginPos(0) == 3);

    REQUIRE(r.at(2).asString() == TRAIT::latin1ToString("c"));
    REQUIRE(r.at(2).virginPos(0) == 5);

    s = TRAIT::InternalString(TRAIT::latin1ToString(" | a | b | c | "));

    r = s.split(TRAIT::InternalString(TRAIT::latin1ToString("|")));

    REQUIRE(r.size() == 5);

    REQUIRE(r.at(0).asString() == TRAIT::latin1ToString(" "));
    REQUIRE(r.at(0).virginPos(0) == 0);

    REQUIRE(r.at(1).asString() == TRAIT::latin1ToString(" a "));
    REQUIRE(r.at(1).virginPos(1) == 3);

    REQUIRE(r.at(2).asString() == TRAIT::latin1ToString(" b "));
    REQUIRE(r.at(2).virginPos(1) == 7);

    REQUIRE(r.at(3).asString() == TRAIT::latin1ToString(" c "));
    REQUIRE(r.at(3).virginPos(1) == 11);

    REQUIRE(r.at(4).asString() == TRAIT::latin1ToString(" "));
    REQUIRE(r.at(4).virginPos(0) == 14);

    s = TRAIT::InternalString(TRAIT::latin1ToString("abc"));

    r = s.split(TRAIT::InternalString(TRAIT::latin1ToString("")));

    REQUIRE(r.size() == 3);

    REQUIRE(r.at(0).asString() == TRAIT::latin1ToString("a"));
    REQUIRE(r.at(0).virginPos(0) == 0);

    REQUIRE(r.at(1).asString() == TRAIT::latin1ToString("b"));
    REQUIRE(r.at(1).virginPos(0) == 1);

    REQUIRE(r.at(2).asString() == TRAIT::latin1ToString("c"));
    REQUIRE(r.at(2).virginPos(0) == 2);

    s = TRAIT::InternalString(TRAIT::latin1ToString(" | a | b | c | "));
    s = s.simplified();

    r = s.split(TRAIT::InternalString(TRAIT::latin1ToString("|")));

    REQUIRE(r.size() == 3);

    REQUIRE(r.at(0).asString() == TRAIT::latin1ToString(" a "));
    REQUIRE(r.at(0).virginPos(1) == 3);

    REQUIRE(r.at(1).asString() == TRAIT::latin1ToString(" b "));
    REQUIRE(r.at(1).virginPos(1) == 7);

    REQUIRE(r.at(2).asString() == TRAIT::latin1ToString(" c "));
    REQUIRE(r.at(2).virginPos(1) == 11);
}

TEST_CASE("sliced")
{
    TRAIT::InternalString s(TRAIT::latin1ToString("aaabbbccc"));

    s = s.sliced(3, 3);

    REQUIRE(s.asString() == TRAIT::latin1ToString("bbb"));
    REQUIRE(s.virginPos(0) == 3);
    REQUIRE(s.virginPos(1) == 4);
    REQUIRE(s.virginPos(2) == 5);

    s = TRAIT::InternalString(TRAIT::latin1ToString("aaabbbccc"));

    s = s.sliced(3);

    REQUIRE(s.asString() == TRAIT::latin1ToString("bbbccc"));
    REQUIRE(s.virginPos(0) == 3);
    REQUIRE(s.virginPos(1) == 4);
    REQUIRE(s.virginPos(2) == 5);
    REQUIRE(s.virginPos(3) == 6);
    REQUIRE(s.virginPos(4) == 7);
    REQUIRE(s.virginPos(5) == 8);
}

TEST_CASE("right")
{
    TRAIT::InternalString s(TRAIT::latin1ToString("aaabbbccc"));

    s = s.right(3);

    REQUIRE(s.asString() == TRAIT::latin1ToString("ccc"));
    REQUIRE(s.virginPos(0) == 6);
    REQUIRE(s.virginPos(1) == 7);
    REQUIRE(s.virginPos(2) == 8);
}

TEST_CASE("insert")
{
    TRAIT::InternalString s(TRAIT::latin1ToString("a"));

    s = s.insert(0, TRAIT::latin1ToChar('b'));

    REQUIRE(s.asString() == TRAIT::latin1ToString("ba"));
    REQUIRE(s.virginPos(0) == 0);
    REQUIRE(s.virginPos(1) == 0);
}

TEST_CASE("double_remove")
{
    TRAIT::InternalString s(TRAIT::latin1ToString("a b c d"));

    s.remove(2, 1);

    REQUIRE(s.asString() == TRAIT::latin1ToString("a  c d"));

    s = s.simplified();

    REQUIRE(s.asString() == TRAIT::latin1ToString("a c d"));

    s.remove(2, 1);

    REQUIRE(s.asString() == TRAIT::latin1ToString("a  d"));

    s = s.simplified();

    REQUIRE(s.asString() == TRAIT::latin1ToString("a d"));

    REQUIRE(s.virginPos(0) == 0);
    REQUIRE(s.virginPos(1) == 1);
    REQUIRE(s.virginPos(2) == 6);
}

TEST_CASE("replace_tabs")
{
    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\tcode\t"));

        MD::replaceTabs<TRAIT>(s);

        REQUIRE(s.virginPos(0) == 0);
        REQUIRE(s.virginPos(1) == 0);
        REQUIRE(s.virginPos(2) == 0);
        REQUIRE(s.virginPos(3) == 0);
        REQUIRE(s.virginPos(4) == 1);
        REQUIRE(s.virginPos(5) == 2);
        REQUIRE(s.virginPos(6) == 3);
        REQUIRE(s.virginPos(7) == 4);
        REQUIRE(s.virginPos(8) == 5);
        REQUIRE(s.virginPos(9) == 5);
        REQUIRE(s.virginPos(10) == 5);
        REQUIRE(s.virginPos(11) == 5);
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\tcode\t"));

        MD::replaceTabs<TRAIT>(s);

        s.remove(0, 2);

        REQUIRE(s.virginPos(0) == 0);
        REQUIRE(s.virginPos(1) == 0);
        REQUIRE(s.virginPos(2) == 1);
        REQUIRE(s.virginPos(3) == 2);
        REQUIRE(s.virginPos(4) == 3);
        REQUIRE(s.virginPos(5) == 4);
        REQUIRE(s.virginPos(6) == 5);
        REQUIRE(s.virginPos(7) == 5);
        REQUIRE(s.virginPos(8) == 5);
        REQUIRE(s.virginPos(9) == 5);
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\tcode\t"));

        MD::replaceTabs<TRAIT>(s);

        s.remove(0, 2);
        s.remove(8, 2);

        REQUIRE(s.virginPos(0) == 0);
        REQUIRE(s.virginPos(1) == 0);
        REQUIRE(s.virginPos(2) == 1);
        REQUIRE(s.virginPos(3) == 2);
        REQUIRE(s.virginPos(4) == 3);
        REQUIRE(s.virginPos(5) == 4);
        REQUIRE(s.virginPos(6) == 5);
        REQUIRE(s.virginPos(7) == 5);
    }
}

TEST_CASE("replace_spaces")
{
    TRAIT::InternalString s(TRAIT::latin1ToString("    code    c"));

    s.remove(0, 4);
    s.remove(4, 4);

    REQUIRE(s.virginPos(0) == 4);
    REQUIRE(s.virginPos(1) == 5);
    REQUIRE(s.virginPos(2) == 6);
    REQUIRE(s.virginPos(3) == 7);
    REQUIRE(s.virginPos(4) == 12);
}

TEST_CASE("virgin_string")
{
    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\tcode"));

        MD::replaceTabs<TRAIT>(s);

        REQUIRE(s.virginSubString() == TRAIT::latin1ToString("\tcode"));
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\tcode"));

        MD::replaceTabs<TRAIT>(s);

        s.remove(0, 2);

        REQUIRE(s.virginSubString() == TRAIT::latin1ToString("  code"));
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\t\tcode"));

        MD::replaceTabs<TRAIT>(s);

        s.remove(0, 2);

        REQUIRE(s.virginSubString() == TRAIT::latin1ToString("  \tcode"));
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\tcode\t"));

        MD::replaceTabs<TRAIT>(s);

        REQUIRE(s.virginSubString() == TRAIT::latin1ToString("\tcode\t"));
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\tcode\t"));

        MD::replaceTabs<TRAIT>(s);

        s.remove(0, 2);

        REQUIRE(s.virginSubString() == TRAIT::latin1ToString("  code\t"));
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\t\tcode\t\t"));

        MD::replaceTabs<TRAIT>(s);

        s.remove(0, 2);
        s.remove(s.length() - 2, 2);

        REQUIRE(s.virginSubString() == TRAIT::latin1ToString("  \tcode\t  "));
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\t\tcode\t\t"));

        s.remove(3, 2);

        MD::replaceTabs<TRAIT>(s);

        REQUIRE(s.virginSubString() == TRAIT::latin1ToString("\t\tcode\t\t"));
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\t\tcode\t\t"));

        MD::replaceTabs<TRAIT>(s);

        s.remove(0, s.length());

        REQUIRE(s.virginSubString() == TRAIT::latin1ToString("\t\tcode\t\t"));
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\t\tcode\t\t"));

        MD::replaceTabs<TRAIT>(s);

        s.remove(0, s.length());

        REQUIRE(s.virginSubString(-1, s.length() + 1) == TRAIT::latin1ToString("\t\tcode\t\t"));
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("text"));

        s.remove(0, 2);
        s.remove(s.length() - 1, 1);

        REQUIRE(s.virginSubString() == TRAIT::latin1ToString("x"));
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("text"));

        REQUIRE(s.virginSubString(1, 1) == TRAIT::latin1ToString("e"));
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\t\tcode\t\t"));

        MD::replaceTabs<TRAIT>(s);

        REQUIRE(s.virginSubString(0, 12) == TRAIT::latin1ToString("\t\tcode"));
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\ta\t\tb"));

        MD::replaceTabs<TRAIT>(s);

        REQUIRE(s.virginSubString(4) == TRAIT::latin1ToString("a\t\tb"));
    }
}

TEST_CASE("backslash")
{
    {
        TRAIT::InternalString s(TRAIT::latin1ToString("\\|"));

        s = MD::removeBackslashes<typename TRAIT::InternalString, TRAIT>(s);

        REQUIRE(s.virginPos(0) == 1);
        REQUIRE(s.virginSubString() == TRAIT::latin1ToString("|"));
    }

    {
        TRAIT::InternalString s(TRAIT::latin1ToString("abcde\\|"));

        s.replace(TRAIT::latin1ToString("\\|"), TRAIT::latin1ToString("|"));

        REQUIRE(s.virginPos(5) == 5);
        REQUIRE(s.virginSubString(5) == TRAIT::latin1ToString("\\|"));
    }
}
