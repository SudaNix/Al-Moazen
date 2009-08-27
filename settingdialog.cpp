#include "settingdialog.h"
#include "services.h"

#include <QDir>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>


#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>

SettingDialog::SettingDialog(const QStringList& list,QWidget* parent):
QDialog(parent)
{
	setupUi(this);
	m_count=0;
	
	connect(okButton,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
	connect(testSoundButton,SIGNAL(clicked()),this,SLOT(testClicked()));

	this->list = list;
	
	init();
}

	



void SettingDialog::init()
{
	Services services;
	athanComboBox->addItems(services.soundFiles());
	
	
	m_count=list.count();
	
	if( m_count == 0 )
	{
		athanComboBox->setEnabled(false);	
		beforeCheckBox->setEnabled(false);
		afterCheckBox->setEnabled(false);
		return;
	}
	
	if( athanComboBox->findText(list.at(0)) != -1)
		athanComboBox->setCurrentIndex(athanComboBox->findText(list.at(0)));
	else
		athanComboBox->setCurrentIndex(0);
	
	if( list.at(1) == "yes")
		beforeCheckBox->setCheckState(Qt::Checked);
	else
		beforeCheckBox->setCheckState(Qt::Unchecked);
	
	if( list.at(2) == "yes")
		afterCheckBox->setCheckState(Qt::Checked);
	else
		afterCheckBox->setCheckState(Qt::Unchecked);
	
}

int SettingDialog::count()const
{
	return m_count;
}

QString SettingDialog::beforeState()
{
	if( beforeCheckBox->isChecked() )
		return "yes";
	else
		return "no";
}

QString SettingDialog::afterState()
{
	if( afterCheckBox->isChecked() )
		return "yes";
	else
		return "no";
}

QString SettingDialog::soundName()
{
	if( athanComboBox->count() == 0 )
		return "";
	
	return athanComboBox->currentText();
}

void SettingDialog::testClicked()
{
	QDir dir(QDir::currentPath());
	dir.cd("sounds");

	QDialog dlg;
	QPushButton ok("Ok",&dlg);
	QLabel label(soundName());
	//dlg.setSoundFile();
	
	QVBoxLayout layout;
	layout.addWidget(&ok);
	layout.addWidget(&label);

	dlg.setWindowTitle(trUtf8("تجربة الصوت"));
	dlg.setLayout(&layout);

	Phonon::MediaObject* player=new Phonon::MediaObject(&dlg);
	player->setCurrentSource(Phonon::MediaSource(dir.filePath(soundName())));
	Phonon::AudioOutput* audioOutput=new Phonon::AudioOutput(&dlg);
	Phonon::createPath(player,audioOutput);
	
	dlg.exec();

	player->stop();
	delete player;
	delete audioOutput;
	player=0;
	audioOutput=0;
}
