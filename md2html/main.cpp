
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// md4qt include.
#define MD4QT_QT_SUPPORT
#include <md4qt/html.h>
#include <md4qt/parser.h>

// Qt include.
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QCommandLineParser argParser;
    argParser.setApplicationDescription(QStringLiteral("Converter of Markdown to HTML."));
    argParser.addHelpOption();
    QCommandLineOption markdownArg(QStringList() << QStringLiteral("m") << QStringLiteral("markdown"),
                                   QStringLiteral("Markdown file to convert to HTML."),
                                   QStringLiteral("md"));
    QCommandLineOption htmlArg(QStringList() << QStringLiteral("o") << QStringLiteral("html"),
                               QStringLiteral("Output HTML file name."),
                               QStringLiteral("html"));
    QCommandLineOption recursiveArg(QStringList() << QStringLiteral("r") << QStringLiteral("recursive"), QStringLiteral("Read all linked Markdown files?"));
    argParser.addOption(markdownArg);
    argParser.addOption(htmlArg);
    argParser.addOption(recursiveArg);

    argParser.process(app);

    const QString markdownFileName = argParser.value(QStringLiteral("m"));
    const QString htmlFileName = argParser.value(QStringLiteral("o"));
    const bool recursive = argParser.isSet(QStringLiteral("r"));

    QTextStream outStream(stdout);

    QFileInfo mdFileInfo(markdownFileName);

    if (mdFileInfo.exists()) {
        if (mdFileInfo.suffix() == QStringLiteral("md") || mdFileInfo.suffix() == QStringLiteral("markdown")) {
            MD::Parser<MD::QStringTrait> parser;

            const auto doc = parser.parse(markdownFileName, recursive);

            QFile html(htmlFileName);

            if (html.open(QIODevice::WriteOnly)) {
                const auto content = MD::toHtml(doc);

                html.write(content.toUtf8());

                html.close();
            } else {
                outStream << "Unable to write output HTML file.\n";

                return 1;
            }
        } else {
            outStream << "Wrong file suffix of Markdown file (supported *.md, *.markdown).\n";

            return 1;
        }
    } else {
        outStream << "Input Markdown file is not exist.\n";

        return 1;
    }

    return 0;
}
