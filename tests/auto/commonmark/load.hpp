
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

#pragma once

#include <md_parser.hpp>
#include <QDir>


inline QSharedPointer< MD::Document >
load_test( int n )
{
	auto fileName = QString::number( n );

	if( fileName.size() < 3 )
		fileName.prepend( QString( 3 - fileName.size(), QLatin1Char( '0' ) ) );

	fileName.prepend( QStringLiteral( "tests/commonmark/0.30/" ) );
	fileName.append( QStringLiteral( ".md" ) );

	MD::Parser p;

	return p.parse( fileName, false );
}
