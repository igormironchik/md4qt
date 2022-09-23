
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2022 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// CommonMark 0.30

// 2.2
#include "tabs.inl"

// 2.3 Insecure characters

// 2.4
#include "backslash.inl"

// 2.5 Entity and numeric character references
#include "entity.inl"

// 3.1 - 4.5
#include "blocks.inl"

// 4.6 HTML blocks
#include "html_blocks.inl"

// 4.7
#include "link_ref.inl"

// 4.8 - 4.9
#include "paragraphs.inl"

// 5.1
#include "blockquote.inl"

// 5.2
#include "list_items.inl"

// 5.3
#include "lists.inl"

// 6.1
#include "code_spans.inl"

// 6.2
#include "emphasis-1-10.inl"
#include "emphasis-11-17.inl"

// 6.3
#include "links-1.inl"
#include "links-2.inl"

// 6.4
#include "images.inl"

// 6.5
#include "autolinks.inl"

// 6.6 Raw HTML
#include "raw_html.inl"

// 6.7
#include "hard_line_breaks.inl"

// 6.8 - 6.9
#include "6.8-6.9.inl"
