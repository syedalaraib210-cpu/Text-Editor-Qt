#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QFontComboBox>
#include <QComboBox>
#include <QColorDialog>
#include <QLabel>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QCloseEvent>
#include <QScrollBar>
#include <QStringList>

class TextEditor : public QMainWindow {
    Q_OBJECT

public:
    QTextEdit* textArea;
    QString currentFile;      
    bool isModified = false;  
    QStringList recentFiles;  

    QFontComboBox* fontBox;
    QComboBox* sizeBox;
    QComboBox* colorBox;

    TextEditor(QWidget* parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Text Editor - DSA Assignment 2");
        setMinimumSize(800, 600);
        textArea = new QTextEdit(this);
        textArea->setAcceptRichText(true);
        textArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        textArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        textArea->setLineWrapMode(QTextEdit::WidgetWidth); // word wrap on
        setCentralWidget(textArea);

        setupMenus();
        setupToolbar();
        setupStatusBar();

        connect(textArea, &QTextEdit::textChanged, this, [this]() {
            isModified = true;
            updateTitleBar();
        });
    }

    void setupMenus() {
        QMenu* fileMenu = menuBar()->addMenu("&File");

        QAction* actNew = fileMenu->addAction("&New");
        actNew->setShortcut(QKeySequence::New);

        QAction* actOpen = fileMenu->addAction("&Open...");
        actOpen->setShortcut(QKeySequence::Open);
        QMenu* recentMenu = fileMenu->addMenu("Open &Recent");
        connect(recentMenu, &QMenu::aboutToShow, this, [=]() {
            recentMenu->clear();
            if (recentFiles.isEmpty()) {
                recentMenu->addAction("(No recent files)")->setEnabled(false);
            }
            for (const QString& f : recentFiles) {
                QAction* a = recentMenu->addAction(f);
                connect(a, &QAction::triggered, this, [=]() { openFile(f); });
            }
        });

        fileMenu->addSeparator();

        QAction* actSave = fileMenu->addAction("&Save");
        actSave->setShortcut(QKeySequence::Save);

        QAction* actSaveAs = fileMenu->addAction("Save &As...");
        actSaveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));

        QAction* actClose = fileMenu->addAction("&Close");
        actClose->setShortcut(QKeySequence("Ctrl+W"));

        fileMenu->addSeparator();

        QAction* actExit = fileMenu->addAction("E&xit");
        actExit->setShortcut(QKeySequence::Quit);

        QMenu* editMenu = menuBar()->addMenu("&Edit");

        QAction* actUndo = editMenu->addAction("&Undo");
        actUndo->setShortcut(QKeySequence::Undo);

        QAction* actRedo = editMenu->addAction("&Redo");
        actRedo->setShortcut(QKeySequence::Redo);

        editMenu->addSeparator();

        QAction* actCut = editMenu->addAction("Cu&t");
        actCut->setShortcut(QKeySequence::Cut);

        QAction* actCopy = editMenu->addAction("&Copy");
        actCopy->setShortcut(QKeySequence::Copy);

        QAction* actPaste = editMenu->addAction("&Paste");
        actPaste->setShortcut(QKeySequence::Paste);

        editMenu->addSeparator();

        QAction* actSelectAll = editMenu->addAction("Select &All");
        actSelectAll->setShortcut(QKeySequence::SelectAll);

        editMenu->addSeparator();

        QAction* actFind = editMenu->addAction("&Find && Replace...");
        actFind->setShortcut(QKeySequence::Find);

        QMenu* formatMenu = menuBar()->addMenu("F&ormat");

        QAction* actBold = formatMenu->addAction("&Bold");
        actBold->setShortcut(QKeySequence::Bold);
        actBold->setCheckable(true);

        QAction* actItalic = formatMenu->addAction("&Italic");
        actItalic->setShortcut(QKeySequence::Italic);
        actItalic->setCheckable(true);

        QAction* actUnderline = formatMenu->addAction("&Underline");
        actUnderline->setShortcut(QKeySequence::Underline);
        actUnderline->setCheckable(true);

        formatMenu->addSeparator();

        QAction* actColor = formatMenu->addAction("Text &Color...");
        QAction* actWordWrap = formatMenu->addAction("Word &Wrap");
        actWordWrap->setCheckable(true);
        actWordWrap->setChecked(true);

        QMenu* viewMenu = menuBar()->addMenu("&View");
        QAction* actZoomIn = viewMenu->addAction("Zoom &In");
        actZoomIn->setShortcut(QKeySequence::ZoomIn);
        QAction* actZoomOut = viewMenu->addAction("Zoom &Out");
        actZoomOut->setShortcut(QKeySequence::ZoomOut);

        connect(actNew, &QAction::triggered, this, &TextEditor::newFile);
        connect(actOpen, &QAction::triggered, this, &TextEditor::openFileDialog);
        connect(actSave, &QAction::triggered, this, &TextEditor::saveFile);
        connect(actSaveAs, &QAction::triggered, this, &TextEditor::saveFileAs);
        connect(actClose, &QAction::triggered, this, &TextEditor::closeFile);
        connect(actExit, &QAction::triggered, this, &QMainWindow::close);

        connect(actUndo, &QAction::triggered, textArea, &QTextEdit::undo);
        connect(actRedo, &QAction::triggered, textArea, &QTextEdit::redo);
        connect(actCut, &QAction::triggered, textArea, &QTextEdit::cut);
        connect(actCopy, &QAction::triggered, textArea, &QTextEdit::copy);
        connect(actPaste, &QAction::triggered, textArea, &QTextEdit::paste);
        connect(actSelectAll, &QAction::triggered, textArea, &QTextEdit::selectAll);
        connect(actFind, &QAction::triggered, this, &TextEditor::findReplace);

        connect(actBold, &QAction::triggered, this, &TextEditor::toggleBold);
        connect(actItalic, &QAction::triggered, this, &TextEditor::toggleItalic);
        connect(actUnderline, &QAction::triggered, this, &TextEditor::toggleUnderline);
        connect(actColor, &QAction::triggered, this, &TextEditor::changeTextColor);
        connect(actWordWrap, &QAction::triggered, this, [this](bool checked) {
            textArea->setLineWrapMode(checked ? QTextEdit::WidgetWidth : QTextEdit::NoWrap);
        });

        connect(actZoomIn, &QAction::triggered, textArea, &QTextEdit::zoomIn);
        connect(actZoomOut, &QAction::triggered, textArea, &QTextEdit::zoomOut);
    }

    void setupToolbar() {
        QToolBar* toolbar = addToolBar("Formatting");
        toolbar->setMovable(false);

        fontBox = new QFontComboBox();
        fontBox->setCurrentFont(QFont("Arial"));
        toolbar->addWidget(new QLabel(" Font: "));
        toolbar->addWidget(fontBox);

        sizeBox = new QComboBox();
        QStringList sizes = {"8","9","10","11","12","14","16","18","20","24","28","32","36","48","72"};
        sizeBox->addItems(sizes);
        sizeBox->setCurrentText("12");
        toolbar->addWidget(new QLabel("  Size: "));
        toolbar->addWidget(sizeBox);

        toolbar->addSeparator();

        QAction* boldBtn = toolbar->addAction("B");
        boldBtn->setFont(QFont("Arial", 10, QFont::Bold));
        boldBtn->setCheckable(true);

        QAction* italicBtn = toolbar->addAction("I");
        QFont italicFont("Arial", 10);
        italicFont.setItalic(true);
        italicBtn->setFont(italicFont);
        italicBtn->setCheckable(true);

        QAction* underBtn = toolbar->addAction("U");
        underBtn->setCheckable(true);

        toolbar->addSeparator();

        QAction* colorBtn = toolbar->addAction("Color");

        connect(fontBox, &QFontComboBox::currentFontChanged, this, [this](const QFont& f) {
            QTextCharFormat fmt;
            fmt.setFontFamilies({f.family()});
            mergeFormat(fmt);
        });

        connect(sizeBox, &QComboBox::currentTextChanged, this, [this](const QString& s) {
            QTextCharFormat fmt;
            fmt.setFontPointSize(s.toInt());
            mergeFormat(fmt);
        });

        connect(boldBtn, &QAction::triggered, this, &TextEditor::toggleBold);
        connect(italicBtn, &QAction::triggered, this, &TextEditor::toggleItalic);
        connect(underBtn, &QAction::triggered, this, &TextEditor::toggleUnderline);
        connect(colorBtn, &QAction::triggered, this, &TextEditor::changeTextColor);
    }

    void setupStatusBar() {
        statusBar()->showMessage("Ready");
        connect(textArea, &QTextEdit::cursorPositionChanged, this, [this]() {
            QTextCursor c = textArea->textCursor();
            int line = c.blockNumber() + 1;
            int col = c.columnNumber() + 1;
            statusBar()->showMessage(QString("Line: %1  Column: %2").arg(line).arg(col));
        });
    }
    void mergeFormat(const QTextCharFormat& fmt) {
        QTextCursor cursor = textArea->textCursor();
        cursor.mergeCharFormat(fmt);
        textArea->mergeCurrentCharFormat(fmt);
    }

    void updateTitleBar() {
        QString title = "Text Editor";
        if (!currentFile.isEmpty()) title += " - " + QFileInfo(currentFile).fileName();
        if (isModified) title += " *";
        setWindowTitle(title);
    }
    bool checkSave() {
        if (!isModified) return true;
        int result = QMessageBox::question(this, "Unsaved Changes",
            "You have unsaved changes. Save before continuing?",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (result == QMessageBox::Yes) return saveFile();
        if (result == QMessageBox::Cancel) return false;
        return true;
    }

public slots:
    void newFile() {
        if (!checkSave()) return;
        textArea->clear();
        currentFile = "";
        isModified = false;
        updateTitleBar();
    }

    void openFileDialog() {
        if (!checkSave()) return;
        QString path = QFileDialog::getOpenFileName(this, "Open File", "",
            "Text Files (*.txt);;HTML Files (*.html);;All Files (*)");
        if (!path.isEmpty()) openFile(path);
    }

    void openFile(const QString& path) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "Could not open file.");
            return;
        }
        textArea->setPlainText(file.readAll());
        currentFile = path;
        isModified = false;

        // add to recent files
        recentFiles.removeAll(path);
        recentFiles.prepend(path);
        if (recentFiles.size() > 5) recentFiles.removeLast(); // keep last 5

        updateTitleBar();
    }

    bool saveFile() {
        if (currentFile.isEmpty()) return saveFileAs();
        QFile file(currentFile);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "Could not save file.");
            return false;
        }
        QTextStream out(&file);
        out << textArea->toPlainText();
        isModified = false;
        updateTitleBar();
        statusBar()->showMessage("File saved.", 2000);
        return true;
    }

    bool saveFileAs() {
        QString path = QFileDialog::getSaveFileName(this, "Save As", "",
            "Text Files (*.txt);;HTML Files (*.html);;All Files (*)");
        if (path.isEmpty()) return false;
        currentFile = path;
        return saveFile();
    }

    void closeFile() {
        if (!checkSave()) return;
        textArea->clear();
        currentFile = "";
        isModified = false;
        updateTitleBar();
    }

    void findReplace() {
        bool ok;
        QString findText = QInputDialog::getText(this, "Find", "Find what:", QLineEdit::Normal, "", &ok);
        if (!ok || findText.isEmpty()) return;

        QString replaceText = QInputDialog::getText(this, "Replace", "Replace with:", QLineEdit::Normal, "", &ok);
        if (!ok) return;

        QString content = textArea->toPlainText();
        int count = content.count(findText);
        if (count == 0) {
            QMessageBox::information(this, "Find", "Text not found.");
            return;
        }
        content.replace(findText, replaceText);
        textArea->setPlainText(content);
        QMessageBox::information(this, "Done", QString("Replaced %1 occurrence(s).").arg(count));
    }

    void toggleBold() {
        QTextCharFormat fmt;
        fmt.setFontWeight(textArea->fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);
        mergeFormat(fmt);
    }

    void toggleItalic() {
        QTextCharFormat fmt;
        fmt.setFontItalic(!textArea->fontItalic());
        mergeFormat(fmt);
    }

    void toggleUnderline() {
        QTextCharFormat fmt;
        fmt.setFontUnderline(!textArea->fontUnderline());
        mergeFormat(fmt);
    }

    void changeTextColor() {
        QColor color = QColorDialog::getColor(textArea->textColor(), this, "Pick Text Color");
        if (!color.isValid()) return;
        QTextCharFormat fmt;
        fmt.setForeground(color);
        mergeFormat(fmt);
    }

protected:
    void closeEvent(QCloseEvent* event) override {
        if (checkSave()) event->accept();
        else event->ignore();
    }
};

#endif // TEXTEDITOR_H
