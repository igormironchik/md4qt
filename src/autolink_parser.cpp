/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// md4qt include.
#include "autolink_parser.h"
#include "constants.h"
#include "inline_context.h"
#include "reverse_solidus.h"
#include "text_stream.h"
#include "utils.h"

namespace MD
{

QString readURI(Line &line,
                ParagraphStream &stream)
{
    QString url;

    while (true) {
        ReverseSolidusHandler reverseSolidus;

        while (line.position() < line.length()) {
            url.append(line.currentChar());

            if (reverseSolidus.isNotEscaped(line.currentChar())) {
                if (isAsciiControl(line.currentChar())) {
                    return QString();
                } else if (line.currentChar() == s_greaterSignChar) {
                    line.nextChar();

                    url.removeLast();

                    return url;
                } else if (line.currentChar() == s_lessSignChar) {
                    return QString();
                }
            }

            line.nextChar();
            reverseSolidus.next();
        }

        if (!stream.atEnd()) {
            line = stream.readLine();
        } else {
            break;
        }
    }

    return QString();
}

bool AutolinkParser::check(Line &line,
                           ParagraphStream &stream,
                           InlineContext &ctx,
                           QSharedPointer<Document>,
                           const QString &,
                           const QString &,
                           QStringList &,
                           Parser &,
                           const ReverseSolidusHandler &rs)
{
    if (line.currentChar() == s_lessSignChar && !rs.isPrevReverseSolidus()) {
        const auto sState = stream.currentState();
        const auto lState = line.currentState();

        const auto startPos = line.position();
        const auto startLine = line.lineNumber();

        line.nextChar();

        const auto uri = readURI(line, stream);

        if (!isValidUrl(uri) && !isEmail(uri)) {
            stream.restoreStateBefore(sState);
            line = stream.readLine();
            line.restoreState(&lState);

            return false;
        }

        auto lnk = QSharedPointer<Link>::create();
        lnk->setStartColumn(startPos);
        lnk->setStartLine(startLine);
        lnk->setEndColumn(line.position() - 1);
        lnk->setEndLine(line.lineNumber());
        lnk->setUrl(uri);
        lnk->setTextPos({startPos + 1, startLine, line.position() - 2, line.lineNumber()});
        lnk->setUrlPos(lnk->textPos());
        ctx.inlines().append(lnk);

        return true;
    }

    return false;
}

QString AutolinkParser::startDelimiterSymbols() const
{
    return QStringLiteral("<");
}

} /* namespace MD */
