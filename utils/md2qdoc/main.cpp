/*
    SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

// md4qt include.
#define MD4QT_QT_SUPPORT
#include <md4qt/parser.h>
#include <md4qt/visitor.h>

// Qt include.
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QScopedValueRollback>
#include <QSharedPointer>
#include <QTextStream>

QTextStream s_outStream(stdout);

//
// QDocVisitor
//

//! Converter of Markdown document into QDoc.
class QDocVisitor : public MD::Visitor<MD::QStringTrait>
{
public:
    explicit QDocVisitor(int offset)
        : m_offset(offset)
        , m_offsetDelta(offset)
    {
    }

    ~QDocVisitor() override
    {
        closeQDoc();
    }

    virtual void toQDoc(std::shared_ptr<MD::Document<MD::QStringTrait>> doc,
                        const QString &workingDirectory,
                        const QString &outputDirectory)
    {
        m_workingDirectory = workingDirectory;
        m_outputDirectory = outputDirectory;

        this->process(doc);
    }

protected:
    virtual void openStyle(const MD::ItemWithOpts<MD::QStringTrait>::Styles &styles)
    {
        for (const auto &s : styles) {
            switch (s.style()) {
            case MD::TextOption::BoldText:
                m_qdoc.append(QStringLiteral("\\b{"));
                break;

            case MD::TextOption::ItalicText:
                m_qdoc.append(QStringLiteral("\\e{"));
                break;

            default:
                break;
            }
        }
    }

    virtual void closeStyle(const MD::ItemWithOpts<MD::QStringTrait>::Styles &styles)
    {
        for (const auto &s : styles) {
            switch (s.style()) {
            case MD::TextOption::BoldText:
                m_qdoc.append(QStringLiteral("}"));
                break;

            case MD::TextOption::ItalicText:
                m_qdoc.append(QStringLiteral("}"));
                break;

            default:
                break;
            }
        }
    }

    void onAddLineEnding() override
    {
        m_qdoc.append(QStringLiteral("\n"));
        m_qdoc.append(currentOffset());
    }

    void onText(MD::Text<MD::QStringTrait> *t) override
    {
        openStyle(t->openStyles());

        m_qdoc.append(t->text());

        closeStyle(t->closeStyles());
    }

    void onMath(MD::Math<MD::QStringTrait> *m) override
    {
        s_outStream << QStringLiteral("QDoc does not support LaTeX maths...\n");
        printWhereInfo(m);
    }

    void onLineBreak(MD::LineBreak<MD::QStringTrait> *) override
    {
        m_qdoc.append(QStringLiteral("\\br "));
    }

    void onParagraph(MD::Paragraph<MD::QStringTrait> *p, bool wrap) override
    {
        if (wrap && !m_firstInListItem) {
            m_qdoc.append(currentOffset());
        }

        m_firstInListItem = false;

        MD::Visitor<MD::QStringTrait>::onParagraph(p, wrap);

        if (wrap) {
            m_qdoc.append(QStringLiteral("\n\n"));
        }
    }

    void onHeading(MD::Heading<MD::QStringTrait> *h) override
    {
        m_qdoc.append(QStringLiteral("%2\\target %1\n").arg(h->label(), currentOffset()));
        m_qdoc.append(QStringLiteral("%2\\section%1 ").arg(QString::number(h->level() > 4 ? 4 : h->level()), currentOffset()));
        incrementOffset();

        if (h->text().get()) {
            onParagraph(h->text().get(), false);
        }

        m_qdoc.append(QStringLiteral("\n\n"));
        decrementOffset();
    }

    void onCode(MD::Code<MD::QStringTrait> *c) override
    {
        bool qml = false;

        if (c->syntax().toLower() == QLatin1String("qml")) {
            qml = true;
            m_qdoc.append(QStringLiteral("%1\\qml /\n").arg(currentOffset()));
        } else {
            m_qdoc.append(QStringLiteral("%1\\code /\n").arg(currentOffset()));
        }

        auto codeStr = c->text();
        codeStr.replace(QLatin1Char('/'), QStringLiteral("\\1"));

        const auto code = codeStr.split(QLatin1Char('\n'));

        for (const auto &line : code) {
            m_qdoc.append(QStringLiteral("%1%2\n").arg(currentOffset(), line));
        }

        if (qml) {
            m_qdoc.append(QStringLiteral("%1\\endqml\n\n").arg(currentOffset()));
        } else {
            m_qdoc.append(QStringLiteral("%1\\endcode\n\n").arg(currentOffset()));
        }
    }

    void onInlineCode(MD::Code<MD::QStringTrait> *c) override
    {
        openStyle(c->openStyles());

        m_qdoc.append(QStringLiteral("\\c {"));

        m_qdoc.append(c->text());

        m_qdoc.append(QStringLiteral("}"));

        closeStyle(c->closeStyles());
    }

    void onBlockquote(MD::Blockquote<MD::QStringTrait> *b) override
    {
        if (!m_inList) {
            m_qdoc.append(QStringLiteral("%1\\quotation\n").arg(currentOffset()));
            incrementOffset();

            Visitor<MD::QStringTrait>::onBlockquote(b);

            decrementOffset();
            m_qdoc.append(QStringLiteral("%1\\endquotation\n\n").arg(currentOffset()));
        } else {
            s_outStream << QStringLiteral("QDoc does not support quotations in list...\n");
            printWhereInfo(b);
        }
    }

    void onList(MD::List<MD::QStringTrait> *l) override
    {
        MD::ListItem<MD::QStringTrait>::ListType type = MD::ListItem<MD::QStringTrait>::Ordered;
        bool first = true;
        QScopedValueRollback inList(m_inList, true);

        for (auto it = l->items().cbegin(), last = l->items().cend(); it != last; ++it) {
            if ((*it)->type() == MD::ItemType::ListItem) {
                auto *item = static_cast<MD::ListItem<MD::QStringTrait> *>(it->get());

                if (first) {
                    type = item->listType();

                    if (type == MD::ListItem<MD::QStringTrait>::Ordered) {
                        m_qdoc.append(QStringLiteral("\n%1\\list 1\n").arg(currentOffset()));
                    } else {
                        m_qdoc.append(QStringLiteral("\n%1\\list\n").arg(currentOffset()));
                    }

                    incrementOffset();
                }

                onListItem(item, first);

                first = false;
            }
        }

        if (!first) {
            decrementOffset();
            m_qdoc.append(QStringLiteral("%1\\endlist\n\n").arg(currentOffset()));
        }
    }

    void onTable(MD::Table<MD::QStringTrait> *t) override
    {
        if (!m_inList) {
            if (!t->isEmpty()) {
                m_qdoc.append(QStringLiteral("\n%1\\table\n").arg(currentOffset()));

                int columns = 0;

                incrementOffset();
                m_qdoc.append(QStringLiteral("\n%1\\header\n").arg(currentOffset()));
                incrementOffset();

                for (auto th = (*t->rows().cbegin())->cells().cbegin(), last = (*t->rows().cbegin())->cells().cend(); th != last; ++th) {
                    m_qdoc.append(QStringLiteral("\n%1\\li ").arg(currentOffset()));

                    this->onTableCell(th->get());

                    m_qdoc.append(QStringLiteral("\n"));

                    ++columns;
                }

                decrementOffset();

                for (auto r = std::next(t->rows().cbegin()), rlast = t->rows().cend(); r != rlast; ++r) {
                    m_qdoc.append(QStringLiteral("\n%1\\row\n").arg(currentOffset()));
                    incrementOffset();

                    int i = 0;

                    for (auto c = (*r)->cells().cbegin(), clast = (*r)->cells().cend(); c != clast; ++c) {
                        m_qdoc.append(QStringLiteral("\n%1\\li ").arg(currentOffset()));

                        this->onTableCell(c->get());

                        m_qdoc.append(QStringLiteral("\n"));

                        ++i;

                        if (i == columns) {
                            break;
                        }
                    }

                    decrementOffset();
                }

                decrementOffset();
                m_qdoc.append(QStringLiteral("\n%1\\endtable\n\n").arg(currentOffset()));
            }
        } else {
            s_outStream << QStringLiteral("QDoc does not support tables in list...\n");
            printWhereInfo(t);
        }
    }

    void onAnchor(MD::Anchor<MD::QStringTrait> *a) override
    {
        closeQDoc();

        auto relative = a->label();
        relative.remove(m_workingDirectory);
        const auto dotPos = relative.lastIndexOf(QLatin1Char('.'));
        m_fileName = relative;

        if (dotPos != -1 ) {
            relative.remove(dotPos, relative.size() - dotPos);
        }

        const QString fileName = m_outputDirectory + relative + QStringLiteral(".qdoc");

        QFileInfo info(fileName);

        QDir dir;
        dir.mkpath(info.absolutePath());

        m_file.reset(new QFile(fileName));

        if (!m_file->open(QIODevice::WriteOnly)) {
            s_outStream << QStringLiteral("Unable to open \"%1\" file.\n").arg(fileName);
            m_file.reset();
        } else {
            m_qdoc.append(QStringLiteral("/*!\n%2\\page %1\n").arg(m_fileName + QStringLiteral(".html"), currentOffset()));
        }
    }

    void onRawHtml(MD::RawHtml<MD::QStringTrait> *h) override
    {
        s_outStream << QStringLiteral("QDoc does not support HTML...\n");
        printWhereInfo(h);
    }

    void onHorizontalLine(MD::HorizontalLine<MD::QStringTrait> *l) override
    {
        s_outStream << QStringLiteral("QDoc does not support horizontal lines...\n");
        printWhereInfo(l);
    }

    void onLink(MD::Link<MD::QStringTrait> *l) override
    {
        QString url = l->url();

        const auto lit = this->m_doc->labeledLinks().find(url);

        if (lit != this->m_doc->labeledLinks().cend()) {
            url = lit->second->url();
        }

        if (std::find(this->m_anchors.cbegin(), this->m_anchors.cend(), url) != this->m_anchors.cend()) {
            url.remove(m_workingDirectory);
            url.append(QStringLiteral(".html"));
        } else if (url.startsWith(QStringLiteral("#"))) {
            const auto it = this->m_doc->labeledHeadings().find(url);

            if (it == this->m_doc->labeledHeadings().cend()) {
                auto path = static_cast<MD::Anchor<MD::QStringTrait> *>(this->m_doc->items().at(0).get())->label();
                const auto sp = path.lastIndexOf(QStringLiteral("/"));
                path.remove(sp, path.length() - sp);
                const auto p = url.indexOf(path) - 1;
                url.remove(p, url.length() - p);
            } else {
                url = it->second->label();
            }
        }

        openStyle(l->openStyles());

        m_qdoc.append(QStringLiteral("\\l {%1} {").arg(url));

        if (l->p() && !l->p()->isEmpty()) {
            QScopedValueRollback skipImage(m_skipImage, true);
            const auto length = m_qdoc.length();
            onParagraph(l->p().get(), false);

            if (m_qdoc.length() == length) {
                m_qdoc.append(l->url());
            }
        } else if (!l->text().isEmpty()) {
            m_qdoc.append(l->text());
        } else {
            m_qdoc.append(l->url());
        }

        m_qdoc.append(QStringLiteral("}"));

        closeStyle(l->closeStyles());
    }

    void onImage(MD::Image<MD::QStringTrait> *i) override
    {
        if (!m_skipImage) {
            openStyle(i->openStyles());

            m_qdoc.append(QStringLiteral("\\image "));
            auto url = i->url();
            url.remove(m_workingDirectory);
            m_qdoc.append(url);

            closeStyle(i->closeStyles());
        }
    }

    void onFootnoteRef(MD::FootnoteRef<MD::QStringTrait> *r) override
    {
        s_outStream << QStringLiteral("QDoc does not support footnote references...\n");
        printWhereInfo(r);
    }

    void onListItem(MD::ListItem<MD::QStringTrait> *i, bool first) override
    {
        m_qdoc.append(QStringLiteral("%1\\li ").arg(currentOffset()));
        incrementOffset();

        m_firstInListItem = true;
        Visitor<MD::QStringTrait>::onListItem(i, first);

        decrementOffset();
        m_qdoc.append(QStringLiteral("\n"));
    }

protected:
    //! QDoc content.
    QString m_qdoc;
    //! Current Markdown file name.
    QString m_fileName;
    //! Working directory.
    QString m_workingDirectory;
    //! Output directory.
    QString m_outputDirectory;
    //! Current file.
    QSharedPointer<QFile> m_file;
    //! Current offset for nested blocks.
    int m_offset = 4;
    //! Offset delta used for increment/decrement current offset.
    int m_offsetDelta = 4;
    //! Flag shows that we are first in list item.
    bool m_firstInListItem = false;
    //! Flag shows that we are in list.
    bool m_inList = false;
    //! Flag to skip image.
    bool m_skipImage = false;

    void closeQDoc()
    {
        if (m_file) {
            QTextStream stream(m_file.data());
            stream << m_qdoc;
            stream << QStringLiteral("\n*/\n");
            m_qdoc.clear();
        }
    }

    QString currentOffset() const
    {
        return QString(m_offset, QLatin1Char(' '));
    }

    void incrementOffset()
    {
        m_offset += m_offsetDelta;
    }

    void decrementOffset()
    {
        m_offset -= m_offsetDelta;
    }

    void printWhereInfo(MD::Item<MD::QStringTrait> *item)
    {
        s_outStream << QLatin1String("%1in \"%2\" at line %3\n")
                    .arg(QString(2, QLatin1Char(' ')), m_fileName, QString::number(item->startLine() + 1));
    }
}; // class QDocVisitor

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Markdown to QDoc converter."));
    parser.addHelpOption();
    parser.addPositionalArgument(QStringLiteral("markdown"), QStringLiteral("Markdown file to convert."));
    QCommandLineOption out(QStringList() << QStringLiteral("o") << QStringLiteral("out"),
                           QStringLiteral("Output directory."), QStringLiteral("dir"), QStringLiteral("./"));
    QCommandLineOption recursive(QStringList() << QStringLiteral("r") << QStringLiteral("recursive"),
                                 QStringLiteral("Load and convert all linked Markdown files."));
    QCommandLineOption offset(QStringList() << QStringLiteral("offset"),
                           QStringLiteral("Amount of spaces used for offset."), QStringLiteral("int"), QStringLiteral("4"));
    parser.addOption(out);
    parser.addOption(recursive);
    parser.addOption(offset);

    parser.process(app);

    const auto files = parser.positionalArguments();
    const auto outputDirectory = parser.value(out);
    const auto isRecursive = parser.isSet(recursive);
    auto offsetValue = parser.value(offset).toInt();

    if (offsetValue <= 0) {
        offsetValue = 4;
    }

    if (!files.isEmpty()) {
        MD::Parser<MD::QStringTrait> parser;

        for (const auto &file : files) {
            QFileInfo info(file);

            if (info.exists(file)) {
                const auto doc = parser.parse(file, isRecursive);

                auto workingDirectory = info.absolutePath();
                auto outputDirectoryPath = QDir(outputDirectory).absolutePath();

                if (!workingDirectory.endsWith(QLatin1Char('/'))) {
                    workingDirectory.append(QLatin1Char('/'));
                }

                if (!outputDirectoryPath.endsWith(QLatin1Char('/'))) {
                    outputDirectoryPath.append(QLatin1Char('/'));
                }

                QDocVisitor visitor(offsetValue);

                visitor.toQDoc(doc, workingDirectory, outputDirectoryPath);
            } else {
                s_outStream << QStringLiteral("File \"%1\" does not exist.\n").arg(file);
            }
        }
    } else {
        s_outStream << QStringLiteral("Input files are not set.\n");
    }

    return 0;
}
