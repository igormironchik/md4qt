/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef MD4QT_MD_POSCACHE_HPP_INCLUDED
#define MD4QT_MD_POSCACHE_HPP_INCLUDED

#include "visitor.hpp"

// C++ include.
#include <vector>
#include <algorithm>


namespace MD {

namespace details {

//
// PosRange
//

template< class Trait >
struct PosRange {
	long long int startColumn = -1;
	long long int startLine = -1;
	long long int endColumn = -1;
	long long int endLine = -1;
	Item< Trait > * item = nullptr;
	std::vector< PosRange< Trait > > children = {};
};

// Look at this equality operator like on rectangles intersection.
// If rectangles in text intersect then rectangles are equal.
template< class Trait >
bool operator == ( const PosRange< Trait > & l, const PosRange< Trait > & r )
{
	return ( l.startLine <= r.endLine && l.endLine >= r.startLine &&
		( l.startLine == r.endLine && l.endLine == r.startLine ?
			l.endColumn >= r.startColumn && l.startColumn <= r.endColumn :
			true ) );
}

template< class Trait >
bool operator < ( const PosRange< Trait > & l, const PosRange< Trait > & r )
{
	return ( l.endLine < r.startLine ||
		( l.endLine == r.startLine && l.endColumn < r.startColumn ) );
}

} /* namespace details */

//
// PosCache
//

//! Cache of Markdown items to be accessed via position.
template< class Trait >
class PosCache
	:	public MD::Visitor< Trait >
{
public:
	PosCache() = default;
	~PosCache() override = default;

	//! Initialize cache with the give document.
	//! \note Document should not be recursive.
	virtual void initialize( std::shared_ptr< MD::Document< Trait > > doc )
	{
		cache.clear();
	
		if( doc )
		{
			Visitor< Trait >::process( doc );
	
			for( auto it = doc->footnotesMap().cbegin(), last = doc->footnotesMap().cend();
				it != last; ++it )
			{
				onFootnote( it->second.get() );
			}
		}
	}
	
	//! Vector with items, where front is a top-level item,
	//! and back is most nested child.
	using Items = typename Trait::template Vector< Item< Trait >* >;
	
	//! \return First occurense of Markdown item with all first children by the give position.
	Items findFirstInCache( const MD::WithPosition & pos ) const
	{
		Items res;
		
		details::PosRange< Trait > tmp{ pos.startColumn(), pos.startLine(), pos.endColumn(), pos.endLine() };
		
		findFirstInCache( cache, tmp, res );
		
		return res;
	}
	
protected:
	details::PosRange< Trait > * findInCache( std::vector< details::PosRange< Trait > > & vec,
		const details::PosRange< Trait > & pos ) const
	{
		const auto it = std::lower_bound( vec.begin(), vec.end(), pos );
		
		if( it != vec.end() && *it == pos )
		{
			if( !it->children.empty() )
			{
				auto nested = findInCache( it->children, pos );
				
				return ( nested ? nested : &(*it) );
			}
			else
				return &(*it);
		}
		else
			return nullptr;
	}
	
	void findFirstInCache( const std::vector< details::PosRange< Trait > > & vec,
		const details::PosRange< Trait > & pos,
		Items & res ) const
	{	
		const auto it = std::lower_bound( vec.begin(), vec.end(), pos );
		
		if( it != vec.end() && *it == pos )
		{
			res.push_back( it->item );
			
			if( !it->children.empty() )
				findFirstInCache( it->children, pos, res );
		}
	}
	
	void insertInCache( const details::PosRange< Trait > & item, bool sort = false )
	{
		if( !skipInCache )
		{
			auto pos = findInCache( cache, item );
			
			if( pos )
				pos->children.push_back( item );
			else
			{
				if( sort )
				{
					const auto it = std::upper_bound( cache.begin(), cache.end(), item );
					
					if( it != cache.end() )
						cache.insert( it, item );
					else
						cache.push_back( item );
				}
				else
					cache.push_back( item );
			}
		}
	}

protected:
	void onAddLineEnding() override
	{
	}
	
	void onText( Text< Trait > * t ) override
	{
		details::PosRange< Trait > r{ t->openStyles().empty() ?
				t->startColumn() : t->openStyles().front().startColumn(),
			t->startLine(),
			t->closeStyles().empty() ? t->endColumn() : t->closeStyles().back().endColumn(),
			t->endLine(), t };
		
		insertInCache( r );
	}
	
	void onMath( Math< Trait > * m ) override
	{
		auto startColumn = m->startDelim().startColumn();
		auto startLine = m->startDelim().startLine();
		auto endColumn = m->endDelim().endColumn();
		auto endLine = m->endDelim().endLine();
		
		if( !m->openStyles().empty() )
		{
			startColumn = m->openStyles().front().startColumn();
			startLine = m->openStyles().front().startLine();
		}
		
		if( !m->closeStyles().empty() )
		{
			endColumn = m->closeStyles().back().endColumn();
			endLine = m->closeStyles().back().endLine();
		}
		
		details::PosRange< Trait > r{ startColumn, startLine, endColumn, endLine, m };
		
		insertInCache( r );
	}
	
	void onLineBreak( LineBreak< Trait > * b ) override
	{
	}
	
	void onParagraph( Paragraph< Trait > * p, bool wrap ) override
	{
		details::PosRange< Trait > r{ p->startColumn(), p->startLine(), p->endColumn(), p->endLine(), p };
		
		insertInCache( r );
		
		Visitor< Trait >::onParagraph( p, wrap );
	}
	
	void onHeading( Heading< Trait > * h ) override
	{
		details::PosRange< Trait > r{ h->startColumn(), h->startLine(), h->endColumn(), h->endLine(), h };
		
		insertInCache( r );
		
		if( h->text() && !h->text()->isEmpty() )
			onParagraph( h->text().get(), false );
	}
	
	void onCode( Code< Trait > * c ) override
	{
		auto startColumn = c->isFensedCode() ? c->startDelim().startColumn() : c->startColumn();
		auto startLine = c->isFensedCode() ? c->startDelim().startLine() : c->startLine();
		auto endColumn = c->isFensedCode() ? c->endDelim().endColumn() : c->endColumn();
		auto endLine = c->isFensedCode() ? c->endDelim().endLine() : c->endLine();
		
		details::PosRange< Trait > r{ startColumn, startLine, endColumn, endLine, c };
		
		insertInCache( r );
	}
	
	void onInlineCode( Code< Trait > * c ) override
	{
		auto startColumn = c->startDelim().startColumn();
		auto startLine = c->startDelim().startLine();
		auto endColumn = c->endDelim().endColumn();
		auto endLine = c->endDelim().endLine();
		
		if( !c->openStyles().empty() )
		{
			startColumn = c->openStyles().front().startColumn();
			startLine = c->openStyles().front().startLine();
		}
		
		if( !c->closeStyles().empty() )
		{
			endColumn = c->closeStyles().back().endColumn();
			endLine = c->closeStyles().back().endLine();
		}
		
		details::PosRange< Trait > r{ startColumn, startLine, endColumn, endLine, c };
		
		insertInCache( r );
	}
	
	void onBlockquote( Blockquote< Trait > * b ) override
	{
		details::PosRange< Trait > r{ b->startColumn(), b->startLine(), b->endColumn(), b->endLine(), b };
		
		insertInCache( r );
	
		Visitor< Trait >::onBlockquote( b );
	}
	
	void onList( List< Trait > * l ) override
	{
		bool first = true;
		
		details::PosRange< Trait > r{ l->startColumn(), l->startLine(), l->endColumn(), l->endLine(), l };
		
		insertInCache( r );
	
		for( auto it = l->items().cbegin(), last = l->items().cend(); it != last; ++it )
		{
			if( (*it)->type() == ItemType::ListItem )
			{
				onListItem( static_cast< ListItem< Trait >* > ( it->get() ), first );
	
				first = false;
			}
		}
	}
	
	void onTable( Table< Trait > * t ) override
	{
		details::PosRange< Trait > r{ t->startColumn(), t->startLine(), t->endColumn(), t->endLine(), t };
		
		insertInCache( r );
	
		if( !t->isEmpty() )
		{
			int columns = 0;
	
			for( auto th = (*t->rows().cbegin())->cells().cbegin(),
				last = (*t->rows().cbegin())->cells().cend(); th != last; ++th )
			{
				Visitor< Trait >::onTableCell( th->get() );
	
				++columns;
			}
	
			for( auto r = std::next( t->rows().cbegin() ), rlast = t->rows().cend(); r != rlast; ++r )
			{
				int i = 0;
	
				for( auto c = (*r)->cells().cbegin(), clast = (*r)->cells().cend(); c != clast; ++c )
				{
					Visitor< Trait >::onTableCell( c->get() );
	
					++i;
	
					if( i == columns )
						break;
				}
			}
		}
	}
	
	void onAnchor( Anchor< Trait > * a ) override
	{
	}
	
	void onRawHtml( RawHtml< Trait > * h ) override
	{
		details::PosRange< Trait > r{ h->startColumn(), h->startLine(), h->endColumn(), h->endLine(), h };
		
		insertInCache( r );
	}
	
	void onHorizontalLine( HorizontalLine< Trait > * l ) override
	{
		details::PosRange< Trait > r{ l->startColumn(), l->startLine(), l->endColumn(), l->endLine(), l };
		
		insertInCache( r );
	}
	
	void onLink( Link< Trait > * l ) override
	{
		auto startColumn = l->startColumn();
		auto startLine = l->startLine();
		auto endColumn = l->endColumn();
		auto endLine = l->endLine();
		
		if( !l->openStyles().empty() )
		{
			startColumn = l->openStyles().front().startColumn();
			startLine = l->openStyles().front().startLine();
		}
		
		if( !l->closeStyles().empty() )
		{
			endColumn = l->closeStyles().back().endColumn();
			endLine = l->closeStyles().back().endLine();
		}
		
		details::PosRange< Trait > r{ startColumn, startLine, endColumn, endLine, l };
		
		insertInCache( r );
	
		if( l->p() )
		{
			skipInCache = true;
			onParagraph( l->p().get(), true );
			skipInCache = false;
		}
	}
	
	void onImage( Image< Trait > * i ) override
	{
		auto startColumn = i->startColumn();
		auto startLine = i->startLine();
		auto endColumn = i->endColumn();
		auto endLine = i->endLine();
		
		if( !i->openStyles().empty() )
		{
			startColumn = i->openStyles().front().startColumn();
			startLine = i->openStyles().front().startLine();
		}
		
		if( !i->closeStyles().empty() )
		{
			endColumn = i->closeStyles().back().endColumn();
			endLine = i->closeStyles().back().endLine();
		}
		
		details::PosRange< Trait > r{ startColumn, startLine, endColumn, endLine, i };
		
		insertInCache( r );
	
		if( i->p() )
		{
			skipInCache = true;
			onParagraph( i->p().get(), true );
			skipInCache = false;
		}
	}
	
	void onFootnoteRef( FootnoteRef< Trait > * ref ) override
	{
		auto startColumn = ref->startColumn();
		auto startLine = ref->startLine();
		auto endColumn = ref->endColumn();
		auto endLine = ref->endLine();
		
		if( !ref->openStyles().empty() )
		{
			startColumn = ref->openStyles().front().startColumn();
			startLine = ref->openStyles().front().startLine();
		}
		
		if( !ref->closeStyles().empty() )
		{
			endColumn = ref->closeStyles().back().endColumn();
			endLine = ref->closeStyles().back().endLine();
		}
		
		details::PosRange< Trait > r{ startColumn, startLine, endColumn, endLine, ref };
		
		insertInCache( r );
	}
	
	void onFootnote( Footnote< Trait > * f ) override
	{
		details::PosRange< Trait > r{ f->startColumn(), f->startLine(), f->endColumn(), f->endLine(), f };
		
		insertInCache( r, true );
	
		Visitor< Trait >::onFootnote( f );
	}
	
	void onListItem( ListItem< Trait > * l, bool first ) override
	{
		details::PosRange< Trait > r{ l->startColumn(), l->startLine(), l->endColumn(), l->endLine(), l };
		
		insertInCache( r );
	
		Visitor< Trait >::onListItem( l, first );
	}
	
protected:
	//! Cache.
	std::vector< details::PosRange< Trait > > cache;
	//! Skip adding in cache.
	bool skipInCache = false;
}; // class PosCache

} /* namespace MD */

#endif // MD4QT_MD_POSCACHE_HPP_INCLUDED
