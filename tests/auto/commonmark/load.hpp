
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

#pragma once

#include <md4qt/parser.hpp>
#include <md4qt/traits.hpp>

#include <string>

#ifdef MD4QT_QT_SUPPORT
#include <QString>
#endif


inline typename TRAIT::template SharedPointer< MD::Document< TRAIT > >
load_test( int n )
{
	auto fileName = std::to_string( n );

	if( fileName.size() < 3 )
		fileName.insert( 0, 3 - fileName.size(), '0' );

	fileName.insert( 0, "tests/commonmark/0.30/" );
	fileName.append( ".md" );

	MD::Parser< TRAIT > p;

	return p.parse(
#ifdef MD4QT_QT_SUPPORT
		QString::fromStdString( fileName ),
#else
		fileName,
#endif
		false );
}
