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

Approximate benchmark with `md4c` says, that `md4qt` is slower only in 10 times.
But you will get complete tree structure of the Markdown document with all
major extensions.

# Q/A

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
