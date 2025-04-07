
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

/*
[\_\_VA\_OPT\_\_]()

*/
TEST_CASE("298")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/298.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT>*>(p->items().at(0).get());
    REQUIRE(l->p()->items().size() == 1);
    REQUIRE(l->p()->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text<TRAIT>*>(l->p()->items().at(0).get());
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == TRAIT::latin1ToString("__VA_OPT__"));
}

/*
- text
  - text
  ```
  code
  ```

*/
TEST_CASE("299")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/299.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem<TRAIT>*>(l->items().at(0).get());
        REQUIRE(li->items().size() == 3);

        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        REQUIRE(li->items().at(2)->type() == MD::ItemType::Code);
    }
}

/*
- text
  - text
  # h

*/
TEST_CASE("300")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/300.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem<TRAIT>*>(l->items().at(0).get());
        REQUIRE(li->items().size() == 3);

        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        REQUIRE(li->items().at(2)->type() == MD::ItemType::Heading);
    }
}

/*
- text
  - text
  > b

*/
TEST_CASE("301")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/301.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);

    {
        auto li = static_cast<MD::ListItem<TRAIT>*>(l->items().at(0).get());
        REQUIRE(li->items().size() == 3);

        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(li->items().at(1)->type() == MD::ItemType::List);
        REQUIRE(li->items().at(2)->type() == MD::ItemType::Blockquote);
    }
}

/*
<p> some HTML </p>
- list with some `inline code`

*/
TEST_CASE("302")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/302.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 29);
    REQUIRE(h->endLine() == 1);
}

/*
[![You sir are my hero. You've pretty much summed up and described my experiences of late, much better than I could have. Cursor and Windsurf both had me frustrated to the point where I was almost yelling at my computer screen. Out of whimsy, I thought to myself why not just ask Claude directly, and haven't looked back since.
Claude first to keep my sanity in check, then if necessary, engage with other IDEs, frameworks, etc. I thought I was the only one, glad to see I'm not lol.
33
1](testemonials/img_4.png)
https://medium.com/@pharmx/you-sir-are-my-hero-62cff5836a3e](https://medium.com/@pharmx/you-sir-are-my-hero-62cff5836a3e)

*/
TEST_CASE("303")
{
    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/303.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT>*>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link<TRAIT>*>(p->items().at(0).get());
    REQUIRE(l->startColumn() == 0);
    REQUIRE(l->startLine() == 0);
    REQUIRE(l->endColumn() == 120);
    REQUIRE(l->endLine() == 4);
    REQUIRE(l->p()->items().size() == 2);
    REQUIRE(l->p()->items().at(0)->type() == MD::ItemType::Image);
    REQUIRE(l->p()->items().at(1)->type() == MD::ItemType::Text);
}

/*
![](./data/a.png)

[](./data/304-1.md)

*/
TEST_CASE("304")
{
    typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
        QDir().absolutePath()
#else
        typename TRAIT::String(std::filesystem::canonical(std::filesystem::current_path()).u8string())
#endif
        + TRAIT::latin1ToString("/tests/parser/");

#ifndef MD4QT_QT_SUPPORT
    std::string tmp;
    wd.toUTF8String(tmp);
    std::replace(tmp.begin(), tmp.end(), '\\', '/');
    wd = icu::UnicodeString::fromUTF8(tmp);
#endif

    MD::Parser<TRAIT> parser;

    auto doc = parser.parse(TRAIT::latin1ToString("tests/parser/data/304.md"), wd);

    REQUIRE(doc->items().size() == 6);

    {
        REQUIRE(doc->items().at(0)->type() == MD::ItemType::Anchor);
        auto a = static_cast<MD::Anchor<TRAIT>*>(doc->items().at(0).get());
        REQUIRE(a->label() == wd + TRAIT::latin1ToString("data/304.md"));
    }

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT>*>(doc->items().at(1).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
        auto i = static_cast<MD::Image<TRAIT>*>(p->items().at(0).get());
        REQUIRE(i->url() == wd + TRAIT::latin1ToString("./data/a.png"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT>*>(doc->items().at(2).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link<TRAIT>*>(p->items().at(0).get());
        REQUIRE(l->url() == wd + TRAIT::latin1ToString("data/304-1.md"));
    }

    REQUIRE(doc->items().at(3)->type() == MD::ItemType::PageBreak);

    {
        REQUIRE(doc->items().at(4)->type() == MD::ItemType::Anchor);
        auto a = static_cast<MD::Anchor<TRAIT>*>(doc->items().at(4).get());
        REQUIRE(a->label() == wd + TRAIT::latin1ToString("data/304-1.md"));
    }

    {
        REQUIRE(doc->items().at(5)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph<TRAIT>*>(doc->items().at(5).get());
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Image);
        auto i = static_cast<MD::Image<TRAIT>*>(p->items().at(0).get());
        REQUIRE(i->url() == wd + TRAIT::latin1ToString("./data/b.png"));
    }
}
