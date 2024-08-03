
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

typename TRAIT::String
fullPath( int num )
{
	typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
		+ u8"/tests/html/data/";

	auto numStr = std::to_string( num );
	numStr = std::string( 3 - numStr.size(), '0' ) + numStr;

	wd += TRAIT::latin1ToString( numStr.c_str() ) + u8".md";

#ifndef MD4QT_QT_SUPPORT
	std::string tmp;
	wd.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	wd = icu::UnicodeString::fromUTF8( tmp );
#endif

	return wd;
}


/*
**[google](www.google.com)* text*

*/
TEST_CASE( "001" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/001.md" ), false, {}, false );
	REQUIRE( html == u8"<p><em><em><a href=\"www.google.com\"> google </a>"
		"</em> text </em></p>" );
}

/*
**text* text*

*/
TEST_CASE( "002" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/002.md" ), false, {}, false );
	REQUIRE( html == u8"<p><em><em> text text </em></em></p>" );
}

/*
# heading

*/
TEST_CASE( "003" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/003.md" ), false, {}, false );
	const auto required = u8"\n<h1 id=\"heading/" + fullPath( 3 ) + u8"\"> heading </h1>\n";
	REQUIRE( html == required );
}

/*
| heading1 | heading2 | heading3 |
| :--- | :---: | ---: |
| data1 | data2 | data3 |

*/
TEST_CASE( "004" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/004.md" ), false, {}, false );
	const auto required = u8"\n<table><thead><tr>\n"
		"<th align=\"left\">\n heading1 \n</th>\n"
		"<th align=\"center\">\n heading2 \n</th>\n"
		"<th align=\"right\">\n heading3 \n</th>\n"
		"</tr></thead><tbody>\n<tr>\n\n"
		"<td align=\"left\">\n data1 \n</td>\n\n"
		"<td align=\"center\">\n data2 \n</td>\n\n"
		"<td align=\"right\">\n data3 \n</td>\n\n</tr>\n</tbody></table>\n";
	REQUIRE( html == required );
}

/*
**bold** ~strike~

*/
TEST_CASE( "005" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/005.md" ), false, {}, false );
	const auto required = u8"<p><strong> bold </strong><del> strike </del></p>";
	REQUIRE( html == required );
}

/*
$a \ne 0$

*/
TEST_CASE( "006" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/006.md" ), false, {}, false );
	const auto required = u8"<p>$ a \\ne 0 $</p>";
	REQUIRE( html == required );
}

/*
```cpp
int i = 0;
```

*/
TEST_CASE( "007" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/007.md" ), false, {}, false );
	const auto required = u8"\n<pre><code class=\"language-cpp\">int i = 0;</code></pre>\n";
	REQUIRE( html == required );
}

/*
`code`

*/
TEST_CASE( "008" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/008.md" ), false, {}, false );
	const auto required = u8"<p><code>code</code></p>";
	REQUIRE( html == required );
}

/*
> blockquote

*/
TEST_CASE( "009" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/009.md" ), false, {}, false );
	const auto required = u8"\n<blockquote><p> blockquote </p></blockquote>\n";
	REQUIRE( html == required );
}

/*
* list

<!-- -->

1. list


*/
TEST_CASE( "010" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/010.md" ), false, {}, false );
	const auto required = u8"\n<ul>\n<li>\n list </li>\n</ul>\n"
		"<!-- -->\n<ol>\n<li value=\"1\">\n list </li>\n</ol>\n";
	REQUIRE( html == required );
}

/*
* [ ] task
1. [x] list

*/
TEST_CASE( "011" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/011.md" ), false, {}, false );
	const auto required = u8"\n<ul class=\"contains-task-list\">\n"
		"<li class=\"task-list-item\"><input type=\"checkbox\" id=\"\" "
		"disabled=\"\" class=\"task-list-item-checkbox\">\n"
		" task </li>\n</ul>\n\n"
		"<ol class=\"contains-task-list\">\n"
		"<li class=\"task-list-item\"><input type=\"checkbox\" id=\"\" "
		"disabled=\"\" class=\"task-list-item-checkbox\" checked=\"\" value=\"1\">\n"
		" list </li>\n</ol>\n";
	REQUIRE( html == required );
}

/*
text[^1]

[^1]: footnote

*/
TEST_CASE( "012" )
{
	const auto path = fullPath( 12 );
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/012.md" ), false, u8"qrc://ref.png", false );
	const auto required = u8"<p> text<sup><a href=\"##^1/" + path +
		u8"\" id=\"ref-#^1/" + path + u8"-1\">1</a></sup></p>"
		"<section class=\"footnotes\"><ol><li id=\"#^1/" + path +
		u8"\"><p> footnote <a href=\"#ref-#^1/" + path +
		u8"-1\"><img src=\"qrc://ref.png\" /></a></p></li></ol></section>\n";
	REQUIRE( html == required );
}

/*
![](https://www.google.com)

*/
TEST_CASE( "013" )
{
	const auto path = fullPath( 13 );
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/013.md" ), false, {}, true );
	const auto required = u8"<article class=\"markdown-body\">\n"
		"<div id=\"" + path + u8"\"></div>\n<p>"
		"<img src=\"https://www.google.com\" alt=\"\" style=\"max-width:100%;\" /></p></article>\n";
	REQUIRE( html == required );
}

/*
text<space><space>
text
text
___


*/
TEST_CASE( "014" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/014.md" ), false, {}, false );
	const auto required = u8"<p> text <br />\n text \n text </p><hr />";
	REQUIRE( html == required );
}

/*
| h1 | h2 |
| - | - |
| d1 |


*/
TEST_CASE( "015" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/015.md" ), true, {}, false );
	const auto required = u8"<!DOCTYPE html>\n<html><head></head><body>\n\n"
		"<table><thead><tr>\n<th align=\"left\">\n h1 \n</th>\n"
		"<th align=\"left\">\n h2 \n</th>\n</tr></thead><tbody>\n"
		"<tr>\n\n<td align=\"left\">\n d1 \n</td>\n<td></td>\n</tr>\n"
		"</tbody></table>\n</body></html>\n";
	REQUIRE( html == required );
}
