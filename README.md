[![Build](https://github.com/igormironchik/md4qt/workflows/build/badge.svg)](https://github.com/igormironchik/md4qt/actions)[![codecov](https://codecov.io/gh/igormironchik/md4qt/branch/main/graph/badge.svg)](https://codecov.io/gh/igormironchik/md4qt)[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

`md4qt` is a header-only C++ library for parsing Markdown.

`md4qt` supports CommonMark 0.30 Spec, and some GitHub extensions, such as
tables, footnotes, tasks lists, strikethroughs, LaTeX Math injections.

`md4qt` can be built with Qt6 or with ICU.

This library parses Markdown into tree structure.

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
[cmark-gfm](https://github.com/github/cmark-gfm) says, that Qt6 version of `md4qt` is slower ~5-7 times.
But you will get complete C++ tree structure of the Markdown document with all
major extensions. Conclusion why it's slower you can read [here](tests/md_benchmark/README.md).

# Q/A

**Why another AST Markdown parser?**

 * When I wrote this library I knew about `md4c` parser, but not about `cmark-gfm`.
 `md4c` was not suitable for my purposes, whereas `cmark-gfm` could do
 everything I needed. But God did it so, so I wrote `md4qt` and only later
 knew about `cmark-gfm`. Ok, code is written and tested. Let it be.

   What I can say yet, is that this library is C++. And for some people can be
   easier to use C++ code instead of C with freeing memory by hands. Qt do things
   easier by handling text encoding... So let it be, guys.

   And one more cherry on the cake - `md4qt` can parse Markdown recursively.
   What it is described bellow.

**What should I know about links in the document?**

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

**What is the second argument of `MD::Parser::parse()`?**

 * Second argument of `MD::Parser::parse()` is a flag that tells to the
parser to process Markdown files recursively or no. If parsing is recursive
then if in the targeted Markdown file exist links to other Markdown files,
then they will be parsed too and will exist in the resulting document.

**What is an `MD::Anchor`?**

 * As `md4qt` supports recursive Markdown parsing, then in the resulting
document can be represented more then one Markdown file. Each file in the
document starts with `MD::Anchor`, it just shows that during traverse through
the document you reached new file.

**Does the library throw exceptions?**

 * No. This library doesn't use exceptions. Any text is a valid Markdown, so I
don't need to inform user about errors. Qt itself doesn't use exceptions too.
So you can caught only standard C++ exceptions, like `std::bad_alloc`, for
example. Possibly with `MD::UnicodeStringTrait` you will catch more standard
exceptions, possibly I missed something somewhere, but I tried to negotiate
all possible exceptions.

**Why `MD::Parser` and `MD::Document` are templates?**

 * Since version `2.0.0` `md4qt` can be built not only with `Qt6`, but with
`STL` too. The code of the parser is the same in both cases. I just added two
ready traits to support different C++ worlds. With `STL` I use `ICU` library
for Unicode handling, and `uriparser` library to parse and check URLs.
These dependencies can be installed with the Conan package manager. I did
an attempt to make pure C++ `STL` solution, but handling Unicode in C++
standard is a pain, it's still not enough powerful to implement all
things without very big effort. But if somebody want to make such pure
C++ `STL` trait - you are more then WELCOME with such PR.

**So, how can I use `md4qt` with `Qt6` and `ICU`?**

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
   
**`ICU` is slower then `Qt6`? Really?**

 * Don't believe anybody, just build built-in `md_benchamrk` and have a
look. Dry numbers says, that `Qt6` `QString` ~2 times faster
`icu::UnicodeString` in such tasks. Markdown parsing implies to check
every symbol, and tied to use access to every character in the string
with `operator [] (...)`, or member `at(...)`. I do it very often in the
parser's code and profiler says that most of the run-time is spent
on such operations. `QString` just more optimized for access separate
character then `icu::UnicodeString`...

**Why is parsing wrong on Windows with `std::ifstream`?**

 * Such problem can occur on Windows with MSVC if you open file in text
mode, so for `MD::Parser` always open `std::ifstream` with `std::ios::binary`
flag. And yes, I expect to receive UTF-8 encoded content...

**How can I convert `MD::Document` into `HTML`?**

 * In the `main` branch were made commits with implementation of
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
   
   But this functionality is not tested yet.
   
**I need to know positions in the `Markdown` file of blocks/elements. How
can I achieve this?**

 * Done in version `2.0.5`. This version is not released yet, but it's
almost done (done, but not fully tested), and can be used from `main` branch.

**Why is implementation of the following method so terrible?**

 *
   ```cpp
   template< class Trait >
   inline void
   Parser< Trait >::parse( StringListStream< Trait > & stream,
       std::shared_ptr< Block< Trait > > parent,
       std::shared_ptr< Document< Trait > > doc,
       typename Trait::StringList & linksToParse,
       const typename Trait::String & workingPath,
       const typename Trait::String & fileName,
       bool collectRefLinks, bool top );
   ```
   
   Implementation of the above method is so ugly because `md4qt` was started from
   the simplest `Markdown` parser, that didn't follow all features of `CommonMark`.
   I started to follow standard and did a lot of iterations to suit all cases, but I
   wasn't a professional at `Markdown` syntax, and did parsing with splitting text
   on blocks first. I caught a lot of problems with such approach, look, for example,
   at handling `HTML`.
   
   But this code works correctly, it tested. OK, OK, I can rewrite the parsing, code will
   be cleaner, more understandable, easier to read. But performance will not be increased,
   it will be the same (+/-). I just don't want to spend a lot of time to rewrite something
   that works to be easier to understand.
   
   Possibly I will do it later, but now for my own needs - it's OK for me. I don't see
   a lot of users of `md4qt` library, as `cmark-gfm` already exists. So, let it be so for now.
   It fast enough, it's simple to use, and it works correctly.
