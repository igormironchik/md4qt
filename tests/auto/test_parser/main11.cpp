/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include "parser.h"

/*
*   text
   * text
   *


*/
TEST_CASE("312")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/312.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 3);
}

/*
>

*/
TEST_CASE("313")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/313.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);
    auto b = static_cast<MD::Blockquote *>(doc->items().at(1).get());
    REQUIRE(b->startColumn() == 0);
    REQUIRE(b->startLine() == 0);
    REQUIRE(b->endColumn() == 1);
    REQUIRE(b->endLine() == 0);
}

/*
* list

  ```cpp
  code
*

  ```

*/
TEST_CASE("314")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/314.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 2);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);
}

/*
*

  ```cpp
  code
*

  ```

*/
TEST_CASE("315")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/315.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 3);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    REQUIRE(doc->items().at(2)->type() == MD::ItemType::Code);
}

/*
*


*

*/
TEST_CASE("316")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/316.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 2);
}

/*
> ```cpp
>

*/
TEST_CASE("317")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/317.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);
    auto b = static_cast<MD::Blockquote *>(doc->items().at(1).get());
    REQUIRE(b->startColumn() == 0);
    REQUIRE(b->startLine() == 0);
    REQUIRE(b->endColumn() == 1);
    REQUIRE(b->endLine() == 1);
    REQUIRE(b->items().size() == 1);
    REQUIRE(b->items().at(0)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(b->items().at(0).get());
    REQUIRE(c->startColumn() == 2);
    REQUIRE(c->startLine() == 1);
    REQUIRE(c->endColumn() == 2);
    REQUIRE(c->endLine() == 1);
    REQUIRE(c->text() == QStringLiteral("\n"));
}

/*
```cpp

*/
TEST_CASE("318")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/318.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Code);
    auto c = static_cast<MD::Code *>(doc->items().at(1).get());
    REQUIRE(c->startColumn() == 6);
    REQUIRE(c->startLine() == 0);
    REQUIRE(c->endColumn() == 6);
    REQUIRE(c->endLine() == 0);
}

/*
### ---

*/
TEST_CASE("319")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/319.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Heading);
    auto h = static_cast<MD::Heading *>(doc->items().at(1).get());
    REQUIRE(h->startColumn() == 0);
    REQUIRE(h->startLine() == 0);
    REQUIRE(h->endColumn() == 6);
    REQUIRE(h->endLine() == 0);
    REQUIRE(h->text()->items().size() == 1);
    REQUIRE(h->text()->startColumn() == 4);
    REQUIRE(h->text()->startLine() == 0);
    REQUIRE(h->text()->endColumn() == 6);
    REQUIRE(h->text()->endLine() == 0);
    REQUIRE(h->text()->items().at(0)->type() == MD::ItemType::Text);
    auto t = static_cast<MD::Text *>(h->text()->items().at(0).get());
    REQUIRE(t->startColumn() == 4);
    REQUIRE(t->startLine() == 0);
    REQUIRE(t->endColumn() == 6);
    REQUIRE(t->endLine() == 0);
}

/*
**text***text*

*/
TEST_CASE("320")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/320.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 2);

    {
        REQUIRE(p->items().at(0)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(0).get());
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
        REQUIRE(t->text() == QStringLiteral("text"));
        REQUIRE(t->opts() == MD::BoldText);
    }

    {
        REQUIRE(p->items().at(1)->type() == MD::ItemType::Text);
        auto t = static_cast<MD::Text *>(p->items().at(1).get());
        REQUIRE(t->openStyles().size() == 1);
        REQUIRE(t->closeStyles().size() == 1);
        REQUIRE(t->text() == QStringLiteral("text"));
        REQUIRE(t->opts() == MD::ItalicText);
    }
}

/*
> | table |
| --- |

*/
TEST_CASE("321")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/321.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Blockquote);
    auto b = static_cast<MD::Blockquote *>(doc->items().at(1).get());
    REQUIRE(b->items().size() == 1);
    REQUIRE(b->items().at(0)->type() == MD::ItemType::Paragraph);
}

/*
* | table |
| --- |

*/
TEST_CASE("322")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/322.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->items().size() == 1);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
}

/*
* list
  * list
    | table |
  | --- |

*/
TEST_CASE("323")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/323.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->items().size() == 2);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::List);

    {
        auto nl = static_cast<MD::List *>(li->items().at(1).get());
        REQUIRE(nl->items().size() == 1);
        REQUIRE(nl->items().at(0)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(nl->items().at(0).get());
        REQUIRE(li->items().size() == 1);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    }
}

/*
* list
  * list
    | table |
    | --- |

*/
TEST_CASE("324")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/324.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(doc->items().at(1).get());
    REQUIRE(l->items().size() == 1);
    REQUIRE(l->items().at(0)->type() == MD::ItemType::ListItem);
    auto li = static_cast<MD::ListItem *>(l->items().at(0).get());
    REQUIRE(li->items().size() == 2);
    REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(li->items().at(1)->type() == MD::ItemType::List);

    {
        auto nl = static_cast<MD::List *>(li->items().at(1).get());
        REQUIRE(nl->items().size() == 1);
        REQUIRE(nl->items().at(0)->type() == MD::ItemType::ListItem);
        auto li = static_cast<MD::ListItem *>(nl->items().at(0).get());
        REQUIRE(li->items().size() == 2);
        REQUIRE(li->items().at(0)->type() == MD::ItemType::Paragraph);
        REQUIRE(li->items().at(1)->type() == MD::ItemType::Table);
    }
}

/*
[^1]: Text

    * list 1
    * list 2

*/
TEST_CASE("325")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/325.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 1);
    REQUIRE(doc->footnotesMap().size() == 1);
    auto f = doc->footnotesMap().cbegin().value();
    REQUIRE(f->items().size() == 2);
    REQUIRE(f->items().at(0)->type() == MD::ItemType::Paragraph);
    REQUIRE(f->items().at(1)->type() == MD::ItemType::List);
    auto l = static_cast<MD::List *>(f->items().at(1).get());
    REQUIRE(l->items().size() == 2);
}

/*
[![License: GFDLv1.3](https://img.shields.io/badge/license-GFDLv1.3-blue
)](https://www.gnu.org/licenses/fdl-1.3.html)

*/
TEST_CASE("326")
{
    MD::Parser parser;

    auto doc = parser.parse(QStringLiteral("tests/parser/data/326.md"));

    REQUIRE(doc->isEmpty() == false);
    REQUIRE(doc->items().size() == 2);
    REQUIRE(doc->items().at(1)->type() == MD::ItemType::Paragraph);
    auto p = static_cast<MD::Paragraph *>(doc->items().at(1).get());
    REQUIRE(p->items().size() == 1);
    REQUIRE(p->items().at(0)->type() == MD::ItemType::Link);
    auto l = static_cast<MD::Link *>(p->items().at(0).get());
    REQUIRE(l->p()->items().size() == 1);
    REQUIRE(l->p()->items().at(0)->type() == MD::ItemType::Image);
}
