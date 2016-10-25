#include "iisTextEditorDialog.h"

#include <QTextEdit>

iisTextEditorDialog::iisTextEditorDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *vbl = new QVBoxLayout(this);

    // main toolbar

 	tb = new QToolBar( tr("Main"), this );
    tb->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	vbl->addWidget(tb);

    // text toolbar

 	tbFormat = new QToolBar( tr("Tools"), this );
    tbFormat->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	vbl->addWidget(tbFormat);

    // text input
    
    textEdit = new QTextEdit(this);
    vbl->addWidget(textEdit);

    // buttons

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
    vbl->addWidget(buttonBox);

    // rest

    QSize size(600, 500);
    size = size.expandedTo(minimumSizeHint());
    resize(size);
    setSizeGripEnabled(true);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    connect(textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
    connect(textEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat&)), 
        this, SLOT(currentCharFormatChanged(const QTextCharFormat&)));

    // actions

    actionClear = tb->addAction(QIcon(":/images/filenew.xpm"), tr( "&Clear"), this, SLOT( fileNew() ));
    actionClear->setToolTip("Clear Text");

    actionLoadText = tb->addAction(QIcon(":/images/fileopen.xpm"), tr( "&Open..."), this, SLOT( fileOpen() ));
    actionLoadText->setToolTip("Open Text from File");

    actionSaveText = tb->addAction(QIcon(":/images/filesave.xpm"), tr( "&Save..."), this, SLOT( fileSave() ));
    actionSaveText->setToolTip("Save Text to File");

    //actionPrintText = tb->addAction(QIcon(":/images/fileprint.xpm"), tr( "&Print..."), this, SLOT( filePrint() ));
    //actionPrintText->setToolTip("Print Text");

	tb->addSeparator();

    actionUndo = tb->addAction(QIcon(":/images/editundo.xpm"), tr( "&Undo"), textEdit, SLOT( undo() ));
    actionUndo->setToolTip("Undo Last Action");

    actionRedo = tb->addAction(QIcon(":/images/editredo.xpm"), tr( "&Redo"), textEdit, SLOT( redo() ));
    actionRedo->setToolTip("Redo Last Action");

    actionCut = tb->addAction(QIcon(":/images/editcut.xpm"), tr( "Cu&t"), textEdit, SLOT( cut() ));
    actionCut->setToolTip("Cut to Clipboard");

    actionCopy = tb->addAction(QIcon(":/images/editcopy.xpm"), tr( "&Copy"), textEdit, SLOT( copy() ));
    actionCopy->setToolTip("Copy to Clipboard");

    actionPaste = tb->addAction(QIcon(":/images/editpaste.xpm"), tr( "&Paste"), textEdit, SLOT( paste() ));
    actionPaste->setToolTip("Paste from Clipboard");

    tb->addSeparator();

    actionSwitchType = tb->addAction(QIcon(":/images/type.png"), tr( "Switch"), this, SLOT( switchType() ));
    actionSwitchType->setToolTip("Switch between Rich and Plain text");
    actionSwitchType->setCheckable(true);




    comboFont = new QFontComboBox( tbFormat );
    tbFormat->addWidget(comboFont);
    connect( comboFont, SIGNAL( currentFontChanged( const QFont & ) ),
	     this, SLOT( currentFontChanged( const QFont & ) ) );

    comboSize = new QComboBox( tbFormat );
    tbFormat->addWidget(comboSize);
    QList<int> sizes = QFontDatabase::standardSizes();
    QList<int>::const_iterator it = sizes.begin();
    for ( ; it != sizes.end(); ++it )
	    comboSize->addItem( QString::number( *it ) );
    connect( comboSize, SIGNAL( activated( const QString & ) ),
	     this, SLOT( textSize( const QString & ) ) );
    comboSize->setEditable(true);
 //   comboSize->lineEdit()->setText( QString::number( QApplication::font().pointSize() ) );

    actionTextColor = tbFormat->addAction(tr( "Color"), this, SLOT( textColor() ));
    actionTextColor->setToolTip("Pick a Color");

    actionTextBold = tbFormat->addAction(QIcon(":/images/textbold.xpm"), tr( "&Bold"), this, SLOT( textBold() ));
    actionTextBold->setToolTip("Switch Bold");
    actionTextBold->setCheckable(true);

    actionTextItalic = tbFormat->addAction(QIcon(":/images/textitalic.xpm"), tr( "&Italic"), this, SLOT( textItalic() ));
    actionTextItalic->setToolTip("Switch Italic");
    actionTextItalic->setCheckable(true);

    actionTextUnderline = tbFormat->addAction(QIcon(":/images/textunder.xpm"), tr( "&Underline"), this, SLOT( textUnderline() ));
    actionTextUnderline->setToolTip("Switch Underline");
    actionTextUnderline->setCheckable(true);

	tbFormat->addSeparator();

    QActionGroup *grp = new QActionGroup( this );
    grp->setExclusive( TRUE );
    connect( grp, SIGNAL( triggered( QAction* ) ), this, SLOT( textAlign( QAction* ) ) ); 

    actionAlignLeft = tbFormat->addAction(QIcon(":/images/textleft.xpm"), tr( "&Left"), this, SLOT( alignLeft() ));
    actionAlignLeft->setToolTip("Align Left");
    actionAlignLeft->setCheckable(true);
    grp->addAction(actionAlignLeft);

    actionAlignCenter = tbFormat->addAction(QIcon(":/images/textcenter.xpm"), tr( "C&enter"), this, SLOT( alignCenter() ));
    actionAlignCenter->setToolTip("Align Center");
    actionAlignCenter->setCheckable(true);
    grp->addAction(actionAlignCenter);

    actionAlignRight = tbFormat->addAction(QIcon(":/images/textright.xpm"), tr( "&Right"), this, SLOT( alignRight() ));
    actionAlignRight->setToolTip("Align Right");
    actionAlignRight->setCheckable(true);
    grp->addAction(actionAlignRight);

    actionAlignJustify = tbFormat->addAction(QIcon(":/images/textjustify.xpm"), tr( "&Justify"), this, SLOT( alignJustify() ));
    actionAlignJustify->setToolTip("Align Justified");
    actionAlignJustify->setCheckable(true);
    grp->addAction(actionAlignJustify);
}

iisTextEditorDialog::~iisTextEditorDialog()
{

}

QString iisTextEditorDialog::getText(const QString &txt)
{
	iisTextEditorDialog d;
    bool isRich = Qt::mightBeRichText( txt );
    if ( !isRich )
        d.textEdit->setText( txt );
        //txt = Qt::convertFromPlainText( txt, Qt::WhiteSpacePre );
    else    
        d.textEdit->setHtml( txt );

    d.actionSwitchType->setChecked(isRich);
    d.tbFormat->setVisible(isRich);

	if (d.exec())
        return d.actionSwitchType->isChecked() ? d.textEdit->toHtml() : d.textEdit->toPlainText();
	else
		return QString::null;
}



void iisTextEditorDialog::load( const QString &f )
{
    if ( !QFile::exists( f ) )
		return;
    QFile file( f );
    if ( !file.open( QIODevice::ReadOnly ) )
		return;

    QTextStream ts( &file );
    QString txt = ts.readAll();
    bool isRich = Qt::mightBeRichText( txt );
    if ( !isRich )
        textEdit->setText( txt );
    else    
        textEdit->setHtml( txt );

    actionSwitchType->setChecked(isRich); 
    tbFormat->setVisible(isRich);

    textEdit->viewport()->setFocus();
}

void iisTextEditorDialog::fileNew()
{
	textEdit->clear();
    textEdit->viewport()->setFocus();
}

void iisTextEditorDialog::fileOpen()
{
    QString fn = QFileDialog::getOpenFileName( this, "","", tr( "All Files (*);;HTML-Files (*.htm *.html);;Text Files (*.txt)" ));
    if ( !fn.isEmpty() )
		load( fn );
}

void iisTextEditorDialog::fileSave()
{
    QString fn = QFileDialog::getSaveFileName( this, "","", tr( "All Files (*);;HTML-Files (*.htm *.html);;Text Files (*.txt)" ));
    if ( !fn.isEmpty() ) {
		QFile file( fn );
		if ( !file.open( QIODevice::ReadOnly ) )
			return;
		QTextStream ts( &file );
        ts << textEdit->toPlainText();
    }
}

void iisTextEditorDialog::filePrint()
{
//#ifndef QT_NO_PRINTER
//    QPrinter printer( QPrinter::HighResolution );
//    printer.setFullPage(TRUE);
//    if ( printer.setup( ) ) {
//	QPainter p( &printer );
//	// Check that there is a valid device to print to.
//	if ( !p.device() ) return;
//	QPaintDeviceMetrics metrics( p.device() );
//	int dpiy = metrics.logicalDpiY();
//	int margin = (int) ( (2/2.54)*dpiy ); // 2 cm margins
//	QRect body( margin, margin, metrics.width() - 2*margin, metrics.height() - 2*margin );
//	QFont font( textEdit->QWidget::font() );
// 	font.setPointSize( 10 ); // we define 10pt to be a nice base size for printing
//
//	QSimpleRichText richText( textEdit->text(), font,
//				  textEdit->context(),
//				  textEdit->styleSheet(),
//				  textEdit->mimeSourceFactory(),
//				  body.height() );
//	richText.setWidth( &p, body.width() );
//  	QRect view( body );
//	int page = 1;
//	do {
//		richText.draw( &p, body.left(), body.top(), view, ((QMainWindow*)parent())->colorGroup() );
//	    view.moveBy( 0, body.height() );
//	    p.translate( 0 , -body.height() );
//	    p.setFont( font );
//	    p.drawText( view.right() - p.fontMetrics().width( QString::number( page ) ),
//			view.bottom() + p.fontMetrics().ascent() + 5, QString::number( page ) );
//	    if ( view.top()  >= richText.height() )
//		break;
//	    printer.newPage();
//	    page++;
//	} while (TRUE);
//    }
//#endif
}

void iisTextEditorDialog::textBold()
{
    textEdit->setFontWeight( actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
}

void iisTextEditorDialog::textUnderline()
{
    textEdit->setFontUnderline( actionTextUnderline->isChecked() );
}

void iisTextEditorDialog::textItalic()
{
    textEdit->setFontItalic( actionTextItalic->isChecked() );
}

void iisTextEditorDialog::currentFontChanged( const QFont &f )
{
    textEdit->setFontFamily( f.family() );
    textEdit->viewport()->setFocus();
}

void iisTextEditorDialog::textSize( const QString &p )
{
    textEdit->setFontPointSize( p.toInt() );
    textEdit->viewport()->setFocus();
}

void iisTextEditorDialog::textStyle( int i )
{
 //   if ( i == 0 )
	//textEdit->setParagType( QStyleSheetItem::DisplayBlock, QStyleSheetItem::ListDisc );
 //   else if ( i == 1 )
	//textEdit->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListDisc );
 //   else if ( i == 2 )
	//textEdit->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListCircle );
 //   else if ( i == 3 )
	//textEdit->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListSquare );
 //   else if ( i == 4 )
	//textEdit->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListDecimal );
 //   else if ( i == 5 )
	//textEdit->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListLowerAlpha );
 //   else if ( i == 6 )
	//textEdit->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListUpperAlpha );
 //   textEdit->viewport()->setFocus();
}

void iisTextEditorDialog::textColor()
{
    QColor col = QColorDialog::getColor( textEdit->textColor() );
    if ( !col.isValid() )
	return;
    textEdit->setTextColor( col );
    QPixmap pix( 16, 16 );
    pix.fill( col );
    actionTextColor->setIcon( pix );
}


void iisTextEditorDialog::setTextColor(const QColor& c)
{
    textEdit->setTextColor( c );
    QPixmap pix( 16, 16 );
    pix.fill( c );
    actionTextColor->setIcon( pix );
}


void iisTextEditorDialog::textAlign( QAction *a )
{
    if ( a == actionAlignLeft )
	textEdit->setAlignment( Qt::AlignLeft );
    else if ( a == actionAlignCenter )
	textEdit->setAlignment( Qt::AlignHCenter );
    else if ( a == actionAlignRight )
	textEdit->setAlignment( Qt::AlignRight );
    else if ( a == actionAlignJustify )
	textEdit->setAlignment( Qt::AlignJustify );
}

void iisTextEditorDialog::fontChanged( const QFont &f )
{
    comboFont->setCurrentFont(f);
    comboSize->setEditText( QString::number( f.pointSize() ) );
    actionTextBold->setChecked( f.bold() );
    actionTextItalic->setChecked( f.italic() );
    actionTextUnderline->setChecked( f.underline() );
}

void iisTextEditorDialog::colorChanged( const QColor &c )
{
    QPixmap pix( 16, 16 );
    pix.fill( c );
    actionTextColor->setIcon( pix );
//    colorBox->setCurrentColor(c);
}

void iisTextEditorDialog::alignmentChanged( int a )
{
    if ( a & Qt::AlignLeft )
	actionAlignLeft->setChecked( TRUE );
    else if ( ( a & Qt::AlignHCenter ) )
	actionAlignCenter->setChecked( TRUE );
    else if ( ( a & Qt::AlignRight ) )
	actionAlignRight->setChecked( TRUE );
    else if ( ( a & Qt::AlignJustify ) )
	actionAlignJustify->setChecked( TRUE );
}



void iisTextEditorDialog::textChanged()
{
	actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
	actionRedo->setEnabled(textEdit->document()->isRedoAvailable());
}
 
void iisTextEditorDialog::currentCharFormatChanged ( const QTextCharFormat & f )
{
    fontChanged(f.font());
    colorChanged(f.foreground().color());
    alignmentChanged(textEdit->alignment());
}

void iisTextEditorDialog::switchType()
{
    if (actionSwitchType->isChecked()) {
        textEdit->setHtml(textEdit->toPlainText());
        tbFormat->setVisible(true);
        currentCharFormatChanged(textEdit->currentCharFormat());
    } else {
        textEdit->setPlainText(textEdit->toPlainText());
        tbFormat->setVisible(false);
        //textEdit->setCurrentFont(QFont());
        //textEdit->setTextColor(Qt::black);
        //textEdit->setFontItalic(true);
    }
}
