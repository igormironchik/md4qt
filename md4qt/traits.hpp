
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

#ifdef MD4QT_ICU_STL_SUPPORT

// C++ include.
#include <memory>
#include <vector>
#include <map>
#include <filesystem>
#include <cctype>
#include <string>

// ICU include.
#include <unicode/unistr.h>
#include <unicode/uchar.h>

// uriparser include.
#include <uriparser/Uri.h>

#endif // MD4QT_ICU_STL_SUPPORT


#ifdef MD4QT_QT_SUPPORT

#ifndef MD4QT_ICU_STL_SUPPORT

// C++ include.
#include <memory>
#include <map>

#endif // MD4QT_ICU_STL_SUPPORT

// Qt include.
#include <QString>
#include <QVector>
#include <QTextStream>
#include <QStringList>
#include <QFileInfo>
#include <QUrl>

#endif // MD4QT_QT_SUPPORT


namespace MD {

#ifdef MD4QT_ICU_STL_SUPPORT

//
// StdChar
//

//! Wrapper for UChar32 to be used with MD::Parser.
class UnicodeChar {
public:
	UnicodeChar()
		:	m_ch( 0 )
	{
	}

	UnicodeChar( UChar32 ch )
		:	m_ch( ch )
	{
	}

	operator UChar32 () const
	{
		return m_ch;
	}

	inline bool isSpace() const
	{
		return std::isspace( static_cast< unsigned char > ( m_ch ) );
	}

	inline bool isDigit() const
	{
		return ( u_charType( m_ch ) == U_DECIMAL_DIGIT_NUMBER );
	}

	inline bool isNull() const
	{
		return m_ch == 0;
	}

	inline UChar32 unicode() const
	{
		return m_ch;
	}

	inline bool isLetter() const
	{
		const auto type = u_charType( m_ch );

		switch( type )
		{
			case U_UPPERCASE_LETTER :
			case U_LOWERCASE_LETTER :
			case U_TITLECASE_LETTER :
			case U_MODIFIER_LETTER :
			case U_OTHER_LETTER :
				return true;

			default :
				return false;
		}
	}

	inline bool isLetterOrNumber() const
	{
		return isLetter() || isDigit();
	}

	inline bool isPunct() const
	{
		const auto type = u_charType( m_ch );

		switch( type )
		{
			case U_DASH_PUNCTUATION :
			case U_START_PUNCTUATION :
			case U_END_PUNCTUATION :
			case U_CONNECTOR_PUNCTUATION :
			case U_OTHER_PUNCTUATION :
			case U_INITIAL_PUNCTUATION :
			case U_FINAL_PUNCTUATION :
				return true;

			default :
				return false;
		}
	}

	UnicodeChar toLower() const
	{
		return icu::UnicodeString( 1, m_ch, 1 ).toLower().char32At( 0 );
	}

	bool operator == ( const UnicodeChar & other ) const
	{
		return m_ch == other.m_ch;
	}

	bool operator == ( UChar32 ch ) const
	{
		return m_ch == ch;
	}

	bool operator != ( UChar32 ch ) const
	{
		return m_ch != ch;
	}

private:
	UChar32 m_ch;
}; // class StdChar

//
// StdString
//

//! Wrapper for icu::UnicodeString to be used with MD::Parser.
class UnicodeString final
	:	public icu::UnicodeString
{
public:
	UnicodeString()
	{
	}

	UnicodeString( const icu::UnicodeString & str )
		:	icu::UnicodeString( str )
	{
	}

	UnicodeString( char ch )
		:	icu::UnicodeString( (char16_t) ch )
	{
	}

	UnicodeString( const UnicodeChar & ch )
		:	icu::UnicodeString( 1, (UChar32) ch, 1 )
	{
	}

	UnicodeString( const char * str )
		:	icu::UnicodeString( icu::UnicodeString::fromUTF8( str ) )
	{
	}

	UnicodeString( const std::string & str )
		:	icu::UnicodeString( icu::UnicodeString::fromUTF8( str ) )
	{
	}

	UnicodeString( long long int count, char ch )
		:	icu::UnicodeString( count, (UChar32) ch, count )
	{
	}

	~UnicodeString() override = default;

	UnicodeChar operator [] ( long long int position ) const
	{
		return UnicodeChar( char32At( position ) );
	}

	void push_back( const UnicodeChar & ch )
	{
		icu::UnicodeString::append( (UChar32) ch );
	}

	void push_back( const UnicodeString & str )
	{
		icu::UnicodeString::append( str );
	}

	int32_t size() const
	{
		return length();
	}

	UnicodeString mid( long long int position, long long int n = -1 ) const
	{
		const auto originalLength = size();

		if( position > originalLength )
			return UnicodeString();

		if( position < 0 )
		{
			if( n < 0 || n + position >= originalLength )
				return *this;

			if( n + position <= 0 )
				return UnicodeString();

			n += position;
			position = 0;
		}
		else if( static_cast< size_t > ( n ) > static_cast< size_t > ( originalLength - position ) )
			n = originalLength - position;

		if( position == 0 && n == originalLength )
			return *this;

		if( n > 0 )
		{
			icu::UnicodeString result;
			extract( position, n, result );

			return result;
		}
		else
			return UnicodeString();
	}

	int toInt() const
	{
		try {
			std::string tmp;
			toUTF8String( tmp );
			return std::stoi( tmp );
		}
		catch( const std::invalid_argument & )
		{
		}
		catch( const std::out_of_range & )
		{
		}

		return 0;
	}

	int toInt( bool * ok, int base ) const
	{
		try {
			std::string tmp;
			toUTF8String( tmp );
			const auto result = std::stoi( tmp, nullptr, base );
			if( ok) *ok = true;
			return result;
		}
		catch( const std::invalid_argument & )
		{
			if( ok ) *ok = false;
		}
		catch( const std::out_of_range & )
		{
			if( ok ) *ok = false;
		}

		return 0;
	}

	bool contains( const UnicodeChar & ch ) const
	{
		return ( icu::UnicodeString::indexOf( (UChar32) ch ) != -1 );
	}

	bool contains( const UnicodeString & str ) const
	{
		return ( icu::UnicodeString::indexOf( str ) != -1 );
	}

	UnicodeString simplified() const
	{
		if( isEmpty() )
			return *this;

		UnicodeString result;
		int32_t i = 0;

		while( true )
		{
			while( i < length() &&
				std::isspace( static_cast< unsigned char > ( char32At( i ) ) ) )
					++i;

			while( i != length() &&
				!std::isspace( static_cast< unsigned char > ( char32At( i ) ) ) )
			{
				result.append( UnicodeChar( char32At( i ) ) );
				++i;
			}

			if( i == length() )
				break;

			result.append( UnicodeChar( ' ' ) );
		}

		if( !result.isEmpty() && result[ result.size() - 1 ] == ' ' )
			result.remove( result.size() - 1, 1 );

		return result;
	}

	std::vector< UnicodeString > split( char ch ) const
	{
		std::vector< UnicodeString > result;

		int32_t pos = 0;

		while( pos < length() )
		{
			const auto fpos = indexOf( ch, pos );

			if( fpos != -1 )
			{
				if( fpos - pos > 0 )
				{
					icu::UnicodeString tmp;
					extract( pos, fpos - pos, tmp );
					result.push_back( tmp );
				}

				pos = fpos + 1;
			}
			else if( pos < length() )
			{
				icu::UnicodeString tmp;
				extract( pos, length() - pos, tmp );
				result.push_back( tmp );

				break;
			}
		}

		return result;
	}

	UnicodeString & replace( const UnicodeChar & before, const UnicodeString & after )
	{
		for( int32_t pos = 0; ( pos = indexOf( before, pos ) ) != -1; pos += after.size() )
			icu::UnicodeString::replace( pos, 1, after );

		return *this;
	}

	UnicodeString & replace( const UnicodeString & before, const UnicodeString & after )
	{
		for( int32_t pos = 0; ( pos = indexOf( before, pos ) ) != -1; pos += after.size() )
			icu::UnicodeString::replace( pos, before.length(), after );

		return *this;
	}

	UnicodeString sliced( long long int pos ) const
	{
		icu::UnicodeString tmp;
		extract( pos, length() - pos, tmp );

		return tmp;
	}

	UnicodeString sliced( long long int pos, long long int length ) const
	{
		icu::UnicodeString tmp;
		extract( pos, length, tmp );

		return tmp;
	}

	UnicodeString right( long long int n ) const
	{
		icu::UnicodeString tmp;
		extract( length() - 1, n, tmp );

		return tmp;
	}

	UnicodeString & toCaseFolded()
	{
		icu::UnicodeString::foldCase();

		return *this;
	}

	UnicodeString toUpper()
	{
		icu::UnicodeString::toUpper();

		return *this;
	}

	UnicodeString toLower() const
	{
		auto tmp = *this;
		tmp.toLower();

		return tmp;
	}

	void clear()
	{
		icu::UnicodeString::remove();
	}
}; // class StdString


//
// UrlUri
//

class UrlUri {
public:
	explicit UrlUri( const UnicodeString & uri )
	{
	}

	~UrlUri()
	{
	}

	bool isValid() const
	{
		return false;
	}

	bool isRelative() const
	{
		return false;
	}

private:
}; // class UrlUri


//
// UnicodeStringTrait
//

//! Trait to use this library with std::string.
struct UnicodeStringTrait {
	template< class T >
	using Vector = std::vector< T >;

	template< class T, class U >
	using Map = std::map< T, U >;

	using String = UnicodeString;

	using Char = UnicodeChar;

	using TextStream = std::istream;

	using StringList = std::vector< String >;

	using Url = UrlUri;

	//! Convert UTF-16 into trait's string.
	static String utf16ToString( const char16_t * u16 )
	{
		return UnicodeString( u16 );
	}

	//! \return Does file exist.
	static bool fileExists( const String & fileName, const String & workingPath )
	{
		std::string path;
		( workingPath + fileName ).toUTF8String( path );

		return std::filesystem::exists( path );
	}

	//! \return Absolute file path.
	static String absoluteFilePath( const String & path )
	{
		std::string tmp;
		path.toUTF8String( tmp );
		std::filesystem::path p( tmp );

		return UnicodeString::fromUTF8( p.lexically_normal().u8string() );
	}
}; // struct UnicodeStringTrait

#endif // MD4QT_ICU_STL_SUPPORT

#ifdef MD4QT_QT_SUPPORT

//
// QStringTrait
//

//! Trait to use this library with QString.
struct QStringTrait {
	template< class T >
	using Vector = QVector< T >;

	template< class T, class U >
	using Map = std::map< T, U >;

	using String = QString;

	using Char = QChar;

	using TextStream = QTextStream;

	using StringList = QStringList;

	using Url = QUrl;

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

	//! \return Absolute file path.
	static String absoluteFilePath( const String & path )
	{
		return QFileInfo( path ).absoluteFilePath();
	}
}; // struct QStringTrait

#endif // MD4QT_QT_SUPPORT

} /* namespace MD */

#endif // MD4QT_MD_TRAITS_HPP_INCLUDED
