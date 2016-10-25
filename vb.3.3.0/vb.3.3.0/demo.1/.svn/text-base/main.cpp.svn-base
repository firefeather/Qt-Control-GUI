#include <QtGui>

#include <iisVB.h>
#include <iisVBAddons.h>


class TForm: public QMainWindow
{
Q_OBJECT
public:
    TForm(QWidget *parent = 0) : QMainWindow(parent)
    {
		QWidget *vbox = new QWidget(this);
		setCentralWidget(vbox);
		resize(400,500);

		QVBoxLayout *vbl = new QVBoxLayout(vbox);

		QTabBar *tw = new QTabBar(vbox);
		tw->addTab("Page 1");
		tw->addTab("Page 2");

		vbl->addWidget(tw);

		// Value Browser

		m_list = new TValueBrowser(vbox);
		vbl->addWidget(m_list);

		// global special value list
		m_list->setSpecialList(&TValueBrowser::s_specialList);


		new TBaseGroup(m_list, "Group 1");


		TBaseValue *v1 = new TBaseValue(m_list, "Value 1 (int as LIST -5-100)");
		v1->setCheckable();
		v1->setValidator(new TIntValidator(-5,100));
		v1->setValue(10);

		m_numbedit.setStep(2.4);
		m_numbedit.enableOverSpin();

		v1->setButtonEditor(&m_numbedit);

		// m_list->addInteger(10, -5, 100, 2.4, true);



		TBaseValue *v2 = new TBaseValue(m_list, "Value 2 (double)");
		v2->setValidator(new TDoubleValidator(-90.9, 90.9876543, 1));
		v2->setValue(1.67);

		m_numbedit2.setStep(10.563);
		m_numbedit2.enableOverSpin();

		v2->setButtonEditor(&m_numbedit2);



		new TBaseGroup(m_list, "Group 2", 0);

		new TBaseValue(m_list, "Value 2.1", 0);

		TBaseValue *v22 = new TBaseValue(m_list, "Value 2.2", 0);
		v22->setHint("Base Value number 2.2");

			TBaseValue *v221 = new TBaseValue(v22, "Value 1 of 2.2 (int)");
			v221->setValidator(new TIntValidator(-100,100));
			v221->setValue(-123);

			new TBaseValue(v22, "Value 2 of 2.2", 0);
			new TTextValue(v22, "Value 3 of 2.2 (multiline text)", 0);


		QIcon *pm1 = new QIcon("Resources/textleft.xpm");
		QIcon *pm2 = new QIcon("Resources/editpaste.xpm");
		QIcon *pm3 = new QIcon("Resources/fileprint.xpm");
		TItemList *il1 = new TItemList(true); 
		il1->setItem(0, "List Item 0");
		il1->setItem(1, "List Item 1", pm1);
		il1->setItem(2, "List Item 2");
		il1->setItem(4, "List Item 4", pm3);
		il1->setItem(5, "List Item 5");
		il1->setItem(8, "List Item 8", pm2);

		TListValue *v24 = new TListValue(m_list, "Value 2.4 (List)", il1);
		v24->setValue(5);

		TListValue *v25 = new TListValue(m_list, "Value 2.5 (List)", il1);
		v25->setValue(1);

		TListValue *v26 = new TListValue(m_list, "Value 2.6 (List)", il1);
		v26->setValue("List Item 8");

		TListValue *v27 = new TListValue(m_list, "Value 2.7 (List)", il1);
		v27->setValue("Some another text");

		TListValue *v28 = new TListValue(m_list, "Value 2.8 (List with own text and autoappend)", il1);
		v28->allowOwnValues(true);
		v28->allowListAppending(true);
		v28->setValue("Auto-appended value");

		TListValue *v29 = new TListValue(m_list, "Value 2.9 (List with free numbers)", il1);
		v29->allowOwnValues(true);
		v29->setValidator(new TIntValidator());
		v29->setValue("Some another (allowed) text");



		TColorValue *v30 = new  TColorValue(m_list, "Value 3 (Colors)", &TColorList::registeredColorList());
		v30->allowOwnValues();
		v30->setValue(QColor(0x00ff00));

		TColorValue *v31 = new  TColorValue(m_list, "Value 4 (Colors)", new TColorList(TColorList::baseColorList()));
		v31->setValue(QColor(Qt::red));
		v31->allowOwnValues();
		v31->allowListAppending();

		
		TBoolValue *v32 = new TBoolValue(m_list, "Value 5 (Boolean)");
		v32->setValue(true);
		


		TBaseGroup *g3 = new TBaseGroup(m_list, "Group 3", 1, false, -1);

			new TBoolValue(g3, "Value 3.1");
			new TColorValue(g3, "Value 3.2");

    			TFontValue *v33 = new TFontValue(m_list, "Value 6 (Font)");
                v33->setValue(QFont("Serif", 11));


		(new TBaseValue(m_list, "Value 3.3"))->setValue("some text");


        (new TPointValue(m_list, "Point Value"))->setValue(QPoint(123,45));
        (new TSizeValue(m_list, "Size Value"))->setValue(QSize(-10,702));
        (new TRectValue(m_list, "Rect Value"))->setValue(QRect(-24,89,700,35));

   

		connect( tw, SIGNAL( currentChanged ( int ) ), m_list, SLOT( switchPage( int ) ) );


		// panel
		m_hint = new QLabel(vbox);
		vbl->addWidget(m_hint);
		m_hint->setText("<empty>");
		connect( m_list, SIGNAL(HintChanged(const QString&, TBaseItem*, const QString&)),
			this, SLOT(onHintChanged(const QString&, TBaseItem*, const QString&)) );

		// buttons
		QHBoxLayout *hbl = new QHBoxLayout();
		vbl->addLayout(hbl);

		//QPushButton *b1 = new QPushButton("On/Off", vbox);
		//hbl->addWidget(b1);
		//connect(b1, SIGNAL(clicked(void)), this, SLOT(onStateChange(void)));
		//QPushButton *b2 = new QPushButton("Hide", hb);
		//QPushButton *b3 = new QPushButton("Kill", hb);

		QPushButton *b4 = new QPushButton("Read", vbox);
		hbl->addWidget(b4);
		connect(b4, SIGNAL(clicked(void)), this, SLOT(onReadItem(void)));

		QPushButton *b5 = new QPushButton("Store", vbox);
		hbl->addWidget(b5);
		connect(b5, SIGNAL(clicked(void)), m_list, SLOT(storeSet(void)));
		QPushButton *b6 = new QPushButton("Restore", vbox);
		hbl->addWidget(b6);
		connect(b6, SIGNAL(clicked(void)), m_list, SLOT(restoreSet(void)));

/*		QPushButton *b7 = new QPushButton("Changes", vbox);
		hbl->addWidget(b7);
		connect(b7, SIGNAL(clicked(void)), this, SLOT(onChangesView(void)))*/;


		// after all data have been set, store it
		m_list->storeSet();

//		connect(m_list, SIGNAL(ValuePicked(TBaseItem*, const SValue&)), this, SLOT(onValuePicked(TBaseItem*, const SValue&)));
		//connect(m_list, SIGNAL(ValueChanged(TBaseItem*, TValue*)), this, SLOT(onValueChanged(TBaseItem*, TValue*)));

		
	}

public slots:

/*
	void	onChangesView(void)
	{
		m_list->setChangeInfo(!m_list->changeInfo());
	}
*/
	void	onHintChanged(const QString &hint, TBaseItem *item, const QString& itemName)
	{
		m_hint->setText(QString("<b>%1</b><br>%2").arg(itemName, hint));
	}
/*
	void	onStateChange()
	{
		if (m_list->currentItem()) {
			m_list->currentItem()->setEnabled(!m_list->currentItem()->isEnabled());
		}
	}
*/
	void	onReadItem()
	{
		if (m_list->currentItem()) {
			TBaseItem *bi = (TBaseItem*)(m_list->currentItem());
			if (bi->hasValue()) {
				QString s;
				SValue &v = ((TBaseValue*)bi)->value();
				switch (v.type) {
					case VT_SPECIAL:
						s = "Special";
						break;
					case VT_VALUE:
						s = "Value";
						break;
					case VT_OWN:
						s = "Own";
						break;
				}
				s = tr("The Value of the Item %1 is:\n%2\nValue Type: %3\nItem Type: %4\nCheck State: %5").arg(bi->text(0), v.value.toString(), s, bi->ItemID()).arg(bi->checkState(0));
				QMessageBox::information(this,
					tr("Item Value"), s);
			} else {
				QMessageBox::critical(this,
					tr("Bad Item"),
					tr("Item %1 does not contain a Value").arg(bi->text(0)));
			}
		}
	}

	void	onValuePicked(TBaseItem *bi, const SValue& v)
	{
		QString s;
		//switch (v.type) {
		//	case VT_SPECIAL:
		//		s = tr("%1 (%2)\n(Value Type: Special)").arg(v.idx).arg((*(m_list->specialList()))[v.idx].value.asString());
		//		break;
		//	case VT_VALUE:
		//		s = tr("%1 (index: %2)\n(type: Value)").arg(v.value.toString()).arg(v.idx);
		//		break;
		//	case VT_OWN: 
		//		s = tr("%1\n(type: Own)").arg(v.value.toString());
		//		break;
		//}
		s = tr("The Value of the Item %1 is:\n %2\n(Item Type: %3)").arg(bi->text(1), s, bi->ItemID());
		//if (bi->isChecked())
		//	s += tr("\n([x] Item is On)");
		QMessageBox::information(this,
			tr("Picked Item"), s);
	}


protected:
	QLabel	*m_hint;
	TValueBrowser	*m_list;

	TNumButtonEditor	m_numbedit, m_numbedit2, m_numbedit3;
};




int main( int argc, char ** argv )
{
    QApplication a( argc, argv );

	// main window
	TForm w;
	w.setWindowTitle("Value Browser 3 Demo");

	// setup
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
	w.show();
    return a.exec();
}

#include "main.moc"