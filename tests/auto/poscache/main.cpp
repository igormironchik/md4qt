/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include <md4qt/parser.h>
#include <md4qt/poscache.h>
#include <md4qt/utils.h>

MD::PosCache<TRAIT> g_cache;

inline typename TRAIT::String to_string(int i)
{
#ifdef MD4QT_QT_SUPPORT
    return QString::number(i);
#else
    return std::to_string(i);
#endif
}

std::shared_ptr<MD::Document<TRAIT>> prepareTest(const typename TRAIT::String &fileName)
{
    MD::Parser<TRAIT> p;
    auto doc = p.parse(TRAIT::latin1ToString("tests/parser/data/") + fileName);

    g_cache.initialize(doc);

    return doc;
}

TEST_CASE("001")
{
    prepareTest(TRAIT::latin1ToString("001.md"));
    REQUIRE(g_cache.findFirstInCache({0, 0, 0, 0}).empty());
    REQUIRE(g_cache.findFirstInCache({1, 1, 1, 1}).empty());
}

/*
This is just a text!

*/
TEST_CASE("002")
{
    prepareTest(TRAIT::latin1ToString("002.md"));
    auto items = g_cache.findFirstInCache({0, 0, 0, 0});
    REQUIRE(items.size() == 2);
    REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(items.at(1)->type() == MD::ItemType::Text);
    REQUIRE(g_cache.findFirstInCache({0, 1, 0, 1}).empty());
}

/*

Paragraph 1.

Paragraph 2.

*/
TEST_CASE("003")
{
    prepareTest(TRAIT::latin1ToString("003.md"));
    REQUIRE(g_cache.findFirstInCache({0, 0, 0, 0}).empty());

    for (int i = 0; i < 2; ++i) {
        auto items = g_cache.findFirstInCache({0, 1 + i * 2, 0, 1 + i * 2});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text<TRAIT> *>(items.at(1));
        REQUIRE(t->text() == TRAIT::latin1ToString("Paragraph ") + to_string(i + 1) + TRAIT::latin1ToString("."));
    }
}

/*
Code in the `text`.

*/
TEST_CASE("012")
{
    prepareTest(TRAIT::latin1ToString("012.md"));

    {
        auto items = g_cache.findFirstInCache({0, 0, 0, 0});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Text);
    }

    {
        auto items = g_cache.findFirstInCache({12, 0, 12, 0});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Code);
    }

    {
        auto items = g_cache.findFirstInCache({13, 0, 13, 0});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Code);
    }

    {
        auto items = g_cache.findFirstInCache({0, 0, 17, 0});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Text);
    }

    {
        auto items = g_cache.findFirstInCache({18, 0, 18, 0});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Text);
    }
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
    prepareTest(TRAIT::latin1ToString("017.md"));

    {
        auto items = g_cache.findFirstInCache({0, 0, 0, 0});
        REQUIRE(items.size() == 1);
        REQUIRE(items.at(0)->type() == MD::ItemType::Blockquote);
    }

    {
        auto items = g_cache.findFirstInCache({1, 4, 1, 4});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Blockquote);
        REQUIRE(items.at(1)->type() == MD::ItemType::Blockquote);
    }

    {
        auto items = g_cache.findFirstInCache({2, 0, 2, 0});
        REQUIRE(items.size() == 3);
        REQUIRE(items.at(0)->type() == MD::ItemType::Blockquote);
        REQUIRE(items.at(1)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(2)->type() == MD::ItemType::Text);
    }

    {
        auto items = g_cache.findFirstInCache({0, 0, 0, 1});
        REQUIRE(items.size() == 3);
        REQUIRE(items.at(0)->type() == MD::ItemType::Blockquote);
        REQUIRE(items.at(1)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(2)->type() == MD::ItemType::Text);
    }

    {
        auto items = g_cache.findFirstInCache({3, 4, 3, 4});
        REQUIRE(items.size() == 4);
        REQUIRE(items.at(0)->type() == MD::ItemType::Blockquote);
        REQUIRE(items.at(1)->type() == MD::ItemType::Blockquote);
        REQUIRE(items.at(2)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(3)->type() == MD::ItemType::Text);
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
    prepareTest(TRAIT::latin1ToString("020.md"));

    {
        auto items = g_cache.findFirstInCache({0, 0, 0, 0});
        REQUIRE(items.size() == 1);
        REQUIRE(items.at(0)->type() == MD::ItemType::Code);
    }

    {
        auto items = g_cache.findFirstInCache({0, 1, 0, 1});
        REQUIRE(items.size() == 1);
        REQUIRE(items.at(0)->type() == MD::ItemType::Code);
    }
}

/*
    if( a > b )
      do_something();
    else
      dont_do_anything();
*/
TEST_CASE("021")
{
    prepareTest(TRAIT::latin1ToString("021.md"));

    {
        auto items = g_cache.findFirstInCache({4, 0, 4, 0});
        REQUIRE(items.size() == 1);
        REQUIRE(items.at(0)->type() == MD::ItemType::Code);
    }
}

/*
* Item 1
* Item 2
* Item 3

*/
TEST_CASE("023")
{
    prepareTest(TRAIT::latin1ToString("023.md"));

    {
        auto items = g_cache.findFirstInCache({4, 0, 4, 0});
        REQUIRE(items.size() == 4);
        REQUIRE(items.at(0)->type() == MD::ItemType::List);
        REQUIRE(items.at(1)->type() == MD::ItemType::ListItem);
        REQUIRE(items.at(2)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(3)->type() == MD::ItemType::Text);
    }

    {
        auto items = g_cache.findFirstInCache({0, 0, 0, 0});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::List);
        REQUIRE(items.at(1)->type() == MD::ItemType::ListItem);
    }
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
    prepareTest(TRAIT::latin1ToString("024.md"));

    {
        auto items = g_cache.findFirstInCache({4, 1, 4, 1});
        REQUIRE(items.size() == 6);
        REQUIRE(items.at(0)->type() == MD::ItemType::List);
        REQUIRE(items.at(1)->type() == MD::ItemType::ListItem);
        REQUIRE(items.at(2)->type() == MD::ItemType::List);
        REQUIRE(items.at(3)->type() == MD::ItemType::ListItem);
        REQUIRE(items.at(4)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(5)->type() == MD::ItemType::Text);
    }

    {
        auto items = g_cache.findFirstInCache({2, 1, 2, 1});
        REQUIRE(items.size() == 4);
        REQUIRE(items.at(0)->type() == MD::ItemType::List);
        REQUIRE(items.at(1)->type() == MD::ItemType::ListItem);
        REQUIRE(items.at(2)->type() == MD::ItemType::List);
        REQUIRE(items.at(3)->type() == MD::ItemType::ListItem);
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
    prepareTest(TRAIT::latin1ToString("025.md"));

    {
        auto items = g_cache.findFirstInCache({2, 2, 2, 2});
        REQUIRE(items.size() == 4);
        REQUIRE(items.at(0)->type() == MD::ItemType::List);
        REQUIRE(items.at(1)->type() == MD::ItemType::ListItem);
        REQUIRE(items.at(2)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(3)->type() == MD::ItemType::Text);
    }
}

/*
Text ![Image 1](a.jpg) continue ![ Image 2 ](b.png) and ![ Image 3]( http://www.where.com/c.jpeg "description" )

*/
TEST_CASE("030")
{
    prepareTest(TRAIT::latin1ToString("030.md"));

    {
        auto items = g_cache.findFirstInCache({5, 0, 5, 0});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Image);
    }
}

/*
[link 0][wrong-label] [link 1](a.md) [![image 1](a.png)](b.md) [link 3][label] [^ref]

[label]: http://www.where.com/a.md (caption)

[^ref] text

[1]: a.md 'title'

[link 4](#label)

*/
TEST_CASE("031")
{
    prepareTest(TRAIT::latin1ToString("031.md"));

    {
        auto items = g_cache.findFirstInCache({22, 0, 22, 0});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Link);
    }

    {
        auto items = g_cache.findFirstInCache({80, 0, 80, 0});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::FootnoteRef);
    }
}

/*
 [^footnote]: Paragraph in footnote

    Paragraph in footnote

    Paragraph in footnote

*/
TEST_CASE("045")
{
    prepareTest(TRAIT::latin1ToString("045.md"));

    {
        auto items = g_cache.findFirstInCache({1, 0, 1, 0});
        REQUIRE(items.size() == 1);
        REQUIRE(items.at(0)->type() == MD::ItemType::Footnote);
    }

    {
        auto items = g_cache.findFirstInCache({4, 2, 4, 2});
        REQUIRE(items.size() == 3);
        REQUIRE(items.at(0)->type() == MD::ItemType::Footnote);
        REQUIRE(items.at(1)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(2)->type() == MD::ItemType::Text);
    }
}

/*
Heading 1
=========
Paragraph 1

Heading 2
---------
Paragraph 2

  # Heading 1

Paragraph 1

## Heading 2

Paragraph 2

Heading 1
=========

Paragraph 1

Heading 2
---------

Paragraph 2

### Heading 3 {#heading-3}

*/
TEST_CASE("046")
{
    prepareTest(TRAIT::latin1ToString("046.md"));

    {
        auto items = g_cache.findFirstInCache({0, 1, 0, 1});
        REQUIRE(items.size() == 1);
        REQUIRE(items.at(0)->type() == MD::ItemType::Heading);
    }

    {
        auto items = g_cache.findFirstInCache({0, 0, 0, 0});
        REQUIRE(items.size() == 3);
        REQUIRE(items.at(0)->type() == MD::ItemType::Heading);
        REQUIRE(items.at(1)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(2)->type() == MD::ItemType::Text);
    }
}

/*

Column 1 | Column 2
---------|---------
Cell 1   | Cell 2

| Column 1 | Column 2 |
|:--------:|---------:|
| Cell 1   | Cell 2   |

*/
TEST_CASE("047")
{
    prepareTest(TRAIT::latin1ToString("047.md"));

    {
        auto items = g_cache.findFirstInCache({0, 1, 0, 1});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Table);
        REQUIRE(items.at(1)->type() == MD::ItemType::Text);
    }

    {
        auto items = g_cache.findFirstInCache({0, 2, 0, 2});
        REQUIRE(items.size() == 1);
        REQUIRE(items.at(0)->type() == MD::ItemType::Table);
    }
}

/*
When $a \ne 0$, there are two solutions to $(ax^2 + bx + c = 0)$ and they are
$$ x = {-b \pm \sqrt{b^2-4ac} \over 2a} $$

*/
TEST_CASE("065")
{
    prepareTest(TRAIT::latin1ToString("065.md"));

    {
        auto items = g_cache.findFirstInCache({5, 0, 5, 0});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Math);
    }
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
    prepareTest(TRAIT::latin1ToString("155.md"));

    {
        auto items = g_cache.findFirstInCache({2, 9, 2, 9});
        REQUIRE(items.size() == 3);
        REQUIRE(items.at(0)->type() == MD::ItemType::List);
        REQUIRE(items.at(1)->type() == MD::ItemType::ListItem);
        REQUIRE(items.at(2)->type() == MD::ItemType::Code);
    }
}

/*
| table |
| ----- |
| <img src="img/img.png"> |

*/
TEST_CASE("168")
{
    prepareTest(TRAIT::latin1ToString("168.md"));

    {
        auto items = g_cache.findFirstInCache({2, 2, 2, 2});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Table);
        REQUIRE(items.at(1)->type() == MD::ItemType::RawHtml);
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
    prepareTest(TRAIT::latin1ToString("174.md"));

    {
        auto items = g_cache.findFirstInCache({0, 1, 0, 1});
        REQUIRE(items.size() == 1);
        REQUIRE(items.at(0)->type() == MD::ItemType::HorizontalLine);
    }
}

/*
==

==

*/
TEST_CASE("258")
{
    auto doc = prepareTest(TRAIT::latin1ToString("258.md"));

    {
        auto items = g_cache.findFirstInCache({0, 0, 0, 0});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Text);
        REQUIRE(items.at(0) == doc->items().at(1).get());
    }

    {
        auto items = g_cache.findFirstInCache({0, 2, 0, 2});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Text);
        REQUIRE(items.at(0) == doc->items().at(2).get());
    }
}

/*
*$a \ne 0$*
*`code`*
*[google](https://www.google.com)*
*![](https://www.google.com)*
*[^1]*

[^1]: foot


*/
TEST_CASE("259")
{
    auto doc = prepareTest(TRAIT::latin1ToString("259.md"));

    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph<TRAIT> *>(doc->items().at(1).get());

    {
        auto items = g_cache.findFirstInCache({0, 0, 0, 0});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Math);
        REQUIRE(items.at(0) == p);
        REQUIRE(items.at(1) == p->items().at(0).get());
    }

    {
        auto items = g_cache.findFirstInCache({0, 1, 0, 1});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Code);
        REQUIRE(items.at(0) == p);
        REQUIRE(items.at(1) == p->items().at(1).get());
    }

    {
        auto items = g_cache.findFirstInCache({0, 2, 0, 2});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Link);
        REQUIRE(items.at(0) == p);
        REQUIRE(items.at(1) == p->items().at(2).get());
    }

    {
        auto items = g_cache.findFirstInCache({0, 3, 0, 3});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::Image);
        REQUIRE(items.at(0) == p);
        REQUIRE(items.at(1) == p->items().at(3).get());
    }

    {
        auto items = g_cache.findFirstInCache({0, 4, 0, 4});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == MD::ItemType::FootnoteRef);
        REQUIRE(items.at(0) == p);
        REQUIRE(items.at(1) == p->items().at(4).get());
    }
}

TEST_CASE("user_defined")
{
    class MyItem : public MD::Item<TRAIT>
    {
    public:
        MyItem() = default;
        ~MyItem() override = default;

        MD::ItemType type() const override
        {
            return MD::ItemType{static_cast<int>(MD::ItemType::UserDefined) + 1};
        }

        std::shared_ptr<Item<TRAIT>> clone(MD::Document<TRAIT> *doc = nullptr) const override
        {
            MD_UNUSED(doc)

            return std::make_shared<MyItem>();
        }
    };

    auto doc = std::make_shared<MD::Document<TRAIT>>();
    auto i = std::make_shared<MyItem>();
    i->setStartColumn(0);
    i->setStartLine(0);
    i->setEndColumn(10);
    i->setEndLine(0);

    auto p = std::make_shared<MD::Paragraph<TRAIT>>();
    p->setStartColumn(0);
    p->setStartLine(2);
    p->setEndColumn(10);
    p->setEndLine(2);

    auto in = std::make_shared<MyItem>();
    in->setStartColumn(0);
    in->setStartLine(2);
    in->setEndColumn(10);
    in->setEndLine(2);
    p->appendItem(in);

    doc->appendItem(i);
    doc->appendItem(p);

    g_cache.initialize(doc);

    {
        auto items = g_cache.findFirstInCache({0, 0, 0, 0});
        REQUIRE(items.size() == 1);
        REQUIRE(items.at(0)->type() == i->type());
    }

    {
        auto items = g_cache.findFirstInCache({1, 2, 1, 2});
        REQUIRE(items.size() == 2);
        REQUIRE(items.at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(items.at(1)->type() == i->type());
    }
}
