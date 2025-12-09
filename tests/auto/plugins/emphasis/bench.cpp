/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QObject>
#include <QTest>

// md4qt include.
#include <md4qt/parser.h>
#include <md4qt/plugins.h>

class Bench final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void with_emphasis_plugin()
    {
        QBENCHMARK {
            MD::Parser<TRAIT> p;
            p.addTextPlugin(MD::TextPlugin::UserDefined,
                            MD::EmphasisPlugin::emphasisTemplatePlugin<TRAIT>,
                            true,
                            typename TRAIT::StringList() << TRAIT::latin1ToString("^") << TRAIT::latin1ToString("8"));
            p.parse(TRAIT::latin1ToString("tests/plugins/emphasis/data/026.md"));
        }
    }

    void without_emphasis_plugin()
    {
        QBENCHMARK {
            MD::Parser<TRAIT> p;
            p.parse(TRAIT::latin1ToString("tests/plugins/emphasis/data/026.md"));
        }
    }
};

QTEST_GUILESS_MAIN(Bench)

#include "bench.moc"
