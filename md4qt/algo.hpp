/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef MD4QT_MD_ALGO_HPP_INCLUDED
#define MD4QT_MD_ALGO_HPP_INCLUDED

// md4qt include.
#include "traits.hpp"
#include "visitor.hpp"

// C++ include.
#include <functional>


namespace MD {

//! Function type for algorithms.
template< class Trait >
using ItemFunctor = std::function< void( Item< Trait >* ) >;

namespace details {

//
// AlgoVisitor
//

//! Visitor for algorithms.
template< class Trait >
class AlgoVisitor
	:	public Visitor< Trait >
{
public:
	AlgoVisitor( unsigned int mnl,
		const typename Trait::template Vector< ItemType > & t,
		ItemFunctor< Trait > f )
		:	maxNestingLevel( mnl )
		,	types( t )
		,	func( f )
	{
	}

	~AlgoVisitor() override = default;

	virtual void walk( std::shared_ptr< Document< Trait > > doc )
	{
		this->process( doc );

		onFootnotes( doc );
	}

protected:
	void onAddLineEnding() override
	{
	}

	void onUserDefined( Item< Trait > * i ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( i->type() ) )
			func( i );
	}

	void onText( Text< Trait > * t ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::Text ) )
			func( t );
	}

	void onMath( Math< Trait > * m ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::Math ) )
			func( m );
	}

	void onLineBreak( LineBreak< Trait > * l ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::LineBreak ) )
			func( l );
	}

	void onParagraph( Paragraph< Trait > * p, bool ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::Paragraph ) )
			func( p );

		if( inc.nextAllowed() )
			Visitor< Trait >::onParagraph( p, true );
	}

	void onHeading( Heading< Trait > * h ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::Heading ) )
			func( h );

		if( h->text() && !h->text()->isEmpty() && inc.nextAllowed() )
			onParagraph( h->text().get(), true );
	}

	void onCode( Code< Trait > * c ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::Code ) )
			func( c );
	}

	void onInlineCode( Code< Trait > * c ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::Code ) )
			func( c );
	}

	void onBlockquote( Blockquote< Trait > * b ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::Blockquote ) )
			func( b );

		if( inc.nextAllowed() )
			Visitor< Trait >::onBlockquote( b );
	}

	void onList( List< Trait > * l ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::List ) )
			func( l );

		for( auto it = l->items().cbegin(), last = l->items().cend(); it != last; ++it )
		{
			if( (*it)->type() == ItemType::ListItem && inc.nextAllowed() )
				onListItem( static_cast< ListItem< Trait>* > ( it->get() ), true );
		}
	}

	void onTable( Table< Trait > * t ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::Table ) )
			func( t );

		if( !t->isEmpty() && inc.nextAllowed() )
		{
			int columns = 0;

			for( auto th = (*t->rows().cbegin())->cells().cbegin(),
				last = (*t->rows().cbegin())->cells().cend(); th != last; ++th )
			{
				this->onTableCell( th->get() );

				++columns;
			}

			for( auto r = std::next( t->rows().cbegin() ), rlast = t->rows().cend(); r != rlast; ++r )
			{
				int i = 0;

				for( auto c = (*r)->cells().cbegin(), clast = (*r)->cells().cend(); c != clast; ++c )
				{
					this->onTableCell( c->get() );

					++i;

					if( i == columns )
						break;
				}
			}
		}
	}

	void onAnchor( Anchor< Trait > * a ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::Anchor ) )
			func( a );
	}

	void onRawHtml( RawHtml< Trait > * h ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::RawHtml ) )
			func( h );
	}

	void onHorizontalLine( HorizontalLine< Trait > * l ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::HorizontalLine ) )
			func( l );
	}

	void onLink( Link< Trait > * l ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::Link ) )
			func( l );

		if( inc.nextAllowed() )
		{
			if( !l->img()->isEmpty() )
				onImage( l->img().get() );
			else if( l->p() && !l->p()->isEmpty() )
				onParagraph( l->p().get(), false );
		}
	}

	void onImage( Image< Trait > * i ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::Image ) )
			func( i );
	}

	void onFootnoteRef( FootnoteRef< Trait > * ref ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::FootnoteRef ) )
			func( ref );
	}

	void onListItem( ListItem< Trait > * i, bool ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::ListItem ) )
			func( i );

		if( inc.nextAllowed() )
			Visitor< Trait >::onListItem( i, true );
	}

	void onTableCell( TableCell< Trait > * c ) override
	{
		Visitor< Trait >::onTableCell( c );
	}

	void onFootnote( Footnote< Trait > * f ) override
	{
		IncrementNestingLevel inc( currentNestingLevel, maxNestingLevel, types );

		if( inc.allowed( ItemType::Footnote ) )
			func( f );

		if( inc.nextAllowed() )
			Visitor< Trait >::onFootnote( f );
	}

	virtual void onFootnotes( std::shared_ptr< Document< Trait > > doc )
	{
		for( const auto & f : doc->footnotesMap() )
			this->onFootnote( f.second.get() );
	}

protected:
	unsigned int currentNestingLevel = 0;
	unsigned int maxNestingLevel = 0;
	const typename Trait::template Vector< ItemType > & types;
	ItemFunctor< Trait > func = {};

	struct IncrementNestingLevel {
		IncrementNestingLevel( unsigned int & l,
			unsigned int m,
			const typename Trait::template Vector< ItemType > & t )
			:	level( l )
			,	maxNestingLevel( m )
			,	types( t )
		{
			++level;
		}

		~IncrementNestingLevel()
		{
			--level;
		}

		bool allowed( ItemType t ) const
		{
			return ( ( maxNestingLevel == 0 || level <= maxNestingLevel ) &&
				std::find( types.cbegin(), types.cend(), t ) != types.cend() );
		}

		bool nextAllowed() const
		{
			return ( maxNestingLevel == 0 || level + 1 <= maxNestingLevel );
		}

		unsigned int & level;
		unsigned int maxNestingLevel;
		const typename Trait::template Vector< ItemType > & types;
	}; // struct IncrementNestingLevel
}; // class HtmlVisitor

} /* namespace details */

//! Calls function for each item in the document with the given type.
template< class Trait >
inline void
forEach(
	//! Vector of item's types to be processed.
	const typename Trait::template Vector< ItemType > & types,
	//! Document.
	std::shared_ptr< Document< Trait > > doc,
	//! Functor object.
	ItemFunctor< Trait > func,
	//! Maximun nesting level.
	//! 0 means infinity, 1 - only top level items...
	unsigned int maxNestingLevel = 0 )
{
	details::AlgoVisitor< Trait > v( maxNestingLevel, types, func );

	v.walk( doc );
}

} /* namespace MD */

#endif // MD4QT_MD_ALGO_HPP_INCLUDED
