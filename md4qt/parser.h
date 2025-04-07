/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef MD4QT_MD_PARSER_HPP_INCLUDED
#define MD4QT_MD_PARSER_HPP_INCLUDED

// md4qt include.
#include "doc.h"
#include "entities_map.h"
#include "traits.h"
#include "utils.h"

#ifdef MD4QT_QT_SUPPORT

// Qt include.
#include <QDir>
#include <QFile>
#include <QTextStream>

#endif // MD4QT_QT_SUPPORT

#ifdef MD4QT_ICU_STL_SUPPORT

// C++ include.
#include <exception>

#endif // MD4QT_ICU_STL_SUPPORT

// C++ include.
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <functional>
#include <memory>
#include <set>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace MD
{

//! Starting HTML comment string.
static const char *s_startComment = "<!--";

//! \return Is \p indent indent belongs to list with previous \p indents indents.
inline bool
indentInList(const std::vector<long long int> *indents,
             long long int indent,
             bool codeIndentedBySpaces)
{
    if (indents && !indents->empty()) {
        return (std::find_if(indents->cbegin(),
                             indents->cend(),
                             [indent, codeIndentedBySpaces](const auto &v) {
                                 return (indent >= v && (codeIndentedBySpaces ?
                                         true : indent <= v + 3));
                             })
                != indents->cend());
    } else {
        return false;
    }
}

//! Skip spaces in line from position \p i.
template<class Trait>
inline long long int
skipSpaces(long long int i, const typename Trait::String &line)
{
    const auto length = line.length();

    while (i < length && line[i].isSpace()) {
        ++i;
    }

    return i;
}

//! \return Last non-space character position.
template<class String>
inline long long int
lastNonSpacePos(const String &line)
{
    long long int i = line.length() - 1;

    while (i >= 0 && line[i].isSpace()) {
        --i;
    }

    return i;
}

//! Remove spaces at the end of string \p s.
template<class String>
inline void
removeSpacesAtEnd(String &s)
{
    const auto i = lastNonSpacePos(s);

    if (i != s.length() - 1) {
        s.remove(i + 1, s.length() - i - 1);
    }
}

//! \return Starting sequence of the same characters.
template<class Trait>
inline typename Trait::String
startSequence(const typename Trait::String &line)
{
    auto pos = skipSpaces<Trait>(0, line);

    if (pos >= line.length()) {
        return {};
    }

    const auto sch = line[pos];
    const auto start = pos;

    ++pos;

    while (pos < line.length() && line[pos] == sch) {
        ++pos;
    }

    return line.sliced(start, pos - start);
}

//! \return Is string an ordered list.
template<class Trait>
inline bool
isOrderedList(const typename Trait::String &s,
              int *num = nullptr,
              int *len = nullptr,
              typename Trait::Char *delim = nullptr,
              bool *isFirstLineEmpty = nullptr)
{
    long long int p = skipSpaces<Trait>(0, s);

    long long int dp = p;

    for (; p < s.size(); ++p) {
        if (!s[p].isDigit()) {
            break;
        }
    }

    if (dp != p && p < s.size()) {
        const auto digits = s.sliced(dp, p - dp);

        if (digits.size() > 9) {
            return false;
        }

        const auto i = digits.toInt();

        if (num) {
            *num = i;
        }

        if (len) {
            *len = p - dp;
        }

        if (s[p] == Trait::latin1ToChar('.') || s[p] == Trait::latin1ToChar(')')) {
            if (delim) {
                *delim = s[p];
            }

            ++p;

            long long int tmp = skipSpaces<Trait>(p, s);

            if (isFirstLineEmpty) {
                *isFirstLineEmpty = (tmp == s.size());
            }

            if ((p < s.size() && s[p] == Trait::latin1ToChar(' ')) || p == s.size()) {
                return true;
            }
        }
    }

    return false;
}

//
// RawHtmlBlock
//

//! Internal structure for pre-storing HTML.
template<class Trait>
struct RawHtmlBlock {
    std::shared_ptr<RawHtml<Trait>> m_html = {};
    std::shared_ptr<Block<Trait>> m_parent = {};
    std::shared_ptr<Block<Trait>> m_topParent = {};
    using SequenceOfBlock = std::vector<std::pair<std::shared_ptr<Block<Trait>>, long long int>>;
    SequenceOfBlock m_blocks = {};
    std::unordered_map<std::shared_ptr<Block<Trait>>, SequenceOfBlock> m_toAdjustLastPos = {};
    int m_htmlBlockType = -1;
    bool m_continueHtml = false;
    bool m_onLine = false;

    std::shared_ptr<Block<Trait>>
    findParent(long long int indent) const
    {
        for (auto it = m_blocks.crbegin(), last = m_blocks.crend(); it != last; ++it) {
            if (indent >= it->second) {
                return it->first;
            }
        }

        return nullptr;
    }
}; // struct RawHtmlBlock

//
// MdLineData
//

//! Internal structure for auxiliary information about a line in Markdown.
struct MdLineData {
    long long int m_lineNumber = -1;
    using CommentData = std::pair<char, bool>;
    using CommentDataMap = std::map<long long int, CommentData>;
    // std::pair< closed, valid >
    CommentDataMap m_htmlCommentData = {};
    // May this line break a list?
    bool m_mayBreakList = false;
}; // struct MdLineData

//
// MdBlock
//

//! Internal structure for block of text in Markdown.
template<class Trait>
struct MdBlock {
    using Line = std::pair<typename Trait::InternalString, MdLineData>;
    using Data = typename Trait::template Vector<Line>;

    Data m_data;
    long long int m_emptyLinesBefore = 0;
    bool m_emptyLineAfter = true;
}; // struct MdBlock

template<class Trait>
inline long long int
emptyLinesBeforeCount(typename MdBlock<Trait>::Data::iterator begin,
                      typename MdBlock<Trait>::Data::iterator it)
{
    long long int count = 0;

    if (it != begin) {
        while (it != begin) {
            it = std::prev(it);

            if (it->first.asString().simplified().isEmpty()) {
                ++count;
            } else {
                break;
            }
        }
    }

    return count;
}

//
// StringListStream
//

//! Wrapper for typename Trait::StringList to be behaved like a stream.
template<class Trait>
class StringListStream final
{
public:
    StringListStream(typename MdBlock<Trait>::Data &stream)
        : m_stream(stream)
        , m_pos(0)
    {
    }

    bool atEnd() const
    {
        return (m_pos >= (long long int)m_stream.size());
    }

    std::pair<typename Trait::InternalString, bool> readLine()
    {
        const std::pair<typename Trait::InternalString, bool> ret =
            {m_stream.at(m_pos).first, m_stream.at(m_pos).second.m_mayBreakList};

        ++m_pos;

        return ret;
    }

    long long int currentLineNumber() const
    {
        return (m_pos < size() ? m_stream.at(m_pos).second.m_lineNumber :
                                 (size() > 0 ? m_stream.at(0).second.m_lineNumber + size() : -1));
    }

    long long int currentStreamPos() const
    {
        return m_pos;
    }

    typename Trait::InternalString lineAt(long long int pos)
    {
        return m_stream.at(pos).first;
    }

    long long int size() const
    {
        return m_stream.size();
    }

    void setLineNumber(long long int lineNumber)
    {
        m_pos = 0;

        m_pos += lineNumber - currentLineNumber();
    }

private:
    typename MdBlock<Trait>::Data &m_stream;
    long long int m_pos;
}; // class StringListStream

//! \return Is string a footnote?
template<class Trait>
inline bool
isFootnote(const typename Trait::String &s)
{
    long long int p = skipSpaces<Trait>(0, s);

    if (s.size() - p < 5) {
        return false;
    }

    if (s[p++] != Trait::latin1ToChar('[')) {
        return false;
    }

    if (s[p++] != Trait::latin1ToChar('^')) {
        return false;
    }

    if (s[p] == Trait::latin1ToChar(']') || s[p].isSpace()) {
        return false;
    }

    for (; p < s.size(); ++p) {
        if (s[p] == Trait::latin1ToChar(']')) {
            break;
        } else if (s[p].isSpace()) {
            return false;
        }
    }

    ++p;

    if (p < s.size() && s[p] == Trait::latin1ToChar(':')) {
        return true;
    } else {
        return false;
    }
}

//! \return Is string a code fences?
template<class Trait>
inline bool
isCodeFences(const typename Trait::String &s, bool closing = false)
{
    auto p = skipSpaces<Trait>(0, s);

    if (p > 3 || p == s.length()) {
        return false;
    }

    const auto ch = s[p];

    if (ch != Trait::latin1ToChar('~') && ch != Trait::latin1ToChar('`')) {
        return false;
    }

    bool space = false;

    long long int c = 1;
    ++p;

    for (; p < s.length(); ++p) {
        if (s[p].isSpace()) {
            space = true;
        } else if (s[p] == ch) {
            if (space && (closing ? true : ch == Trait::latin1ToChar('`'))) {
                return false;
            }

            if (!space) {
                ++c;
            }
        } else if (closing) {
            return false;
        } else {
            break;
        }
    }

    if (c < 3) {
        return false;
    }

    if (ch == Trait::latin1ToChar('`')) {
        for (; p < s.length(); ++p) {
            if (s[p] == Trait::latin1ToChar('`')) {
                return false;
            }
        }
    }

    return true;
}

//! Skip escaped sequence of characters till first space.
template<class Trait>
inline typename Trait::String
readEscapedSequence(long long int i,
                    const typename Trait::String &str,
                    long long int *endPos = nullptr)
{
    bool backslash = false;
    const auto start = i;

    if (start >= str.length()) {
        return {};
    }

    while (i < str.length()) {
        bool now = false;

        if (str[i] == Trait::latin1ToChar('\\') && !backslash) {
            backslash = true;
            now = true;
        } else if (str[i].isSpace() && !backslash) {
            break;
        }

        if (!now) {
            backslash = false;
        }

        ++i;
    }

    if (endPos) {
        *endPos = i - 1;
    }

    return str.sliced(start, i - start);
}

//! Characters that can be escaped.
template<class Trait>
static const typename Trait::String s_canBeEscaped =
    Trait::latin1ToString("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~");

//! Remove backslashes from the string.
template<class String, class Trait>
inline String
removeBackslashes(const String &s)
{
    String r = s;
    bool backslash = false;
    long long int extra = 0;

    for (long long int i = 0; i < s.length(); ++i) {
        bool now = false;

        if (s[i] == Trait::latin1ToChar('\\') && !backslash && i != s.length() - 1) {
            backslash = true;
            now = true;
        } else if (s_canBeEscaped<Trait>.contains(s[i]) && backslash) {
            r.remove(i - extra - 1, 1);
            ++extra;
        }

        if (!now) {
            backslash = false;
        }
    }

    return r;
}

//! \return Is string a start of code?
template<class Trait>
inline bool
isStartOfCode(const typename Trait::String &str,
              typename Trait::String *syntax = nullptr,
              WithPosition *delim = nullptr,
              WithPosition *syntaxPos = nullptr)
{
    long long int p = skipSpaces<Trait>(0, str);

    if (delim) {
        delim->setStartColumn(p);
    }

    if (p > 3) {
        return false;
    }

    if (str.size() - p < 3) {
        return false;
    }

    const bool c96 = str[p] == Trait::latin1ToChar('`');
    const bool c126 = str[p] == Trait::latin1ToChar('~');

    if (c96 || c126) {
        ++p;
        long long int c = 1;

        while (p < str.length()) {
            if (str[p] != (c96 ? Trait::latin1ToChar('`') : Trait::latin1ToChar('~'))) {
                break;
            }

            ++c;
            ++p;
        }

        if (delim) {
            delim->setEndColumn(p - 1);
        }

        if (c < 3) {
            return false;
        }

        if (syntax) {
            p = skipSpaces<Trait>(p, str);
            long long int endSyntaxPos = p;

            if (p < str.size()) {
                *syntax = removeBackslashes<typename Trait::String, Trait>(
					readEscapedSequence<Trait>(p, str, &endSyntaxPos));

                if (syntaxPos) {
                    syntaxPos->setStartColumn(p);
                    syntaxPos->setEndColumn(endSyntaxPos);
                }
            }
        }

        return true;
    }

    return false;
}

//! \return Is string a horizontal line?
template<class Trait>
inline bool
isHorizontalLine(const typename Trait::String &s)
{
    if (s.size() < 3) {
        return false;
    }

    typename Trait::Char c;

    if (s[0] == Trait::latin1ToChar('*')) {
        c = Trait::latin1ToChar('*');
    } else if (s[0] == Trait::latin1ToChar('-')) {
        c = Trait::latin1ToChar('-');
    } else if (s[0] == Trait::latin1ToChar('_')) {
        c = Trait::latin1ToChar('_');
    } else {
        return false;
    }

    long long int p = 1;
    long long int count = 1;

    for (; p < s.size(); ++p) {
        if (s[p] != c && !s[p].isSpace()) {
            break;
        } else if (s[p] == c) {
            ++count;
        }
    }

    if (count < 3) {
        return false;
    }

    if (p == s.size()) {
        return true;
    }

    return false;
}

//! \return Is string a column alignment?
template<class Trait>
inline bool
isColumnAlignment(const typename Trait::String &s)
{
    long long int p = skipSpaces<Trait>(0, s);

    static const typename Trait::String s_legitime = Trait::latin1ToString(":-");

    if (p >= s.length()) {
        return false;
    }

    if (!s_legitime.contains(s[p])) {
        return false;
    }

    if (s[p] == Trait::latin1ToChar(':')) {
        ++p;
    }

    for (; p < s.size(); ++p) {
        if (s[p] != Trait::latin1ToChar('-')) {
            break;
        }
    }

    if (p == s.size()) {
        return true;
    }

    if (s[p] != Trait::latin1ToChar(':') && !s[p].isSpace()) {
        return false;
    }

    ++p;

    for (; p < s.size(); ++p) {
        if (!s[p].isSpace()) {
            return false;
        }
    }

    return true;
}

//! Split string.
template<class Trait>
typename Trait::StringList
splitString(const typename Trait::String &str, const typename Trait::Char &ch);

#ifdef MD4QT_ICU_STL_SUPPORT

template<>
inline typename UnicodeStringTrait::StringList
splitString<UnicodeStringTrait>(const UnicodeString &str, const UnicodeChar &ch)
{
    return str.split(ch);
}

#endif

#ifdef MD4QT_QT_SUPPORT

template<>
inline typename QStringTrait::StringList
splitString<QStringTrait>(const QString &str, const QChar &ch)
{
    return str.split(ch, Qt::SkipEmptyParts);
}

#endif

//! \return Number of columns?
template<class Trait>
inline int
isTableAlignment(const typename Trait::String &s)
{
    const auto columns = splitString<Trait>(s.simplified(), Trait::latin1ToChar('|'));

    for (const auto &c : columns) {
        if (!isColumnAlignment<Trait>(c)) {
            return 0;
        }
    }

    return columns.size();
}

//! \return Is given string a HTML comment.
template<class Trait>
inline bool
isHtmlComment(const typename Trait::String &s)
{
    auto c = s;

    if (s.startsWith(Trait::latin1ToString(s_startComment))) {
        c.remove(0, 4);
    } else {
        return false;
    }

    long long int p = -1;
    bool endFound = false;

    while ((p = c.indexOf(Trait::latin1ToString("--"), p + 1)) > -1) {
        if (c.size() > p + 2 && c[p + 2] == Trait::latin1ToChar('>')) {
            if (!endFound) {
                endFound = true;
            } else {
                return false;
            }
        } else if (p - 2 >= 0 && c.sliced(p - 2, 4) == Trait::latin1ToString("<!--")) {
            return false;
        } else if (c.size() > p + 3 && c.sliced(p, 4) == Trait::latin1ToString("--!>")) {
            return false;
        }
    }

    return endFound;
}

//! Replace entities in the string with corresponding character.
template<class Trait>
inline typename Trait::String
replaceEntity(const typename Trait::String &s)
{
    long long int p1 = 0;

    typename Trait::String res;
    long long int i = 0;

    while ((p1 = s.indexOf(Trait::latin1ToChar('&'), p1)) != -1) {
        if (p1 > 0 && s[p1 - 1] == Trait::latin1ToChar('\\')) {
            ++p1;

            continue;
        }

        const auto p2 = s.indexOf(Trait::latin1ToChar(';'), p1);

        if (p2 != -1) {
            const auto en = s.sliced(p1, p2 - p1 + 1);

            if (en.size() > 2 && en[1] == Trait::latin1ToChar('#')) {
                if (en.size() > 3 && en[2].toLower() == Trait::latin1ToChar('x')) {
                    const auto hex = en.sliced(3, en.size() - 4);

                    if (hex.size() <= 6 && hex.size() > 0) {
                        bool ok = false;

                        const char32_t c = hex.toInt(&ok, 16);

                        if (ok) {
                            res.push_back(s.sliced(i, p1 - i));
                            i = p2 + 1;

                            if (c) {
                                Trait::appendUcs4(res, c);
                            } else {
                                res.push_back(typename Trait::Char(0xFFFD));
                            }
                        }
                    }
                } else {
                    const auto dec = en.sliced(2, en.size() - 3);

                    if (dec.size() <= 7 && dec.size() > 0) {
                        bool ok = false;

                        const char32_t c = dec.toInt(&ok, 10);

                        if (ok) {
                            res.push_back(s.sliced(i, p1 - i));
                            i = p2 + 1;

                            if (c) {
                                Trait::appendUcs4(res, c);
                            } else {
                                res.push_back(typename Trait::Char(0xFFFD));
                            }
                        }
                    }
                }
            } else {
                const auto it = s_entityMap<Trait>.find(en);

                if (it != s_entityMap<Trait>.cend()) {
                    res.push_back(s.sliced(i, p1 - i));
                    i = p2 + 1;
                    res.push_back(Trait::utf16ToString(it->second));
                }
            }
        } else {
            break;
        }

        p1 = p2 + 1;
    }

    res.push_back(s.sliced(i, s.size() - i));

    return res;
}

//! Remove backslashes in block.
template<class Trait>
inline typename MdBlock<Trait>::Data
removeBackslashes(const typename MdBlock<Trait>::Data &d)
{
    auto tmp = d;

    for (auto &line : tmp) {
        line.first = removeBackslashes<typename Trait::InternalString, Trait>(line.first);
    }

    return tmp;
}

//! Type of the paragraph's optimization.
enum class OptimizeParagraphType {
    //! Full optimization.
    Full,
    //! Semi optimization, optimization won't concatenate text
    //! items if style delimiters will be in the middle.
    Semi,
    //! Full optimization, but raw text data won't be concatenated (will be untouched).
    FullWithoutRawData,
    //! Semi optimization, but raw text data won't be concatenated (will be untouched).
    SemiWithoutRawData
};

//
// TextPlugin
//

//! ID of text plugin.
enum TextPlugin : int {
    //! Unknown plugin.
    UnknownPluginID = 0,
    //! GitHub's autolinks plugin.
    GitHubAutoLinkPluginID = 1,
    //! First user defined plugin ID.
    UserDefinedPluginID = 255
}; // enum TextPlugin

//
// Style
//

//! Emphasis type.
enum class Style {
    //! "*"
    Italic1,
    //! "_"
    Italic2,
    //! "**"
    Bold1,
    //! "__"
    Bold2,
    //! "~"
    Strikethrough,
    //! Unknown.
    Unknown
};

//! \return Text option from style.
inline TextOption
styleToTextOption(Style s)
{
    switch (s) {
    case Style::Italic1:
    case Style::Italic2:
        return ItalicText;

    case Style::Bold1:
    case Style::Bold2:
        return BoldText;

    case Style::Strikethrough:
        return StrikethroughText;

    default:
        return TextWithoutFormat;
    }
}

//
// TextPluginFunc
//

template<class Trait>
struct TextParsingOpts;

//! Functor type for text plugin.
template<class Trait>
using TextPluginFunc = std::function<void(std::shared_ptr<Paragraph<Trait>>,
                                          TextParsingOpts<Trait> &,
                                          const typename Trait::StringList &)>;

//
// TextPluginsMap
//

//! Type of the map of text plugins.
template<class Trait>
using TextPluginsMap = std::map<int, std::tuple<TextPluginFunc<Trait>,
                                                bool,
                                                typename Trait::StringList>>;

//
// TextParsingOpts
//

//! Internal structure for auxiliary options for parser.
template<class Trait>
struct TextParsingOpts {
    MdBlock<Trait> &m_fr;
    std::shared_ptr<Block<Trait>> m_parent;
    std::shared_ptr<RawHtml<Trait>> m_tmpHtml;
    std::shared_ptr<Document<Trait>> m_doc;
    typename Trait::StringList &m_linksToParse;
    typename Trait::String m_workingPath;
    typename Trait::String m_fileName;
    bool m_collectRefLinks;
    bool m_ignoreLineBreak;
    RawHtmlBlock<Trait> &m_html;
    const TextPluginsMap<Trait> &m_textPlugins;
    std::shared_ptr<Text<Trait>> m_lastText = {};
    bool m_wasRefLink = false;
    bool m_checkLineOnNewType = false;
    // This flag is set only in second step!
    bool m_firstInParagraph = true;
    bool m_headingAllowed = false;

    struct TextData {
        typename Trait::String m_str;
        long long int m_pos = -1;
        long long int m_line = -1;
    };

    std::vector<TextData> m_rawTextData = {};

    inline void
    concatenateAuxText(long long int start, long long int end)
    {
        if (start < end && (end - start > 1)) {
            for (auto i = start + 1; i < end; ++i) {
                m_rawTextData[start].m_str += m_rawTextData[i].m_str;
            }

            m_rawTextData.erase(m_rawTextData.cbegin() + start + 1, m_rawTextData.cbegin() + end);
        }
    }

    enum class Detected { Nothing, Table, HTML, Code, List, Blockquote }; // enum class Detected

    Detected m_detected = Detected::Nothing;

    inline bool
    shouldStopParsing() const
    {
        switch (m_detected) {
        case Detected::Table:
        case Detected::Code:
        case Detected::List:
        case Detected::Blockquote:
        case Detected::HTML:
            return true;

        default:
            return false;
        }
    }

    long long int m_line = 0;
    long long int m_pos = 0;
    long long int m_startTableLine = -1;
    long long int m_lastTextLine = -1;
    long long int m_lastTextPos = -1;
    int m_columnsCount = -1;
    int m_opts = TextWithoutFormat;

    struct StyleInfo {
        Style m_style;
        long long int m_length;
        bool m_bothFlanking;
    };

    std::vector<StyleInfo> m_styles = {};
    typename ItemWithOpts<Trait>::Styles m_openStyles = {};
    std::shared_ptr<ItemWithOpts<Trait>> m_lastItemWithStyle = nullptr;
}; // struct TextParsingOpts

//! Reset pre-stored HTML.
template<class Trait>
inline void resetHtmlTag(RawHtmlBlock<Trait> &html, TextParsingOpts<Trait> *po = nullptr)
{
    html.m_html.reset();
    html.m_parent.reset();
    html.m_htmlBlockType = -1;
    html.m_continueHtml = false;
    html.m_onLine = false;

    if (po && po->m_detected == TextParsingOpts<Trait>::Detected::HTML) {
        po->m_detected = TextParsingOpts<Trait>::Detected::Nothing;
    }
}

//
// virginSubstr
//

//! \return Substring from fragment with given virgin positions.
template<class Trait>
inline typename Trait::String
virginSubstr(const MdBlock<Trait> &fr, const WithPosition &virginPos)
{
    if (fr.m_data.empty()) {
        return {};
    }

    long long int startLine = virginPos.startLine() < fr.m_data.at(0).second.m_lineNumber ?
        (virginPos.endLine() < fr.m_data.at(0).second.m_lineNumber ? -1 : 0) :
        virginPos.startLine() - fr.m_data.at(0).second.m_lineNumber;

    if (startLine >= static_cast<long long int>(fr.m_data.size()) || startLine < 0) {
        return {};
    }

    auto spos = virginPos.startColumn() - fr.m_data.at(startLine).first.virginPos(0);

    if (spos < 0) {
        spos = 0;
    }

    long long int epos = 0;
    long long int linesCount = virginPos.endLine() - virginPos.startLine() -
        (virginPos.startLine() < fr.m_data.at(0).second.m_lineNumber ?
            fr.m_data.at(0).second.m_lineNumber - virginPos.startLine() : 0);

    if (startLine + linesCount > static_cast<long long int>(fr.m_data.size())) {
        linesCount = fr.m_data.size() - startLine - 1;
        epos = fr.m_data.back().first.length();
    } else {
        epos = virginPos.endColumn() - fr.m_data.at(linesCount + startLine).first.virginPos(0) + 1;
    }

    if (epos < 0) {
        epos = 0;
    }

    if (epos > fr.m_data.at(linesCount + startLine).first.length()) {
        epos = fr.m_data.at(linesCount + startLine).first.length();
    }

    typename Trait::String str =
        (linesCount ? fr.m_data.at(startLine).first.sliced(spos).asString() :
            fr.m_data.at(startLine).first.sliced(spos, epos - spos).asString());

    long long int i = startLine + 1;

    for (; i < startLine + linesCount; ++i) {
        str.push_back(Trait::latin1ToString("\n"));
        str.push_back(fr.m_data.at(i).first.asString());
    }

    if (linesCount) {
        str.push_back(Trait::latin1ToString("\n"));
        str.push_back(fr.m_data.at(i).first.sliced(0, epos).asString());
    }

    return str;
}

//
// localPosFromVirgin
//

//! \return Local position ( { column, line } ) in fragment for given virgin position if exists.
//! \return { -1, -1 } if there is no given position.
template<class Trait>
inline std::pair<long long int, long long int>
localPosFromVirgin(const MdBlock<Trait> &fr, long long int virginColumn, long long int virginLine)
{
    if (fr.m_data.empty()) {
        return {-1, -1};
    }

    if (fr.m_data.front().second.m_lineNumber > virginLine ||
        fr.m_data.back().second.m_lineNumber < virginLine) {
        return {-1, -1};
    }

    auto line = virginLine - fr.m_data.front().second.m_lineNumber;

    if (fr.m_data.at(line).first.isEmpty()) {
        return {-1, -1};
    }

    const auto vzpos = fr.m_data.at(line).first.virginPos(0);

    if (vzpos > virginColumn || virginColumn > vzpos + fr.m_data.at(line).first.length() - 1) {
        return {-1, -1};
    }

    return {virginColumn - vzpos, line};
}

//
// GitHubAutolinkPlugin
//

/*
    "^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?"
    "(?:\\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$"
*/
//! \return Is the given string a valid email?
template<class Trait>
inline bool
isEmail(const typename Trait::String &url)
{
    auto isAllowed = [](const typename Trait::Char &ch) -> bool {
        const auto unicode = ch.unicode();
        return ((unicode >= 48 && unicode <= 57) || (unicode >= 97 && unicode <= 122) ||
                (unicode >= 65 && unicode <= 90));
    };

    auto isAdditional = [](const typename Trait::Char &ch) -> bool {
        const auto unicode = ch.unicode();
        return (unicode == 33 || (unicode >= 35 && unicode <= 39) ||
                unicode == 42 || unicode == 43 || (unicode >= 45 && unicode <= 47) ||
                unicode == 61 || unicode == 63 || (unicode >= 94 && unicode <= 96) ||
                (unicode >= 123 && unicode <= 126));
    };

    static const auto s_delim = Trait::latin1ToChar('-');
    static const auto s_dog = Trait::latin1ToChar('@');
    static const auto s_dot = Trait::latin1ToChar('.');

    long long int i = (url.startsWith(Trait::latin1ToString("mailto:")) ? 7 : 0);
    const auto dogPos = url.indexOf(s_dog, i);

    if (dogPos != -1) {
        if (i == dogPos) {
            return false;
        }

        for (; i < dogPos; ++i) {
            if (!isAllowed(url[i]) && !isAdditional(url[i])) {
                return false;
            }
        }

        auto checkToDot = [&](long long int start, long long int dotPos) -> bool {
            static const long long int maxlen = 63;

            if (dotPos - start > maxlen ||
                start + 1 > dotPos ||
                start >= url.length() ||
                dotPos > url.length()) {
                return false;
            }

            if (url[start] == s_delim) {
                return false;
            }

            if (url[dotPos - 1] == s_delim) {
                return false;
            }

            for (; start < dotPos; ++start) {
                if (!isAllowed(url[start]) && url[start] != s_delim) {
                    return false;
                }
            }

            return true;
        };

        long long int dotPos = url.indexOf(s_dot, dogPos + 1);

        if (dotPos != -1) {
            i = dogPos + 1;

            while (dotPos != -1) {
                if (!checkToDot(i, dotPos)) {
                    return false;
                }

                i = dotPos + 1;
                dotPos = url.indexOf(s_dot, i);
            }

            if (!checkToDot(i, url.length())) {
                return false;
            }

            return true;
        }
    }

    return false;
}

//! \return Is the fiven string a valid URL?
template<class Trait>
inline bool
isValidUrl(const typename Trait::String &url);

//! \return Is the given string a GitHub autolink?
template<class Trait>
inline bool
isGitHubAutolink(const typename Trait::String &url);

#ifdef MD4QT_QT_SUPPORT

template<>
inline bool
isValidUrl<QStringTrait>(const QString &url)
{
    const QUrl u(url, QUrl::StrictMode);

    return (u.isValid() && !u.isRelative());
}

template<>
inline bool
isGitHubAutolink<QStringTrait>(const QString &url)
{
    const QUrl u(url, QUrl::StrictMode);

    return (u.isValid()
            && ((!u.scheme().isEmpty() && !u.host().isEmpty())
                || (url.startsWith(QStringLiteral("www.")) && url.length() >= 7 &&
                    url.indexOf(QLatin1Char('.'), 4) != -1)));
}

#endif

#ifdef MD4QT_ICU_STL_SUPPORT

template<>
inline bool
isValidUrl<UnicodeStringTrait>(const UnicodeString &url)
{
    const UrlUri u(url);

    return (u.isValid() && !u.isRelative());
}

template<>
inline bool
isGitHubAutolink<UnicodeStringTrait>(const UnicodeString &url)
{
    const UrlUri u(url);

    return (u.isValid()
            && ((!u.scheme().isEmpty() && !u.host().isEmpty())
                || (url.startsWith(UnicodeString("www.")) && url.length() >= 7 &&
                    url.indexOf(UnicodeChar('.'), 4) != -1)));
}

#endif

//! Process GitHub autolinks for the text with index \p idx.
template<class Trait>
inline long long int
processGitHubAutolinkExtension(std::shared_ptr<Paragraph<Trait>> p,
                               TextParsingOpts<Trait> &po,
                               long long int idx)
{
    if (idx < 0 || idx >= (long long int)po.m_rawTextData.size()) {
        return idx;
    }

    static const auto s_delims = Trait::latin1ToString("*_~()<>");
    auto s = po.m_rawTextData[idx];
    bool first = true;
    long long int j = 0;
    auto end = typename Trait::Char(0x00);
    bool skipSpace = true;
    long long int ret = idx;

    while (s.m_str.length()) {
        long long int i = 0;
        end = typename Trait::Char(0x00);

        for (; i < s.m_str.length(); ++i) {
            if (first) {
                if (s.m_str[i] == Trait::latin1ToChar('(')) {
                    end = Trait::latin1ToChar(')');
                }

                if (s_delims.indexOf(s.m_str[i]) == -1 && !s.m_str[i].isSpace()) {
                    first = false;
                    j = i;
                }
            } else {
                if (s.m_str[i].isSpace() || i == s.m_str.length() - 1 || s.m_str[i] == end) {
                    auto tmp = s.m_str.sliced(j, i - j +
                        (i == s.m_str.length() - 1 && s.m_str[i] != end && !s.m_str[i].isSpace() ?
                            1 : 0));
                    skipSpace = s.m_str[i].isSpace();

                    const auto email = isEmail<Trait>(tmp);

                    if (isGitHubAutolink<Trait>(tmp) || email) {
                        auto ti = textAtIdx(p, idx);

                        if (ti >= 0 && ti < static_cast<long long int>(p->items().size())) {
                            typename ItemWithOpts<Trait>::Styles openStyles, closeStyles;
                            const auto opts = std::static_pointer_cast<Text<Trait>>(p->items().at(ti))->opts();

                            if (j == 0 || s.m_str.sliced(0, j).isEmpty()) {
                                openStyles = std::static_pointer_cast<ItemWithOpts<Trait>>(p->items().at(ti))->openStyles();
                                closeStyles = std::static_pointer_cast<ItemWithOpts<Trait>>(p->items().at(ti))->closeStyles();
                                p->removeItemAt(ti);
                                po.m_rawTextData.erase(po.m_rawTextData.cbegin() + idx);
                                --ret;
                            } else {
                                const auto tmp = s.m_str.sliced(0, j);

                                auto t = std::static_pointer_cast<Text<Trait>>(p->items().at(ti));
                                t->setEndColumn(po.m_fr.m_data.at(s.m_line).first.virginPos(s.m_pos + j - 1));
                                closeStyles = t->closeStyles();
                                t->closeStyles() = {};
                                po.m_rawTextData[idx].m_str = tmp;
                                ++idx;
                                t->setText(removeBackslashes<typename Trait::String, Trait>(replaceEntity<Trait>(tmp)));
                                ++ti;
                            }

                            std::shared_ptr<Link<Trait>> lnk(new Link<Trait>);
                            lnk->setStartColumn(po.m_fr.m_data.at(s.m_line).first.virginPos(s.m_pos + j));
                            lnk->setStartLine(po.m_fr.m_data.at(s.m_line).second.m_lineNumber);
                            lnk->setEndColumn(
                                po.m_fr.m_data.at(s.m_line).first.virginPos(s.m_pos + i -
                                    (i == s.m_str.length() - 1 && s.m_str[i] != end && !s.m_str[i].isSpace() ?
                                        0 : 1)));
                            lnk->setEndLine(po.m_fr.m_data.at(s.m_line).second.m_lineNumber);
                            lnk->openStyles() = openStyles;
                            lnk->setTextPos({lnk->startColumn(), lnk->startLine(), lnk->endColumn(), lnk->endLine()});
                            lnk->setUrlPos(lnk->textPos());

                            if (email && !tmp.toLower().startsWith(Trait::latin1ToString("mailto:"))) {
                                tmp = Trait::latin1ToString("mailto:") + tmp;
                            }

                            if (!email && tmp.toLower().startsWith(Trait::latin1ToString("www."))) {
                                tmp = Trait::latin1ToString("http://") + tmp;
                            }

                            lnk->setUrl(tmp);
                            lnk->setOpts(opts);
                            p->insertItem(ti, lnk);

                            s.m_pos += i + (s.m_str[i] == end || s.m_str[i].isSpace() ? 0 : 1);
                            s.m_str.remove(0, i + (s.m_str[i] == end || s.m_str[i].isSpace() ? 0 : 1));
                            j = 0;
                            i = 0;

                            if (!s.m_str.isEmpty()) {
                                po.m_rawTextData.insert(po.m_rawTextData.cbegin() + idx, s);
                                ++ret;

                                auto t = std::make_shared<Text<Trait>>();
                                t->setStartColumn(po.m_fr.m_data[s.m_line].first.virginPos(s.m_pos));
                                t->setStartLine(po.m_fr.m_data.at(s.m_line).second.m_lineNumber);
                                t->setEndLine(po.m_fr.m_data.at(s.m_line).second.m_lineNumber);
                                t->setEndColumn(po.m_fr.m_data.at(s.m_line).first.virginPos(s.m_pos + s.m_str.length() - 1));
                                t->setText(removeBackslashes<typename Trait::String, Trait>(replaceEntity<Trait>(s.m_str)));
                                t->closeStyles() = closeStyles;
                                p->insertItem(ti + 1, t);
                            } else {
                                lnk->closeStyles() = closeStyles;
                            }

                            break;
                        }
                    }

                    j = i + (skipSpace ? 1 : 0);
                }
            }
        }

        first = true;

        if (i == s.m_str.length()) {
            break;
        }
    }

    return ret;
}

//! GitHub autolinks plugin.
template<class Trait>
inline void
githubAutolinkPlugin(std::shared_ptr<Paragraph<Trait>> p,
                     TextParsingOpts<Trait> &po,
                     const typename Trait::StringList &)
{
    if (!po.m_collectRefLinks) {
        long long int i = 0;

        while (i >= 0 && i < (long long int)po.m_rawTextData.size()) {
            i = processGitHubAutolinkExtension(p, po, i);

            ++i;
        }
    }
}

//
// Parser
//

//! Markdown parser.
template<class Trait>
class Parser final
{
public:
    Parser()
    {
        addTextPlugin(GitHubAutoLinkPluginID, githubAutolinkPlugin<Trait>, false, {});
    }

    ~Parser() = default;

    //! \return Parsed Markdown document.
    std::shared_ptr<Document<Trait>>
    parse(
        //! File name of the Markdown document.
        const typename Trait::String &fileName,
        //! Should parsing be recursive? If recursive all links to existing Markdown
        //! files will be parsed and presented in the returned document.
        bool recursive = true,
        //! Allowed extensions for Markdonw document files. If Markdown file doesn't
        //! have given extension it will be ignored.
        const typename Trait::StringList &ext = {Trait::latin1ToString("md"), Trait::latin1ToString("markdown")},
        //! Make full optimization, or just semi one. In full optimization
        //! text items with one style but with some closing delimiters
        //! in the middle will be concatenated in one, like in **text* text*,
        //! here in full optimization will be "text text" with 2 open/close
        //! style delimiters, but one closing delimiter is in the middle.
        bool fullyOptimizeParagraphs = true);

    //! \return Parsed Markdown document.
    std::shared_ptr<Document<Trait>>
    parse(
        //! Stream to parse.
        typename Trait::TextStream &stream,
        //! Absolute path to the root folder for the document.
        //! This path will be used to resolve local links.
        const typename Trait::String &path,
        //! This argument needed only for anchor.
        const typename Trait::String &fileName,
        //! Make full optimization, or just semi one. In full optimization
        //! text items with one style but with some closing delimiters
        //! in the middle will be concatenated in one, like in **text* text*,
        //! here in full optimization will be "text text" with 2 open/close
        //! style delimiters, but one closing delimiter is in the middle.
        bool fullyOptimizeParagraphs = true);

    //! \return Parsed Markdown document.
    std::shared_ptr<Document<Trait>>
    parse(
        //! File name of the Markdown document (full path).
        const typename Trait::String &fileName,
        //! Absolute path to the working directory for the document.
        //! This path will be used to resolve local links.
        //! \warning This path should be in \p fileName path.
        const typename Trait::String &workingDirectory,
        //! Should parsing be recursive? If recursive all links to existing Markdown
        //! files will be parsed and presented in the returned document.
        bool recursive = true,
        //! Allowed extensions for Markdonw document files. If Markdown file doesn't
        //! have given extension it will be ignored.
        const typename Trait::StringList &ext = {Trait::latin1ToString("md"), Trait::latin1ToString("markdown")},
        //! Make full optimization, or just semi one. In full optimization
        //! text items with one style but with some closing delimiters
        //! in the middle will be concatenated in one, like in **text* text*,
        //! here in full optimization will be "text text" with 2 open/close
        //! style delimiters, but one closing delimiter is in the middle.
        bool fullyOptimizeParagraphs = true);

    //! Add text plugin.
    void
    addTextPlugin(
        //! ID of a plugin. Use TextPlugin::UserDefinedPluginID value for start ID.
        int id,
        //! Function of a plugin, that will be invoked to processs raw text.
        TextPluginFunc<Trait> plugin,
        //! Should this plugin be used in parsing of internals of links?
        bool processInLinks,
        //! User data that will be passed to plugin function.
        const typename Trait::StringList &userData)
    {
        m_textPlugins.insert({id, {plugin, processInLinks, userData}});
    }

    //! Remove text plugin.
    void
    removeTextPlugin(
        //! ID of plugin that should be removed.
        int id)
    {
        m_textPlugins.erase(id);
    }

private:
    void
    parseFile(const typename Trait::String &fileName,
              bool recursive,
              std::shared_ptr<Document<Trait>> doc,
              const typename Trait::StringList &ext,
              typename Trait::StringList *parentLinks = nullptr,
              typename Trait::String workingDirectory = {});

    void
    parseStream(typename Trait::TextStream &stream,
                const typename Trait::String &workingPath,
                const typename Trait::String &fileName,
                bool recursive,
                std::shared_ptr<Document<Trait>> doc,
                const typename Trait::StringList &ext,
                typename Trait::StringList *parentLinks = nullptr,
                const typename Trait::String &workingDirectory = {});

    void
    clearCache();

    enum class BlockType {
        Unknown,
        EmptyLine,
        Text,
        List,
        ListWithFirstEmptyLine,
        CodeIndentedBySpaces,
        Code,
        Blockquote,
        Heading,
        SomethingInList,
        FensedCodeInList,
        Footnote
    }; // enum BlockType

    struct ListIndent {
        long long int m_level = -1;
        long long int m_indent = -1;
    }; // struct ListIndent

    BlockType
    whatIsTheLine(typename Trait::InternalString &str,
                  bool inList = false,
                  bool inListWithFirstEmptyLine = false,
                  bool fensedCodeInList = false,
                  typename Trait::String *startOfCode = nullptr,
                  ListIndent *indent = nullptr,
                  bool emptyLinePreceded = false,
                  bool calcIndent = false,
                  const std::vector<long long int> *indents = nullptr);

    long long int
    parseFragment(MdBlock<Trait> &fr,
                  std::shared_ptr<Block<Trait>> parent,
                  std::shared_ptr<Document<Trait>> doc,
                  typename Trait::StringList &linksToParse,
                  const typename Trait::String &workingPath,
                  const typename Trait::String &fileName,
                  bool collectRefLinks,
                  RawHtmlBlock<Trait> &html);

    long long int
    parseText(MdBlock<Trait> &fr,
              std::shared_ptr<Block<Trait>> parent,
              std::shared_ptr<Document<Trait>> doc,
              typename Trait::StringList &linksToParse,
              const typename Trait::String &workingPath,
              const typename Trait::String &fileName,
              bool collectRefLinks,
              RawHtmlBlock<Trait> &html);

    long long int
    parseBlockquote(MdBlock<Trait> &fr,
                    std::shared_ptr<Block<Trait>> parent,
                    std::shared_ptr<Document<Trait>> doc,
                    typename Trait::StringList &linksToParse,
                    const typename Trait::String &workingPath,
                    const typename Trait::String &fileName,
                    bool collectRefLinks,
                    RawHtmlBlock<Trait> &html);

    long long int
    parseList(MdBlock<Trait> &fr,
              std::shared_ptr<Block<Trait>> parent,
              std::shared_ptr<Document<Trait>> doc,
              typename Trait::StringList &linksToParse,
              const typename Trait::String &workingPath,
              const typename Trait::String &fileName,
              bool collectRefLinks,
              RawHtmlBlock<Trait> &html);

    long long int
    parseCode(MdBlock<Trait> &fr,
              std::shared_ptr<Block<Trait>> parent,
              bool collectRefLinks);

    long long int
    parseCodeIndentedBySpaces(MdBlock<Trait> &fr,
                              std::shared_ptr<Block<Trait>> parent,
                              bool collectRefLinks,
                              int indent,
                              const typename Trait::String &syntax,
                              long long int emptyColumn,
                              long long int startLine,
                              bool fensedCode,
                              const WithPosition &startDelim = {},
                              const WithPosition &endDelim = {},
                              const WithPosition &syntaxPos = {});

    long long int
    parseListItem(MdBlock<Trait> &fr,
                  std::shared_ptr<Block<Trait>> parent,
                  std::shared_ptr<Document<Trait>> doc,
                  typename Trait::StringList &linksToParse,
                  const typename Trait::String &workingPath,
                  const typename Trait::String &fileName,
                  bool collectRefLinks,
                  RawHtmlBlock<Trait> &html,
                  std::shared_ptr<ListItem<Trait>> *resItem = nullptr);

    void
    parseHeading(MdBlock<Trait> &fr,
                 std::shared_ptr<Block<Trait>> parent,
                 std::shared_ptr<Document<Trait>> doc,
                 typename Trait::StringList &linksToParse,
                 const typename Trait::String &workingPath,
                 const typename Trait::String &fileName,
                 bool collectRefLinks);

    void
    parseFootnote(MdBlock<Trait> &fr,
                  std::shared_ptr<Block<Trait>> parent,
                  std::shared_ptr<Document<Trait>> doc,
                  typename Trait::StringList &linksToParse,
                  const typename Trait::String &workingPath,
                  const typename Trait::String &fileName,
                  bool collectRefLinks);

    void
    parseTable(MdBlock<Trait> &fr,
               std::shared_ptr<Block<Trait>> parent,
               std::shared_ptr<Document<Trait>> doc,
               typename Trait::StringList &linksToParse,
               const typename Trait::String &workingPath,
               const typename Trait::String &fileName,
               bool collectRefLinks,
               int columnsCount);

    long long int
    parseParagraph(MdBlock<Trait> &fr,
                   std::shared_ptr<Block<Trait>> parent,
                   std::shared_ptr<Document<Trait>> doc,
                   typename Trait::StringList &linksToParse,
                   const typename Trait::String &workingPath,
                   const typename Trait::String &fileName,
                   bool collectRefLinks,
                   RawHtmlBlock<Trait> &html);

    long long int
    parseFormattedTextLinksImages(MdBlock<Trait> &fr,
                                  std::shared_ptr<Block<Trait>> parent,
                                  std::shared_ptr<Document<Trait>> doc,
                                  typename Trait::StringList &linksToParse,
                                  const typename Trait::String &workingPath,
                                  const typename Trait::String &fileName,
                                  bool collectRefLinks,
                                  bool ignoreLineBreak,
                                  RawHtmlBlock<Trait> &html,
                                  bool inLink);

    struct ParserContext {
        typename Trait::template Vector<MdBlock<Trait>> m_splitted;
        typename MdBlock<Trait>::Data m_fragment;
        bool m_emptyLineInList = false;
        bool m_fensedCodeInList = false;
        long long int m_emptyLinesCount = 0;
        long long int m_lineCounter = 0;
        std::vector<long long int> m_indents;
        ListIndent m_indent;
        RawHtmlBlock<Trait> m_html;
        long long int m_emptyLinesBefore = 0;
        MdLineData::CommentDataMap m_htmlCommentData;
        typename Trait::String m_startOfCode;
        typename Trait::String m_startOfCodeInList;
        BlockType m_type = BlockType::EmptyLine;
        BlockType m_lineType = BlockType::Unknown;
        BlockType m_prevLineType = BlockType::Unknown;
    }; // struct ParserContext

    std::pair<long long int, bool>
    parseFirstStep(ParserContext &ctx,
          StringListStream<Trait> &stream,
          std::shared_ptr<Block<Trait>> parent,
          std::shared_ptr<Document<Trait>> doc,
          typename Trait::StringList &linksToParse,
          const typename Trait::String &workingPath,
          const typename Trait::String &fileName,
          bool collectRefLinks);

    void
    parseSecondStep(ParserContext &ctx,
          std::shared_ptr<Block<Trait>> parent,
          std::shared_ptr<Document<Trait>> doc,
          typename Trait::StringList &linksToParse,
          const typename Trait::String &workingPath,
          const typename Trait::String &fileName,
          bool collectRefLinks,
          bool top,
          bool dontProcessLastFreeHtml);

    std::pair<RawHtmlBlock<Trait>, long long int>
    parse(StringListStream<Trait> &stream,
          std::shared_ptr<Block<Trait>> parent,
          std::shared_ptr<Document<Trait>> doc,
          typename Trait::StringList &linksToParse,
          const typename Trait::String &workingPath,
          const typename Trait::String &fileName,
          bool collectRefLinks,
          bool top = false,
          bool dontProcessLastFreeHtml = false,
          bool stopOnMayBreakList = false);

    std::pair<long long int, bool>
    parseFragment(ParserContext &ctx,
                  std::shared_ptr<Block<Trait>> parent,
                  std::shared_ptr<Document<Trait>> doc,
                  typename Trait::StringList &linksToParse,
                  const typename Trait::String &workingPath,
                  const typename Trait::String &fileName,
                  bool collectRefLinks);

    void
    eatFootnote(ParserContext &ctx,
                StringListStream<Trait> &stream,
                std::shared_ptr<Block<Trait>> parent,
                std::shared_ptr<Document<Trait>> doc,
                typename Trait::StringList &linksToParse,
                const typename Trait::String &workingPath,
                const typename Trait::String &fileName,
                bool collectRefLinks);

    void
    finishHtml(ParserContext &ctx,
               std::shared_ptr<Block<Trait>> parent,
               std::shared_ptr<Document<Trait>> doc,
               bool collectRefLinks,
               bool top,
               bool dontProcessLastFreeHtml);

    void
    makeLineMain(ParserContext &ctx,
                 const typename Trait::InternalString &line,
                 long long int emptyLinesCount,
                 const ListIndent &currentIndent,
                 long long int ns,
                 long long int currentLineNumber);

    std::pair<long long int, bool>
    parseFragmentAndMakeNextLineMain(ParserContext &ctx,
                                     std::shared_ptr<Block<Trait>> parent,
                                     std::shared_ptr<Document<Trait>> doc,
                                     typename Trait::StringList &linksToParse,
                                     const typename Trait::String &workingPath,
                                     const typename Trait::String &fileName,
                                     bool collectRefLinks,
                                     const typename Trait::InternalString &line,
                                     const ListIndent &currentIndent,
                                     long long int ns,
                                     long long int currentLineNumber);

    bool
    isListType(BlockType t);

    std::pair<typename Trait::InternalString, bool>
    readLine(ParserContext &ctx, StringListStream<Trait> &stream);

    std::shared_ptr<Image<Trait>>
    makeImage(const typename Trait::String &url,
              const typename MdBlock<Trait>::Data &text,
              TextParsingOpts<Trait> &po,
              bool doNotCreateTextOnFail,
              long long int startLine,
              long long int startPos,
              long long int lastLine,
              long long int lastPos,
              const WithPosition &textPos,
              const WithPosition &urlPos);

    std::shared_ptr<Link<Trait>>
    makeLink(const typename Trait::String &url,
             const typename MdBlock<Trait>::Data &text,
             TextParsingOpts<Trait> &po,
             bool doNotCreateTextOnFail,
             long long int startLine,
             long long int startPos,
             long long int lastLine,
             long long int lastPos,
             const WithPosition &textPos,
             const WithPosition &urlPos);

    struct Delimiter {
        enum DelimiterType {
            // (
            ParenthesesOpen,
            // )
            ParenthesesClose,
            // [
            SquareBracketsOpen,
            // ]
            SquareBracketsClose,
            // ![
            ImageOpen,
            // ~~
            Strikethrough,
            // *
            Emphasis1,
            // _
            Emphasis2,
            // `
            InlineCode,
            // <
            Less,
            // >
            Greater,
            // $
            Math,
            HorizontalLine,
            H1,
            H2,
            Unknown
        }; // enum DelimiterType

        DelimiterType m_type = Unknown;
        long long int m_line = -1;
        long long int m_pos = -1;
        long long int m_len = 0;
        bool m_isWordBefore = false;
        bool m_backslashed = false;
        bool m_leftFlanking = false;
        bool m_rightFlanking = false;
        bool m_skip = false;
    }; // struct Delimiter

    using Delims = typename Trait::template Vector<Delimiter>;

    bool
    createShortcutImage(const typename MdBlock<Trait>::Data &text,
                        TextParsingOpts<Trait> &po,
                        long long int startLine,
                        long long int startPos,
                        long long int lastLineForText,
                        long long int lastPosForText,
                        typename Delims::iterator lastIt,
                        const typename MdBlock<Trait>::Data &linkText,
                        bool doNotCreateTextOnFail,
                        const WithPosition &textPos,
                        const WithPosition &linkTextPos);

    typename Delims::iterator
    checkForImage(typename Delims::iterator it,
                  typename Delims::iterator last,
                  TextParsingOpts<Trait> &po);

    bool
    createShortcutLink(const typename MdBlock<Trait>::Data &text,
                       TextParsingOpts<Trait> &po,
                       long long int startLine,
                       long long int startPos,
                       long long int lastLineForText,
                       long long int lastPosForText,
                       typename Delims::iterator lastIt,
                       const typename MdBlock<Trait>::Data &linkText,
                       bool doNotCreateTextOnFail,
                       const WithPosition &textPos,
                       const WithPosition &linkTextPos);

    typename Delims::iterator
    checkForLink(typename Delims::iterator it,
                 typename Delims::iterator last,
                 TextParsingOpts<Trait> &po);

    Delims
    collectDelimiters(const typename MdBlock<Trait>::Data &fr);

    std::pair<typename Trait::String, bool>
    readHtmlTag(typename Delims::iterator it, TextParsingOpts<Trait> &po);

    typename Delims::iterator
    findIt(typename Delims::iterator it,
           typename Delims::iterator last,
           TextParsingOpts<Trait> &po);

    typename Delims::iterator
    eatRawHtmlTillEmptyLine(typename Delims::iterator it,
                            typename Delims::iterator last,
                            long long int line,
                            long long int pos,
                            TextParsingOpts<Trait> &po,
                            int htmlRule,
                            bool onLine,
                            bool continueEating = false);

    void
    finishRule1HtmlTag(typename Delims::iterator it,
                       typename Delims::iterator last,
                       TextParsingOpts<Trait> &po,
                       bool skipFirst);

    void
    finishRule2HtmlTag(typename Delims::iterator it,
                       typename Delims::iterator last,
                       TextParsingOpts<Trait> &po);

    void
    finishRule3HtmlTag(typename Delims::iterator it,
                       typename Delims::iterator last,
                       TextParsingOpts<Trait> &po);

    void
    finishRule4HtmlTag(typename Delims::iterator it,
                       typename Delims::iterator last,
                       TextParsingOpts<Trait> &po);

    void
    finishRule5HtmlTag(typename Delims::iterator it,
                       typename Delims::iterator last,
                       TextParsingOpts<Trait> &po);

    void
    finishRule6HtmlTag(typename Delims::iterator it,
                       typename Delims::iterator last,
                       TextParsingOpts<Trait> &po);

    void
    finishRule7HtmlTag(typename Delims::iterator it,
                       typename Delims::iterator last,
                       TextParsingOpts<Trait> &po);

    typename Delims::iterator
    finishRawHtmlTag(typename Delims::iterator it,
                     typename Delims::iterator last,
                     TextParsingOpts<Trait> &po,
                     bool skipFirst);

    int
    htmlTagRule(typename Delims::iterator it,
                typename Delims::iterator last,
                TextParsingOpts<Trait> &po);

    typename Delims::iterator
    checkForRawHtml(typename Delims::iterator it,
                    typename Delims::iterator last,
                    TextParsingOpts<Trait> &po);

    typename Delims::iterator
    checkForMath(typename Delims::iterator it,
                 typename Delims::iterator last,
                 TextParsingOpts<Trait> &po);

    typename Delims::iterator
    checkForAutolinkHtml(typename Delims::iterator it,
                         typename Delims::iterator last,
                         TextParsingOpts<Trait> &po,
                         bool updatePos);

    typename Delims::iterator
    checkForInlineCode(typename Delims::iterator it,
                       typename Delims::iterator last,
                       TextParsingOpts<Trait> &po);

    std::pair<typename MdBlock<Trait>::Data, typename Delims::iterator>
    readTextBetweenSquareBrackets(typename Delims::iterator start,
                                  typename Delims::iterator it,
                                  typename Delims::iterator last,
                                  TextParsingOpts<Trait> &po,
                                  bool doNotCreateTextOnFail,
                                  WithPosition *pos = nullptr);

    std::pair<typename MdBlock<Trait>::Data, typename Delims::iterator>
    checkForLinkText(typename Delims::iterator it,
                     typename Delims::iterator last,
                     TextParsingOpts<Trait> &po,
                     WithPosition *pos = nullptr);

    std::pair<typename MdBlock<Trait>::Data, typename Delims::iterator>
    checkForLinkLabel(typename Delims::iterator it,
                      typename Delims::iterator last,
                      TextParsingOpts<Trait> &po,
                      WithPosition *pos = nullptr);

    std::tuple<typename Trait::String, typename Trait::String, typename Delims::iterator, bool>
    checkForInlineLink(typename Delims::iterator it,
                       typename Delims::iterator last,
                       TextParsingOpts<Trait> &po,
                       WithPosition *urlPos = nullptr);

    inline std::tuple<typename Trait::String, typename Trait::String, typename Delims::iterator, bool>
    checkForRefLink(typename Delims::iterator it,
                    typename Delims::iterator last,
                    TextParsingOpts<Trait> &po,
                    WithPosition *urlPos = nullptr);

    typename Trait::String
    toSingleLine(const typename MdBlock<Trait>::Data &d);

    template<class Func>
    typename Delims::iterator
    checkShortcut(typename Delims::iterator it,
                  typename Delims::iterator last,
                  TextParsingOpts<Trait> &po,
                  Func functor)
    {
        const auto start = it;

        typename MdBlock<Trait>::Data text;

        WithPosition labelPos;
        std::tie(text, it) = checkForLinkLabel(start, last, po, &labelPos);

        if (it != start && !toSingleLine(text).simplified().isEmpty()) {
            if ((this->*functor)(text, po, start->m_line, start->m_pos, start->m_line,
                start->m_pos + start->m_len, it, {}, false, labelPos, {})) {
                return it;
            }
        }

        return start;
    }

    bool
    isSequence(typename Delims::iterator it,
               long long int itLine,
               long long int itPos,
               typename Delimiter::DelimiterType t);

    std::pair<typename Delims::iterator, typename Delims::iterator>
    readSequence(typename Delims::iterator first,
                 typename Delims::iterator it,
                 typename Delims::iterator last,
                 long long int &pos,
                 long long int &length,
                 long long int &itCount,
                 long long int &lengthFromIt,
                 long long int &itCountFromIt);

    typename Delims::iterator
    readSequence(typename Delims::iterator it,
                 typename Delims::iterator last,
                 long long int &line,
                 long long int &pos,
                 long long int &len,
                 long long int &itCount);

    int
    emphasisToInt(typename Delimiter::DelimiterType t);

    void
    createStyles(std::vector<std::pair<Style, long long int>> & styles,
                 typename Delimiter::DelimiterType t,
                 long long int style);

    std::vector<std::pair<Style, long long int>>
    createStyles(typename Delimiter::DelimiterType t,
                 const std::vector<long long int> &styles,
                 long long int lastStyle);

    std::tuple<bool, std::vector<std::pair<Style, long long int>>, long long int, long long int>
    isStyleClosed(typename Delims::iterator first,
                  typename Delims::iterator it,
                  typename Delims::iterator last,
                  typename Delims::iterator &stackBottom,
                  TextParsingOpts<Trait> &po);

    typename Delims::iterator
    incrementIterator(typename Delims::iterator it,
                      typename Delims::iterator last,
                      long long int count);

    typename Delims::iterator
    checkForStyle(typename Delims::iterator first,
                  typename Delims::iterator it,
                  typename Delims::iterator last,
                  typename Delims::iterator &stackBottom,
                  TextParsingOpts<Trait> &po);

    bool
    isNewBlockIn(MdBlock<Trait> &fr,
                 long long int startLine,
                 long long int endLine);

    void
    makeInlineCode(long long int startLine,
                   long long int startPos,
                   long long int lastLine,
                   long long int lastPos,
                   TextParsingOpts<Trait> &po,
                   typename Delims::iterator startDelimIt,
                   typename Delims::iterator endDelimIt);

    OptimizeParagraphType
    defaultParagraphOptimization() const
    {
        return (m_fullyOptimizeParagraphs ? OptimizeParagraphType::Full :
            OptimizeParagraphType::Semi);
    }

private:
    //! Used in tests.
    friend struct PrivateAccess;

private:
    typename Trait::StringList m_parsedFiles;
    TextPluginsMap<Trait> m_textPlugins;
    bool m_fullyOptimizeParagraphs = true;

    MD_DISABLE_COPY(Parser)
}; // class Parser

//
// Parser
//

template<class Trait>
inline std::shared_ptr<Document<Trait>>
Parser<Trait>::parse(const typename Trait::String &fileName,
                     bool recursive,
                     const typename Trait::StringList &ext,
                     bool fullyOptimizeParagraphs)
{
    m_fullyOptimizeParagraphs = fullyOptimizeParagraphs;

    std::shared_ptr<Document<Trait>> doc(new Document<Trait>);

    parseFile(fileName, recursive, doc, ext);

    clearCache();

    return doc;
}

template<class Trait>
inline std::shared_ptr<Document<Trait>>
Parser<Trait>::parse(typename Trait::TextStream &stream,
                     const typename Trait::String &path,
                     const typename Trait::String &fileName,
                     bool fullyOptimizeParagraphs)
{
    m_fullyOptimizeParagraphs = fullyOptimizeParagraphs;

    std::shared_ptr<Document<Trait>> doc(new Document<Trait>);

    parseStream(stream, path, fileName, false, doc, typename Trait::StringList());

    clearCache();

    return doc;
}

template<class Trait>
inline std::shared_ptr<Document<Trait>>
Parser<Trait>::parse(const typename Trait::String &fileName,
                     const typename Trait::String &workingDirectory,
                     bool recursive,
                     const typename Trait::StringList &ext,
                     bool fullyOptimizeParagraphs)
{
    m_fullyOptimizeParagraphs = fullyOptimizeParagraphs;

    std::shared_ptr<Document<Trait>> doc(new Document<Trait>);

    typename Trait::String wd;
    auto i = workingDirectory.length() - 1;

    while (i > 0) {
        if (workingDirectory[i] != Trait::latin1ToChar('\\') &&
            workingDirectory[i] != Trait::latin1ToChar('/')) {
            break;
        } else {
            --i;
        }
    }

    if (i == 0 && workingDirectory[i] == Trait::latin1ToChar('/')) {
        wd = Trait::latin1ToString("/");
    } else if (i > 0) {
        wd = workingDirectory.sliced(0, i + 1);
    }

    parseFile(fileName, recursive, doc, ext, nullptr, wd);

    clearCache();

    return doc;
}

template<class Trait>
class TextStream;

#ifdef MD4QT_QT_SUPPORT

//! Wrapper for QTextStream.
template<>
class TextStream<QStringTrait>
{
public:
    TextStream(QTextStream &stream)
        : m_stream(stream)
        , m_lastBuf(false)
        , m_pos(0)
    {
    }

    bool
    atEnd() const
    {
        return (m_lastBuf && m_pos == m_buf.size());
    }

    QString
    readLine()
    {
        QString line;
        bool rFound = false;

        while (!atEnd()) {
            const auto c = getChar();

            if (rFound && c != QLatin1Char('\n')) {
                --m_pos;

                return line;
            }

            if (c == QLatin1Char('\r')) {
                rFound = true;

                continue;
            } else if (c == QLatin1Char('\n')) {
                return line;
            }

            if (!c.isNull()) {
                line.push_back(c);
            }
        }

        return line;
    }

private:
    void
    fillBuf()
    {
        m_buf = m_stream.read(512);

        if (m_stream.atEnd()) {
            m_lastBuf = true;
        }

        m_pos = 0;
    }

    QChar
    getChar()
    {
        if (m_pos < m_buf.size()) {
            return m_buf.at(m_pos++);
        } else if (!atEnd()) {
            fillBuf();

            return getChar();
        } else {
            return QChar();
        }
    }

private:
    QTextStream &m_stream;
    QString m_buf;
    bool m_lastBuf;
    long long int m_pos;
}; // class TextStream

#endif

#ifdef MD4QT_ICU_STL_SUPPORT

//! Wrapper for std::istream.
template<>
class TextStream<UnicodeStringTrait>
{
public:
    TextStream(std::istream &stream)
        : m_pos(0)
    {
        std::vector<unsigned char> content;

        stream.seekg(0, std::ios::end);
        const auto ssize = stream.tellg();
        content.resize((size_t)ssize + 1);
        stream.seekg(0, std::ios::beg);
        stream.read((char *)&content[0], ssize);
        content[(size_t)ssize] = 0;

        const auto z = std::count(content.cbegin(), content.cend(), 0);

        if (z > 1) {
            std::vector<unsigned char> tmp;
            tmp.resize(content.size() + (z - 1) * 2);

            for (size_t i = 0, j = 0; i < content.size() - 1; ++i, ++j) {
                if (content[i] == 0) {
                    // 0xFFFD - replacement character in UTF-8.
                    tmp[j++] = 0xEF;
                    tmp[j++] = 0xBF;
                    tmp[j] = 0xBD;
                } else {
                    tmp[j] = content[i];
                }
            }

            tmp[tmp.size() - 1] = 0;

            std::swap(content, tmp);
        }

        m_str = UnicodeString::fromUTF8((char *)&content[0]);
    }

    bool
    atEnd() const
    {
        return m_pos == m_str.size();
    }

    UnicodeString
    readLine()
    {
        UnicodeString line;

        bool rFound = false;

        while (!atEnd()) {
            const auto c = getChar();

            if (rFound && c != UnicodeChar('\n')) {
                --m_pos;

                return line;
            }

            if (c == UnicodeChar('\r')) {
                rFound = true;

                continue;
            } else if (c == UnicodeChar('\n')) {
                return line;
            }

            if (!c.isNull()) {
                line.push_back(c);
            }
        }

        return line;
    }

private:
    UnicodeChar
    getChar()
    {
        if (!atEnd()) {
            return m_str[m_pos++];
        } else {
            return UnicodeChar();
        }
    }

private:
    UnicodeString m_str;
    long long int m_pos;
};

#endif

//! \return Is HTML comment closed?
template<class Trait>
inline bool
checkForEndHtmlComments(const typename Trait::String &line,
                        long long int pos)
{
    const long long int e = line.indexOf(Trait::latin1ToString("-->"), pos);

    if (e != -1) {
        return isHtmlComment<Trait>(line.sliced(0, e + 3));
    }

    return false;
}

//! Collect information about HTML comments.
template<class Trait>
inline void
checkForHtmlComments(const typename Trait::InternalString &line,
                     StringListStream<Trait> &stream,
                     MdLineData::CommentDataMap &res)
{
    long long int p = 0, l = stream.currentStreamPos();

    const auto &str = line.asString();

    while ((p = str.indexOf(Trait::latin1ToString(s_startComment), p)) != -1) {
        bool addNegative = false;

        auto c = str.sliced(p);

        if (c.startsWith(Trait::latin1ToString("<!-->"))) {
            res.insert({line.virginPos(p), {0, true}});

            p += 5;

            continue;
        } else if (c.startsWith(Trait::latin1ToString("<!--->"))) {
            res.insert({line.virginPos(p), {1, true}});

            p += 6;

            continue;
        }

        if (checkForEndHtmlComments<Trait>(c, 4)) {
            res.insert({line.virginPos(p), {2, true}});
        } else {
            addNegative = true;

            for (; l < stream.size(); ++l) {
                c.push_back(Trait::latin1ToChar(' '));
                c.push_back(stream.lineAt(l).asString());

                if (checkForEndHtmlComments<Trait>(c, 4)) {
                    res.insert({line.virginPos(p), {2, true}});

                    addNegative = false;

                    break;
                }
            }
        }

        if (addNegative) {
            res.insert({line.virginPos(p), {-1, false}});
        }

        ++p;
    }
}

template<class Trait>
inline std::pair<long long int, bool>
Parser<Trait>::parseFragment(typename Parser<Trait>::ParserContext &ctx,
                             std::shared_ptr<Block<Trait>> parent,
                             std::shared_ptr<Document<Trait>> doc,
                             typename Trait::StringList &linksToParse,
                             const typename Trait::String &workingPath,
                             const typename Trait::String &fileName,
                             bool collectRefLinks)
{
    auto clearCtx = [&ctx] () {
        ctx.m_fragment.clear();
        ctx.m_type = BlockType::EmptyLine;
        ctx.m_emptyLineInList = false;
        ctx.m_fensedCodeInList = false;
        ctx.m_emptyLinesCount = 0;
        ctx.m_lineCounter = 0;
        ctx.m_indents.clear();
        ctx.m_indent = {-1, -1};
        ctx.m_startOfCode.clear();
        ctx.m_startOfCodeInList.clear();
    };

    if (!ctx.m_fragment.empty()) {
        MdBlock<Trait> block = {ctx.m_fragment, ctx.m_emptyLinesBefore, ctx.m_emptyLinesCount > 0};

        const auto line = parseFragment(block, parent, doc, linksToParse, workingPath,
                                        fileName, collectRefLinks, ctx.m_html);

        assert(line != ctx.m_fragment.front().second.m_lineNumber);

        if (line > 0) {
            if (ctx.m_html.m_html) {
                if (!collectRefLinks) {
                    ctx.m_html.m_parent->appendItem(ctx.m_html.m_html);
                }

                resetHtmlTag<Trait>(ctx.m_html);
            }

            const auto it = ctx.m_fragment.cbegin() + (line - ctx.m_fragment.cbegin()->second.m_lineNumber);

            MdBlock<Trait> tmp = {{}, ctx.m_emptyLinesBefore, false};
            std::copy(ctx.m_fragment.cbegin(), it, std::back_inserter(tmp.m_data));

            long long int emptyLines = 0;

            while (!tmp.m_data.empty() && tmp.m_data.back().first.asString().simplified().isEmpty()) {
                tmp.m_data.pop_back();
                tmp.m_emptyLineAfter = true;
                ++emptyLines;
            }

            if (!tmp.m_data.empty()) {
                ctx.m_splitted.push_back(tmp);
            }

            const auto retLine = it->second.m_lineNumber;
            const auto retMayBreakList = it->second.m_mayBreakList;

            clearCtx();

            ctx.m_emptyLinesBefore = emptyLines;

            return {retLine, retMayBreakList};
        }

        ctx.m_splitted.push_back({ctx.m_fragment, ctx.m_emptyLinesBefore, ctx.m_emptyLinesCount > 0});
    }

    clearCtx();

    return {-1, false};
}

//! Replace tabs with spaces (just for internal simpler use).
template<class Trait>
inline void
replaceTabs(typename Trait::InternalString &s)
{
    unsigned char size = 4;
    long long int len = s.length();

    for (long long int i = 0; i < len; ++i, --size) {
        if (s[i] == Trait::latin1ToChar('\t')) {
            s.replaceOne(i, 1, typename Trait::String(size, Trait::latin1ToChar(' ')));

            len += size - 1;
            i += size - 1;
            size = 5;
        }

        if (size == 1) {
            size = 5;
        }
    }
}

template<class Trait>
inline void
Parser<Trait>::eatFootnote(typename Parser<Trait>::ParserContext &ctx,
                           StringListStream<Trait> &stream,
                           std::shared_ptr<Block<Trait>> parent,
                           std::shared_ptr<Document<Trait>> doc,
                           typename Trait::StringList &linksToParse,
                           const typename Trait::String &workingPath,
                           const typename Trait::String &fileName,
                           bool collectRefLinks)
{
    long long int emptyLinesCount = 0;
    bool wasEmptyLine = false;

    while (!stream.atEnd()) {
        const auto currentLineNumber = stream.currentLineNumber();

        typename Trait::InternalString line;
        bool mayBreak;

        std::tie(line, mayBreak) = readLine(ctx, stream);

        replaceTabs<Trait>(line);

        const auto ns = skipSpaces<Trait>(0, line.asString());

        if (ns == line.length() || line.asString().startsWith(Trait::latin1ToString("    "))) {
            if (ns == line.length()) {
                ++emptyLinesCount;
                wasEmptyLine = true;
            } else {
                emptyLinesCount = 0;
            }

            ctx.m_fragment.push_back({line, {currentLineNumber, ctx.m_htmlCommentData, mayBreak}});
        } else if (!wasEmptyLine) {
            if (isFootnote<Trait>(line.sliced(ns).asString())) {
                parseFragment(ctx, parent, doc, linksToParse, workingPath, fileName, collectRefLinks);

                ctx.m_lineType = BlockType::Footnote;

                makeLineMain(ctx, line, emptyLinesCount, ctx.m_indent, ns, currentLineNumber);

                continue;
            } else {
                ctx.m_fragment.push_back({line, {currentLineNumber, ctx.m_htmlCommentData, mayBreak}});
            }
        } else {
            parseFragment(ctx, parent, doc, linksToParse, workingPath, fileName, collectRefLinks);

            ctx.m_lineType =
                whatIsTheLine(line, false, false, false, &ctx.m_startOfCodeInList, &ctx.m_indent,
                    ctx.m_lineType == BlockType::EmptyLine, true, &ctx.m_indents);

            makeLineMain(ctx, line, emptyLinesCount, ctx.m_indent, ns, currentLineNumber);

            if (ctx.m_type == BlockType::Footnote) {
                wasEmptyLine = false;

                continue;
            } else {
                break;
            }
        }
    }

    if (stream.atEnd() && !ctx.m_fragment.empty()) {
        parseFragment(ctx, parent, doc, linksToParse, workingPath, fileName, collectRefLinks);
    }
}

template<class Trait>
inline void
Parser<Trait>::finishHtml(ParserContext &ctx,
                          std::shared_ptr<Block<Trait>> parent,
                          std::shared_ptr<Document<Trait>> doc,
                          bool collectRefLinks,
                          bool top,
                          bool dontProcessLastFreeHtml)
{
    if (!collectRefLinks || top) {
        if (ctx.m_html.m_html->isFreeTag()) {
            if (!dontProcessLastFreeHtml) {
                if (ctx.m_html.m_parent) {
                    ctx.m_html.m_parent->appendItem(ctx.m_html.m_html);

                    updateLastPosInList(ctx.m_html);
                } else {
                    parent->appendItem(ctx.m_html.m_html);
                }
            }
        } else {
            std::shared_ptr<Paragraph<Trait>> p(new Paragraph<Trait>);
            p->appendItem(ctx.m_html.m_html);
            p->setStartColumn(ctx.m_html.m_html->startColumn());
            p->setStartLine(ctx.m_html.m_html->startLine());
            p->setEndColumn(ctx.m_html.m_html->endColumn());
            p->setEndLine(ctx.m_html.m_html->endLine());
            doc->appendItem(p);
        }
    }

    if (!dontProcessLastFreeHtml) {
        resetHtmlTag(ctx.m_html);
    }

    ctx.m_html.m_toAdjustLastPos.clear();
}

template<class Trait>
inline void
Parser<Trait>::makeLineMain(ParserContext &ctx,
                            const typename Trait::InternalString &line,
                            long long int emptyLinesCount,
                            const ListIndent &currentIndent,
                            long long int ns,
                            long long int currentLineNumber)
{
    if (ctx.m_html.m_htmlBlockType >= 6) {
        ctx.m_html.m_continueHtml = (emptyLinesCount <= 0);
    }

    ctx.m_type = ctx.m_lineType;

    switch (ctx.m_type) {
    case BlockType::List:
    case BlockType::ListWithFirstEmptyLine: {
        if (ctx.m_indents.empty())
            ctx.m_indents.push_back(currentIndent.m_indent);

        ctx.m_indent = currentIndent;
    } break;

    case BlockType::Code:
        ctx.m_startOfCode = startSequence<Trait>(line.asString());
        break;

    default:
        break;
    }

    if (!line.isEmpty() && ns < line.length()) {
        ctx.m_fragment.push_back({line, {currentLineNumber, ctx.m_htmlCommentData}});
    }

    ctx.m_lineCounter = 1;
    ctx.m_emptyLinesCount = 0;
    ctx.m_emptyLinesBefore = emptyLinesCount;
}

template<class Trait>
inline std::pair<long long int, bool>
Parser<Trait>::parseFragmentAndMakeNextLineMain(ParserContext &ctx,
                                                std::shared_ptr<Block<Trait>> parent,
                                                std::shared_ptr<Document<Trait>> doc,
                                                typename Trait::StringList &linksToParse,
                                                const typename Trait::String &workingPath,
                                                const typename Trait::String &fileName,
                                                bool collectRefLinks,
                                                const typename Trait::InternalString &line,
                                                const ListIndent &currentIndent,
                                                long long int ns,
                                                long long int currentLineNumber)
{
    const auto empty = ctx.m_emptyLinesCount;

    const auto ret = parseFragment(ctx, parent, doc, linksToParse, workingPath,
                                   fileName, collectRefLinks);

    makeLineMain(ctx, line, empty, currentIndent, ns, currentLineNumber);

    return ret;
}

template<class Trait>
inline bool
Parser<Trait>::isListType(BlockType t)
{
    switch (t) {
    case BlockType::List:
    case BlockType::ListWithFirstEmptyLine:
        return true;

    default:
        return false;
    }
}

template<class Trait>
std::pair<typename Trait::InternalString, bool>
Parser<Trait>::readLine(typename Parser<Trait>::ParserContext &ctx,
                        StringListStream<Trait> &stream)
{
    ctx.m_htmlCommentData.clear();

    auto line = stream.readLine();

    static const char16_t c_zeroReplaceWith[2] = {0xFFFD, 0};

    line.first.replace(typename Trait::Char(0), Trait::utf16ToString(&c_zeroReplaceWith[0]));

    checkForHtmlComments(line.first, stream, ctx.m_htmlCommentData);

    return line;
}

template<class Trait>
inline std::pair<long long int, bool>
Parser<Trait>::parseFirstStep(ParserContext &ctx,
      StringListStream<Trait> &stream,
      std::shared_ptr<Block<Trait>> parent,
      std::shared_ptr<Document<Trait>> doc,
      typename Trait::StringList &linksToParse,
      const typename Trait::String &workingPath,
      const typename Trait::String &fileName,
      bool collectRefLinks)
{
    while (!stream.atEnd()) {
        const auto currentLineNumber = stream.currentLineNumber();

        typename Trait::InternalString line;
        bool mayBreak;

        std::tie(line, mayBreak) = readLine(ctx, stream);

        if (ctx.m_lineType != BlockType::Unknown) {
            ctx.m_prevLineType = ctx.m_lineType;
        }

        ctx.m_lineType = whatIsTheLine(line,
                                       (ctx.m_emptyLineInList || isListType(ctx.m_type)),
                                       ctx.m_prevLineType == BlockType::ListWithFirstEmptyLine,
                                       ctx.m_fensedCodeInList,
                                       &ctx.m_startOfCodeInList,
                                       &ctx.m_indent,
                                       ctx.m_lineType == BlockType::EmptyLine,
                                       true,
                                       &ctx.m_indents);

        if (isListType(ctx.m_type) && ctx.m_lineType == BlockType::FensedCodeInList) {
            ctx.m_fensedCodeInList = !ctx.m_fensedCodeInList;
        }

        const auto currentIndent = ctx.m_indent;

        const auto ns = skipSpaces<Trait>(0, line.asString());

        const auto indentInListValue = indentInList(&ctx.m_indents, ns, true);

        if (isListType(ctx.m_lineType) && !ctx.m_fensedCodeInList && ctx.m_indent.m_level > -1) {
            if (ctx.m_indent.m_level < (long long int)ctx.m_indents.size()) {
                ctx.m_indents.erase(ctx.m_indents.cbegin() + ctx.m_indent.m_level, ctx.m_indents.cend());
            }

            ctx.m_indents.push_back(ctx.m_indent.m_indent);
        }

        if (ctx.m_type == BlockType::CodeIndentedBySpaces && ns > 3) {
            ctx.m_lineType = BlockType::CodeIndentedBySpaces;
        }

        if (ctx.m_type == BlockType::ListWithFirstEmptyLine && ctx.m_lineCounter == 2 &&
            !isListType(ctx.m_lineType)) {
            if (ctx.m_emptyLinesCount > 0) {
                const auto l = parseFragmentAndMakeNextLineMain(ctx,
                                                 parent,
                                                 doc,
                                                 linksToParse,
                                                 workingPath,
                                                 fileName,
                                                 collectRefLinks,
                                                 line,
                                                 currentIndent,
                                                 ns,
                                                 currentLineNumber);

                if (l.first != -1) {
                    return l;
                }

                continue;
            } else {
                ctx.m_emptyLineInList = false;
                ctx.m_emptyLinesCount = 0;
            }
        }

        if (ctx.m_type == BlockType::ListWithFirstEmptyLine && ctx.m_lineCounter == 2) {
            ctx.m_type = BlockType::List;
        }

        // Footnote.
        if (ctx.m_lineType == BlockType::Footnote) {
            const auto l = parseFragmentAndMakeNextLineMain(ctx,
                                             parent,
                                             doc,
                                             linksToParse,
                                             workingPath,
                                             fileName,
                                             collectRefLinks,
                                             line,
                                             currentIndent,
                                             ns,
                                             currentLineNumber);

            if (l.first != -1) {
                return l;
            }

            eatFootnote(ctx, stream, parent, doc, linksToParse, workingPath, fileName, collectRefLinks);

            continue;
        }

        // First line of the fragment.
        if (ns != line.length() && ctx.m_type == BlockType::EmptyLine) {
            makeLineMain(ctx, line, ctx.m_emptyLinesCount, currentIndent, ns, currentLineNumber);

            continue;
        } else if (ns == line.length() && ctx.m_type == BlockType::EmptyLine) {
            ++ctx.m_emptyLinesCount;
            continue;
        }

        ++ctx.m_lineCounter;

        // Got new empty line.
        if (ns == line.length()) {
            ++ctx.m_emptyLinesCount;

            switch (ctx.m_type) {
            case BlockType::Blockquote: {
                const auto l = parseFragment(ctx, parent, doc, linksToParse, workingPath, fileName,
                                             collectRefLinks);

                if (l.first != -1) {
                    return l;
                }

                continue;
            }

            case BlockType::Text:
            case BlockType::CodeIndentedBySpaces:
                continue;
                break;

            case BlockType::Code: {
                ctx.m_fragment.push_back({line, {currentLineNumber, ctx.m_htmlCommentData, mayBreak}});
                ctx.m_emptyLinesCount = 0;

                continue;
            }

            case BlockType::List:
            case BlockType::ListWithFirstEmptyLine: {
                ctx.m_emptyLineInList = true;

                continue;
            }

            default:
                break;
            }
        }
        // Empty new line in list.
        else if (ctx.m_emptyLineInList) {
            if (indentInListValue || isListType(ctx.m_lineType) || ctx.m_lineType == BlockType::SomethingInList) {
                for (long long int i = 0; i < ctx.m_emptyLinesCount; ++i) {
                    ctx.m_fragment.push_back({typename Trait::String(),
                        {currentLineNumber - ctx.m_emptyLinesCount + i, {}, false}});
                }

                ctx.m_fragment.push_back({line, {currentLineNumber, ctx.m_htmlCommentData, mayBreak}});

                ctx.m_emptyLineInList = false;
                ctx.m_emptyLinesCount = 0;

                continue;
            } else {
                const auto empty = ctx.m_emptyLinesCount;

                const auto l = parseFragment(ctx, parent, doc, linksToParse, workingPath, fileName,
                                             collectRefLinks);

                if (l.first != -1) {
                    return l;
                }

                ctx.m_lineType = whatIsTheLine(line, false, false, false, nullptr, nullptr,
                    true, false, &ctx.m_indents);

                makeLineMain(ctx, line, empty, currentIndent, ns, currentLineNumber);

                continue;
            }
        } else if (ctx.m_emptyLinesCount > 0) {
            if (ctx.m_type == BlockType::CodeIndentedBySpaces &&
                ctx.m_lineType == BlockType::CodeIndentedBySpaces) {
                const auto indent = skipSpaces<Trait>(0, ctx.m_fragment.front().first.asString());

                for (long long int i = 0; i < ctx.m_emptyLinesCount; ++i) {
                    ctx.m_fragment.push_back({typename Trait::String(indent, Trait::latin1ToChar(' ')),
                        {currentLineNumber - ctx.m_emptyLinesCount + i, {}, false}});
                }

                ctx.m_fragment.push_back({line, {currentLineNumber, ctx.m_htmlCommentData, mayBreak}});
                ctx.m_emptyLinesCount = 0;
            } else {
                const auto l = parseFragmentAndMakeNextLineMain(ctx,
                                                 parent,
                                                 doc,
                                                 linksToParse,
                                                 workingPath,
                                                 fileName,
                                                 collectRefLinks,
                                                 line,
                                                 currentIndent,
                                                 ns,
                                                 currentLineNumber);

                if (l.first != -1) {
                    return l;
                }
            }

            continue;
        }

        // Something new and first block is not a code block or a list, blockquote.
        if (ctx.m_type != ctx.m_lineType && ctx.m_type != BlockType::Code &&
            !isListType(ctx.m_type) && ctx.m_type != BlockType::Blockquote) {
            if (ctx.m_type == BlockType::Text && ctx.m_lineType == BlockType::CodeIndentedBySpaces) {
                ctx.m_fragment.push_back({line, {currentLineNumber, ctx.m_htmlCommentData, mayBreak}});
            }
            else {
                if (ctx.m_type == BlockType::Text && isListType(ctx.m_lineType)) {
                    if (ctx.m_lineType != BlockType::ListWithFirstEmptyLine) {
                        int num = 0;

                        if (isOrderedList<Trait>(line.asString(), &num)) {
                            if (num != 1) {
                                ctx.m_fragment.push_back({line, {currentLineNumber, ctx.m_htmlCommentData, mayBreak}});

                                continue;
                            }
                        }
                    } else {
                        ctx.m_fragment.push_back({line, {currentLineNumber, ctx.m_htmlCommentData, mayBreak}});

                        continue;
                    }
                }

                const auto l = parseFragmentAndMakeNextLineMain(ctx,
                                                 parent,
                                                 doc,
                                                 linksToParse,
                                                 workingPath,
                                                 fileName,
                                                 collectRefLinks,
                                                 line,
                                                 currentIndent,
                                                 ns,
                                                 currentLineNumber);

                if (l.first != -1) {
                    return l;
                }
            }
        }
        // End of code block.
        else if (ctx.m_type == BlockType::Code && ctx.m_type == ctx.m_lineType &&
                 !ctx.m_startOfCode.isEmpty() &&
                 startSequence<Trait>(line.asString()).contains(ctx.m_startOfCode) &&
                 isCodeFences<Trait>(line.asString(), true)) {
            ctx.m_fragment.push_back({line, {currentLineNumber, ctx.m_htmlCommentData, mayBreak}});

            if (!stream.atEnd()) {
                typename Trait::InternalString line;

                std::tie(line, std::ignore) = readLine(ctx, stream);

                if (line.asString().simplified().isEmpty()) {
                    ++ctx.m_emptyLinesCount;
                }

                stream.setLineNumber(stream.currentLineNumber() - 1);
            } else {
                ++ctx.m_emptyLinesCount;
            }

            const auto l = parseFragment(ctx, parent, doc, linksToParse, workingPath, fileName,
                                         collectRefLinks);

            if (l.first != -1) {
                return l;
            }
        }
        // Not a continue of list.
        else if (ctx.m_type != ctx.m_lineType && isListType(ctx.m_type) &&
                 ctx.m_lineType != BlockType::SomethingInList &&
                 ctx.m_lineType != BlockType::FensedCodeInList && !isListType(ctx.m_lineType)) {
            const auto l = parseFragmentAndMakeNextLineMain(ctx,
                                             parent,
                                             doc,
                                             linksToParse,
                                             workingPath,
                                             fileName,
                                             collectRefLinks,
                                             line,
                                             currentIndent,
                                             ns,
                                             currentLineNumber);

            if (l.first != -1) {
                return l;
            }
        } else if (ctx.m_type == BlockType::Heading) {
            const auto l = parseFragmentAndMakeNextLineMain(ctx,
                                             parent,
                                             doc,
                                             linksToParse,
                                             workingPath,
                                             fileName,
                                             collectRefLinks,
                                             line,
                                             currentIndent,
                                             ns,
                                             currentLineNumber);

            if (l.first != -1) {
                return l;
            }
        } else {
            ctx.m_fragment.push_back({line, {currentLineNumber, ctx.m_htmlCommentData, mayBreak}});
        }

        ctx.m_emptyLinesCount = 0;
    }

    if (!ctx.m_fragment.empty()) {
        if (ctx.m_type == BlockType::Code && !ctx.m_html.m_html && !ctx.m_html.m_continueHtml) {
            ctx.m_fragment.push_back({ctx.m_startOfCode, {-1, {}, false}});
        }

        const auto l = parseFragment(ctx, parent, doc, linksToParse, workingPath, fileName,
                                     collectRefLinks);

        if (l.first != -1) {
            return l;
        }
    }

    return {-1, false};
}

template<class Trait>
inline void
Parser<Trait>::parseSecondStep(ParserContext &ctx,
      std::shared_ptr<Block<Trait>> parent,
      std::shared_ptr<Document<Trait>> doc,
      typename Trait::StringList &linksToParse,
      const typename Trait::String &workingPath,
      const typename Trait::String &fileName,
      bool collectRefLinks,
      bool top,
      bool dontProcessLastFreeHtml)
{
    if (top) {
        resetHtmlTag(ctx.m_html);

        for (long long int i = 0; i < (long long int)ctx.m_splitted.size(); ++i) {
            parseFragment(ctx.m_splitted[i], parent, doc, linksToParse, workingPath, fileName, false,
                                 ctx.m_html);

            if (ctx.m_html.m_htmlBlockType >= 6) {
                ctx.m_html.m_continueHtml = (!ctx.m_splitted[i].m_emptyLineAfter);
            }

            if (ctx.m_html.m_html && !ctx.m_html.m_continueHtml) {
                finishHtml(ctx, parent, doc, collectRefLinks, top, dontProcessLastFreeHtml);
            } else if (!ctx.m_html.m_html) {
                ctx.m_html.m_toAdjustLastPos.clear();
            }
        }
    }

    if (ctx.m_html.m_html) {
        finishHtml(ctx, parent, doc, collectRefLinks, top, dontProcessLastFreeHtml);
    }
}

template<class Trait>
inline std::pair<RawHtmlBlock<Trait>, long long int>
Parser<Trait>::parse(StringListStream<Trait> &stream,
                     std::shared_ptr<Block<Trait>> parent,
                     std::shared_ptr<Document<Trait>> doc,
                     typename Trait::StringList &linksToParse,
                     const typename Trait::String &workingPath,
                     const typename Trait::String &fileName,
                     bool collectRefLinks,
                     bool top,
                     bool dontProcessLastFreeHtml,
                     bool stopOnMayBreakList)
{
    ParserContext ctx;

    auto clearCtx = [&]()
    {
        ctx.m_fragment.clear();
        ctx.m_type = BlockType::EmptyLine;
        ctx.m_lineCounter = 0;
    };

    auto line = parseFirstStep(ctx, stream, parent, doc, linksToParse, workingPath, fileName,
                               collectRefLinks);

    clearCtx();

    while (line.first != -1 && !(stopOnMayBreakList && line.second)) {
        stream.setLineNumber(line.first);

        line = parseFirstStep(ctx, stream, parent, doc, linksToParse, workingPath, fileName,
                              collectRefLinks);

        clearCtx();
    }

    parseSecondStep(ctx, parent, doc, linksToParse, workingPath, fileName,
                    collectRefLinks, top, dontProcessLastFreeHtml);

    return {ctx.m_html, line.first};
}

#ifdef MD4QT_QT_SUPPORT

template<>
inline void
Parser<QStringTrait>::parseFile(const QString &fileName,
                                bool recursive,
                                std::shared_ptr<Document<QStringTrait>> doc,
                                const QStringList &ext,
                                QStringList *parentLinks,
                                QString workingDirectory)
{
    QFileInfo fi(fileName);

    if (fi.exists() && ext.contains(fi.suffix().toLower())) {
        QFile f(fileName);

        if (f.open(QIODevice::ReadOnly)) {
            QTextStream s(f.readAll());
            f.close();

            auto wd = fi.absolutePath();
            auto fn = fi.fileName();

            workingDirectory.replace(QLatin1Char('\\'), QLatin1Char('/'));

            if (!workingDirectory.isEmpty() && wd.contains(workingDirectory)) {
                QFileInfo folder(workingDirectory);

                if (folder.exists() && folder.isDir()) {
                    wd = folder.absoluteFilePath();

                    auto tmp = fi.absoluteFilePath();
                    fn = tmp.remove(wd);
                    fn.removeAt(0);
                }
            }

            parseStream(s, wd, fn, recursive, doc, ext, parentLinks, workingDirectory);
        }
    }
}

#endif

#ifdef MD4QT_ICU_STL_SUPPORT

template<>
inline void
Parser<UnicodeStringTrait>::parseFile(const UnicodeString &fileName,
                                      bool recursive,
                                      std::shared_ptr<Document<UnicodeStringTrait>> doc,
                                      const std::vector<UnicodeString> &ext,
                                      std::vector<UnicodeString> *parentLinks,
                                      UnicodeString workingDirectory)
{
    if (UnicodeStringTrait::fileExists(fileName)) {
        std::string fn;
        fileName.toUTF8String(fn);

        try {
            auto e = UnicodeString::fromUTF8(std::filesystem::u8path(fn).extension().u8string());

            if (!e.isEmpty()) {
                e.remove(0, 1);
            }

            if (std::find(ext.cbegin(), ext.cend(), e.toLower()) != ext.cend()) {
                auto path = std::filesystem::canonical(std::filesystem::u8path(fn));
                std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);

                if (file.good()) {
                    const auto fileNameS = path.filename().u8string();
                    auto wd = path.remove_filename().u8string();
                    auto ufn = UnicodeString::fromUTF8(fileNameS);

                    if (!wd.empty()) {
                        wd.erase(wd.size() - 1, 1);
                    }

                    std::replace(wd.begin(), wd.end(), '\\', '/');
                    workingDirectory.findAndReplace(UnicodeStringTrait::latin1ToString("\\"),
                                                    UnicodeStringTrait::latin1ToString("/"));

                    if (!workingDirectory.isEmpty() &&
                        UnicodeStringTrait::String(UnicodeString::fromUTF8(wd)).contains(workingDirectory)) {
                        std::string folderPath;
                        workingDirectory.toUTF8String(folderPath);
                        auto folder = std::filesystem::directory_entry(folderPath);

                        if (folder.exists() && folder.is_directory()) {
                            auto tmp = UnicodeString::fromUTF8(
                                        std::filesystem::canonical(std::filesystem::u8path(fn)).u8string());
                            path = std::filesystem::canonical(folder.path());
                            wd = path.u8string();

                            std::replace(wd.begin(), wd.end(), '\\', '/');
                            tmp.findAndReplace(UnicodeStringTrait::latin1ToString("\\"),
                                               UnicodeStringTrait::latin1ToString("/"));

                            ufn = tmp.findAndReplace(UnicodeString::fromUTF8(wd), {});
                            ufn.remove(0, 1);
                        }
                    }

                    parseStream(file, UnicodeString::fromUTF8(wd), ufn,
                                recursive, doc, ext, parentLinks, workingDirectory);

                    file.close();
                }
            }
        } catch (const std::exception &) {
        }
    }
}

#endif

//! Resolve links in the document.
template<class Trait>
void
resolveLinks(typename Trait::StringList &linksToParse,
             std::shared_ptr<Document<Trait>> doc)
{
    for (auto it = linksToParse.begin(), last = linksToParse.end(); it != last; ++it) {
        auto nextFileName = *it;

        if (nextFileName.startsWith(Trait::latin1ToString("#"))) {
            const auto lit = doc->labeledLinks().find(nextFileName);

            if (lit != doc->labeledLinks().cend()) {
                nextFileName = lit->second->url();
            } else {
                continue;
            }
        }

        if (Trait::fileExists(nextFileName)) {
            *it = Trait::absoluteFilePath(nextFileName);
        }
    }
}

template<class Trait>
inline void
Parser<Trait>::parseStream(typename Trait::TextStream &s,
                           const typename Trait::String &workingPath,
                           const typename Trait::String &fileName,
                           bool recursive,
                           std::shared_ptr<Document<Trait>> doc,
                           const typename Trait::StringList &ext,
                           typename Trait::StringList *parentLinks,
                           const typename Trait::String &workingDirectory)
{
    typename Trait::StringList linksToParse;

    const auto path = workingPath.isEmpty() ? typename Trait::String(fileName) :
        typename Trait::String(workingPath + Trait::latin1ToString("/") + fileName);

    doc->appendItem(std::shared_ptr<Anchor<Trait>>(new Anchor<Trait>(path)));

    typename MdBlock<Trait>::Data data;

    {
        TextStream<Trait> stream(s);

        long long int i = 0;

        while (!stream.atEnd()) {
            data.push_back(std::pair<typename Trait::InternalString, MdLineData>(stream.readLine(), {i}));
            ++i;
        }
    }

    StringListStream<Trait> stream(data);

    parse(stream, doc, doc, linksToParse, workingPath, fileName, true, true);

    m_parsedFiles.push_back(path);

    resolveLinks<Trait>(linksToParse, doc);

    // Parse all links if parsing is recursive.
    if (recursive && !linksToParse.empty()) {
        const auto tmpLinks = linksToParse;

        while (!linksToParse.empty()) {
            auto nextFileName = linksToParse.front();
            linksToParse.erase(linksToParse.cbegin());

            if (parentLinks) {
                const auto pit = std::find(parentLinks->cbegin(), parentLinks->cend(), nextFileName);

                if (pit != parentLinks->cend()) {
                    continue;
                }
            }

            if (nextFileName.startsWith(Trait::latin1ToString("#"))) {
                continue;
            }

            const auto pit = std::find(m_parsedFiles.cbegin(), m_parsedFiles.cend(), nextFileName);

            if (pit == m_parsedFiles.cend()) {
                if (!doc->isEmpty() && doc->items().back()->type() != ItemType::PageBreak) {
                    doc->appendItem(std::shared_ptr<PageBreak<Trait>>(new PageBreak<Trait>));
                }

                parseFile(nextFileName, recursive, doc, ext, &linksToParse, workingDirectory);
            }
        }

        if (parentLinks) {
            std::copy(tmpLinks.cbegin(), tmpLinks.cend(), std::back_inserter(*parentLinks));
        }
    }
}

//! \return Position of first character in list item.
template<class Trait>
inline long long int
posOfListItem(const typename Trait::String &s,
              bool ordered)
{
    long long int p = 0;

    for (; p < s.size(); ++p) {
        if (!s[p].isSpace()) {
            break;
        }
    }

    if (ordered) {
        for (; p < s.size(); ++p) {
            if (!s[p].isDigit()) {
                break;
            }
        }
    }

    ++p;

    long long int sc = 0;

    for (; p < s.size(); ++p) {
        if (!s[p].isSpace()) {
            break;
        } else {
            ++sc;
        }
    }

    if (p == s.length() || sc > 4) {
        p = p - sc + 1;
    } else if (sc == 0) {
        ++p;
    }

    return p;
}

//! \return Level in indents for the given position.
inline long long int
listLevel(const std::vector<long long int> &indents,
          long long int pos)
{
    long long int level = indents.size();

    for (auto it = indents.crbegin(), last = indents.crend(); it != last; ++it) {
        if (pos >= *it) {
            break;
        } else {
            --level;
        }
    }

    return level;
}

template<class Trait>
inline typename Parser<Trait>::BlockType
Parser<Trait>::whatIsTheLine(typename Trait::InternalString &str,
                             bool inList,
                             bool inListWithFirstEmptyLine,
                             bool fensedCodeInList,
                             typename Trait::String *startOfCode,
                             ListIndent *indent,
                             bool emptyLinePreceded,
                             bool calcIndent,
                             const std::vector<long long int> *indents)
{
    replaceTabs<Trait>(str);

    const auto first = skipSpaces<Trait>(0, str.asString());

    if (first < str.length()) {
        auto s = str.sliced(first);

        const bool isBlockquote = s.asString().startsWith(Trait::latin1ToString(">"));
        const bool indentIn = indentInList(indents, first, false);
        bool isHeading = false;

        if (first < 4 && isFootnote<Trait>(s.asString())) {
            return BlockType::Footnote;
        }

        if (s.asString().startsWith(Trait::latin1ToString("#")) &&
            (indent ? first - indent->m_indent < 4 : first < 4)) {
            long long int c = 0;

            while (c < s.length() && s[c] == Trait::latin1ToChar('#')) {
                ++c;
            }

            if (c <= 6 && ((c < s.length() && s[c].isSpace()) || c == s.length())) {
                isHeading = true;
            }
        }

        if (inList) {
            bool isFirstLineEmpty = false;
            const auto orderedList = isOrderedList<Trait>(str.asString(), nullptr, nullptr, nullptr,
                &isFirstLineEmpty);
            const bool fensedCode = isCodeFences<Trait>(s.asString());
            const auto codeIndentedBySpaces = emptyLinePreceded && first >= 4 &&
                !indentInList(indents, first, true);

            if (fensedCodeInList) {
                if (indentInList(indents, first, true)) {
                    if (fensedCode) {
                        if (startOfCode && startSequence<Trait>(s.asString()).contains(*startOfCode)) {
                            return BlockType::FensedCodeInList;
                        }
                    }

                    return BlockType::SomethingInList;
                }
            }

            if (fensedCode && indentIn) {
                if (startOfCode) {
                    *startOfCode = startSequence<Trait>(s.asString());
                }

                return BlockType::FensedCodeInList;
            } else if ((((s.asString().startsWith(Trait::latin1ToString("-")) ||
                          s.asString().startsWith(Trait::latin1ToString("+")) ||
                          s.asString().startsWith(Trait::latin1ToString("*"))) &&
                         ((s.length() > 1 && s[1] == Trait::latin1ToChar(' ')) || s.length() == 1)) ||
                         orderedList) && (first < 4 || indentIn)) {
                if (codeIndentedBySpaces) {
                    return BlockType::CodeIndentedBySpaces;
                }

                if (indent && calcIndent) {
                    indent->m_indent = posOfListItem<Trait>(str.asString(), orderedList);
                    indent->m_level = (indents ? listLevel(*indents, first) : -1);
                }

                if (s.simplified().length() == 1 || isFirstLineEmpty) {
                    return BlockType::ListWithFirstEmptyLine;
                } else {
                    return BlockType::List;
                }
            } else if (indentInList(indents, first, true)) {
                return BlockType::SomethingInList;
            }
            else {
                if (!isHeading && !isBlockquote &&
                    !(fensedCode && first < 4) && !emptyLinePreceded && !inListWithFirstEmptyLine) {
                    return BlockType::SomethingInList;
                }
            }
        } else {
            bool isFirstLineEmpty = false;

            const auto orderedList = isOrderedList<Trait>(str.asString(), nullptr, nullptr, nullptr,
                &isFirstLineEmpty);
            const bool isHLine = first < 4 && isHorizontalLine<Trait>(s.asString());

            if (!isHLine &&
                (((s.asString().startsWith(Trait::latin1ToString("-")) || s.asString().startsWith(Trait::latin1ToString("+")) ||
                      s.asString().startsWith(Trait::latin1ToString("*"))) &&
                     ((s.length() > 1 && s[1] == Trait::latin1ToChar(' ')) || s.length() == 1)) ||
                    orderedList) && first < 4) {
                if (indent && calcIndent) {
                    indent->m_indent = posOfListItem<Trait>(str.asString(), orderedList);
                    indent->m_level = (indents ? listLevel(*indents, first) : -1);
                }

                if (s.simplified().length() == 1 || isFirstLineEmpty) {
                    return BlockType::ListWithFirstEmptyLine;
                } else {
                    return BlockType::List;
                }
            }
        }

        if (str.asString().startsWith(typename Trait::String(4, Trait::latin1ToChar(' ')))) {
            return BlockType::CodeIndentedBySpaces;
        } else if (isCodeFences<Trait>(str.asString())) {
            return BlockType::Code;
        } else if (isBlockquote) {
            return BlockType::Blockquote;
        } else if (isHeading) {
            return BlockType::Heading;
        }
    } else {
        return BlockType::EmptyLine;
    }

    return BlockType::Text;
}

template<class Trait>
inline long long int
Parser<Trait>::parseFragment(MdBlock<Trait> &fr,
                             std::shared_ptr<Block<Trait>> parent,
                             std::shared_ptr<Document<Trait>> doc,
                             typename Trait::StringList &linksToParse,
                             const typename Trait::String &workingPath,
                             const typename Trait::String &fileName,
                             bool collectRefLinks,
                             RawHtmlBlock<Trait> &html)
{
    if (html.m_continueHtml) {
        return parseText(fr, parent, doc, linksToParse, workingPath, fileName, collectRefLinks, html);
    } else {
        if (html.m_html) {
            if (!collectRefLinks) {
                parent->appendItem(html.m_html);
            }

            resetHtmlTag(html);
        }

        switch (whatIsTheLine(fr.m_data.front().first)) {
        case BlockType::Footnote:
            parseFootnote(fr, parent, doc, linksToParse, workingPath, fileName, collectRefLinks);
            break;

        case BlockType::Text:
            return parseText(fr, parent, doc, linksToParse, workingPath, fileName, collectRefLinks, html);
            break;

        case BlockType::Blockquote:
            return parseBlockquote(fr, parent, doc, linksToParse, workingPath, fileName, collectRefLinks, html);
            break;

        case BlockType::Code:
            return parseCode(fr, parent, collectRefLinks);
            break;

        case BlockType::CodeIndentedBySpaces: {
            int indent = 1;

            if (fr.m_data.front().first.asString().startsWith(Trait::latin1ToString("    "))) {
                indent = 4;
            }

            return parseCodeIndentedBySpaces(fr, parent, collectRefLinks, indent, {}, -1, -1, false);
        } break;

        case BlockType::Heading:
            parseHeading(fr, parent, doc, linksToParse, workingPath, fileName, collectRefLinks);
            break;

        case BlockType::List:
        case BlockType::ListWithFirstEmptyLine:
            return parseList(fr, parent, doc, linksToParse, workingPath, fileName, collectRefLinks, html);

        default:
            break;
        }
    }

    return -1;
}

template<class Trait>
inline void
Parser<Trait>::clearCache()
{
    m_parsedFiles.clear();
}

//! \return Number of columns in table, if the given string is a table header.
template<class Trait>
inline int
isTableHeader(const typename Trait::String &s)
{
    if (s.contains(Trait::latin1ToChar('|'))) {
        int c = 0;

        const auto tmp = s.simplified();
        const auto p = tmp.startsWith(Trait::latin1ToString("|")) ? 1 : 0;
        const auto n = tmp.size() - p - (tmp.endsWith(Trait::latin1ToString("|")) && tmp.size() > 1 ? 1 : 0);
        const auto v = tmp.sliced(p, n);

        bool backslash = false;

        for (long long int i = 0; i < v.size(); ++i) {
            bool now = false;

            if (v[i] == Trait::latin1ToChar('\\') && !backslash) {
                backslash = true;
                now = true;
            } else if (v[i] == Trait::latin1ToChar('|') && !backslash) {
                ++c;
            }

            if (!now) {
                backslash = false;
            }
        }

        ++c;

        return c;
    } else {
        return 0;
    }
}

template<class Trait>
inline long long int
Parser<Trait>::parseText(MdBlock<Trait> &fr,
                         std::shared_ptr<Block<Trait>> parent,
                         std::shared_ptr<Document<Trait>> doc,
                         typename Trait::StringList &linksToParse,
                         const typename Trait::String &workingPath,
                         const typename Trait::String &fileName,
                         bool collectRefLinks,
                         RawHtmlBlock<Trait> &html)
{
    const auto h = isTableHeader<Trait>(fr.m_data.front().first.asString());
    const auto c = fr.m_data.size() > 1 ? isTableAlignment<Trait>(fr.m_data[1].first.asString()) : 0;

    if (c && h && c == h && !html.m_continueHtml) {
        parseTable(fr, parent, doc, linksToParse, workingPath, fileName, collectRefLinks, c);

        if (!fr.m_data.empty()) {
            return fr.m_data.front().second.m_lineNumber;
        } else {
            return -1;
        }
    } else {
        return parseParagraph(fr, parent, doc, linksToParse, workingPath, fileName, collectRefLinks, html);
    }
}

//! Find and remove heading label.
template<class Trait>
inline std::pair<typename Trait::String, WithPosition>
findAndRemoveHeaderLabel(typename Trait::InternalString &s)
{
    const auto start = s.asString().indexOf(Trait::latin1ToString("{#"));

    if (start >= 0) {
        long long int p = start + 2;

        for (; p < s.length(); ++p) {
            if (s[p] == Trait::latin1ToChar('}')) {
                break;
            }
        }

        if (p < s.length() && s[p] == Trait::latin1ToChar('}')) {
            WithPosition pos;
            pos.setStartColumn(s.virginPos(start));
            pos.setEndColumn(s.virginPos(p));

            const auto label = s.sliced(start, p - start + 1).asString();
            s.remove(start, p - start + 1);
            return {label, pos};
        }
    }

    return {};
}

//! Convert string to label.
template<class Trait>
inline typename Trait::String
stringToLabel(const typename Trait::String &s)
{
    typename Trait::String res;

    for (long long int i = 0; i < s.length(); ++i) {
        const auto c = s[i];

        if (c.isLetter() || c.isDigit() || c == Trait::latin1ToChar('-') ||
            c == Trait::latin1ToChar('_')) {
            res.push_back(c);
        } else if (c.isSpace()) {
            res.push_back(Trait::latin1ToString("-"));
        }
    }

    return res;
}

//! Convert Paragraph to label.
template<class Trait>
inline typename Trait::String
paragraphToLabel(Paragraph<Trait> *p)
{
    typename Trait::String l;

    if (!p) {
        return l;
    }

    for (auto it = p->items().cbegin(), last = p->items().cend(); it != last; ++it) {
        switch ((*it)->type()) {
        case ItemType::Text: {
            auto t = static_cast<Text<Trait> *>(it->get());
            const auto text = t->text();
            l.push_back(stringToLabel<Trait>(text));
        } break;

        case ItemType::Image: {
            auto i = static_cast<Image<Trait> *>(it->get());

            if (!i->p()->isEmpty()) {
                l.push_back(paragraphToLabel(i->p().get()));
            } else if (!i->text().isEmpty()) {
                l.push_back(stringToLabel<Trait>(i->text()));
            }
        } break;

        case ItemType::Link: {
            auto link = static_cast<Link<Trait> *>(it->get());

            if (!link->p()->isEmpty()) {
                l.push_back(paragraphToLabel(link->p().get()));
            } else if (!link->text().isEmpty()) {
                l.push_back(stringToLabel<Trait>(link->text()));
            }
        } break;

        case ItemType::Code: {
            auto c = static_cast<Code<Trait> *>(it->get());

            if (!c->text().isEmpty()) {
                l.push_back(stringToLabel<Trait>(c->text()));
            }
        } break;

        default:
            break;
        }
    }

    return l;
}

//! Find and remove closing sequence of "#" in heading.
template<class Trait>
inline WithPosition
findAndRemoveClosingSequence(typename Trait::InternalString &s)
{
    long long int end = -1;
    long long int start = -1;

    for (long long int i = s.length() - 1; i >= 0; --i) {
        if (!s[i].isSpace() && s[i] != Trait::latin1ToChar('#') && end == -1) {
            return {};
        }

        if (s[i] == Trait::latin1ToChar('#')) {
            if (end == -1) {
                end = i;
            }

            if (i - 1 >= 0) {
                if (s[i - 1].isSpace()) {
                    start = i;
                    break;
                } else if (s[i - 1] != Trait::latin1ToChar('#')) {
                    return {};
                }
            } else {
                start = 0;
            }
        }
    }

    WithPosition ret;

    if (start != -1 && end != -1) {
        ret.setStartColumn(s.virginPos(start));
        ret.setEndColumn(s.virginPos(end));

        s.remove(start, end - start + 1);
    }

    return ret;
}

template<class Trait>
inline void
Parser<Trait>::parseHeading(MdBlock<Trait> &fr,
                            std::shared_ptr<Block<Trait>> parent,
                            std::shared_ptr<Document<Trait>> doc,
                            typename Trait::StringList &linksToParse,
                            const typename Trait::String &workingPath,
                            const typename Trait::String &fileName,
                            bool collectRefLinks)
{
    if (!fr.m_data.empty() && !collectRefLinks) {
        auto line = fr.m_data.front().first;

        std::shared_ptr<Heading<Trait>> h(new Heading<Trait>);
        h->setStartColumn(line.virginPos(skipSpaces<Trait>(0, line.asString())));
        h->setStartLine(fr.m_data.front().second.m_lineNumber);
        h->setEndColumn(line.virginPos(line.length() - 1));
        h->setEndLine(h->startLine());

        long long int pos = 0;
        pos = skipSpaces<Trait>(pos, line.asString());

        if (pos > 0) {
            line = line.sliced(pos);
        }

        pos = 0;
        int lvl = 0;

        while (pos < line.length() && line[pos] == Trait::latin1ToChar('#')) {
            ++lvl;
            ++pos;
        }

        WithPosition startDelim = {h->startColumn(), h->startLine(),
            line.virginPos(pos - 1), h->startLine()};

        pos = skipSpaces<Trait>(pos, line.asString());

        if (pos > 0) {
            fr.m_data.front().first = line.sliced(pos);
        }

        auto label = findAndRemoveHeaderLabel<Trait>(fr.m_data.front().first);

        typename Heading<Trait>::Delims delims = {startDelim};

        auto endDelim = findAndRemoveClosingSequence<Trait>(fr.m_data.front().first);

        if (endDelim.startColumn() != -1) {
            endDelim.setStartLine(fr.m_data.front().second.m_lineNumber);
            endDelim.setEndLine(endDelim.startLine());

            delims.push_back(endDelim);
        }

        h->setDelims(delims);

        h->setLevel(lvl);

        if (!label.first.isEmpty()) {
            h->setLabel(label.first.sliced(1, label.first.length() - 2) + Trait::latin1ToString("/") +
                        (!workingPath.isEmpty() ? workingPath + Trait::latin1ToString("/") :
                            Trait::latin1ToString("")) + fileName);

            label.second.setStartLine(fr.m_data.front().second.m_lineNumber);
            label.second.setEndLine(label.second.startLine());

            h->setLabelPos(label.second);
        }

        std::shared_ptr<Paragraph<Trait>> p(new Paragraph<Trait>);

        typename MdBlock<Trait>::Data tmp;
        removeSpacesAtEnd<typename Trait::InternalString>(fr.m_data.front().first);
        tmp.push_back(fr.m_data.front());
        MdBlock<Trait> block = {tmp, 0};

        RawHtmlBlock<Trait> html;

        parseFormattedTextLinksImages(block, p, doc, linksToParse, workingPath, fileName,
            false, false, html, false);

        fr.m_data.erase(fr.m_data.cbegin());

        if (p->items().size() && p->items().at(0)->type() == ItemType::Paragraph) {
            h->setText(std::static_pointer_cast<Paragraph<Trait>>(p->items().at(0)));
        } else {
            h->setText(p);
        }

        if (h->isLabeled()) {
            doc->insertLabeledHeading(h->label(), h);
            h->labelVariants().push_back(h->label());
        } else {
            typename Trait::String label = Trait::latin1ToString("#") +
                paragraphToLabel(h->text().get());

            const auto path = Trait::latin1ToString("/") +
                (!workingPath.isEmpty() ? workingPath + Trait::latin1ToString("/") :
                    Trait::latin1ToString("")) + fileName;

            h->setLabel(label + path);
            h->labelVariants().push_back(h->label());

            doc->insertLabeledHeading(label + path, h);

            if (label != label.toLower()) {
                doc->insertLabeledHeading(label.toLower() + path, h);
                h->labelVariants().push_back(label.toLower() + path);
            }
        }

        parent->appendItem(h);
    }
}

//! Prepare data in table cell for parsing.
template<class Trait>
inline typename Trait::InternalString
prepareTableData(typename Trait::InternalString s)
{
    s.replace(Trait::latin1ToString("\\|"), Trait::latin1ToString("|"));

    return s;
}

//! Split table's row on cells.
template<class Trait>
inline std::pair<typename Trait::InternalStringList, std::vector<long long int>>
splitTableRow(const typename Trait::InternalString &s)
{
    typename Trait::InternalStringList res;
    std::vector<long long int> columns;

    bool backslash = false;
    long long int start = 0;

    for (long long int i = 0; i < s.length(); ++i) {
        bool now = false;

        if (s[i] == Trait::latin1ToChar('\\') && !backslash) {
            backslash = true;
            now = true;
        } else if (s[i] == Trait::latin1ToChar('|') && !backslash) {
            res.push_back(prepareTableData<Trait>(s.sliced(start, i - start)));
            columns.push_back(s.virginPos(i));
            start = i + 1;
        }

        if (!now) {
            backslash = false;
        }
    }

    res.push_back(prepareTableData<Trait>(s.sliced(start, s.length() - start)));

    return {res, columns};
}

template<class Trait>
inline void
Parser<Trait>::parseTable(MdBlock<Trait> &fr,
                          std::shared_ptr<Block<Trait>> parent,
                          std::shared_ptr<Document<Trait>> doc,
                          typename Trait::StringList &linksToParse,
                          const typename Trait::String &workingPath,
                          const typename Trait::String &fileName,
                          bool collectRefLinks,
                          int columnsCount)
{
    static const char sep = '|';

    if (fr.m_data.size() >= 2) {
        std::shared_ptr<Table<Trait>> table(new Table<Trait>);
        table->setStartColumn(fr.m_data.front().first.virginPos(0));
        table->setStartLine(fr.m_data.front().second.m_lineNumber);
        table->setEndColumn(fr.m_data.back().first.virginPos(fr.m_data.back().first.length() - 1));
        table->setEndLine(fr.m_data.back().second.m_lineNumber);

        auto parseTableRow = [&](const typename MdBlock<Trait>::Line &lineData) -> bool {
            const auto &row = lineData.first;

            if (row.asString().startsWith(Trait::latin1ToString("    "))) {
                return false;
            }

            auto line = row;
            auto p = skipSpaces<Trait>(0, line.asString());

            if (p == line.length()) {
                return false;
            }

            if (line[p] == Trait::latin1ToChar(sep)) {
                line.remove(0, p + 1);
            }

            for (p = line.length() - 1; p >= 0; --p) {
                if (!line[p].isSpace()) {
                    break;
                }
            }

            if (p < 0) {
                return false;
            }

            if (line[p] == Trait::latin1ToChar(sep)) {
                line.remove(p, line.length() - p);
            }

            auto columns = splitTableRow<Trait>(line);
            columns.second.insert(columns.second.begin(), row.virginPos(0));
            columns.second.push_back(row.virginPos(row.length() - 1));

            std::shared_ptr<TableRow<Trait>> tr(new TableRow<Trait>);
            tr->setStartColumn(row.virginPos(0));
            tr->setStartLine(lineData.second.m_lineNumber);
            tr->setEndColumn(row.virginPos(row.length() - 1));
            tr->setEndLine(lineData.second.m_lineNumber);

            int col = 0;

            for (auto it = columns.first.begin(), last = columns.first.end(); it != last; ++it, ++col) {
                if (col == columnsCount) {
                    break;
                }

                std::shared_ptr<TableCell<Trait>> c(new TableCell<Trait>);
                c->setStartColumn(columns.second.at(col));
                c->setStartLine(lineData.second.m_lineNumber);
                c->setEndColumn(columns.second.at(col + 1));
                c->setEndLine(lineData.second.m_lineNumber);

                if (!it->isEmpty()) {
                    it->replace(Trait::latin1ToString("&#124;"), Trait::latin1ToChar(sep));

                    typename MdBlock<Trait>::Data fragment;
                    fragment.push_back({*it, lineData.second});
                    MdBlock<Trait> block = {fragment, 0};

                    std::shared_ptr<Paragraph<Trait>> p(new Paragraph<Trait>);

                    RawHtmlBlock<Trait> html;

                    parseFormattedTextLinksImages(block, p, doc, linksToParse, workingPath, fileName,
                        collectRefLinks, false, html, false);

                    if (!p->isEmpty()) {
                        for (auto it = p->items().cbegin(), last = p->items().cend(); it != last; ++it ) {
                            switch ((*it)->type()) {
                            case ItemType::Paragraph: {
                                const auto pp = std::static_pointer_cast<Paragraph<Trait>>(*it);

                                for (auto it = pp->items().cbegin(), last = pp->items().cend(); it != last; ++it) {
                                    c->appendItem((*it));
                                }
                            }
                                break;

                            default:
                                c->appendItem((*it));
                                break;
                            }
                        }
                    }

                    if (html.m_html.get()) {
                        c->appendItem(html.m_html);
                    }
                }

                tr->appendCell(c);
            }

            if (!tr->isEmpty())
                table->appendRow(tr);

            return true;
        };

        {
            auto fmt = fr.m_data.at(1).first;

            auto columns = fmt.split(typename Trait::InternalString(Trait::latin1ToChar(sep)));

            for (auto it = columns.begin(), last = columns.end(); it != last; ++it) {
                *it = it->simplified();

                if (!it->isEmpty()) {
                    typename Table<Trait>::Alignment a = Table<Trait>::AlignLeft;

                    if (it->asString().endsWith(Trait::latin1ToString(":")) &&
                        it->asString().startsWith(Trait::latin1ToString(":"))) {
                        a = Table<Trait>::AlignCenter;
                    } else if (it->asString().endsWith(Trait::latin1ToString(":"))) {
                        a = Table<Trait>::AlignRight;
                    }

                    table->setColumnAlignment(table->columnsCount(), a);
                }
            }
        }

        fr.m_data.erase(fr.m_data.cbegin() + 1);

        long long int r = 0;

        for (const auto &line : std::as_const(fr.m_data)) {
            if (!parseTableRow(line)) {
                break;
            }

            ++r;
        }

        fr.m_data.erase(fr.m_data.cbegin(), fr.m_data.cbegin() + r);

        if (!table->isEmpty() && !collectRefLinks) {
            parent->appendItem(table);
        }
    }
}

//! \return Is the given string a heading's service sequence?
template<class Trait>
inline bool
isH(const typename Trait::String &s,
    const typename Trait::Char &c)
{
    long long int p = skipSpaces<Trait>(0, s);

    if (p > 3) {
        return false;
    }

    const auto start = p;

    for (; p < s.size(); ++p) {
        if (s[p] != c) {
            break;
        }
    }

    if (p - start < 1) {
        return false;
    }

    for (; p < s.size(); ++p) {
        if (!s[p].isSpace()) {
            return false;
        }
    }

    return true;
}

//! \return Is the given string a heading's service sequence of level 1?
template<class Trait>
inline bool
isH1(const typename Trait::String &s)
{
    return isH<Trait>(s, Trait::latin1ToChar('='));
}

//! \return Is the given string a heading's service sequence of level 2?
template<class Trait>
inline bool
isH2(const typename Trait::String &s)
{
    return isH<Trait>(s, Trait::latin1ToChar('-'));
}

//! \return Previous position in the block.
template<class Trait>
inline std::pair<long long int, long long int>
prevPosition(const MdBlock<Trait> &fr,
             long long int pos,
             long long int line)
{
    if (pos > 0) {
        return {pos - 1, line};
    }

    for (long long int i = 0; i < static_cast<long long int>(fr.m_data.size()); ++i) {
        if (fr.m_data.at(i).second.m_lineNumber == line) {
            if (i > 0) {
                return {fr.m_data.at(i - 1).first.virginPos(fr.m_data.at(i - 1).first.length() - 1),
                    line - 1};
            }
        }
    }

    return {pos, line};
}

//! \return Next position in the block.
template<class Trait>
inline std::pair<long long int, long long int>
nextPosition(const MdBlock<Trait> &fr,
             long long int pos,
             long long int line)
{
    for (long long int i = 0; i < static_cast<long long int>(fr.m_data.size()); ++i) {
        if (fr.m_data.at(i).second.m_lineNumber == line) {
            if (fr.m_data.at(i).first.virginPos(fr.m_data.at(i).first.length() - 1) >= pos + 1) {
                return {pos + 1, line};
            } else if (i + 1 < static_cast<long long int>(fr.m_data.size())) {
                return {fr.m_data.at(i + 1).first.virginPos(0), fr.m_data.at(i + 1).second.m_lineNumber};
            } else {
                return {pos, line};
            }
        }
    }

    return {pos, line};
}

template<class Trait>
inline long long int
Parser<Trait>::parseParagraph(MdBlock<Trait> &fr,
                              std::shared_ptr<Block<Trait>> parent,
                              std::shared_ptr<Document<Trait>> doc,
                              typename Trait::StringList &linksToParse,
                              const typename Trait::String &workingPath,
                              const typename Trait::String &fileName,
                              bool collectRefLinks,
                              RawHtmlBlock<Trait> &html)
{
    return parseFormattedTextLinksImages(fr, parent, doc, linksToParse, workingPath, fileName,
        collectRefLinks, false, html, false);
}

template<class Trait>
struct UnprotectedDocsMethods {
    static bool
    isFreeTag(std::shared_ptr<RawHtml<Trait>> html)
    {
        return html->isFreeTag();
    }

    static void
    setFreeTag(std::shared_ptr<RawHtml<Trait>> html, bool on)
    {
        html->setFreeTag(on);
    }
};

template<class Trait>
inline typename Parser<Trait>::Delims
Parser<Trait>::collectDelimiters(const typename MdBlock<Trait>::Data &fr)
{
    Delims d;

    for (long long int line = 0; line < (long long int)fr.size(); ++line) {
        const typename Trait::String &str = fr.at(line).first.asString();
        const auto p = skipSpaces<Trait>(0, str);
        const auto withoutSpaces = str.sliced(p);

        if (isHorizontalLine<Trait>(withoutSpaces) && p < 4) {
            d.push_back({Delimiter::HorizontalLine, line, 0, str.length(), false, false, false});
        } else if (isH1<Trait>(withoutSpaces) && p < 4) {
            d.push_back({Delimiter::H1, line, 0, str.length(), false, false, false});
        } else if (isH2<Trait>(withoutSpaces) && p < 4) {
            d.push_back({Delimiter::H2, line, 0, str.length(), false, false, false});
        } else {
            bool backslash = false;
            bool word = false;

            for (long long int i = p; i < str.size(); ++i) {
                bool now = false;

                if (str[i] == Trait::latin1ToChar('\\') && !backslash) {
                    backslash = true;
                    now = true;
                }
                // * or _
                else if ((str[i] == Trait::latin1ToChar('_') || str[i] == Trait::latin1ToChar('*')) && !backslash) {
                    typename Trait::String style;

                    const bool punctBefore = (i > 0 ? str[i - 1].isPunct() || str[i - 1].isSymbol() : true);
                    const bool uWhitespaceBefore = (i > 0 ? Trait::isUnicodeWhitespace(str[i - 1]) : true);
                    const bool uWhitespaceOrPunctBefore = uWhitespaceBefore || punctBefore;
                    const bool alNumBefore = (i > 0 ? str[i - 1].isLetterOrNumber() : false);

                    const auto ch = str[i];

                    while (i < str.length() && str[i] == ch) {
                        style.push_back(str[i]);
                        ++i;
                    }

                    typename Delimiter::DelimiterType dt = Delimiter::Unknown;

                    if (ch == Trait::latin1ToChar('*')) {
                        dt = Delimiter::Emphasis1;
                    } else {
                        dt = Delimiter::Emphasis2;
                    }

                    const bool punctAfter = (i < str.length() ? str[i].isPunct() || str[i].isSymbol() : true);
                    const bool uWhitespaceAfter = (i < str.length() ? Trait::isUnicodeWhitespace(str[i]) : true);
                    const bool alNumAfter = (i < str.length() ? str[i].isLetterOrNumber() : false);
                    const bool leftFlanking = !uWhitespaceAfter && (!punctAfter || (punctAfter && uWhitespaceOrPunctBefore))
                        && !(ch == Trait::latin1ToChar('_') && alNumBefore && alNumAfter);
                    const bool rightFlanking = !uWhitespaceBefore && (!punctBefore || (punctBefore && (uWhitespaceAfter || punctAfter)))
                        && !(ch == Trait::latin1ToChar('_') && alNumBefore && alNumAfter);

                    if (leftFlanking || rightFlanking) {
                        for (auto j = 0; j < style.length(); ++j) {
                            d.push_back({dt, line, i - style.length() + j, 1,
                                word, false, leftFlanking, rightFlanking});
                        }

                        word = false;
                    } else {
                        word = true;
                    }

                    --i;
                }
                // ~
                else if (str[i] == Trait::latin1ToChar('~') && !backslash) {
                    typename Trait::String style;

                    const bool punctBefore = (i > 0 ? str[i - 1].isPunct() || str[i - 1].isSymbol() : true);
                    const bool uWhitespaceBefore = (i > 0 ? Trait::isUnicodeWhitespace(str[i - 1]) : true);
                    const bool uWhitespaceOrPunctBefore = uWhitespaceBefore || punctBefore;

                    while (i < str.length() && str[i] == Trait::latin1ToChar('~')) {
                        style.push_back(str[i]);
                        ++i;
                    }

                    if (style.length() <= 2) {
                        const bool punctAfter = (i < str.length() ? str[i].isPunct() || str[i].isSymbol() : true);
                        const bool uWhitespaceAfter = (i < str.length() ? Trait::isUnicodeWhitespace(str[i]) : true);
                        const bool leftFlanking = !uWhitespaceAfter && (!punctAfter || (punctAfter && uWhitespaceOrPunctBefore));
                        const bool rightFlanking = !uWhitespaceBefore && (!punctBefore || (punctBefore && (uWhitespaceAfter || punctAfter)));

                        if (leftFlanking || rightFlanking) {
                            d.push_back({Delimiter::Strikethrough,
                                         line,
                                         i - style.length(),
                                         style.length(),
                                         word,
                                         false,
                                         leftFlanking,
                                         rightFlanking});

                            word = false;
                        } else {
                            word = true;
                        }
                    } else {
                        word = true;
                    }

                    --i;
                }
                // [
                else if (str[i] == Trait::latin1ToChar('[') && !backslash) {
                    d.push_back({Delimiter::SquareBracketsOpen, line, i, 1, word, false});

                    word = false;
                }
                // !
                else if (str[i] == Trait::latin1ToChar('!') && !backslash) {
                    if (i + 1 < str.length()) {
                        if (str[i + 1] == Trait::latin1ToChar('[')) {
                            d.push_back({Delimiter::ImageOpen, line, i, 2, word, false});

                            ++i;

                            word = false;
                        } else {
                            word = true;
                        }
                    } else {
                        word = true;
                    }
                }
                // (
                else if (str[i] == Trait::latin1ToChar('(') && !backslash) {
                    d.push_back({Delimiter::ParenthesesOpen, line, i, 1, word, false});

                    word = false;
                }
                // ]
                else if (str[i] == Trait::latin1ToChar(']') && !backslash) {
                    d.push_back({Delimiter::SquareBracketsClose, line, i, 1, word, false});

                    word = false;
                }
                // )
                else if (str[i] == Trait::latin1ToChar(')') && !backslash) {
                    d.push_back({Delimiter::ParenthesesClose, line, i, 1, word, false});

                    word = false;
                }
                // <
                else if (str[i] == Trait::latin1ToChar('<') && !backslash) {
                    d.push_back({Delimiter::Less, line, i, 1, word, false});

                    word = false;
                }
                // >
                else if (str[i] == Trait::latin1ToChar('>') && !backslash) {
                    d.push_back({Delimiter::Greater, line, i, 1, word, false});

                    word = false;
                }
                // `
                else if (str[i] == Trait::latin1ToChar('`')) {
                    typename Trait::String code;

                    while (i < str.length() && str[i] == Trait::latin1ToChar('`')) {
                        code.push_back(str[i]);
                        ++i;
                    }

                    d.push_back({Delimiter::InlineCode,
                                 line,
                                 i - code.length() - (backslash ? 1 : 0),
                                 code.length() + (backslash ? 1 : 0),
                                 word,
                                 backslash});

                    word = false;

                    --i;
                }
                // $
                else if (str[i] == Trait::latin1ToChar('$')) {
                    typename Trait::String m;

                    while (i < str.length() && str[i] == Trait::latin1ToChar('$')) {
                        m.push_back(str[i]);
                        ++i;
                    }

                    if (m.length() <= 2 && !backslash) {
                        d.push_back({Delimiter::Math, line, i - m.length(), m.length(),
                            false, false, false, false});
                    }

                    word = false;

                    --i;
                } else {
                    word = true;
                }

                if (!now) {
                    backslash = false;
                }
            }
        }
    }

    return d;
}

//! \return Is the given string a line break.
template<class Trait>
inline bool
isLineBreak(const typename Trait::String &s)
{
    long long int count = 0, pos = s.length() - 1, end = s.length() - 1;

    while ((pos = Trait::lastIndexOf(s, Trait::latin1ToString("\\"), pos)) != -1 && pos == end) {
        --end;
        --pos;
        ++count;
    }

    return (s.endsWith(Trait::latin1ToString("  ")) || (count % 2 != 0));
}

//! \return Length of line break.
template<class Trait>
inline long long int
lineBreakLength(const typename Trait::String &s)
{
    return (s.endsWith(Trait::latin1ToString("  ")) ? 2 : 1);
}

//! Remove line break from the end of string.
template<class Trait>
inline typename Trait::String
removeLineBreak(const typename Trait::String &s)
{
    if (s.endsWith(Trait::latin1ToString("\\"))) {
        return s.sliced(0, s.size() - 1);
    } else {
        return s;
    }
}

//! Initialize item with style information and set it as last item.
template<class Trait>
inline void
initLastItemWithOpts(TextParsingOpts<Trait> &po,
                     std::shared_ptr<ItemWithOpts<Trait>> item)
{
    item->openStyles() = po.m_openStyles;
    po.m_openStyles.clear();
    po.m_lastItemWithStyle = item;
}

//! Make text item.
template<class Trait>
inline void
makeTextObject(const typename Trait::String &text,
               TextParsingOpts<Trait> &po,
               long long int startPos,
               long long int startLine,
               long long int endPos,
               long long int endLine,
               bool doRemoveSpacesAtEnd = false)
{
    if (endPos < 0 && endLine - 1 >= 0) {
        endPos = po.m_fr.m_data.at(endLine - 1).first.length() - 1;
        --endLine;
    }

    if (endPos == po.m_fr.m_data.at(endLine).first.length() - 1) {
        doRemoveSpacesAtEnd = true;
    }

    auto s = removeBackslashes<typename Trait::String, Trait>(replaceEntity<Trait>(text));

    if (doRemoveSpacesAtEnd) {
        removeSpacesAtEnd<typename Trait::String>(s);
    }

    if (startPos == 0) {
        if (s.length()) {
            const auto p = skipSpaces<Trait>(0, s);

            if (p > 0) {
                s.remove(0, p);
            }
        }
    }

    if (!s.isEmpty()) {
        std::shared_ptr<Text<Trait>> t;

        if (!po.m_collectRefLinks) {
            po.m_rawTextData.push_back({text, startPos, startLine});

            t.reset(new Text<Trait>);
            t->setText(s);
            t->setOpts(po.m_opts);
            t->setStartColumn(po.m_fr.m_data.at(startLine).first.virginPos(startPos));
            t->setStartLine(po.m_fr.m_data.at(startLine).second.m_lineNumber);
            t->setEndColumn(po.m_fr.m_data.at(endLine).first.virginPos(endPos, true));
            t->setEndLine(po.m_fr.m_data.at(endLine).second.m_lineNumber);

            initLastItemWithOpts<Trait>(po, t);

            po.m_parent->setEndColumn(t->endColumn());
            po.m_parent->setEndLine(t->endLine());
        }

        po.m_wasRefLink = false;
        po.m_firstInParagraph = false;
        po.m_headingAllowed = true;

        if (!po.m_collectRefLinks) {
            po.m_parent->appendItem(t);

            po.m_lastText = t;
        }
    } else {
        po.m_pos = startPos;
    }
}

//! Make text item with line break.
template<class Trait>
inline void
makeTextObjectWithLineBreak(const typename Trait::String &text,
                            TextParsingOpts<Trait> &po,
                            long long int startPos,
                            long long int startLine,
                            long long int endPos,
                            long long int endLine)
{
    makeTextObject(text, po, startPos, startLine, endPos, endLine, true);

    std::shared_ptr<LineBreak<Trait>> hr;

    if (!po.m_collectRefLinks) {
        hr.reset(new LineBreak<Trait>);
        hr->setText(po.m_fr.m_data.at(endLine).first.asString().sliced(endPos + 1));
        hr->setStartColumn(po.m_fr.m_data.at(endLine).first.virginPos(endPos + 1));
        hr->setStartLine(po.m_fr.m_data.at(endLine).second.m_lineNumber);
        hr->setEndColumn(po.m_fr.m_data.at(endLine).first.virginPos(po.m_fr.m_data.at(endLine).first.length() - 1));
        hr->setEndLine(po.m_fr.m_data.at(endLine).second.m_lineNumber);
        po.m_parent->setEndColumn(hr->endColumn());
        po.m_parent->setEndLine(hr->endLine());
    }

    po.m_wasRefLink = false;
    po.m_firstInParagraph = false;

    if (!po.m_collectRefLinks) {
        po.m_parent->appendItem(hr);
    }
}

//! Check for table in paragraph.
template<class Trait>
inline void
checkForTableInParagraph(TextParsingOpts<Trait> &po,
                         long long int lastLine)
{
    if (!po.m_opts) {
        long long int i = po.m_pos > 0 ? po.m_line + 1 : po.m_line;

        for (; i <= lastLine; ++i) {
            const auto h = isTableHeader<Trait>(po.m_fr.m_data[i].first.asString());
            const auto c = i + 1 < static_cast<long long int>(po.m_fr.m_data.size()) ?
                                    isTableAlignment<Trait>(po.m_fr.m_data[i + 1].first.asString()) : 0;

            if (h && c && c == h) {
                po.m_detected = TextParsingOpts<Trait>::Detected::Table;
                po.m_startTableLine = i;
                po.m_columnsCount = c;
                po.m_lastTextLine = i - 1;
                po.m_lastTextPos = po.m_fr.m_data[po.m_lastTextLine].first.length();

                return;
            }
        }
    }

    po.m_lastTextLine = po.m_fr.m_data.size() - 1;
    po.m_lastTextPos = po.m_fr.m_data.back().first.length();
}

//! Make text item.
template<class Trait>
inline void
makeText(
    // Inclusive. Don't pass lastLine > actual line position with 0 lastPos. Pass as is,
    // i.e. if line length is 18 and you need whole line then pass lastLine = index of line,
    //	and lastPos = 18, or you may crash here if you will pass lastLine = index of line + 1
    // and lastPos = 0...
    long long int lastLine,
    // Not inclusive
    long long int lastPos,
    TextParsingOpts<Trait> &po)
{
    if (po.m_line > lastLine) {
        return;
    } else if (po.m_line == lastLine && po.m_pos >= lastPos) {
        return;
    }

    typename Trait::String text;

    const auto isLastChar = po.m_pos >= po.m_fr.m_data.at(po.m_line).first.length();
    long long int startPos = (isLastChar ? 0 : po.m_pos);
    long long int startLine = (isLastChar ? po.m_line + 1 : po.m_line);

    bool lineBreak =
        (!po.m_ignoreLineBreak && po.m_line != (long long int)(po.m_fr.m_data.size() - 1) &&
            (po.m_line == lastLine ? (lastPos == po.m_fr.m_data.at(po.m_line).first.length() &&
                isLineBreak<Trait>(po.m_fr.m_data.at(po.m_line).first.virginSubString())) :
            isLineBreak<Trait>(po.m_fr.m_data.at(po.m_line).first.virginSubString())));

    // makeTOWLB
    auto makeTOWLB = [&]() {
        if (po.m_line != (long long int)(po.m_fr.m_data.size() - 1)) {
            const auto &line = po.m_fr.m_data.at(po.m_line).first.asString();

            makeTextObjectWithLineBreak(text, po, startPos, startLine,
                line.length() - lineBreakLength<Trait>(line) - 1, po.m_line);

            startPos = 0;
            startLine = po.m_line + 1;

            text.clear();
        }
    }; // makeTOWLB

    if (lineBreak) {
        text.push_back(removeLineBreak<Trait>(po.m_fr.m_data.at(po.m_line).first.virginSubString(po.m_pos)));

        makeTOWLB();
    } else {
        const auto length = (po.m_line == lastLine ?
            lastPos - po.m_pos : po.m_fr.m_data.at(po.m_line).first.length() - po.m_pos);
        const auto s = po.m_fr.m_data.at(po.m_line).first.virginSubString(po.m_pos, length);
        text.push_back(s);

        po.m_pos = (po.m_line == lastLine ? lastPos : po.m_fr.m_data.at(po.m_line).first.length());

        makeTextObject(text,
                       po,
                       startPos,
                       startLine,
                       po.m_line == lastLine ? lastPos - 1 : po.m_fr.m_data.at(po.m_line).first.length() - 1,
                       po.m_line);

        text.clear();
    }

    if (po.m_line != lastLine) {
        ++po.m_line;

        for (; po.m_line < lastLine; ++po.m_line) {
            startPos = 0;
            startLine = po.m_line;

            lineBreak = (!po.m_ignoreLineBreak && po.m_line != (long long int)(po.m_fr.m_data.size() - 1) &&
                isLineBreak<Trait>(po.m_fr.m_data.at(po.m_line).first.asString()));

            const auto s = (lineBreak ? removeLineBreak<Trait>(po.m_fr.m_data.at(po.m_line).first.virginSubString()) :
                po.m_fr.m_data.at(po.m_line).first.virginSubString());
            text.push_back(s);

            if (lineBreak) {
                makeTOWLB();
            } else {
                makeTextObject(text, po, 0, po.m_line,
                    po.m_fr.m_data.at(po.m_line).first.length() - 1, po.m_line);
            }

            text.clear();
        }

        lineBreak = (!po.m_ignoreLineBreak && po.m_line != (long long int)(po.m_fr.m_data.size() - 1) &&
            lastPos == po.m_fr.m_data.at(po.m_line).first.length() &&
            isLineBreak<Trait>(po.m_fr.m_data.at(po.m_line).first.asString()));

        auto s = po.m_fr.m_data.at(po.m_line).first.virginSubString(0, lastPos);

        po.m_pos = lastPos;

        if (!lineBreak) {
            text.push_back(s);

            makeTextObject(text, po, 0, lastLine, lastPos - 1, lastLine);
        } else {
            s = removeLineBreak<Trait>(s);
            text.push_back(s);

            makeTOWLB();
        }
    }
}

//! Skip spaces.
template<class Trait>
inline void
skipSpacesInHtml(long long int &l,
                 long long int &p,
                 const typename MdBlock<Trait>::Data &fr)
{
    while (l < (long long int)fr.size()) {
        p = skipSpaces<Trait>(p, fr[l].first.asString());

        if (p < fr[l].first.length()) {
            return;
        }

        p = 0;
        ++l;
    }
}

//! Read HTML attribute value.
template<class Trait>
inline std::pair<bool, bool>
readUnquotedHtmlAttrValue(long long int &l,
                          long long int &p,
                          const typename MdBlock<Trait>::Data &fr)
{
    static const typename Trait::String notAllowed = Trait::latin1ToString("\"`=<'");

    const auto start = p;

    for (; p < fr[l].first.length(); ++p) {
        if (fr[l].first[p].isSpace()) {
            break;
        } else if (notAllowed.contains(fr[l].first[p])) {
            return {false, false};
        } else if (fr[l].first[p] == Trait::latin1ToChar('>')) {
            return {p - start > 0, p - start > 0};
        }
    }

    return {p - start > 0, p - start > 0};
}

//! Read HTML attribute value.
template<class Trait>
inline std::pair<bool, bool>
readHtmlAttrValue(long long int &l,
                  long long int &p,
                  const typename MdBlock<Trait>::Data &fr)
{
    if (p < fr[l].first.length() && fr[l].first[p] != Trait::latin1ToChar('"') &&
        fr[l].first[p] != Trait::latin1ToChar('\'')) {
        return readUnquotedHtmlAttrValue<Trait>(l, p, fr);
    }

    const auto s = fr[l].first[p];

    ++p;

    if (p >= fr[l].first.length()) {
        return {false, false};
    }

    for (; l < (long long int)fr.size(); ++l) {
        bool doBreak = false;

        for (; p < fr[l].first.length(); ++p) {
            const auto ch = fr[l].first[p];

            if (ch == s) {
                doBreak = true;

                break;
            }
        }

        if (doBreak) {
            break;
        }

        p = 0;
    }

    if (l >= (long long int)fr.size()) {
        return {false, false};
    }

    if (p >= fr[l].first.length()) {
        return {false, false};
    }

    if (fr[l].first[p] != s) {
        return {false, false};
    }

    ++p;

    return {true, true};
}

//! Read HTML attribute.
template<class Trait>
inline std::pair<bool, bool>
readHtmlAttr(long long int &l,
             long long int &p,
             const typename MdBlock<Trait>::Data &fr,
             bool checkForSpace)
{
    long long int tl = l, tp = p;

    skipSpacesInHtml<Trait>(l, p, fr);

    if (l >= (long long int)fr.size()) {
        return {false, false};
    }

    // /
    if (p < fr[l].first.length() && fr[l].first[p] == Trait::latin1ToChar('/')) {
        return {false, true};
    }

    // >
    if (p < fr[l].first.length() && fr[l].first[p] == Trait::latin1ToChar('>')) {
        return {false, true};
    }

    if (checkForSpace) {
        if (tl == l && tp == p) {
            return {false, false};
        }
    }

    const auto start = p;

    for (; p < fr[l].first.length(); ++p) {
        const auto ch = fr[l].first[p];

        if (ch.isSpace() || ch == Trait::latin1ToChar('>') || ch == Trait::latin1ToChar('=')) {
            break;
        }
    }

    const typename Trait::String name = fr[l].first.asString().sliced(start, p - start).toLower();

    if (!name.startsWith(Trait::latin1ToString("_")) && !name.startsWith(Trait::latin1ToString(":")) &&
        !name.isEmpty() && !(name[0].unicode() >= 97 && name[0].unicode() <= 122)) {
        return {false, false};
    }

    static const typename Trait::String allowedInName =
        Trait::latin1ToString("abcdefghijklmnopqrstuvwxyz0123456789_.:-");

    for (long long int i = 1; i < name.length(); ++i) {
        if (!allowedInName.contains(name[i])) {
            return {false, false};
        }
    }

    // >
    if (p < fr[l].first.length() && fr[l].first[p] == Trait::latin1ToChar('>')) {
        return {false, true};
    }

    tl = l;
    tp = p;

    skipSpacesInHtml<Trait>(l, p, fr);

    if (l >= (long long int)fr.size()) {
        return {false, false};
    }

    // =
    if (p < fr[l].first.length()) {
        if (fr[l].first[p] != Trait::latin1ToChar('=')) {
            l = tl;
            p = tp;

            return {true, true};
        } else {
            ++p;
        }
    } else {
        return {true, false};
    }

    skipSpacesInHtml<Trait>(l, p, fr);

    if (l >= (long long int)fr.size()) {
        return {false, false};
    }

    return readHtmlAttrValue<Trait>(l, p, fr);
}

//! \return Is HTML tag at the given position?
template<class Trait>
inline std::tuple<bool, long long int, long long int, bool, typename Trait::String>
isHtmlTag(long long int line, long long int pos, TextParsingOpts<Trait> &po, int rule);

//! \return Is after the given position only HTML tags?
template<class Trait>
inline bool
isOnlyHtmlTagsAfterOrClosedRule1(long long int line,
                                 long long int pos,
                                 TextParsingOpts<Trait> &po,
                                 int rule)
{
    static const std::set<typename Trait::String> s_rule1Finish = {Trait::latin1ToString("/pre"),
                                                                   Trait::latin1ToString("/script"),
                                                                   Trait::latin1ToString("/style"),
                                                                   Trait::latin1ToString("/textarea")};

    auto p = skipSpaces<Trait>(pos, po.m_fr.m_data[line].first.asString());

    while (p < po.m_fr.m_data[line].first.length()) {
        bool ok = false;

        long long int l;
        typename Trait::String tag;

        std::tie(ok, l, p, std::ignore, tag) = isHtmlTag(line, p, po, rule);

        ++p;

        if (rule != 1) {
            if (!ok) {
                return false;
            }

            if (l > line) {
                return true;
            }
        } else {
            if (s_rule1Finish.find(tag.toLower()) != s_rule1Finish.cend() && l == line) {
                return true;
            }

            if (l > line) {
                return false;
            }
        }

        p = skipSpaces<Trait>(p, po.m_fr.m_data[line].first.asString());
    }

    if (p >= po.m_fr.m_data[line].first.length()) {
        return true;
    }

    return false;
}

//! \return Is setext heading in the lines?
template<class Trait>
inline bool
isSetextHeadingBetween(const TextParsingOpts<Trait> &po,
                       long long int startLine,
                       long long int endLine)
{
    for (; startLine <= endLine; ++startLine) {
        const auto pos = skipSpaces<Trait>(0, po.m_fr.m_data.at(startLine).first.asString());
        const auto line = po.m_fr.m_data.at(startLine).first.asString().sliced(pos);

        if ((isH1<Trait>(line) || isH2<Trait>(line)) && pos < 4) {
            return true;
        }
    }

    return false;
}

//! \return Is HTML tag at the given position?
template<class Trait>
inline std::tuple<bool, long long int, long long int, bool, typename Trait::String>
isHtmlTag(long long int line,
          long long int pos,
          TextParsingOpts<Trait> &po,
          int rule)
{
    if (po.m_fr.m_data[line].first[pos] != Trait::latin1ToChar('<')) {
        return {false, line, pos, false, {}};
    }

    typename Trait::String tag;

    long long int l = line;
    long long int p = pos + 1;
    bool first = false;

    {
        const auto tmp = skipSpaces<Trait>(0, po.m_fr.m_data[l].first.asString());
        first = (tmp == pos);
    }

    if (p >= po.m_fr.m_data[l].first.length()) {
        return {false, line, pos, first, tag};
    }

    bool closing = false;

    if (po.m_fr.m_data[l].first[p] == Trait::latin1ToChar('/')) {
        closing = true;

        tag.push_back(Trait::latin1ToChar('/'));

        ++p;
    }

    const auto start = p;

    // tag
    for (; p < po.m_fr.m_data[l].first.length(); ++p) {
        const auto ch = po.m_fr.m_data[l].first[p];

        if (ch.isSpace() || ch == Trait::latin1ToChar('>') || ch == Trait::latin1ToChar('/')) {
            break;
        }
    }

    tag.push_back(po.m_fr.m_data[l].first.asString().sliced(start, p - start));

    if (p < po.m_fr.m_data[l].first.length() && po.m_fr.m_data[l].first[p] == Trait::latin1ToChar('/')) {
        if (p + 1 < po.m_fr.m_data[l].first.length() &&
            po.m_fr.m_data[l].first[p + 1] == Trait::latin1ToChar('>')) {
            long long int tmp = 0;

            if (rule == 7) {
                tmp = skipSpaces<Trait>(p + 2, po.m_fr.m_data[l].first.asString());
            }

            bool onLine = (first && (rule == 7 ? tmp == po.m_fr.m_data[l].first.length() :
                isOnlyHtmlTagsAfterOrClosedRule1(l, p + 2, po, rule == 1)));

            if (!isSetextHeadingBetween(po, line, l)) {
                return {true, l, p + 1, onLine, tag};
            } else {
                return {false, line, pos, first, tag};
            }
        } else {
            return {false, line, pos, first, tag};
        }
    }

    if (p < po.m_fr.m_data[l].first.length() && po.m_fr.m_data[l].first[p] == Trait::latin1ToChar('>')) {
        long long int tmp = 0;

        if (rule == 7) {
            tmp = skipSpaces<Trait>(p + 1, po.m_fr.m_data[l].first.asString());
        }

        bool onLine = (first && (rule == 7 ? tmp == po.m_fr.m_data[l].first.length() :
            isOnlyHtmlTagsAfterOrClosedRule1(l, p + 1, po, rule == 1)));

        if (!isSetextHeadingBetween(po, line, l)) {
            return {true, l, p, onLine, tag};
        } else {
            return {false, line, pos, first, tag};
        }
    }

    skipSpacesInHtml<Trait>(l, p, po.m_fr.m_data);

    if (l >= (long long int)po.m_fr.m_data.size()) {
        return {false, line, pos, first, tag};
    }

    if (po.m_fr.m_data[l].first[p] == Trait::latin1ToChar('>')) {
        long long int tmp = 0;

        if (rule == 7) {
            tmp = skipSpaces<Trait>(p + 1, po.m_fr.m_data[l].first.asString());
        }

        bool onLine = (first && (rule == 7 ? tmp == po.m_fr.m_data[l].first.length() :
            isOnlyHtmlTagsAfterOrClosedRule1(l, p + 1, po, rule == 1)));

        if (!isSetextHeadingBetween(po, line, l)) {
            return {true, l, p, onLine, tag};
        } else {
            return {false, line, pos, first, tag};
        }
    }

    bool attr = true;
    bool firstAttr = true;

    while (attr) {
        bool ok = false;

        std::tie(attr, ok) = readHtmlAttr<Trait>(l, p, po.m_fr.m_data, !firstAttr);

        firstAttr = false;

        if (closing && attr) {
            return {false, line, pos, first, tag};
        }

        if (!ok) {
            return {false, line, pos, first, tag};
        }
    }

    if (po.m_fr.m_data[l].first[p] == Trait::latin1ToChar('/')) {
        ++p;
    } else {
        skipSpacesInHtml<Trait>(l, p, po.m_fr.m_data);

        if (l >= (long long int)po.m_fr.m_data.size()) {
            return {false, line, pos, first, tag};
        }
    }

    if (po.m_fr.m_data[l].first[p] == Trait::latin1ToChar('>')) {
        long long int tmp = 0;

        if (rule == 7) {
            tmp = skipSpaces<Trait>(p + 1, po.m_fr.m_data[l].first.asString());
        }

        bool onLine = (first && (rule == 7 ? tmp == po.m_fr.m_data[l].first.length() :
            isOnlyHtmlTagsAfterOrClosedRule1(l, p + 1, po, rule == 1)));

        if (!isSetextHeadingBetween(po, line, l)) {
            return {true, l, p, onLine, tag};
        } else {
            return {false, line, pos, first, tag};
        }
    }

    return {false, line, pos, first, {}};
}

//! Read HTML tag.
template<class Trait>
inline std::pair<typename Trait::String, bool>
Parser<Trait>::readHtmlTag(typename Delims::iterator it,
                           TextParsingOpts<Trait> &po)
{
    long long int i = it->m_pos + 1;
    const auto start = i;

    if (start >= po.m_fr.m_data[it->m_line].first.length()) {
        return {{}, false};
    }

    for (; i < po.m_fr.m_data[it->m_line].first.length(); ++i) {
        const auto ch = po.m_fr.m_data[it->m_line].first[i];

        if (ch.isSpace() || ch == Trait::latin1ToChar('>')) {
            break;
        }
    }

    return {po.m_fr.m_data[it->m_line].first.asString().sliced(start, i - start),
            i < po.m_fr.m_data[it->m_line].first.length() ?
                po.m_fr.m_data[it->m_line].first[i] == Trait::latin1ToChar('>') : false};
}

template<class Trait>
inline typename Parser<Trait>::Delims::iterator
Parser<Trait>::findIt(typename Delims::iterator it,
                      typename Delims::iterator last,
                      TextParsingOpts<Trait> &po)
{
    auto ret = it;

    for (; it != last; ++it) {
        if ((it->m_line == po.m_line && it->m_pos < po.m_pos) || it->m_line < po.m_line) {
            ret = it;
        } else {
            break;
        }
    }

    return ret;
}

//! Read HTML data.
template<class Trait>
inline void
eatRawHtml(long long int line,
           long long int pos,
           long long int toLine,
           long long int toPos,
           TextParsingOpts<Trait> &po,
           bool finish,
           int htmlRule,
           bool onLine,
           bool continueEating = false)
{
    MD_UNUSED(htmlRule)

    if (line <= toLine) {
        typename Trait::String h = po.m_html.m_html->text();

        if (!h.isEmpty() && !continueEating) {
            for (long long int i = 0; i < po.m_fr.m_emptyLinesBefore; ++i) {
                h.push_back(Trait::latin1ToChar('\n'));
            }
        }

        const auto first = po.m_fr.m_data[line].first.asString().sliced(
            pos,
            (line == toLine ? (toPos >= 0 ? toPos - pos : po.m_fr.m_data[line].first.length() - pos) :
                po.m_fr.m_data[line].first.length() - pos));

        if (!h.isEmpty() && !first.isEmpty() && po.m_html.m_html->endLine() != po.m_fr.m_data[line].second.m_lineNumber) {
            h.push_back(Trait::latin1ToChar('\n'));
        }

        if (!first.isEmpty()) {
            h.push_back(first);
        }

        ++line;

        for (; line < toLine; ++line) {
            h.push_back(Trait::latin1ToChar('\n'));
            h.push_back(po.m_fr.m_data[line].first.asString());
        }

        if (line == toLine && toPos != 0) {
            h.push_back(Trait::latin1ToChar('\n'));
            h.push_back(po.m_fr.m_data[line].first.asString().sliced(0, toPos > 0 ?
                toPos : po.m_fr.m_data[line].first.length()));
        }

        auto endColumn = toPos;
        auto endLine = toLine;

        if (endColumn == 0 && endLine > 0) {
            --endLine;
            endColumn = po.m_fr.m_data.at(endLine).first.length();
        }

        po.m_html.m_html->setEndColumn(po.m_fr.m_data.at(endLine).first.virginPos(endColumn >= 0 ?
            endColumn - 1 : po.m_fr.m_data.at(endLine).first.length() - 1));
        po.m_html.m_html->setEndLine(po.m_fr.m_data.at(endLine).second.m_lineNumber);

        po.m_line = (toPos >= 0 ? toLine : toLine + 1);
        po.m_pos = (toPos >= 0 ? toPos : 0);

        if (po.m_line + 1 < static_cast<long long int>(po.m_fr.m_data.size()) &&
            po.m_pos >= po.m_fr.m_data.at(po.m_line).first.length()) {
            ++po.m_line;
            po.m_pos = 0;
        }

        po.m_html.m_html->setText(h);
    }

    UnprotectedDocsMethods<Trait>::setFreeTag(po.m_html.m_html, onLine);

    if (onLine) {
        po.m_headingAllowed = false;
        po.m_checkLineOnNewType = true;
    }

    if (finish) {
        if (!po.m_collectRefLinks) {
            po.m_parent->appendItem(po.m_html.m_html);
            po.m_parent->setEndColumn(po.m_html.m_html->endColumn());
            po.m_parent->setEndLine(po.m_html.m_html->endLine());
            initLastItemWithOpts<Trait>(po, po.m_html.m_html);
            po.m_html.m_html->setOpts(po.m_opts);
            po.m_lastText = nullptr;
        } else {
            po.m_tmpHtml = po.m_html.m_html;
        }

        const auto online = po.m_html.m_onLine;

        resetHtmlTag(po.m_html, &po);

        if (online) {
            po.m_detected = TextParsingOpts<Trait>::Detected::HTML;
        }
    } else {
        po.m_html.m_continueHtml = true;
    }
}

template<class Trait>
inline typename Parser<Trait>::Delims::iterator
Parser<Trait>::eatRawHtmlTillEmptyLine(typename Delims::iterator it,
                                       typename Delims::iterator last,
                                       long long int line,
                                       long long int pos,
                                       TextParsingOpts<Trait> &po,
                                       int htmlRule,
                                       bool onLine,
                                       bool continueEating)
{
    long long int emptyLine = line;

    if (po.m_fr.m_emptyLinesBefore > 0 && po.m_html.m_html && po.m_html.m_continueHtml) {
        po.m_html.m_continueHtml = false;
        return it;
    }

    for (auto it = po.m_fr.m_data.cbegin() + line, last = po.m_fr.m_data.cend(); it != last; ++it) {
        if (it->first.asString().simplified().isEmpty()) {
            break;
        }

        ++emptyLine;
    }

    if (emptyLine < static_cast<long long int>(po.m_fr.m_data.size())) {
        eatRawHtml(line, pos, emptyLine, 0, po, true, htmlRule, onLine, continueEating);

        return findIt(it, last, po);
    } else {
        eatRawHtml(line, pos, po.m_fr.m_data.size() - 1, -1, po, false, htmlRule, onLine, continueEating);

        if (it != last) {
            return std::prev(last);
        } else {
            return last;
        }
    }
}

template<class Trait>
inline bool
Parser<Trait>::isNewBlockIn(MdBlock<Trait> &fr,
                            long long int startLine,
                            long long int endLine)
{
    for (auto i = startLine + 1; i <= endLine; ++i) {
        const auto type = whatIsTheLine(fr.m_data[i].first);

        switch (type) {
        case Parser<Trait>::BlockType::Footnote:
        case Parser<Trait>::BlockType::FensedCodeInList:
        case Parser<Trait>::BlockType::SomethingInList:
        case Parser<Trait>::BlockType::List:
        case Parser<Trait>::BlockType::ListWithFirstEmptyLine:
        case Parser<Trait>::BlockType::Code:
        case Parser<Trait>::BlockType::Blockquote:
        case Parser<Trait>::BlockType::Heading:
        case Parser<Trait>::BlockType::EmptyLine:
            return true;

        default:
            break;
        }

        const auto ns = skipSpaces<Trait>(0, fr.m_data[i].first.asString());

        if (ns < 4) {
            const auto s = fr.m_data[i].first.asString().sliced(ns);

            if (isHorizontalLine<Trait>(s) || isH1<Trait>(s) || isH2<Trait>(s)) {
                return true;
            }
        }
    }

    return false;
}

template<class Trait>
inline void
Parser<Trait>::finishRule1HtmlTag(typename Delims::iterator it,
                                  typename Delims::iterator last,
                                  TextParsingOpts<Trait> &po,
                                  bool skipFirst)
{
    static const std::set<typename Trait::String> s_finish = {Trait::latin1ToString("/pre"),
                                                              Trait::latin1ToString("/script"),
                                                              Trait::latin1ToString("/style"),
                                                              Trait::latin1ToString("/textarea")};

    if (it != last) {
        bool ok = false;
        long long int l = -1, p = -1;

        if (po.m_html.m_html->text().isEmpty() && it->m_type == Delimiter::Less && skipFirst) {
            std::tie(ok, l, p, po.m_html.m_onLine, std::ignore) =
                isHtmlTag(it->m_line, it->m_pos, po, 1);
        }

        if (po.m_html.m_onLine) {
            for (it = (skipFirst && it != last ? std::next(it) : it); it != last; ++it) {
                if (it->m_type == Delimiter::Less) {
                    typename Trait::String tag;
                    bool closed = false;

                    std::tie(tag, closed) = readHtmlTag(it, po);

                    if (closed) {
                        if (s_finish.find(tag.toLower()) != s_finish.cend()) {
                            eatRawHtml(po.m_line, po.m_pos, it->m_line, -1, po,
                                true, 1, po.m_html.m_onLine);

                            return;
                        }
                    }
                }
            }
        } else if (ok && !isNewBlockIn(po.m_fr, it->m_line, l)) {
            eatRawHtml(po.m_line, po.m_pos, l, p + 1, po, true, 1, false);

            return;
        } else {
            resetHtmlTag(po.m_html, &po);

            return;
        }
    }

    if (po.m_html.m_onLine) {
        eatRawHtml(po.m_line, po.m_pos, po.m_fr.m_data.size() - 1, -1, po, false, 1, po.m_html.m_onLine);
    } else {
        resetHtmlTag(po.m_html, &po);
    }
}

template<class Trait>
inline void
Parser<Trait>::finishRule2HtmlTag(typename Delims::iterator it,
                                  typename Delims::iterator last,
                                  TextParsingOpts<Trait> &po)
{
    if (it != last) {
        const auto start = it;

        MdLineData::CommentData commentData = {2, true};
        bool onLine = po.m_html.m_onLine;

        if (po.m_html.m_html->text().isEmpty() && it->m_type == Delimiter::Less) {
            long long int i = po.m_fr.m_data[it->m_line].first.virginPos(it->m_pos);

            commentData = po.m_fr.m_data[it->m_line].second.m_htmlCommentData[i];

            onLine = (it->m_pos == skipSpaces<Trait>(0, po.m_fr.m_data[it->m_line].first.asString()));
            po.m_html.m_onLine = onLine;
        }

        if (commentData.first != -1 && commentData.second) {
            for (; it != last; ++it) {
                if (it->m_type == Delimiter::Greater) {
                    auto p = it->m_pos;

                    bool doContinue = false;

                    for (char i = 0; i < commentData.first; ++i) {
                        if (!(p > 0 && po.m_fr.m_data[it->m_line].first[p - 1] == Trait::latin1ToChar('-'))) {
                            doContinue = true;

                            break;
                        }

                        --p;
                    }

                    if (doContinue) {
                        continue;
                    }

                    if (onLine || !isNewBlockIn(po.m_fr, start->m_line, it->m_line)) {
                        eatRawHtml(po.m_line, po.m_pos, it->m_line,
                            onLine ? po.m_fr.m_data[it->m_line].first.length() : it->m_pos + 1,
                            po, true, 2, onLine);
                    } else {
                        resetHtmlTag(po.m_html, &po);
                    }

                    return;
                }
            }
        }
    }

    if (po.m_html.m_onLine) {
        eatRawHtml(po.m_line, po.m_pos, po.m_fr.m_data.size() - 1, -1, po, false, 2, po.m_html.m_onLine);
    } else {
        resetHtmlTag(po.m_html, &po);
    }
}

template<class Trait>
inline void
Parser<Trait>::finishRule3HtmlTag(typename Delims::iterator it,
                                  typename Delims::iterator last,
                                  TextParsingOpts<Trait> &po)
{
    bool onLine = po.m_html.m_onLine;

    if (it != last) {
        const auto start = it;

        if (po.m_html.m_html->text().isEmpty() && it->m_type == Delimiter::Less) {
            onLine = (it->m_pos == skipSpaces<Trait>(0, po.m_fr.m_data[it->m_line].first.asString()));
            po.m_html.m_onLine = onLine;
        }

        for (; it != last; ++it) {
            if (it->m_type == Delimiter::Greater) {
                if (it->m_pos > 0 && po.m_fr.m_data[it->m_line].first[it->m_pos - 1] == Trait::latin1ToChar('?')) {
                    long long int i = it->m_pos + 1;

                    for (; i < po.m_fr.m_data[it->m_line].first.length(); ++i) {
                        if (po.m_fr.m_data[it->m_line].first[i] == Trait::latin1ToChar('<')) {
                            break;
                        }
                    }

                    if (onLine || !isNewBlockIn(po.m_fr, start->m_line, it->m_line)) {
                        eatRawHtml(po.m_line, po.m_pos, it->m_line, i, po, true, 3, onLine);
                    } else {
                        resetHtmlTag(po.m_html, &po);
                    }

                    return;
                }
            }
        }
    }

    if (po.m_html.m_onLine) {
        eatRawHtml(po.m_line, po.m_pos, po.m_fr.m_data.size() - 1, -1, po, false, 3, onLine);
    } else {
        resetHtmlTag(po.m_html, &po);
    }
}

template<class Trait>
inline void
Parser<Trait>::finishRule4HtmlTag(typename Delims::iterator it,
                                  typename Delims::iterator last,
                                  TextParsingOpts<Trait> &po)
{
    if (it != last) {
        const auto start = it;

        bool onLine = po.m_html.m_onLine;

        if (po.m_html.m_html->text().isEmpty() && it->m_type == Delimiter::Less) {
            onLine = (it->m_pos == skipSpaces<Trait>(0, po.m_fr.m_data[it->m_line].first.asString()));
            po.m_html.m_onLine = onLine;
        }

        for (; it != last; ++it) {
            if (it->m_type == Delimiter::Greater) {
                long long int i = it->m_pos + 1;

                for (; i < po.m_fr.m_data[it->m_line].first.length(); ++i) {
                    if (po.m_fr.m_data[it->m_line].first[i] == Trait::latin1ToChar('<')) {
                        break;
                    }
                }

                if (onLine || !isNewBlockIn(po.m_fr, start->m_line, it->m_line)) {
                    eatRawHtml(po.m_line, po.m_pos, it->m_line, i, po, true, 4, onLine);
                } else {
                    resetHtmlTag(po.m_html, &po);
                }

                return;
            }
        }
    }

    if (po.m_html.m_onLine) {
        eatRawHtml(po.m_line, po.m_pos, po.m_fr.m_data.size() - 1, -1, po, false, 4, true);
    } else {
        resetHtmlTag(po.m_html, &po);
    }
}

template<class Trait>
inline void
Parser<Trait>::finishRule5HtmlTag(typename Delims::iterator it,
                                  typename Delims::iterator last,
                                  TextParsingOpts<Trait> &po)
{
    if (it != last) {
        const auto start = it;

        bool onLine = po.m_html.m_onLine;

        if (po.m_html.m_html->text().isEmpty() && it->m_type == Delimiter::Less) {
            onLine = (it->m_pos == skipSpaces<Trait>(0, po.m_fr.m_data[it->m_line].first.asString()));
            po.m_html.m_onLine = onLine;
        }

        for (; it != last; ++it) {
            if (it->m_type == Delimiter::Greater) {
                if (it->m_pos > 1 && po.m_fr.m_data[it->m_line].first[it->m_pos - 1] == Trait::latin1ToChar(']') &&
                    po.m_fr.m_data[it->m_line].first[it->m_pos - 2] == Trait::latin1ToChar(']')) {
                    long long int i = it->m_pos + 1;

                    for (; i < po.m_fr.m_data[it->m_line].first.length(); ++i) {
                        if (po.m_fr.m_data[it->m_line].first[i] == Trait::latin1ToChar('<')) {
                            break;
                        }
                    }

                    if (onLine || !isNewBlockIn(po.m_fr, start->m_line, it->m_line)) {
                        eatRawHtml(po.m_line, po.m_pos, it->m_line, i, po, true, 5, onLine);
                    } else {
                        resetHtmlTag(po.m_html, &po);
                    }

                    return;
                }
            }
        }
    }

    if (po.m_html.m_onLine) {
        eatRawHtml(po.m_line, po.m_pos, po.m_fr.m_data.size() - 1, -1, po, false, 5, true);
    } else {
        resetHtmlTag(po.m_html, &po);
    }
}

template<class Trait>
inline void
Parser<Trait>::finishRule6HtmlTag(typename Delims::iterator it,
                                  typename Delims::iterator last,
                                  TextParsingOpts<Trait> &po)
{
    if (!po.m_html.m_onLine) {
        po.m_html.m_onLine = (it != last ?
            it->m_pos == skipSpaces<Trait>(0, po.m_fr.m_data[it->m_line].first.asString()) : true);
    }

    if (po.m_html.m_onLine) {
        eatRawHtmlTillEmptyLine(it, last, po.m_line, po.m_pos, po, 6, true, true);
    } else {
        const auto nit = std::find_if(std::next(it), last, [](const auto &d) {
            return (d.m_type == Delimiter::Greater);
        });

        if (nit != last && !isNewBlockIn(po.m_fr, it->m_line, nit->m_line)) {
            eatRawHtml(po.m_line, po.m_pos, nit->m_line, nit->m_pos + nit->m_len, po,
                true, 6, false);
        }
    }
}

template<class Trait>
inline void
Parser<Trait>::finishRule7HtmlTag(typename Delims::iterator it,
                                  typename Delims::iterator last,
                                  TextParsingOpts<Trait> &po)
{
    if (po.m_html.m_onLine) {
        eatRawHtmlTillEmptyLine(it, last, po.m_line, po.m_pos, po, 7, true, true);
    } else if (it != last) {
        const auto start = it;
        long long int l = -1, p = -1;
        bool onLine = false;
        bool ok = false;

        std::tie(ok, l, p, onLine, std::ignore) = isHtmlTag(it->m_line, it->m_pos, po, 7);

        onLine = onLine && it->m_line == 0 && l == start->m_line;

        if (ok) {
            eatRawHtml(po.m_line, po.m_pos, l, ++p, po, !onLine, 7, onLine);

            po.m_html.m_onLine = onLine;

            if (onLine) {
                eatRawHtmlTillEmptyLine(it, last, po.m_line, po.m_pos, po, 7, onLine, true);
            }
        }
    } else {
        resetHtmlTag(po.m_html, &po);
    }
}

template<class Trait>
inline typename Parser<Trait>::Delims::iterator
Parser<Trait>::finishRawHtmlTag(typename Delims::iterator it,
                                typename Delims::iterator last,
                                TextParsingOpts<Trait> &po,
                                bool skipFirst)
{
    po.m_detected = TextParsingOpts<Trait>::Detected::HTML;

    switch (po.m_html.m_htmlBlockType) {
    case 1:
        finishRule1HtmlTag(it, last, po, skipFirst);
        break;

    case 2:
        finishRule2HtmlTag(it, last, po);
        break;

    case 3:
        finishRule3HtmlTag(it, last, po);
        break;

    case 4:
        finishRule4HtmlTag(it, last, po);
        break;

    case 5:
        finishRule5HtmlTag(it, last, po);
        break;

    case 6:
        finishRule6HtmlTag(it, last, po);
        break;

    case 7:
        finishRule7HtmlTag(it, last, po);
        break;

    default:
        po.m_detected = TextParsingOpts<Trait>::Detected::Nothing;
        break;
    }

    return findIt(it, last, po);
}

template<class Trait>
inline int
Parser<Trait>::htmlTagRule(typename Delims::iterator it,
                           typename Delims::iterator last,
                           TextParsingOpts<Trait> &po)
{
    MD_UNUSED(last)

    typename Trait::String tag;

    std::tie(tag, std::ignore) = readHtmlTag(it, po);

    if (tag.startsWith(Trait::latin1ToString("![CDATA["))) {
        return 5;
    }

    tag = tag.toLower();

    static const typename Trait::String s_validHtmlTagLetters =
        Trait::latin1ToString("abcdefghijklmnopqrstuvwxyz0123456789-");

    bool closing = false;

    if (tag.startsWith(Trait::latin1ToString("/"))) {
        tag.remove(0, 1);
        closing = true;
    }

    if (tag.endsWith(Trait::latin1ToString("/"))) {
        tag.remove(tag.size() - 1, 1);
    }

    if (tag.isEmpty()) {
        return -1;
    }

    if (!tag.startsWith(Trait::latin1ToString("!")) &&
        !tag.startsWith(Trait::latin1ToString("?")) &&
        !(tag[0].unicode() >= 97 && tag[0].unicode() <= 122)) {
        return -1;
    }

    static const std::set<typename Trait::String> s_rule1 = {Trait::latin1ToString("pre"),
                                                             Trait::latin1ToString("script"),
                                                             Trait::latin1ToString("style"),
                                                             Trait::latin1ToString("textarea")};

    if (!closing && s_rule1.find(tag) != s_rule1.cend()) {
        return 1;
    } else if (tag.startsWith(Trait::latin1ToString("!--"))) {
        return 2;
    } else if (tag.startsWith(Trait::latin1ToString("?"))) {
        return 3;
    } else if (tag.startsWith(Trait::latin1ToString("!")) && tag.size() > 1 &&
               ((tag[1].unicode() >= 65 && tag[1].unicode() <= 90) ||
                    (tag[1].unicode() >= 97 && tag[1].unicode() <= 122))) {
        return 4;
    } else {
        static const std::set<typename Trait::String> s_rule6 = {
            Trait::latin1ToString("address"),  Trait::latin1ToString("article"),    Trait::latin1ToString("aside"),    Trait::latin1ToString("base"),
            Trait::latin1ToString("basefont"), Trait::latin1ToString("blockquote"), Trait::latin1ToString("body"),     Trait::latin1ToString("caption"),
            Trait::latin1ToString("center"),   Trait::latin1ToString("col"),        Trait::latin1ToString("colgroup"), Trait::latin1ToString("dd"),
            Trait::latin1ToString("details"),  Trait::latin1ToString("dialog"),     Trait::latin1ToString("dir"),      Trait::latin1ToString("div"),
            Trait::latin1ToString("dl"),       Trait::latin1ToString("dt"),         Trait::latin1ToString("fieldset"), Trait::latin1ToString("figcaption"),
            Trait::latin1ToString("figure"),   Trait::latin1ToString("footer"),     Trait::latin1ToString("form"),     Trait::latin1ToString("frame"),
            Trait::latin1ToString("frameset"), Trait::latin1ToString("h1"),         Trait::latin1ToString("h2"),       Trait::latin1ToString("h3"),
            Trait::latin1ToString("h4"),       Trait::latin1ToString("h5"),         Trait::latin1ToString("h6"),       Trait::latin1ToString("head"),
            Trait::latin1ToString("header"),   Trait::latin1ToString("hr"),         Trait::latin1ToString("html"),     Trait::latin1ToString("iframe"),
            Trait::latin1ToString("legend"),   Trait::latin1ToString("li"),         Trait::latin1ToString("link"),     Trait::latin1ToString("main"),
            Trait::latin1ToString("menu"),     Trait::latin1ToString("menuitem"),   Trait::latin1ToString("nav"),      Trait::latin1ToString("noframes"),
            Trait::latin1ToString("ol"),       Trait::latin1ToString("optgroup"),   Trait::latin1ToString("option"),   Trait::latin1ToString("p"),
            Trait::latin1ToString("param"),    Trait::latin1ToString("section"),    Trait::latin1ToString("search"),   Trait::latin1ToString("summary"),
            Trait::latin1ToString("table"),    Trait::latin1ToString("tbody"),      Trait::latin1ToString("td"),       Trait::latin1ToString("tfoot"),
            Trait::latin1ToString("th"),       Trait::latin1ToString("thead"),      Trait::latin1ToString("title"),    Trait::latin1ToString("tr"),
            Trait::latin1ToString("track"),    Trait::latin1ToString("ul")};

        for (long long int i = 1; i < tag.size(); ++i) {
            if (!s_validHtmlTagLetters.contains(tag[i])) {
                return -1;
            }
        }

        if (s_rule6.find(tag) != s_rule6.cend()) {
            return 6;
        } else {
            bool tag = false;

            std::tie(tag, std::ignore, std::ignore, std::ignore, std::ignore) =
                isHtmlTag(it->m_line, it->m_pos, po, 7);

            if (tag) {
                return 7;
            }
        }
    }

    return -1;
}

template<class Trait>
inline typename Parser<Trait>::Delims::iterator
Parser<Trait>::checkForRawHtml(typename Delims::iterator it,
                               typename Delims::iterator last,
                               TextParsingOpts<Trait> &po)
{
    const auto rule = htmlTagRule(it, last, po);

    if (rule == -1) {
        resetHtmlTag(po.m_html, &po);

        po.m_firstInParagraph = false;

        return it;
    }

    po.m_html.m_htmlBlockType = rule;
    po.m_html.m_html.reset(new RawHtml<Trait>);
    po.m_html.m_html->setStartColumn(po.m_fr.m_data.at(it->m_line).first.virginPos(it->m_pos));
    po.m_html.m_html->setStartLine(po.m_fr.m_data.at(it->m_line).second.m_lineNumber);

    return finishRawHtmlTag(it, last, po, true);
}

template<class Trait>
inline typename Parser<Trait>::Delims::iterator
Parser<Trait>::checkForMath(typename Delims::iterator it,
                            typename Delims::iterator last,
                            TextParsingOpts<Trait> &po)
{
    po.m_wasRefLink = false;
    po.m_firstInParagraph = false;
    po.m_headingAllowed = true;

    const auto end = std::find_if(std::next(it), last, [&](const auto &d) {
        return (d.m_type == Delimiter::Math && d.m_len == it->m_len);
    });

    if (end != last && end->m_line <= po.m_lastTextLine) {
        typename Trait::String math;

        if (it->m_line == end->m_line) {
            math = po.m_fr.m_data[it->m_line].first.asString().sliced(
                it->m_pos + it->m_len, end->m_pos - (it->m_pos + it->m_len));
        } else {
            math = po.m_fr.m_data[it->m_line].first.asString().sliced(it->m_pos + it->m_len);

            for (long long int i = it->m_line + 1; i < end->m_line; ++i) {
                math.push_back(Trait::latin1ToChar('\n'));
                math.push_back(po.m_fr.m_data[i].first.asString());
            }

            math.push_back(Trait::latin1ToChar('\n'));
            math.push_back(po.m_fr.m_data[end->m_line].first.asString().sliced(0, end->m_pos));
        }

        if (!po.m_collectRefLinks) {
            std::shared_ptr<Math<Trait>> m(new Math<Trait>);

            auto startLine = po.m_fr.m_data.at(it->m_line).second.m_lineNumber;
            auto startColumn = po.m_fr.m_data.at(it->m_line).first.virginPos(it->m_pos + it->m_len);

            if (it->m_pos + it->m_len >= po.m_fr.m_data.at(it->m_line).first.length()) {
                std::tie(startColumn, startLine) = nextPosition(po.m_fr, startColumn, startLine);
            }

            auto endColumn = po.m_fr.m_data.at(end->m_line).first.virginPos(end->m_pos);
            auto endLine = po.m_fr.m_data.at(end->m_line).second.m_lineNumber;

            if (endColumn == 0) {
                std::tie(endColumn, endLine) = prevPosition(po.m_fr, endColumn, endLine);
            } else {
                --endColumn;
            }

            m->setStartColumn(startColumn);
            m->setStartLine(startLine);
            m->setEndColumn(endColumn);
            m->setEndLine(endLine);
            m->setInline(it->m_len == 1);
            m->setStartDelim({po.m_fr.m_data[it->m_line].first.virginPos(it->m_pos),
                              po.m_fr.m_data[it->m_line].second.m_lineNumber,
                              po.m_fr.m_data[it->m_line].first.virginPos(it->m_pos + it->m_len - 1),
                              po.m_fr.m_data[it->m_line].second.m_lineNumber});
            m->setEndDelim({po.m_fr.m_data[end->m_line].first.virginPos(end->m_pos),
                            po.m_fr.m_data[end->m_line].second.m_lineNumber,
                            po.m_fr.m_data[end->m_line].first.virginPos(end->m_pos + end->m_len - 1),
                            po.m_fr.m_data[end->m_line].second.m_lineNumber});
            m->setFensedCode(false);

            initLastItemWithOpts<Trait>(po, m);

            if (math.startsWith(Trait::latin1ToString("`")) &&
                math.endsWith(Trait::latin1ToString("`")) &&
                !math.endsWith(Trait::latin1ToString("\\`")) &&
                math.length() > 1) {
                math = math.sliced(1, math.length() - 2);
            }

            m->setExpr(math);

            po.m_parent->appendItem(m);

            po.m_pos = end->m_pos + end->m_len;
            po.m_line = end->m_line;
            po.m_lastText = nullptr;
        }

        return end;
    }

    return it;
}

template<class Trait>
inline typename Parser<Trait>::Delims::iterator
Parser<Trait>::checkForAutolinkHtml(typename Delims::iterator it,
                                    typename Delims::iterator last,
                                    TextParsingOpts<Trait> &po,
                                    bool updatePos)
{
    const auto nit = std::find_if(std::next(it), last, [](const auto &d) {
        return (d.m_type == Delimiter::Greater);
    });

    if (nit != last) {
        if (nit->m_line == it->m_line) {
            const auto url = po.m_fr.m_data.at(it->m_line).first.asString().sliced(
                it->m_pos + 1, nit->m_pos - it->m_pos - 1);

            bool isUrl = true;

            for (long long int i = 0; i < url.size(); ++i) {
                if (url[i].isSpace()) {
                    isUrl = false;

                    break;
                }
            }

            if (isUrl) {
                if (!isValidUrl<Trait>(url) && !isEmail<Trait>(url)) {
                    isUrl = false;
                }
            }

            if (isUrl) {
                if (!po.m_collectRefLinks) {
                    std::shared_ptr<Link<Trait>> lnk(new Link<Trait>);
                    lnk->setStartColumn(po.m_fr.m_data.at(it->m_line).first.virginPos(it->m_pos));
                    lnk->setStartLine(po.m_fr.m_data.at(it->m_line).second.m_lineNumber);
                    lnk->setEndColumn(po.m_fr.m_data.at(nit->m_line).first.virginPos(nit->m_pos + nit->m_len - 1));
                    lnk->setEndLine(po.m_fr.m_data.at(nit->m_line).second.m_lineNumber);
                    lnk->setUrl(url);
                    lnk->setOpts(po.m_opts);
                    lnk->setTextPos({po.m_fr.m_data[it->m_line].first.virginPos(it->m_pos + 1),
                                     po.m_fr.m_data[it->m_line].second.m_lineNumber,
                                     po.m_fr.m_data[nit->m_line].first.virginPos(nit->m_pos - 1),
                                     po.m_fr.m_data[nit->m_line].second.m_lineNumber});
                    lnk->setUrlPos(lnk->textPos());
                    po.m_parent->appendItem(lnk);
                }

                po.m_wasRefLink = false;
                po.m_firstInParagraph = false;
                po.m_lastText = nullptr;

                if (updatePos) {
                    po.m_pos = nit->m_pos + nit->m_len;
                    po.m_line = nit->m_line;
                }

                return nit;
            } else {
                return checkForRawHtml(it, last, po);
            }
        } else {
            return checkForRawHtml(it, last, po);
        }
    } else {
        return checkForRawHtml(it, last, po);
    }
}

template<class Trait>
inline void
Parser<Trait>::makeInlineCode(long long int startLine,
                              long long int startPos,
                              long long int lastLine,
                              long long int lastPos,
                              TextParsingOpts<Trait> &po,
                              typename Delims::iterator startDelimIt,
                              typename Delims::iterator endDelimIt)
{
    typename Trait::String c;

    for (; po.m_line <= lastLine; ++po.m_line) {
        c.push_back(po.m_fr.m_data.at(po.m_line).first.asString().sliced(
            po.m_pos, (po.m_line == lastLine ? lastPos - po.m_pos :
                po.m_fr.m_data.at(po.m_line).first.length() - po.m_pos)));

        if (po.m_line < lastLine) {
            c.push_back(Trait::latin1ToChar(' '));
        }

        po.m_pos = 0;
    }

    po.m_line = lastLine;

    if (c[0] == Trait::latin1ToChar(' ') && c[c.size() - 1] == Trait::latin1ToChar(' ') &&
        skipSpaces<Trait>(0, c) < c.size()) {
        c.remove(0, 1);
        c.remove(c.size() - 1, 1);
        ++startPos;
        --lastPos;
    }

    if (!c.isEmpty()) {
        auto code = std::make_shared<Code<Trait>>(c, false, true);

        code->setStartColumn(po.m_fr.m_data.at(startLine).first.virginPos(startPos));
        code->setStartLine(po.m_fr.m_data.at(startLine).second.m_lineNumber);
        code->setEndColumn(po.m_fr.m_data.at(lastLine).first.virginPos(lastPos - 1));
        code->setEndLine(po.m_fr.m_data.at(lastLine).second.m_lineNumber);
        code->setStartDelim({po.m_fr.m_data.at(startDelimIt->m_line).first.virginPos(
                                startDelimIt->m_pos + (startDelimIt->m_backslashed ? 1 : 0)),
                             po.m_fr.m_data.at(startDelimIt->m_line).second.m_lineNumber,
                             po.m_fr.m_data.at(startDelimIt->m_line).first.virginPos(
                                startDelimIt->m_pos + (startDelimIt->m_backslashed ? 1 : 0)) +
                                startDelimIt->m_len - 1 - (startDelimIt->m_backslashed ? 1 : 0),
                             po.m_fr.m_data.at(startDelimIt->m_line).second.m_lineNumber});
        code->setEndDelim(
            {po.m_fr.m_data.at(endDelimIt->m_line).first.virginPos(
                endDelimIt->m_pos + (endDelimIt->m_backslashed ? 1 : 0)),
             po.m_fr.m_data.at(endDelimIt->m_line).second.m_lineNumber,
             po.m_fr.m_data.at(endDelimIt->m_line).first.virginPos(
                endDelimIt->m_pos + (endDelimIt->m_backslashed ? 1 : 0) +
                    endDelimIt->m_len - 1 - (endDelimIt->m_backslashed ? 1 : 0)),
             po.m_fr.m_data.at(endDelimIt->m_line).second.m_lineNumber});
        code->setOpts(po.m_opts);

        initLastItemWithOpts<Trait>(po, code);

        po.m_parent->appendItem(code);
    }

    po.m_wasRefLink = false;
    po.m_firstInParagraph = false;
    po.m_lastText = nullptr;
}

template<class Trait>
inline typename Parser<Trait>::Delims::iterator
Parser<Trait>::checkForInlineCode(typename Delims::iterator it,
                                  typename Delims::iterator last,
                                  TextParsingOpts<Trait> &po)
{
    const auto len = it->m_len;
    const auto start = it;

    po.m_wasRefLink = false;
    po.m_firstInParagraph = false;
    po.m_headingAllowed = true;

    ++it;

    for (; it != last; ++it) {
        if (it->m_line <= po.m_lastTextLine) {
            const auto p = skipSpaces<Trait>(0, po.m_fr.m_data.at(it->m_line).first.asString());
            const auto withoutSpaces = po.m_fr.m_data.at(it->m_line).first.asString().sliced(p);

            if ((it->m_type == Delimiter::HorizontalLine && withoutSpaces[0] == Trait::latin1ToChar('-')) ||
                it->m_type == Delimiter::H1 || it->m_type == Delimiter::H2) {
                break;
            } else if (it->m_type == Delimiter::InlineCode && (it->m_len - (it->m_backslashed ? 1 : 0)) == len) {
                makeText(start->m_line, start->m_pos, po);

                if (!po.m_collectRefLinks) {
                    po.m_pos = start->m_pos + start->m_len;

                    makeInlineCode(start->m_line, start->m_pos + start->m_len, it->m_line,
                        it->m_pos + (it->m_backslashed ? 1 : 0), po, start, it);

                    po.m_line = it->m_line;
                    po.m_pos = it->m_pos + it->m_len;
                }

                return it;
            }
        } else {
            break;
        }
    }

    makeText(start->m_line, start->m_pos + start->m_len, po);

    return start;
}

template<class Trait>
inline std::pair<typename MdBlock<Trait>::Data, typename Parser<Trait>::Delims::iterator>
Parser<Trait>::readTextBetweenSquareBrackets(typename Delims::iterator start,
                                             typename Delims::iterator it,
                                             typename Delims::iterator last,
                                             TextParsingOpts<Trait> &po,
                                             bool doNotCreateTextOnFail,
                                             WithPosition *pos)
{
    if (it != last && it->m_line <= po.m_lastTextLine) {
        if (start->m_line == it->m_line) {
            const auto p = start->m_pos + start->m_len;
            const auto n = it->m_pos - p;

            if (pos) {
                long long int startPos, startLine, endPos, endLine;
                std::tie(startPos, startLine) = nextPosition(po.m_fr,
                                                             po.m_fr.m_data[start->m_line].first.virginPos(
                                                                start->m_pos + start->m_len - 1),
                                                             po.m_fr.m_data[start->m_line].second.m_lineNumber);
                std::tie(endPos, endLine) =
                    prevPosition(po.m_fr, po.m_fr.m_data[it->m_line].first.virginPos(it->m_pos),
                        po.m_fr.m_data[it->m_line].second.m_lineNumber);

                *pos = {startPos, startLine, endPos, endLine};
            }

            return {{{po.m_fr.m_data.at(start->m_line).first.sliced(p, n),
                    {po.m_fr.m_data.at(start->m_line).second.m_lineNumber}}}, it};
        } else {
            typename MdBlock<Trait>::Data res;
            res.push_back({po.m_fr.m_data.at(start->m_line).first.sliced(
                start->m_pos + start->m_len), po.m_fr.m_data.at(start->m_line).second});

            long long int i = start->m_line + 1;

            for (; i <= it->m_line; ++i) {
                if (i == it->m_line) {
                    res.push_back({po.m_fr.m_data.at(i).first.sliced(0, it->m_pos),
                        po.m_fr.m_data.at(i).second});
                } else {
                    res.push_back({po.m_fr.m_data.at(i).first, po.m_fr.m_data.at(i).second});
                }
            }

            if (pos) {
                long long int startPos, startLine, endPos, endLine;
                std::tie(startPos, startLine) = nextPosition(po.m_fr,
                                                             po.m_fr.m_data[start->m_line].first.virginPos(
                                                                start->m_pos + start->m_len - 1),
                                                             po.m_fr.m_data[start->m_line].second.m_lineNumber);
                std::tie(endPos, endLine) =
                    prevPosition(po.m_fr, po.m_fr.m_data[it->m_line].first.virginPos(it->m_pos),
                        po.m_fr.m_data[it->m_line].second.m_lineNumber);

                *pos = {startPos, startLine, endPos, endLine};
            }

            return {res, it};
        }
    } else {
        if (!doNotCreateTextOnFail) {
            makeText(start->m_line, start->m_pos + start->m_len, po);
        }

        return {{}, start};
    }
}

template<class Trait>
inline std::pair<typename MdBlock<Trait>::Data, typename Parser<Trait>::Delims::iterator>
Parser<Trait>::checkForLinkText(typename Delims::iterator it,
                                typename Delims::iterator last,
                                TextParsingOpts<Trait> &po,
                                WithPosition *pos)
{
    const auto start = it;

    long long int brackets = 0;

    const bool collectRefLinks = po.m_collectRefLinks;
    po.m_collectRefLinks = true;
    long long int l = po.m_line, p = po.m_pos;

    for (it = std::next(it); it != last; ++it) {
        bool quit = false;

        switch (it->m_type) {
        case Delimiter::SquareBracketsClose: {
            if (!brackets)
                quit = true;
            else
                --brackets;
        } break;

        case Delimiter::SquareBracketsOpen:
        case Delimiter::ImageOpen:
            ++brackets;
            break;

        case Delimiter::InlineCode:
            it = checkForInlineCode(it, last, po);
            break;

        case Delimiter::Less:
            it = checkForAutolinkHtml(it, last, po, false);
            break;

        default:
            break;
        }

        if (quit) {
            break;
        }
    }

    const auto r = readTextBetweenSquareBrackets(start, it, last, po, false, pos);

    po.m_collectRefLinks = collectRefLinks;
    resetHtmlTag(po.m_html, &po);
    po.m_line = l;
    po.m_pos = p;

    return r;
}

template<class Trait>
inline std::pair<typename MdBlock<Trait>::Data, typename Parser<Trait>::Delims::iterator>
Parser<Trait>::checkForLinkLabel(typename Delims::iterator it,
                                 typename Delims::iterator last,
                                 TextParsingOpts<Trait> &po,
                                 WithPosition *pos)
{
    const auto start = it;

    for (it = std::next(it); it != last; ++it) {
        bool quit = false;

        switch (it->m_type) {
        case Delimiter::SquareBracketsClose: {
            quit = true;
        } break;

        case Delimiter::SquareBracketsOpen:
        case Delimiter::ImageOpen: {
            it = last;
            quit = true;
        } break;

        default:
            break;
        }

        if (quit)
            break;
    }

    return readTextBetweenSquareBrackets(start, it, last, po, true, pos);
}

template<class Trait>
inline typename Trait::String
Parser<Trait>::toSingleLine(const typename MdBlock<Trait>::Data &d)
{
    typename Trait::String res;
    bool first = true;

    for (const auto &s : d) {
        if (!first) {
            res.push_back(Trait::latin1ToChar(' '));
        }
        res.push_back(s.first.asString().simplified());
        first = false;
    }

    return res;
}

template<class Trait>
inline std::shared_ptr<Link<Trait>>
Parser<Trait>::makeLink(const typename Trait::String &url,
                        const typename MdBlock<Trait>::Data &text,
                        TextParsingOpts<Trait> &po,
                        bool doNotCreateTextOnFail,
                        long long int startLine,
                        long long int startPos,
                        long long int lastLine,
                        long long int lastPos,
                        const WithPosition &textPos,
                        const WithPosition &urlPos)
{
    MD_UNUSED(doNotCreateTextOnFail)

    typename Trait::String u = (url.startsWith(Trait::latin1ToString("#")) ?
        url : removeBackslashes<typename Trait::String, Trait>(replaceEntity<Trait>(url)));

    if (!u.isEmpty()) {
        if (!u.startsWith(Trait::latin1ToString("#"))) {
            const auto checkForFile = [&](typename Trait::String &url,
                                          const typename Trait::String &ref = {}) -> bool {
                if (Trait::fileExists(url)) {
                    url = Trait::absoluteFilePath(url);

                    if (!po.m_collectRefLinks) {
                        po.m_linksToParse.push_back(url);
                    }

                    if (!ref.isEmpty()) {
                        url = ref + Trait::latin1ToString("/") + url;
                    }

                    return true;
                } else if (Trait::fileExists(url, po.m_workingPath)) {
                    url = Trait::absoluteFilePath(po.m_workingPath + Trait::latin1ToString("/") + url);

                    if (!po.m_collectRefLinks) {
                        po.m_linksToParse.push_back(url);
                    }

                    if (!ref.isEmpty()) {
                        url = ref + Trait::latin1ToString("/") + url;
                    }

                    return true;
                } else {
                    return false;
                }
            };

            if (!checkForFile(u) && u.contains(Trait::latin1ToChar('#'))) {
                const auto i = u.indexOf(Trait::latin1ToChar('#'));
                const auto ref = u.sliced(i);
                u = u.sliced(0, i);

                if (!checkForFile(u, ref)) {
                    u = u + ref;
                }
            }
        } else
            u = u + (po.m_workingPath.isEmpty() ? typename Trait::String() :
                Trait::latin1ToString("/") + po.m_workingPath) + Trait::latin1ToString("/") +
                po.m_fileName;
    }

    std::shared_ptr<Link<Trait>> link(new Link<Trait>);
    link->setUrl(u);
    link->setOpts(po.m_opts);
    link->setTextPos(textPos);
    link->setUrlPos(urlPos);

    MdBlock<Trait> block = {text, 0};

    std::shared_ptr<Paragraph<Trait>> p(new Paragraph<Trait>);

    RawHtmlBlock<Trait> html;

    parseFormattedTextLinksImages(block,
                                  std::static_pointer_cast<Block<Trait>>(p),
                                  po.m_doc,
                                  po.m_linksToParse,
                                  po.m_workingPath,
                                  po.m_fileName,
                                  po.m_collectRefLinks,
                                  true,
                                  html,
                                  true);

    if (!p->isEmpty()) {
        std::shared_ptr<Image<Trait>> img;

        if (p->items().size() == 1 && p->items().at(0)->type() == ItemType::Paragraph) {
            const auto ip = std::static_pointer_cast<Paragraph<Trait>>(p->items().at(0));

            for (auto it = ip->items().cbegin(), last = ip->items().cend(); it != last; ++it) {
                switch ((*it)->type()) {
                case ItemType::Link:
                    return {};

                case ItemType::Image: {
                    img = std::static_pointer_cast<Image<Trait>>(*it);
                } break;

                default:
                    break;
                }
            }

            if (img.get()) {
                link->setImg(img);
            }

            link->setP(ip);
        }
    }

    if (html.m_html.get()) {
        link->p()->appendItem(html.m_html);
    }

    link->setText(toSingleLine(removeBackslashes<Trait>(text)));
    link->setStartColumn(po.m_fr.m_data.at(startLine).first.virginPos(startPos));
    link->setStartLine(po.m_fr.m_data.at(startLine).second.m_lineNumber);
    link->setEndColumn(po.m_fr.m_data.at(lastLine).first.virginPos(lastPos - 1));
    link->setEndLine(po.m_fr.m_data.at(lastLine).second.m_lineNumber);

    initLastItemWithOpts<Trait>(po, link);

    po.m_lastText = nullptr;

    return link;
}

template<class Trait>
inline bool
Parser<Trait>::createShortcutLink(const typename MdBlock<Trait>::Data &text,
                                  TextParsingOpts<Trait> &po,
                                  long long int startLine,
                                  long long int startPos,
                                  long long int lastLineForText,
                                  long long int lastPosForText,
                                  typename Delims::iterator lastIt,
                                  const typename MdBlock<Trait>::Data &linkText,
                                  bool doNotCreateTextOnFail,
                                  const WithPosition &textPos,
                                  const WithPosition &linkTextPos)
{
    const auto u = Trait::latin1ToString("#") + toSingleLine(text).toCaseFolded().toUpper();
    const auto url = u + Trait::latin1ToString("/") + (po.m_workingPath.isEmpty() ?
        typename Trait::String() : po.m_workingPath + Trait::latin1ToString("/")) + po.m_fileName;

    po.m_wasRefLink = false;
    po.m_firstInParagraph = false;
    po.m_headingAllowed = true;

    if (po.m_doc->labeledLinks().find(url) != po.m_doc->labeledLinks().cend()) {
        if (!po.m_collectRefLinks) {
            const auto isLinkTextEmpty = toSingleLine(linkText).isEmpty();

            const auto link = makeLink(u,
                                       (isLinkTextEmpty ? text : linkText),
                                       po,
                                       doNotCreateTextOnFail,
                                       startLine,
                                       startPos,
                                       lastIt->m_line,
                                       lastIt->m_pos + lastIt->m_len,
                                       (isLinkTextEmpty ? textPos : linkTextPos),
                                       textPos);

            if (link.get()) {
                po.m_linksToParse.push_back(url);
                po.m_parent->appendItem(link);

                po.m_line = lastIt->m_line;
                po.m_pos = lastIt->m_pos + lastIt->m_len;
            } else {
                if (!doNotCreateTextOnFail) {
                    makeText(lastLineForText, lastPosForText, po);
                }

                return false;
            }
        }

        return true;
    } else if (!doNotCreateTextOnFail) {
        makeText(lastLineForText, lastPosForText, po);
    }

    return false;
}

template<class Trait>
inline std::shared_ptr<Image<Trait>>
Parser<Trait>::makeImage(const typename Trait::String &url,
                         const typename MdBlock<Trait>::Data &text,
                         TextParsingOpts<Trait> &po,
                         bool doNotCreateTextOnFail,
                         long long int startLine,
                         long long int startPos,
                         long long int lastLine,
                         long long int lastPos,
                         const WithPosition &textPos,
                         const WithPosition &urlPos)
{
    MD_UNUSED(doNotCreateTextOnFail)

    std::shared_ptr<Image<Trait>> img(new Image<Trait>);

    typename Trait::String u = (url.startsWith(Trait::latin1ToString("#")) ? url :
        removeBackslashes<typename Trait::String, Trait>(replaceEntity<Trait>(url)));

    if (Trait::fileExists(u)) {
        img->setUrl(u);
    } else if (Trait::fileExists(u, po.m_workingPath)) {
        img->setUrl(po.m_workingPath + Trait::latin1ToString("/") + u);
    } else {
        img->setUrl(u);
    }

    MdBlock<Trait> block = {text, 0};

    std::shared_ptr<Paragraph<Trait>> p(new Paragraph<Trait>);

    RawHtmlBlock<Trait> html;

    parseFormattedTextLinksImages(block,
                                  std::static_pointer_cast<Block<Trait>>(p),
                                  po.m_doc,
                                  po.m_linksToParse,
                                  po.m_workingPath,
                                  po.m_fileName,
                                  po.m_collectRefLinks,
                                  true,
                                  html,
                                  true);

    if (!p->isEmpty()) {
        if (p->items().size() == 1 && p->items().at(0)->type() == ItemType::Paragraph) {
            img->setP(std::static_pointer_cast<Paragraph<Trait>>(p->items().at(0)));
        }
    }

    img->setText(toSingleLine(removeBackslashes<Trait>(text)));
    img->setStartColumn(po.m_fr.m_data.at(startLine).first.virginPos(startPos));
    img->setStartLine(po.m_fr.m_data.at(startLine).second.m_lineNumber);
    img->setEndColumn(po.m_fr.m_data.at(lastLine).first.virginPos(lastPos - 1));
    img->setEndLine(po.m_fr.m_data.at(lastLine).second.m_lineNumber);
    img->setTextPos(textPos);
    img->setUrlPos(urlPos);

    initLastItemWithOpts<Trait>(po, img);

    po.m_lastText = nullptr;

    return img;
}

template<class Trait>
inline bool
Parser<Trait>::createShortcutImage(const typename MdBlock<Trait>::Data &text,
                                   TextParsingOpts<Trait> &po,
                                   long long int startLine,
                                   long long int startPos,
                                   long long int lastLineForText,
                                   long long int lastPosForText,
                                   typename Delims::iterator lastIt,
                                   const typename MdBlock<Trait>::Data &linkText,
                                   bool doNotCreateTextOnFail,
                                   const WithPosition &textPos,
                                   const WithPosition &linkTextPos)
{
    const auto url = Trait::latin1ToString("#") + toSingleLine(text).toCaseFolded().toUpper() +
        Trait::latin1ToString("/") + (po.m_workingPath.isEmpty() ? typename Trait::String() :
            po.m_workingPath + Trait::latin1ToString("/")) + po.m_fileName;

    po.m_wasRefLink = false;
    po.m_firstInParagraph = false;
    po.m_headingAllowed = true;

    const auto iit = po.m_doc->labeledLinks().find(url);

    if (iit != po.m_doc->labeledLinks().cend()) {
        if (!po.m_collectRefLinks) {
            const auto isLinkTextEmpty = toSingleLine(linkText).isEmpty();

            const auto img = makeImage(iit->second->url(),
                                       (isLinkTextEmpty ? text : linkText),
                                       po,
                                       doNotCreateTextOnFail,
                                       startLine,
                                       startPos,
                                       lastIt->m_line,
                                       lastIt->m_pos + lastIt->m_len,
                                       (isLinkTextEmpty ? textPos : linkTextPos),
                                       textPos);

            po.m_parent->appendItem(img);

            po.m_line = lastIt->m_line;
            po.m_pos = lastIt->m_pos + lastIt->m_len;
        }

        return true;
    } else if (!doNotCreateTextOnFail) {
        makeText(lastLineForText, lastPosForText, po);
    }

    return false;
}

//! Skip space in the block up to 1 new line.
template<class Trait>
inline void
skipSpacesUpTo1Line(long long int &line,
                    long long int &pos,
                    const typename MdBlock<Trait>::Data &fr)
{
    pos = skipSpaces<Trait>(pos, fr.at(line).first.asString());

    if (pos == fr.at(line).first.length() && line + 1 < (long long int)fr.size()) {
        ++line;
        pos = skipSpaces<Trait>(0, fr.at(line).first.asString());
    }
}

//! Read link's destination.
template<class Trait>
inline std::tuple<long long int, long long int, bool, typename Trait::String, long long int>
readLinkDestination(long long int line,
                    long long int pos,
                    const TextParsingOpts<Trait> &po,
                    WithPosition *urlPos = nullptr)
{
    skipSpacesUpTo1Line<Trait>(line, pos, po.m_fr.m_data);

    const auto destLine = line;
    const auto &s = po.m_fr.m_data.at(line).first.asString();
    bool backslash = false;

    if (pos < s.length() && line <= po.m_lastTextLine) {
        if (s[pos] == Trait::latin1ToChar('<')) {
            ++pos;

            if (urlPos) {
                urlPos->setStartColumn(po.m_fr.m_data[line].first.virginPos(pos));
                urlPos->setStartLine(po.m_fr.m_data[line].second.m_lineNumber);
            }

            const auto start = pos;

            while (pos < s.size()) {
                bool now = false;

                if (s[pos] == Trait::latin1ToChar('\\') && !backslash) {
                    backslash = true;
                    now = true;
                } else if (!backslash && s[pos] == Trait::latin1ToChar('<')) {
                    return {line, pos, false, {}, destLine};
                } else if (!backslash && s[pos] == Trait::latin1ToChar('>')) {
                    break;
                }

                if (!now) {
                    backslash = false;
                }

                ++pos;
            }

            if (pos < s.size() && s[pos] == Trait::latin1ToChar('>')) {
                if (urlPos) {
                    urlPos->setEndColumn(po.m_fr.m_data[line].first.virginPos(pos - 1));
                    urlPos->setEndLine(po.m_fr.m_data[line].second.m_lineNumber);
                }

                ++pos;

                return {line, pos, true, s.sliced(start, pos - start - 1), destLine};
            } else {
                return {line, pos, false, {}, destLine};
            }
        } else {
            long long int pc = 0;

            const auto start = pos;

            if (urlPos) {
                urlPos->setStartColumn(po.m_fr.m_data[line].first.virginPos(pos));
                urlPos->setStartLine(po.m_fr.m_data[line].second.m_lineNumber);
            }

            while (pos < s.size()) {
                bool now = false;

                if (s[pos] == Trait::latin1ToChar('\\') && !backslash) {
                    backslash = true;
                    now = true;
                } else if (!backslash && s[pos] == Trait::latin1ToChar(' ')) {
                    if (!pc) {
                        if (urlPos) {
                            urlPos->setEndColumn(po.m_fr.m_data[line].first.virginPos(pos - 1));
                            urlPos->setEndLine(po.m_fr.m_data[line].second.m_lineNumber);
                        }

                        return {line, pos, true, s.sliced(start, pos - start), destLine};
                    } else {
                        return {line, pos, false, {}, destLine};
                    }
                } else if (!backslash && s[pos] == Trait::latin1ToChar('(')) {
                    ++pc;
                } else if (!backslash && s[pos] == Trait::latin1ToChar(')')) {
                    if (!pc) {
                        if (urlPos) {
                            urlPos->setEndColumn(po.m_fr.m_data[line].first.virginPos(pos - 1));
                            urlPos->setEndLine(po.m_fr.m_data[line].second.m_lineNumber);
                        }

                        return {line, pos, true, s.sliced(start, pos - start), destLine};
                    } else {
                        --pc;
                    }
                }

                if (!now) {
                    backslash = false;
                }

                ++pos;
            }

            if (urlPos) {
                urlPos->setEndColumn(po.m_fr.m_data[line].first.virginPos(pos - 1));
                urlPos->setEndLine(po.m_fr.m_data[line].second.m_lineNumber);
            }

            return {line, pos, true, s.sliced(start, pos - start), destLine};
        }
    } else {
        return {line, pos, false, {}, destLine};
    }
}

//! Read link's title.
template<class Trait>
inline std::tuple<long long int, long long int, bool, typename Trait::String, long long int>
readLinkTitle(long long int line,
              long long int pos,
              const TextParsingOpts<Trait> &po)
{
    const auto space = (pos < po.m_fr.m_data.at(line).first.length() ?
        po.m_fr.m_data.at(line).first[pos].isSpace() : true);

    const auto firstLine = line;

    skipSpacesUpTo1Line<Trait>(line, pos, po.m_fr.m_data);

    if (pos >= po.m_fr.m_data.at(line).first.length()) {
        return {line, pos, true, {}, firstLine};
    }

    const auto sc = po.m_fr.m_data.at(line).first[pos];

    if (sc != Trait::latin1ToChar('"') && sc != Trait::latin1ToChar('\'') &&
        sc != Trait::latin1ToChar('(') && sc != Trait::latin1ToChar(')')) {
        return {line, pos, (firstLine != line && line <= po.m_lastTextLine), {}, firstLine};
    } else if (!space && sc != Trait::latin1ToChar(')')) {
        return {line, pos, false, {}, firstLine};
    }

    if (sc == Trait::latin1ToChar(')')) {
        return {line, pos, line <= po.m_lastTextLine, {}, firstLine};
    }

    const auto startLine = line;

    bool backslash = false;

    ++pos;

    skipSpacesUpTo1Line<Trait>(line, pos, po.m_fr.m_data);

    typename Trait::String title;

    while (line < (long long int)po.m_fr.m_data.size() && pos < po.m_fr.m_data.at(line).first.length()) {
        bool now = false;

        if (po.m_fr.m_data.at(line).first[pos] == Trait::latin1ToChar('\\') && !backslash) {
            backslash = true;
            now = true;
        } else if (sc == Trait::latin1ToChar('(') &&
            po.m_fr.m_data.at(line).first[pos] == Trait::latin1ToChar(')') && !backslash) {
            ++pos;
            return {line, pos, line <= po.m_lastTextLine, title, startLine};
        } else if (sc == Trait::latin1ToChar('(') &&
            po.m_fr.m_data.at(line).first[pos] == Trait::latin1ToChar('(') && !backslash) {
            return {line, pos, false, {}, startLine};
        } else if (sc != Trait::latin1ToChar('(') && po.m_fr.m_data.at(line).first[pos] == sc && !backslash) {
            ++pos;
            return {line, pos, line <= po.m_lastTextLine, title, startLine};
        } else {
            title.push_back(po.m_fr.m_data.at(line).first[pos]);
        }

        if (!now) {
            backslash = false;
        }

        ++pos;

        if (pos == po.m_fr.m_data.at(line).first.length()) {
            skipSpacesUpTo1Line<Trait>(line, pos, po.m_fr.m_data);
        }
    }

    return {line, pos, false, {}, startLine};
}

template<class Trait>
inline std::tuple<typename Trait::String, typename Trait::String, typename Parser<Trait>::Delims::iterator, bool>
Parser<Trait>::checkForInlineLink(typename Delims::iterator it,
                                  typename Delims::iterator last,
                                  TextParsingOpts<Trait> &po,
                                  WithPosition *urlPos)
{
    long long int p = it->m_pos + it->m_len;
    long long int l = it->m_line;
    bool ok = false;
    typename Trait::String dest, title;
    long long int destStartLine = 0;

    std::tie(l, p, ok, dest, destStartLine) = readLinkDestination<Trait>(l, p, po, urlPos);

    if (!ok) {
        return {{}, {}, it, false};
    }

    long long int s = 0;

    std::tie(l, p, ok, title, s) = readLinkTitle<Trait>(l, p, po);

    skipSpacesUpTo1Line<Trait>(l, p, po.m_fr.m_data);

    if (!ok || (l >= (long long int)po.m_fr.m_data.size() || p >= po.m_fr.m_data.at(l).first.length() ||
        po.m_fr.m_data.at(l).first[p] != Trait::latin1ToChar(')'))) {
        return {{}, {}, it, false};
    }

    for (; it != last; ++it) {
        if (it->m_line == l && it->m_pos == p) {
            return {dest, title, it, true};
        }
    }

    return {{}, {}, it, false};
}

template<class Trait>
inline std::tuple<typename Trait::String, typename Trait::String, typename Parser<Trait>::Delims::iterator, bool>
Parser<Trait>::checkForRefLink(typename Delims::iterator it,
                               typename Delims::iterator last,
                               TextParsingOpts<Trait> &po,
                               WithPosition *urlPos)
{
    long long int p = it->m_pos + it->m_len + 1;
    long long int l = it->m_line;
    bool ok = false;
    typename Trait::String dest, title;
    long long int destStartLine = 0;

    std::tie(l, p, ok, dest, destStartLine) = readLinkDestination<Trait>(l, p, po, urlPos);

    if (!ok) {
        return {{}, {}, it, false};
    }

    long long int titleStartLine = 0;

    std::tie(l, p, ok, title, titleStartLine) = readLinkTitle<Trait>(l, p, po);

    if (!ok) {
        return {{}, {}, it, false};
    }

    if (!title.isEmpty()) {
        p = skipSpaces<Trait>(p, po.m_fr.m_data.at(l).first.asString());

        if (titleStartLine == destStartLine && p < po.m_fr.m_data.at(l).first.length()) {
            return {{}, {}, it, false};
        } else if (titleStartLine != destStartLine && p < po.m_fr.m_data.at(l).first.length()) {
            l = destStartLine;
            p = po.m_fr.m_data.at(l).first.length();
            title.clear();
        }
    }

    for (; it != last; ++it) {
        if (it->m_line > l || (it->m_line == l && it->m_pos >= p)) {
            break;
        }
    }

    po.m_line = l;
    po.m_pos = p;

    return {dest, title, std::prev(it), true};
}

template<class Trait>
inline typename Parser<Trait>::Delims::iterator
Parser<Trait>::checkForImage(typename Delims::iterator it,
                             typename Delims::iterator last,
                             TextParsingOpts<Trait> &po)
{
    const auto start = it;

    typename MdBlock<Trait>::Data text;

    po.m_wasRefLink = false;
    po.m_firstInParagraph = false;
    po.m_headingAllowed = true;

    WithPosition textPos;
    std::tie(text, it) = checkForLinkText(it, last, po, &textPos);

    if (it != start) {
        if (it->m_pos + it->m_len < po.m_fr.m_data.at(it->m_line).first.length()) {
            // Inline -> (
            if (po.m_fr.m_data.at(it->m_line).first[it->m_pos + it->m_len] == Trait::latin1ToChar('(')) {
                typename Trait::String url, title;
                typename Delims::iterator iit;
                bool ok;

                WithPosition urlPos;
                std::tie(url, title, iit, ok) = checkForInlineLink(std::next(it), last, po, &urlPos);

                if (ok) {
                    if (!po.m_collectRefLinks) {
                        po.m_parent->appendItem(
                            makeImage(url, text, po, false, start->m_line, start->m_pos,
                                iit->m_line, iit->m_pos + iit->m_len, textPos, urlPos));
                    }

                    po.m_line = iit->m_line;
                    po.m_pos = iit->m_pos + iit->m_len;

                    return iit;
                } else if (createShortcutImage(text, po, start->m_line, start->m_pos, start->m_line,
                    start->m_pos + start->m_len, it, {}, false, textPos, {})) {
                    return it;
                }
            }
            // Reference -> [
            else if (po.m_fr.m_data.at(it->m_line).first[it->m_pos + it->m_len] == Trait::latin1ToChar('[')) {
                typename MdBlock<Trait>::Data label;
                typename Delims::iterator lit;

                WithPosition labelPos;
                std::tie(label, lit) = checkForLinkLabel(std::next(it), last, po, &labelPos);

                if (lit != std::next(it)) {
                    const auto isLabelEmpty = toSingleLine(label).isEmpty();

                    if (!isLabelEmpty
                        && createShortcutImage(label,
                                               po,
                                               start->m_line,
                                               start->m_pos,
                                               start->m_line,
                                               start->m_pos + start->m_len,
                                               lit,
                                               text,
                                               true,
                                               labelPos,
                                               textPos)) {
                        return lit;
                    } else if (isLabelEmpty
                               && createShortcutImage(text,
                                                      po,
                                                      start->m_line,
                                                      start->m_pos,
                                                      start->m_line,
                                                      start->m_pos + start->m_len,
                                                      lit,
                                                      {},
                                                      false,
                                                      textPos,
                                                      {})) {
                        return lit;
                    }
                } else if (createShortcutImage(text, po, start->m_line, start->m_pos, start->m_line,
                    start->m_pos + start->m_len, it, {}, false, textPos, {})) {
                    return it;
                }
            } else {
                return checkShortcut(start, last, po, &Parser<Trait>::createShortcutImage);
            }
        } else {
            return checkShortcut(start, last, po, &Parser<Trait>::createShortcutImage);
        }
    }

    return start;
}

template<class Trait>
inline typename Parser<Trait>::Delims::iterator
Parser<Trait>::checkForLink(typename Delims::iterator it,
                            typename Delims::iterator last,
                            TextParsingOpts<Trait> &po)
{
    const auto start = it;

    typename MdBlock<Trait>::Data text;

    const auto wasRefLink = po.m_wasRefLink;
    const auto firstInParagraph = po.m_firstInParagraph;
    po.m_wasRefLink = false;
    po.m_firstInParagraph = false;
    po.m_headingAllowed = true;

    const auto ns = skipSpaces<Trait>(0, po.m_fr.m_data.at(po.m_line).first.asString());

    WithPosition textPos;
    std::tie(text, it) = checkForLinkText(it, last, po, &textPos);

    if (it != start) {
        // Footnote reference.
        if (text.front().first.asString().startsWith(Trait::latin1ToString("^")) &&
            text.front().first.asString().length() > 1 && text.size() == 1 &&
            start->m_line == it->m_line) {
            if (!po.m_collectRefLinks) {
                std::shared_ptr<FootnoteRef<Trait>> fnr(new FootnoteRef<Trait>(
                    Trait::latin1ToString("#") + toSingleLine(text).toCaseFolded().toUpper() +
                    Trait::latin1ToString("/") + (po.m_workingPath.isEmpty() ? typename Trait::String() :
                        po.m_workingPath + Trait::latin1ToString("/")) + po.m_fileName));
                fnr->setStartColumn(po.m_fr.m_data.at(start->m_line).first.virginPos(start->m_pos));
                fnr->setStartLine(po.m_fr.m_data.at(start->m_line).second.m_lineNumber);
                fnr->setEndColumn(po.m_fr.m_data.at(it->m_line).first.virginPos(it->m_pos + it->m_len - 1));
                fnr->setEndLine(po.m_fr.m_data.at(it->m_line).second.m_lineNumber);
                fnr->setIdPos(textPos);

                typename Trait::String fnrText = Trait::latin1ToString("[");
                bool firstFnrText = true;

                for (const auto &t : text) {
                    if (!firstFnrText) {
                        fnrText.push_back(Trait::latin1ToString("\n"));
                    }

                    firstFnrText = false;

                    fnrText.push_back(t.first.asString());
                }

                fnrText.push_back(Trait::latin1ToString("]"));
                fnr->setText(fnrText);
                po.m_parent->appendItem(fnr);

                initLastItemWithOpts<Trait>(po, fnr);
            }

            po.m_line = it->m_line;
            po.m_pos = it->m_pos + it->m_len;

            return it;
        } else if (it->m_pos + it->m_len < po.m_fr.m_data.at(it->m_line).first.length()) {
            // Reference definition -> :
            if (po.m_fr.m_data.at(it->m_line).first[it->m_pos + it->m_len] == Trait::latin1ToChar(':')) {
                // Reference definitions allowed only at start of paragraph.
                if ((po.m_line == 0 || wasRefLink || firstInParagraph) && ns < 4 && start->m_pos == ns) {
                    typename Trait::String url, title;
                    typename Delims::iterator iit;
                    bool ok;

                    WithPosition labelPos;

                    std::tie(text, it) = checkForLinkLabel(start, last, po, &labelPos);

                    if (it != start && !toSingleLine(text).simplified().isEmpty()) {
                        WithPosition urlPos;
                        std::tie(url, title, iit, ok) = checkForRefLink(it, last, po, &urlPos);

                        if (ok) {
                            const auto label = Trait::latin1ToString("#") +
                                toSingleLine(text).toCaseFolded().toUpper() +
                                Trait::latin1ToString("/") +
                                (po.m_workingPath.isEmpty() ? typename Trait::String() :
                                    po.m_workingPath + Trait::latin1ToString("/")) + po.m_fileName;

                            std::shared_ptr<Link<Trait>> link(new Link<Trait>);
                            link->setStartColumn(po.m_fr.m_data.at(start->m_line).first.virginPos(
                                start->m_pos));
                            link->setStartLine(po.m_fr.m_data.at(start->m_line).second.m_lineNumber);

                            const auto endPos = prevPosition(po.m_fr,
                                po.m_fr.m_data.at(po.m_line).first.virginPos(po.m_pos),
                                po.m_fr.m_data.at(po.m_line).second.m_lineNumber);

                            link->setEndColumn(endPos.first);
                            link->setEndLine(endPos.second);

                            link->setTextPos(labelPos);
                            link->setUrlPos(urlPos);

                            url = removeBackslashes<typename Trait::String, Trait>(
								replaceEntity<Trait>(url));

                            if (!url.isEmpty()) {
                                if (Trait::fileExists(url)) {
                                    url = Trait::absoluteFilePath(url);
                                } else if (Trait::fileExists(url, po.m_workingPath)) {
                                    url = Trait::absoluteFilePath(
                                        (po.m_workingPath.isEmpty() ? typename Trait::String() :
                                            po.m_workingPath + Trait::latin1ToString("/")) + url);
                                }
                            }

                            link->setUrl(url);

                            po.m_wasRefLink = true;
                            po.m_headingAllowed = false;

                            if (po.m_doc->labeledLinks().find(label) == po.m_doc->labeledLinks().cend()) {
                                po.m_doc->insertLabeledLink(label, link);
                            }

                            return iit;
                        } else {
                            return checkShortcut(start, last, po, &Parser<Trait>::createShortcutLink);
                        }
                    } else {
                        return start;
                    }
                } else {
                    return checkShortcut(start, last, po, &Parser<Trait>::createShortcutLink);
                }
            }
            // Inline -> (
            else if (po.m_fr.m_data.at(it->m_line).first[it->m_pos + it->m_len] == Trait::latin1ToChar('(')) {
                typename Trait::String url, title;
                typename Delims::iterator iit;
                bool ok;

                WithPosition urlPos;
                std::tie(url, title, iit, ok) = checkForInlineLink(std::next(it), last, po, &urlPos);

                if (ok) {
                    const auto link = makeLink(url,
                                               text,
                                               po,
                                               false,
                                               start->m_line,
                                               start->m_pos,
                                               iit->m_line,
                                               iit->m_pos + iit->m_len,
                                               textPos,
                                               urlPos);

                    if (link.get()) {
                        if (!po.m_collectRefLinks) {
                            po.m_parent->appendItem(link);
                        }

                        po.m_line = iit->m_line;
                        po.m_pos = iit->m_pos + iit->m_len;

                        return iit;
                    } else {
                        return start;
                    }
                } else if (createShortcutLink(text, po, start->m_line, start->m_pos, start->m_line,
                    start->m_pos + start->m_len, it, {}, false, textPos, {})) {
                    return it;
                }
            }
            // Reference -> [
            else if (po.m_fr.m_data.at(it->m_line).first[it->m_pos + it->m_len] == Trait::latin1ToChar('[')) {
                typename MdBlock<Trait>::Data label;
                typename Delims::iterator lit;

                WithPosition labelPos;
                std::tie(label, lit) = checkForLinkLabel(std::next(it), last, po, &labelPos);

                const auto isLabelEmpty = toSingleLine(label).isEmpty();

                if (lit != std::next(it)) {
                    if (!isLabelEmpty
                        && createShortcutLink(label,
                                              po,
                                              start->m_line,
                                              start->m_pos,
                                              start->m_line,
                                              start->m_pos + start->m_len,
                                              lit,
                                              text,
                                              true,
                                              labelPos,
                                              textPos)) {
                        return lit;
                    } else if (isLabelEmpty
                               && createShortcutLink(text,
                                                     po,
                                                     start->m_line,
                                                     start->m_pos,
                                                     start->m_line,
                                                     start->m_pos + start->m_len,
                                                     lit,
                                                     {},
                                                     false,
                                                     textPos,
                                                     {})) {
                        return lit;
                    }
                } else if (createShortcutLink(text, po, start->m_line, start->m_pos, start->m_line,
                    start->m_pos + start->m_len, it, {}, false, textPos, {})) {
                    return it;
                }
            } else {
                return checkShortcut(start, last, po, &Parser<Trait>::createShortcutLink);
            }
        } else {
            return checkShortcut(start, last, po, &Parser<Trait>::createShortcutLink);
        }
    }

    return start;
}

//! Close style.
template<class Trait>
inline void
closeStyle(std::vector<typename TextParsingOpts<Trait>::StyleInfo> &styles,
           Style s)
{
    const auto it = std::find_if(styles.crbegin(), styles.crend(), [&](const auto &p) {
        return (p.m_style == s);
    });

    if (it != styles.crend()) {
        styles.erase(it.base() - 1);
    }
}

//! Apply styles.
template<class Trait>
inline void
applyStyles(int &opts,
            std::vector<typename TextParsingOpts<Trait>::StyleInfo> &styles)
{
    opts = 0;

    for (const auto &s : styles) {
        switch (s.m_style) {
        case Style::Strikethrough:
            opts |= StrikethroughText;
            break;

        case Style::Italic1:
        case Style::Italic2:
            opts |= ItalicText;
            break;

        case Style::Bold1:
        case Style::Bold2:
            opts |= BoldText;
            break;

        default:
            break;
        }
    }
}

template<class Trait>
inline int
Parser<Trait>::emphasisToInt(typename Delimiter::DelimiterType t)
{
    switch (t) {
    case Delimiter::Strikethrough:
        return 0;

    case Delimiter::Emphasis1:
        return 1;

    case Delimiter::Emphasis2:
        return 2;

    default:
        return -1;
    }
}

template<class Trait>
inline void
Parser<Trait>::createStyles(std::vector<std::pair<Style, long long int>> & styles,
             typename Delimiter::DelimiterType t,
             long long int style)
{
    if (t != Delimiter::Strikethrough) {
        if (style % 2 == 1) {
            styles.push_back({t == Delimiter::Emphasis1 ? Style::Italic1 : Style::Italic2, 1});
        }

        if (style >= 2) {
            for (long long int i = 0; i < style / 2; ++i) {
                styles.push_back({t == Delimiter::Emphasis1 ? Style::Bold1 : Style::Bold2, 2});
            }
        }
    } else {
        styles.push_back({Style::Strikethrough, style});
    }
}

template<class Trait>
inline std::vector<std::pair<Style, long long int>>
Parser<Trait>::createStyles(typename Delimiter::DelimiterType t,
                            const std::vector<long long int> &styles,
                            long long int lastStyle)
{
    std::vector<std::pair<Style, long long int>> ret;

    createStyles(ret, t, lastStyle);

    for (auto it = styles.crbegin(), last = styles.crend(); it != last; ++it) {
        createStyles(ret, t, *it);
    }

    return ret;
}

template<class Trait>
inline bool
Parser<Trait>::isSequence(typename Delims::iterator it,
                          long long int itLine,
                          long long int itPos,
                          typename Delimiter::DelimiterType t)
{
    return (itLine == it->m_line && itPos + it->m_len == it->m_pos && it->m_type == t);
}

template<class Trait>
inline typename Parser<Trait>::Delims::iterator
Parser<Trait>::readSequence(typename Delims::iterator it,
                            typename Delims::iterator last,
                            long long int &line,
                            long long int &pos,
                            long long int &len,
                            long long int &itCount)
{
    line = it->m_line;
    pos = it->m_pos;
    len = it->m_len;
    const auto t = it->m_type;
    itCount = 1;

    it = std::next(it);

    while (it != last && isSequence(it, line, pos, t)) {
        pos += it->m_len;
        len += it->m_len;

        ++it;
        ++itCount;
    }

    return std::prev(it);
}

inline bool
isMult3(long long int i1, long long int i2)
{
    return ((((i1 + i2) % 3) == 0) && !((i1 % 3 == 0) && (i2 % 3 == 0)));
}

template<class Trait>
inline std::tuple<bool, std::vector<std::pair<Style, long long int>>, long long int, long long int>
Parser<Trait>::isStyleClosed(typename Delims::iterator first,
                             typename Delims::iterator it,
                             typename Delims::iterator last,
                             typename Delims::iterator &stackBottom,
                             TextParsingOpts<Trait> &po)
{
    const auto open = it;
    long long int openPos, openLength, itCount, lengthFromIt, tmp;

    it = std::next(readSequence(first, open, last, openPos, openLength, tmp, lengthFromIt, itCount).second);

    const auto length = lengthFromIt;
    long long int itLine, itPos, itLength;

    struct RollbackValues {
        RollbackValues(TextParsingOpts<Trait> &po,
                       long long int line,
                       long long int pos,
                       bool collectRefLinks,
                       typename Delims::iterator &stackBottom,
                       typename Delims::iterator last)
            : m_po(po)
            , m_line(line)
            , m_pos(pos)
            , m_collectRefLinks(collectRefLinks)
            , m_stackBottom(stackBottom)
            , m_last(last)
            , m_it(m_last)
        {
        }

        void setIterator(typename Delims::iterator it)
        {
            m_it = it;
        }

        ~RollbackValues()
        {
            m_po.m_line = m_line;
            m_po.m_pos = m_pos;
            m_po.m_collectRefLinks = m_collectRefLinks;

            if (m_it != m_last && (m_it > m_stackBottom || m_stackBottom == m_last)) {
                m_stackBottom = m_it;
            }
        }

        TextParsingOpts<Trait> &m_po;
        long long int m_line;
        long long int m_pos;
        bool m_collectRefLinks;
        typename Delims::iterator &m_stackBottom;
        typename Delims::iterator m_last;
        typename Delims::iterator m_it;
    };

    RollbackValues rollback(po, po.m_line, po.m_pos, po.m_collectRefLinks, stackBottom, last);

    po.m_collectRefLinks = true;

    std::vector<long long int> styles;

    struct Opener {
        std::vector<typename Delims::iterator> m_its;
        long long int m_length;
    };

    std::vector<Opener> openers;

    std::function<void(long long int, long long int)> dropOpeners;

    dropOpeners = [&openers](long long int pos, long long int line) {
        while (!openers.empty()) {
            if (openers.back().m_its.front()->m_line > line || (openers.back().m_its.front()->m_line == line &&
                                                        openers.back().m_its.front()->m_pos > pos)) {
                std::for_each( openers.back().m_its.begin(), openers.back().m_its.end(),
                               [](auto &i) { i->m_skip = true; });
                openers.pop_back();
            } else {
                break;
            }
        }
    };

    auto tryCloseEmphasis = [&dropOpeners, this, &openers, &open](typename Delims::iterator first,
                               typename Delims::iterator it,
                               typename Delims::iterator last) -> bool
    {
        const auto type = it->m_type;
        const auto both = it->m_leftFlanking && it->m_rightFlanking;
        long long int tmp1, tmp2, tmp3, tmp4;
        long long int closeLength;

        it = this->readSequence(first, it, last, tmp1, closeLength, tmp2, tmp3, tmp4).first;
        it = std::prev(it);

        long long int tmpLength = closeLength;

        for (;; --it) {
            switch (it->m_type) {
            case Delimiter::Strikethrough: {
                if (it->m_leftFlanking && it->m_len == closeLength && type == it->m_type) {
                    dropOpeners(it->m_pos, it->m_line);
                    return true;
                }
            } break;

            case Delimiter::Emphasis1:
            case Delimiter::Emphasis2:
            {
                if (it->m_leftFlanking && type == it->m_type) {
                    long long int pos, len;
                    this->readSequence(first, it, last, pos, len, tmp1, tmp2, tmp3);

                    if ((both || (it->m_leftFlanking && it->m_rightFlanking)) && isMult3(len, closeLength)) {
                        continue;
                    }

                    dropOpeners(pos - len, it->m_line);

                    if (tmpLength >= len) {
                        tmpLength -= len;

                        if (open->m_type == it->m_type) {
                            openers.pop_back();
                        }

                        if (!tmpLength) {
                            return true;
                        }
                    } else {
                        if (open->m_type == it->m_type) {
                            openers.back().m_length -= tmpLength;
                        }

                        return true;
                    }
                }
            } break;

            default:
                break;
            }

            if (it == first) {
                break;
            }
        }

        return false;
    };

    auto fillIterators = [](typename Delims::iterator first,
                            typename Delims::iterator last) -> std::vector<typename Delims::iterator>
    {
        std::vector<typename Delims::iterator> res;

        for (; first != last; ++first) {
            res.push_back(first);
        }

        res.push_back(last);

        return res;
    };

    for (; it != last; ++it) {
        if (it > stackBottom) {
            return {false, {{Style::Unknown, 0}}, open->m_len, 1};
        }

        if (it->m_line <= po.m_lastTextLine) {
            po.m_line = it->m_line;
            po.m_pos = it->m_pos;

            switch (it->m_type) {
            case Delimiter::SquareBracketsOpen:
                it = checkForLink(it, last, po);
                break;

            case Delimiter::ImageOpen:
                it = checkForImage(it, last, po);
                break;

            case Delimiter::Less:
                it = checkForAutolinkHtml(it, last, po, false);
                break;

            case Delimiter::Strikethrough: {
                if (open->m_type == it->m_type && open->m_len == it->m_len && it->m_rightFlanking) {
                    rollback.setIterator(it);
                    return {true, createStyles(open->m_type, styles, open->m_len), open->m_len, 1};
                } else if (it->m_rightFlanking && tryCloseEmphasis(open, it, last)) {
                } else if (it->m_leftFlanking && open->m_type == it->m_type) {
                    openers.push_back({{it}, it->m_len});
                }
            } break;

            case Delimiter::Emphasis1:
            case Delimiter::Emphasis2: {
                if (open->m_type == it->m_type) {
                    const auto itBoth = (it->m_leftFlanking && it->m_rightFlanking);

                    if (it->m_rightFlanking) {
                        bool notCheck = (open->m_leftFlanking && open->m_rightFlanking) || itBoth;

                        long long int count;
                        auto firstIt = it;
                        it = readSequence(it, last, itLine, itPos, itLength, count);

                        if (notCheck) {
                            notCheck = isMult3(openLength, itLength);
                        }

                        if (!openers.empty()) {
                            long long int i = openers.size() - 1;
                            auto &top = openers[i];

                            while (!openers.empty()) {
                                if (i >= 0) {
                                    top = openers[i];
                                } else {
                                    break;
                                }

                                if ((itBoth || (top.m_its.front()->m_rightFlanking && top.m_its.front()->m_leftFlanking))
                                    && isMult3(itLength, top.m_length)) {
                                    --i;
                                    continue;
                                }

                                if (top.m_length <= itLength) {
                                    itLength -= top.m_length;
                                    openers.erase(openers.begin() + i);
                                } else {
                                    top.m_length -= itLength;
                                    itLength = 0;
                                }

                                --i;

                                if (!itLength) {
                                    break;
                                }
                            }
                        }

                        if (itLength) {
                            if (!notCheck) {
                                if (itLength >= lengthFromIt) {
                                    rollback.setIterator(it);
                                    return {true, createStyles(open->m_type, styles, lengthFromIt), length, itCount};
                                } else {
                                    styles.push_back(itLength);
                                    lengthFromIt -= itLength;
                                }
                            } else if (firstIt->m_leftFlanking) {
                                openers.push_back({fillIterators(firstIt, it), itLength});
                            }
                        }
                    } else {
                        long long int count;
                        auto firstIt = it;
                        it = readSequence(it, last, itLine, itPos, itLength, count);
                        openers.push_back({fillIterators(firstIt, it), itLength});
                    }
                } else if (it->m_rightFlanking) {
                    tryCloseEmphasis(open, it, last);
                }
            } break;

            case Delimiter::InlineCode:
                it = checkForInlineCode(it, last, po);
                break;

            default:
                break;
            }
        } else {
            break;
        }
    }

    return {false, {{Style::Unknown, 0}}, open->m_len, 1};
}

template<class Trait>
inline typename Parser<Trait>::Delims::iterator
Parser<Trait>::incrementIterator(typename Delims::iterator it,
                                 typename Delims::iterator last,
                                 long long int count)
{
    const auto len = std::distance(it, last);

    if (count < len) {
        return it + count;
    } else {
        return it + (len - 1);
    }
}

//! Append close style.
template<class Trait>
inline void
appendCloseStyle(TextParsingOpts<Trait> &po,
                 const StyleDelim &s)
{
    if (po.m_lastItemWithStyle) {
        po.m_lastItemWithStyle->closeStyles().push_back(s);
    }
}

template<class Trait>
inline std::pair<typename Parser<Trait>::Delims::iterator, typename Parser<Trait>::Delims::iterator>
Parser<Trait>::readSequence(typename Delims::iterator first,
                            typename Delims::iterator it,
                            typename Delims::iterator last,
                            long long int &pos,
                            long long int &length,
                            long long int &itCount,
                            long long int &lengthFromIt,
                            long long int &itCountFromIt)
{
    long long int line = it->m_line;
    pos = it->m_pos + it->m_len;
    long long int ppos = it->m_pos;
    const auto t = it->m_type;
    lengthFromIt = it->m_len;
    itCountFromIt = 1;

    auto retItLast = std::next(it);

    for (; retItLast != last; ++retItLast) {
        if (retItLast->m_line == line && pos == retItLast->m_pos && retItLast->m_type == t) {
            lengthFromIt += retItLast->m_len;
            pos = retItLast->m_pos + retItLast->m_len;
            ++itCountFromIt;
        } else {
            break;
        }
    }

    length = lengthFromIt;
    itCount = itCountFromIt;

    auto retItFirst = it;
    bool useNext = false;

    if (retItFirst != first) {
        retItFirst = std::prev(retItFirst);
        useNext = true;

        for (;; --retItFirst) {
            if (retItFirst->m_line == line && ppos - retItFirst->m_len == retItFirst->m_pos && retItFirst->m_type == t) {
                length += retItFirst->m_len;
                ppos = retItFirst->m_pos;
                ++itCount;
                useNext = false;
            } else {
                useNext = true;
                break;
            }

            if (retItFirst == first) {
                break;
            }
        }
    }

    return {useNext ? std::next(retItFirst) : retItFirst, std::prev(retItLast)};
}

template<class Trait>
inline typename Parser<Trait>::Delims::iterator
Parser<Trait>::checkForStyle(typename Delims::iterator first,
                             typename Delims::iterator it,
                             typename Delims::iterator last,
                             typename Delims::iterator &stackBottom,
                             TextParsingOpts<Trait> &po)
{
    long long int count = 1;

    po.m_wasRefLink = false;
    po.m_firstInParagraph = false;

    if (it->m_rightFlanking) {
        long long int pos, len, tmp1, tmp2;
        readSequence(first, it, last, pos, len, count, tmp1, tmp2);
        const auto t = it->m_type;

        long long int opened = 0;
        bool bothFlanking = false;

        for (auto it = po.m_styles.crbegin(), last = po.m_styles.crend(); it != last; ++it) {
            bool doBreak = false;

            switch (t) {
            case Delimiter::Emphasis1: {
                if (it->m_style == Style::Italic1 || it->m_style == Style::Bold1) {
                    opened = it->m_length;
                    bothFlanking = it->m_bothFlanking;
                    doBreak = true;
                }
            } break;

            case Delimiter::Emphasis2: {
                if (it->m_style == Style::Italic2 || it->m_style == Style::Bold2) {
                    opened = it->m_length;
                    bothFlanking = it->m_bothFlanking;
                    doBreak = true;
                }
            } break;

            case Delimiter::Strikethrough: {
                if (it->m_style == Style::Strikethrough) {
                    opened = it->m_length;
                    bothFlanking = it->m_bothFlanking;
                    doBreak = true;
                }
            } break;

            default:
                break;
            }

            if (doBreak)
                break;
        }

        const bool sumMult3 = (it->m_leftFlanking || bothFlanking ? isMult3(opened, len) : false);

        if (count && opened && !sumMult3) {
            if (count > opened) {
                count = opened;
            }

            auto pos = po.m_fr.m_data.at(it->m_line).first.virginPos(it->m_pos);
            const auto line = po.m_fr.m_data.at(it->m_line).second.m_lineNumber;

            if (it->m_type == Delimiter::Strikethrough) {
                const auto len = it->m_len;

                for (auto i = 0; i < count; ++i) {
                    closeStyle<Trait>(po.m_styles, Style::Strikethrough);
                    appendCloseStyle(po, {StrikethroughText, pos, line, pos + len - 1, line});
                    pos += len;
                }
            } else {
                if (count % 2 == 1) {
                    const auto st = (it->m_type == Delimiter::Emphasis1 ? Style::Italic1 : Style::Italic2);

                    closeStyle<Trait>(po.m_styles, st);
                    appendCloseStyle(po, {ItalicText, pos, line, pos, line});
                    ++pos;
                }

                if (count >= 2) {
                    const auto st = (it->m_type == Delimiter::Emphasis1 ? Style::Bold1 : Style::Bold2);

                    for (auto i = 0; i < count / 2; ++i) {
                        closeStyle<Trait>(po.m_styles, st);
                        appendCloseStyle(po, {BoldText, pos, line, pos + 1, line});
                        pos += 2;
                    }
                }
            }

            applyStyles<Trait>(po.m_opts, po.m_styles);

            const auto j = incrementIterator(it, last, count - 1);

            po.m_pos = j->m_pos + j->m_len;
            po.m_line = j->m_line;

            return j;
        }
    }

    count = 1;

    if (it->m_leftFlanking) {
        switch (it->m_type) {
        case Delimiter::Strikethrough:
        case Delimiter::Emphasis1:
        case Delimiter::Emphasis2: {
            bool closed = false;
            std::vector<std::pair<Style, long long int>> styles;
            long long int len = 0;

            if (it > stackBottom) {
                stackBottom = last;
            }

            if (it->m_skip) {
                closed = false;
                long long int tmp1, tmp2, tmp3;
                readSequence(it, last, tmp1, tmp2, len, tmp3);
            } else {
                std::tie(closed, styles, len, count) = isStyleClosed(first, it, last, stackBottom, po);
            }

            if (closed) {
                auto pos = po.m_fr.m_data.at(it->m_line).first.virginPos(it->m_pos);
                const auto line = po.m_fr.m_data.at(it->m_line).second.m_lineNumber;

                for (const auto &p : styles) {
                    po.m_styles.push_back({p.first, p.second, it->m_leftFlanking && it->m_rightFlanking});

                    if (!po.m_collectRefLinks) {
                        po.m_openStyles.push_back({styleToTextOption(p.first), pos, line,
                            pos + p.second - 1, line});
                    }

                    pos += p.second;
                }

                po.m_pos = it->m_pos + len;
                po.m_line = it->m_line;

                applyStyles<Trait>(po.m_opts, po.m_styles);
            } else {
                makeText(it->m_line, it->m_pos + len, po);
            }
        } break;

        default: {
            makeText(it->m_line, it->m_pos + it->m_len, po);
        } break;
        }
    }

    if (!count) {
        count = 1;
    }

    resetHtmlTag(po.m_html, &po);

    return incrementIterator(it, last, count - 1);
}

//! Concatenate texts in block.
template<class Trait>
inline std::shared_ptr<Text<Trait>>
concatenateText(typename Block<Trait>::Items::const_iterator it,
                typename Block<Trait>::Items::const_iterator last)
{
    std::shared_ptr<Text<Trait>> t(new Text<Trait>);
    t->setOpts(std::static_pointer_cast<Text<Trait>>(*it)->opts());
    t->setStartColumn((*it)->startColumn());
    t->setStartLine((*it)->startLine());

    typename ItemWithOpts<Trait>::Styles close;

    typename Trait::String data;

    for (; it != last; ++it) {
        const auto tt = std::static_pointer_cast<Text<Trait>>(*it);

        data.push_back(tt->text());

        if (!tt->openStyles().empty()) {
            std::copy(tt->openStyles().cbegin(), tt->openStyles().cend(),
                std::back_inserter(t->openStyles()));
        }

        if (!tt->closeStyles().empty()) {
            std::copy(tt->closeStyles().cbegin(), tt->closeStyles().cend(),
                std::back_inserter(close));
        }
    }

    it = std::prev(it);

    t->setText(data);
    t->setEndColumn((*it)->endColumn());
    t->setEndLine((*it)->endLine());
    t->closeStyles() = close;

    return t;
}

//! \return Is optimization type a semi one.
inline bool
isSemiOptimization(OptimizeParagraphType t)
{
    switch (t) {
    case OptimizeParagraphType::Semi:
    case OptimizeParagraphType::SemiWithoutRawData:
        return true;

    default:
        return false;
    }
}

//! \return Is optimization type without raw data optimization?
inline bool
isWithoutRawDataOptimization(OptimizeParagraphType t)
{
    switch (t) {
    case OptimizeParagraphType::FullWithoutRawData:
    case OptimizeParagraphType::SemiWithoutRawData:
        return true;

    default:
        return false;
    }
}

//! Optimize Paragraph.
template<class Trait>
inline std::shared_ptr<Paragraph<Trait>>
optimizeParagraph(std::shared_ptr<Paragraph<Trait>> &p,
                  TextParsingOpts<Trait> &po,
                  OptimizeParagraphType type = OptimizeParagraphType::Full)
{
    std::shared_ptr<Paragraph<Trait>> np(new Paragraph<Trait>);
    np->setStartColumn(p->startColumn());
    np->setStartLine(p->startLine());
    np->setEndColumn(p->endColumn());
    np->setEndLine(p->endLine());

    int opts = TextWithoutFormat;
    auto start = p->items().cend();
    long long int line = -1;
    long long int auxStart = 0, auxIt = 0;
    bool finished = false;

    for (auto it = p->items().cbegin(), last = p->items().cend(); it != last; ++it) {
        if ((*it)->type() == ItemType::Text) {
            const auto t = std::static_pointer_cast<Text<Trait>>(*it);

            if (start == last) {
                start = it;
                opts = t->opts();
                line = t->endLine();
                finished = (isSemiOptimization(type) && !t->closeStyles().empty());
            } else {
                if (opts != t->opts() || t->startLine() != line || finished ||
                    (!t->openStyles().empty() && isSemiOptimization(type))) {
                    if (!isWithoutRawDataOptimization(type)) {
                        po.concatenateAuxText(auxStart, auxIt);
                        auxIt = auxIt - (auxIt - auxStart) + 1;
                        auxStart = auxIt;
                    }

                    np->appendItem(concatenateText<Trait>(start, it));
                    start = it;
                    opts = t->opts();
                    line = t->endLine();
                }

                finished = (isSemiOptimization(type) && !t->closeStyles().empty());
            }

            if (!isWithoutRawDataOptimization(type))
                ++auxIt;
        } else {
            finished = false;

            if (start != last) {
                if (!isWithoutRawDataOptimization(type)) {
                    po.concatenateAuxText(auxStart, auxIt);
                    auxIt = auxIt - (auxIt - auxStart) + 1;
                    auxStart = auxIt;
                }

                np->appendItem(concatenateText<Trait>(start, it));
                start = last;
                opts = TextWithoutFormat;
                line = (*it)->endLine();
            }

            np->appendItem((*it));
        }
    }

    if (start != p->items().cend()) {
        np->appendItem(concatenateText<Trait>(start, p->items().cend()));

        if (!isWithoutRawDataOptimization(type)) {
            po.concatenateAuxText(auxStart, po.m_rawTextData.size());
        }
    }

    p = np;

    return p;
}

//! Normalize position.
inline void
normalizePos(long long int &pos,
             long long int &line,
             long long int length,
             long long int linesCount)
{
    if (pos != 0 && line < linesCount && pos == length) {
        pos = 0;
        ++line;
    }
}

//! Make Paragraph.
template<class Trait>
inline std::shared_ptr<Paragraph<Trait>>
makeParagraph(typename Block<Trait>::Items::const_iterator first,
              typename Block<Trait>::Items::const_iterator last)
{
    auto p = std::make_shared<Paragraph<Trait>>();

    p->setStartColumn((*first)->startColumn());
    p->setStartLine((*first)->startLine());

    for (; first != last; ++first) {
        p->appendItem(*first);
        p->setEndColumn((*first)->endColumn());
        p->setEndLine((*first)->endLine());
    }

    return p;
}

//! Split Paragraph and free HTML.
template<class Trait>
inline std::shared_ptr<Paragraph<Trait>>
splitParagraphsAndFreeHtml(std::shared_ptr<Block<Trait>> parent,
                           std::shared_ptr<Paragraph<Trait>> p,
                           TextParsingOpts<Trait> &po,
                           bool collectRefLinks,
                           bool fullyOptimizeParagraphs = true)
{
    auto first = p->items().cbegin();
    auto it = first;
    auto last = p->items().cend();

    for (; it != last; ++it) {
        if (first == last) {
            first = it;
        }

        if ((*it)->type() == ItemType::RawHtml &&
            UnprotectedDocsMethods<Trait>::isFreeTag(std::static_pointer_cast<RawHtml<Trait>>(*it))) {
            auto p = makeParagraph<Trait>(first, it);

            if (!collectRefLinks) {
                if (!p->isEmpty()) {
                    parent->appendItem(optimizeParagraph<Trait>(p, po,
                                                                fullyOptimizeParagraphs ?
                                                                    OptimizeParagraphType::FullWithoutRawData :
                                                                    OptimizeParagraphType::SemiWithoutRawData));
                }

                parent->appendItem(*it);
            }

            first = last;
        }
    }

    if (first != last) {
        if (first != p->items().cbegin()) {
            const auto c = std::count_if(first, last, [](const auto &i) {
                return (i->type() == MD::ItemType::Text);
            });
            po.m_rawTextData.erase(po.m_rawTextData.cbegin(), po.m_rawTextData.cbegin() +
                (po.m_rawTextData.size() - c));

            return makeParagraph<Trait>(first, last);
        } else {
            return p;
        }
    } else {
        po.m_rawTextData.clear();

        return std::make_shared<Paragraph<Trait>>();
    }
}

//! \return Last virgin position of the item.
template<class Trait>
inline long long int
lastVirginPositionInParagraph(Item<Trait> *item)
{
    switch (item->type()) {
    case ItemType::Text:
    case ItemType::Link:
    case ItemType::Image:
    case ItemType::FootnoteRef:
    case ItemType::RawHtml:
    {
        auto i = static_cast<ItemWithOpts<Trait> *>(item);

        if (!i->closeStyles().empty()) {
            return i->closeStyles().back().endColumn();
        } else {
            return i->endColumn();
        }
    }
        break;

    case ItemType::Code:
    case ItemType::Math:
    {
        auto c = static_cast<Code<Trait> *>(item);

        if (!c->closeStyles().empty()) {
            return c->closeStyles().back().endColumn();
        } else {
            return c->endDelim().endColumn();
        }
    }
        break;

    default:
        return -1;
    }
}

//! Make heading.
template<class Trait>
inline void
makeHeading(std::shared_ptr<Block<Trait>> parent,
            std::shared_ptr<Document<Trait>> doc,
            std::shared_ptr<Paragraph<Trait>> p,
            long long int lastColumn,
            long long int lastLine,
            int level,
            const typename Trait::String &workingPath,
            const typename Trait::String &fileName,
            bool collectRefLinks,
            const WithPosition &delim,
            TextParsingOpts<Trait> &po)
{
    if (!collectRefLinks) {
        if (p->items().back()->type() == ItemType::LineBreak) {
            auto lb = std::static_pointer_cast<LineBreak<Trait>>(p->items().back());
            const auto lineBreakBySpaces = lb->text().simplified().isEmpty();

            p = makeParagraph<Trait>(p->items().cbegin(), std::prev(p->items().cend()));
            const auto lineBreakPos = localPosFromVirgin(po.m_fr, lb->startColumn(), lb->startLine());

            if (!p->isEmpty()) {
                if (p->items().back()->type() == ItemType::Text) {
                    auto lt = std::static_pointer_cast<Text<Trait>>(p->items().back());

                    if (!lineBreakBySpaces) {
                        auto text = po.m_fr.m_data.at(lineBreakPos.second).first.fullVirginString().sliced(
                                    lt->startColumn());
                        po.m_rawTextData.back().m_str = text;

                        if (!lt->text()[0].isSpace()) {
                            const auto notSpacePos = skipSpaces<Trait>(0, text);

                            text.remove(0, notSpacePos);
                        }

                        lt->setText(removeBackslashes<typename Trait::String, Trait>(replaceEntity<Trait>(text)));
                    }

                    lt->setEndColumn(lt->endColumn() + lb->text().length());
                } else {
                    if (!lineBreakBySpaces) {
                        const auto lastItemVirginPos = lastVirginPositionInParagraph<Trait>(p->items().back().get());
                        const auto lastItemPos = localPosFromVirgin(po.m_fr, lastItemVirginPos, lineBreakPos.second);
                        const auto endOfLine = po.m_fr.m_data.at(lineBreakPos.second).first.virginSubString(
                                    lastItemPos.first + 1);
                        auto t = std::make_shared<Text<Trait>>();
                        t->setText(endOfLine);
                        t->setStartColumn(lastItemVirginPos + 1);
                        t->setStartLine(lb->startLine());
                        t->setEndColumn(lb->endColumn());
                        t->setEndLine(lb->endLine());

                        p->appendItem(t);

                        const auto pos = localPosFromVirgin(po.m_fr, lb->startColumn(), lb->startLine());

                        po.m_rawTextData.push_back({lb->text(), pos.first, pos.second});
                    }
                }
            }
        }

        std::pair<typename Trait::String, WithPosition> label;

        if (p->items().back()->type() == ItemType::Text) {
            auto t = std::static_pointer_cast<Text<Trait>>(p->items().back());

            if (t->opts() == TextWithoutFormat) {
                auto text = po.m_rawTextData.back();
                typename Trait::InternalString tmp(text.m_str);
                label = findAndRemoveHeaderLabel<Trait>(tmp);

                if (!label.first.isEmpty()) {
                    label.first = label.first.sliced(1, label.first.length() - 2);

                    if (tmp.asString().simplified().isEmpty()) {
                        p->removeItemAt(p->items().size() - 1);
                        po.m_rawTextData.pop_back();

                        if (!p->items().empty()) {
                            const auto last = std::static_pointer_cast<WithPosition>(p->items().back());
                            p->setEndColumn(last->endColumn());
                            p->setEndLine(last->endLine());
                        }
                    } else {
                        const auto notSpacePos = tmp.virginPos(skipSpaces<Trait>(0, tmp.asString()));
                        const auto virginLine = t->endLine();

                        if (label.second.startColumn() > notSpacePos) {
                            auto text = tmp.fullVirginString().sliced(0, label.second.startColumn());
                            po.m_rawTextData.back().m_str = text;

                            if (!t->text()[0].isSpace()) {
                                const auto notSpacePos = skipSpaces<Trait>(0, text);

                                text.remove(0, notSpacePos);
                            }

                            t->setText(removeBackslashes<typename Trait::String, Trait>(replaceEntity<Trait>(text)));
                            t->setEndColumn(label.second.startColumn() - 1);

                            const auto lastPos = t->endColumn();
                            const auto pos = localPosFromVirgin(po.m_fr, label.second.endColumn() + 1, virginLine);

                            if (pos.first != -1) {
                                t = std::make_shared<Text<Trait>>();
                                t->setStartColumn(label.second.endColumn() + 1);
                                t->setStartLine(virginLine);
                                t->setEndColumn(lastPos);
                                t->setEndLine(virginLine);
                                p->appendItem(t);

                                po.m_rawTextData.push_back({po.m_fr.m_data[pos.second].first.asString().sliced(pos.first),
                                                            pos.first, pos.second});
                            }
                        }

                        const auto pos = localPosFromVirgin(po.m_fr, label.second.endColumn() + 1, virginLine);

                        if (pos.first != -1) {
                            po.m_rawTextData.back() = {po.m_fr.m_data[pos.second].first.asString().sliced(pos.first),
                                                       pos.first, pos.second};

                            auto text = po.m_rawTextData.back().m_str;

                            if (!text.simplified().isEmpty()) {
                                if (p->items().size() == 1) {
                                    const auto ns = skipSpaces<Trait>(0, text);

                                    text.remove(0, ns);
                                }

                                t->setStartColumn(label.second.endColumn() + 1);
                                t->setText(removeBackslashes<typename Trait::String, Trait>(replaceEntity<Trait>(text)));
                            } else {
                                po.m_rawTextData.pop_back();
                                p->removeItemAt(p->items().size() - 1);
                            }
                        }

                        p->setEndColumn(t->endColumn());
                    }
                } else {
                    label.first.clear();
                }

                label.second.setStartLine(t->startLine());
                label.second.setEndLine(t->endLine());
            }
        }

        std::shared_ptr<Heading<Trait>> h(new Heading<Trait>);
        h->setStartColumn(p->startColumn());
        h->setStartLine(p->startLine());
        h->setEndColumn(lastColumn);
        h->setEndLine(lastLine);
        h->setLevel(level);

        if (!p->items().empty()) {
            h->setText(p);
        }

        h->setDelims({delim});

        if (label.first.isEmpty() && !p->items().empty()) {
            label.first = Trait::latin1ToString("#") + paragraphToLabel(p.get());
        } else {
            h->setLabelPos(label.second);
        }

        if (!label.first.isEmpty()) {
            const auto path = Trait::latin1ToString("/") + (!workingPath.isEmpty() ?
                workingPath + Trait::latin1ToString("/") : typename Trait::String()) + fileName;

            h->setLabel(label.first + path);

            doc->insertLabeledHeading(label.first + path, h);
            h->labelVariants().push_back(h->label());

            if (label.first != label.first.toLower()) {
                doc->insertLabeledHeading(label.first.toLower() + path, h);
                h->labelVariants().push_back(label.first.toLower() + path);
            }
        }

        parent->appendItem(h);
    }
}

//! \return Index of text item for the given index in raw text data.
template<class Trait>
inline long long int
textAtIdx(std::shared_ptr<Paragraph<Trait>> p,
          size_t idx)
{
    size_t i = 0;

    for (auto it = p->items().cbegin(), last = p->items().cend(); it != last; ++it) {
        if ((*it)->type() == ItemType::Text) {
            if (i == idx) {
                return std::distance(p->items().cbegin(), it);
            }

            ++i;
        }
    }

    return -1;
}

//! Process text plugins.
template<class Trait>
inline void
checkForTextPlugins(std::shared_ptr<Paragraph<Trait>> p,
                    TextParsingOpts<Trait> &po,
                    const TextPluginsMap<Trait> &textPlugins,
                    bool inLink)
{
    for (const auto &plugin : textPlugins) {
        if (inLink && !std::get<bool>(plugin.second)) {
            continue;
        }

        std::get<TextPluginFunc<Trait>>(plugin.second)(p, po,
            std::get<typename Trait::StringList>(plugin.second));
    }
}

//! Make horizontal line.
template<class Trait>
inline void
makeHorLine(const typename MdBlock<Trait>::Line &line,
            std::shared_ptr<Block<Trait>> parent)
{
    std::shared_ptr<Item<Trait>> hr(new HorizontalLine<Trait>);
    hr->setStartColumn(line.first.virginPos(skipSpaces<Trait>(0, line.first.asString())));
    hr->setStartLine(line.second.m_lineNumber);
    hr->setEndColumn(line.first.virginPos(line.first.length() - 1));
    hr->setEndLine(line.second.m_lineNumber);
    parent->appendItem(hr);
}

template<class Trait>
inline long long int
Parser<Trait>::parseFormattedTextLinksImages(MdBlock<Trait> &fr,
                                             std::shared_ptr<Block<Trait>> parent,
                                             std::shared_ptr<Document<Trait>> doc,
                                             typename Trait::StringList &linksToParse,
                                             const typename Trait::String &workingPath,
                                             const typename Trait::String &fileName,
                                             bool collectRefLinks,
                                             bool ignoreLineBreak,
                                             RawHtmlBlock<Trait> &html,
                                             bool inLink)

{
    if (fr.m_data.empty()) {
        return -1;
    }

    std::shared_ptr<Paragraph<Trait>> p(new Paragraph<Trait>);
    p->setStartColumn(fr.m_data.at(0).first.virginPos(0));
    p->setStartLine(fr.m_data.at(0).second.m_lineNumber);

    auto delims = collectDelimiters(fr.m_data);

    TextParsingOpts<Trait> po = {fr, p, nullptr, doc, linksToParse, workingPath, fileName,
        collectRefLinks, ignoreLineBreak, html, m_textPlugins};
    typename Delims::iterator styleStackBottom = delims.end();

    if (html.m_html.get() && html.m_continueHtml) {
        finishRawHtmlTag(delims.begin(), delims.end(), po, false);
    } else if (!delims.empty()) {
        for (auto it = delims.begin(), last = delims.end(); it != last; ++it) {
            if (po.m_line > po.m_lastTextLine) {
                checkForTableInParagraph(po, fr.m_data.size() - 1);
            }

            if (po.shouldStopParsing() && po.m_lastTextLine < it->m_line) {
                break;
            } else {
                makeText(po.m_lastTextLine < it->m_line ? po.m_lastTextLine : it->m_line,
                    po.m_lastTextLine < it->m_line ? po.m_lastTextPos : it->m_pos, po);
            }

            switch (it->m_type) {
            case Delimiter::SquareBracketsOpen: {
                it = checkForLink(it, last, po);
                p->setEndColumn(fr.m_data.at(it->m_line).first.virginPos(it->m_pos + it->m_len - 1));
                p->setEndLine(fr.m_data.at(it->m_line).second.m_lineNumber);
            } break;

            case Delimiter::ImageOpen: {
                it = checkForImage(it, last, po);
                p->setEndColumn(fr.m_data.at(it->m_line).first.virginPos(it->m_pos + it->m_len - 1));
                p->setEndLine(fr.m_data.at(it->m_line).second.m_lineNumber);
            } break;

            case Delimiter::Less: {
                it = checkForAutolinkHtml(it, last, po, true);

                if (!html.m_html.get()) {
                    p->setEndColumn(fr.m_data.at(it->m_line).first.virginPos(it->m_pos + it->m_len - 1));
                    p->setEndLine(fr.m_data.at(it->m_line).second.m_lineNumber);
                }
            } break;

            case Delimiter::Strikethrough:
            case Delimiter::Emphasis1:
            case Delimiter::Emphasis2: {
                if (!collectRefLinks) {
                    it = checkForStyle(delims.begin(), it, last, styleStackBottom, po);
                    p->setEndColumn(fr.m_data.at(it->m_line).first.virginPos(it->m_pos + it->m_len - 1));
                    p->setEndLine(fr.m_data.at(it->m_line).second.m_lineNumber);
                }
            } break;

            case Delimiter::Math: {
                it = checkForMath(it, last, po);
                p->setEndColumn(fr.m_data.at(it->m_line).first.virginPos(it->m_pos + it->m_len - 1));
                p->setEndLine(fr.m_data.at(it->m_line).second.m_lineNumber);
            } break;

            case Delimiter::InlineCode: {
                if (!it->m_backslashed) {
                    it = checkForInlineCode(it, last, po);
                    p->setEndColumn(fr.m_data.at(it->m_line).first.virginPos(it->m_pos + it->m_len - 1));
                    p->setEndLine(fr.m_data.at(it->m_line).second.m_lineNumber);
                }
            } break;

            case Delimiter::HorizontalLine: {
                po.m_wasRefLink = false;
                po.m_firstInParagraph = false;

                const auto pos = skipSpaces<Trait>(0, po.m_fr.m_data[it->m_line].first.asString());
                const auto withoutSpaces = po.m_fr.m_data[it->m_line].first.asString().sliced(pos);

                auto h2 = isH2<Trait>(withoutSpaces);

                optimizeParagraph<Trait>(p, po, OptimizeParagraphType::Semi);

                checkForTextPlugins<Trait>(p, po, m_textPlugins, inLink);

                if (it->m_line - 1 >= 0) {
                    p->setEndColumn(fr.m_data.at(it->m_line - 1).first.virginPos(
                        fr.m_data.at(it->m_line - 1).first.length() - 1));
                    p->setEndLine(fr.m_data.at(it->m_line - 1).second.m_lineNumber);
                }

                p = splitParagraphsAndFreeHtml(parent, p, po, collectRefLinks, m_fullyOptimizeParagraphs);

                if (!h2 || !po.m_headingAllowed) {
                    if (!collectRefLinks && !p->isEmpty()) {
                        parent->appendItem(p);
                    }

                    h2 = false;
                } else {
                    makeHeading(parent,
                                doc,
                                optimizeParagraph<Trait>(p, po, defaultParagraphOptimization()),
                                fr.m_data[it->m_line].first.virginPos(it->m_pos + it->m_len - 1),
                                fr.m_data[it->m_line].second.m_lineNumber,
                                2,
                                workingPath,
                                fileName,
                                collectRefLinks,
                                {po.m_fr.m_data[it->m_line].first.virginPos(pos),
                                 fr.m_data[it->m_line].second.m_lineNumber,
                                 po.m_fr.m_data[it->m_line].first.virginPos(
                                    lastNonSpacePos(po.m_fr.m_data[it->m_line].first.asString())),
                                 fr.m_data[it->m_line].second.m_lineNumber},
                                po);

                    po.m_checkLineOnNewType = true;
                }

                p.reset(new Paragraph<Trait>);
                po.m_rawTextData.clear();

                if (it->m_line + 1 < static_cast<long long int>(fr.m_data.size())) {
                    p->setStartColumn(fr.m_data.at(it->m_line + 1).first.virginPos(0));
                    p->setStartLine(fr.m_data.at(it->m_line + 1).second.m_lineNumber);
                }

                po.m_parent = p;
                po.m_line = it->m_line;
                po.m_pos = it->m_pos + it->m_len;

                if (!h2 && !collectRefLinks) {
                    makeHorLine<Trait>(fr.m_data[it->m_line], parent);
                }
            } break;

            case Delimiter::H1:
            case Delimiter::H2: {
                po.m_wasRefLink = false;
                po.m_firstInParagraph = false;

                optimizeParagraph<Trait>(p, po, OptimizeParagraphType::Semi);

                checkForTextPlugins<Trait>(p, po, m_textPlugins, inLink);

                if (it->m_line - 1 >= 0) {
                    p->setEndColumn(fr.m_data.at(it->m_line - 1).first.virginPos(
                        fr.m_data.at(it->m_line - 1).first.length() - 1));
                    p->setEndLine(fr.m_data.at(it->m_line - 1).second.m_lineNumber);
                }

                p = splitParagraphsAndFreeHtml(parent, p, po, collectRefLinks,
                                        m_fullyOptimizeParagraphs);

                if (po.m_headingAllowed) {
                    makeHeading(parent,
                                doc,
                                optimizeParagraph<Trait>(p, po, defaultParagraphOptimization()),
                                fr.m_data[it->m_line].first.virginPos(it->m_pos + it->m_len - 1),
                                fr.m_data[it->m_line].second.m_lineNumber,
                                it->m_type == Delimiter::H1 ? 1 : 2,
                                workingPath,
                                fileName,
                                collectRefLinks,
                                {po.m_fr.m_data[it->m_line].first.virginPos(skipSpaces<Trait>(
                                    0, po.m_fr.m_data[it->m_line].first.asString())),
                                 fr.m_data[it->m_line].second.m_lineNumber,
                                 po.m_fr.m_data[it->m_line].first.virginPos(lastNonSpacePos(
                                    po.m_fr.m_data[it->m_line].first.asString())),
                                 fr.m_data[it->m_line].second.m_lineNumber},
                                po);

                    po.m_checkLineOnNewType = true;

                    p.reset(new Paragraph<Trait>);
                    po.m_rawTextData.clear();

                    if (it->m_line + 1 < static_cast<long long int>(fr.m_data.size())) {
                        p->setStartColumn(fr.m_data.at(it->m_line + 1).first.virginPos(0));
                        p->setStartLine(fr.m_data.at(it->m_line + 1).second.m_lineNumber);
                    }

                    po.m_line = it->m_line;
                    po.m_pos = it->m_pos + it->m_len;
                } else if (p->startColumn() == -1) {
                    p->setStartColumn(fr.m_data.at(it->m_line).first.virginPos(it->m_pos));
                    p->setStartLine(fr.m_data.at(it->m_line).second.m_lineNumber);
                }

                po.m_parent = p;
            } break;

            default: {
                if (!po.shouldStopParsing()) {
                    po.m_wasRefLink = false;
                    po.m_firstInParagraph = false;

                    makeText(it->m_line, it->m_pos + it->m_len, po);
                }
            } break;
            }

            if (po.shouldStopParsing()) {
                break;
            }

            if (po.m_checkLineOnNewType) {
                if (po.m_line + 1 < static_cast<long long int>(po.m_fr.m_data.size())) {
                    const auto type = Parser<Trait>::whatIsTheLine(po.m_fr.m_data[po.m_line + 1].first);

                    bool doBreak = false;

                    switch (type) {
                    case Parser<Trait>::BlockType::CodeIndentedBySpaces:
                        po.m_detected = TextParsingOpts<Trait>::Detected::Code;
                        doBreak = true;
                        break;

                    case Parser<Trait>::BlockType::List:
                    case Parser<Trait>::BlockType::ListWithFirstEmptyLine:
                        po.m_detected = TextParsingOpts<Trait>::Detected::List;
                        doBreak = true;
                        break;

                    case Parser<Trait>::BlockType::Blockquote:
                        po.m_detected = TextParsingOpts<Trait>::Detected::Blockquote;
                        doBreak = true;
                        break;

                    default:
                        break;
                    }

                    if (doBreak) {
                        break;
                    }
                }

                po.m_checkLineOnNewType = false;
            }
        }
    }

    if (po.m_lastTextLine == -1) {
        checkForTableInParagraph(po, po.m_fr.m_data.size() - 1);
    }

    switch(po.m_detected) {
    case TextParsingOpts<Trait>::Detected::Table:
        makeText(po.m_lastTextLine, po.m_lastTextPos, po);
        break;

    case TextParsingOpts<Trait>::Detected::Nothing:
    {
        if(po.m_line <= static_cast<long long int>(po.m_fr.m_data.size() - 1)) {
            makeText(po.m_fr.m_data.size() - 1, po.m_fr.m_data.back().first.length(), po);
        }
    }
        break;

    default:
        break;
    }

    if (!p->isEmpty()) {
        optimizeParagraph<Trait>(p, po, OptimizeParagraphType::Semi);

        checkForTextPlugins<Trait>(p, po, m_textPlugins, inLink);

        p = splitParagraphsAndFreeHtml(parent, p, po, collectRefLinks, m_fullyOptimizeParagraphs);

        if (!p->isEmpty() && !collectRefLinks) {
            parent->appendItem(optimizeParagraph<Trait>(p, po, defaultParagraphOptimization()));
        }

        po.m_rawTextData.clear();
    }

    normalizePos(po.m_pos, po.m_line, po.m_line < static_cast<long long int>(po.m_fr.m_data.size()) ?
                                    po.m_fr.m_data[po.m_line].first.length() : 0, po.m_fr.m_data.size());

    if (po.m_detected != TextParsingOpts<Trait>::Detected::Nothing) {
        if (po.m_line < static_cast<long long int>(po.m_fr.m_data.size())) {
            return po.m_fr.m_data.at(po.m_line).second.m_lineNumber;
        }
    }

    return -1;
}

template<class Trait>
inline void
Parser<Trait>::parseFootnote(MdBlock<Trait> &fr,
                             std::shared_ptr<Block<Trait>>,
                             std::shared_ptr<Document<Trait>> doc,
                             typename Trait::StringList &linksToParse,
                             const typename Trait::String &workingPath,
                             const typename Trait::String &fileName,
                             bool collectRefLinks)
{
    {
        const auto it = (std::find_if(fr.m_data.rbegin(), fr.m_data.rend(), [](const auto &s) {
                            return !s.first.isEmpty();
                        })).base();

        if (it != fr.m_data.end()) {
            fr.m_data.erase(it, fr.m_data.end());
        }
    }

    if (!fr.m_data.empty()) {
        std::shared_ptr<Footnote<Trait>> f(new Footnote<Trait>);
        f->setStartColumn(fr.m_data.front().first.virginPos(0));
        f->setStartLine(fr.m_data.front().second.m_lineNumber);
        f->setEndColumn(fr.m_data.back().first.virginPos(fr.m_data.back().first.length() - 1));
        f->setEndLine(fr.m_data.back().second.m_lineNumber);

        auto delims = collectDelimiters(fr.m_data);

        RawHtmlBlock<Trait> html;

        TextParsingOpts<Trait> po = {fr, f, nullptr, doc, linksToParse, workingPath, fileName,
            collectRefLinks, false, html, m_textPlugins};
        po.m_lastTextLine = fr.m_data.size();
        po.m_lastTextPos = fr.m_data.back().first.length();

        if (!delims.empty() && delims.cbegin()->m_type == Delimiter::SquareBracketsOpen &&
            !delims.cbegin()->m_isWordBefore) {
            typename MdBlock<Trait>::Data id;
            typename Delims::iterator it = delims.end();

            po.m_line = delims.cbegin()->m_line;
            po.m_pos = delims.cbegin()->m_pos;

            std::tie(id, it) = checkForLinkText(delims.begin(), delims.end(), po);

            if (!toSingleLine(id).isEmpty() &&
                id.front().first.asString().startsWith(Trait::latin1ToString("^")) &&
                it != delims.cend() &&
                fr.m_data.at(it->m_line).first.length() > it->m_pos + 2 &&
                fr.m_data.at(it->m_line).first[it->m_pos + 1] == Trait::latin1ToChar(':') &&
                fr.m_data.at(it->m_line).first[it->m_pos + 2].isSpace()) {
                f->setIdPos({fr.m_data[delims.cbegin()->m_line].first.virginPos(delims.cbegin()->m_pos),
                             fr.m_data[delims.cbegin()->m_line].second.m_lineNumber,
                             fr.m_data.at(it->m_line).first.virginPos(it->m_pos + 1),
                             fr.m_data.at(it->m_line).second.m_lineNumber});

                {
                    typename MdBlock<Trait>::Data tmp;
                    std::copy(fr.m_data.cbegin() + it->m_line, fr.m_data.cend(),
                        std::back_inserter(tmp));
                    fr.m_data = tmp;
                }

                fr.m_data.front().first = fr.m_data.front().first.sliced(it->m_pos + 3);

                for (auto it = fr.m_data.begin(), last = fr.m_data.end(); it != last; ++it) {
                    if (it->first.asString().startsWith(Trait::latin1ToString("    "))) {
                        it->first = it->first.sliced(4);
                    }
                }

                StringListStream<Trait> stream(fr.m_data);

                parse(stream, f, doc, linksToParse, workingPath, fileName, collectRefLinks);

                if (!f->isEmpty()) {
                    doc->insertFootnote(Trait::latin1ToString("#") + toSingleLine(id) +
                        Trait::latin1ToString("/") + (!workingPath.isEmpty() ?
                            workingPath + Trait::latin1ToString("/") : typename Trait::String()) + fileName,
                        f);
                }
            }
        }
    }
}

template<class Trait>
inline long long int
Parser<Trait>::parseBlockquote(MdBlock<Trait> &fr,
                               std::shared_ptr<Block<Trait>> parent,
                               std::shared_ptr<Document<Trait>> doc,
                               typename Trait::StringList &linksToParse,
                               const typename Trait::String &workingPath,
                               const typename Trait::String &fileName,
                               bool collectRefLinks,
                               RawHtmlBlock<Trait> &)
{
    const long long int pos = fr.m_data.front().first.asString().indexOf(Trait::latin1ToChar('>'));
    long long int extra = 0;

    long long int line = -1;

    if (pos > -1) {
        typename Blockquote<Trait>::Delims delims;

        long long int i = 0, j = 0;

        BlockType bt = BlockType::EmptyLine;

        for (auto it = fr.m_data.begin(), last = fr.m_data.end(); it != last; ++it, ++i) {
            const auto ns = skipSpaces<Trait>(0, it->first.asString());
            const auto gt = (ns < it->first.length() ? (it->first[ns] == Trait::latin1ToChar('>') ? ns : -1) : -1);

            if (gt > -1) {
                const auto dp = it->first.virginPos(gt);
                delims.push_back({dp, it->second.m_lineNumber, dp, it->second.m_lineNumber});

                if (it == fr.m_data.begin()) {
                    extra = gt + (it->first.length() > gt + 1 ?
                        (it->first[gt + 1] == Trait::latin1ToChar(' ') ? 1 : 0) : 0) + 1;
                }

                it->first = it->first.sliced(gt + (it->first.length() > gt + 1 ?
                    (it->first[gt + 1] == Trait::latin1ToChar(' ') ? 1 : 0) : 0) + 1);

                bt = whatIsTheLine(it->first);
            }
            // Process lazyness...
            else {
                if (ns < 4 && isHorizontalLine<Trait>(it->first.asString().sliced(ns))) {
                    line = it->second.m_lineNumber;
                    break;
                }

                const auto tmpBt = whatIsTheLine(it->first);

                if (isListType(tmpBt)) {
                    line = it->second.m_lineNumber;
                    break;
                }

                if (bt == BlockType::Text) {
                    if (isH1<Trait>(it->first.asString())) {
                        const auto p = it->first.asString().indexOf(Trait::latin1ToChar('='));

                        it->first.insert(p, Trait::latin1ToChar('\\'));

                        continue;
                    } else if (isH2<Trait>(it->first.asString())) {
                        const auto p = it->first.asString().indexOf(Trait::latin1ToChar('-'));

                        it->first.insert(p, Trait::latin1ToChar('\\'));

                        continue;
                    }
                } else if ((bt == BlockType::Code || bt == BlockType::CodeIndentedBySpaces) &&
                           it->second.m_mayBreakList) {
                    line = it->second.m_lineNumber;
                    break;
                }

                if ((bt == BlockType::Text || bt == BlockType::Blockquote || bt == BlockType::List)
                    && (tmpBt == BlockType::Text || tmpBt == BlockType::CodeIndentedBySpaces)) {
                    continue;
                } else {
                    line = it->second.m_lineNumber;
                    break;
                }
            }
        }

        typename MdBlock<Trait>::Data tmp;

        for (; j < i; ++j) {
            tmp.push_back(fr.m_data.at(j));
        }

        StringListStream<Trait> stream(tmp);

        std::shared_ptr<Blockquote<Trait>> bq(new Blockquote<Trait>);
        bq->setStartColumn(fr.m_data.at(0).first.virginPos(0) - extra);
        bq->setStartLine(fr.m_data.at(0).second.m_lineNumber);
        bq->setEndColumn(fr.m_data.at(j - 1).first.virginPos(fr.m_data.at(j - 1).first.length() - 1));
        bq->setEndLine(fr.m_data.at(j - 1).second.m_lineNumber);
        bq->delims() = delims;

        parse(stream, bq, doc, linksToParse, workingPath, fileName, collectRefLinks);

        if (!collectRefLinks) {
            parent->appendItem(bq);
        }
    }

    return line;
}

//! \return Is the given string a new list item.
template<class Trait>
inline bool
isListItemAndNotNested(const typename Trait::String &s,
                       long long int indent)
{
    long long int p = skipSpaces<Trait>(0, s);

    if (p >= indent || p == s.size()) {
        return false;
    }

    bool space = false;

    if (p + 1 >= s.size()) {
        space = true;
    } else {
        space = s[p + 1].isSpace();
    }

    if (p < 4) {
        if (s[p] == Trait::latin1ToChar('*') && space) {
            return true;
        } else if (s[p] == Trait::latin1ToChar('-') && space) {
            return true;
        } else if (s[p] == Trait::latin1ToChar('+') && space) {
            return true;
        } else {
            return isOrderedList<Trait>(s);
        }
    } else
        return false;
}

//! \return Indent.
template<class Trait>
inline std::pair<long long int, long long int>
calculateIndent(const typename Trait::String &s,
                long long int p)
{
    return {0, skipSpaces<Trait>(p, s)};
}

//! \return List item data.
template<class Trait>
inline std::tuple<bool, long long int, typename Trait::Char, bool>
listItemData(const typename Trait::String &s,
             bool wasText)
{
    long long int p = skipSpaces<Trait>(0, s);

    if (p == s.size()) {
        return {false, 0, typename Trait::Char(), false};
    }

    bool space = false;

    if (p + 1 >= s.size()) {
        space = true;
    } else {
        space = s[p + 1].isSpace();
    }

    if (p < 4) {
        if (s[p] == Trait::latin1ToChar('*') && space) {
            return {true, p + 2, Trait::latin1ToChar('*'),
                p + 2 < s.size() ? !s.sliced(p + 2).isEmpty() : false};
        } else if (s[p] == Trait::latin1ToChar('-')) {
            if (isH2<Trait>(s) && wasText) {
                return {false, p + 2, Trait::latin1ToChar('-'), false};
            } else if (space) {
                return {true, p + 2, Trait::latin1ToChar('-'),
                    p + 2 < s.size() ? !s.sliced(p + 2).isEmpty() : false};
            }
        } else if (s[p] == Trait::latin1ToChar('+') && space) {
            return {true, p + 2, Trait::latin1ToChar('+'),
                p + 2 < s.size() ? !s.sliced(p + 2).isEmpty() : false};
        } else {
            int d = 0, l = 0;
            typename Trait::Char c;

            if (isOrderedList<Trait>(s, &d, &l, &c)) {
                return {true, p + l + 2, c,
                    p + l + 2 < s.size() ? !s.sliced(p + l + 2).isEmpty() : false};
            } else {
                return {false, 0, typename Trait::Char(), false};
            }
        }
    }

    return {false, 0, typename Trait::Char(), false};
}

//! Set last position of the item.
template<class Trait>
inline void
setLastPos(std::shared_ptr<Item<Trait>> item,
           long long int pos,
           long long int line)
{
    item->setEndColumn(pos);
    item->setEndLine(line);
}

//! Update last position of all parent.
template<class Trait>
inline void
updateLastPosInList(const RawHtmlBlock<Trait> &html)
{
    if (html.m_parent != html.m_topParent) {
        const auto it = html.m_toAdjustLastPos.find(html.m_parent);

        if (it != html.m_toAdjustLastPos.end()) {
            for (auto &i : it->second) {
                i.first->setEndColumn(html.m_html->endColumn());
                i.first->setEndLine(html.m_html->endLine());
            }
        }
    }
}

template<class Trait>
inline long long int
Parser<Trait>::parseList(MdBlock<Trait> &fr,
                         std::shared_ptr<Block<Trait>> parent,
                         std::shared_ptr<Document<Trait>> doc,
                         typename Trait::StringList &linksToParse,
                         const typename Trait::String &workingPath,
                         const typename Trait::String &fileName,
                         bool collectRefLinks,
                         RawHtmlBlock<Trait> &html)
{
    bool resetTopParent = false;
    long long int line = -1;

    if (!html.m_topParent) {
        html.m_topParent = parent;
        resetTopParent = true;
    }

    const auto p = skipSpaces<Trait>(0, fr.m_data.front().first.asString());

    if (p != fr.m_data.front().first.length()) {
        std::shared_ptr<List<Trait>> list(new List<Trait>);

        typename MdBlock<Trait>::Data listItem;
        auto it = fr.m_data.begin();
        listItem.push_back(*it);
        list->setStartColumn(it->first.virginPos(p));
        list->setStartLine(it->second.m_lineNumber);
        ++it;

        long long int indent = 0;
        typename Trait::Char marker;

        std::tie(std::ignore, indent, marker, std::ignore) =
            listItemData<Trait>(listItem.front().first.asString(), false);

        html.m_blocks.push_back({list, list->startColumn() + indent});

        if (!collectRefLinks) {
            html.m_toAdjustLastPos.insert({list, html.m_blocks});
        }

        bool updateIndent = false;

        auto addListMakeNew = [&]() {
            if (!list->isEmpty() && !collectRefLinks) {
                parent->appendItem(list);
            }

            html.m_blocks.pop_back();

            list.reset(new List<Trait>);

            html.m_blocks.push_back({list, indent});

            if (!collectRefLinks) {
                html.m_toAdjustLastPos.insert({list, html.m_blocks});
            }
        };

        auto processLastHtml = [&](std::shared_ptr<ListItem<Trait>> resItem) {
            if (html.m_html && resItem) {
                html.m_parent = (resItem->startLine() == html.m_html->startLine() ||
                    html.m_html->startColumn() >= resItem->startColumn() + indent ?
                        resItem : html.findParent(html.m_html->startColumn()));

                if (!html.m_parent) {
                    html.m_parent = html.m_topParent;
                }

                if (html.m_parent != resItem) {
                    addListMakeNew();
                }

                const auto continueHtml = html.m_onLine && html.m_continueHtml && html.m_parent == html.m_topParent;

                if (!collectRefLinks) {
                    if (!continueHtml) {
                        html.m_parent->appendItem(html.m_html);
                    }

                    updateLastPosInList<Trait>(html);
                }

                if (!continueHtml) {
                    resetHtmlTag<Trait>(html);
                }
            }
        };

        auto processListItem = [&]() {
            MdBlock<Trait> block = {listItem, 0};

            std::shared_ptr<ListItem<Trait>> resItem;

            line = parseListItem(block, list, doc, linksToParse, workingPath, fileName,
                collectRefLinks, html, &resItem);
            listItem.clear();

            if (html.m_html) {
                processLastHtml(resItem);
            } else if (line >= 0) {
                addListMakeNew();
            }
        };

        for (auto last = fr.m_data.end(); it != last; ++it) {
            if (updateIndent) {
                std::tie(std::ignore, indent, marker, std::ignore) =
                    listItemData<Trait>(it->first.asString(), false);

                if (!collectRefLinks) {
                    html.m_blocks.back().second = indent;
                }

                updateIndent = false;
            }

            const auto ns = skipSpaces<Trait>(0, it->first.asString());

            if (isH1<Trait>(it->first.asString().sliced(ns)) && ns < indent && !listItem.empty()) {
                const auto p = it->first.asString().indexOf(Trait::latin1ToChar('='));

                it->first.insert(p, Trait::latin1ToChar('\\'));
            } else if (isHorizontalLine<Trait>(it->first.asString().sliced(ns)) &&
                ns < indent && !listItem.empty()) {
                updateIndent = true;

                processListItem();

                if (!list->isEmpty()) {
                    addListMakeNew();
                }

                if (!collectRefLinks) {
                    makeHorLine<Trait>(*it, parent);
                }

                continue;
            } else if (isListItemAndNotNested<Trait>(it->first.asString(), indent) &&
                !listItem.empty() && !it->second.m_mayBreakList) {
                typename Trait::Char tmpMarker;
                std::tie(std::ignore, indent, tmpMarker, std::ignore) =
                    listItemData<Trait>(it->first.asString(), false);

                processListItem();

                if (tmpMarker != marker) {
                    if (!list->isEmpty()) {
                        addListMakeNew();
                    }

                    marker = tmpMarker;
                }
            }

            if (line > 0) {
                break;
            }

            listItem.push_back(*it);

            if (list->startColumn() == -1) {
                list->setStartColumn(
                    it->first.virginPos(std::min(it->first.length() ?
                        it->first.length() - 1 : 0, skipSpaces<Trait>(0, it->first.asString()))));
                list->setStartLine(it->second.m_lineNumber);

                if (!collectRefLinks) {
                    html.m_blocks.back().second += list->startColumn();
                }
            }
        }

        if (!listItem.empty()) {
            MdBlock<Trait> block = {listItem, 0};
            line = parseListItem(block, list, doc, linksToParse, workingPath, fileName,
                collectRefLinks, html);
        }

        if (!list->isEmpty() && !collectRefLinks) {
            parent->appendItem(list);
        }

        html.m_blocks.pop_back();
    }

    if (resetTopParent) {
        html.m_topParent.reset();
    }

    return line;
}

template<class Trait>
inline long long int
Parser<Trait>::parseListItem(MdBlock<Trait> &fr,
                             std::shared_ptr<Block<Trait>> parent,
                             std::shared_ptr<Document<Trait>> doc,
                             typename Trait::StringList &linksToParse,
                             const typename Trait::String &workingPath,
                             const typename Trait::String &fileName,
                             bool collectRefLinks,
                             RawHtmlBlock<Trait> &html,
                             std::shared_ptr<ListItem<Trait>> *resItem)
{
    {
        const auto it = (std::find_if(fr.m_data.rbegin(), fr.m_data.rend(), [](const auto &s) {
                            return !s.first.isEmpty();
                        })).base();

        if (it != fr.m_data.end()) {
            fr.m_data.erase(it, fr.m_data.end());
        }
    }

    const auto p = skipSpaces<Trait>(0, fr.m_data.front().first.asString());

    std::shared_ptr<ListItem<Trait>> item(new ListItem<Trait>);

    item->setStartColumn(fr.m_data.front().first.virginPos(p));
    item->setStartLine(fr.m_data.front().second.m_lineNumber);

    int i = 0, len = 0;

    if (isOrderedList<Trait>(fr.m_data.front().first.asString(), &i, &len)) {
        item->setListType(ListItem<Trait>::Ordered);
        item->setStartNumber(i);
        item->setDelim({item->startColumn(), item->startLine(), item->startColumn() + len, item->startLine()});
    } else {
        item->setListType(ListItem<Trait>::Unordered);
        item->setDelim({item->startColumn(), item->startLine(), item->startColumn(), item->startLine()});
    }

    if (item->listType() == ListItem<Trait>::Ordered) {
        item->setOrderedListPreState(i == 1 ? ListItem<Trait>::Start : ListItem<Trait>::Continue);
    }

    typename MdBlock<Trait>::Data data;

    auto it = fr.m_data.begin();
    ++it;

    int pos = 1;

    long long int indent = 0;
    bool wasText = false;

    std::tie(std::ignore, indent, std::ignore, wasText) =
        listItemData<Trait>(fr.m_data.front().first.asString(), wasText);

    html.m_blocks.push_back({item, item->startColumn() + indent});

    if (!collectRefLinks) {
        html.m_toAdjustLastPos.insert({item, html.m_blocks});
    }

    const auto firstNonSpacePos = calculateIndent<Trait>(
        fr.m_data.front().first.asString(), indent).second;

    if (firstNonSpacePos - indent < 4) {
        indent = firstNonSpacePos;
    }

    if (indent < fr.m_data.front().first.length()) {
        data.push_back({fr.m_data.front().first.right(fr.m_data.front().first.length() - indent),
            fr.m_data.front().second});
    }

    bool taskList = false;
    bool checked = false;

    if (!data.empty()) {
        auto p = skipSpaces<Trait>(0, data.front().first.asString());

        if (p < data.front().first.length()) {
            if (data.front().first[p] == Trait::latin1ToChar('[')) {
                const auto startTaskDelimPos = data.front().first.virginPos(p);

                ++p;

                if (p < data.front().first.length()) {
                    if (data.front().first[p] == Trait::latin1ToChar(' ') ||
                        data.front().first[p].toLower() == Trait::latin1ToChar('x')) {
                        if (data.front().first[p].toLower() == Trait::latin1ToChar('x')) {
                            checked = true;
                        }

                        ++p;

                        if (p < data.front().first.length()) {
                            if (data.front().first[p] == Trait::latin1ToChar(']')) {
                                item->setTaskDelim({startTaskDelimPos, item->startLine(), data.front().first.virginPos(p), item->startLine()});

                                taskList = true;

                                data[0].first = data[0].first.sliced(p + 1);
                            }
                        }
                    }
                }
            }
        }
    }

    if (taskList) {
        item->setTaskList();
        item->setChecked(checked);
    }

    bool fencedCode = false;
    typename Trait::String startOfCode;
    bool wasEmptyLine = false;

    std::vector<std::pair<RawHtmlBlock<Trait>, long long int>> htmlToAdd;
    long long int line = -1;

    auto parseStream = [&](StringListStream<Trait> &stream) -> long long int
    {
        const auto tmpHtml = html;
        long long int line = -1;
        std::tie(html, line) = parse(stream, item, doc, linksToParse, workingPath, fileName,
                                     collectRefLinks, false, true, true);
        html.m_topParent = tmpHtml.m_topParent;
        html.m_blocks = tmpHtml.m_blocks;
        html.m_toAdjustLastPos = tmpHtml.m_toAdjustLastPos;

        return line;
    };

    auto processHtml = [&](auto it) -> long long int
    {
        auto finishHtml = [&]()
        {
            if (html.m_html) {
                htmlToAdd.push_back({html, html.m_parent->items().size()});
                updateLastPosInList<Trait>(html);
                resetHtmlTag<Trait>(html);
            }
        };

        if (html.m_html.get()) {
            html.m_parent = html.findParent(html.m_html->startColumn());

            if (!html.m_parent) {
                html.m_parent = html.m_topParent;
            }

            data.clear();

            if (html.m_continueHtml) {
                MdBlock<Trait> tmp;
                tmp.m_emptyLineAfter = fr.m_emptyLineAfter;
                tmp.m_emptyLinesBefore = emptyLinesBeforeCount<Trait>(fr.m_data.begin(), it);
                std::copy(it, fr.m_data.end(), std::back_inserter(tmp.m_data));

                const auto line = parseText(tmp, html.m_parent, doc, linksToParse, workingPath, fileName,
                    collectRefLinks, html);

                if (!html.m_continueHtml) {
                    finishHtml();
                }

                return line;
            }

            finishHtml();
        }

        return -2;
    };

    if (processHtml(std::prev(it)) == -2) {
        for (auto last = fr.m_data.end(); it != last; ++it, ++pos) {
            if (!fencedCode) {
                fencedCode = isCodeFences<Trait>(it->first.asString().startsWith(
                    typename Trait::String(indent, Trait::latin1ToChar(' '))) ?
                        it->first.asString().sliced(indent) : it->first.asString());

                if (fencedCode) {
                    startOfCode = startSequence<Trait>(it->first.asString());
                }
            } else if (fencedCode &&
                       isCodeFences<Trait>(it->first.asString().startsWith(
                            typename Trait::String(indent, Trait::latin1ToChar(' '))) ?
                                it->first.asString().sliced(indent) : it->first.asString(),
                            true) && startSequence<Trait>(it->first.asString()).contains(startOfCode)) {
                fencedCode = false;
            }

            if (!fencedCode) {
                long long int newIndent = 0;
                bool ok = false;

                std::tie(ok, newIndent, std::ignore, wasText) = listItemData<Trait>(
                    it->first.asString().startsWith(typename Trait::String(indent, Trait::latin1ToChar(' '))) ?
                        it->first.asString().sliced(indent) : it->first.asString(),
                    wasText);

                if (ok && !it->second.m_mayBreakList) {
                    StringListStream<Trait> stream(data);

                    line = parseStream(stream);

                    data.clear();

                    const auto lineAfterHtml = processHtml(it);

                    if (lineAfterHtml != -2) {
                        if (lineAfterHtml == -1) {
                            break;
                        } else {
                            if (html.m_parent == html.m_topParent) {
                                line = lineAfterHtml;
                            } else {
                                it += (lineAfterHtml - it->second.m_lineNumber);
                            }
                        }
                    }

                    if (line != -1) {
                        break;
                    }

                    if (!htmlToAdd.empty() && htmlToAdd.back().first.m_parent == html.m_topParent) {
                        line = it->second.m_lineNumber;

                        break;
                    } else {
                        typename MdBlock<Trait>::Data nestedList;
                        nestedList.push_back(*it);
                        const auto emptyLinesBefore = emptyLinesBeforeCount<Trait>(fr.m_data.begin(), it);
                        ++it;

                        wasEmptyLine = false;

                        for (; it != last; ++it) {
                            const auto ns = skipSpaces<Trait>(0, it->first.asString());
                            std::tie(ok, std::ignore, std::ignore, wasText) =
                                listItemData<Trait>((ns >= indent ? it->first.asString().sliced(indent) :
                                    it->first.asString()), wasText);

                            if (ok) {
                                wasEmptyLine = false;
                            }

                            auto currentStr = it->first.asString().startsWith(
                                typename Trait::String(indent, Trait::latin1ToChar(' '))) ?
                                    it->first.sliced(indent) : it->first;

                            const auto type = whatIsTheLine(currentStr);

                            bool mayBreak = false;

                            switch (type) {
                            case BlockType::Code:
                            case BlockType::Blockquote:
                            case BlockType::Heading:
                                mayBreak = true;
                                break;

                            default:
                                break;
                            }

                            if (ok || ns >= indent + newIndent || ns == it->first.length() || (!wasEmptyLine && !mayBreak)) {
                                nestedList.push_back(*it);
                            } else {
                                break;
                            }

                            wasEmptyLine = (ns == it->first.length());

                            wasText = (wasEmptyLine ? false : wasText);
                        }

                        for (auto it = nestedList.begin(), last = nestedList.end(); it != last; ++it) {
                            const auto ns = skipSpaces<Trait>(0, it->first.asString());

                            if (ns < indent && ns != it->first.length()) {
                                it->second.m_mayBreakList = true;
                            } else {
                                it->first = it->first.sliced(std::min(ns, indent));
                            }
                        }

                        while (!nestedList.empty() &&
                            nestedList.back().first.asString().isEmpty()) {
                            nestedList.pop_back();
                        }

                        MdBlock<Trait> block = {nestedList, emptyLinesBefore, wasEmptyLine};

                        line = parseList(block, item, doc, linksToParse, workingPath, fileName,
                            collectRefLinks, html);

                        if (line >= 0) {
                            break;
                        }

                        for (; it != last; ++it) {
                            if (it->first.asString().startsWith(typename Trait::String(
                                indent, Trait::latin1ToChar(' ')))) {
                                it->first = it->first.sliced(indent);
                            }

                            data.push_back(*it);
                        }

                        break;
                    }
                } else {
                    if (!it->second.m_mayBreakList &&
                        it->first.asString().startsWith(typename Trait::String(
                            indent, Trait::latin1ToChar(' ')))) {
                            it->first = it->first.sliced(indent);
                    }

                    data.push_back(*it);

                    wasEmptyLine = (skipSpaces<Trait>(0, it->first.asString()) == it->first.length());

                    wasText = !wasEmptyLine;
                }
            } else {
                if (!it->second.m_mayBreakList &&
                    it->first.asString().startsWith(typename Trait::String(
                        indent, Trait::latin1ToChar(' ')))) {
                        it->first = it->first.sliced(indent);
                }

                data.push_back(*it);
            }
        }

        if (!data.empty()) {
            StringListStream<Trait> stream(data);

            line = parseStream(stream);

            if (html.m_html) {
                html.m_parent = html.findParent(html.m_html->startColumn());

                if (!html.m_parent) {
                    html.m_parent = html.m_topParent;
                }
            }
        }
    } else {
        item.reset();
    }

    if (!collectRefLinks) {
        if (item) {
            parent->appendItem(item);
        }

        long long int i = 0;

        for (auto &h : htmlToAdd) {
            if (h.first.m_parent != h.first.m_topParent) {
                h.first.m_parent->insertItem(h.second + i, h.first.m_html);

                ++i;

                updateLastPosInList(h.first);
            } else {
                html = h.first;

                break;
            }
        }

        if (item) {
            long long int htmlStartColumn = -1;
            long long int htmlStartLine = -1;

            if (html.m_html) {
                std::tie(htmlStartColumn, htmlStartLine) =
                    localPosFromVirgin<Trait>(fr, html.m_html->startColumn(), html.m_html->startLine());
            }

            long long int localLine = (html.m_html ? htmlStartLine : fr.m_data.size() - 1);

            if (html.m_html) {
                if (skipSpaces<Trait>(0, fr.m_data[localLine].first.asString()) >= htmlStartColumn) {
                    --localLine;
                }
            }

            const auto lastLine = fr.m_data[localLine].second.m_lineNumber;

            const auto lastColumn = fr.m_data[localLine].first.virginPos(
                fr.m_data[localLine].first.length() ? fr.m_data[localLine].first.length() - 1 : 0);

            item->setEndColumn(lastColumn);
            item->setEndLine(lastLine);
            parent->setEndColumn(lastColumn);
            parent->setEndLine(lastLine);
        }
    }

    if (resItem) {
        *resItem = item;
    }

    html.m_blocks.pop_back();

    return line;
}

template<class Trait>
inline long long int
Parser<Trait>::parseCode(MdBlock<Trait> &fr,
                         std::shared_ptr<Block<Trait>> parent,
                         bool collectRefLinks)
{
    const auto indent = skipSpaces<Trait>(0, fr.m_data.front().first.asString());

    if (indent != fr.m_data.front().first.length()) {
        WithPosition startDelim, endDelim, syntaxPos;
        typename Trait::String syntax;
        isStartOfCode<Trait>(fr.m_data.front().first.asString(), &syntax, &startDelim, &syntaxPos);
        syntax = replaceEntity<Trait>(syntax);
        startDelim.setStartLine(fr.m_data.front().second.m_lineNumber);
        startDelim.setEndLine(startDelim.startLine());
        startDelim.setStartColumn(fr.m_data.front().first.virginPos(startDelim.startColumn()));
        startDelim.setEndColumn(fr.m_data.front().first.virginPos(startDelim.endColumn()));

        if (syntaxPos.startColumn() != -1) {
            syntaxPos.setStartLine(startDelim.startLine());
            syntaxPos.setEndLine(startDelim.startLine());
            syntaxPos.setStartColumn(fr.m_data.front().first.virginPos(syntaxPos.startColumn()));
            syntaxPos.setEndColumn(fr.m_data.front().first.virginPos(syntaxPos.endColumn()));
        }

        const long long int startPos = fr.m_data.front().first.virginPos(indent);
        const long long int emptyColumn = fr.m_data.front().first.virginPos(fr.m_data.front().first.length());
        const long long int startLine = fr.m_data.front().second.m_lineNumber;
        const long long int endPos = fr.m_data.back().first.virginPos(fr.m_data.back().first.length() - 1);
        const long long int endLine = fr.m_data.back().second.m_lineNumber;

        fr.m_data.erase(fr.m_data.cbegin());

        {
            const auto it = std::prev(fr.m_data.cend());

            if (it->second.m_lineNumber > -1) {
                endDelim.setStartColumn(it->first.virginPos(skipSpaces<Trait>(0, it->first.asString())));
                endDelim.setStartLine(it->second.m_lineNumber);
                endDelim.setEndLine(endDelim.startLine());
                endDelim.setEndColumn(it->first.virginPos(it->first.length() - 1));
            }

            fr.m_data.erase(it);
        }

        if (syntax.toLower() == Trait::latin1ToString("math")) {
            typename Trait::String math;
            bool first = true;

            for (const auto &l : std::as_const(fr.m_data)) {
                if (!first) {
                    math.push_back(Trait::latin1ToChar('\n'));
                }

                math.push_back(l.first.virginSubString());

                first = false;
            }

            if (!collectRefLinks) {
                std::shared_ptr<Paragraph<Trait>> p(new Paragraph<Trait>);
                p->setStartColumn(startPos);
                p->setStartLine(startLine);
                p->setEndColumn(endPos);
                p->setEndLine(endLine);

                std::shared_ptr<Math<Trait>> m(new Math<Trait>);

                if (!fr.m_data.empty()) {
                    m->setStartColumn(fr.m_data.front().first.virginPos(0));
                    m->setStartLine(fr.m_data.front().second.m_lineNumber);
                    m->setEndColumn(fr.m_data.back().first.virginPos(fr.m_data.back().first.length() - 1));
                    m->setEndLine(fr.m_data.back().second.m_lineNumber);
                } else {
                    m->setStartColumn(emptyColumn);
                    m->setStartLine(startLine);
                    m->setEndColumn(emptyColumn);
                    m->setEndLine(startLine);
                }

                m->setInline(false);
                m->setExpr(math);
                m->setStartDelim(startDelim);
                m->setEndDelim(endDelim);
                m->setSyntaxPos(syntaxPos);
                m->setFensedCode(true);
                p->appendItem(m);

                parent->appendItem(p);
            }
        } else {
            return parseCodeIndentedBySpaces(fr, parent, collectRefLinks, indent, syntax, emptyColumn,
                                      startLine, true, startDelim, endDelim, syntaxPos);
        }
    }

    return -1;
}

template<class Trait>
inline long long int
Parser<Trait>::parseCodeIndentedBySpaces(MdBlock<Trait> &fr,
                                         std::shared_ptr<Block<Trait>> parent,
                                         bool collectRefLinks,
                                         int indent,
                                         const typename Trait::String &syntax,
                                         long long int emptyColumn,
                                         long long int startLine,
                                         bool fensedCode,
                                         const WithPosition &startDelim,
                                         const WithPosition &endDelim,
                                         const WithPosition &syntaxPos)
{
    typename Trait::String code;
    long long int startPos = 0;
    bool first = true;

    auto it = fr.m_data.begin(), lastIt = fr.m_data.end();

    for (; it != lastIt; ++it) {
        if (it->second.m_mayBreakList) {
            lastIt = it;
            break;
        }

        if (!collectRefLinks) {
            const auto ns = skipSpaces<Trait>(0, it->first.asString());
            if (first) {
                startPos = ns;
            }
            first = false;

            code.push_back((indent > 0 ? it->first.virginSubString(ns < indent ? ns : indent) +
                    typename Trait::String(Trait::latin1ToChar('\n')) :
                typename Trait::String(it->first.virginSubString()) +
                    typename Trait::String(Trait::latin1ToChar('\n'))));
        }
    }

    if (!collectRefLinks) {
        if (!code.isEmpty()) {
            code.remove(code.length() - 1, 1);
        }

        std::shared_ptr<Code<Trait>> codeItem(new Code<Trait>(code, fensedCode, false));
        codeItem->setSyntax(syntax);
        codeItem->setStartDelim(startDelim);
        codeItem->setEndDelim(endDelim);
        codeItem->setSyntaxPos(syntaxPos);

        if (lastIt != fr.m_data.end() || (it == fr.m_data.end() && !fr.m_data.empty())) {
            codeItem->setStartColumn(fr.m_data.front().first.virginPos(startPos));
            codeItem->setStartLine(fr.m_data.front().second.m_lineNumber);
            auto tmp = std::prev(lastIt);
            codeItem->setEndColumn(tmp->first.virginPos(tmp->first.length() - 1));
            codeItem->setEndLine(tmp->second.m_lineNumber);
        } else {
            codeItem->setStartColumn(emptyColumn);
            codeItem->setStartLine(startLine);
            codeItem->setEndColumn(emptyColumn);
            codeItem->setEndLine(startLine);
        }

        if (fensedCode) {
            parent->appendItem(codeItem);
        } else if (!parent->items().empty() && parent->items().back()->type() == ItemType::Code) {
            auto c = std::static_pointer_cast<Code<Trait>>(parent->items().back());

            if (!c->isFensedCode()) {
                auto line = c->endLine();
                auto text = c->text();

                for (; line < codeItem->startLine(); ++line) {
                    text.push_back(Trait::latin1ToString("\n"));
                }

                text.push_back(codeItem->text());
                c->setText(text);
                c->setEndColumn(codeItem->endColumn());
                c->setEndLine(codeItem->endLine());
            } else {
                parent->appendItem(codeItem);
            }
        } else {
            parent->appendItem(codeItem);
        }
    }

    if (lastIt != fr.m_data.end()) {
        return lastIt->second.m_lineNumber;
    }

    return -1;
}

} /* namespace MD */

#endif // MD4QT_MD_PARSER_HPP_INCLUDED
