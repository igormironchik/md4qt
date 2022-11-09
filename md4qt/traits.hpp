
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

#ifndef MD4QT_MD_TRAITS_HPP_INCLUDED
#define MD4QT_MD_TRAITS_HPP_INCLUDED

// C++ include.
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string_view>


#ifdef MD4QT_QT_SUPPORT

// Qt include.
#include <QString>
#include <QSharedPointer>
#include <QHash>
#include <QVector>
#include <QTextStream>
#include <QStringList>
#include <QStringView>

#endif


namespace MD {

//
// StdStringTrait
//

//! Trait to use this library with std::string.
struct StdStringTrait {
	template< class T >
	using SharedPointer = std::shared_ptr< T >;

	template< class T >
	using Vector = std::vector< T >;

	template< class T, class U >
	using Map = std::unordered_map< T, U >;

	using String = std::string;

	using Char = char;

	using TextStream = std::istream;

	using StringList = std::vector< String >;

	using StringView = std::string_view;

	static String utf16ToString( const char16_t * u16 )
	{
		return String();
	}
}; // struct StdStringTrait


#ifdef MD4QT_QT_SUPPORT

//
// QStringTrait
//

//! Trait to use this library with QString.
struct QStringTrait {
	template< class T >
	using SharedPointer = QSharedPointer< T >;

	template< class T >
	using Vector = QVector< T >;

	template< class T, class U >
	using Map = QHash< T, U >;

	using String = QString;

	using Char = QChar;

	using TextStream = QTextStream;

	using StringList = QStringList;

	using StringView = QStringView;

	static String utf16ToString( const char16_t * u16 )
	{
		return QString::fromUtf16( u16 );
	}
}; // struct QStringTrait

#endif

} /* namespace MD */

#endif // MD4QT_MD_TRAITS_HPP_INCLUDED
