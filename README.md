[![Build](https://github.com/igormironchik/md4qt/workflows/build/badge.svg)](https://github.com/igormironchik/md4qt/actions)[![codecov](https://codecov.io/gh/igormironchik/md4qt/branch/main/graph/badge.svg)](https://codecov.io/gh/igormironchik/md4qt)[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

`md4qt` is a static C++ library for parsing Markdown with Qt.

`md4qt` supports CommonMark 0.30 Spec, and some GitHub extensions, such as
tables, footnotes, tasks lists, strikethroughs, LaTeX Math injections.

This library parses Markdown into tree structure.

# Example

```cpp
#include <md4qt/md_parser.hpp>

int main()
{
    MD::Parser p;

    auto doc = p.parse( QStringLiteral( "your_markdown.md" ) );

    return 0;
}
```

# Benchmark

Approximate benchmark with [md4c](https://github.com/mity/md4c) and
[cmark-gfm](https://github.com/github/cmark-gfm) says, that `md4qt` is slower ~5 times.
But you will get complete C++ tree structure of the Markdown document with all
major extensions. Conclusion why it's so you can read [here](tests/md_benchmark/README.md).

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
   What is it described bellow.

**Why this library tied to Qt? I want to see STL only in dependencies.**

 * This library was born in the Qt based project. Long time the library
 was not in separate project, and the code was under the GPL license.
 I decied to weak restrictions for the possible users, and separated code
 into this library, and gave it MIT license. Ok, I can template the code,
 make Qt dependency as optional, and make library header-only. Yes, it's
 not so big effort, I guess a week or two. But I don't need it. I don't
 believe that this library will become very popular and will has a lot
 of users, so I don't see a need for this job. But if you need pure STL
 solution for `std::wstring` and `std::wistream`, just let me know. Submit an
 issue, and we'll discuss.

**What should I know about links in the document?**

 * In some cases in Markdown link's URL is something document related. So, when
you got a `MD::Link` in the document check if the labeled links of the
document contains key with URL in the link, and if so, use URL from
labeled links, look:

   ```cpp
   MD::Link * item = ...;

   QString url = item->url();

   if( doc->labeledLinks().contains( url ) )
       url = doc->labeledLinks()[ url ]->url();
   ```

**What is the second argument of `MD::Parser::parse()`?**

 * Second argument of `MD::Parser::parse()` is a flag that tells to the
parser to process Markdown files recursively or not. If parsing is recursive
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
example.
