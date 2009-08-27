#include <QDate>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QTime>
#include <QAction>
#include <QMenu>
#include <QCloseEvent>
#include <QDebug>
#include <QSettings>
#include <QTimer>
#include <QApplication>

#include "almoazendialog.h"
#include "placedialog.h"
#include "prayertime.h"
#include "popupdialog.h"
#include "aboutdialog.h"
#include "settingdialog.h"
#include "changetimedialog.h"
#include "azkarpopupdialog.h"
#include "azkardialog.h" 
#include "prayertimesreport.h"


AlMoazenDialog::AlMoazenDialog(QDialog* parent):
QDialog(parent)
{
	setupUi(this);

	readPlaceSettings();
	readProgramSettings();
	readAzkarSettings();
	
	calculatePrayerTime();
	//computeNextPrayerTimeName();
	computeNextPrayerTime();
	
	progressBar->setMinimum(0);
	progressBar->setMaximum(totalSec);

	
	initSystemTrayIcon();
	
	
	placeDialog=0;
	popupDialog=0;
	
	finished = 0;	//progressBar counter
	//totalSec = 0 ;	
	azkarEnabledCounter = m_playAzkarAfter*60;


	QTime time=QTime::currentTime();
	QString stime=time.toString("h:m:s A");
	
	QDate date=QDate::currentDate();
	currentDate=date;
	QString sdate=date.toString("dddd MMM yyyy");
	
	displayPlaceLabel();
	dateLabel->setText(stime+"       "+sdate);

	initPopupButton();

	connect(minimizeButton,SIGNAL(clicked()),this,SLOT(minimizeClicked()));
	connect(aboutButton,SIGNAL(clicked()),this,SLOT(aboutClicked()));
	
	connect(this,SIGNAL(placeChanged()),this,SLOT(calculatePrayerTime()));
	
	startTimer(1000);	// one second
	
	//QTimer* t = new QTimer(this);
	
	//connect(t,SIGNAL(timeout()),this,SLOT(advanceProgressBar()));
	//t->start(1000);
	
	setWindowIcon(QIcon(":/images/home.png"));
	setWindowTitle(trUtf8("Al-Moazen   المؤذن"));
}

AlMoazenDialog::~AlMoazenDialog()
{
	//delete prayerTime;
}

void AlMoazenDialog::advanceProgressBar()
{
	qDebug() << "bar: "<< finished;
	
	progressBar->setValue(++finished);
	//qApp->processEvents();
}

void AlMoazenDialog::readPlaceSettings()
{
	QSettings settings("qt-ar","Al-Moazen");
	
	m_country=settings.value("place_settings/country","Saudi Arabia").toString();
	m_city=settings.value("place_settings/city","Ar Riyad").toString();
	m_longitude=settings.value("place_settings/longitude","46.7825").toString();
	m_latitude=settings.value("place_settings/latitude","24.6505").toString();
	m_timeZone=settings.value("place_settings/timeZone","3").toString();
	m_calender=settings.value("place_settings/calender","UmmAlQuraUniv").toString();
	m_mazhab=settings.value("place_settings/mazhab","Default").toString();
	m_season=settings.value("place_settings/season","Winter").toString();
	m_countryNumber=settings.value("place_settings/countryNumber","198").toString();
	m_cityNumber=settings.value("place_settings/cityNumber","34").toString();

	
}

void AlMoazenDialog::writePlaceSettings()
{
	QSettings settings("qt-ar","Al-Moazen");
	
	settings.setValue("place_settings/country",m_country);
	settings.setValue("place_settings/city",m_city);
	settings.setValue("place_settings/longitude",m_longitude);
	settings.setValue("place_settings/latitude",m_latitude);
	settings.setValue("place_settings/timeZone",m_timeZone);
	settings.setValue("place_settings/calender",m_calender);
	settings.setValue("place_settings/mazhab",m_mazhab);
	settings.setValue("place_settings/season",m_season);
	settings.setValue("place_settings/countryNumber",m_countryNumber);
	settings.setValue("place_settings/cityNumber",m_cityNumber);	
}

void AlMoazenDialog::readProgramSettings()
{
	QSettings settings("qt-ar","Al-Moazen");
	m_soundFileName=settings.value("program_settings/soundFileName","4001.mp3").toString();
	m_alertBeforeFiveMin=settings.value("program_settings/alertBeforeFiveMin","yes").toString();
	m_alertAfterFiveMin=settings.value("program_settings/alertAfterFiveMin","yes").toString();

}

void AlMoazenDialog::writeProgramSettings()
{
	QSettings settings("qt-ar","Al-Moazen");
	settings.setValue("program_settings/soundFileName",m_soundFileName);
	settings.setValue("program_settings/alertBeforeFiveMin",m_alertBeforeFiveMin);
	settings.setValue("program_settings/alertAfterFiveMin",m_alertAfterFiveMin);
	
}

void AlMoazenDialog::readAzkarSettings()
{
	QSettings settings("qt-ar","Al-Moazen");
	m_azkarEnabled=settings.value("azkar_settings/azkarEnabled","yes").toString();
	m_automaticClose=settings.value("azkar_settings/automaticClose","yes").toString();
	m_playAzkarAfter=settings.value("azkar_settings/playAzkarAfter","5").toInt();
	m_closeAzkrAfter=settings.value("azkar_settings/closeAzkrAfter","10").toInt();
}

void AlMoazenDialog::writeAzkarSettings()
{
	QSettings settings("qt-ar","Al-Moazen");
	settings.setValue("azkar_settings/azkarEnabled",m_azkarEnabled);
	settings.setValue("azkar_settings/automaticClose",m_automaticClose);
	settings.setValue("azkar_settings/playAzkarAfter",m_playAzkarAfter);
	settings.setValue("azkar_settings/closeAzkrAfter",m_closeAzkrAfter);
}
		
void AlMoazenDialog::calculatePrayerTime()
{
	PrayerTime prayerTime(m_longitude.toDouble(),m_latitude.toDouble(),m_timeZone.toInt(),QDate::currentDate().day(),QDate::currentDate().month(),QDate::currentDate().year(),m_calender.toStdString(),m_mazhab.toStdString(),m_season.toStdString());
	
	prayerTime.calculate();
	
	displayTimes(prayerTime.fajrTime(),prayerTime.shroukTime(),prayerTime.zuhrTime(),prayerTime.asrTime(),prayerTime.maghribTime(),prayerTime.ishaTime());
	
	computeNextPrayerTimeName();
}

void AlMoazenDialog::displayPlaceLabel()
{
	placeLabel->setText(m_country+" -- "+m_city);
}

void AlMoazenDialog::placeClicked()
{
	QStringList list;
	list << m_country << m_city << m_longitude << m_latitude << m_timeZone << m_calender << m_mazhab << m_season << m_countryNumber << m_cityNumber;
	
	if(!placeDialog)
		placeDialog=new PlaceDialog(list,this);

	if( placeDialog->exec() != QDialog::Accepted )
		return;

	m_country=placeDialog->currentCountry();
	m_city=placeDialog->currentCity();
	m_longitude=QString::number(placeDialog->longitude());
	m_latitude=QString::number(placeDialog->latitude());
	m_timeZone=QString::number(placeDialog->timeZone());
	m_calender=placeDialog->currentCalender();
	m_mazhab=placeDialog->currentMazhab();
	m_season=placeDialog->currentSeason();
	m_countryNumber=QString::number(placeDialog->countryNumber());
	m_cityNumber=QString::number(placeDialog->cityNumber());
	
	displayPlaceLabel();
	writePlaceSettings();

	delete placeDialog;
	placeDialog=0;
	
	emit placeChanged();
	
}

void AlMoazenDialog::displayTimes(double f,double s,double z,double a,double m,double i)
{
	QString fajr=convertIntoTime(f);
	QString shrouk=convertIntoTime(s);
	QString zuhr=convertIntoTime(z);
	QString asr=convertIntoTime(a);
	QString maghrib=convertIntoTime(m);
	QString isha=convertIntoTime(i);
	
	fajrLineEdit->setText(fajr);
	shroukLineEdit->setText(shrouk);
	zuhrLineEdit->setText(zuhr);
	asrLineEdit->setText(asr);
	maghribLineEdit->setText(maghrib);
	ishaLineEdit->setText(isha);
}

QString AlMoazenDialog::convertIntoTime(double var)
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

int AlMoazenDialog::hour(const QString& time)
{
	return time.section(':',0,0).toInt();
}

int AlMoazenDialog::minute(const QString& time)
{
	return time.section(':',1,1).toInt();
}

int AlMoazenDialog::second(const QString& time)
{
	return time.split(" ").at(0).split(":").at(2).toInt();
}

QString AlMoazenDialog::zone(const QString& time)
{
	return time.split(" ").at(1);
}

void AlMoazenDialog::computeNextPrayerTime()
{
	QTime time = QTime::currentTime();
	qDebug() << time.toString();
	QTime nextTime;     // next prayer time.
	QTime total;
	int totalSec =0;
	
	if( nextPrayer == trUtf8("الفجر") )
		total = ishaUntilFajr;
	
	else if( nextPrayer == trUtf8("الظهر") )
		total = fajrUntilZuhr;

	
	else if( nextPrayer == trUtf8("العصر") )
		total = zuhrUntilAsr;
	
	else if( nextPrayer == trUtf8("المغرب") )
	  
		total = asrUntilMaghrib;
	
	else 
		total = maghribUntilIsha;
	
	totalSec = total.hour()*60*60+total.minute()*60+total.second();
	progressBar->setMinimum(0);
	progressBar->setMaximum(totalSec);
	
	downTimer = total;
	//nextPrayerTimeLabel->setText(QString::number(abs(downTimer.hour()-time.hour()))+":"+QString::number(abs(downTimer.minute()-time.minute()))+":"+QString::number(abs(downTimer.second()-time.second())));
	//int p = finished / total;

}

void AlMoazenDialog::computePrayerTimeDifferences(QTime fajrTime,QTime zuhrTime,QTime asrTime,QTime maghribTime,QTime ishaTime)
{
	fajrUntilZuhr = QTime(abs(zuhrTime.hour()-fajrTime.hour()),abs(zuhrTime.minute()-fajrTime.minute()),abs(zuhrTime.second()-fajrTime.second())); 

	zuhrUntilAsr = QTime(abs(asrTime.hour()-zuhrTime.hour()),abs(asrTime.minute()-zuhrTime.minute()),abs(asrTime.second()-zuhrTime.second())); 

	asrUntilMaghrib = QTime(abs(maghribTime.hour()-asrTime.hour()),abs(maghribTime.minute()-asrTime.minute()),abs(maghribTime.second()-asrTime.second())); 

	maghribUntilIsha = QTime(abs(ishaTime.hour()-maghribTime.hour()),abs(ishaTime.minute()-maghribTime.minute()),abs(ishaTime.second()-maghribTime.second())); 
	
	ishaUntilFajr = QTime(abs(abs(ishaTime.hour()-24)+fajrTime.hour()),abs(fajrTime.minute()-ishaTime.minute()),abs(fajrTime.second()-ishaTime.second())); 
}

// Must call after calculate
void AlMoazenDialog::computeNextPrayerTimeName()
{
	QTime time=QTime::currentTime();
	qDebug() << time.toString();
	
	QTime fajrTime(zone(fajrLineEdit->text()) == "AM" ? hour(fajrLineEdit->text()):hour(fajrLineEdit->text())+12,minute(fajrLineEdit->text()),second(fajrLineEdit->text()));
	
	QTime zuhrTime(zone(zuhrLineEdit->text()) == "PM" && hour(zuhrLineEdit->text()) > 12 ? hour(zuhrLineEdit->text())+12:hour(zuhrLineEdit->text()),minute(zuhrLineEdit->text()),second(zuhrLineEdit->text()));
	
	QTime asrTime(zone(asrLineEdit->text()) == "AM" ? hour(asrLineEdit->text()):hour(asrLineEdit->text())+12,minute(asrLineEdit->text()),second(asrLineEdit->text()));
	
	QTime maghribTime(zone(maghribLineEdit->text()) == "AM" ? hour(maghribLineEdit->text()):hour(maghribLineEdit->text())+12,minute(maghribLineEdit->text()),second(maghribLineEdit->text()));
	
	QTime ishaTime(zone(ishaLineEdit->text()) == "AM" ? hour(ishaLineEdit->text()):hour(ishaLineEdit->text())+12,minute(ishaLineEdit->text()),second(ishaLineEdit->text()));
	
	/*qDebug() <<"time: "<<time.toString();
	qDebug() <<"fajr time  : "<< fajrTime.toString();
	qDebug() <<"zuhr time: "<< zuhrTime.toString();
	qDebug() <<"asr time: "<< asrTime.toString();
	qDebug() <<"mag time: "<< maghribTime.toString();
	qDebug() <<"isha time: "<< ishaTime.toString();
	*/

	computePrayerTimeDifferences(fajrTime,zuhrTime,asrTime,maghribTime,ishaTime);
	
	/*
	qDebug() <<"fajrUntilZuhr: "<<fajrUntilZuhr.toString();
	qDebug() <<"zuhrUntilAsr: "<< zuhrUntilAsr.toString();
	qDebug() <<"asrUntilMaghrib: "<< asrUntilMaghrib.toString();
	qDebug() <<"maghribUntilIsha: "<< maghribUntilIsha.toString();
	qDebug() <<"ishaUntilFajr: "<< ishaUntilFajr.toString();
	*/

	if( time >= fajrTime && time < zuhrTime )
	{
		currentPrayer = trUtf8("الفجر");
		nextPrayer = trUtf8("الظهر");
	}
	
	else if( time >= zuhrTime && time < asrTime )
	{
		currentPrayer = trUtf8("الظهر");
		nextPrayer = trUtf8("العصر");
	}
	
	else if( time >= asrTime && time < maghribTime )
	{
		currentPrayer = trUtf8("العصر");
		nextPrayer = trUtf8("المغرب");
	}
	
	else if( time >= maghribTime && time < ishaTime )
	{
		currentPrayer = trUtf8("المغرب");
		nextPrayer = trUtf8("العشاء");
	}
	
	else
	{
		currentPrayer = trUtf8("العشاء");
		nextPrayer = trUtf8("الفجر");
	}
	
	
	prayerNameLabel->setText(nextPrayer);
	//prayerTimeLabel->setText("");

	//qDebug() << "current prayer: "<<currentPrayer;
	//qDebug() << "next prayer: "<<nextPrayer;
	
}

void AlMoazenDialog::timerEvent(QTimerEvent* event)
{
	popupDialog=new PopupDialog(this);
	
	QTime time=QTime::currentTime();
	QString stime=time.toString("h:m:s A");


	  //****************  Azkar pop-up ***************
	 if( azkarEnabledCounter != 0 && m_azkarEnabled == "yes" )
		azkarEnabledCounter--;
		
	 if( azkarEnabledCounter == 0 && m_azkarEnabled == "yes" )
	 {
		qDebug() << "im in: "<< azkarEnabledCounter;
		
		AzkarPopupDialog* dlg=new AzkarPopupDialog(this);
		dlg->setCloseAfter(m_closeAzkrAfter);
		dlg->display();
		
		azkarEnabledCounter = m_playAzkarAfter*60;
	 }
    
	//*************************************************
	
	
	//qDebug() << "downTimer.hour(): " << downTimer.hour();
	//qDebug() << "time.hour(): " << time.hour();

	//int h = hour(time.toString());
	//int m = minute(time.toString());
	//int s = second(time.toString());
	//QString c = zone(time.toString());

	//if( h > 12 )
	//      h-=12;


	//qDebug() << "h: " << h;


	//nextPrayerTimeLabel->setText(QString::number(abs(downTimer.hour()-h))+":"+QString::number(abs(downTimer.minute()-m))+":"+QString::number(abs(downTimer.second()-s)));

	QDate date=QDate::currentDate();
	QString sdate=date.toString("dddd MMM yyyy");
	
	dateLabel->setText(stime+"       "+sdate);
	
	progressBar->setValue(++finished);
	
	// check befor 5 minutes
	if( m_alertBeforeFiveMin == "yes" )
	{
		QTime timebeforeFiveMin = time.addSecs(-300);
		QString sTimebeforeFiveMin=timebeforeFiveMin.toString("h:m:s A");
	
		checkAlarm(sTimebeforeFiveMin,true);
	}
	
	
	// check after 5 minutes
	if( m_alertAfterFiveMin == "yes" )
	{
		QTime timeAfterFiveMin = time.addSecs(300);
		QString sTimeAfterFiveMin=timeAfterFiveMin.toString("h:m:s A");
		
		checkAlarm(sTimeAfterFiveMin,false);
	}
	
	
	
	if( stime == fajrLineEdit->text() )
	{
		popupDialog->display(trUtf8("الفجر"),m_soundFileName);
		currentPrayer = trUtf8("الفجر");
		nextPrayer = trUtf8("الظهر");
		
		computeNextPrayerTimeName();
		finished = 0;
		computeNextPrayerTime();
		progressBar->setValue(finished);
		
	}
	
	else if ( stime == shroukLineEdit->text() )
	{
		//popupDialog->show();
	}
	
	else if ( stime == zuhrLineEdit->text() )
	{
		popupDialog->display(trUtf8("الظهر"),m_soundFileName);
		currentPrayer = trUtf8("الظهر");
		nextPrayer = trUtf8("العصر");
		
		computeNextPrayerTimeName();
		finished = 0;
		computeNextPrayerTime();
		progressBar->setValue(finished);
	}
			
	else if ( stime == asrLineEdit->text() )
	{
		popupDialog->display(trUtf8("العصر"),m_soundFileName);
		currentPrayer = trUtf8("العصر");
		nextPrayer = trUtf8("المغرب");
		computeNextPrayerTimeName();
		finished = 0;
		computeNextPrayerTime();
		progressBar->setValue(finished);
	}
	
	else if ( stime == maghribLineEdit->text() )
	{
		popupDialog->display(trUtf8("المغرب"),m_soundFileName);
		currentPrayer = trUtf8("المغرب");
		nextPrayer = trUtf8("العشاء");
		
		computeNextPrayerTimeName();
		finished = 0;
		computeNextPrayerTime();
		progressBar->setValue(finished);
	}
	
	else if( stime == ishaLineEdit->text() )
	{
		popupDialog->display(trUtf8("العشاء"),m_soundFileName);
		currentPrayer = trUtf8("العشاء");
		nextPrayer = trUtf8("الفجر");
		
		computeNextPrayerTimeName();
		finished = 0;
		computeNextPrayerTime();
		progressBar->setValue(finished);
	}
	
	if( currentDate != date )
	{
		calculatePrayerTime();
		currentDate=date;
	}
	
	//delete popupDialog;
	//popupDialog = 0;
}

void AlMoazenDialog::initSystemTrayIcon()
{
	trayIcon=new QSystemTrayIcon(this);
		
	maximizeAct=new QAction(trUtf8("تكبير"),this);
	connect(maximizeAct,SIGNAL(triggered()),this,SLOT(showMaximized()));
		
	minimizeAct=new QAction(trUtf8("تصغير"),this);
	connect(minimizeAct,SIGNAL(triggered()),this,SLOT(hide()));
	
			
	quitAct=new QAction(QIcon(":/images/exit.png"),trUtf8("خروج"),this);
	connect(quitAct,SIGNAL(triggered()),this,SLOT(quit()));
		
	restoreAct=new QAction(QIcon(":/images/restore.png"),trUtf8("استعادة"),this);
	connect(restoreAct,SIGNAL(triggered()),this,SLOT(showNormal()));
		
	menu=new QMenu(this);
	menu->addAction(maximizeAct);
	menu->addAction(minimizeAct);
	menu->addAction(restoreAct);
	menu->addSeparator();
	menu->addAction(quitAct);
		
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
		
		
	trayIcon->setContextMenu(menu);
	trayIcon->setIcon(QIcon(":/images/home.png"));
	trayIcon->setToolTip(trUtf8("برنامج المؤذن"));
	trayIcon->show();
	

}

void AlMoazenDialog::closeEvent(QCloseEvent* event)
{
	if (trayIcon->isVisible()) 
	{
		hide();
		event->ignore();
	}
}


void AlMoazenDialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
     switch (reason)
	 {
    	case QSystemTrayIcon::DoubleClick:
			showNormal();
        	break;

    	default:
        	;
    }
}

void AlMoazenDialog::minimizeClicked()
{
	close();
}


void AlMoazenDialog::aboutClicked()
{
	AboutDialog dlg;
	dlg.exec();
}

void AlMoazenDialog::quit()
{
	QDir dir(QDir::currentPath());
	dir.cd("configures");
	
	dir.remove("run.dat");
	
	qApp->quit();
}

void AlMoazenDialog::initPopupButton()
{
	popupMenu=new QMenu(this);
	
	placeSettingsAct=new QAction(QIcon(":/images/epiphany.png"),trUtf8("اعدادات المنطقة"),this);
	changeTimeSettingsAct=new QAction(QIcon(":/images/config.png"),trUtf8("الضبط اليدوي"),this);
	azanSettingsAct=new QAction(QIcon(":/images/multimedia.png"),trUtf8("اعدادات التذكير"),this);
	azkarAlsabahSettingsAct=new QAction(QIcon(":/images/package_editors.png"),trUtf8("عرض أذكار الصباح"),this);
	azkarAlmasaSettingsAct=new QAction(QIcon(":/images/package_editors.png"),trUtf8("عرض أذكار المساء"),this);
	azkarAlsabahSoundSettingsAct=new QAction(QIcon(":/images/multimedia.png"),trUtf8("استماع الى أذكار الصباح"),this);
	azkarAlmasaSoundSettingsAct=new QAction(QIcon(":/images/multimedia.png"),trUtf8("استماع الى أذكار المساء"),this);

	azkarSettingsAct=new QAction(QIcon(":/images/Utilities.png"),trUtf8("إعدادات الأذكار"),this);
	printTimeSettingsAct=new QAction(QIcon(":/images/calendar.png"),trUtf8("طباعة المواقيت"),this);
	//qiblaSettingsAct=new QAction(QIcon(":/images/config.png"),trUtf8("سمت القبلة"),this);

	
	connect(placeSettingsAct,SIGNAL(triggered()),this,SLOT(placeClicked()));
	connect(azanSettingsAct,SIGNAL(triggered()),this,SLOT(settingClicked()));
	connect(changeTimeSettingsAct,SIGNAL(triggered()),this,SLOT(changeTimeClicked()));
	
	connect(azkarAlsabahSettingsAct,SIGNAL(triggered()),this,SLOT(displayAzkarAlsabah()));
	connect(azkarAlmasaSettingsAct,SIGNAL(triggered()),this,SLOT(displayAzkarAlmasa()));
	connect(azkarAlsabahSoundSettingsAct,SIGNAL(triggered()),this,SLOT(displayAzkarAlsabahSound()));
	connect(azkarAlmasaSoundSettingsAct,SIGNAL(triggered()),this,SLOT(displayAzkarAlmasaSound()));

	connect(azkarSettingsAct,SIGNAL(triggered()),this,SLOT(azkarClicked()));
	connect(printTimeSettingsAct,SIGNAL(triggered()),this,SLOT(printTimeClicked()));
	//connect(qiblaSettingsAct,SIGNAL(triggered()),this,SLOT(qiblaClicked()));
	
	popupMenu->addAction(placeSettingsAct);
	popupMenu->addAction(azanSettingsAct);
	popupMenu->addAction(changeTimeSettingsAct);
	azkarMenu = popupMenu->addMenu(trUtf8("الأذكار"));
	popupMenu->addAction(printTimeSettingsAct);
	//popupMenu->addAction(qiblaSettingsAct);

	azkarMenu->addAction(azkarAlsabahSettingsAct);
	azkarMenu->addAction(azkarAlmasaSettingsAct);
	azkarMenu->addAction(azkarAlsabahSoundSettingsAct);
	azkarMenu->addAction(azkarAlmasaSoundSettingsAct);
	azkarMenu->addAction(azkarSettingsAct);

		
	
	configureButton->setMenu(popupMenu);
}

void AlMoazenDialog::settingClicked()
{
	SettingDialog settingDialog(QStringList() << m_soundFileName << m_alertBeforeFiveMin << m_alertAfterFiveMin);
	
	if( settingDialog.exec() != QDialog::Accepted )
		return;
	
	m_soundFileName =settingDialog.soundName();
	m_alertBeforeFiveMin=settingDialog.beforeState();
	m_alertAfterFiveMin=settingDialog.afterState();

	writeProgramSettings();
}

void AlMoazenDialog::changeTimeClicked()
{
	ChangeTimeDialog dlg(QStringList() << fajrLineEdit->text() << shroukLineEdit->text() << zuhrLineEdit->text() << asrLineEdit->text() << maghribLineEdit->text() << ishaLineEdit->text(),this);
	
	if( dlg.exec() != QDialog::Accepted )
		return;
		
	
	// TODO: put news time in local var and recompute and display.
	calculatePrayerTime();
}

void AlMoazenDialog::checkAlarm(const QString& stime,bool isBefore)
{
	if(!popupDialog)
		return;
	
	if( stime == fajrLineEdit->text() )
		popupDialog->display(trUtf8(" الفجر"),isBefore);

	
	else if ( stime == zuhrLineEdit->text() )
		popupDialog->display(trUtf8(" الظهر"),isBefore);
	
	else if ( stime == asrLineEdit->text() )
		popupDialog->display(trUtf8(" العصر"),isBefore);

	else if ( stime == maghribLineEdit->text() )
		popupDialog->display(trUtf8(" المغرب"),isBefore);

	else if( stime == ishaLineEdit->text() )
		popupDialog->display(trUtf8(" العشاء"),isBefore);
		
}


void AlMoazenDialog::displayAzkarAlsabah()
{
	AzkarPopupDialog* dlg=new AzkarPopupDialog;
	dlg->displayAzkarAlsabah();
	
}

void AlMoazenDialog::displayAzkarAlmasa()
{
	AzkarPopupDialog* dlg=new AzkarPopupDialog;
	dlg->displayAzkarAlmasa();
}

void AlMoazenDialog::azkarClicked()
{
	AzkarDialog dlg;
	
	// read previous settings
	dlg.setAzkarEnable(m_azkarEnabled);
	dlg.setAutomaticClose(m_automaticClose);
	dlg.setPlayAzkarAfter(m_playAzkarAfter);
	dlg.setCloseAzkarAfter(m_closeAzkrAfter);
	
	if( dlg.exec() != QDialog::Accepted )
		return;
	
	m_azkarEnabled = dlg.azkarEnabled();
	m_automaticClose = dlg.automaticClose();
	m_playAzkarAfter = dlg.playAzkarAfter();
	m_closeAzkrAfter = dlg.closeAzkrAfter();
	
	writeAzkarSettings();
	
	azkarEnabledCounter = m_playAzkarAfter*60;
}

void AlMoazenDialog::printTimeClicked()
{
	PrayerTimesReport* r=new PrayerTimesReport(QStringList() << m_longitude << m_latitude << m_timeZone << m_calender << m_mazhab << m_season ,this);
	r->show();
}

/*
void AlMoazenDialog::qiblaClicked()
{
	QMessageBox::information(this,"not avialable","Not Implemented until now.");
}
*/

void AlMoazenDialog::displayAzkarAlsabahSound()
{
	AzkarPopupDialog* dlg=new AzkarPopupDialog;
	dlg->displayAzkarAlsabahSound();
}




void AlMoazenDialog::displayAzkarAlmasaSound()
{
	AzkarPopupDialog* dlg=new AzkarPopupDialog;
	dlg->displayAzkarAlmasaSound();
}
