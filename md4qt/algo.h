/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef MD4QT_MD_ALGO_H_INCLUDED
#define MD4QT_MD_ALGO_H_INCLUDED

// md4qt include.
#include "traits.h"
#include "visitor.h"

// C++ include.
#include <functional>

namespace MD
{

//! Function type for algorithms.
template<class Trait>
using ItemFunctor = std::function<void(Item<Trait> *)>;

namespace details
{

//
// AlgoVisitor
//

//! Visitor for algorithms.
template<class Trait>
class AlgoVisitor : public Visitor<Trait>
{
public:
    AlgoVisitor(unsigned int mnl,
                const typename Trait::template Vector<ItemType> &t,
                ItemFunctor<Trait> f)
        : m_maxNestingLevel(mnl)
        , m_types(t)
        , m_func(f)
    {
    }

    ~AlgoVisitor() override = default;

    //! Walk through the document.
    virtual void walk(std::shared_ptr<Document<Trait>> doc)
    {
        this->process(doc);

        onFootnotes(doc);
    }

protected:
    void onAddLineEnding() override
    {
    }

    //! Process user defined item type.
    void onUserDefined(Item<Trait> *i) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(i->type())) {
            m_func(i);
        }
    }

    //! Process text item.
    void onText(Text<Trait> *t) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::Text)) {
            m_func(t);
        }
    }

    //! Process LaTeX math item.
    void onMath(Math<Trait> *m) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::Math)) {
            m_func(m);
        }
    }

    //! Process line break.
    void onLineBreak(LineBreak<Trait> *l) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::LineBreak)) {
            m_func(l);
        }
    }

    //! Process paragraph.
    void onParagraph(Paragraph<Trait> *p, bool) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::Paragraph)) {
            m_func(p);
        }

        if (inc.nextAllowed()) {
            Visitor<Trait>::onParagraph(p, true);
        }
    }

    //! Process heading.
    void onHeading(Heading<Trait> *h) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::Heading)) {
            m_func(h);
        }

        if (h->text() && !h->text()->isEmpty() && inc.nextAllowed()) {
            onParagraph(h->text().get(), true);
        }
    }

    //! Process code block.
    void onCode(Code<Trait> *c) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::Code)) {
            m_func(c);
        }
    }

    //! Process inline code.
    void onInlineCode(Code<Trait> *c) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::Code)) {
            m_func(c);
        }
    }

    //! Process blockquote.
    void onBlockquote(Blockquote<Trait> *b) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::Blockquote)) {
            m_func(b);
        }

        if (inc.nextAllowed()) {
            Visitor<Trait>::onBlockquote(b);
        }
    }

    //! Process list.
    void onList(List<Trait> *l) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::List)) {
            m_func(l);
        }

        for (auto it = l->items().cbegin(), last = l->items().cend(); it != last; ++it) {
            if ((*it)->type() == ItemType::ListItem && inc.nextAllowed()) {
                onListItem(static_cast<ListItem<Trait> *>(it->get()), true);
            }
        }
    }

    //! Process table.
    void onTable(Table<Trait> *t) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::Table)) {
            m_func(t);
        }

        if (!t->isEmpty() && inc.nextAllowed()) {
            int columns = 0;

            for (auto th = (*t->rows().cbegin())->cells().cbegin(), last = (*t->rows().cbegin())->cells().cend(); th != last; ++th) {
                this->onTableCell(th->get());

                ++columns;
            }

            for (auto r = std::next(t->rows().cbegin()), rlast = t->rows().cend(); r != rlast; ++r) {
                int i = 0;

                for (auto c = (*r)->cells().cbegin(), clast = (*r)->cells().cend(); c != clast; ++c) {
                    this->onTableCell(c->get());

                    ++i;

                    if (i == columns) {
                        break;
                    }
                }
            }
        }
    }

    //! Process anchor.
    void onAnchor(Anchor<Trait> *a) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::Anchor)) {
            m_func(a);
        }
    }

    //! Process raw HTML block.
    void onRawHtml(RawHtml<Trait> *h) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::RawHtml)) {
            m_func(h);
        }
    }

    //! Process horizontal line.
    void onHorizontalLine(HorizontalLine<Trait> *l) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::HorizontalLine)) {
            m_func(l);
        }
    }

    //! Process link.
    void onLink(Link<Trait> *l) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::Link)) {
            m_func(l);
        }

        if (inc.nextAllowed()) {
            if (!l->img()->isEmpty()) {
                onImage(l->img().get());
            } else if (l->p() && !l->p()->isEmpty()) {
                onParagraph(l->p().get(), false);
            }
        }
    }

    //! Process Image.
    void onImage(Image<Trait> *i) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::Image)) {
            m_func(i);
        }
    }

    //! Process footnote reference.
    void onFootnoteRef(FootnoteRef<Trait> *ref) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::FootnoteRef)) {
            m_func(ref);
        }
    }

    //! Process list item.
    void onListItem(ListItem<Trait> *i, bool) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::ListItem)) {
            m_func(i);
        }

        if (inc.nextAllowed()) {
            Visitor<Trait>::onListItem(i, true);
        }
    }

    //! Process table cell.
    void onTableCell(TableCell<Trait> *c) override
    {
        Visitor<Trait>::onTableCell(c);
    }

    //! Process footnote.
    void onFootnote(Footnote<Trait> *f) override
    {
        IncrementNestingLevel inc(m_currentNestingLevel, m_maxNestingLevel, m_types);

        if (inc.allowed(ItemType::Footnote)) {
            m_func(f);
        }

        if (inc.nextAllowed()) {
            Visitor<Trait>::onFootnote(f);
        }
    }

    //! Process all footnotes.
    virtual void onFootnotes(std::shared_ptr<Document<Trait>> doc)
    {
        for (const auto &f : doc->footnotesMap()) {
            this->onFootnote(f.second.get());
        }
    }

protected:
    //! Current nesting level, increments/decrements during walking through te document.
    unsigned int m_currentNestingLevel = 0;
    //! Maximum allowed nesting level.
    unsigned int m_maxNestingLevel = 0;
    //! List of types that should be processed.
    const typename Trait::template Vector<ItemType> &m_types;
    //! Functor that will be invoked if all circumstances will be observed.
    ItemFunctor<Trait> m_func = {};

    //! Auxiliary structure to increment/decrement nesting level during walking through
    //! the document, and checking circumstances for the algorithm.
    struct IncrementNestingLevel {
        IncrementNestingLevel(unsigned int &l,
                              unsigned int m,
                              const typename Trait::template Vector<ItemType> &t)
            : m_level(l)
            , m_maxNestingLevel(m)
            , m_types(t)
        {
            ++m_level;
        }

        ~IncrementNestingLevel()
        {
            --m_level;
        }

        //! \return Is this item type allowed at this nesting level.
        bool allowed(ItemType t) const
        {
            return ((m_maxNestingLevel == 0 || m_level <= m_maxNestingLevel) &&
                    std::find(m_types.cbegin(), m_types.cend(), t) != m_types.cend());
        }

        //! \return Is next nesting level allowed.
        bool nextAllowed() const
        {
            return (m_maxNestingLevel == 0 || m_level + 1 <= m_maxNestingLevel);
        }

        //! Reference to nesting level.
        unsigned int &m_level;
        //! Maximum allowed nesting level.
        unsigned int m_maxNestingLevel;
        //! Reference to list of allowed types.
        const typename Trait::template Vector<ItemType> &m_types;
    }; // struct IncrementNestingLevel
}; // class HtmlVisitor

} /* namespace details */

//! Calls function for each item in the document with the given type.
template<class Trait>
inline void forEach(
    //! Vector of item's types to be processed.
    const typename Trait::template Vector<ItemType> &types,
    //! Document.
    std::shared_ptr<Document<Trait>> doc,
    //! Functor object.
    ItemFunctor<Trait> func,
    //! Maximun nesting level.
    //! 0 means infinity, 1 - only top level items...
    unsigned int maxNestingLevel = 0)
{
    details::AlgoVisitor<Trait> v(maxNestingLevel, types, func);

    v.walk(doc);
}

} /* namespace MD */

#endif // MD4QT_MD_ALGO_H_INCLUDED
