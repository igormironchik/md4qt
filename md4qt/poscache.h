/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef MD4QT_MD_POSCACHE_HPP_INCLUDED
#define MD4QT_MD_POSCACHE_HPP_INCLUDED

#include "visitor.h"

// C++ include.
#include <algorithm>
#include <cassert>
#include <vector>

namespace MD
{

namespace details
{

//
// PosRange
//

//! Cached position of Item.
template<class Trait>
struct PosRange {
    //! Start column
    long long int m_startColumn = -1;
    //! Start line.
    long long int m_startLine = -1;
    //! End column.
    long long int m_endColumn = -1;
    //! End line.
    long long int m_endLine = -1;
    //! Pointer to this item.
    Item<Trait> *m_item = nullptr;
    //! List of children.
    std::vector<PosRange<Trait>> m_children = {};

    //! \return Is position valid.
    inline bool isValidPos() const
    {
        return m_startColumn > -1 && m_startLine > -1 && m_endColumn > -1 && m_endLine > -1;
    }
};

// Look at this equality operator like on rectangles intersection.
// If rectangles in text intersect then rectangles are equal.
template<class Trait>
bool operator==(const PosRange<Trait> &l, const PosRange<Trait> &r)
{
    return (l.m_startLine <= r.m_endLine && l.m_endLine >= r.m_startLine &&
           (l.m_startLine == r.m_endLine && l.m_endLine == r.m_startLine ?
                l.m_endColumn >= r.m_startColumn && l.m_startColumn <= r.m_endColumn : true));
}

template<class Trait>
bool operator<(const PosRange<Trait> &l, const PosRange<Trait> &r)
{
    return (l.m_endLine < r.m_startLine ||
            (l.m_endLine == r.m_startLine && l.m_endColumn < r.m_startColumn));
}

} /* namespace details */

//
// PosCache
//

//! Cache of Markdown items to be accessed via position.
template<class Trait>
class PosCache : public MD::Visitor<Trait>
{
public:
    PosCache() = default;
    ~PosCache() override = default;

    //! Initialize m_cache with the give document.
    //! \note Document should not be recursive.
    virtual void initialize(std::shared_ptr<MD::Document<Trait>> doc)
    {
        m_cache.clear();

        if (doc) {
            Visitor<Trait>::process(doc);

            for (auto it = doc->footnotesMap().cbegin(), last = doc->footnotesMap().cend(); it != last; ++it) {
                onFootnote(it->second.get());
            }

            for (auto it = doc->labeledLinks().cbegin(), last = doc->labeledLinks().cend(); it != last; ++it) {
                onReferenceLink(it->second.get());
            }
        }
    }

    //! Vector with items, where front is a top-level item,
    //! and back is most nested child.
    using Items = typename Trait::template Vector<Item<Trait> *>;

    //! \return First occurense of Markdown item with all first children by the give position.
    Items findFirstInCache(const MD::WithPosition &pos) const
    {
        Items res;

        details::PosRange<Trait> tmp{pos.startColumn(), pos.startLine(),
                                     pos.endColumn(), pos.endLine()};

        findFirstInCache(m_cache, tmp, res);

        return res;
    }

protected:
    //! Find in cache an item with the given position.
    details::PosRange<Trait> *findInCache(
            //! Cache of position.
            std::vector<details::PosRange<Trait>> &vec,
            //! Position of sought-for item.
            const details::PosRange<Trait> &pos) const
    {
        const auto it = std::lower_bound(vec.begin(), vec.end(), pos);

        if (it != vec.end() && *it == pos) {
            if (!it->m_children.empty()) {
                auto nested = findInCache(it->m_children, pos);

                return (nested ? nested : &(*it));
            } else {
                return &(*it);
            }
        } else {
            return nullptr;
        }
    }

    //! Find in cache items with the given position with all parents.
    void findFirstInCache(
            //! Cache.
            const std::vector<details::PosRange<Trait>> &vec,
            //! Position of sought-for item.
            const details::PosRange<Trait> &pos,
            //! Reference to result of search.
            Items &res) const
    {
        const auto it = std::lower_bound(vec.begin(), vec.end(), pos);

        if (it != vec.end() && *it == pos) {
            res.push_back(it->m_item);

            if (!it->m_children.empty()) {
                findFirstInCache(it->m_children, pos, res);
            }
        }
    }

    //! Insert in cache.
    void insertInCache(
            //! Position for insertion.
            const details::PosRange<Trait> &item,
            //! Should we sord when insert top-level item, or we can be sure that this item is last?
            bool sort = false)
    {
        if (!m_skipInCache) {
            assert(item.isValidPos());

            auto pos = findInCache(m_cache, item);

            if (pos) {
                pos->m_children.push_back(item);
            } else {
                if (sort) {
                    const auto it = std::upper_bound(m_cache.begin(), m_cache.end(), item);

                    if (it != m_cache.end()) {
                        m_cache.insert(it, item);
                    } else {
                        m_cache.push_back(item);
                    }
                } else {
                    m_cache.push_back(item);
                }
            }
        }
    }

protected:
    //! Cache user defined item.
    void onUserDefined(Item<Trait> *i) override
    {
        details::PosRange<Trait> r{i->startColumn(), i->startLine(), i->endColumn(), i->endLine(), i};

        insertInCache(r);
    }

    //! Cache shortcut link.
    virtual void onReferenceLink(
        //! Link.
        Link<Trait> *l)
    {
        details::PosRange<Trait> r{l->startColumn(), l->startLine(), l->endColumn(), l->endLine(), l};

        insertInCache(r, true);
    }

    void onAddLineEnding() override
    {
    }

    //! Cache text item.
    void onText(Text<Trait> *t) override
    {
        details::PosRange<Trait> r{t->openStyles().empty() ? t->startColumn() : t->openStyles().front().startColumn(),
                                   t->startLine(),
                                   t->closeStyles().empty() ? t->endColumn() : t->closeStyles().back().endColumn(),
                                   t->endLine(),
                                   t};

        insertInCache(r);
    }

    //! Cache LaTeX math expression.
    void onMath(Math<Trait> *m) override
    {
        auto startColumn = m->startDelim().startColumn();
        auto startLine = m->startDelim().startLine();
        auto endColumn = m->endDelim().endColumn();
        auto endLine = m->endDelim().endLine();

        if (!m->openStyles().empty()) {
            startColumn = m->openStyles().front().startColumn();
            startLine = m->openStyles().front().startLine();
        }

        if (!m->closeStyles().empty()) {
            endColumn = m->closeStyles().back().endColumn();
            endLine = m->closeStyles().back().endLine();
        }

        details::PosRange<Trait> r{startColumn, startLine, endColumn, endLine, m};

        insertInCache(r);
    }

    void onLineBreak(LineBreak<Trait> *) override
    {
    }

    //! Cache paragraph.
    void onParagraph(Paragraph<Trait> *p, bool wrap) override
    {
        details::PosRange<Trait> r{p->startColumn(), p->startLine(), p->endColumn(), p->endLine(), p};

        insertInCache(r);

        Visitor<Trait>::onParagraph(p, wrap);
    }

    //! Cache heading.
    void onHeading(Heading<Trait> *h) override
    {
        details::PosRange<Trait> r{h->startColumn(), h->startLine(), h->endColumn(), h->endLine(), h};

        insertInCache(r);

        if (h->text() && !h->text()->isEmpty()) {
            onParagraph(h->text().get(), false);
        }
    }

    //! Cache code.
    void onCode(Code<Trait> *c) override
    {
        auto startColumn = c->isFensedCode() ? c->startDelim().startColumn() : c->startColumn();
        auto startLine = c->isFensedCode() ? c->startDelim().startLine() : c->startLine();
        auto endColumn = c->isFensedCode() && c->endDelim().endColumn() > -1 ? c->endDelim().endColumn() : c->endColumn();
        auto endLine = c->isFensedCode() && c->endDelim().endLine() > -1 ? c->endDelim().endLine() : c->endLine();

        details::PosRange<Trait> r{startColumn, startLine, endColumn, endLine, c};

        insertInCache(r);
    }

    //! Cache inline code.
    void onInlineCode(Code<Trait> *c) override
    {
        auto startColumn = c->startDelim().startColumn();
        auto startLine = c->startDelim().startLine();
        auto endColumn = c->endDelim().endColumn();
        auto endLine = c->endDelim().endLine();

        if (!c->openStyles().empty()) {
            startColumn = c->openStyles().front().startColumn();
            startLine = c->openStyles().front().startLine();
        }

        if (!c->closeStyles().empty()) {
            endColumn = c->closeStyles().back().endColumn();
            endLine = c->closeStyles().back().endLine();
        }

        details::PosRange<Trait> r{startColumn, startLine, endColumn, endLine, c};

        insertInCache(r);
    }

    //! Cache blockquote.
    void onBlockquote(Blockquote<Trait> *b) override
    {
        details::PosRange<Trait> r{b->startColumn(), b->startLine(), b->endColumn(), b->endLine(), b};

        insertInCache(r);

        Visitor<Trait>::onBlockquote(b);
    }

    //! Cache list.
    void onList(List<Trait> *l) override
    {
        bool first = true;

        details::PosRange<Trait> r{l->startColumn(), l->startLine(), l->endColumn(), l->endLine(), l};

        insertInCache(r);

        for (auto it = l->items().cbegin(), last = l->items().cend(); it != last; ++it) {
            if ((*it)->type() == ItemType::ListItem) {
                onListItem(static_cast<ListItem<Trait> *>(it->get()), first);

                first = false;
            }
        }
    }

    //! Cache table.
    void onTable(Table<Trait> *t) override
    {
        details::PosRange<Trait> r{t->startColumn(), t->startLine(), t->endColumn(), t->endLine(), t};

        insertInCache(r);

        if (!t->isEmpty()) {
            int columns = 0;

            for (auto th = (*t->rows().cbegin())->cells().cbegin(), last = (*t->rows().cbegin())->cells().cend(); th != last; ++th) {
                Visitor<Trait>::onTableCell(th->get());

                ++columns;
            }

            for (auto r = std::next(t->rows().cbegin()), rlast = t->rows().cend(); r != rlast; ++r) {
                int i = 0;

                for (auto c = (*r)->cells().cbegin(), clast = (*r)->cells().cend(); c != clast; ++c) {
                    Visitor<Trait>::onTableCell(c->get());

                    ++i;

                    if (i == columns) {
                        break;
                    }
                }
            }
        }
    }

    void onAnchor(Anchor<Trait> *) override
    {
    }

    //! Cache raw HTML.
    void onRawHtml(RawHtml<Trait> *h) override
    {
        details::PosRange<Trait> r{h->startColumn(), h->startLine(), h->endColumn(), h->endLine(), h};

        insertInCache(r);
    }

    //! Cache horizontal line.
    void onHorizontalLine(HorizontalLine<Trait> *l) override
    {
        details::PosRange<Trait> r{l->startColumn(), l->startLine(), l->endColumn(), l->endLine(), l};

        insertInCache(r);
    }

    //! Cache link.
    void onLink(Link<Trait> *l) override
    {
        auto startColumn = l->startColumn();
        auto startLine = l->startLine();
        auto endColumn = l->endColumn();
        auto endLine = l->endLine();

        if (!l->openStyles().empty()) {
            startColumn = l->openStyles().front().startColumn();
            startLine = l->openStyles().front().startLine();
        }

        if (!l->closeStyles().empty()) {
            endColumn = l->closeStyles().back().endColumn();
            endLine = l->closeStyles().back().endLine();
        }

        details::PosRange<Trait> r{startColumn, startLine, endColumn, endLine, l};

        insertInCache(r);

        if (l->p()) {
            m_skipInCache = true;
            onParagraph(l->p().get(), true);
            m_skipInCache = false;
        }
    }

    //! Cache image.
    void onImage(Image<Trait> *i) override
    {
        auto startColumn = i->startColumn();
        auto startLine = i->startLine();
        auto endColumn = i->endColumn();
        auto endLine = i->endLine();

        if (!i->openStyles().empty()) {
            startColumn = i->openStyles().front().startColumn();
            startLine = i->openStyles().front().startLine();
        }

        if (!i->closeStyles().empty()) {
            endColumn = i->closeStyles().back().endColumn();
            endLine = i->closeStyles().back().endLine();
        }

        details::PosRange<Trait> r{startColumn, startLine, endColumn, endLine, i};

        insertInCache(r);

        if (i->p()) {
            m_skipInCache = true;
            onParagraph(i->p().get(), true);
            m_skipInCache = false;
        }
    }

    //! Cache footnote reference.
    void onFootnoteRef(FootnoteRef<Trait> *ref) override
    {
        auto startColumn = ref->startColumn();
        auto startLine = ref->startLine();
        auto endColumn = ref->endColumn();
        auto endLine = ref->endLine();

        if (!ref->openStyles().empty()) {
            startColumn = ref->openStyles().front().startColumn();
            startLine = ref->openStyles().front().startLine();
        }

        if (!ref->closeStyles().empty()) {
            endColumn = ref->closeStyles().back().endColumn();
            endLine = ref->closeStyles().back().endLine();
        }

        details::PosRange<Trait> r{startColumn, startLine, endColumn, endLine, ref};

        insertInCache(r);
    }

    //! Cache footnote.
    void onFootnote(Footnote<Trait> *f) override
    {
        details::PosRange<Trait> r{f->startColumn(), f->startLine(), f->endColumn(), f->endLine(), f};

        insertInCache(r, true);

        Visitor<Trait>::onFootnote(f);
    }

    //! Cache list item.
    void onListItem(ListItem<Trait> *l, bool first) override
    {
        details::PosRange<Trait> r{l->startColumn(), l->startLine(), l->endColumn(), l->endLine(), l};

        insertInCache(r);

        Visitor<Trait>::onListItem(l, first);
    }

protected:
    //! Cache.
    std::vector<details::PosRange<Trait>> m_cache;
    //! Skip adding in cache.
    bool m_skipInCache = false;
}; // class PosCache

} /* namespace MD */

#endif // MD4QT_MD_POSCACHE_HPP_INCLUDED
