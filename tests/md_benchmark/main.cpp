
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#define MD4QT_QT_SUPPORT
#define MD4QT_ICU_STL_SUPPORT
#include <md4qt/doc.h>
#include <md4qt/parser.h>
#include <md4qt/html.h>

#include <QFile>
#include <QTest>
#include <QObject>

#include <cmark-gfm-core-extensions.h>
#include <cmark-gfm.h>
#include <registry.h>

class MdBenchmark : public QObject
{
	Q_OBJECT

private Q_SLOTS:
    void md4qt_with_icu()
    {
        QBENCHMARK {
            MD::Parser<MD::UnicodeStringTrait> parser;

            parser.parse(MD::UnicodeString("tests/manual/complex.md"), false);
        }
    }

    void md4qt_with_qt6()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/manual/complex.md"), false);
        }
    }

    void md4qt_to_html()
    {
        MD::Parser<MD::QStringTrait> parser;

        const auto doc = parser.parse(QStringLiteral("tests/manual/complex.md"), false);

        QBENCHMARK {
            MD::toHtml( doc );
        }
    }

    void md4qt_with_qt6_without_autolinks()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;
            parser.removeTextPlugin(MD::GitHubAutoLinkPluginID);

            parser.parse(QStringLiteral("tests/manual/complex.md"), false);
        }
    }

    void cmark_gfm()
    {
        QBENCHMARK {
            QFile file(QStringLiteral("tests/manual/complex.md"));

            if (file.open(QIODevice::ReadOnly)) {
                const auto md = file.readAll();

                file.close();

                cmark_gfm_core_extensions_ensure_registered();

                auto doc = cmark_parse_document(md.constData(), md.size(), CMARK_OPT_FOOTNOTES);

                cmark_node_free(doc);

                cmark_release_plugins();
            }
        }
    }
};

QTEST_GUILESS_MAIN(MdBenchmark)

#include "main.moc"
