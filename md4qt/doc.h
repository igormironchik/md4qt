/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef MD4QT_MD_DOC_H_INCLUDED
#define MD4QT_MD_DOC_H_INCLUDED

// md4qt include.
#include "utils.h"

// C++ include.
#include <memory>

namespace MD
{

//
// ItemType
//

//! Enumeration of item types.
enum class ItemType : int {
    //! Heading.
    Heading = 0,
    //! Text.
    Text,
    //! Paragraph.
    Paragraph,
    //! Line break.
    LineBreak,
    //! Blockquote.
    Blockquote,
    //! List item.
    ListItem,
    //! List.
    List,
    //! Link.
    Link,
    //! Image.
    Image,
    //! Code.
    Code,
    //! Table cell.
    TableCell,
    //! Table row.
    TableRow,
    //! Table.
    Table,
    //! Footnote ref.
    FootnoteRef,
    //! Footnote.
    Footnote,
    //! Document.
    Document,
    //! Page break.
    PageBreak,
    //! Anchor.
    Anchor,
    //! Horizontal line.
    HorizontalLine,
    //! Raw HTML.
    RawHtml,
    //! Math expression.
    Math,
    //! Start item for user-defined types.
    UserDefined = 255
}; // enum class ItemType

//
// WithPosition
//

//! Base for any thing with start and end position.
class WithPosition
{
public:
    WithPosition() = default;
    virtual ~WithPosition() = default;

    WithPosition(long long int startColumn,
                 long long int startLine,
                 long long int endColumn,
                 long long int endLine)
        : m_startColumn(startColumn)
        , m_startLine(startLine)
        , m_endColumn(endColumn)
        , m_endLine(endLine)
    {
    }

    //! Apply positions to this from other.
    void applyPositions(const WithPosition &other)
    {
        if (this != &other) {
            *this = other;
        }
    }

    //! \return Start column.
    long long int startColumn() const
    {
        return m_startColumn;
    }

    //! \return Start line.
    long long int startLine() const
    {
        return m_startLine;
    }

    //! \return End column.
    long long int endColumn() const
    {
        return m_endColumn;
    }

    //! \return End line.
    long long int endLine() const
    {
        return m_endLine;
    }

    //! Set start column.
    void setStartColumn(long long int c)
    {
        m_startColumn = c;
    }

    //! Set start line.
    void setStartLine(long long int l)
    {
        m_startLine = l;
    }

    //! Set end column.
    void setEndColumn(long long int c)
    {
        m_endColumn = c;
    }

    //! Set end line.
    void setEndLine(long long int l)
    {
        m_endLine = l;
    }

private:
    //! Start column
    long long int m_startColumn = -1;
    //! Start line.
    long long int m_startLine = -1;
    //! End column.
    long long int m_endColumn = -1;
    //! End line.
    long long int m_endLine = -1;
}; // class WithPosition

inline bool operator==(const WithPosition &l, const WithPosition &r)
{
    return (l.startColumn() == r.startColumn() &&
            l.startLine() == r.startLine() &&
            l.endColumn() == r.endColumn() &&
            l.endLine() == r.endLine());
}

template<class Trait>
class Document;

//
// Item
//

//! Base class for item in Markdown document.
template<class Trait>
class Item : public WithPosition
{
protected:
    Item() = default;

public:
    ~Item() override = default;

    //! \return Type of the item.
    virtual ItemType type() const = 0;

    //! Clone this item.
    virtual std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const = 0;

private:
    MD_DISABLE_COPY(Item)
}; // class Item

//
// TextOption
//

//! Text option.
enum TextOption {
    //! No format.
    TextWithoutFormat = 0,
    //! Bold text.
    BoldText = 1,
    //! Italic text.
    ItalicText = 2,
    //! Strikethrough.
    StrikethroughText = 4
}; // enum TextOption

//
// StyleDelim
//

//! Emphasis in the Markdown document.
class StyleDelim final : public WithPosition
{
public:
    StyleDelim(int s,
               long long int startColumn,
               long long int startLine,
               long long int endColumn,
               long long int endLine)
        : WithPosition(startColumn, startLine, endColumn, endLine)
        , m_style(s)
    {
    }

    ~StyleDelim() override = default;

    //! \return Style.
    int style() const
    {
        return m_style;
    }

    //! Set style.
    void setStyle(int t)
    {
        m_style = t;
    }

private:
    int m_style = TextWithoutFormat;
}; // class StyleDelim

inline bool operator==(const StyleDelim &l, const StyleDelim &r)
{
    return (static_cast<WithPosition>(l) == static_cast<WithPosition>(r) && l.style() == r.style());
}

//
// ItemWithOpts
//

//! Base class for items that can have style options.
//! These are all items in Paragraph.
template<class Trait>
class ItemWithOpts : public Item<Trait>
{
protected:
    ItemWithOpts() = default;

public:
    ~ItemWithOpts() override = default;

    //! Apply other item with options to this.
    void applyItemWithOpts(const ItemWithOpts<Trait> &other)
    {
        if (this != &other) {
            WithPosition::applyPositions(other);
            m_opts = other.m_opts;
            m_openStyles = other.m_openStyles;
            m_closeStyles = other.m_closeStyles;
        }
    }

    //! Type of list of emphasis.
    using Styles = typename Trait::template Vector<StyleDelim>;

    //! \return Style options.
    int opts() const
    {
        return m_opts;
    }

    //! Set style options.
    void setOpts(int o)
    {
        m_opts = o;
    }

    //! \return List of all opening emphasises.
    const Styles &openStyles() const
    {
        return m_openStyles;
    }

    //! \return List of all opening emphasises.
    Styles &openStyles()
    {
        return m_openStyles;
    }

    //! \return List of all closing emphasises.
    const Styles &closeStyles() const
    {
        return m_closeStyles;
    }

    //! \return List of all closing emphasises.
    Styles &closeStyles()
    {
        return m_closeStyles;
    }

private:
    //! Style options.
    int m_opts = 0;
    //! List of opening emphasises.
    Styles m_openStyles;
    //! List of closing emphasises.
    Styles m_closeStyles;

    MD_DISABLE_COPY(ItemWithOpts)
}; // class ItemWithOpts

//
// PageBreak
//

//! Page break.
template<class Trait>
class PageBreak final : public Item<Trait>
{
public:
    PageBreak() = default;
    ~PageBreak() override = default;

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::PageBreak;
    }

    //! Clone this page break.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        MD_UNUSED(doc)

        return std::make_shared<PageBreak<Trait>>();
    }

private:
    MD_DISABLE_COPY(PageBreak)
}; // class PageBreak

//
// HorizontalLine
//

//! Horizontal line.
template<class Trait>
class HorizontalLine final : public Item<Trait>
{
public:
    HorizontalLine() = default;
    ~HorizontalLine() override = default;

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::HorizontalLine;
    }

    //! Clone this horizontal line.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        MD_UNUSED(doc)

        auto h = std::make_shared<HorizontalLine<Trait>>();
        h->applyPositions(*this);

        return h;
    }

private:
    MD_DISABLE_COPY(HorizontalLine)
}; // class HorizontalLine

//
// Anchor
//

//! Just an anchor.
template<class Trait>
class Anchor final : public Item<Trait>
{
public:
    explicit Anchor(const typename Trait::String &l)
        : m_label(l)
    {
    }

    ~Anchor() override = default;

    //! Clone this anchor.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        MD_UNUSED(doc)

        return std::make_shared<Anchor<Trait>>(m_label);
    }

    //! \return item type.
    ItemType type() const override
    {
        return ItemType::Anchor;
    }

    //! \return Label of this anchor.
    const typename Trait::String &label() const
    {
        return m_label;
    }

private:
    MD_DISABLE_COPY(Anchor)

    //! Label
    typename Trait::String m_label;
}; // class Anchor

//
// RawHtml
//

//! Raw HTML.
template<class Trait>
class RawHtml final : public ItemWithOpts<Trait>
{
public:
    RawHtml() = default;
    ~RawHtml() override = default;

    //! Clone this raw HTML.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        MD_UNUSED(doc)

        auto h = std::make_shared<RawHtml<Trait>>();
        h->applyItemWithOpts(*this);
        h->setText(m_text);
        h->setFreeTag(m_isFreeTag);

        return h;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::RawHtml;
    }

    //! \return HTML content.
    const typename Trait::String &text() const
    {
        return m_text;
    }

    //! Set HTML content.
    void setText(const typename Trait::String &t)
    {
        m_text = t;
    }

protected:
    template<class T>
    friend class Parser;

    template<class T>
    friend struct UnprotectedDocsMethods;

    //! \return Is this HTML a free tag, not inline one.
    //! \note This method is for internal use only.
    bool isFreeTag() const
    {
        return m_isFreeTag;
    }

    //! Set that this HTML is a free, not inline one.
    //! \note This method is for internal use only.
    void setFreeTag(bool on = true)
    {
        m_isFreeTag = on;
    }

private:
    //! HTML content.
    typename Trait::String m_text;
    //! Is this HTML a free tag, not inline one.
    bool m_isFreeTag = true;

    MD_DISABLE_COPY(RawHtml)
}; // class RawHtml

//
// Text
//

//! Text item in Paragraph.
template<typename Trait>
class Text : public ItemWithOpts<Trait>
{
public:
    Text() = default;
    ~Text() override = default;

    //! Apply other text to this.
    void applyText(const Text<Trait> &t)
    {
        if (this != &t) {
            ItemWithOpts<Trait>::applyItemWithOpts(t);
            setText(t.text());
        }
    }

    //! Clone this text item.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        MD_UNUSED(doc)

        auto t = std::make_shared<Text<Trait>>();
        t->applyText(*this);

        return t;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::Text;
    }

    //! \return Text content.
    const typename Trait::String &text() const
    {
        return m_text;
    }

    //! Set text content.
    void setText(const typename Trait::String &t)
    {
        m_text = t;
    }

private:
    //! Text content.
    typename Trait::String m_text;

    MD_DISABLE_COPY(Text)
}; // class Text

//
// LineBreak
//

//! Line break.
template<class Trait>
class LineBreak final : public Text<Trait>
{
public:
    LineBreak() = default;
    ~LineBreak() override = default;

    //! Clone this line break.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        MD_UNUSED(doc)

        auto b = std::make_shared<LineBreak<Trait>>();
        b->applyText(*this);

        return b;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::LineBreak;
    }

private:
    MD_DISABLE_COPY(LineBreak)
}; // class LineBreak

//
// Block
//

//! Abstract block (storage of child items).
template<class Trait>
class Block : public Item<Trait>
{
protected:
    Block() = default;

public:
    ~Block() override = default;

    //! Type of pointer to child item.
    using ItemSharedPointer = std::shared_ptr<Item<Trait>>;
    //! Type of list of children.
    using Items = typename Trait::template Vector<ItemSharedPointer>;

    //! Apply other block to this.
    void applyBlock(const Block<Trait> &other, Document<Trait> *doc = nullptr)
    {
        if (this != &other) {
            WithPosition::applyPositions(other);

            m_items.clear();

            for (const auto &i : other.items())
                appendItem(i->clone(doc));
        }
    }

    //! \return List of child items.
    const Items &items() const
    {
        return m_items;
    }

    //! Insert child item at give position.
    void insertItem(long long int idx, ItemSharedPointer i)
    {
        m_items.insert(m_items.cbegin() + idx, i);
    }

    //! Append child item.
    void appendItem(ItemSharedPointer i)
    {
        m_items.push_back(i);
    }

    //! Remove child item at the given position.
    void removeItemAt(long long int idx)
    {
        if (idx >= 0 && idx < static_cast<long long int>(m_items.size()))
            m_items.erase(m_items.cbegin() + idx);
    }

    //! \return Child item at the given position.
    ItemSharedPointer getItemAt(long long int idx) const
    {
        return m_items.at(idx);
    }

    //! \return Is there no children.
    bool isEmpty() const
    {
        return m_items.empty();
    }

private:
    //! Child items.
    Items m_items;

    MD_DISABLE_COPY(Block)
}; // class Block

//
// Paragraph
//

//! Paragraph.
template<class Trait>
class Paragraph final : public Block<Trait>
{
public:
    Paragraph() = default;
    ~Paragraph() override = default;

    //! Clone this paragraph.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        auto p = std::make_shared<Paragraph<Trait>>();
        p->applyBlock(*this, doc);

        return p;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::Paragraph;
    }

private:
    MD_DISABLE_COPY(Paragraph)
}; // class Paragraph

//
// Heading
//

//! Heading.
template<class Trait>
class Heading final : public Item<Trait>
{
public:
    Heading()
        : m_text(new Paragraph<Trait>)
    {
    }

    ~Heading() override = default;

    //! Type of list of service chanracters.
    using Delims = typename Trait::template Vector<WithPosition>;

    //! Clone this heading.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        auto h = std::make_shared<Heading<Trait>>();
        h->applyPositions(*this);
        h->setText(std::static_pointer_cast<Paragraph<Trait>>(m_text->clone(doc)));
        h->setLevel(m_level);
        h->setLabel(m_label);
        h->setDelims(m_delims);
        h->setLabelPos(m_labelPos);

        if (doc && isLabeled())
            doc->insertLabeledHeading(m_label, h);

        return h;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::Heading;
    }

    //! Type of smart pointer to paragraph.
    using ParagraphSharedPointer = std::shared_ptr<Paragraph<Trait>>;

    //! \return Content of the heading.
    ParagraphSharedPointer text() const
    {
        return m_text;
    }

    //! Set content of the heading.
    void setText(ParagraphSharedPointer t)
    {
        m_text = t;
    }

    //! \return Level of the heading.
    int level() const
    {
        return m_level;
    }

    //! Set level of the heading.
    void setLevel(int l)
    {
        m_level = l;
    }

    //! \return Is this heading has label?
    bool isLabeled() const
    {
        return m_label.size() > 0;
    }

    //! \return Label of the heading.
    const typename Trait::String &label() const
    {
        return m_label;
    }

    //! Set label of the heading.
    void setLabel(const typename Trait::String &l)
    {
        m_label = l;
    }

    //! \return List of service characters.
    const Delims &delims() const
    {
        return m_delims;
    }

    //! Set list of service characters.
    void setDelims(const Delims &d)
    {
        m_delims = d;
    }

    //! \return Position of a label in the heading.
    const WithPosition &labelPos() const
    {
        return m_labelPos;
    }

    //! Set position of a label in the heading.
    void setLabelPos(const WithPosition &p)
    {
        m_labelPos = p;
    }

private:
    //! Content of the heading.
    ParagraphSharedPointer m_text;
    //! Level of the heading.
    int m_level = 0;
    //! Label of the heading.
    typename Trait::String m_label;
    //! List of service characters.
    Delims m_delims;
    //! Position of the label.
    WithPosition m_labelPos;

    MD_DISABLE_COPY(Heading)
}; // class Heading

//
// Blockquote
//

//! Blockquote.
template<class Trait>
class Blockquote final : public Block<Trait>
{
public:
    Blockquote() = default;
    ~Blockquote() override = default;

    //! Clone this blockquote.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        auto b = std::make_shared<Blockquote<Trait>>();
        b->applyBlock(*this, doc);
        b->delims() = m_delims;

        return b;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::Blockquote;
    }

    //! Type of a list of service characters.
    using Delims = typename Trait::template Vector<WithPosition>;

    //! \return List of service characters.
    const Delims &delims() const
    {
        return m_delims;
    }

    //! \return List of service characters.
    Delims &delims()
    {
        return m_delims;
    }

private:
    //! List of service characters.
    Delims m_delims;

    MD_DISABLE_COPY(Blockquote)
}; // class Blockquote

//
// ListItem
//

//! List item in a list.
template<class Trait>
class ListItem final : public Block<Trait>
{
public:
    ListItem() = default;
    ~ListItem() override = default;

    //! Clone this list item.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        auto l = std::make_shared<ListItem<Trait>>();
        l->applyBlock(*this, doc);
        l->setListType(m_listType);
        l->setOrderedListPreState(m_orderedListState);
        l->setStartNumber(m_startNumber);
        l->setTaskList(m_isTaskList);
        l->setChecked(m_isChecked);
        l->setDelim(m_delim);
        l->setTaskDelim(m_taskDelim);

        return l;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::ListItem;
    }

    //! Type of the list.
    enum ListType {
        //! Ordered.
        Ordered,
        //! Unordered
        Unordered
    }; // enum ListType

    //! Preliminary state of the ordered list.
    enum OrderedListPreState {
        //! Start item.
        Start,
        //! Continue of the list
        Continue
    }; // enum OrderedListPreState

    //! \return Type of the list.
    ListType listType() const
    {
        return m_listType;
    }

    //! Set type of the list.
    void setListType(ListType t)
    {
        m_listType = t;
    }

    //! \return Preliminary state of the ordered list.
    OrderedListPreState orderedListPreState() const
    {
        return m_orderedListState;
    }

    //! Set preliminary state of the ordered list.
    void setOrderedListPreState(OrderedListPreState s)
    {
        m_orderedListState = s;
    }

    //! \return Start number of the ordered list
    int startNumber() const
    {
        return m_startNumber;
    }

    //! Set start number of the ordered list.
    void setStartNumber(int n)
    {
        m_startNumber = n;
    }

    //! \return Is this list item a task list item?
    bool isTaskList() const
    {
        return m_isTaskList;
    }

    //! Set this list item to be a tsk list item.
    void setTaskList(bool on = true)
    {
        m_isTaskList = on;
    }

    //! \return Is this task list item checked?
    bool isChecked() const
    {
        return m_isChecked;
    }

    //! Set this task list item to be checked.
    void setChecked(bool on = true)
    {
        m_isChecked = on;
    }

    //! \return Service character position.
    const WithPosition &delim() const
    {
        return m_delim;
    }

    //! Set service character position.
    void setDelim(const WithPosition &d)
    {
        m_delim = d;
    }

    //! \return Position of the task list "checkbox" in Markdown.
    const WithPosition &taskDelim() const
    {
        return m_taskDelim;
    }

    //! Set position of the task list "checkbox" in Markdown.
    void setTaskDelim(const WithPosition &d)
    {
        m_taskDelim = d;
    }

private:
    //! Type of the list.
    ListType m_listType = Unordered;
    //! Preliminary state of the ordered list.
    OrderedListPreState m_orderedListState = Start;
    //! Start number of the ordered list.
    int m_startNumber = 1;
    //! Is this list item a task list item?
    bool m_isTaskList = false;
    //! Is this task list item checked?
    bool m_isChecked = false;
    //! Service character position.
    WithPosition m_delim = {};
    //! Task list "checkbox" position.
    WithPosition m_taskDelim = {};

    MD_DISABLE_COPY(ListItem)
}; // class ListItem

//
// List
//

//! List.
template<class Trait>
class List final : public Block<Trait>
{
public:
    List() = default;
    ~List() override = default;

    //! Clone this list.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        auto l = std::make_shared<List<Trait>>();
        l->applyBlock(*this, doc);

        return l;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::List;
    }

private:
    MD_DISABLE_COPY(List)
}; // class List

//
// LinkBase
//

//! Base class for links.
template<class Trait>
class LinkBase : public ItemWithOpts<Trait>
{
public:
    LinkBase()
        : m_p(new Paragraph<Trait>)
    {
    }

    ~LinkBase() override = default;

    //! Apply other base of link to this.
    void applyLinkBase(const LinkBase<Trait> &other, Document<Trait> *doc = nullptr)
    {
        if (this != &other) {
            ItemWithOpts<Trait>::applyItemWithOpts(other);
            setUrl(other.url());
            setText(other.text());
            setP(std::static_pointer_cast<Paragraph<Trait>>(other.p()->clone(doc)));
            setTextPos(other.textPos());
            setUrlPos(other.urlPos());
        }
    }

    //! Type of a smart pointer to link's description.
    using ParagraphSharedPointer = std::shared_ptr<Paragraph<Trait>>;

    //! \return URL of the link.
    const typename Trait::String &url() const
    {
        return m_url;
    }

    //! Set URL of the link.
    void setUrl(const typename Trait::String &u)
    {
        m_url = u;
    }

    //! Not parsed text of link's description.
    const typename Trait::String &text() const
    {
        return m_text;
    }

    //! Set not parsed text of link's description.
    void setText(const typename Trait::String &t)
    {
        m_text = t;
    }

    //! \return Is this link empty?
    bool isEmpty() const
    {
        return m_url.size() <= 0;
    }

    //! \return Pointer to parsed text of link's description.
    ParagraphSharedPointer p() const
    {
        return m_p;
    }

    //! Set pointer to parsed text of link's description.
    void setP(ParagraphSharedPointer v)
    {
        m_p = v;
    }

    //! \return Position of link's desciption.
    const WithPosition &textPos() const
    {
        return m_textPos;
    }

    //! Set position of link's description.
    void setTextPos(const WithPosition &pos)
    {
        m_textPos = pos;
    }

    //! \return Position of URL.
    const WithPosition &urlPos() const
    {
        return m_urlPos;
    }

    //! Set position of URL.
    void setUrlPos(const WithPosition &pos)
    {
        m_urlPos = pos;
    }

private:
    //! URL.
    typename Trait::String m_url;
    //! Not parsed content of link's description.
    typename Trait::String m_text;
    //! Parsed content of link's description.
    ParagraphSharedPointer m_p;
    //! Position of link's description.
    WithPosition m_textPos = {};
    //! URL position.
    WithPosition m_urlPos = {};

    MD_DISABLE_COPY(LinkBase)
}; // class LinkBase

//
// Image
//

//! Image.
template<class Trait>
class Image final : public LinkBase<Trait>
{
public:
    Image() = default;
    ~Image() override = default;

    //! Clone this image.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        auto i = std::make_shared<Image<Trait>>();
        i->applyLinkBase(*this, doc);

        return i;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::Image;
    }

private:
    MD_DISABLE_COPY(Image)
}; // class Image

//
// Link
//

//! Link.
template<class Trait>
class Link final : public LinkBase<Trait>
{
public:
    Link()
        : LinkBase<Trait>()
        , m_img(new Image<Trait>)
    {
    }

    //! Clone this link.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        auto l = std::make_shared<Link<Trait>>();
        l->applyLinkBase(*this, doc);
        l->setImg(std::static_pointer_cast<Image<Trait>>(m_img->clone(doc)));

        return l;
    }

    ~Link() override = default;

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::Link;
    }

    //! Type of a smart pointer to image.
    using ImageSharedPointer = std::shared_ptr<Image<Trait>>;

    //! \return Image of the link.
    ImageSharedPointer img() const
    {
        return m_img;
    }

    //! Set image of the link.
    void setImg(ImageSharedPointer i)
    {
        m_img = i;
    }

private:
    //! Image of the link.
    ImageSharedPointer m_img;

    MD_DISABLE_COPY(Link)
}; // class Link

//
// Code
//

//! Code.
template<class Trait>
class Code : public ItemWithOpts<Trait>
{
public:
    explicit Code(const typename Trait::String &t, bool fensedCode, bool inl)
        : ItemWithOpts<Trait>()
        , m_text(t)
        , m_inlined(inl)
        , m_fensed(fensedCode)
    {
    }

    ~Code() override = default;

    //! Apply other code to this.
    void applyCode(const Code<Trait> &other)
    {
        if (this != &other) {
            ItemWithOpts<Trait>::applyItemWithOpts(other);
            setText(other.text());
            setInline(other.isInline());
            setSyntax(other.syntax());
            setSyntaxPos(other.syntaxPos());
            setStartDelim(other.startDelim());
            setEndDelim(other.endDelim());
            setFensedCode(other.isFensedCode());
        }
    }

    //! Clone this code.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        MD_UNUSED(doc)

        auto c = std::make_shared<Code<Trait>>(m_text, m_fensed, m_inlined);
        c->applyCode(*this);

        return c;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::Code;
    }

    //! \return Content of the code.
    const typename Trait::String &text() const
    {
        return m_text;
    }

    //! Set content of the code.
    void setText(const typename Trait::String &t)
    {
        m_text = t;
    }

    //! \return Is this code inline?
    bool isInline() const
    {
        return m_inlined;
    }

    //! Set this code to be inline.
    void setInline(bool on = true)
    {
        m_inlined = on;
    }

    //! \return Syntax of the fensed code block.
    const typename Trait::String &syntax() const
    {
        return m_syntax;
    }

    //! Set syntax of the fensed code block.
    void setSyntax(const typename Trait::String &s)
    {
        m_syntax = s;
    }

    //! \return Position of the syntax of the fensed code block.
    const WithPosition &syntaxPos() const
    {
        return m_syntaxPos;
    }

    //! Set position of the syntax of the fensed code block.
    void setSyntaxPos(const WithPosition &p)
    {
        m_syntaxPos = p;
    }

    //! \return Position of the start service characters.
    const WithPosition &startDelim() const
    {
        return m_startDelim;
    }

    //! Set position of the start service characters.
    void setStartDelim(const WithPosition &d)
    {
        m_startDelim = d;
    }

    //! \return Position of the ending service characters.
    const WithPosition &endDelim() const
    {
        return m_endDelim;
    }

    //! Set position of the ending service characters.
    void setEndDelim(const WithPosition &d)
    {
        m_endDelim = d;
    }

    //! \return Is this a fensed code block?
    bool isFensedCode() const
    {
        return m_fensed;
    }

    //! Set this code block to be a fensed code block.
    void setFensedCode(bool on = true)
    {
        m_fensed = on;
    }

private:
    //! Content of the code.
    typename Trait::String m_text;
    //! Is this code inline?
    bool m_inlined = true;
    //! Is this code a fensed code block.
    bool m_fensed = false;
    //! Syntax of the fensed code lock.
    typename Trait::String m_syntax;
    //! Position of start service characters.
    WithPosition m_startDelim = {};
    //! Position of ending service characters.
    WithPosition m_endDelim = {};
    //! Position of syntax of fensed code block.
    WithPosition m_syntaxPos = {};

    MD_DISABLE_COPY(Code)
}; // class Code

//
// Math
//

//! LaTeX math expression.
template<class Trait>
class Math final : public Code<Trait>
{
public:
    Math()
        : Code<Trait>(typename Trait::String(), false, true)
    {
    }

    ~Math() override = default;

    //! Clone this LaTeX math expression.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        MD_UNUSED(doc)

        auto m = std::make_shared<Math<Trait>>();
        m->applyCode(*this);

        return m;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::Math;
    }

    //! \return Content.
    const typename Trait::String &expr() const
    {
        return Code<Trait>::text();
    }

    //! Set content.
    void setExpr(const typename Trait::String &e)
    {
        Code<Trait>::setText(e);
    }

private:
    MD_DISABLE_COPY(Math)
}; // class Math

//
// TableCell
//

//! Table cell.
template<class Trait>
class TableCell final : public Block<Trait>
{
public:
    TableCell() = default;
    ~TableCell() override = default;

    //! Clone this table cell.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        auto c = std::make_shared<TableCell<Trait>>();
        c->applyBlock(*this, doc);

        return c;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::TableCell;
    }

private:
    MD_DISABLE_COPY(TableCell)
}; // class TableCell

//
// TableRow
//

//! Table row.
template<class Trait>
class TableRow final : public Item<Trait>
{
public:
    TableRow() = default;
    ~TableRow() override = default;

    //! Clone this table row.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        auto t = std::make_shared<TableRow<Trait>>();
        t->applyPositions(*this);

        for (const auto &c : cells()) {
            t->appendCell(std::static_pointer_cast<TableCell<Trait>>(c->clone(doc)));
        }

        return t;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::TableRow;
    }

    //! Type of a smart pointer to table cell.
    using TableCellSharedPointer = std::shared_ptr<TableCell<Trait>>;
    //! Type of a list of table cells.
    using Cells = typename Trait::template Vector<TableCellSharedPointer>;

    //! \return List of cells.
    const Cells &cells() const
    {
        return m_cells;
    }

    //! Append cell.
    void appendCell(TableCellSharedPointer c)
    {
        m_cells.push_back(c);
    }

    //! \return Is this row empty?
    bool isEmpty() const
    {
        return m_cells.empty();
    }

private:
    //! List of cells.
    Cells m_cells;

    MD_DISABLE_COPY(TableRow)
}; // class TableRow

//
// Table
//

//! Table.
template<class Trait>
class Table final : public Item<Trait>
{
public:
    Table() = default;
    ~Table() override = default;

    //! Clone this table.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        auto t = std::make_shared<Table<Trait>>();
        t->applyPositions(*this);

        for (const auto &r : rows()) {
            t->appendRow(std::static_pointer_cast<TableRow<Trait>>(r->clone(doc)));
        }

        for (int i = 0; i < columnsCount(); ++i) {
            t->setColumnAlignment(i, columnAlignment(i));
        }

        return t;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::Table;
    }

    //! Type of a smart pointer to table row.
    using TableRowSharedPointer = std::shared_ptr<TableRow<Trait>>;
    //! Type of list of rows.
    using Rows = typename Trait::template Vector<TableRowSharedPointer>;

    //! \return List of rows.
    const Rows &rows() const
    {
        return m_rows;
    }

    //! Append row.
    void appendRow(TableRowSharedPointer r)
    {
        m_rows.push_back(r);
    }

    //! Alignment.
    enum Alignment {
        //! Left.
        AlignLeft,
        //! Right.
        AlignRight,
        //! Center.
        AlignCenter
    }; // enum Alignmnet.

    //! Type of list alignments.
    using ColumnsAlignments = typename Trait::template Vector<Alignment>;

    //! \return Alignment of the given column.
    Alignment columnAlignment(int idx) const
    {
        return m_aligns.at(idx);
    }

    //! Set alignment of the given column.
    void setColumnAlignment(int idx, Alignment a)
    {
        if (idx + 1 > columnsCount()) {
            m_aligns.push_back(a);
        } else {
            m_aligns[idx] = a;
        }
    }

    //! \return Count of columns.
    int columnsCount() const
    {
        return m_aligns.size();
    }

    //! \return Is this table empty?
    bool isEmpty() const
    {
        return (m_aligns.empty() || m_rows.empty());
    }

private:
    //! Rows.
    Rows m_rows;
    //! Columns' alignments.
    ColumnsAlignments m_aligns;

    MD_DISABLE_COPY(Table)
}; // class Table

//
// FootnoteRef
//

//! Footnote reference.
template<class Trait>
class FootnoteRef final : public Text<Trait>
{
public:
    explicit FootnoteRef(const typename Trait::String &i)
        : m_id(i)
    {
    }

    ~FootnoteRef() override = default;

    //! Clone this footnote reference.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        MD_UNUSED(doc)

        auto f = std::make_shared<FootnoteRef<Trait>>(m_id);
        f->applyText(*this);
        f->setIdPos(m_idPos);

        return f;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::FootnoteRef;
    }

    //! \return ID of footnote reference.
    const typename Trait::String &id() const
    {
        return m_id;
    }

    //! \return Position of ID.
    const WithPosition &idPos() const
    {
        return m_idPos;
    }

    //! Set position of ID.
    void setIdPos(const WithPosition &pos)
    {
        m_idPos = pos;
    }

private:
    //! ID.
    typename Trait::String m_id;
    //! Position of ID.
    WithPosition m_idPos;

    MD_DISABLE_COPY(FootnoteRef)
}; // class FootnoteRef

//
// Footnote
//

//! Footnote.
template<class Trait>
class Footnote final : public Block<Trait>
{
public:
    Footnote() = default;
    ~Footnote() override = default;

    //! Clone this footnote.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        auto f = std::make_shared<Footnote<Trait>>();
        f->applyBlock(*this, doc);
        f->setIdPos(m_idPos);

        return f;
    }

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::Footnote;
    }

    //! \return Position of ID.
    const WithPosition &idPos() const
    {
        return m_idPos;
    }

    //! Set position of ID.
    void setIdPos(const WithPosition &pos)
    {
        m_idPos = pos;
    }

private:
    //! Position of ID.
    WithPosition m_idPos = {};

    MD_DISABLE_COPY(Footnote)
}; // class Footnote

//
// Document
//

//! Document.
template<class Trait>
class Document final : public Block<Trait>
{
public:
    Document() = default;
    ~Document() override = default;

    //! \return Type of the item.
    ItemType type() const override
    {
        return ItemType::Document;
    }

    //! Clone this document.
    std::shared_ptr<Item<Trait>> clone(Document<Trait> *doc = nullptr) const override
    {
        MD_UNUSED(doc)

        auto d = std::make_shared<Document<Trait>>();
        d->applyBlock(*this, d.get());

        for (auto it = m_footnotes.cbegin(), last = m_footnotes.cend(); it != last; ++it) {
            d->insertFootnote(it->first,
                std::static_pointer_cast<Footnote<Trait>>(it->second->clone(d.get())));
        }

        for (auto it = m_labeledLinks.cbegin(), last = m_labeledLinks.cend(); it != last; ++it) {
            d->insertLabeledLink(it->first,
                std::static_pointer_cast<Link<Trait>>(it->second->clone(d.get())));
        }

        return d;
    }

    //! Type of a smart pointer to footnote.
    using FootnoteSharedPointer = std::shared_ptr<Footnote<Trait>>;
    //! Type of a map of footnotes.
    using Footnotes = typename Trait::template Map<typename Trait::String, FootnoteSharedPointer>;

    //! \return Map of footnotes.
    const Footnotes &footnotesMap() const
    {
        return m_footnotes;
    }

    //! Insert footnote with the give ID.
    void insertFootnote(const typename Trait::String &id, FootnoteSharedPointer fn)
    {
        m_footnotes.insert({id, fn});
    }

    //! Type of a smart pointer to link.
    using LinkSharedPointer = std::shared_ptr<Link<Trait>>;
    //! Type of a map of shortcut links.
    using LabeledLinks = typename Trait::template Map<typename Trait::String, LinkSharedPointer>;

    //! \return Map of shortcut links.
    const LabeledLinks &labeledLinks() const
    {
        return m_labeledLinks;
    }

    //! Insert shortcut link with the given label.
    void insertLabeledLink(const typename Trait::String &label, LinkSharedPointer lnk)
    {
        m_labeledLinks.insert({label, lnk});
    }

    //! Type of a smart pointer to heading.
    using HeadingSharedPointer = std::shared_ptr<Heading<Trait>>;
    //! Type of a map of headings.
    using LabeledHeadings = typename Trait::template Map<typename Trait::String, HeadingSharedPointer>;

    //! \return Map of headings.
    const LabeledHeadings &labeledHeadings() const
    {
        return m_labeledHeadings;
    }

    //! Insert heading with the given label.
    void insertLabeledHeading(const typename Trait::String &label, HeadingSharedPointer h)
    {
        m_labeledHeadings.insert({label, h});
    }

private:
    //! Map of footnotes.
    Footnotes m_footnotes;
    //! Map of shortcut links.
    LabeledLinks m_labeledLinks;
    //! Map of headings.
    LabeledHeadings m_labeledHeadings;

    MD_DISABLE_COPY(Document)
}; // class Document;

} /* namespace MD */

#endif // MD4QT_MD_DOC_H_INCLUDED
