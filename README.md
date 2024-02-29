[![Build](https://github.com/igormironchik/md4qt/workflows/build/badge.svg)](https://github.com/igormironchik/md4qt/actions)[![codecov](https://codecov.io/gh/igormironchik/md4qt/branch/main/graph/badge.svg)](https://codecov.io/gh/igormironchik/md4qt)[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

`md4qt` is a header-only C++ library for parsing Markdown.

`md4qt` supports CommonMark 0.31.2 Spec, and some GitHub extensions, such as
tables, footnotes, tasks lists, strikethroughs, LaTeX Math injections, GitHub's autolinks.

`md4qt` can be built with Qt6 or with ICU.

This library parses Markdown into tree structure.

* [Example](#example)
* [Benchmark](#benchmark)
* [Playground](#playground)
* [Q/A](#qa)
  * [Why another AST Markdown parser?](#why-another-ast-markdown-parser)
  * [What should I know about links in the document?](#what-should-i-know-about-links-in-the-document)
  * [What is the second argument of `MD::Parser::parse()`?](#what-is-the-second-argument-of-mdparserparse)
  * [What is an `MD::Anchor`?](#what-is-an-mdanchor)
  * [Does the library throw exceptions?](#does-the-library-throw-exceptions)
  * [Why `MD::Parser` and `MD::Document` are templates?](#why-mdparser-and-mddocument-are-templates)
  * [So, how can I use `md4qt` with `Qt6` and `ICU`?](#so-how-can-i-use-md4qt-with-qt6-and-icu)
  * [`ICU` is slower then `Qt6`? Really?](#icu-is-slower-then-qt6-really)
  * [Why is parsing wrong on Windows with `std::ifstream`?](#why-is-parsing-wrong-on-windows-with-stdifstream)
  * [How can I convert `MD::Document` into `HTML`?](#how-can-i-convert-mddocument-into-html)
  * [I need to know positions in the `Markdown` file of blocks/elements. How can I achieve this?](#i-need-to-know-positions-in-the-markdown-file-of-blockselements-howcan-i-achieve-this)
  * [How can I easily traverse through the `MD::Document`?](#how-can-i-easily-traverse-through-the-mddocument)
  * [Why don't you have an implementation for pure `STL` with `std::string`?](#why-dont-you-have-an-implementation-for-pure-stl-with-stdstring)
  * [Where are empty list items?](#where-are-empty-list-items)

# Example

```cpp
#define MD4QT_QT_SUPPORT
#include <md4qt/parser.hpp>

int main()
{
    MD::Parser< MD::QStringTrait > p;

    auto doc = p.parse( QStringLiteral( "your_markdown.md" ) );
	
    for( auto it = doc->items().cbegin(), last = doc->items().cend(); it != last; ++it )
    {
        switch( (*it)->type() )
        {
            case MD::ItemType::Anchor :
            {
                auto a = static_cast< MD::Anchor< MD::QStringTrait >* > ( it->get() );
                qDebug() << a->label();
            }
                break;

            default :
                break;
        }
    }

    return 0;
}
```

# Benchmark

Approximate benchmark with [md4c](https://github.com/mity/md4c) and
[cmark-gfm](https://github.com/github/cmark-gfm) says, that Qt6 version of `md4qt` is slower ~6-9 times.
But you will get complete C++ tree structure of the Markdown document with all
major extensions. Conclusion why it's slower you can read [here](tests/md_benchmark/README.md).

| [cmark-gfm](https://github.com/github/cmark-gfm) | [md4c](https://github.com/mity/md4c) | [markdown-it (Rust)](https://github.com/markdown-it-rust/markdown-it) | `md4qt` with `Qt6` |
| --- | --- | --- | --- |
| ~284 microseconds | ~427 microseconds | ~6700 microseconds | ~3394 microseconds |


> [!NOTE]
>
> *This measurement done with test file in [markdown-it (Rust)](https://github.com/markdown-it-rust/markdown-it)*
>
> *`markdown-it (Rust)` measurement done with `markdown_it::plugins::extra`*
>
> **Keep in mind that `markdown-it (Rust)` doesn't support all GFM extensions, so it's not honest
> to compare. `md4qt` supports GitHub's autolinks extension, that `markdown-it (Rust)` doesn't, and
> this requires ~500 microseconds, for example...**

# Playground

You can play with `md4qt` in action in
[`Markdown` Editor/Viewer](https://github.com/igormironchik/md-editor),
[`Markdown` to `PDF` converter](https://github.com/igormironchik/md-pdf),
binaries of which for `Windows` and `Linux` you can get
[here](https://github.com/igormironchik/markdown).

# Q/A

Why another AST Markdown parser?
---

 * When I wrote this library I knew about `md4c` parser, but not about `cmark-gfm`.
 `md4c` was not suitable for my purposes, whereas `cmark-gfm` could do
 everything I needed. But God did it so, so I wrote `md4qt` and only later
 knew about `cmark-gfm`. Ok, code is written and tested. Let it be.

   What I can say yet, is that this library is C++. And for some people can be
   easier to use C++ code instead of C with freeing memory by hands. Qt do things
   easier by handling text encoding... So let it be, guys.

   And one more cherry on the cake - `md4qt` can parse Markdown recursively.
   What it is described bellow.

What should I know about links in the document?
---

 * In some cases in Markdown link's URL is something document related. So, when
you got a `MD::Link` in the document check if the labeled links of the
document contains key with URL in the link, and if so, use URL from
labeled links, look:

   ```cpp
   MD::Link< MD::QStringTrait > * item = ...;

   QString url = item->url();

   const auto it = doc->labeledLinks().find( url );
   
   if( it != doc->labeledLinks().cend() )
       url = it->second->url();
   ```

What is the second argument of `MD::Parser::parse()`?
---

 * Second argument of `MD::Parser::parse()` is a flag that tells to the
parser to process Markdown files recursively or no. If parsing is recursive
then if in the targeted Markdown file exist links to other Markdown files,
then they will be parsed too and will exist in the resulting document.

What is an `MD::Anchor`?
---

 * As `md4qt` supports recursive Markdown parsing, then in the resulting
document can be represented more then one Markdown file. Each file in the
document starts with `MD::Anchor`, it just shows that during traverse through
the document you reached new file.

Does the library throw exceptions?
---

 * No. This library doesn't use exceptions. Any text is a valid Markdown, so I
don't need to inform user about errors. Qt itself doesn't use exceptions too.
So you can caught only standard C++ exceptions, like `std::bad_alloc`, for
example. Possibly with `MD::UnicodeStringTrait` you will catch more standard
exceptions, possibly I missed something somewhere, but I tried to negotiate
all possible exceptions.

Why `MD::Parser` and `MD::Document` are templates?
---

 * Since version `2.0.0` `md4qt` can be built not only with `Qt6`, but with
`STL` too. The code of the parser is the same in both cases. I just added two
ready traits to support different C++ worlds. With `STL` I use `ICU` library
for Unicode handling, and `uriparser` library to parse and check URLs.
These dependencies can be installed with the Conan package manager.

So, how can I use `md4qt` with `Qt6` and `ICU`?
---

 * To build with `ICU` support you need to define `MD4QT_ICU_STL_SUPPORT`
before including `md4qt/parser.hpp`. In this case you will get access to
`MD::UnicodeStringTrait`, that can be passed to `MD::Parser` as template
parameter. You will receive in dependencies `C++ STL`, `ICU` and
`uriparser`.

   To build with `Qt6` support you need to define `MD4QT_QT_SUPPORT`.
   In this case you will get access to `MD::QStringTrait` to work with
   Qt's classes and functions. In this case in dependencies you will
   receive `Qt6`.
   
   You can define both to have ability to use `md4qt` with `Qt6` and
   `ICU`.

`ICU` is slower then `Qt6`? Really?
---

 * Don't believe anybody, just build built-in `md_benchamrk` and have a
look. Dry numbers says, that `Qt6` `QString` ~2 times faster
`icu::UnicodeString` in such tasks. Markdown parsing implies to check
every symbol, and tied to use access to every character in the string
with `operator [] (...)`, or member `at(...)`. I do it very often in the
parser's code and profiler says that most of the run-time is spent
on such operations. `QString` just more optimized for access separate
character then `icu::UnicodeString`...

Why is parsing wrong on Windows with `std::ifstream`?
---

 * Such problem can occur on Windows with MSVC if you open file in text
mode, so for `MD::Parser` always open `std::ifstream` with `std::ios::binary`
flag. And yes, I expect to receive UTF-8 encoded content...

How can I convert `MD::Document` into `HTML`?
---

 * In version `2.0.5` were made commits with implementation of
`MD::toHtml()` function. You can do the following:

   ```cpp
   #define MD4QT_QT_SUPPORT
   #include <md4qt/traits.hpp>
   #include <md4qt/parser.hpp>
   #include <md4qt/html.hpp>

   int main()
   {
       MD::Parser< MD::QStringTrait > p;

       auto doc = p.parse( QStringLiteral( "your_markdown.md" ) );

       const auto html = MD::toHtml( doc );

       return 0;
   }
   ```

I need to know positions in the `Markdown` file of blocks/elements. How
can I achieve this?
---

 * Done in version `2.0.5`.

## How can I easily traverse through the `MD::Document`?

* Since version `2.6.0` in `md4qt/visitor.hpp` header implemented `MD::Visitor` interface
with which you can easily walk through the document, all you need is implement/override
virtual methods to handle that or another element in the document, like:

  ```cpp
  virtual void onHeading(
     //! Heading.
     Heading< Trait > * h ) = 0;
  ```

## Why don't you have an implementation for pure `STL` with `std::string`?

* Because of performance, I did an pure `STL` implementation where string class
was an `std::string` with some small third-party library to handle `UTF8`, and
benchmark said that the performance was like with `Qt6` `QString`, so I decided
to not support third trait. Maybe because I so lazy?

## Where are empty list items?

* I don't add empty (withouth any data in it) list items in the list. Parser do everything right,
I just decided to not add empty list items into `MD::Document`. For this reason you can even
not find whole list if it will contain only empty list items.
