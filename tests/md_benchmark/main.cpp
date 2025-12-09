/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// md4qt include.
#include <src/doc.h>
#include <src/gfm_autolink_parser.h>
#include <src/html.h>
#include <src/parser.h>
#include <src/poscache.h>

// QT include.
#include <QFile>
#include <QObject>
#include <QTest>

// cmark-gfm include.
#include <cmark-gfm-core-extensions.h>
#include <cmark-gfm.h>
#include <registry.h>

class MdBenchmark : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase()
    {
        m_qtFileName = QStringLiteral("tests/manual/complex.md");
        m_qtWd = QDir().absolutePath();

        QFile file(m_qtFileName);

        if (file.open(QIODeviceBase::ReadOnly)) {
            m_qtData = file.readAll();
            file.close();
        } else {
            QFAIL("Unable to open file with Qt.");
        }
    }

    void md4qt_with_qt6()
    {
        QBENCHMARK {
            MD::Parser parser;

            QTextStream stream(m_qtData);

            parser.parse(stream, m_qtWd, m_qtFileName);
        }
    }

    void md4qt_to_html()
    {
        MD::Parser parser;

        const auto doc = parser.parse(QStringLiteral("tests/manual/complex.md"), false);

        QBENCHMARK {
            MD::toHtml(doc);
        }
    }

    void md4qt_poscache_walk()
    {
        MD::Parser parser;

        const auto doc = parser.parse(QStringLiteral("tests/manual/complex.md"), false);

        QBENCHMARK {
            MD::PosCache cache;
            cache.initialize(doc);
        }
    }

    void md4qt_with_qt6_without_autolinks()
    {
        auto inlines = MD::Parser::makeDefaultInlineParsersPipeline();
        inlines.removeIf([](const auto &i) {
            return dynamic_cast<MD::GfmAutolinkParser *>(i.get());
        });

        QBENCHMARK {
            MD::Parser parser;
            parser.setInlineParsers(inlines);

            QTextStream stream(m_qtData);

            parser.parse(stream, m_qtWd, m_qtFileName);
        }
    }

    void cmark_gfm()
    {
        QBENCHMARK {
            cmark_gfm_core_extensions_ensure_registered();

            auto doc = cmark_parse_document(m_qtData.constData(), m_qtData.size(), CMARK_OPT_FOOTNOTES);

            cmark_node_free(doc);

            cmark_release_plugins();
        }
    }

private:
    QString m_qtWd;
    QString m_qtFileName;
    QByteArray m_qtData;
};

QTEST_GUILESS_MAIN(MdBenchmark)

#include "main.moc"
