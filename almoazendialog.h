#ifndef ALMOAZENDIALOG_H
#define ALMOAZENDIALOG_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QDate>

#include "ui_almoazendialog.h"

class PlaceDialog;
class PrayerTime;
class PopupDialog;
class QMenu;
class QAction;

class AlMoazenDialog:public QDialog,private Ui::AlMoazenDialog
{
	Q_OBJECT
	public:
		AlMoazenDialog(QDialog* =0);
		~AlMoazenDialog();

	public slots:
		void placeClicked();
		void settingClicked();
		void changeTimeClicked();
		
		void minimizeClicked();
		void aboutClicked();
		void quit();

		void iconActivated(QSystemTrayIcon::ActivationReason);
		
		void calculatePrayerTime();
		void displayPlaceLabel();
		
		void displayAzkarAlsabah();
		void displayAzkarAlmasa();
		void displayAzkarAlsabahSound();
		void displayAzkarAlmasaSound();

		void azkarClicked();
		void printTimeClicked();
		//void qiblaClicked();
				
	signals:
		void placeChanged();
		
	protected:
		void timerEvent(QTimerEvent*);
		void closeEvent(QCloseEvent* event);

	private:
		void readPlaceSettings();
		void writePlaceSettings();
		
		void readProgramSettings();
		void writeProgramSettings();
		
		void readAzkarSettings();
		void writeAzkarSettings();
		
		
		void displayTimes(double,double,double,double,double,double); //display the prayer times.
		QString convertIntoTime(double); // convert the decimal value of prayer time to HH:MM .
		void checkAlarm(const QString& stime,bool isBefore);

		void initSystemTrayIcon();
		void initPopupButton();

		void computeNextPrayerTimeName();
		void computePrayerTimeDifferences(QTime,QTime,QTime,QTime,QTime);
		void computeNextPrayerTime();
                void calcNextPrayer();
		
		
		
		int hour(const QString& time);
		int minute(const QString& time);
		int second(const QString& time);
		QString zone(const QString& time);
		
		PlaceDialog* placeDialog;
		QStringList currentArgs;

		QDate currentDate;
	
		PrayerTime* prayerTime;
		PopupDialog* popupDialog;
		
		//System tray reqiurements
		QSystemTrayIcon* trayIcon;
		QMenu* menu;
		QAction* maximizeAct;
		QAction* minimizeAct;
		QAction* quitAct;
		QAction* restoreAct;

		QMenu* popupMenu;
		QMenu* azkarMenu;
		
		QAction* placeSettingsAct;
		QAction* azanSettingsAct;
		QAction* changeTimeSettingsAct;
		//QAction* azkarAct;
		QAction* azkarAlsabahSettingsAct;
		QAction* azkarAlmasaSettingsAct;
		QAction* azkarAlsabahSoundSettingsAct;
		QAction* azkarAlmasaSoundSettingsAct;

		QAction* azkarSettingsAct;
		QAction* printTimeSettingsAct;
		//QAction* qiblaSettingsAct;
		
		bool beforeIsChecked;
		bool afterIsChecked;
		QString currentSoundName;
		
		QString currentPrayer;
		QString nextPrayer;
		int finished;
		int totalSec;
		
		QTime fajrUntilZuhr;
		QTime zuhrUntilAsr;
		QTime asrUntilMaghrib;
		QTime maghribUntilIsha;
		QTime ishaUntilFajr;
	
		QTime downTimer;

		int azkarEnabledCounter;
		
		// vaiable for saving current settings
		QString m_country;
		QString m_city;
		QString m_longitude;
		QString m_latitude;
		QString m_timeZone;
		QString m_calender;
		QString m_mazhab;
		QString m_season;
		QString m_countryNumber;
		QString m_cityNumber;

		QString m_soundFileName;
		QString m_alertBeforeFiveMin;
		QString m_alertAfterFiveMin;
		
		QString m_azkarEnabled;
		QString m_automaticClose;
		int m_playAzkarAfter;
		int m_closeAzkrAfter;

};

#endif  // ALMOAZENDIALOG_H

