
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2022-2023 Igor Mironchik

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

// args-parser include.
#include <args-parser/all.hpp>

// md4qt include.
#define MD4QT_QT_SUPPORT
#include <md4qt/traits.hpp>
#include <md4qt/parser.hpp>
#include <md4qt/html.hpp>

// Qt include.
#include <QFile>
#include <QFileInfo>


using namespace Args;
using namespace MD;

int main( int argc, char ** argv )
{
	try {
		CmdLine cmd( argc, argv );

		cmd.addArgWithFlagAndName( 'm', "markdown", true, true, "Markdown file to convert to HTML." )
			.addArgWithFlagAndName( 'o', "html", true, true, "Output HTML file name." )
			.addArgWithFlagAndName( 'r', "recursive", false, false, "Read all linked Markdown files?" )
			.addHelp( true, argv[ 0 ], "Converter of Markdown to HTML." );

		cmd.parse();

		const auto markdownFileName = cmd.value( "-m" );
		const auto htmlFileName = cmd.value( "-o" );
		const auto recursive = cmd.isDefined( "-r" );

		QFileInfo mdFileInfo( markdownFileName );

		if( mdFileInfo.exists() )
		{
			if( mdFileInfo.suffix() == "md" || mdFileInfo.suffix() == "markdown" )
			{
				Parser< QStringTrait > parser;

				const auto doc = parser.parse( markdownFileName, recursive );

				QFile html( htmlFileName );

				if( html.open( QIODevice::WriteOnly ) )
				{
					const auto content = toHtml( doc );

					html.write( content.toUtf8() );

					html.close();
				}
				else
				{
					outStream() << "Unable to write output HTML file.\n";

					return 1;
				}
			}
			else
			{
				outStream() << "Wrong file suffix of Markdown file (supported *.md, *.markdown).\n";

				return 1;
			}
		}
		else
		{
			outStream() << "Input Markdown file is not exist.\n";

			return 1;
		}
	}
	catch( const HelpHasBeenPrintedException & )
	{
	}
	catch( const BaseException & x )
	{
		outStream() << x.desc() << "\n";

		return 1;
	}

  return 0;
}
