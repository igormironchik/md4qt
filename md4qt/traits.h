/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef MD4QT_MD_TRAITS_HPP_INCLUDED
#define MD4QT_MD_TRAITS_HPP_INCLUDED

#ifdef MD4QT_ICU_STL_SUPPORT

// C++ include.
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// ICU include.
#include <unicode/uchar.h>
#include <unicode/unistr.h>

// uriparser include.
#include <uriparser/Uri.h>

#endif // MD4QT_ICU_STL_SUPPORT

#ifdef MD4QT_QT_SUPPORT

#ifndef MD4QT_ICU_STL_SUPPORT

// C++ include.
#include <map>
#include <memory>

#endif // MD4QT_ICU_STL_SUPPORT

// Qt include.
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QUrl>
#include <QVector>

#endif // MD4QT_QT_SUPPORT

namespace MD
{

//! Internal string, used to get virgin (original) string from transformed string.
template<class String, class Char, class Latin1Char>
class InternalStringT
{
public:
    InternalStringT()
    {
    }
    InternalStringT(const String &s)
        : m_str(s)
        , m_virginStr(s)
    {
    }

    //! \return Reference to string.
    String &asString()
    {
        return m_str;
    }

    //! \return Reference to string.
    const String &asString() const
    {
        return m_str;
    }

    //! \return Full virgin string.
    const String & fullVirginString() const
    {
        return m_virginStr;
    }

    //! \return Virgin sub-string with position and length in the transformed string.
    String virginSubString(long long int pos = 0, long long int len = -1) const
    {
        if (pos < 0) {
            pos = 0;
        }

        if (pos + len > m_str.length() || len < 0) {
            len = m_str.length() - pos;
        }

        if (len == 0) {
            return (m_str.isEmpty() ? m_virginStr : String());
        }

        auto virginStartPos = virginPos(pos);
        String startStr, endStr;

        if (m_virginStr[virginStartPos] == Latin1Char('\t')) {
            const auto spaces = countOfSpacesForTab(virginStartPos);

            for (long long int i = 1; i < spaces; ++i) {
                if (virginPos(pos + i) != virginStartPos) {
                    startStr = String(i, Latin1Char(' '));
                    ++virginStartPos;
                    break;
                }
            }
        }

        auto virginEndPos = virginPos(pos + len - 1, true);

        if (m_virginStr[virginEndPos] == Latin1Char('\t')) {
            const auto spaces = countOfSpacesForTab(virginEndPos);

            for (long long int i = 1; i < spaces; ++i) {
                if (virginPos(pos + len - 1 - i) != virginEndPos) {
                    endStr = String(i, Latin1Char(' '));
                    --virginEndPos;
                    break;
                }
            }
        }

        return startStr + m_virginStr.sliced(virginStartPos, virginEndPos - virginStartPos + 1) + endStr;
    }

    //! \return Virgin position from transformed.
    long long int virginPos(
            //! Transformed position.
            long long int pos,
            //! If true will be return last virgin position before transformation.
            //! For example if in virgin string 2 characters were replaced with 1,
            //! we will receive position of second character if \p end is true.
            bool end = false) const
    {
        for (auto it = m_changedPos.crbegin(), last = m_changedPos.crend(); it != last; ++it) {
            pos = virginPosImpl(pos, *it, end);
        }

        return pos;
    }

    Char operator[](long long int position) const
    {
        return m_str[position];
    }

    //! Replace substring.
    InternalStringT &replaceOne(long long int pos, long long int size, const String &with)
    {
        const auto len = m_str.length();

        m_str.remove(pos, size);
        m_str.insert(pos, with);

        if (with.length() != size) {
            m_changedPos.push_back({{0, len}, {}});
            m_changedPos.back().second.push_back({pos, size, with.size()});
        }

        return *this;
    }

    //! Replace string.
    InternalStringT &replace(const String &what, const String &with)
    {
        String tmp;
        bool init = false;
        const auto len = m_str.length();

        for (long long int i = 0; i < m_str.size();) {
            long long int p = m_str.indexOf(what, i);

            if (p != -1) {
                tmp.push_back(m_str.sliced(i, p - i));
                tmp.push_back(with);

                i = p + what.size();

                if (what.size() != with.size()) {
                    if (!init) {
                        m_changedPos.push_back({{0, len}, {}});
                        init = true;
                    }

                    m_changedPos.back().second.push_back({p, what.size(), with.size()});
                }
            } else {
                tmp.push_back(m_str.sliced(i));

                i = m_str.size();
            }
        }

        std::swap(m_str, tmp);

        return *this;
    }

    //! Remove sub-string.
    InternalStringT &remove(long long int pos, long long int size)
    {
        const auto len = m_str.length();

        m_str.remove(pos, size);

        m_changedPos.push_back({{0, len}, {}});
        m_changedPos.back().second.push_back({pos, size, 0});

        return *this;
    }

    //! \return Is this string empty?
    bool isEmpty() const
    {
        return m_str.isEmpty();
    }

    //! \return Length of the string.
    long long int length() const
    {
        return m_str.length();
    }

    //! \return Simplified string.
    InternalStringT simplified() const
    {
        if (isEmpty()) {
            return *this;
        }

        const auto len = m_str.length();

        InternalStringT result = *this;
        result.m_str.clear();
        long long int i = 0;
        bool init = false;
        bool first = true;
        long long int spaces = 0;

        while (true) {
            long long tmp = i;

            while (i < length() && m_str[i].isSpace()) {
                ++i;
            }

            spaces = i - tmp;

            if (i != tmp) {
                if (!init) {
                    result.m_changedPos.push_back({{0, len}, {}});
                    init = true;
                }

                if (i - tmp > 1 || first) {
                    result.m_changedPos.back().second.push_back({tmp, i - tmp, (first ? 0 : 1)});
                }
            }

            first = false;

            while (i != length() && !m_str[i].isSpace()) {
                result.m_str.push_back(m_str[i]);
                ++i;
            }

            if (i == length()) {
                break;
            }

            result.m_str.push_back(Latin1Char(' '));
        }

        if (!result.isEmpty() && result.m_str[result.length() - 1] == Latin1Char(' ')) {
            result.m_str.remove(result.length() - 1, 1);

            if (spaces > 1) {
                result.m_changedPos.back().second.back().m_len = 0;
            } else if (spaces == 1) {
                result.m_changedPos.back().second.push_back({m_str.length() - spaces, spaces, 0});
            }
        }

        return result;
    }

    //! Split string.
    std::vector<InternalStringT> split(const InternalStringT &sep) const
    {
        std::vector<InternalStringT> result;
        const auto len = m_str.length();

        if (sep.isEmpty()) {
            for (long long int i = 0; i < m_str.length(); ++i) {
                auto is = *this;
                is.m_str = m_str[i];
                is.m_changedPos.push_back({{i, len}, {}});

                result.push_back(is);
            }

            return result;
        }

        long long int pos = 0;
        long long int fpos = 0;

        while ((fpos = m_str.indexOf(sep.asString(), pos)) != -1 && fpos < length()) {
            if (fpos - pos > 0) {
                auto is = *this;
                is.m_str = m_str.sliced(pos, fpos - pos);
                is.m_changedPos.push_back({{pos, len}, {}});

                result.push_back(is);
            }

            pos = fpos + sep.length();
        }

        if (pos < m_str.length()) {
            auto is = *this;
            is.m_str = m_str.sliced(pos, m_str.length() - pos);
            is.m_changedPos.push_back({{pos, len}, {}});

            result.push_back(is);
        }

        return result;
    }

    //! \return Sliced sub-string.
    InternalStringT sliced(long long int pos, long long int len = -1) const
    {
        InternalStringT tmp = *this;
        const auto oldLen = m_str.length();
        tmp.m_str = tmp.m_str.sliced(pos, (len == -1 ? tmp.m_str.length() - pos : len));
        tmp.m_changedPos.push_back({{pos, oldLen}, {}});
        if (len != -1 && len < length() - pos) {
            tmp.m_changedPos.back().second.push_back({pos + len, length() - pos - len, 0});
        }

        return tmp;
    }

    //! \return Right sub-string.
    InternalStringT right(long long int n) const
    {
        InternalStringT tmp = *this;
        const auto len = m_str.length();
        tmp.m_str = tmp.m_str.right(n);
        tmp.m_changedPos.push_back({{length() - n, len}, {}});

        return tmp;
    }

    //! Insert one character.
    InternalStringT &insert(long long int pos, Char ch)
    {
        return insert(pos, String(1, ch));
    }

    //! Insert string.
    InternalStringT &insert(long long int pos, const String &s)
    {
        const auto len = m_str.length();
        const auto ilen = s.length();

        m_str.insert(pos, s);

        m_changedPos.push_back({{0, len}, {}});
        m_changedPos.back().second.push_back({pos, 1, ilen + 1});

        return *this;
    }

private:
    //! Transformed string.
    String m_str;
    //! Virgin (original) string.
    String m_virginStr;

    //! Auxiliary struct to store information about transformation.
    struct ChangedPos {
        long long int m_pos = -1;
        long long int m_oldLen = -1;
        long long int m_len = -1;
    };

    //! Auxiliary struct to store information about transformation.
    struct LengthAndStartPos {
        long long int m_firstPos = 0;
        long long int m_length = 0;
    };

    //! Information about transformations.
    std::vector<std::pair<LengthAndStartPos, std::vector<ChangedPos>>> m_changedPos;

private:
    long long int virginPosImpl(long long int pos,
                                const std::pair<LengthAndStartPos, std::vector<ChangedPos>> &changed,
                                bool end) const
    {
        for (const auto &c : changed.second) {
            const auto startPos = c.m_pos;
            const auto endPos = startPos + c.m_len - 1;

            if (pos >= startPos && pos <= endPos) {
                const auto oldEndPos = startPos + c.m_oldLen - 1;

                if (pos > oldEndPos || end) {
                    return oldEndPos + changed.first.m_firstPos;
                } else {
                    return pos + changed.first.m_firstPos;
                }
            } else if (pos > endPos) {
                pos += c.m_oldLen - c.m_len;
            } else {
                break;
            }
        }

        pos += changed.first.m_firstPos;

        return (pos > changed.first.m_length ? changed.first.m_length : pos);
    }

    long long int countOfSpacesForTab(long long int virginPos) const
    {
        long long int p = 0;

        for (const auto &v : std::as_const(m_changedPos)) {
            p += v.first.m_firstPos;

            if (virginPos < p) {
                break;
            }

            for (const auto &c : std::as_const(v.second)) {
                if (c.m_pos + p == virginPos) {
                    return c.m_len;
                }

                virginPos += (virginPos > c.m_pos ? c.m_len - c.m_oldLen : 0);
            }
        }

        return -1;
    }
}; // class InternalString

#ifdef MD4QT_ICU_STL_SUPPORT

//
// UnicodeChar
//

//! Wrapper for UChar32 to be used with MD::Parser.
class UnicodeChar
{
public:
    UnicodeChar()
        : m_ch(0)
    {
    }

    UnicodeChar(UChar32 ch)
        : m_ch(ch)
    {
    }

    operator UChar32() const
    {
        return m_ch;
    }

    inline bool isSpace() const
    {
        bool unicodeSpace = false;

        const auto type = u_charType(m_ch);

        switch (type) {
        case U_SPACE_SEPARATOR:
        case U_LINE_SEPARATOR:
        case U_PARAGRAPH_SEPARATOR:
            unicodeSpace = true;
            break;

        default:
            break;
        }

        return m_ch == 0x20 || (m_ch <= 0x0D && m_ch >= 0x09) ||
               (m_ch > 127 && (m_ch == 0x85 || m_ch == 0xA0 || unicodeSpace));
    }

    inline bool isDigit() const
    {
        return (u_charType(m_ch) == U_DECIMAL_DIGIT_NUMBER);
    }

    inline bool isNull() const
    {
        return m_ch == 0;
    }

    inline UChar32 unicode() const
    {
        return m_ch;
    }

    inline bool isLetter() const
    {
        const auto type = u_charType(m_ch);

        switch (type) {
        case U_UPPERCASE_LETTER:
        case U_LOWERCASE_LETTER:
        case U_TITLECASE_LETTER:
        case U_MODIFIER_LETTER:
        case U_OTHER_LETTER:
            return true;

        default:
            return false;
        }
    }

    inline bool isLetterOrNumber() const
    {
        return isLetter() || isDigit();
    }

    inline bool isPunct() const
    {
        const auto type = u_charType(m_ch);

        switch (type) {
        case U_DASH_PUNCTUATION:
        case U_START_PUNCTUATION:
        case U_END_PUNCTUATION:
        case U_CONNECTOR_PUNCTUATION:
        case U_OTHER_PUNCTUATION:
        case U_INITIAL_PUNCTUATION:
        case U_FINAL_PUNCTUATION:
            return true;

        default:
            return false;
        }
    }

    inline bool isSymbol() const
    {
        const auto type = u_charType(m_ch);

        switch (type) {
        case U_MATH_SYMBOL:
        case U_CURRENCY_SYMBOL:
        case U_MODIFIER_SYMBOL:
        case U_OTHER_SYMBOL:
            return true;

        default:
            return false;
        }
    }

    UnicodeChar toLower() const
    {
        return icu::UnicodeString(1, m_ch, 1).toLower().char32At(0);
    }

    bool operator==(const UnicodeChar &other) const
    {
        return m_ch == other.m_ch;
    }

    bool operator!=(const UnicodeChar &other) const
    {
        return m_ch != other.m_ch;
    }

private:
    UChar32 m_ch;
}; // class UnicodeChar

//
// UnicodeString
//

//! Wrapper for icu::UnicodeString to be used with MD::Parser.
class UnicodeString final : public icu::UnicodeString
{
public:
    UnicodeString()
    {
    }

    UnicodeString(const icu::UnicodeString &str)
        : icu::UnicodeString(str)
    {
    }

    UnicodeString(char ch)
        : icu::UnicodeString((char16_t)ch)
    {
    }

    UnicodeString(const char16_t *str)
        : icu::UnicodeString(str)
    {
    }

    UnicodeString(const UnicodeChar &ch)
        : icu::UnicodeString(1, (UChar32)ch, 1)
    {
    }

    UnicodeString(const char *str)
        : icu::UnicodeString(icu::UnicodeString::fromUTF8(str))
    {
    }

    UnicodeString(const std::string &str)
        : icu::UnicodeString(icu::UnicodeString::fromUTF8(str))
    {
    }

    UnicodeString(long long int count, char ch)
        : icu::UnicodeString((int32_t)count, (UChar32)ch, (int32_t)count)
    {
    }

    ~UnicodeString() override = default;

    UnicodeChar operator[](long long int position) const
    {
        return UnicodeChar(char32At((int32_t)position));
    }

    void push_back(const UnicodeChar &ch)
    {
        icu::UnicodeString::append((UChar32)ch);
    }

    void push_back(const UnicodeString &str)
    {
        icu::UnicodeString::append(str);
    }

    int32_t size() const
    {
        return length();
    }

    int toInt(bool *ok = nullptr, int base = 10) const
    {
        try {
            std::string tmp;
            toUTF8String(tmp);
            const auto result = std::stoi(tmp, nullptr, base);
            if (ok) {
                *ok = true;
            }
            return result;
        } catch (const std::invalid_argument &) {
            if (ok) {
                *ok = false;
            }
        } catch (const std::out_of_range &) {
            if (ok) {
                *ok = false;
            }
        }

        return 0;
    }

    bool contains(const UnicodeChar &ch) const
    {
        return (icu::UnicodeString::indexOf((UChar32)ch) != -1);
    }

    bool contains(const UnicodeString &str) const
    {
        return (icu::UnicodeString::indexOf(str) != -1);
    }

    UnicodeString simplified() const
    {
        if (isEmpty()) {
            return *this;
        }

        UnicodeString result;
        int32_t i = 0;

        while (true) {
            while (i < length() && UnicodeChar(char32At(i)).isSpace()) {
                ++i;
            }

            while (i != length() && !UnicodeChar(char32At(i)).isSpace()) {
                result.append(UnicodeChar(char32At(i)));
                ++i;
            }

            if (i == length()) {
                break;
            }

            result.append(UnicodeChar(' '));
        }

        if (!result.isEmpty() && result[result.size() - 1] == UnicodeChar(' ')) {
            result.remove(result.size() - 1, 1);
        }

        return result;
    }

    std::vector<UnicodeString> split(const UnicodeChar &ch) const
    {
        std::vector<UnicodeString> result;

        int32_t pos = 0;
        int32_t fpos = 0;

        while ((fpos = indexOf(ch, pos)) != -1 && fpos < length()) {
            if (fpos - pos > 0) {
                icu::UnicodeString tmp;
                extract(pos, fpos - pos, tmp);
                result.push_back(tmp);
            }

            pos = fpos + 1;
        }

        if (pos < length()) {
            icu::UnicodeString tmp;
            extract(pos, length() - pos, tmp);
            result.push_back(tmp);
        }

        return result;
    }

    std::vector<UnicodeString> split(char ch) const
    {
        return split(UnicodeChar(ch));
    }

    UnicodeString &replace(const UnicodeChar &before, const UnicodeString &after)
    {
        for (int32_t pos = 0; (pos = indexOf(before, pos)) != -1; pos += after.size()) {
            icu::UnicodeString::replace(pos, 1, after);
        }

        return *this;
    }

    UnicodeString &replace(const UnicodeString &before, const UnicodeString &after)
    {
        for (int32_t pos = 0; (pos = indexOf(before, pos)) != -1; pos += after.size()) {
            icu::UnicodeString::replace(pos, before.length(), after);
        }

        return *this;
    }

    UnicodeString sliced(long long int pos, long long int len = -1) const
    {
        icu::UnicodeString tmp;
        extract((int32_t)pos, (int32_t)(len == -1 ? length() - pos : len), tmp);

        return tmp;
    }

    UnicodeString right(long long int n) const
    {
        icu::UnicodeString tmp;
        extract(length() - (int32_t)n, (int32_t)n, tmp);

        return tmp;
    }

    UnicodeString toCaseFolded() const
    {
        icu::UnicodeString tmp = *this;
        tmp.foldCase();

        return tmp;
    }

    UnicodeString toUpper() const
    {
        icu::UnicodeString tmp = *this;
        tmp.toUpper();

        return tmp;
    }

    UnicodeString toLower() const
    {
        icu::UnicodeString tmp = *this;
        tmp.toLower();

        return tmp;
    }

    void clear()
    {
        icu::UnicodeString::remove();
    }
}; // class UnicodeString

//
// UrlUri
//

class UrlUri
{
public:
    explicit UrlUri(const UnicodeString &uriStr)
        : m_valid(false)
        , m_relative(false)
    {
        UriUriA uri;
        std::string uriString;
        uriStr.toUTF8String(uriString);

        if (uriParseSingleUriA(&uri, uriString.c_str(), NULL) == URI_SUCCESS) {
            m_valid = true;
            m_relative = !(uri.scheme.first && uri.scheme.afterLast);

            if (!m_relative) {
                m_scheme = UnicodeString(std::string(uri.scheme.first,
                                                     uri.scheme.afterLast - uri.scheme.first).c_str());
            }

            if (uri.hostText.first && uri.hostText.afterLast) {
                m_host = UnicodeString(std::string(uri.hostText.first,
                                                   uri.hostText.afterLast - uri.hostText.first).c_str());
            }

            uriFreeUriMembersA(&uri);
        }
    }

    ~UrlUri()
    {
    }

    bool isValid() const
    {
        return m_valid;
    }

    bool isRelative() const
    {
        return m_relative;
    }

    UnicodeString scheme() const
    {
        return m_scheme;
    }

    UnicodeString host() const
    {
        return m_host;
    }

private:
    bool m_valid;
    bool m_relative;
    UnicodeString m_scheme;
    UnicodeString m_host;
}; // class UrlUri

//
// UnicodeStringTrait
//

//! Trait to use this library with std::string.
struct UnicodeStringTrait {
    template<class T>
    using Vector = std::vector<T>;

    template<class T, class U>
    using Map = std::map<T, U>;

    using String = UnicodeString;

    using Char = UnicodeChar;

    using InternalString = InternalStringT<String, Char, Char>;

    using TextStream = std::istream;

    using StringList = std::vector<String>;

    using InternalStringList = std::vector<InternalString>;

    using Url = UrlUri;

    //! \return Is Unicode whitespace?
    static bool isUnicodeWhitespace(const UnicodeChar &ch)
    {
        const auto c = ch.unicode();

        if (u_charType(c) == U_SPACE_SEPARATOR) {
            return true;
        } else if (c == 0x09 || c == 0x0A || c == 0x0C || c == 0x0D) {
            return true;
        } else {
            return false;
        }
    }

    //! Convert UTF-16 into trait's string.
    static String utf16ToString(const char16_t *u16)
    {
        return UnicodeString(u16);
    }

    //! Convert Latin1 into trait's string.
    static String latin1ToString(const char *latin1)
    {
        return UnicodeString(latin1);
    }

    //! Convert Latin1 char into trait's char.
    static Char latin1ToChar(char latin1)
    {
        return UnicodeChar(latin1);
    }

    //! Convert UTF8 into trait's string.
    static String utf8ToString(const char *utf8)
    {
        return UnicodeString(utf8);
    }

    //! \return Does file exist.
    static bool fileExists(const String &fileName, const String &workingPath)
    {
        std::string path;
        (workingPath.isEmpty() ? fileName : String(workingPath + "/" + fileName)).toUTF8String(path);

        std::error_code er;

        const auto result = std::filesystem::exists(path, er);

        return (er ? false : result);
    }

    //! \return Does file exist.
    static bool fileExists(const String &fileName)
    {
        std::string path;
        fileName.toUTF8String(path);

        std::error_code er;

        const auto result = std::filesystem::exists(path, er);

        return (er ? false : result);
    }

    //! \return Absolute file path.
    static String absoluteFilePath(const String &path)
    {
        std::string tmp;
        path.toUTF8String(tmp);
        std::error_code er;
        auto p = std::filesystem::canonical(tmp, er).u8string();

        std::replace(p.begin(), p.end(), '\\', '/');

        return (er ? "" : UnicodeString::fromUTF8(p));
    }

    //! Add UCS4 to string.
    static void appendUcs4(String &str, char32_t ch)
    {
        str.push_back(Char(ch));
    }

    //! Search for last occurrence of string.
    static long long int lastIndexOf(const String &where, const String &what, long long int from)
    {
        if (from < 0) {
            return -1;
        } else {
            return where.lastIndexOf(what, 0, from + 1);
        }
    }
}; // struct UnicodeStringTrait

#endif // MD4QT_ICU_STL_SUPPORT

#ifdef MD4QT_QT_SUPPORT

//
// QStringTrait
//

//! Trait to use this library with QString.
struct QStringTrait {
    template<class T>
    using Vector = QVector<T>;

    template<class T, class U>
    using Map = std::map<T, U>;

    using String = QString;

    using Char = QChar;

    using InternalString = InternalStringT<String, Char, QLatin1Char>;

    using InternalStringList = std::vector<InternalString>;

    using TextStream = QTextStream;

    using StringList = QStringList;

    using Url = QUrl;

    //! \return Is Unicode whitespace?
    static bool isUnicodeWhitespace(const QChar &ch)
    {
        const auto c = ch.unicode();

        if (ch.category() == QChar::Separator_Space) {
            return true;
        } else if (c == 0x09 || c == 0x0A || c == 0x0C || c == 0x0D) {
            return true;
        } else {
            return false;
        }
    }

    //! Convert UTF-16 into trait's string.
    static String utf16ToString(const char16_t *u16)
    {
        return QString::fromUtf16(u16);
    }

    //! Convert Latin1 into trait's string.
    static String latin1ToString(const char *latin1)
    {
        return QLatin1String(latin1);
    }

    //! Convert Latin1 char into trait's char.
    static Char latin1ToChar(char latin1)
    {
        return QLatin1Char(latin1);
    }

    //! Convert UTF8 into trait's string.
    static String utf8ToString(const char *utf8)
    {
        return QString::fromUtf8(utf8, -1);
    }

    //! \return Does file exist.
    static bool fileExists(const String &fileName, const String &workingPath)
    {
        return QFileInfo::exists((workingPath.isEmpty() ?
                                    QString() : workingPath + latin1ToString("/")) + fileName);
    }

    //! \return Does file exist.
    static bool fileExists(const String &fileName)
    {
        return QFileInfo::exists(fileName);
    }

    //! \return Absolute file path.
    static String absoluteFilePath(const String &path)
    {
        return QFileInfo(path).absoluteFilePath();
    }

    //! Add UCS4 to string.
    static void appendUcs4(String &str, char32_t ch)
    {
        str += QChar::fromUcs4(ch);
    }

    //! Search for last occurrence of string.
    static long long int lastIndexOf(const String &where, const String &what, long long int from)
    {
        if (from < 0) {
            return -1;
        } else {
            return where.lastIndexOf(what, from);
        }
    }
}; // struct QStringTrait

#endif // MD4QT_QT_SUPPORT

} /* namespace MD */

#endif // MD4QT_MD_TRAITS_HPP_INCLUDED
