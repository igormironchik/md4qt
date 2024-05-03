
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

inline typename TRAIT::String
to_string( int i )
{
#ifdef MD4QT_QT_SUPPORT
	return QString::number( i );
#else
	return std::to_string( i );
#endif
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
TEST_CASE( "153" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/153.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->startColumn() == 2 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 6 );
	REQUIRE( l->endLine() == 17 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( i->items().size() == 2 );
	REQUIRE( i->startColumn() == 2 );
	REQUIRE( i->startLine() == 0 );
	REQUIRE( i->endColumn() == 6 );
	REQUIRE( i->endLine() == 17 );
	REQUIRE( i->delim() == MD::WithPosition{ 2, 0, 2, 0 } );

	REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 0 ).get() );
	REQUIRE( p->startColumn() == 4 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 7 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( i->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( i->items().at( 1 ).get() );
	REQUIRE( c->startColumn() == 0 );
	REQUIRE( c->startLine() == 3 );
	REQUIRE( c->endColumn() == 5 );
	REQUIRE( c->endLine() == 16 );
	REQUIRE( c->text() == u8"\n// very bad\nfunction\n*\nfoo() {\n  // ...\n}\n\n"
		"// very bad\nconst wat = function\n*\n() {\n  // ...\n};" );
}

/*
  - list

    ```javascript
    /**
     * make() returns a new element
     * based on the passed-in tag name
     -/
    function make(tag) {

      // ...

      return element;
    }
    ```

*/
TEST_CASE( "154" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/154.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->startColumn() == 2 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 6 );
	REQUIRE( l->endLine() == 13 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( i->items().size() == 2 );
	REQUIRE( i->startColumn() == 2 );
	REQUIRE( i->startLine() == 0 );
	REQUIRE( i->endColumn() == 6 );
	REQUIRE( i->endLine() == 13 );
	REQUIRE( i->delim() == MD::WithPosition{ 2, 0, 2, 0 } );

	REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 0 ).get() );
	REQUIRE( p->startColumn() == 4 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 7 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( i->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( i->items().at( 1 ).get() );
	REQUIRE( c->startColumn() == 4 );
	REQUIRE( c->startLine() == 3 );
	REQUIRE( c->endColumn() == 4 );
	REQUIRE( c->endLine() == 12 );
	REQUIRE( c->text() == u8"/**\n * make() returns a new element\n * based on the passed-in "
		"tag name\n */\nfunction make(tag) {\n\n  // ...\n\n  return element;\n}" );
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
TEST_CASE( "155" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/155.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 2 );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 7 );
	REQUIRE( l->endLine() == 9 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( i->items().size() == 2 );
		REQUIRE( i->startColumn() == 0 );
		REQUIRE( i->startLine() == 0 );
		REQUIRE( i->endColumn() == 4 );
		REQUIRE( i->endLine() == 4 );
		REQUIRE( i->delim() == MD::WithPosition{ 0, 0, 0, 0 } );

		REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 5 );
		REQUIRE( p->endLine() == 0 );

		REQUIRE( i->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( i->items().at( 1 ).get() );
		REQUIRE( c->startColumn() == 2 );
		REQUIRE( c->startLine() == 3 );
		REQUIRE( c->endColumn() == 7 );
		REQUIRE( c->endLine() == 3 );
		REQUIRE( c->text() == u8"int i;" );
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( i->items().size() == 2 );
		REQUIRE( i->startColumn() == 0 );
		REQUIRE( i->startLine() == 6 );
		REQUIRE( i->endColumn() == 7 );
		REQUIRE( i->endLine() == 9 );
		REQUIRE( i->delim() == MD::WithPosition{ 0, 6, 0, 6 } );

		REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 6 );
		REQUIRE( p->endColumn() == 5 );
		REQUIRE( p->endLine() == 6 );

		REQUIRE( i->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( i->items().at( 1 ).get() );
		REQUIRE( c->startColumn() == 2 );
		REQUIRE( c->startLine() == 9 );
		REQUIRE( c->endColumn() == 7 );
		REQUIRE( c->endLine() == 9 );
		REQUIRE( c->text() == u8"int i;" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( c->startColumn() == 0 );
	REQUIRE( c->startLine() == 12 );
	REQUIRE( c->endColumn() == 5 );
	REQUIRE( c->endLine() == 12 );
	REQUIRE( c->text() == u8"int i;" );
}

/*
<a id='column-width'></a>(<a href='#column-width'>link</a>)

*/
TEST_CASE( "156" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/156.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 58 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 7 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::RawHtml );
	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
	auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
	REQUIRE( t1->text() == u8"(" );
	REQUIRE( !t1->isSpaceBefore() );
	REQUIRE( !t1->isSpaceAfter() );
	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::RawHtml );
	REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
	auto t2 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );
	REQUIRE( t2->text() == u8"link" );
	REQUIRE( !t2->isSpaceBefore() );
	REQUIRE( !t2->isSpaceAfter() );
	REQUIRE( p->items().at( 5 )->type() == MD::ItemType::RawHtml );
	REQUIRE( p->items().at( 6 )->type() == MD::ItemType::Text );
	auto t3 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 6 ).get() );
	REQUIRE( t3->text() == u8")" );
	REQUIRE( !t3->isSpaceBefore() );
	REQUIRE( t3->isSpaceAfter() );
}

/*
<br><pre>
<b>To get started, [view our webpage](https://cockatrice.github.io/)</b><br>
</pre><br>

*/
TEST_CASE( "157" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/157.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 9 );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 9 );
	REQUIRE( p->endLine() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT > * > ( p->items().at( 0 ).get() );
		REQUIRE( h->text() == u8"<br>" );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 0 );
		REQUIRE( h->endColumn() == 3 );
		REQUIRE( h->endLine() == 0 );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT > * > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<pre>" );
		REQUIRE( h->startColumn() == 4 );
		REQUIRE( h->startLine() == 0 );
		REQUIRE( h->endColumn() == 8 );
		REQUIRE( h->endLine() == 0 );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT > * > ( p->items().at( 2 ).get() );
		REQUIRE( h->text() == u8"<b>" );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 1 );
		REQUIRE( h->endColumn() == 2 );
		REQUIRE( h->endLine() == 1 );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT > * > ( p->items().at( 3 ).get() );
		REQUIRE( t->text() == u8"To get started," );
		REQUIRE( t->startColumn() == 3 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 18 );
		REQUIRE( t->endLine() == 1 );
		REQUIRE( !t->isSpaceBefore() );
		REQUIRE( t->isSpaceAfter() );
	}

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT > * > ( p->items().at( 4 ).get() );
		REQUIRE( l->text() == u8"view our webpage" );
		REQUIRE( l->url() == u8"https://cockatrice.github.io/" );
		REQUIRE( l->startColumn() == 19 );
		REQUIRE( l->startLine() == 1 );
		REQUIRE( l->endColumn() == 67 );
		REQUIRE( l->endLine() == 1 );
	}

	{
		REQUIRE( p->items().at( 5 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT > * > ( p->items().at( 5 ).get() );
		REQUIRE( h->text() == u8"</b>" );
		REQUIRE( h->startColumn() == 68 );
		REQUIRE( h->startLine() == 1 );
		REQUIRE( h->endColumn() == 71 );
		REQUIRE( h->endLine() == 1 );
	}

	{
		REQUIRE( p->items().at( 6 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT > * > ( p->items().at( 6 ).get() );
		REQUIRE( h->text() == u8"<br>" );
		REQUIRE( h->startColumn() == 72 );
		REQUIRE( h->startLine() == 1 );
		REQUIRE( h->endColumn() == 75 );
		REQUIRE( h->endLine() == 1 );
	}

	{
		REQUIRE( p->items().at( 7 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT > * > ( p->items().at( 7 ).get() );
		REQUIRE( h->text() == u8"</pre>" );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 2 );
		REQUIRE( h->endColumn() == 5 );
		REQUIRE( h->endLine() == 2 );
	}

	{
		REQUIRE( p->items().at( 8 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT > * > ( p->items().at( 8 ).get() );
		REQUIRE( h->text() == u8"<br>" );
		REQUIRE( h->startColumn() == 6 );
		REQUIRE( h->startLine() == 2 );
		REQUIRE( h->endColumn() == 9 );
		REQUIRE( h->endLine() == 2 );
	}
}

/*
<br><pre
<b>To get started, [view our webpage](https://cockatrice.github.io/)</b><br>
</pre><br>

*/
TEST_CASE( "158" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/158.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 9 );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 9 );
	REQUIRE( p->endLine() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT > * > ( p->items().at( 0 ).get() );
		REQUIRE( h->text() == u8"<br>" );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 0 );
		REQUIRE( h->endColumn() == 3 );
		REQUIRE( h->endLine() == 0 );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT > * > ( p->items().at( 1 ).get() );
		REQUIRE( t->text() == u8"<pre" );
		REQUIRE( t->startColumn() == 4 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 7 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( !t->isSpaceBefore() );
		REQUIRE( t->isSpaceAfter() );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT > * > ( p->items().at( 2 ).get() );
		REQUIRE( h->text() == u8"<b>" );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 1 );
		REQUIRE( h->endColumn() == 2 );
		REQUIRE( h->endLine() == 1 );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT > * > ( p->items().at( 3 ).get() );
		REQUIRE( t->text() == u8"To get started," );
		REQUIRE( t->startColumn() == 3 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 18 );
		REQUIRE( t->endLine() == 1 );
		REQUIRE( !t->isSpaceBefore() );
		REQUIRE( t->isSpaceAfter() );
	}

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT > * > ( p->items().at( 4 ).get() );
		REQUIRE( l->text() == u8"view our webpage" );
		REQUIRE( l->url() == u8"https://cockatrice.github.io/" );
		REQUIRE( l->startColumn() == 19 );
		REQUIRE( l->startLine() == 1 );
		REQUIRE( l->endColumn() == 67 );
		REQUIRE( l->endLine() == 1 );
	}

	{
		REQUIRE( p->items().at( 5 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT > * > ( p->items().at( 5 ).get() );
		REQUIRE( h->text() == u8"</b>" );
		REQUIRE( h->startColumn() == 68 );
		REQUIRE( h->startLine() == 1 );
		REQUIRE( h->endColumn() == 71 );
		REQUIRE( h->endLine() == 1 );
	}

	{
		REQUIRE( p->items().at( 6 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT > * > ( p->items().at( 6 ).get() );
		REQUIRE( h->text() == u8"<br>" );
		REQUIRE( h->startColumn() == 72 );
		REQUIRE( h->startLine() == 1 );
		REQUIRE( h->endColumn() == 75 );
		REQUIRE( h->endLine() == 1 );
	}

	{
		REQUIRE( p->items().at( 7 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT > * > ( p->items().at( 7 ).get() );
		REQUIRE( h->text() == u8"</pre>" );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 2 );
		REQUIRE( h->endColumn() == 5 );
		REQUIRE( h->endLine() == 2 );
	}

	{
		REQUIRE( p->items().at( 8 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT > * > ( p->items().at( 8 ).get() );
		REQUIRE( h->text() == u8"<br>" );
		REQUIRE( h->startColumn() == 6 );
		REQUIRE( h->startLine() == 2 );
		REQUIRE( h->endColumn() == 9 );
		REQUIRE( h->endLine() == 2 );
	}
}

/*
<pre
</pre>

*/
TEST_CASE( "159" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/159.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<pre\n</pre>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 5 );
	REQUIRE( h->endLine() == 1 );
}

/*
[^pdfium_docs]: Unfortunately, no recent HTML-rendered docs are available for PDFium at the moment.

<!-- TODO write something about weakref.finalize(); add example on creating a C page array -->

*/
TEST_CASE( "160" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/160.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!-- TODO write something about weakref.finalize(); "
		"add example on creating a C page array -->" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 2 );
	REQUIRE( h->endColumn() == 93 );
	REQUIRE( h->endLine() == 2 );
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
TEST_CASE( "161" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/161.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->startColumn() == 2 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 6 );
	REQUIRE( l->endLine() == 8 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( i->items().size() == 2 );
		REQUIRE( i->startColumn() == 2 );
		REQUIRE( i->startLine() == 0 );
		REQUIRE( i->endColumn() == 6 );
		REQUIRE( i->endLine() == 8 );
		REQUIRE( i->delim() == MD::WithPosition{ 2, 0, 2, 0 } );

		REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 4 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 42 );
		REQUIRE( p->endLine() == 0 );

		REQUIRE( i->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( i->items().at( 1 ).get() );
		REQUIRE( c->startColumn() == 4 );
		REQUIRE( c->startLine() == 2 );
		REQUIRE( c->endColumn() == 31 );
		REQUIRE( c->endLine() == 7 );
		REQUIRE( c->text() == u8"- name: ...\n"
			"  uses: conda-incubator/setup-miniconda@v2\n"
			"  with:\n"
			"    # ... your options\n"
			"    channels: bblanchon,pypdfium2-team\n"
			"    channel-priority: strict" );
	}
}

/*
<!--
- -->

- list

*/
TEST_CASE( "162" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/162.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 4 );
	REQUIRE( h->endLine() == 1 );
	REQUIRE( h->text() == u8"<!--\n- -->" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 3 );
	REQUIRE( l->endColumn() == 5 );
	REQUIRE( l->endLine() == 3 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( i->startColumn() == 0 );
	REQUIRE( i->startLine() == 3 );
	REQUIRE( i->endColumn() == 5 );
	REQUIRE( i->endLine() == 3 );
	REQUIRE( i->delim() == MD::WithPosition{ 0, 3, 0, 3 } );
	REQUIRE( i->items().size() == 1 );
	REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 0 ).get() );
	REQUIRE( p->startColumn() == 2 );
	REQUIRE( p->startLine() == 3 );
	REQUIRE( p->endColumn() == 5 );
	REQUIRE( p->endLine() == 3 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 2 );
	REQUIRE( t->startLine() == 3 );
	REQUIRE( t->endColumn() == 5 );
	REQUIRE( t->endLine() == 3 );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"list" );
}

/*
<!--
> -->
>
> list

*/
TEST_CASE( "163" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/163.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 4 );
	REQUIRE( h->endLine() == 1 );
	REQUIRE( h->text() == u8"<!--\n> -->" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( b->startColumn() == 0 );
	REQUIRE( b->startLine() == 2 );
	REQUIRE( b->endColumn() == 5 );
	REQUIRE( b->endLine() == 3 );
	REQUIRE( b->delims() == MD::Blockquote< TRAIT >::Delims{ { 0, 2, 0, 2 }, { 0, 3, 0, 3 } } );
	REQUIRE( b->items().size() == 1 );
	REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
	REQUIRE( p->startColumn() == 2 );
	REQUIRE( p->startLine() == 3 );
	REQUIRE( p->endColumn() == 5 );
	REQUIRE( p->endLine() == 3 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 2 );
	REQUIRE( t->startLine() == 3 );
	REQUIRE( t->endColumn() == 5 );
	REQUIRE( t->endLine() == 3 );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"list" );
}

/*
<!--
- -->
-
- list

*/
TEST_CASE( "164" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/164.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 4 );
	REQUIRE( h->endLine() == 1 );
	REQUIRE( h->text() == u8"<!--\n- -->" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 2 );
	REQUIRE( l->endColumn() == 5 );
	REQUIRE( l->endLine() == 3 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( i->startColumn() == 0 );
	REQUIRE( i->startLine() == 3 );
	REQUIRE( i->endColumn() == 5 );
	REQUIRE( i->endLine() == 3 );
	REQUIRE( i->delim() == MD::WithPosition{ 0, 3, 0, 3 } );
	REQUIRE( i->items().size() == 1 );
	REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 0 ).get() );
	REQUIRE( p->startColumn() == 2 );
	REQUIRE( p->startLine() == 3 );
	REQUIRE( p->endColumn() == 5 );
	REQUIRE( p->endLine() == 3 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 2 );
	REQUIRE( t->startLine() == 3 );
	REQUIRE( t->endColumn() == 5 );
	REQUIRE( t->endLine() == 3 );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"list" );
}

/*
<a href="www.google.com">
Google
</a>
-

<a href="www.google.com">Google</a>
-

*/
TEST_CASE( "165" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/165.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 0 );
	REQUIRE( h->endLine() == 3 );
	REQUIRE( h->text() == u8"<a href=\"www.google.com\">\nGoogle\n</a>\n-" );

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 5 );
		REQUIRE( h->endColumn() == 0 );
		REQUIRE( h->endLine() == 6 );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 5 );
		REQUIRE( p->endColumn() == 34 );
		REQUIRE( p->endLine() == 5 );
		REQUIRE( p->items().size() == 3 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::RawHtml );
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::RawHtml );
	}
}

/*
heading
---
    code


    code

*/
TEST_CASE( "166" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/166.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 2 );
	REQUIRE( h->endLine() == 1 );
	REQUIRE( h->delim() == MD::WithPosition{ 0, 1, 2, 1 } );
	REQUIRE( h->level() == 2 );
	REQUIRE( h->text().get() );
	auto p = h->text().get();
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 6 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"heading" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 6 );
	REQUIRE( t->endLine() == 0 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( c->startColumn() == 4 );
	REQUIRE( c->startLine() == 2 );
	REQUIRE( c->endColumn() == 7 );
	REQUIRE( c->endLine() == 5 );
	REQUIRE( c->text() == u8"code\n\n\ncode" );
	REQUIRE( !c->isInlined() );
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
TEST_CASE( "167" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/167.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 8 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 0 );
		REQUIRE( h->endColumn() == 9 );
		REQUIRE( h->endLine() == 0 );
		REQUIRE( h->text() == u8"<!-- c -->" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 3 );
		REQUIRE( h->endColumn() == 1 );
		REQUIRE( h->endLine() == 4 );
		REQUIRE( h->text() == u8"<?php\n?>" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 7 );
		REQUIRE( h->endColumn() == 5 );
		REQUIRE( h->endLine() == 8 );
		REQUIRE( h->text() == u8"<pre>\n</pre>" );
	}

	{
		REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 4 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 11 );
		REQUIRE( h->endColumn() == 1 );
		REQUIRE( h->endLine() == 12 );
		REQUIRE( h->text() == u8"<?php\n?>" );
	}

	{
		REQUIRE( doc->items().at( 5 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 5 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 13 );
		REQUIRE( h->endColumn() == 0 );
		REQUIRE( h->endLine() == 14 );
		REQUIRE( h->delim() == MD::WithPosition{ 0, 14, 0, 14 } );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 13 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 13 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 13 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 13 );
	}

	{
		REQUIRE( doc->items().at( 6 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 6 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 16 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 16 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 16 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 16 );
	}

	{
		REQUIRE( doc->items().at( 7 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 7 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 17 );
		REQUIRE( h->endColumn() == 1 );
		REQUIRE( h->endLine() == 18 );
		REQUIRE( h->text() == u8"<?php\n?>" );
	}
}

/*
| table |
| ----- |
| <img src="img/img.png"> |

*/
TEST_CASE( "168" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/168.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 26 );
	REQUIRE( t->endLine() == 2 );

	REQUIRE( t->columnsCount() == 1 );
	REQUIRE( t->rows().size() == 2 );

	auto r0 = t->rows().at( 0 );
	REQUIRE( r0->startColumn() == 0 );
	REQUIRE( r0->startLine() == 0 );
	REQUIRE( r0->endColumn() == 8 ) ;
	REQUIRE( r0->endLine() == r0->startLine() );

	REQUIRE( r0->type() == MD::ItemType::TableRow );

	REQUIRE( r0->cells().size() == 1 );

	REQUIRE( r0->cells().at( 0 )->type() == MD::ItemType::TableCell );
	auto c0 = static_cast< MD::TableCell< TRAIT >* > ( r0->cells().at( 0 ).get() );
	REQUIRE( c0->startColumn() == 0 );
	REQUIRE( c0->startLine() == r0->startLine() );
	REQUIRE( c0->endColumn() == r0->endColumn() ) ;
	REQUIRE( c0->endLine() == c0->startLine() );

	REQUIRE( c0->items().size() == 1 );
	REQUIRE( c0->items().at( 0 )->type() == MD::ItemType::Text );

	auto t0 = static_cast< MD::Text< TRAIT >* > ( c0->items().at( 0 ).get() );
	REQUIRE( t0->startColumn() == 2 );
	REQUIRE( t0->startLine() == c0->startLine() );
	REQUIRE( t0->endColumn() == 6 );
	REQUIRE( t0->endLine() == t0->startLine() );

	REQUIRE( t0->text() == u8"table" );

	auto r1 = t->rows().at( 1 );
	REQUIRE( r1->startColumn() == 0 );
	REQUIRE( r1->startLine() == 2 );
	REQUIRE( r1->endColumn() == 26 ) ;
	REQUIRE( r1->endLine() == r1->startLine() );

	REQUIRE( r1->type() == MD::ItemType::TableRow );

	REQUIRE( r1->cells().size() == 1 );

	REQUIRE( r1->cells().at( 0 )->type() == MD::ItemType::TableCell );
	auto c1 = static_cast< MD::TableCell< TRAIT >* > ( r1->cells().at( 0 ).get() );
	REQUIRE( c1->startColumn() == 0 );
	REQUIRE( c1->startLine() == r1->startLine() );
	REQUIRE( c1->endColumn() == r1->endColumn() ) ;
	REQUIRE( c1->endLine() == c1->startLine() );

	REQUIRE( c1->items().size() == 1 );
	REQUIRE( c1->items().at( 0 )->type() == MD::ItemType::RawHtml );

	auto h1 = static_cast< MD::RawHtml< TRAIT >* > ( c1->items().at( 0 ).get() );
	REQUIRE( h1->startColumn() == 2 );
	REQUIRE( h1->startLine() == c1->startLine() );
	REQUIRE( h1->endColumn() == 24 );
	REQUIRE( h1->endLine() == c1->startLine() );

	REQUIRE( h1->text() == u8"<img src=\"img/img.png\">" );
}

/*
| table |
| ----- |
| <pre>code</pre> |

*/
TEST_CASE( "169" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/169.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 18 );
	REQUIRE( t->endLine() == 2 );

	REQUIRE( t->columnsCount() == 1 );
	REQUIRE( t->rows().size() == 2 );

	auto r0 = t->rows().at( 0 );
	REQUIRE( r0->startColumn() == 0 );
	REQUIRE( r0->startLine() == 0 );
	REQUIRE( r0->endColumn() == 8 ) ;
	REQUIRE( r0->endLine() == r0->startLine() );

	REQUIRE( r0->type() == MD::ItemType::TableRow );

	REQUIRE( r0->cells().size() == 1 );

	REQUIRE( r0->cells().at( 0 )->type() == MD::ItemType::TableCell );
	auto c0 = static_cast< MD::TableCell< TRAIT >* > ( r0->cells().at( 0 ).get() );
	REQUIRE( c0->startColumn() == 0 );
	REQUIRE( c0->startLine() == r0->startLine() );
	REQUIRE( c0->endColumn() == r0->endColumn() ) ;
	REQUIRE( c0->endLine() == c0->startLine() );

	REQUIRE( c0->items().size() == 1 );
	REQUIRE( c0->items().at( 0 )->type() == MD::ItemType::Text );

	auto t0 = static_cast< MD::Text< TRAIT >* > ( c0->items().at( 0 ).get() );
	REQUIRE( t0->startColumn() == 2 );
	REQUIRE( t0->startLine() == c0->startLine() );
	REQUIRE( t0->endColumn() == 6 );
	REQUIRE( t0->endLine() == t0->startLine() );

	REQUIRE( t0->text() == u8"table" );

	auto r1 = t->rows().at( 1 );
	REQUIRE( r1->startColumn() == 0 );
	REQUIRE( r1->startLine() == 2 );
	REQUIRE( r1->endColumn() == 18 ) ;
	REQUIRE( r1->endLine() == r1->startLine() );

	REQUIRE( r1->type() == MD::ItemType::TableRow );

	REQUIRE( r1->cells().size() == 1 );

	REQUIRE( r1->cells().at( 0 )->type() == MD::ItemType::TableCell );
	auto c1 = static_cast< MD::TableCell< TRAIT >* > ( r1->cells().at( 0 ).get() );
	REQUIRE( c1->startColumn() == 0 );
	REQUIRE( c1->startLine() == r1->startLine() );
	REQUIRE( c1->endColumn() == r1->endColumn() ) ;
	REQUIRE( c1->endLine() == c1->startLine() );

	REQUIRE( c1->items().size() == 1 );
	REQUIRE( c1->items().at( 0 )->type() == MD::ItemType::RawHtml );

	auto h1 = static_cast< MD::RawHtml< TRAIT >* > ( c1->items().at( 0 ).get() );
	REQUIRE( h1->startColumn() == 2 );
	REQUIRE( h1->startLine() == c1->startLine() );
	REQUIRE( h1->endColumn() == 16 );
	REQUIRE( h1->endLine() == c1->startLine() );

	REQUIRE( h1->text() == u8"<pre>code</pre>" );
}

/*
Supported codecs:
| Format       |  Decoders        |  Encoders           |
|:-------------|:----------------:|:-------------------:|
| JPEG         | libjpeg(-turbo)  | libjpeg(-turbo)     |

*/
TEST_CASE( "170" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/170.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 16 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 16 );
	REQUIRE( t->endLine() == 0 );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Supported codecs:" );

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Table );
		auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 56 );
		REQUIRE( t->endLine() == 3 );

		REQUIRE( t->columnsCount() == 3 );
		REQUIRE( t->rows().size() == 2 );
	}
}

/*
`code
| table` |
| --- |
| data |

*/
TEST_CASE( "171" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/171.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 4 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 4 );
	REQUIRE( t->endLine() == 0 );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"`code" );

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Table );
		auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 7 );
		REQUIRE( t->endLine() == 3 );

		REQUIRE( t->columnsCount() == 1 );
		REQUIRE( t->rows().size() == 2 );
	}
}

/*
<?php
| table` |
| --- |
| data |
?>

*/
TEST_CASE( "172" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/172.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 1 );
	REQUIRE( h->endLine() == 4 );
	REQUIRE( h->text() == u8"<?php\n| table` |\n| --- |\n| data |\n?>" );
}

/*
$$
| table |
| --- |
| data |
$$

*/
TEST_CASE( "173" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/173.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 1 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 1 );
	REQUIRE( t->endLine() == 0 );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"$$" );

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Table );
		auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 1 );
		REQUIRE( t->endLine() == 4 );

		REQUIRE( t->columnsCount() == 1 );
		REQUIRE( t->rows().size() == 3 );
	}
}

/*
Text
***
| table |
| --- |
| data |

*/
TEST_CASE( "174" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/174.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 0 );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Table );
		auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 2 );
		REQUIRE( t->endColumn() == 7 );
		REQUIRE( t->endLine() == 4 );

		REQUIRE( t->columnsCount() == 1 );
		REQUIRE( t->rows().size() == 2 );
	}
}

/*
[link](www.google.com)
| table |
| ----- |
| data  |

*/
TEST_CASE( "175" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/175.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 21 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Table );
		auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 8 );
		REQUIRE( t->endLine() == 3 );

		REQUIRE( t->columnsCount() == 1 );
		REQUIRE( t->rows().size() == 2 );
	}
}

/*
[link](www.google.com
| table |
| ----- |
| data  |

*/
TEST_CASE( "176" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/176.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 20 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Table );
		auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 8 );
		REQUIRE( t->endLine() == 3 );

		REQUIRE( t->columnsCount() == 1 );
		REQUIRE( t->rows().size() == 2 );
	}
}

/*
*text
| table* |
| ----- |
| data  |

*/
TEST_CASE( "177" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/177.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 4 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"*text" );

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Table );
		auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 8 );
		REQUIRE( t->endLine() == 3 );

		REQUIRE( t->columnsCount() == 1 );
		REQUIRE( t->rows().size() == 2 );
	}
}

/*
<?php


data


?>

*/
TEST_CASE( "178" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/178.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 1 );
	REQUIRE( h->endLine() == 6 );
	REQUIRE( h->text() == u8"<?php\n\n\ndata\n\n\n?>" );
}

/*
<form>

<form>

*/
TEST_CASE( "179" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/179.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 0 );
		REQUIRE( h->endColumn() == 5 );
		REQUIRE( h->endLine() == 0 );
		REQUIRE( h->text() == u8"<form>" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 2 );
		REQUIRE( h->endColumn() == 5 );
		REQUIRE( h->endLine() == 2 );
		REQUIRE( h->text() == u8"<form>" );
	}
}

/*
* The

      ```
      # This
      ```

    The
*/
TEST_CASE( "180" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/180.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 6 );
	REQUIRE( l->endLine() == 6 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( i->items().size() == 3 );
	REQUIRE( i->startColumn() == 0 );
	REQUIRE( i->startLine() == 0 );
	REQUIRE( i->endColumn() == 6 );
	REQUIRE( i->endLine() == 6 );
	REQUIRE( i->delim() == MD::WithPosition{ 0, 0, 0, 0 } );

	REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 0 ).get() );
	REQUIRE( p->startColumn() == 2 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 4 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( i->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( i->items().at( 1 ).get() );
	REQUIRE( c->startColumn() == 6 );
	REQUIRE( c->startLine() == 2 );
	REQUIRE( c->endColumn() == 8 );
	REQUIRE( c->endLine() == 4 );
	REQUIRE( c->text() == u8"```\n# This\n```" );

	{
		REQUIRE( i->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 2 ).get() );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 6 );
		REQUIRE( p->endColumn() == 6 );
		REQUIRE( p->endLine() == 6 );
	}
}

/*
[<picture><source media="(prefers-color-scheme: dark)" srcset="https://steampipe.io/images/steampipe-color-logo-and-wordmark-with-white-bubble.svg"><source media="(prefers-color-scheme: light)" srcset="https://steampipe.io/images/steampipe-color-logo-and-wordmark-with-white-bubble.svg"><img width="67%" alt="Steampipe Logo" src="https://steampipe.io/images/steampipe-color-logo-and-wordmark-with-white-bubble.svg"></picture>](https://steampipe.io?utm_id=gspreadme&utm_source=github&utm_medium=repo&utm_campaign=github&utm_content=readme)

*/
TEST_CASE( "181" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/181.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 537 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().front()->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().front().get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 537 );
	REQUIRE( l->endLine() == 0 );
	REQUIRE( l->url() == u8"https://steampipe.io?utm_id=gspreadme&utm_source=github&"
		"utm_medium=repo&utm_campaign=github&utm_content=readme" );
	REQUIRE( l->p().get() );
	auto pp = l->p().get();
	REQUIRE( pp->startColumn() == 1 );
	REQUIRE( pp->startLine() == 0 );
	REQUIRE( pp->endColumn() == 424 );
	REQUIRE( pp->endLine() == 0 );
	REQUIRE( pp->items().size() == 5 );
	for( int i = 0; i < pp->items().size(); ++i )
		REQUIRE( pp->items().at( i )->type() == MD::ItemType::RawHtml );
}

/*
text

# h
## hh
### hhh

*/
TEST_CASE( "182" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/182.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 5 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 0 );

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 2 );
		REQUIRE( h->endColumn() == 2 );
		REQUIRE( h->endLine() == 2 );
		REQUIRE( h->delim() == MD::WithPosition{ 0, 2, 0, 2 } );
		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 2 );
		REQUIRE( p->endColumn() == 2 );
		REQUIRE( p->endLine() == 2 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 3 );
		REQUIRE( h->endColumn() == 4 );
		REQUIRE( h->endLine() == 3 );
		REQUIRE( h->delim() == MD::WithPosition{ 0, 3, 1, 3 } );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->startColumn() == 3 );
		REQUIRE( p->startLine() == 3 );
		REQUIRE( p->endColumn() == 4 );
		REQUIRE( p->endLine() == 3 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	}

	{
		REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 4 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 4 );
		REQUIRE( h->endColumn() == 6 );
		REQUIRE( h->endLine() == 4 );
		REQUIRE( h->delim() == MD::WithPosition{ 0, 4, 2, 4 } );
		REQUIRE( h->level() == 3 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->startColumn() == 4 );
		REQUIRE( p->startLine() == 4 );
		REQUIRE( p->endColumn() == 6 );
		REQUIRE( p->endLine() == 4 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	}
}

/*
# h
## hh
### hhh

*/
TEST_CASE( "183" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/183.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 0 );
		REQUIRE( h->endColumn() == 2 );
		REQUIRE( h->endLine() == 0 );
		REQUIRE( h->delim() == MD::WithPosition{ 0, 0, 0, 0 } );
		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 2 );
		REQUIRE( p->endLine() == 0 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 1 );
		REQUIRE( h->endColumn() == 4 );
		REQUIRE( h->endLine() == 1 );
		REQUIRE( h->delim() == MD::WithPosition{ 0, 1, 1, 1 } );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->startColumn() == 3 );
		REQUIRE( p->startLine() == 1 );
		REQUIRE( p->endColumn() == 4 );
		REQUIRE( p->endLine() == 1 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 2 );
		REQUIRE( h->endColumn() == 6 );
		REQUIRE( h->endLine() == 2 );
		REQUIRE( h->delim() == MD::WithPosition{ 0, 2, 2, 2 } );
		REQUIRE( h->level() == 3 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->startColumn() == 4 );
		REQUIRE( p->startLine() == 2 );
		REQUIRE( p->endColumn() == 6 );
		REQUIRE( p->endLine() == 2 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	}
}
