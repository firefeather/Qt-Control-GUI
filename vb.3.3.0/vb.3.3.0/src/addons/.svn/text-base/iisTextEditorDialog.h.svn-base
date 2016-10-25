#ifndef IISTEXTEDITORDIALOG_H
#define IISTEXTEDITORDIALOG_H

#include <QtGui>

#include <iisDefines.h>

class LIBVB4_EXPORT iisTextEditorDialog : public QDialog
{
    Q_OBJECT

public:
    iisTextEditorDialog(QWidget *parent = 0);
    ~iisTextEditorDialog();

	static QString getText(const QString &text = QString::null);

private slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void filePrint();

    void textBold();
    void textUnderline();
    void textItalic();
    void currentFontChanged( const QFont & );
    void textSize( const QString &p );
    void textStyle( int s );
    void textColor();
	void setTextColor(const QColor &c);
    void textAlign( QAction *a );

    void fontChanged( const QFont &f );
    void colorChanged( const QColor &c );
    void alignmentChanged( int a );

    void currentCharFormatChanged ( const QTextCharFormat & f );
	void textChanged();

    void switchType();

private:
    void load( const QString &f );

    QTextEdit *textEdit;
    QDialogButtonBox *buttonBox;

    QAction *actionTextBold,
			*actionTextUnderline,
			*actionTextItalic,
			*actionTextColor,
			*actionClear,
			*actionLoadText,
			*actionSaveText,
			*actionPrintText,
			*actionUndo,
			*actionRedo,
			*actionCopy,
			*actionPaste,
			*actionCut,
			*actionAlignLeft,
			*actionAlignCenter,
			*actionAlignRight,
			*actionAlignJustify;
    QAction *actionSwitchType;
    QFontComboBox *comboFont;
    QComboBox *comboSize;

	QToolBar *tb, *tbFormat;
	QPopupMenu *mFile, *mEdit, *mFormat;
};

#endif // IISTEXTEDITORDIALOG_H
