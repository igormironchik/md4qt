
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
#include <filesystem>
#include <locale>
#include <cctype>
#include <codecvt>


#ifdef MD4QT_QT_SUPPORT

// Qt include.
#include <QString>
#include <QSharedPointer>
#include <QHash>
#include <QVector>
#include <QTextStream>
#include <QStringList>
#include <QStringView>
#include <QFileInfo>

#endif


namespace MD {

//
// StdChar
//

//! Wrapper for char to be used with MD::Parser.
class StdChar {
public:
	StdChar()
		:	m_ch( 0 )
	{
	}

	StdChar( char ch )
		:	m_ch( ch )
	{
	}

	operator char () const
	{
		return m_ch;
	}

	inline bool isSpace() const
	{
		return std::isspace( static_cast< unsigned char > ( m_ch ) );
	}

	inline bool isDigit() const
	{
		return std::isdigit( static_cast< unsigned char > ( m_ch ) );
	}

	bool operator == ( const StdChar & other ) const
	{
		return m_ch == other.m_ch;
	}

private:
	char m_ch;
}; // class StdChar

//
// StdString
//

//! Wrapper for std::string to be used with MD::Parser.
class StdString final {
public:
	StdString()
	{
	}

	StdString( const std::string & str )
		:	m_str( str )
	{
	}

	StdString( const StdChar & ch )
		:	m_str( 1, (char) ch )
	{
	}

	operator std::string () const
	{
		return m_str;
	}

	StdChar operator [] ( long long int position ) const
	{
		return StdChar( m_str.at( static_cast< size_t > ( position ) ) );
	}

	void append( const StdChar & ch )
	{
		m_str.append( 1, (char) ch );
	}

	long long int size() const
	{
		return static_cast< long long int > ( m_str.size() );
	}

	StdString mid( long long int position, long long int n = -1 ) const
	{
		const auto originalLength = size();

		if( position > originalLength )
			return StdString();

		if( position < 0 )
		{
			if( n < 0 || n + position >= originalLength )
				return StdString( m_str );

			if( n + position <= 0 )
				return StdString();

			n += position;
			position = 0;
		}
		else if( static_cast< size_t > ( n ) > static_cast< size_t > ( originalLength - position ) )
			n = originalLength - position;

		if( position == 0 && n == originalLength )
			return StdString( m_str );

		return n > 0 ? StdString( std::string( m_str.c_str() + position,
			static_cast< size_t > ( n ) ) ) : StdString();
	}

	friend StdString operator + ( const StdString & s1, const StdString & s2 );

private:
	std::string m_str;
}; // class StdString

StdString operator + ( const StdString & s1, const StdString & s2 )
{
	return StdString( s1.m_str + s2.m_str );
}


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

	using String = StdString;

	using Char = StdChar;

	using TextStream = std::istream;

	using StringList = std::vector< String >;

	using StringView = std::string_view;

	//! Convert UTF-16 into trait's string.
	static String utf16ToString( const char16_t * u16 )
	{
		static std::wstring_convert< std::codecvt_utf8_utf16< char16_t >, char16_t > conv;
		return conv.to_bytes( u16 );
	}

	//! \return Does file exist.
	static bool fileExists( const String & fileName, const String & workingPath )
	{
		return std::filesystem::exists( (std::string) ( workingPath + fileName ) );
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

	//! Convert UTF-16 into trait's string.
	static String utf16ToString( const char16_t * u16 )
	{
		return QString::fromUtf16( u16 );
	}

	//! \return Does file exist.
	static bool fileExists( const String & fileName, const String & workingPath )
	{
		return QFileInfo::exists( workingPath + fileName );
	}
}; // struct QStringTrait

#endif

} /* namespace MD */

#endif // MD4QT_MD_TRAITS_HPP_INCLUDED
