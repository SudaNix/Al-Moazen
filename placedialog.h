#ifndef PLACEDIALOG_H
#define PLACEDIALOG_H

#include <QDialog>
#include <ui_placedialog.h>
#include "prayertime.h"

class PlaceDialog:public QDialog,private Ui::PlaceDialog
{
	Q_OBJECT
	public:
		PlaceDialog(QStringList,QDialog* =0);
		
		double longitude()const;
		double latitude()const;
		int timeZone()const;
		
		PrayerTime::Mazhab mazhab()const;
		PrayerTime::Calender calender()const;
		PrayerTime::Season season()const;
		
		QString currentCountry()const;
		QString currentCity()const;
		QString currentMazhab()const;
		QString currentCalender()const;
		QString currentSeason()const;
		
		int countryNumber()const;
		int cityNumber()const;
		
	public slots:
		void populateCitiesListWidget(const QString&);
		void populateInfo(const QString&);
		void okClicked();
		void cancelClicked();
		
	private:
		void populateCountriesListWidget(const QString&);
		void populateCountriesList();
		void setPreviousSettings(QStringList);  //  set the previous configuration.
		
		int countryNumber(const QString&);
		int cityNumber(const QString&);
		
		int toCountry(const QString& city);
		int minCityNumber(const QString& city);
		
		int countryNo;
		int cityNo;
		
		QStringList countriesList;
		QStringList defaultArgs;
	
};

#endif
