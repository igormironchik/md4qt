
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2022 Igor Mironchik

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
