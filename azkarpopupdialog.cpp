#include <QTextEdit>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QTextOption>
#include <QtSql>
#include <QTextCursor>

#include <ctime>
#include <cstdlib>

#include "azkarpopupdialog.h"

AzkarPopupDialog::AzkarPopupDialog(QWidget* parent):
QDialog(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);

	player = 0;
	audioOutput = 0;

	srand(time(0));
	
	timer = 0;
	closeAfter = 0;
	
	QDesktopWidget* desktop=QApplication::desktop();
	int screenWidth=desktop->width();
	int screenHeight=desktop->height();

	setFixedSize(QSize(300,200));
	move(screenWidth-width(),screenHeight-height());
	
	textEdit = new QTextEdit;
	textEdit->setReadOnly(true);
	
	QTextOption option=textEdit->document()->defaultTextOption();
	option.setAlignment(Qt::AlignRight);
	textEdit->document()->setDefaultTextOption(option);
	
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(textEdit);

}

AzkarPopupDialog::~AzkarPopupDialog()
{
	if(player)
	{	
		player->stop();
		delete player;
		delete audioOutput;
		player=0;
		audioOutput=0;
	}
}

void AzkarPopupDialog::setCloseAfter(int value)
{
	  closeAfter = value;
}

void AzkarPopupDialog::display()
{
	// number of records
	QSqlQuery query;
	query.exec("select count(*) from azkar");
	
	query.next();
	
	int number = query.value(0).toInt();
	
	// compute random number.
	int zkerNo = rand()%number + 1;  
	
	query.prepare("select title,zker from azkar where no=?");
	query.addBindValue(zkerNo);
	query.exec();
	
	query.next();
	
	QString title = query.value(0).toString();
	QString zker = query.value(1).toString();


	textEdit->setText(zker);
	setWindowTitle(title);
	
	QTextCursor c = textEdit->textCursor();
	c.movePosition(QTextCursor::Start);
	textEdit->setTextCursor(c);
	
	show();
	
	if( closeAfter )
		startTimer(1000);
}

void AzkarPopupDialog::displayAzkarAlsabah()
{

	QSqlQuery query;
	query.exec("select zker from azkarAlsabah");
	
	while( query.next() )
	{
		textEdit->append(query.value(0).toString());
		textEdit->append("\n\n");
	}
	 
	
	setWindowTitle(trUtf8("أذكار الصباح"));
	show();
	
	
	QTextCursor c = textEdit->textCursor();
	c.movePosition(QTextCursor::Start);
	textEdit->setTextCursor(c);
}

void AzkarPopupDialog::displayAzkarAlmasa()
{
	QSqlQuery query;
	query.exec("select zker from azkarAlmasa");
	
	while( query.next() )
	{
		textEdit->append(query.value(0).toString());
		textEdit->append("\n\n");
	}
	 
	
	setWindowTitle(trUtf8("أذكار المساء"));
	
	show();
	//startTimer(1000);
	
	QTextCursor c = textEdit->textCursor();
	c.movePosition(QTextCursor::Start);
	textEdit->setTextCursor(c);
}


void AzkarPopupDialog::timerEvent(QTimerEvent* event)
{
	if( timer++ != closeAfter )
		return;
		
	timer = 0;
	close();
}


void AzkarPopupDialog::displayAzkarAlmasaSound()
{
	QDir dir(QDir::currentPath());
	dir.cd("azkar");
	
	player=new Phonon::MediaObject(this);
	player->setCurrentSource(Phonon::MediaSource(dir.filePath("masa.mp3")));
	audioOutput=new Phonon::AudioOutput(this);
	Phonon::createPath(player,audioOutput);
	

	setWindowTitle(trUtf8("أذكار المساء الصوتية"));
	
	textEdit->setText(trUtf8("بصوت القارئ: مشاري العفاسي"));

	QTextCursor c = textEdit->textCursor();
	c.movePosition(QTextCursor::Start);
	textEdit->setTextCursor(c);

	show();
	player->play();
	//startTimer(1000);
	
}

void AzkarPopupDialog::displayAzkarAlsabahSound()
{
	QDir dir(QDir::currentPath());
	dir.cd("azkar");
	
	player=new Phonon::MediaObject(this);
	player->setCurrentSource(Phonon::MediaSource(dir.filePath("sabah.mp3")));
	audioOutput=new Phonon::AudioOutput(this);
	Phonon::createPath(player,audioOutput);
	

	setWindowTitle(trUtf8("أذكار الصباح الصوتية"));
	
	textEdit->setText(trUtf8("بصوت القارئ: مشاري العفاسي"));

	QTextCursor c = textEdit->textCursor();
	c.movePosition(QTextCursor::Start);
	textEdit->setTextCursor(c);

	show();
	player->play();
	//startTimer(1000);
	
}