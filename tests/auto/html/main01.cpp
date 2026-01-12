/*
    SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include "html.h"
#include "parser.h"

// Qt include.
#include <QDir>

QString fullPath(int num)
{
    QString wd = QDir().absolutePath() + QStringLiteral("/tests/html/data/");

    auto numStr = std::to_string(num);
    numStr = std::string(3 - numStr.size(), '0') + numStr;

    wd.append(QString::fromStdString(numStr) + QStringLiteral(".md"));

    return wd;
}

/*
**[google](www.google.com)* text*

*/
TEST_CASE("001")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/001.md")), false, {}, false);
    REQUIRE(html
            == QStringLiteral("<p dir=\"auto\"><em><em><a href=\"www.google.com\">google</a>"
                              "</em> text</em></p>"));
}

/*
**text* text*

*/
TEST_CASE("002")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/002.md")), false, {}, false);
    REQUIRE(html == QStringLiteral("<p dir=\"auto\"><em><em>text</em> text</em></p>"));
}

/*
# heading

*/
TEST_CASE("003")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/003.md")), false, {}, false);
    const QString required =
        QStringLiteral("\n<h1 id=\"heading/") + fullPath(3) + QStringLiteral("\" dir=\"auto\">heading</h1>\n");
    REQUIRE(html == required);
}

/*
| heading1 | heading2 | heading3 |
| :--- | :---: | ---: |
| data1 | data2 | data3 |

*/
TEST_CASE("004")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/004.md")), false, {}, false);
    const QString required = QStringLiteral(
        "\n<table><thead><tr>\n"
        "<th align=\"left\" dir=\"auto\">\nheading1\n</th>\n"
        "<th align=\"center\" dir=\"auto\">\nheading2\n</th>\n"
        "<th align=\"right\" dir=\"auto\">\nheading3\n</th>\n"
        "</tr></thead><tbody>\n<tr>\n\n"
        "<td align=\"left\" dir=\"auto\">\ndata1\n</td>\n\n"
        "<td align=\"center\" dir=\"auto\">\ndata2\n</td>\n\n"
        "<td align=\"right\" dir=\"auto\">\ndata3\n</td>\n\n</tr>\n</tbody></table>\n");
    REQUIRE(html == required);
}

/*
**bold** ~strike~

*/
TEST_CASE("005")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/005.md")), false, {}, false);
    const QString required = QStringLiteral("<p dir=\"auto\"><strong>bold</strong> <del>strike</del></p>");
    REQUIRE(html == required);
}

/*
$a \ne 0$

*/
TEST_CASE("006")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/006.md")), false, {}, false);
    const QString required = QStringLiteral("<p dir=\"auto\">$a \\ne 0$</p>");
    REQUIRE(html == required);
}

/*
```cpp
int i = 0;
```

*/
TEST_CASE("007")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/007.md")), false, {}, false);
    const QString required = QStringLiteral("\n<pre><code class=\"language-cpp\">int i = 0;</code></pre>\n");
    REQUIRE(html == required);
}

/*
`code`

*/
TEST_CASE("008")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/008.md")), false, {}, false);
    const QString required = QStringLiteral("<p dir=\"auto\"><code>code</code></p>");
    REQUIRE(html == required);
}

/*
> blockquote

*/
TEST_CASE("009")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/009.md")), false, {}, false);
    const QString required = QStringLiteral("\n<blockquote><p dir=\"auto\">blockquote</p></blockquote>\n");
    REQUIRE(html == required);
}

/*
* list

<!-- -->

1. list


*/
TEST_CASE("010")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/010.md")), false, {}, false);
    const QString required = QStringLiteral(
        "\n<ul dir=\"auto\">\n<li>\nlist</li>\n</ul>\n"
        "<!-- -->\n<ol dir=\"auto\">\n<li value=\"1\">\nlist</li>\n</ol>\n");
    REQUIRE(html == required);
}

/*
* [ ] task
1. [x] list

*/
TEST_CASE("011")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/011.md")), false, {}, false);
    const QString required = QStringLiteral(
        "\n<ul class=\"contains-task-list\" dir=\"auto\">\n"
        "<li class=\"task-list-item\"><input type=\"checkbox\" id=\"\" "
        "disabled=\"\" class=\"task-list-item-checkbox\">\n"
        "task</li>\n</ul>\n\n"
        "<ol class=\"contains-task-list\" dir=\"auto\">\n"
        "<li class=\"task-list-item\"><input type=\"checkbox\" id=\"\" "
        "disabled=\"\" class=\"task-list-item-checkbox\" checked=\"\" value=\"1\">\n"
        "list</li>\n</ol>\n");
    REQUIRE(html == required);
}

/*
text[^1]

[^1]: footnote

*/
TEST_CASE("012")
{
    const auto path = fullPath(12);
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/012.md")),
                           false,
                           QStringLiteral("<img src=\"qrc://ref.png\" />"),
                           false);
    const QString required = QStringLiteral("<p dir=\"auto\">text<sup><a href=\"##^1/")
        + path
        + QStringLiteral("\" id=\"ref-#^1/")
        + path
        + QStringLiteral("-1\">1</a></sup></p>"
                         "<section class=\"footnotes\"><ol dir=\"auto\"><li id=\"#^1/")
        + path
        + QStringLiteral("\"><p dir=\"auto\">footnote<a href=\"#ref-#^1/")
        + path
        + QStringLiteral("-1\"><img src=\"qrc://ref.png\" /></a></p></li></ol></section>\n");
    REQUIRE(html == required);
}

/*
![](https://www.google.com)

*/
TEST_CASE("013")
{
    const auto path = fullPath(13);
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/013.md")), false, {}, true);
    const QString required = QStringLiteral(
                                 "<article class=\"markdown-body\">\n"
                                 "<div id=\"")
        + path
        + QStringLiteral("\"></div>\n<p dir=\"auto\">"
                         "<img src=\"https://www.google.com\" alt=\"\" style=\"max-width:100%;\" /></p></article>\n");
    REQUIRE(html == required);
}

/*
text<space><space>
text
text
___


*/
TEST_CASE("014")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/014.md")), false, {}, false);
    const QString required = QStringLiteral("<p dir=\"auto\">text<br />\ntext\ntext</p><hr />");
    REQUIRE(html == required);
}

/*
| h1 | h2 |
| - | - |
| d1 |


*/
TEST_CASE("015")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/015.md")), true, {}, false);
    const QString required = QStringLiteral(
        "<!DOCTYPE html>\n<html><head></head><body>\n\n"
        "<table><thead><tr>\n<th align=\"left\" dir=\"auto\">\nh1\n</th>\n"
        "<th align=\"left\" dir=\"auto\">\nh2\n</th>\n</tr></thead><tbody>\n"
        "<tr>\n\n<td align=\"left\" dir=\"auto\">\nd1\n</td>\n<td dir=\"auto\"></td>\n</tr>\n"
        "</tbody></table>\n</body></html>\n");
    REQUIRE(html == required);
}

/*
> # heading
>
>     code
>
> * list
>
> | h |
> | - |
> | d |
>
> ---
>
> <table></table>
>
> text <a></a>
>
> > nested quote

*/
TEST_CASE("016")
{
    const auto path = fullPath(16);
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/016.md")), false, {}, false);
    const QString required = QStringLiteral(
                                 "\n<blockquote>\n"
                                 "<h1 id=\"heading/")
        + path
        + QStringLiteral("\" dir=\"auto\">heading</h1>\n\n"
                         "<pre><code>code</code></pre>\n\n"
                         "<ul dir=\"auto\">\n<li>\nlist</li>\n</ul>\n\n"
                         "<table><thead><tr>\n<th align=\"left\" dir=\"auto\">\nh\n</th>\n</tr></thead>"
                         "<tbody>\n<tr>\n\n<td align=\"left\" dir=\"auto\">\nd\n</td>\n\n</tr>\n</tbody></table>\n"
                         "<hr />"
                         "<table></table>"
                         "<p dir=\"auto\">text <a></a></p>\n"
                         "<blockquote><p dir=\"auto\">nested quote</p></blockquote>\n"
                         "</blockquote>\n");
    REQUIRE(html == required);
}

/*
* list

  # Heading

      code

  > quote

  | t |
  | - |
  | d |

  <div></div>

  ___


*/
TEST_CASE("017")
{
    const auto path = fullPath(17);
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/017.md")), false, {}, false);
    const QString required = QStringLiteral(
                                 "\n<ul dir=\"auto\">\n<li>\n<p dir=\"auto\">list</p>\n"
                                 "<h1 id=\"Heading/")
        + path
        + QStringLiteral("\" dir=\"auto\">Heading</h1>\n\n"
                         "<pre><code>code</code></pre>\n\n"
                         "<blockquote><p dir=\"auto\">quote</p></blockquote>\n\n"
                         "<table><thead><tr>\n<th align=\"left\" dir=\"auto\">\nt\n</th>\n</tr></thead>"
                         "<tbody>\n<tr>\n\n<td align=\"left\" dir=\"auto\">\nd\n</td>\n\n</tr>\n</tbody></table>\n"
                         "<div></div><hr /></li>\n</ul>\n");
    REQUIRE(html == required);
}

/*
| head |
| ---- |
| `code` [google](https://www.google.com)[^1] ![](https://www.google.com) $a /ne 0$ |

[^1]: # heading

        code

    > quote

    * list

    | t |
    | - |
    | d |

    <div></div>

    ___


*/
TEST_CASE("018")
{
    const auto path = fullPath(18);
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/018.md")), false, {}, false);
    const QString required = QStringLiteral(
                                 "\n<table><thead><tr>\n<th align=\"left\" dir=\"auto\">\nhead\n</th>\n</tr></thead>"
                                 "<tbody>\n<tr>\n\n<td align=\"left\" dir=\"auto\">\n"
                                 "<code>code</code> "
                                 "<a href=\"https://www.google.com\">google</a>"
                                 "<sup><a href=\"##^1/")
        + path
        + QStringLiteral("\" id=\"ref-#^1/")
        + path
        + QStringLiteral("-1\">1</a></sup> "
                         "<img src=\"https://www.google.com\" alt=\"\" style=\"max-width:100%;\" /> "
                         "$a /ne 0$\n</td>\n\n</tr>\n</tbody></table>\n"
                         "<section class=\"footnotes\"><ol dir=\"auto\"><li id=\"#^1/")
        + path
        + QStringLiteral("\">\n"
                         "<h1 id=\"heading/")
        + path
        + QStringLiteral("\" dir=\"auto\">heading</h1>\n\n"
                         "<pre><code>code</code></pre>\n\n"
                         "<blockquote><p dir=\"auto\">quote</p></blockquote>\n\n"
                         "<ul dir=\"auto\">\n<li>\nlist</li>\n</ul>\n\n"
                         "<table><thead><tr>\n<th align=\"left\" dir=\"auto\">\nt\n</th>\n</tr></thead>"
                         "<tbody>\n<tr>\n\n<td align=\"left\" dir=\"auto\">\nd\n</td>\n\n</tr>\n</tbody></table>\n"
                         "<div></div><hr /></li></ol></section>\n");

    REQUIRE(html == required);
}

/*
+ Create a list by starting a line with `+`, `-`, or `*`
+ Sub-lists are made by indenting 2 spaces:
  - Marker character change forces new list start:
    * Ac tristique libero volutpat at
    + Facilisis in pretium nisl aliquet
    - Nulla volutpat aliquam velit
+ Very easy!

*/
TEST_CASE("019")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/019.md")), false, {}, false);
    const QString required = QStringLiteral(
        "\n<ul dir=\"auto\">\n<li>\nCreate a list by starting a line with "
        "<code>+</code>, <code>-</code>, or <code>*</code></li>\n"
        "<li>\nSub-lists are made by indenting 2 spaces:\n"
        "<ul dir=\"auto\">\n<li>\nMarker character change forces new list start:\n"
        "<ul dir=\"auto\">\n<li>\nAc tristique libero volutpat at</li>\n</ul>\n\n"
        "<ul dir=\"auto\">\n<li>\nFacilisis in pretium nisl aliquet</li>\n</ul>\n\n"
        "<ul dir=\"auto\">\n<li>\nNulla volutpat aliquam velit</li>\n</ul>\n"
        "</li>\n</ul>\n</li>\n<li>\nVery easy!</li>\n</ul>\n");

    REQUIRE(html == required);
}

/*
What is a [Google]?

[Google]: https://www.google.com

*/
TEST_CASE("020")
{
    MD::Parser p;
    auto html = MD::toHtml(p.parse(QStringLiteral("tests/html/data/020.md")), false, {}, false);
    const QString required =
        QStringLiteral("<p dir=\"auto\">What is a <a href=\"https://www.google.com\">Google</a>?</p>");
    REQUIRE(html == required);
}
