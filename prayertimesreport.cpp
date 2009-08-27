#include <QtGui>
#include <QtSql>
#include <QDebug>

#include "prayertime.h"
#include "prayertimesreport.h"

PrayerTimesReport::PrayerTimesReport(const QStringList& infoList,QWidget* parent):
QMainWindow(parent)
{
	//setAttribute(Qt::WA_DeleteOnClose);
	passedList = infoList;
	
	setLayoutDirection(Qt::RightToLeft);
	
	textEdit=new QTextEdit(this);
	textEdit->setReadOnly(true);
	
	QTextOption option=textEdit->document()->defaultTextOption();
	option.setAlignment(Qt::AlignRight);
	textEdit->document()->setDefaultTextOption(option);

	setCentralWidget(textEdit);
	
	createActions();
	createMenus();
	
	init();
	displayHeader();
	displayReport();

		
	QTextCursor c = textEdit->textCursor();
	c.movePosition(QTextCursor::Start);
	textEdit->setTextCursor(c);
	
	setWindowTitle(trUtf8("مواقيت الصلاة"));
}

QString PrayerTimesReport::convertIntoTime(double var)
{
	QString time;
	QString ch="AM";
	
	if( int(var) > 12 )
	{
		time.append(QString::number(int(var)-12));
		ch="PM";
	}
	
	else if (int(var) == 12)
	{
		time.append(QString::number(int(var)));
		ch="PM";
	}
	
	else
		time.append(QString::number(int(var)));
	
	time.append(":");
	
	var-=int(var);
	var*=60;
	int min=int(var);
	
	time.append(QString::number(min));
	time.append(":");
		
	var-=int(var);
	var*=60;
	int sec=int(var);
	
	time.append(QString::number(sec));
	
	time.append(" ");
	time.append(ch);
	
	return time;
}

void PrayerTimesReport::createList()
{
	list.clear();
	QStringList record;
	QDate date = QDate::currentDate();
	//int month = date.month();
	int dayNo = date.daysInMonth();
	
	qDebug() << dayNo;

	PrayerTime prayerTime(passedList.at(0).toDouble(),passedList.at(1).toDouble(),passedList.at(2).toInt(),QDate::currentDate().day(),QDate::currentDate().month(),QDate::currentDate().year(),passedList.at(3).toStdString(),passedList.at(4).toStdString(),passedList.at(5).toStdString());
	
	//prayerTime.calculate();
	
	
	for(int i=0;i<dayNo;++i)
	{
		prayerTime.setDay(i+1);
		prayerTime.calculate();
		
		record << QString::number(prayerTime.day())+"/"+QString::number(prayerTime.month())+"/"+QString::number(prayerTime.year());
		record << convertIntoTime(prayerTime.fajrTime());
		record << convertIntoTime(prayerTime.shroukTime());
		record << convertIntoTime(prayerTime.zuhrTime());
		record << convertIntoTime(prayerTime.asrTime());
		record << convertIntoTime(prayerTime.maghribTime());
		record << convertIntoTime(prayerTime.ishaTime());
		
		qDebug() << QString::number(prayerTime.day())+"/"+QString::number(prayerTime.month())+"/"+QString::number(prayerTime.year());   
		qDebug() << convertIntoTime(prayerTime.fajrTime());
		qDebug() << convertIntoTime(prayerTime.shroukTime());
		qDebug() << convertIntoTime(prayerTime.zuhrTime());
		qDebug() << convertIntoTime(prayerTime.asrTime());
		qDebug() << convertIntoTime(prayerTime.maghribTime());
		qDebug() << convertIntoTime(prayerTime.ishaTime());

		list.append(record);
		record.clear();
	}
	
	

}


void PrayerTimesReport::printClicked()
{
     QTextDocument *document = textEdit->document();
     QPrinter printer;

     QPrintDialog *dlg = new QPrintDialog(&printer, this);
     if (dlg->exec() != QDialog::Accepted)
         return;

     document->print(&printer);

     statusBar()->showMessage(tr("Ready"), 2000);
}
	


void PrayerTimesReport::displayHeader()
{
	cursor.insertText("بسم الله الرحمن الرحيم",defaultCharFormat);

	cursor.insertBlock();
	cursor.insertBlock();
	cursor.insertBlock();
	
	cursor.insertText("مواقيت الصلاة",titleCharFormat);
	cursor.insertBlock();
	cursor.insertText(QDate::currentDate().toString("MM/yyyy"),titleCharFormat);
	
	cursor.insertBlock();
	cursor.insertText("+++++++++++++++++++++++++++++++++++",defaultCharFormat);
	cursor.insertBlock();
	cursor.insertBlock();
}


void PrayerTimesReport::displayReport()
{
	createList();

	cursor.insertBlock();
	
	
	int column=headerList.count();
	int row=list.size()+1; // add one row for the header 
	
	table=cursor.insertTable(row,column,tableFormat);
	
	// initilize table header
	
	
	for(int j=0;j<column;j++)
	{
		QTextTableCell cell=table->cellAt(0,j);
		cell.setFormat(cellBackgroundFormat);
		cursor=cell.firstCursorPosition();
		cursor.insertText(headerList[j],boldCharFormat);
	}
	
	
	// populate table with data..
	
	for(int i=1;i<row;i++)
	{
		for(int j=0;j<column;j++)
		{
			QTextTableCell cell=table->cellAt(i,j);
			cursor=cell.firstCursorPosition();
			cursor.insertText(list[i-1].at(j),redBoldCharFormat);
		}
	}
	
}



void PrayerTimesReport::init()
{
	cursor=textEdit->textCursor();
	rootFrame=cursor.currentFrame();
	
	blockFormat.setLayoutDirection(Qt::RightToLeft);
	centerBlockFormat.setAlignment(Qt::AlignHCenter);
	cursor.setBlockFormat(centerBlockFormat);
	
	rootFrameFormat.setPadding(12);
	rootFrameFormat.setLayoutDirection(Qt::RightToLeft);
	rootFrame->setFrameFormat(rootFrameFormat);
	
	boldCharFormat.setFont(QFont("tahoma"));
	boldCharFormat.setFontWeight(QFont::Bold);
	
	titleCharFormat.setFont(QFont("tahoma",12,QFont::Bold));
	titleCharFormat.setForeground(QColor(Qt::darkBlue));
	
	defaultCharFormat.setFont(QFont("tahoma"));
	defaultCharFormat.setFont(QFont("tahoma",11,QFont::Bold));
	
	greenTitleCharFormat.setFont(QFont("tahoma",11,QFont::Bold));
	greenTitleCharFormat.setForeground(QColor(Qt::darkGreen));
	
	cellBackgroundFormat.setBackground(QColor(Qt::lightGray));
	
	//redBoldCharFormat.setForeground(QColor(Qt::red));
	//redBoldCharFormat.setFontWeight(QFont::Bold);
	redBoldCharFormat.setFont(QFont("tahoma",10));
	
	
	//constraints<<QTextLength(QTextLength::PercentageLength,7);
	//constraints<<QTextLength(QTextLength::PercentageLength,13);
	//constraints<<QTextLength(QTextLength::PercentageLength,20);
	//constraints<<QTextLength(QTextLength::PercentageLength,50);
	//constraints<<QTextLength(QTextLength::PercentageLength,10);

	
	tableFormat.setCellPadding(3);
	tableFormat.setCellSpacing(3);
	tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Ridge);
	tableFormat.setAlignment(Qt::AlignHCenter);
	tableFormat.setColumnWidthConstraints(constraints);

	headerList<<"اليوم"<<"الفجر"<<"الشروق"<<"الظهر"<<"العصر"<<"المغرب"<<"العشاء";

}





void PrayerTimesReport::createMenus()
{
	fileMenu=menuBar()->addMenu("ملف");
	fileMenu->addAction(printAct);
	fileMenu->addSeparator();
	fileMenu->addAction(closeAct);
	
	
}

void PrayerTimesReport::createActions()
{
	printAct=new QAction(QIcon(":/images/printAct.png"),"طباعة",this);
	connect(printAct,SIGNAL(triggered()),this,SLOT(printClicked()));
	
	closeAct=new QAction(QIcon(":/images/closeAct.png"),"اغلاق",this);
	connect(closeAct,SIGNAL(triggered()),this,SLOT(close()));

}



