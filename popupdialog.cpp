#include <QApplication>
#include <QDesktopWidget>
#include <QSound>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDir>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QTextOption>

#include "popupdialog.h"

PopupDialog::PopupDialog(QWidget* parent):
QDialog(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
	//Qt::Popup|Qt::FramelessWindowHint
	//sound=0;
	player = 0;
	audioOutput = 0;

	QDesktopWidget* desktop=QApplication::desktop();
	int screenWidth=desktop->width();
	int screenHeight=desktop->height();

	setFixedSize(QSize(300,200));
	move(screenWidth-width(),screenHeight-height());

	textEdit=new QTextEdit;
	textEdit->setReadOnly(true);
	
	QTextOption option=textEdit->document()->defaultTextOption();
	option.setAlignment(Qt::AlignRight);
	textEdit->document()->setDefaultTextOption(option);
					  
	QHBoxLayout* layout=new QHBoxLayout;
	layout->addWidget(textEdit);

	setLayout(layout);
	
} 

void PopupDialog::playSound(const QString& wav)
{
	QDir dir(QDir::currentPath());
	dir.cd("sounds");

	//if(!sound)
	//	sound=new QSound(dir.filePath(wav),this);

	//if( !QSound::isAvailable() )
	//	QMessageBox::warning(this,"Error","Sound is not support");

	player=new Phonon::MediaObject(this);
	player->setCurrentSource(Phonon::MediaSource(dir.filePath(wav)));
	audioOutput=new Phonon::AudioOutput(this);
	Phonon::createPath(player,audioOutput);
	player->play();
	
	//else
	//	sound->play();

}

PopupDialog::~PopupDialog()
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


/*void PopupDialog::closeEvent(QCloseEvent* event)
{


	if(sound)
	{	
		sound->stop();
		delete sound;
		sound=0;
	}
	
	
	//close();
	event->accept();
}
*/



void PopupDialog::display(const QString& sala,const QString& sound)
{
	setWindowTitle(trUtf8("حان وقت الصلاة"));
	textEdit->clear();
	textEdit->append(trUtf8("فضلا ، اغلق ما تستمع اليه الان"));
	textEdit->append(trUtf8("واستمع الى الاذان فقد حان وقت صلاة " )+ sala);

	show();

	if( sound.isEmpty() )
		return;

	playSound(sound);
}

void PopupDialog::display(const QString& sala,bool isBefore)
{
	setWindowTitle(trUtf8("انتبه"));
	textEdit->clear();
	
	if( !isBefore )
		textEdit->append(trUtf8("تبقى خمس دقائق حتى صلاة ")+sala);
	else
		textEdit->append(trUtf8("مضت خمس دقائق منذ اذان صلاة ")+sala);

	show();
}
