
/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QObject>
#include <QTest>

// md4qt include.
#include <md4qt/parser.h>

class Bench final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void block_bq_flat()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/block-bq-flat.md"), false);
        }
    }

    void block_bq_nested()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/block-bq-nested.md"), false);
        }
    }

    void block_code()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/block-code.md"), false);
        }
    }

    void block_fences()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/block-fences.md"), false);
        }
    }

    void block_heading()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/block-heading.md"), false);
        }
    }

    void block_hr()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/block-hr.md"), false);
        }
    }

    void block_html()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/block-html.md"), false);
        }
    }

    void block_lheading()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/block-lheading.md"), false);
        }
    }

    void block_list_flat()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/block-list-flat.md"), false);
        }
    }

    void block_list_nested()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/block-list-nested.md"), false);
        }
    }

    void block_ref_flat()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/block-ref-flat.md"), false);
        }
    }

    void block_ref_nested()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/block-ref-nested.md"), false);
        }
    }

    void inline_autolink()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/inline-autolink.md"), false);
        }
    }

    void inline_backticks()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/inline-backticks.md"), false);
        }
    }

    void inline_em_flat()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/inline-em-flat.md"), false);
        }
    }

    void inline_em_nested()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/inline-em-nested.md"), false);
        }
    }

    void inline_em_worst()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/inline-em-worst.md"), false);
        }
    }

    void inline_entity()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/inline-entity.md"), false);
        }
    }

    void inline_escape()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/inline-escape.md"), false);
        }
    }

    void inline_html()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/inline-html.md"), false);
        }
    }

    void inline_links_flat()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/inline-links-flat.md"), false);
        }
    }

    void inline_links_nested()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/inline-links-nested.md"), false);
        }
    }

    void inline_newlines()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/inline-newlines.md"), false);
        }
    }

    void lorem1()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/lorem1.md"), false);
        }
    }

    void rawtabs()
    {
        QBENCHMARK {
            MD::Parser<MD::QStringTrait> parser;

            parser.parse(QStringLiteral("tests/bench/data/rawtabs.md"), false);
        }
    }
};

QTEST_GUILESS_MAIN(Bench)

#include "main.moc"
