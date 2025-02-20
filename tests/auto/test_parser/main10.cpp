
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

/*
 - 1
  - 2
   - 3
    - 4
     - 5
      - 6
       - 7
      - 6
     -
    - 4
   - 3
  - 2
 - 1

*/
TEST_CASE("281")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/281.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 3);
    REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem<TRAIT>*>(l->items().at(2).get());
        REQUIRE(li->items().size() == 2);

        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(li->items().at(1).get());
        REQUIRE(l->items().size() == 5);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(3).get());
        REQUIRE(l->items().size() == 3);
    }
}

/*
 - 1
  - 2
   - 3
    - 4
     - 5
      - 6
       - 7
      - 6
     -     code
    - 4
   - 3
  - 2
 - 1

*/
TEST_CASE("282")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/282.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 3);
    REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem<TRAIT>*>(l->items().at(2).get());
        REQUIRE(li->items().size() == 2);

        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(li->items().at(1).get());
        REQUIRE(l->items().size() == 5);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(3).get());
        REQUIRE(l->items().size() == 3);
    }
}

/*
 - 1
  - 2
   - 3
    - 4
     - 5
      - 6
       - 7
      - 6
     - ```
    - 4
   - 3
  - 2
 - 1

*/
TEST_CASE("283")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/283.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 3);
    REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem<TRAIT>*>(l->items().at(2).get());
        REQUIRE(li->items().size() == 2);

        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(li->items().at(1).get());
        REQUIRE(l->items().size() == 5);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(3).get());
        REQUIRE(l->items().size() == 3);
    }
}

/*
 - 1
  - 2
   - 3
    - 4
     - 5
      - 6
       - 7
      - 6
     - > ```
    - 4
   - 3
  - 2
 - 1

*/
TEST_CASE("284")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/284.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 3);
    REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem<TRAIT>*>(l->items().at(2).get());
        REQUIRE(li->items().size() == 2);

        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(li->items().at(1).get());
        REQUIRE(l->items().size() == 5);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(3).get());
        REQUIRE(l->items().size() == 3);
    }
}

/*
 - 1
  - 2
   - 3
    - 4
     - 5
      - 6
       - 7
      - 6
     -     code

           code
    - 4
   - 3
  - 2
 - 1

*/
TEST_CASE("285")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/285.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 3);
    REQUIRE(l->items().at(2)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem<TRAIT>*>(l->items().at(2).get());
        REQUIRE(li->items().size() == 2);

        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(li->items().at(1).get());
        REQUIRE(l->items().size() == 5);
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code<TRAIT>*>(doc->items().at(2).get());
    REQUIRE(c->text() == TRAIT::latin1ToString("- 4"));
    REQUIRE(c->startColumn() == 4);
    REQUIRE(c->startLine() == 11);
    REQUIRE(c->endColumn() == 6);
    REQUIRE(c->endLine() == 11);

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(3).get());
        REQUIRE(l->items().size() == 3);
    }
}

/*

* <!--
-->

In CommonMark will be.

*/
TEST_CASE("286")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/286.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
}

/*
* <div>

text

*/
TEST_CASE("287")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/287.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
}

/*
* <?
?>

text

*/
TEST_CASE("288")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/288.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
}

/*
<!--
```
``` -->

# Head

*/
TEST_CASE("289")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/289.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
}

/*
<pre>

```
```

</pre>

# Heading

*/
TEST_CASE("290")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/290.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
}

/*
<?

```
```

?>

# Heading

*/
TEST_CASE("291")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/291.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
}

/*
<!a

```
```

>

# Heading

*/
TEST_CASE("292")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/292.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
}

/*
<![CDATA[

```
```

]]>

# Heading

*/
TEST_CASE("293")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/293.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
}

/*
<div>
```
```

# Heading

*/
TEST_CASE("294")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/294.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
}

/*
<a>
```
```

# Heading

*/
TEST_CASE("295")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/295.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
}

/*
> <div>
> ```

# Heading

*/
TEST_CASE("296")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/296.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);
    auto b = static_cast<MD::Blockquote<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(b->items().size() == 1);
    REQUIRE(b->items().at(0)->type() == MD::ItemType::RawHtml);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
}

/*
> <div>
```
code
```
# Heading

*/
TEST_CASE("297")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/297.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);
    auto b = static_cast<MD::Blockquote<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(b->items().size() == 1);
    REQUIRE(b->items().at(0)->type() == MD::ItemType::RawHtml);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);
    REQUIRE(doc->items().at(3)->type() == MD::ItemType::Heading);
}
