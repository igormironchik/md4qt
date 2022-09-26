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

Approximate benchmark with `md4c` says, that `md4qt` is slower only in 5.5 times.
But you will get complete tree structure of the Markdown document with all
major extensions. Conclusion why it's so you can read [here](tests/md_benchmark/README.md).

Is it so important to you and you don't want to pay extra ~1500 microseconds?
Is it valuable to you to parse with `md4c` by 365 microseonds, instead of
1970 microseconds with `md4qt`? Yes? And it's not a problem for you that,
for example, complex footnotes will be wrongly parsed? Then `md4c` is better for you.

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

**What is the second argument of `MD::Parser::parse()`?**

 * Second argument of `MD::Parser::parse()` is a flag that tells to the
parser to process Markdown files recursively or not. If parsing is recursive
then if in the targeted Markdown file exist links to other Markdown files,
then they will be parsed too and will exist in the resulting document.

**What is an `MD::Anchor`?**

 * As `md4qt` supports recursive Markdown parsing, then in the resulting
document can be represented more then one Markdown file. Each file in the
document starts with `MD::Anchor`, it's just show that during traverse through
the document you reached new file.
