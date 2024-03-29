
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
