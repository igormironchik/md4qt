/*
    SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QObject>
#include <QTest>

// md4qt include.
#include "parser.h"

class Bench final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void with_yaml_plugin()
    {
        QBENCHMARK {
            MD::Parser p;
            // p.addBlockPlugin(std::make_shared<MD::YAMLBlockPlugin<TRAIT>>());
            p.parse(QStringLiteral("tests/plugins/yaml/data/002.md"));
        }
    }

    void without_yaml_plugin()
    {
        QBENCHMARK {
            MD::Parser p;
            p.parse(QStringLiteral("tests/plugins/yaml/data/002.md"));
        }
    }
};

QTEST_GUILESS_MAIN(Bench)

#include "bench.moc"
