/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include "parser.h"

#include "test_utils.h"

inline QString to_string(int i)
{
    return QString::number(i);
}

/*
  - list

    ```javascript

    // very bad
    function
    *
    foo() {
      // ...
    }

    // very bad
    const wat = function
    *
    () {
      // ...
    };
    ```


*/
TEST_CASE("153")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/153.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    CHECK_POSITIONS(l, 2, 0, 0, 18);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto i = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(i->items().size() == 2);
    CHECK_POSITIONS(i, 2, 0, 0, 18);
    REQUIRE(i->delim() == MD::WithPosition{2, 0, 2, 0});

    REQUIRE(i->items().at(0)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(i->items().at(0).get());
    CHECK_POSITIONS(p, 4, 0, 7, 0);

    REQUIRE(i->items().at(1)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(i->items().at(1).get());
    CHECK_POSITIONS(c, 0, 3, 5, 16);
    REQUIRE(c->startDelim() == MD::WithPosition{4, 2, 6, 2});
    REQUIRE(c->endDelim() == MD::WithPosition{4, 17, 6, 17});
    REQUIRE(c->syntaxPos() == MD::WithPosition{7, 2, 16, 2});
    REQUIRE(c->text()
            == QStringLiteral("\n// very bad\nfunction\n*\nfoo() {\n  // ...\n}\n\n"
                              "// very bad\nconst wat = function\n*\n() {\n  // ...\n};"));
}

/*
  - list

    ```javascript
    |**
     * make() returns a new element
     * based on the passed-in tag name
     -/
    function make(tag) {

      // ...

      return element;
    }
    ```

*/
TEST_CASE("154")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/154.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    CHECK_POSITIONS(l, 2, 0, 0, 14);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto i = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(i->items().size() == 2);
    CHECK_POSITIONS(i, 2, 0, 0, 14);
    REQUIRE(i->delim() == MD::WithPosition{2, 0, 2, 0});

    REQUIRE(i->items().at(0)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(i->items().at(0).get());
    CHECK_POSITIONS(p, 4, 0, 7, 0);

    REQUIRE(i->items().at(1)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(i->items().at(1).get());
    CHECK_POSITIONS(c, 4, 3, 4, 12);
    REQUIRE(c->startDelim() == MD::WithPosition{4, 2, 6, 2});
    REQUIRE(c->endDelim() == MD::WithPosition{4, 13, 6, 13});
    REQUIRE(c->syntaxPos() == MD::WithPosition{7, 2, 16, 2});
    REQUIRE(c->text()
            == QStringLiteral("/**\n * make() returns a new element\n * based on the passed-in "
                              "tag name\n */\nfunction make(tag) {\n\n  // ...\n\n  return element;\n}"));
}

/*
* list

  ```
  int i;
  ```

* list

  ```
  int i;

```
int i;
```

*/
TEST_CASE("155")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/155.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 2);
    CHECK_POSITIONS(l, 0, 0, 0, 10);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto i = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(i->items().size() == 2);
        CHECK_POSITIONS(i, 0, 0, 0, 5);
        REQUIRE(i->delim() == MD::WithPosition{0, 0, 0, 0});

        REQUIRE(i->items().at(0)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(i->items().at(0).get());
        CHECK_POSITIONS(p, 2, 0, 5, 0);

        REQUIRE(i->items().at(1)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code *>(i->items().at(1).get());
        CHECK_POSITIONS(c, 2, 3, 7, 3);
        REQUIRE(c->text() == QStringLiteral("int i;"));
        REQUIRE(c->startDelim() == MD::WithPosition{2, 2, 4, 2});
        REQUIRE(c->endDelim() == MD::WithPosition{2, 4, 4, 4});
        REQUIRE(c->syntaxPos() == MD::WithPosition{-1, -1, -1, -1});
    }

    {
        REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);
        auto i = static_cast<MD::ListItem *>(l->items().at(1).get());
        REQUIRE(i->items().size() == 2);
        CHECK_POSITIONS(i, 0, 6, 0, 10);
        REQUIRE(i->delim() == MD::WithPosition{0, 6, 0, 6});

        REQUIRE(i->items().at(0)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(i->items().at(0).get());
        CHECK_POSITIONS(p, 2, 6, 5, 6);

        REQUIRE(i->items().at(1)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code *>(i->items().at(1).get());
        CHECK_POSITIONS(c, 2, 9, 0, 10);
        REQUIRE(c->startDelim() == MD::WithPosition{2, 8, 4, 8});
        REQUIRE(c->endDelim() == MD::WithPosition{-1, -1, -1, -1});
        REQUIRE(c->syntaxPos() == MD::WithPosition{-1, -1, -1, -1});
        REQUIRE(c->text() == QStringLiteral("int i;\n"));
    }

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(doc->items().at(2).get());
    CHECK_POSITIONS(c, 0, 12, 5, 12);
    REQUIRE(c->text() == QStringLiteral("int i;"));
    REQUIRE(c->startDelim() == MD::WithPosition{0, 11, 2, 11});
    REQUIRE(c->endDelim() == MD::WithPosition{0, 13, 2, 13});
    REQUIRE(c->syntaxPos() == MD::WithPosition{-1, -1, -1, -1});
}

/*
<a id='column-width'></a>(<a href='#column-width'>link</a>)

*/
TEST_CASE("156")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/156.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 58, 0);
    REQUIRE(p->items().size() == 7);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::RawHtml);
    REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
    REQUIRE(p->items().at(2)->type() == MD::ItemType::Text);
    auto t1 = static_cast<MD::Text *>(p->items().at(2).get());
    REQUIRE(t1->text() == QStringLiteral("("));
    REQUIRE(p->items().at(3)->type() == MD::ItemType::RawHtml);
    REQUIRE(p->items().at(4)->type() == MD::ItemType::Text);
    auto t2 = static_cast<MD::Text *>(p->items().at(4).get());
    REQUIRE(t2->text() == QStringLiteral("link"));
    REQUIRE(p->items().at(5)->type() == MD::ItemType::RawHtml);
    REQUIRE(p->items().at(6)->type() == MD::ItemType::Text);
    auto t3 = static_cast<MD::Text *>(p->items().at(6).get());
    REQUIRE(t3->text() == QStringLiteral(")"));
}

/*
<br><pre>
<b>To get started, [view our webpage](https://cockatrice.github.io/)</b><br>
</pre><br>

*/
TEST_CASE("157")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/157.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 9);
    CHECK_POSITIONS(p, 0, 0, 9, 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(0).get());
        REQUIRE(h->text() == QStringLiteral("<br>"));
        CHECK_POSITIONS(h, 0, 0, 3, 0);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(1).get());
        REQUIRE(h->text() == QStringLiteral("<pre>"));
        CHECK_POSITIONS(h, 4, 0, 8, 0);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(2).get());
        REQUIRE(h->text() == QStringLiteral("<b>"));
        CHECK_POSITIONS(h, 0, 1, 2, 1);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(3).get());
        REQUIRE(t->text() == QStringLiteral("To get started, "));
        CHECK_POSITIONS(t, 3, 1, 18, 1);
    }

    {
        REQUIRE(p->items().at(4)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(4).get());
        REQUIRE(l->text() == QStringLiteral("view our webpage"));
        REQUIRE(l->url() == QStringLiteral("https://cockatrice.github.io/"));
        CHECK_POSITIONS(l, 19, 1, 67, 1);
        REQUIRE(l->textPos() == MD::WithPosition{20, 1, 35, 1});
        REQUIRE(l->urlPos() == MD::WithPosition{38, 1, 66, 1});
    }

    {
        REQUIRE(p->items().at(5)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(5).get());
        REQUIRE(h->text() == QStringLiteral("</b>"));
        CHECK_POSITIONS(h, 68, 1, 71, 1);
    }

    {
        REQUIRE(p->items().at(6)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(6).get());
        REQUIRE(h->text() == QStringLiteral("<br>"));
        CHECK_POSITIONS(h, 72, 1, 75, 1);
    }

    {
        REQUIRE(p->items().at(7)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(7).get());
        REQUIRE(h->text() == QStringLiteral("</pre>"));
        CHECK_POSITIONS(h, 0, 2, 5, 2);
    }

    {
        REQUIRE(p->items().at(8)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(8).get());
        REQUIRE(h->text() == QStringLiteral("<br>"));
        CHECK_POSITIONS(h, 6, 2, 9, 2);
    }
}

/*
<br><pre
<b>To get started, [view our webpage](https://cockatrice.github.io/)</b><br>
</pre><br>

*/
TEST_CASE("158")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/158.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 9);
    CHECK_POSITIONS(p, 0, 0, 9, 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(0).get());
        REQUIRE(h->text() == QStringLiteral("<br>"));
        CHECK_POSITIONS(h, 0, 0, 3, 0);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->text() == QStringLiteral("<pre"));
        CHECK_POSITIONS(t, 4, 0, 7, 0);
    }

    {
        REQUIRE(p->items().at(2)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(2).get());
        REQUIRE(h->text() == QStringLiteral("<b>"));
        CHECK_POSITIONS(h, 0, 1, 2, 1);
    }

    {
        REQUIRE(p->items().at(3)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(3).get());
        REQUIRE(t->text() == QStringLiteral("To get started, "));
        CHECK_POSITIONS(t, 3, 1, 18, 1);
    }

    {
        REQUIRE(p->items().at(4)->type() == MD::ItemType::Link);
        auto l = static_cast<MD::Link *>(p->items().at(4).get());
        REQUIRE(l->text() == QStringLiteral("view our webpage"));
        REQUIRE(l->url() == QStringLiteral("https://cockatrice.github.io/"));
        CHECK_POSITIONS(l, 19, 1, 67, 1);
        REQUIRE(l->textPos() == MD::WithPosition{20, 1, 35, 1});
        REQUIRE(l->urlPos() == MD::WithPosition{38, 1, 66, 1});
    }

    {
        REQUIRE(p->items().at(5)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(5).get());
        REQUIRE(h->text() == QStringLiteral("</b>"));
        CHECK_POSITIONS(h, 68, 1, 71, 1);
    }

    {
        REQUIRE(p->items().at(6)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(6).get());
        REQUIRE(h->text() == QStringLiteral("<br>"));
        CHECK_POSITIONS(h, 72, 1, 75, 1);
    }

    {
        REQUIRE(p->items().at(7)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(7).get());
        REQUIRE(h->text() == QStringLiteral("</pre>"));
        CHECK_POSITIONS(h, 0, 2, 5, 2);
    }

    {
        REQUIRE(p->items().at(8)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(p->items().at(8).get());
        REQUIRE(h->text() == QStringLiteral("<br>"));
        CHECK_POSITIONS(h, 6, 2, 9, 2);
    }
}

/*
<pre
</pre>

*/
TEST_CASE("159")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/159.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text() == QStringLiteral("<pre\n</pre>"));
    CHECK_POSITIONS(h, 0, 0, 5, 1);
}

/*
[^pdfium_docs]: Unfortunately, no recent HTML-rendered docs are available for PDFium at the moment.

<!-- TODO write something about weakref.finalize(); add example on creating a C page array -->

*/
TEST_CASE("160")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/160.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    REQUIRE(h->text()
            == QStringLiteral("<!-- TODO write something about weakref.finalize(); "
                              "add example on creating a C page array -->"));
    CHECK_POSITIONS(h, 0, 2, 93, 2);
}

/*
  + To use set up channels in a GH workflow
    ```yaml
    - name: ...
      uses: conda-incubator/setup-miniconda@v2
      with:
        # ... your options
        channels: bblanchon,pypdfium2-team
        channel-priority: strict
    ```

*/
TEST_CASE("161")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/161.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    CHECK_POSITIONS(l, 2, 0, 0, 9);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto i = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(i->items().size() == 2);
        CHECK_POSITIONS(i, 2, 0, 0, 9);
        REQUIRE(i->delim() == MD::WithPosition{2, 0, 2, 0});

        REQUIRE(i->items().at(0)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(i->items().at(0).get());
        CHECK_POSITIONS(p, 4, 0, 42, 0);

        REQUIRE(i->items().at(1)->type() == MD::ItemType::Code);
        auto c = static_cast<MD::Code *>(i->items().at(1).get());
        CHECK_POSITIONS(c, 4, 2, 31, 7);
        REQUIRE(c->startDelim() == MD::WithPosition{4, 1, 6, 1});
        REQUIRE(c->endDelim() == MD::WithPosition{4, 8, 6, 8});
        REQUIRE(c->syntaxPos() == MD::WithPosition{7, 1, 10, 1});
        REQUIRE(c->text()
                == QStringLiteral("- name: ...\n"
                                  "  uses: conda-incubator/setup-miniconda@v2\n"
                                  "  with:\n"
                                  "    # ... your options\n"
                                  "    channels: bblanchon,pypdfium2-team\n"
                                  "    channel-priority: strict"));
    }
}

/*
<!--
- -->

- list

*/
TEST_CASE("162")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/162.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    CHECK_POSITIONS(h, 0, 0, 4, 1);
    REQUIRE(h->text() == QStringLiteral("<!--\n- -->"));

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(2).get());
    REQUIRE(l->items().size() == 1);
    CHECK_POSITIONS(l, 0, 3, 0, 4);
    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto i = static_cast<MD::ListItem *>(l->items().at(0).get());
    CHECK_POSITIONS(i, 0, 3, 0, 4);
    REQUIRE(i->delim() == MD::WithPosition{0, 3, 0, 3});
    REQUIRE(i->items().size() == 1);
    REQUIRE(i->items().at(0)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(i->items().at(0).get());
    CHECK_POSITIONS(p, 2, 3, 5, 3);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    CHECK_POSITIONS(t, 2, 3, 5, 3);
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("list"));
}

/*
<!--
> -->
>
> list

*/
TEST_CASE("163")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/163.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    CHECK_POSITIONS(h, 0, 0, 4, 1);
    REQUIRE(h->text() == QStringLiteral("<!--\n> -->"));

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Blockquote);
    auto b = static_cast<MD::Blockquote *>(doc->items().at(2).get());
    CHECK_POSITIONS(b, 0, 2, 5, 3);
    REQUIRE(b->delims() == MD::Blockquote::Delims{{0, 2, 0, 2}, {0, 3, 0, 3}});
    REQUIRE(b->items().size() == 1);
    REQUIRE(b->items().at(0)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(b->items().at(0).get());
    CHECK_POSITIONS(p, 2, 3, 5, 3);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    CHECK_POSITIONS(t, 2, 3, 5, 3);
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("list"));
}

/*
<!--
- -->
-
- list

*/
TEST_CASE("164")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/164.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    CHECK_POSITIONS(h, 0, 0, 4, 1);
    REQUIRE(h->text() == QStringLiteral("<!--\n- -->"));

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(2).get());
    REQUIRE(l->items().size() == 2);
    CHECK_POSITIONS(l, 0, 2, 0, 4);

    {
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto i = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(i->isEmpty());
    }

    {
        REQUIRE(l->items().at(1)->type() == MD::ItemType::ListItem);
        auto i = static_cast<MD::ListItem *>(l->items().at(1).get());
        CHECK_POSITIONS(i, 0, 3, 0, 4);
        REQUIRE(i->delim() == MD::WithPosition{0, 3, 0, 3});
        REQUIRE(i->items().size() == 1);
        REQUIRE(i->items().at(0)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(i->items().at(0).get());
        CHECK_POSITIONS(p, 2, 3, 5, 3);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 2, 3, 5, 3);
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("list"));
    }
}

/*
<a href="www.google.com">
Google
</a>
-

<a href="www.google.com">Google</a>
-

*/
TEST_CASE("165")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/165.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    CHECK_POSITIONS(h, 0, 0, 0, 3);
    REQUIRE(h->text() == QStringLiteral("<a href=\"www.google.com\">\nGoogle\n</a>\n-"));

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(2).get());
        CHECK_POSITIONS(h, 0, 5, 0, 6);
        REQUIRE(h->level() == 2);
        REQUIRE(h->text().get());
        auto p = h->text().get();
        CHECK_POSITIONS(p, 0, 5, 34, 5);
        REQUIRE(p->items().size() == 3);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::RawHtml);
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        REQUIRE(p->items().at(2)->type() == MD::ItemType::RawHtml);
    }
}

/*
heading
---
    code


    code

*/
TEST_CASE("166")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/166.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
    auto h = static_cast<MD::Heading *>(doc->items().at(1).get());
    CHECK_POSITIONS(h, 0, 0, 2, 1);
    REQUIRE(h->delims().size() == 1);
    REQUIRE(h->delims().front() == MD::WithPosition{0, 1, 2, 1});
    REQUIRE(h->level() == 2);
    REQUIRE(h->text().get());
    auto p = h->text().get();
    CHECK_POSITIONS(p, 0, 0, 6, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->text() == QStringLiteral("heading"));
    CHECK_POSITIONS(t, 0, 0, 6, 0);

    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(doc->items().at(2).get());
    CHECK_POSITIONS(c, 4, 2, 7, 5);
    REQUIRE(c->text() == QStringLiteral("code\n\n\ncode"));
    REQUIRE(!c->isInline());
}

/*
<!-- c -->
-

<?php
?>
-

<pre>
</pre>
-

<?php
?>
text
-

text
<?php
?>
-

*/
TEST_CASE("167")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/167.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 12);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
        CHECK_POSITIONS(h, 0, 0, 9, 0);
        REQUIRE(h->text() == QStringLiteral("<!-- c -->"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List *>(doc->items().at(2).get());
        REQUIRE(l->items().size() == 1);
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(li->isEmpty());
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(doc->items().at(3).get());
        CHECK_POSITIONS(h, 0, 3, 1, 4);
        REQUIRE(h->text() == QStringLiteral("<?php\n?>"));
    }

    {
        REQUIRE(doc->items().at(4)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List *>(doc->items().at(4).get());
        REQUIRE(l->items().size() == 1);
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(li->isEmpty());
    }

    {
        REQUIRE(doc->items().at(5)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(doc->items().at(5).get());
        CHECK_POSITIONS(h, 0, 7, 5, 8);
        REQUIRE(h->text() == QStringLiteral("<pre>\n</pre>"));
    }

    {
        REQUIRE(doc->items().at(6)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List *>(doc->items().at(6).get());
        REQUIRE(l->items().size() == 1);
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(li->isEmpty());
    }

    {
        REQUIRE(doc->items().at(7)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(doc->items().at(7).get());
        CHECK_POSITIONS(h, 0, 11, 1, 12);
        REQUIRE(h->text() == QStringLiteral("<?php\n?>"));
    }

    {
        REQUIRE(doc->items().at(8)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(8).get());
        CHECK_POSITIONS(h, 0, 13, 0, 14);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 14, 0, 14});
        REQUIRE(h->level() == 2);
        REQUIRE(h->text().get());
        auto p = h->text().get();
        CHECK_POSITIONS(p, 0, 13, 3, 13);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->text() == QStringLiteral("text"));
        CHECK_POSITIONS(t, 0, 13, 3, 13);
    }

    {
        REQUIRE(doc->items().at(9)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(9).get());
        CHECK_POSITIONS(p, 0, 16, 3, 16);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->text() == QStringLiteral("text"));
        CHECK_POSITIONS(t, 0, 16, 3, 16);
    }

    {
        REQUIRE(doc->items().at(10)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(doc->items().at(10).get());
        CHECK_POSITIONS(h, 0, 17, 1, 18);
        REQUIRE(h->text() == QStringLiteral("<?php\n?>"));
    }

    {
        REQUIRE(doc->items().at(11)->type() == MD::ItemType::List);
        auto l = static_cast<MD::List *>(doc->items().at(11).get());
        REQUIRE(l->items().size() == 1);
        REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
        REQUIRE(li->isEmpty());
    }
}

/*
| table |
| ----- |
| <img src="img/img.png"> |

*/
TEST_CASE("168")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/168.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Table);
    auto t = static_cast<MD::Table *>(doc->items().at(1).get());
    CHECK_POSITIONS(t, 0, 0, 26, 2);

    REQUIRE(t->columnsCount() == 1);
    REQUIRE(t->rows().size() == 2);

    auto r0 = t->rows().at(0);
    CHECK_POSITIONS(r0.get(), 0, 0, 8, r0->startLine());

    REQUIRE(r0->type() == MD::ItemType::TableRow);

    REQUIRE(r0->cells().size() == 1);

    REQUIRE(r0->cells().at(0)->type() == MD::ItemType::TableCell);
    auto c0 = static_cast<MD::TableCell *>(r0->cells().at(0).get());
    CHECK_POSITIONS(c0, 2, r0->startLine(), 6, c0->startLine());

    REQUIRE(c0->items().size() == 1);
    REQUIRE(c0->items().at(0)->type() == MD::ItemType::Text);

    auto t0 = static_cast<MD::Text *>(c0->items().at(0).get());
    CHECK_POSITIONS(t0, 2, c0->startLine(), 6, t0->startLine());

    REQUIRE(t0->text() == QStringLiteral("table"));

    auto r1 = t->rows().at(1);
    CHECK_POSITIONS(r1.get(), 0, 2, 26, r1->startLine());

    REQUIRE(r1->type() == MD::ItemType::TableRow);

    REQUIRE(r1->cells().size() == 1);

    REQUIRE(r1->cells().at(0)->type() == MD::ItemType::TableCell);
    auto c1 = static_cast<MD::TableCell *>(r1->cells().at(0).get());
    CHECK_POSITIONS(c1, 2, r1->startLine(), 24, c1->startLine());

    REQUIRE(c1->items().size() == 1);
    REQUIRE(c1->items().at(0)->type() == MD::ItemType::RawHtml);

    auto h1 = static_cast<MD::RawHtml *>(c1->items().at(0).get());
    CHECK_POSITIONS(h1, 2, c1->startLine(), 24, c1->startLine());

    REQUIRE(h1->text() == QStringLiteral("<img src=\"img/img.png\">"));
}

/*
| table |
| ----- |
| <pre>code</pre> |

*/
TEST_CASE("169")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/169.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Table);
    auto t = static_cast<MD::Table *>(doc->items().at(1).get());
    CHECK_POSITIONS(t, 0, 0, 18, 2);

    REQUIRE(t->columnsCount() == 1);
    REQUIRE(t->rows().size() == 2);

    auto r0 = t->rows().at(0);
    CHECK_POSITIONS(r0.get(), 0, 0, 8, r0->startLine());

    REQUIRE(r0->type() == MD::ItemType::TableRow);

    REQUIRE(r0->cells().size() == 1);

    REQUIRE(r0->cells().at(0)->type() == MD::ItemType::TableCell);
    auto c0 = static_cast<MD::TableCell *>(r0->cells().at(0).get());
    CHECK_POSITIONS(c0, 2, r0->startLine(), 6, c0->startLine());

    REQUIRE(c0->items().size() == 1);
    REQUIRE(c0->items().at(0)->type() == MD::ItemType::Text);

    auto t0 = static_cast<MD::Text *>(c0->items().at(0).get());
    CHECK_POSITIONS(t0, 2, c0->startLine(), 6, t0->startLine());

    REQUIRE(t0->text() == QStringLiteral("table"));

    auto r1 = t->rows().at(1);
    CHECK_POSITIONS(r1.get(), 0, 2, 18, r1->startLine());

    REQUIRE(r1->type() == MD::ItemType::TableRow);

    REQUIRE(r1->cells().size() == 1);

    REQUIRE(r1->cells().at(0)->type() == MD::ItemType::TableCell);
    auto c1 = static_cast<MD::TableCell *>(r1->cells().at(0).get());
    CHECK_POSITIONS(c1, 2, r1->startLine(), 16, c1->startLine());

    REQUIRE(c1->items().size() == 3);
    REQUIRE(c1->items().at(0)->type() == MD::ItemType::RawHtml);

    {
        auto h1 = static_cast<MD::RawHtml *>(c1->items().at(0).get());
        CHECK_POSITIONS(h1, 2, c1->startLine(), 6, c1->startLine());

        REQUIRE(h1->text() == QStringLiteral("<pre>"));
    }

    {
        auto t1 = static_cast<MD::Text *>(c1->items().at(1).get());
        CHECK_POSITIONS(t1, 7, c1->startLine(), 10, c1->startLine());

        REQUIRE(t1->text() == QStringLiteral("code"));
    }

    {
        auto h1 = static_cast<MD::RawHtml *>(c1->items().at(2).get());
        CHECK_POSITIONS(h1, 11, c1->startLine(), 16, c1->startLine());

        REQUIRE(h1->text() == QStringLiteral("</pre>"));
    }
}

/*
Supported codecs:
| Format       |  Decoders        |  Encoders           |
|:-------------|:----------------:|:-------------------:|
| JPEG         | libjpeg(-turbo)  | libjpeg(-turbo)     |

*/
TEST_CASE("170")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/170.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 16, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    CHECK_POSITIONS(t, 0, 0, 16, 0);
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("Supported codecs:"));

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Table);
        auto t = static_cast<MD::Table *>(doc->items().at(2).get());
        CHECK_POSITIONS(t, 0, 1, 56, 3);

        REQUIRE(t->columnsCount() == 3);
        REQUIRE(t->rows().size() == 2);
    }
}

/*
`code
| table` |
| --- |
| data |

*/
TEST_CASE("171")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/171.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 4, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    CHECK_POSITIONS(t, 0, 0, 4, 0);
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("`code"));

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Table);
        auto t = static_cast<MD::Table *>(doc->items().at(2).get());
        CHECK_POSITIONS(t, 0, 1, 7, 3);

        REQUIRE(t->columnsCount() == 1);
        REQUIRE(t->rows().size() == 2);
    }
}

/*
<?php
| table` |
| --- |
| data |
?>

*/
TEST_CASE("172")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/172.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    CHECK_POSITIONS(h, 0, 0, 1, 4);
    REQUIRE(h->text() == QStringLiteral("<?php\n| table` |\n| --- |\n| data |\n?>"));
}

/*
$$
| table |
| --- |
| data |
$$

*/
TEST_CASE("173")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/173.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 1, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    CHECK_POSITIONS(t, 0, 0, 1, 0);
    REQUIRE(t->opts() == MD::TextWithoutFormat);
    REQUIRE(t->text() == QStringLiteral("$$"));

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Table);
        auto t = static_cast<MD::Table *>(doc->items().at(2).get());
        CHECK_POSITIONS(t, 0, 1, 1, 4);

        REQUIRE(t->columnsCount() == 1);
        REQUIRE(t->rows().size() == 3);
    }
}

/*
Text
***
| table |
| --- |
| data |

*/
TEST_CASE("174")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/174.md"));

    auto checkDoc = [](QSharedPointer<MD::Document> doc) {
        REQUIRE(doc->isEmpty() == false);
        REQUIRE(doc->items().size() == 4);

        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
        CHECK_POSITIONS(p, 0, 0, 3, 0);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        CHECK_POSITIONS(t, 0, 0, 3, 0);
        REQUIRE(t->opts() == MD::TextWithoutFormat);
        REQUIRE(t->text() == QStringLiteral("Text"));

        REQUIRE(doc->items().at(2)->type() == MD::ItemType::HorizontalLine);

        {
            REQUIRE(doc->items().at(3)->type() == MD::ItemType::Table);
            auto t = static_cast<MD::Table *>(doc->items().at(3).get());
            CHECK_POSITIONS(t, 0, 2, 7, 4);

            REQUIRE(t->columnsCount() == 1);
            REQUIRE(t->rows().size() == 2);
        }
    };

    checkDoc(doc);

    checkDoc(doc->clone().staticCast<MD::Document>());
}

/*
[link](www.google.com)
| table |
| ----- |
| data  |

*/
TEST_CASE("175")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/175.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 21, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Table);
        auto t = static_cast<MD::Table *>(doc->items().at(2).get());
        CHECK_POSITIONS(t, 0, 1, 8, 3);

        REQUIRE(t->columnsCount() == 1);
        REQUIRE(t->rows().size() == 2);
    }
}

/*
[link](www.google.com
| table |
| ----- |
| data  |

*/
TEST_CASE("176")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/176.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 20, 0);
    REQUIRE(p->items().size() == 2);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    REQUIRE(p->items().at(1)->type() == MD::ItemType::Link);

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Table);
        auto t = static_cast<MD::Table *>(doc->items().at(2).get());
        CHECK_POSITIONS(t, 0, 1, 8, 3);

        REQUIRE(t->columnsCount() == 1);
        REQUIRE(t->rows().size() == 2);
    }
}

/*
*text
| table* |
| ----- |
| data  |

*/
TEST_CASE("177")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/177.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 4, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(p->items().at(0).get());
    REQUIRE(t->text() == QStringLiteral("*text"));

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Table);
        auto t = static_cast<MD::Table *>(doc->items().at(2).get());
        CHECK_POSITIONS(t, 0, 1, 8, 3);

        REQUIRE(t->columnsCount() == 1);
        REQUIRE(t->rows().size() == 2);
    }
}

/*
<?php


data


?>

*/
TEST_CASE("178")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/178.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
    auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
    CHECK_POSITIONS(h, 0, 0, 1, 6);
    REQUIRE(h->text() == QStringLiteral("<?php\n\n\ndata\n\n\n?>"));
}

/*
<form>

<form>

*/
TEST_CASE("179")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/179.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(doc->items().at(1).get());
        CHECK_POSITIONS(h, 0, 0, 5, 0);
        REQUIRE(h->text() == QStringLiteral("<form>"));
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::RawHtml);
        auto h = static_cast<MD::RawHtml *>(doc->items().at(2).get());
        CHECK_POSITIONS(h, 0, 2, 5, 2);
        REQUIRE(h->text() == QStringLiteral("<form>"));
    }
}

/*
* The

      ```
      # This
      ```

    The
*/
TEST_CASE("180")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/180.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    CHECK_POSITIONS(l, 0, 0, 6, 6);

    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto i = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(i->items().size() == 3);
    CHECK_POSITIONS(i, 0, 0, 6, 6);
    REQUIRE(i->delim() == MD::WithPosition{0, 0, 0, 0});

    REQUIRE(i->items().at(0)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(i->items().at(0).get());
    CHECK_POSITIONS(p, 2, 0, 4, 0);

    REQUIRE(i->items().at(1)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(i->items().at(1).get());
    CHECK_POSITIONS(c, 6, 2, 8, 4);
    REQUIRE(c->startDelim() == MD::WithPosition{-1, -1, -1, -1});
    REQUIRE(c->endDelim() == MD::WithPosition{-1, -1, -1, -1});
    REQUIRE(c->syntaxPos() == MD::WithPosition{-1, -1, -1, -1});
    REQUIRE(c->text() == QStringLiteral("```\n# This\n```"));

    {
        REQUIRE(i->items().at(2)->type() == MD::ItemType::Paragraph);
        auto p = static_cast<MD::Paragraph *>(i->items().at(2).get());
        CHECK_POSITIONS(p, 4, 6, 6, 6);
    }
}

/*
[<picture><source media="(prefers-color-scheme: dark)"
srcset="https://steampipe.io/images/steampipe-color-logo-and-wordmark-with-white-bubble.svg"><source
media="(prefers-color-scheme: light)"
srcset="https://steampipe.io/images/steampipe-color-logo-and-wordmark-with-white-bubble.svg"><img width="67%"
alt="Steampipe Logo"
src="https://steampipe.io/images/steampipe-color-logo-and-wordmark-with-white-bubble.svg"></picture>](https://steampipe.io?utm_id=gspreadme&utm_source=github&utm_medium=repo&utm_campaign=github&utm_content=readme)

*/
TEST_CASE("181")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/181.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 537, 0);
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().front()->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().front().get());
    CHECK_POSITIONS(l, 0, 0, 537, 0);
    REQUIRE(l->textPos() == MD::WithPosition{1, 0, 424, 0});
    REQUIRE(l->urlPos() == MD::WithPosition{427, 0, 536, 0});
    REQUIRE(l->url()
            == QStringLiteral("https://steampipe.io?utm_id=gspreadme&utm_source=github&"
                              "utm_medium=repo&utm_campaign=github&utm_content=readme"));
    REQUIRE(l->p().get());
    auto pp = l->p().get();
    CHECK_POSITIONS(pp, 1, 0, 424, 0);
    REQUIRE(pp->items().size() == 5);
    for (int i = 0; i < static_cast<long long int>(pp->items().size()); ++i) {
        REQUIRE(pp->items().at(i)->type() == MD::ItemType::RawHtml);
    }
}

/*
text

# h
## hh
### hhh

*/
TEST_CASE("182")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/182.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 5);

    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    CHECK_POSITIONS(p, 0, 0, 3, 0);

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(2).get());
        CHECK_POSITIONS(h, 0, 2, 2, 2);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 2, 0, 2});
        REQUIRE(h->level() == 1);
        REQUIRE(h->text().get());
        auto p = h->text().get();
        CHECK_POSITIONS(p, 2, 2, 2, 2);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(3).get());
        CHECK_POSITIONS(h, 0, 3, 4, 3);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 3, 1, 3});
        REQUIRE(h->level() == 2);
        REQUIRE(h->text().get());
        auto p = h->text().get();
        CHECK_POSITIONS(p, 3, 3, 4, 3);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    }

    {
        REQUIRE(doc->items().at(4)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(4).get());
        CHECK_POSITIONS(h, 0, 4, 6, 4);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 4, 2, 4});
        REQUIRE(h->level() == 3);
        REQUIRE(h->text().get());
        auto p = h->text().get();
        CHECK_POSITIONS(p, 4, 4, 6, 4);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    }
}

/*
# h
## hh
### hhh

*/
TEST_CASE("183")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/183.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 4);

    {
        REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(1).get());
        CHECK_POSITIONS(h, 0, 0, 2, 0);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 0, 0, 0});
        REQUIRE(h->level() == 1);
        REQUIRE(h->text().get());
        auto p = h->text().get();
        CHECK_POSITIONS(p, 2, 0, 2, 0);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    }

    {
        REQUIRE(doc->items().at(2)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(2).get());
        CHECK_POSITIONS(h, 0, 1, 4, 1);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 1, 1, 1});
        REQUIRE(h->level() == 2);
        REQUIRE(h->text().get());
        auto p = h->text().get();
        CHECK_POSITIONS(p, 3, 1, 4, 1);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    }

    {
        REQUIRE(doc->items().at(3)->type() == MD::ItemType::Heading);
        auto h = static_cast<MD::Heading *>(doc->items().at(3).get());
        CHECK_POSITIONS(h, 0, 2, 6, 2);
        REQUIRE(h->delims().size() == 1);
        REQUIRE(h->delims().front() == MD::WithPosition{0, 2, 2, 2});
        REQUIRE(h->level() == 3);
        REQUIRE(h->text().get());
        auto p = h->text().get();
        CHECK_POSITIONS(p, 4, 2, 6, 2);
        REQUIRE(p->items().size() == 1);
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
    }
}
