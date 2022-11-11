
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

#define MD4QT_QT_SUPPORT
#define MD4QT_ICU_STL_SUPPORT

#include <md4qt/doc.hpp>
#include <md4qt/parser.hpp>
#include <md4qt/traits.hpp>

#include <md4c.h>

#include <chrono>
#include <iostream>
#include <vector>
#include <memory>

#include <QFile>

#include <cmark-gfm.h>


struct DATA {
	std::shared_ptr< MD::Document< MD::QStringTrait > > * doc;
	std::vector< std::shared_ptr< MD::Item< MD::QStringTrait > > > elems;
};


int enter_block( MD_BLOCKTYPE type, void * data, void * doc )
{
	auto d = reinterpret_cast< DATA* >( doc );

	std::shared_ptr< MD::Item< MD::QStringTrait > > item;

	switch( type )
	{
		case MD_BLOCK_H :
		{
			item.reset( new MD::Heading< MD::QStringTrait > );
		}
			break;

		case MD_BLOCK_CODE :
		{
			item.reset( new MD::Code< MD::QStringTrait >( QStringLiteral( "" ), false ) );
		}
			break;

		case MD_BLOCK_P :
		{
			item.reset( new MD::Paragraph< MD::QStringTrait > );
		}
			break;

		case MD_BLOCK_QUOTE :
		{
			item.reset( new MD::Blockquote< MD::QStringTrait > );
		}
			break;

		case MD_BLOCK_UL :
		case MD_BLOCK_OL :
		{
			item.reset( new MD::List< MD::QStringTrait > );
		}
			break;

		case MD_BLOCK_LI :
		{
			item.reset( new MD::ListItem< MD::QStringTrait > );
		}
			break;

		case MD_BLOCK_TABLE :
		{
			item.reset( new MD::Table< MD::QStringTrait > );
		}
			break;

		case MD_BLOCK_THEAD :
		case MD_BLOCK_TR :
		{
			item.reset( new MD::TableRow< MD::QStringTrait > );
		}
			break;

		case MD_BLOCK_TH :
		case MD_BLOCK_TD :
		{
			item.reset( new MD::TableCell< MD::QStringTrait > );
		}
			break;

		default :
			break;
	}

	if( item )
	{
		if( d->elems.empty() )
			(*d->doc)->appendItem( item );
		else
		{
			switch( d->elems.back()->type() )
			{
				case MD::ItemType::Paragraph :
				case MD::ItemType::Blockquote :
				case MD::ItemType::ListItem :
				case MD::ItemType::List :
				{
					auto * b = static_cast< MD::Block< MD::QStringTrait >* > ( d->elems.back().get() );

					b->appendItem( item );
				}
					break;

				case MD::ItemType::Table :
				{
					auto * t = static_cast< MD::Table< MD::QStringTrait >* > ( d->elems.back().get() );

					t->appendRow( std::static_pointer_cast< MD::TableRow< MD::QStringTrait > > ( item ) );
				}
					break;

				case MD::ItemType::TableRow :
				{
					auto * t = static_cast< MD::TableRow< MD::QStringTrait >* > ( d->elems.back().get() );

					t->appendCell( std::static_pointer_cast< MD::TableCell< MD::QStringTrait > > ( item ) );
				}
					break;

				default :
					(*d->doc)->appendItem( item );
					break;
			}
		}

		d->elems.push_back( item );
	}

	return 0;
}

int leave_block( MD_BLOCKTYPE type, void * data, void * doc )
{
	auto d = reinterpret_cast< DATA* >( doc );

	switch( type )
	{
		case MD_BLOCK_H :
		case MD_BLOCK_CODE :
		case MD_BLOCK_P :
		case MD_BLOCK_QUOTE :
		case MD_BLOCK_UL :
		case MD_BLOCK_OL :
		case MD_BLOCK_LI :
		case MD_BLOCK_TABLE :
		case MD_BLOCK_THEAD :
		case MD_BLOCK_TR :
		case MD_BLOCK_TH :
		case MD_BLOCK_TD :
			d->elems.pop_back();
			break;

		default :
			break;
	}

	return 0;
}

int enter_span( MD_SPANTYPE type, void * data, void * doc )
{
	auto d = reinterpret_cast< DATA* >( doc );

	switch( type )
	{
		case MD_SPAN_CODE :
		{
			std::shared_ptr< MD::Item< MD::QStringTrait > > item(
				new MD::Code< MD::QStringTrait >( QStringLiteral( "" ), true ) );

			if( d->elems.empty() )
				(*d->doc)->appendItem( item );
			else
			{
				switch( d->elems.back()->type() )
				{
					case MD::ItemType::Paragraph :
					case MD::ItemType::Blockquote :
					case MD::ItemType::ListItem :
					case MD::ItemType::List :
					{
						auto * b = static_cast< MD::Block< MD::QStringTrait >* > ( d->elems.back().get() );

						b->appendItem( item );
					}
						break;

					default :
						break;
				}
			}
		}
			break;

		default :
		{
			std::shared_ptr< MD::Item< MD::QStringTrait > > item( new MD::Text< MD::QStringTrait > );

			if( d->elems.empty() )
				(*d->doc)->appendItem( item );
			else
			{
				switch( d->elems.back()->type() )
				{
					case MD::ItemType::Paragraph :
					case MD::ItemType::Blockquote :
					case MD::ItemType::ListItem :
					case MD::ItemType::List :
					{
						auto * b = static_cast< MD::Block< MD::QStringTrait >* > ( d->elems.back().get() );

						b->appendItem( item );
					}
						break;

					default :
						break;
				}
			}
		}
			break;
	}

	return 0;
}

int leave_span( MD_SPANTYPE type, void * data, void * doc )
{
	auto d = reinterpret_cast< DATA* >( doc );

	switch( type )
	{
		default :
			break;
	}

	return 0;
}

int text( MD_TEXTTYPE type, const MD_CHAR * data, MD_SIZE size, void * doc )
{
	auto d = reinterpret_cast< DATA* >( doc );

	QByteArray buf( data, size );
	QString txt( buf );

	switch( d->elems.back()->type() )
	{
		case MD::ItemType::Code :
		{
			// I removed setText() method from MD::Code.
			// Creation of MD::DOcument in this benchamrk with md4c
			// is just for approximate comparison...
		}
			break;

		case MD::ItemType::Paragraph :
		{
			auto * p = static_cast< MD::Paragraph< MD::QStringTrait >* > ( d->elems.back().get() );
			std::shared_ptr< MD::Text< MD::QStringTrait > > t( new MD::Text< MD::QStringTrait > );
			t->setText( txt );
			p->appendItem( t );
		}
			break;

		case MD::ItemType::Heading :
		{
			auto * h = static_cast< MD::Heading< MD::QStringTrait >* > ( d->elems.back().get() );
			std::shared_ptr< MD::Paragraph< MD::QStringTrait > > p(
				new MD::Paragraph< MD::QStringTrait > );
			std::shared_ptr< MD::Text< MD::QStringTrait > > t( new MD::Text< MD::QStringTrait > );
			t->setText( txt );
			p->appendItem( t );
			h->setText( p );
		}
			break;

		default :
			break;
	}

	return 0;
}

int main( int argc, char ** argv )
{
	//md4c
	{
		const auto start = std::chrono::high_resolution_clock::now();

		std::shared_ptr< MD::Document< MD::QStringTrait > > doc( new MD::Document< MD::QStringTrait > );

		QFile file( QStringLiteral( "tests/manual/complex.md" ) );

		if( file.open( QIODevice::ReadOnly ) )
		{
			const auto md = file.readAll();

			file.close();

			DATA data;
			data.doc = &doc;

			MD_PARSER parser;
			parser.abi_version = 0;
			parser.flags = MD_FLAG_TABLES;
			parser.enter_block = enter_block;
			parser.leave_block = leave_block;
			parser.enter_span = enter_span;
			parser.leave_span = leave_span;
			parser.text = text;
			parser.debug_log = nullptr;
			parser.syntax = nullptr;

			if( md_parse( md.constData(), md.size(), &parser, &data ) != 0 )
				std::cout << "failed to parse input with md4c" << std::endl;
			else if( data.elems.size() != 0 )
				std::cout << "parsing done. not finished blocks count: " << data.elems.size() << std::endl;
		}
		else
			std::cout << "failed to open complex.md" << std::endl;

		const auto end = std::chrono::high_resolution_clock::now();

		const auto d = std::chrono::duration_cast< std::chrono::microseconds > ( end - start );

		std::cout << "md4c parsing: " << d.count() << " us" << std::endl;
	}

	// md4qt
	{
		const auto start = std::chrono::high_resolution_clock::now();

		MD::Parser< MD::UnicodeStringTrait > parser;

		const auto doc = parser.parse( MD::UnicodeString( "tests/manual/complex.md" ), false );

		const auto end = std::chrono::high_resolution_clock::now();

		const auto d = std::chrono::duration_cast< std::chrono::microseconds > ( end - start );

		std::cout << "md4qt with ICU parsing: " << d.count() << " us" << std::endl;
	}

	// md4qt
	{
		const auto start = std::chrono::high_resolution_clock::now();

		MD::Parser< MD::QStringTrait > parser;

		const auto doc = parser.parse( QStringLiteral( "tests/manual/complex.md" ), false );

		const auto end = std::chrono::high_resolution_clock::now();

		const auto d = std::chrono::duration_cast< std::chrono::microseconds > ( end - start );

		std::cout << "md4qt with Qt6 parsing: " << d.count() << " us" << std::endl;
	}

	// cmark-gfm
	{
		const auto start = std::chrono::high_resolution_clock::now();

		QFile file( QStringLiteral( "tests/manual/complex.md" ) );

		if( file.open( QIODevice::ReadOnly ) )
		{
			const auto md = file.readAll();

			file.close();

			auto doc = cmark_parse_document( md.constData(), md.size(), CMARK_OPT_FOOTNOTES );

			cmark_node_free( doc );
		}
		else
			std::cout << "failed to open complex.md" << std::endl;

		const auto end = std::chrono::high_resolution_clock::now();

		const auto d = std::chrono::duration_cast< std::chrono::microseconds > ( end - start );

		std::cout << "cmark-gfm parsing: " << d.count() << " us" << std::endl;
	}

	return 0;
}
