/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef MD4QT_MD_HTML_HPP_INCLUDED
#define MD4QT_MD_HTML_HPP_INCLUDED

// md4qt include.
#include "doc.h"
#include "visitor.h"

// C++ include.
#include <algorithm>
#include <string>
#include <utility>

namespace MD
{

namespace details
{

//
// HtmlVisitor
//

//! HTML visitor interface to walk through Document.
template<class Trait>
class HtmlVisitor : public Visitor<Trait>
{
public:
    HtmlVisitor() = default;
    ~HtmlVisitor() override = default;

    //! Walk through the document.
    virtual typename Trait::String toHtml(std::shared_ptr<Document<Trait>> doc,
                                          const typename Trait::String &hrefForRefBackImage,
                                          bool wrappedInArticle = true)
    {
        m_isWrappedInArticle = wrappedInArticle;

        m_html.clear();
        m_fns.clear();

        this->process(doc);

        onFootnotes(hrefForRefBackImage);

        return m_html;
    }

protected:
    //! Insert into HTML tags for opening styles.
    virtual void openStyle(const typename ItemWithOpts<Trait>::Styles &styles)
    {
        for (const auto &s : styles) {
            switch (s.style()) {
            case TextOption::BoldText:
                m_html.push_back(Trait::latin1ToString("<strong>"));
                break;

            case TextOption::ItalicText:
                m_html.push_back(Trait::latin1ToString("<em>"));
                break;

            case TextOption::StrikethroughText:
                m_html.push_back(Trait::latin1ToString("<del>"));
                break;

            default:
                break;
            }
        }
    }

    //! Insert into HTML tags for closing styles.
    virtual void closeStyle(const typename ItemWithOpts<Trait>::Styles &styles)
    {
        for (const auto &s : styles) {
            switch (s.style()) {
            case TextOption::BoldText:
                m_html.push_back(Trait::latin1ToString("</strong>"));
                break;

            case TextOption::ItalicText:
                m_html.push_back(Trait::latin1ToString("</em>"));
                break;

            case TextOption::StrikethroughText:
                m_html.push_back(Trait::latin1ToString("</del>"));
                break;

            default:
                break;
            }
        }
    }

    //! Handle new line in HTML.
    void onAddLineEnding() override
    {
        if (!m_justCollectFootnoteRefs)
            m_html.push_back(Trait::latin1ToString("\n"));
    }

    //! Handle text item.
    void onText(
        //! Text.
        Text<Trait> *t) override
    {
        if (!m_justCollectFootnoteRefs) {
            openStyle(t->openStyles());

            m_html.push_back(prepareTextForHtml(t->text()));

            closeStyle(t->closeStyles());
        }
    }

    //! Handle LaTeX math expression.
    void onMath(
        //! Math.
        Math<Trait> *m) override
    {
        if (!m_justCollectFootnoteRefs) {
            openStyle(m->openStyles());

            m_html.push_back(m->isInline() ? Trait::latin1ToString("$") : Trait::latin1ToString("$$"));
            m_html.push_back(prepareTextForHtml(m->expr()));
            m_html.push_back(m->isInline() ? Trait::latin1ToString("$") : Trait::latin1ToString("$$"));

            closeStyle(m->closeStyles());
        }
    }

    //! Handle line break.
    void onLineBreak(
        //! Linebreak.
        LineBreak<Trait> *) override
    {
        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("<br />"));
        }
    }

    //! Handle paragraph.
    void onParagraph(
        //! Paragraph.
        Paragraph<Trait> *p,
        //! Wrap this paragraph with something or no? It's useful to not wrap standalone
        //! paragraph in list item, for example.
        bool wrap) override
    {
        if (wrap && !m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("<p dir=\"auto\">"));
        }

        Visitor<Trait>::onParagraph(p, wrap);

        if (wrap && !m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("</p>"));
        }
    }

    //! Handle heading.
    void onHeading(
        //! Heading.
        Heading<Trait> *h) override
    {
        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("\n"));
        }

        switch (h->level()) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            onHeading(h, Trait::latin1ToString("h") + Trait::latin1ToString(std::to_string(h->level()).c_str()));
            break;

        default:
            break;
        }

        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("\n"));
        }
    }

    //! Handle code block.
    void onCode(
        //! Code.
        Code<Trait> *c) override
    {
        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("\n"));
            m_html.push_back(Trait::latin1ToString("<pre><code"));

            if (!c->syntax().isEmpty()) {
                m_html.push_back(Trait::latin1ToString(" class=\"language-"));
                m_html.push_back(c->syntax());
                m_html.push_back(Trait::latin1ToString("\""));
            }

            m_html.push_back(Trait::latin1ToString(">"));
            m_html.push_back(prepareTextForHtml(c->text()));
            m_html.push_back(Trait::latin1ToString("</code></pre>"));
            m_html.push_back(Trait::latin1ToString("\n"));
        }
    }

    //! Handle inline code.
    void onInlineCode(
        //! Code.
        Code<Trait> *c) override
    {
        if (!m_justCollectFootnoteRefs) {
            openStyle(c->openStyles());

            m_html.push_back(Trait::latin1ToString("<code>"));

            m_html.push_back(prepareTextForHtml(c->text()));

            m_html.push_back(Trait::latin1ToString("</code>"));

            closeStyle(c->closeStyles());
        }
    }

    //! Handle blockquote.
    void onBlockquote(
        //! Blockquote.
        Blockquote<Trait> *b) override
    {
        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("\n<blockquote>"));
        }

        Visitor<Trait>::onBlockquote(b);

        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("</blockquote>\n"));
        }
    }

    //! Handle list.
    void onList(
        //! List.
        List<Trait> *l) override
    {
        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("\n"));
        }

        typename ListItem<Trait>::ListType type = ListItem<Trait>::Ordered;
        bool first = true;

        for (auto it = l->items().cbegin(), last = l->items().cend(); it != last; ++it) {
            if ((*it)->type() == ItemType::ListItem) {
                auto *item = static_cast<ListItem<Trait> *>(it->get());

                if (first) {
                    type = item->listType();

                    if (type == ListItem<Trait>::Ordered) {
                        if (!m_justCollectFootnoteRefs) {
                            m_html.push_back(Trait::latin1ToString("<ol"));

                            if (item->isTaskList()) {
                                m_html.push_back(Trait::latin1ToString(" class=\"contains-task-list\""));
                            }

                            m_html.push_back(Trait::latin1ToString(" dir=\"auto\">\n"));
                        }
                    } else {
                        if (!m_justCollectFootnoteRefs) {
                            m_html.push_back(Trait::latin1ToString("<ul"));

                            if (item->isTaskList()) {
                                m_html.push_back(Trait::latin1ToString(" class=\"contains-task-list\""));
                            }

                            m_html.push_back(Trait::latin1ToString(" dir=\"auto\">\n"));
                        }
                    }
                }

                onListItem(item, first);

                first = false;
            }
        }

        if (!first) {
            if (!m_justCollectFootnoteRefs) {
                if (type == ListItem<Trait>::Ordered) {
                    m_html.push_back(Trait::latin1ToString("</ol>\n"));
                } else {
                    m_html.push_back(Trait::latin1ToString("</ul>\n"));
                }
            }
        }
    }

    //! Handle table.
    void onTable(
        //! Table.
        Table<Trait> *t) override
    {
        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("\n"));
        }

        if (!t->isEmpty()) {
            if (!m_justCollectFootnoteRefs) {
                m_html.push_back(Trait::latin1ToString("<table><thead><tr>\n"));
            }

            int columns = 0;

            for (auto th = (*t->rows().cbegin())->cells().cbegin(), last = (*t->rows().cbegin())->cells().cend(); th != last; ++th) {
                if (!m_justCollectFootnoteRefs) {
                    m_html.push_back(Trait::latin1ToString("<th"));
                    m_html.push_back(tableAlignmentToHtml(t->columnAlignment(columns)));
                    m_html.push_back(Trait::latin1ToString(" dir=\"auto\">\n"));
                }

                this->onTableCell(th->get());

                if (!m_justCollectFootnoteRefs) {
                    m_html.push_back(Trait::latin1ToString("\n</th>\n"));
                }

                ++columns;
            }

            if (!m_justCollectFootnoteRefs) {
                m_html.push_back(Trait::latin1ToString("</tr></thead><tbody>\n"));
            }

            for (auto r = std::next(t->rows().cbegin()), rlast = t->rows().cend(); r != rlast; ++r) {
                if (!m_justCollectFootnoteRefs) {
                    m_html.push_back(Trait::latin1ToString("<tr>\n"));
                }

                int i = 0;

                for (auto c = (*r)->cells().cbegin(), clast = (*r)->cells().cend(); c != clast; ++c) {
                    if (!m_justCollectFootnoteRefs) {
                        m_html.push_back(Trait::latin1ToString("\n<td"));
                        m_html.push_back(tableAlignmentToHtml(t->columnAlignment(i)));
                        m_html.push_back(Trait::latin1ToString(" dir=\"auto\">\n"));
                    }

                    this->onTableCell(c->get());

                    if (!m_justCollectFootnoteRefs) {
                        m_html.push_back(Trait::latin1ToString("\n</td>\n"));
                    }

                    ++i;

                    if (i == columns) {
                        break;
                    }
                }

                if (!m_justCollectFootnoteRefs) {
                    for (; i < columns; ++i) {
                        m_html.push_back(Trait::latin1ToString("<td dir=\"auto\"></td>"));
                    }

                    m_html.push_back(Trait::latin1ToString("\n</tr>\n"));
                }
            }

            if (!m_justCollectFootnoteRefs) {
                m_html.push_back(Trait::latin1ToString("</tbody></table>"));
            }
        }

        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("\n"));
        }
    }

    //! Handle anchor.
    void onAnchor(
        //! Anchor.
        Anchor<Trait> *a) override
    {
        if (!m_justCollectFootnoteRefs && m_isWrappedInArticle) {
            m_html.push_back(Trait::latin1ToString("\n<div id=\""));
            m_html.push_back(a->label());
            m_html.push_back(Trait::latin1ToString("\"></div>\n"));
        }
    }

    //! Handle raw HTML.
    void onRawHtml(
        //! Raw HTML.
        RawHtml<Trait> *h) override
    {
        if (!m_justCollectFootnoteRefs) {
            openStyle(h->openStyles());

            m_html.push_back(h->text());

            closeStyle(h->closeStyles());
        }
    }

    //! Handle horizontal line.
    void onHorizontalLine(
        //! Horizontal line.
        HorizontalLine<Trait> *) override
    {
        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("<hr />"));
        }
    }

    //! Handle link.
    void onLink(
        //! Link.
        Link<Trait> *l) override
    {
        typename Trait::String url = l->url();

        const auto lit = this->m_doc->labeledLinks().find(url);

        if (lit != this->m_doc->labeledLinks().cend()) {
            url = lit->second->url();
        }

        if (std::find(this->m_anchors.cbegin(), this->m_anchors.cend(), url) != this->m_anchors.cend()) {
            url = Trait::latin1ToString("#") + url;
        } else if (url.startsWith(Trait::latin1ToString("#"))) {
            const auto it = this->m_doc->labeledHeadings().find(url);

            if (it == this->m_doc->labeledHeadings().cend()) {
                auto path = static_cast<Anchor<Trait> *>(this->m_doc->items().at(0).get())->label();
                const auto sp = path.lastIndexOf(Trait::latin1ToString("/"));
                path.remove(sp, path.length() - sp);
                const auto p = url.indexOf(path) - 1;
                url.remove(p, url.length() - p);
            } else {
                url = it->second->label();
            }
        }

        if (!m_justCollectFootnoteRefs) {
            openStyle(l->openStyles());

            m_html.push_back(Trait::latin1ToString("<a href=\""));
            m_html.push_back(url);
            m_html.push_back(Trait::latin1ToString("\">"));
        }

        if (l->p() && !l->p()->isEmpty()) {
            onParagraph(l->p().get(), false);
        } else if (!l->img()->isEmpty()) {
            if (!m_justCollectFootnoteRefs) {
                onImage(l->img().get());
            }
        } else if (!l->text().isEmpty()) {
            if (!m_justCollectFootnoteRefs) {
                m_html.push_back(prepareTextForHtml(l->text()));
            }
        } else {
            if (!m_justCollectFootnoteRefs) {
                m_html.push_back(prepareTextForHtml(l->url()));
            }
        }

        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("</a>"));

            closeStyle(l->closeStyles());
        }
    }

    //! Handle image.
    void onImage(
        //! Image.
        Image<Trait> *i) override
    {
        if (!m_justCollectFootnoteRefs) {
            openStyle(i->openStyles());

            m_html.push_back(Trait::latin1ToString("<img src=\""));
            m_html.push_back(i->url());
            m_html.push_back(Trait::latin1ToString("\" alt=\""));
            m_html.push_back(prepareTextForHtml(i->text()));
            m_html.push_back(Trait::latin1ToString("\" style=\"max-width:100%;\" />"));

            closeStyle(i->closeStyles());
        }
    }

    //! Handle footnote reference.
    void onFootnoteRef(
        //! Footnote reference.
        FootnoteRef<Trait> *ref) override
    {
        const auto fit = this->m_doc->footnotesMap().find(ref->id());

        if (fit != this->m_doc->footnotesMap().cend()) {
            const auto r = std::find_if(m_fns.begin(), m_fns.end(), [&ref](const auto &stuff) {
                return ref->id() == stuff.m_id;
            });

            if (!m_justCollectFootnoteRefs) {
                openStyle(ref->openStyles());

                m_html.push_back(Trait::latin1ToString("<sup>"));
                m_html.push_back(Trait::latin1ToString("<a href=\"#"));
                m_html.push_back(ref->id());
                m_html.push_back(Trait::latin1ToString("\" id=\"ref-"));
                m_html.push_back(ref->id());
                m_html.push_back(Trait::latin1ToString("-"));
            }

            if (r == m_fns.end()) {
                if (!m_justCollectFootnoteRefs) {
                    m_html.push_back(Trait::latin1ToString("1"));
                }
            } else {
                if (!m_justCollectFootnoteRefs) {
                    m_html.push_back(Trait::latin1ToString(std::to_string(++(r->m_current)).c_str()));
                }

                if (!m_dontIncrementFootnoteCount) {
                    ++(r->m_count);
                }
            }

            if (!m_justCollectFootnoteRefs) {
                m_html.push_back(Trait::latin1ToString("\">"));
            }

            if (r == m_fns.end()) {
                if (!m_justCollectFootnoteRefs) {
                    m_html.push_back(Trait::latin1ToString(std::to_string(m_fns.size() + 1).c_str()));
                }

                m_fns.push_back({ref->id(), 1, 0});
            } else if (!m_justCollectFootnoteRefs)
                m_html.push_back(Trait::latin1ToString(std::to_string(std::distance(m_fns.begin(), r) + 1).c_str()));

            if (!m_justCollectFootnoteRefs) {
                m_html.push_back(Trait::latin1ToString("</a></sup>"));

                closeStyle(ref->closeStyles());
            }
        } else
            onText(static_cast<Text<Trait> *>(ref));
    }

    //! Handle list item.
    void onListItem(
        //! List item.
        ListItem<Trait> *i,
        //! Is this item first in the list?
        bool first) override
    {
        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("<li"));

            if (i->isTaskList()) {
                m_html.push_back(
                    Trait::latin1ToString(" class=\"task-list-item\"><input "
                                          "type=\"checkbox\" id=\"\" disabled=\"\" class=\"task-list-item-checkbox\""));

                if (i->isChecked()) {
                    m_html.push_back(Trait::latin1ToString(" checked=\"\""));
                }
            }

            if (i->listType() == ListItem<Trait>::Ordered && first) {
                m_html.push_back(Trait::latin1ToString(" value=\""));
                m_html.push_back(Trait::latin1ToString(std::to_string(i->startNumber()).c_str()));
                m_html.push_back(Trait::latin1ToString("\""));
            }

            m_html.push_back(Trait::latin1ToString(">\n"));
        }

        Visitor<Trait>::onListItem(i, first);

        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("</li>\n"));
        }
    }

    //! Handle heading.
    virtual void onHeading(
        //! Heading.
        Heading<Trait> *h,
        //! Heading tag.
        const typename Trait::String &ht)
    {
        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("<"));
            m_html.push_back(ht);
            m_html.push_back(headingIdToHtml(h));
            m_html.push_back(Trait::latin1ToString(" dir=\"auto\">"));
        }

        if (h->text().get()) {
            onParagraph(h->text().get(), false);
        }

        if (!m_justCollectFootnoteRefs) {
            m_html.push_back(Trait::latin1ToString("</"));
            m_html.push_back(ht);
            m_html.push_back(Trait::latin1ToString(">"));
        }
    }

    //! Handle footnotes.
    virtual void onFootnotes(const typename Trait::String &hrefForRefBackImage)
    {
        if (!m_fns.empty()) {
            m_html.push_back(Trait::latin1ToString("<section class=\"footnotes\"><ol dir=\"auto\">"));
        }

        int i = 1;

        const auto tmpm_fns = m_fns;
        m_justCollectFootnoteRefs = true;

        for (const auto &id : tmpm_fns) {
            const auto fit = this->m_doc->footnotesMap().find(id.m_id);

            if (fit != this->m_doc->footnotesMap().cend()) {
                this->onFootnote(fit->second.get());
            }
        }

        m_justCollectFootnoteRefs = false;
        m_dontIncrementFootnoteCount = true;

        for (const auto &id : m_fns) {
            m_html.push_back(Trait::latin1ToString("<li id=\""));
            m_html.push_back(id.m_id);
            m_html.push_back(Trait::latin1ToString("\">"));
            ++i;

            const auto fit = this->m_doc->footnotesMap().find(id.m_id);

            if (fit != this->m_doc->footnotesMap().cend()) {
                this->onFootnote(fit->second.get());

                if (!hrefForRefBackImage.isEmpty()) {
                    typename Trait::String backRef;
                    long long int backRefPos = m_html.endsWith(Trait::latin1ToString("</p>")) ? 4 : 0;

                    for (long long int i = 1; i <= id.m_count; ++i) {
                        backRef.push_back(Trait::latin1ToString("<a href=\"#ref-"));
                        backRef.push_back(id.m_id);
                        backRef.push_back(Trait::latin1ToString("-"));
                        backRef.push_back(Trait::latin1ToString(std::to_string(i).c_str()));
                        backRef.push_back(Trait::latin1ToString("\"><img src=\""));
                        backRef.push_back(hrefForRefBackImage);
                        backRef.push_back(Trait::latin1ToString("\" /></a>"));
                    }

                    m_html.insert(m_html.length() - backRefPos, backRef);
                }

                m_html.push_back(Trait::latin1ToString("</li>"));
            }
        }

        if (!m_fns.empty()) {
            m_html.push_back(Trait::latin1ToString("</ol></section>\n"));
        }
    }

    //! \return HTML content for heading's ID.
    virtual typename Trait::String headingIdToHtml(Heading<Trait> *h)
    {
        typename Trait::String html;

        if (h->isLabeled()) {
            html.push_back(Trait::latin1ToString(" id=\""));
            auto label = h->label();
            if (label.startsWith(Trait::latin1ToString("#"))) {
                label.remove(0, 1);
            }
            html.push_back(label);
            html.push_back(Trait::latin1ToString("\""));
        }

        return html;
    }

    //! Prepare text to insert into HTML content.
    virtual typename Trait::String prepareTextForHtml(const typename Trait::String &t)
    {
        auto tmp = t;
        tmp.replace(Trait::latin1ToChar('&'), Trait::latin1ToString("&amp;"));
        tmp.replace(Trait::latin1ToChar('<'), Trait::latin1ToString("&lt;"));
        tmp.replace(Trait::latin1ToChar('>'), Trait::latin1ToString("&gt;"));

        return tmp;
    }

    //! \return HTML content for table alignment.
    virtual typename Trait::String tableAlignmentToHtml(typename Table<Trait>::Alignment a)
    {
        typename Trait::String html;

        switch (a) {
        case Table<Trait>::AlignLeft:
            html.push_back(Trait::latin1ToString(" align=\"left\""));
            break;

        case Table<Trait>::AlignCenter:
            html.push_back(Trait::latin1ToString(" align=\"center\""));
            break;

        case Table<Trait>::AlignRight:
            html.push_back(Trait::latin1ToString(" align=\"right\""));
            break;

        default:
            break;
        }

        return html;
    }

protected:
    //! HTML content.
    typename Trait::String m_html;
    //! Just collect footnote references?
    bool m_justCollectFootnoteRefs = false;
    //! Just process footnote references and don't increment count number.
    bool m_dontIncrementFootnoteCount = false;
    //! Is this HTML wrapped in artcile tag?
    bool m_isWrappedInArticle = true;

    //! Auxiliary struct to process footnotes.
    struct FootnoteRefStuff {
        //! ID of footnote.
        typename Trait::String m_id;
        //! Count of references with this ID.
        long long int m_count = 0;
        //! Number of footnote reference with this ID.
        long long int m_current = 0;
    };

    //! Vector of processed footnotes references.
    typename Trait::template Vector<FootnoteRefStuff> m_fns;
}; // class HtmlVisitor

} /* namespace details */

//! Convert Document to HTML.
template<class Trait, class HtmlVisitor = details::HtmlVisitor<Trait>>
typename Trait::String
toHtml(
       //! Markdown document.
       std::shared_ptr<Document<Trait>> doc,
       //! Wrap HTML into <body> tag?
       bool wrapInBodyTag = true,
       //! String that will be applied as URL for image for back link from footnote.
       const typename Trait::String &hrefForRefBackImage = {},
       //! Wrap HTML with <article> tag?
       bool wrapInArticle = true)
{
    typename Trait::String html;

    if (wrapInBodyTag) {
        html.push_back(Trait::latin1ToString("<!DOCTYPE html>\n<html><head></head><body>\n"));
    }

    if (wrapInArticle) {
        html.push_back(Trait::latin1ToString("<article class=\"markdown-body\">"));
    }

    HtmlVisitor visitor;

    html.push_back(visitor.toHtml(doc, hrefForRefBackImage, wrapInArticle));

    if (wrapInArticle) {
        html.push_back(Trait::latin1ToString("</article>\n"));
    }

    if (wrapInBodyTag) {
        html.push_back(Trait::latin1ToString("</body></html>\n"));
    }

    return html;
}

} /* namespace MD */

#endif // MD4QT_MD_HTML_HPP_INCLUDED
