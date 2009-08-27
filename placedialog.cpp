#include <QtSql>
#include <QDebug>
#include <QMessageBox>

#include "placedialog.h"


PlaceDialog::PlaceDialog(QStringList args,QDialog* parent):
QDialog(parent)
{
	setupUi(this);
	defaultArgs=args;


	connect(countriesListWidget,SIGNAL(currentTextChanged(const QString&)),this,SLOT(populateCitiesListWidget(const QString&)));
	
	connect(citiesListWidget,SIGNAL(currentTextChanged(const QString&)),this,SLOT(populateInfo(const QString&)));
	
	connect(okButton,SIGNAL(clicked()),this,SLOT(okClicked()));
	connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancelClicked()));

	populateCountriesListWidget(args.at(8)); // populate list widget with countries.
	setPreviousSettings(args);  // load file config just for the first appear of PlaceDialog Object.

}


void PlaceDialog::okClicked()
{
	// compute country number.
	QString country=currentCountry();
	countryNo=countryNumber(country)-1;
	
	// compute city number.
	QString city=currentCity();
	cityNo=cityNumber(city);
	
	
	this->accept();
}


void PlaceDialog::cancelClicked()
{
	// restore desfualt settings
	countriesListWidget->setCurrentRow(defaultArgs.at(8).toInt());
	citiesListWidget->setCurrentRow(defaultArgs.at(9).toInt());
	
	this->reject();
}

void PlaceDialog::populateCountriesListWidget(const QString& arg)
{
	populateCountriesList();
	
	for(int i=0;i<countriesList.count();++i)
		countriesListWidget->addItem(countriesList.at(i));

	countriesListWidget->setCurrentRow(arg.toInt());

}


void PlaceDialog::populateCountriesList()
{
	countriesList.clear();

	QSqlQuery query;
	if(query.exec("select countryName from countriesTable") == false )
	{
		QMessageBox::critical(0, "Error",tr("Unable to read country from database File.\n"
		"Click Cancel to exit.\nError: %1").arg(query.lastError().text()),QMessageBox::Cancel);
		return;
	}
	
	while( query.next() )
	{
		countriesList.append(query.value(0).toString());
	}

}


void PlaceDialog::populateCitiesListWidget(const QString& country)
{
	citiesListWidget->clear();

	int tmp=countryNumber(country);

	QSqlQuery query;
	query.prepare("select cityName from citiesTable where countryNO=?");
	query.addBindValue(tmp);
	
	if(query.exec() == false)
	{
		QMessageBox::critical(0, "Error","Unable to read city from database File.\n"
		"Click Cancel to exit.", QMessageBox::Cancel);
		return;
	}
	
	while( query.next() )
	{

		citiesListWidget->addItem(query.value(0).toString());
	}

	citiesListWidget->setCurrentRow(0);
}

int PlaceDialog::countryNumber(const QString& country)
{
	QSqlQuery query;
	query.prepare("select countryNO from countriesTable where countryName=?");
	query.addBindValue(country);
	query.exec();
	
	query.next();
	
	return query.value(0).toInt();
}


int PlaceDialog::minCityNumber(const QString& city)
{
	QSqlQuery query;
	query.prepare("select min(cityNO) from citiesTable where countryNO=?");
	query.addBindValue(toCountry(city));
	query.exec();
	
	query.next();
	// group by countryName
	return query.value(0).toInt();
}



int PlaceDialog::toCountry(const QString& city)
{
	QSqlQuery query;
	query.prepare("select countryNO from citiesTable where cityName=?");
	query.addBindValue(city);
	query.exec();
	
	query.next();
	
	return query.value(0).toInt();
}



int PlaceDialog::cityNumber(const QString& city)
{
	
	QSqlQuery query;
	query.prepare("select cityNO from citiesTable where cityName=?");
	query.addBindValue(city);
	query.exec();
	
	query.next();
	
	return query.value(0).toInt() - minCityNumber(city) ;
}


void PlaceDialog::populateInfo(const QString& city)
{
	if( city.isEmpty() )
		return;

	QSqlQuery query;
	query.prepare("select latitude,longitude,timeZone from citiesTable where cityName=?");
	query.addBindValue(city);
	query.exec();
	
	query.first();

	latitudeLineEdit->setText(QString::number((query.value(0).toDouble())/10000));
	longitudeLineEdit->setText(QString::number((query.value(1).toDouble())/10000));
	zoneLineEdit->setText(QString::number((query.value(2).toDouble())/100));
}

double PlaceDialog::longitude()const
{
	return longitudeLineEdit->text().toDouble();
}

double PlaceDialog::latitude()const
{
	return latitudeLineEdit->text().toDouble();
}

int PlaceDialog::timeZone()const
{
	return zoneLineEdit->text().toInt();
}
		
PrayerTime::Mazhab PlaceDialog::mazhab()const
{
	if( mazhabComboBox->currentIndex() == 0 )
		return PrayerTime::Default;
	else
		return PrayerTime::Hanafi;
}

PrayerTime::Calender PlaceDialog::calender()const
{
	PrayerTime::Calender tmp=PrayerTime::UmmAlQuraUniv;
	
	if(calenderComboBox->currentIndex() == 0 )
		tmp=PrayerTime::UmmAlQuraUniv;
		
	else if(calenderComboBox->currentIndex() == 1 )
		tmp=PrayerTime::EgytionGeneralAuthorityofSurvey;
			
	else if(calenderComboBox->currentIndex() == 2 )
		tmp=PrayerTime::UnivOfIslamicScincesKarachi;
		
	else if(calenderComboBox->currentIndex() == 3 )
		tmp=PrayerTime::IslamicSocietyOfNorthAmerica;
			
	else if(calenderComboBox->currentIndex() == 4 )
		tmp=PrayerTime::MuslimWorldLeague;
		
	return tmp;
}

PrayerTime::Season PlaceDialog::season()const
{
	if (winterRadioButton->isChecked() )
		return PrayerTime::Winter;
	else
		return PrayerTime::Summer;
}

QString PlaceDialog::currentCountry()const
{
	return countriesListWidget->currentItem()->text();
}

QString PlaceDialog::currentCity()const
{
	return citiesListWidget->currentItem()->text();
}

QString PlaceDialog::currentMazhab()const
{
	if( mazhabComboBox->currentIndex() == 0 )
		return "Default";
	else
		return "Hanafi";
}

QString PlaceDialog::currentCalender()const
{
	QString tmp;
	
	if(calenderComboBox->currentIndex() == 0 )
		tmp="UmmAlQuraUniv";
		
	else if(calenderComboBox->currentIndex() == 1 )
		tmp="EgytionGeneralAuthorityofSurvey";
			
	else if(calenderComboBox->currentIndex() == 2 )
		tmp="UnivOfIslamicScincesKarachi";
		
	else if(calenderComboBox->currentIndex() == 3 )
		tmp="IslamicSocietyOfNorthAmerica";
			
	else if(calenderComboBox->currentIndex() == 4 )
		tmp="MuslimWorldLeague";
		
	return tmp;
}

QString PlaceDialog::currentSeason()const
{
	if (winterRadioButton->isChecked() )
		return "Winter";
	else
		return "Summer";
}

void PlaceDialog::setPreviousSettings(QStringList args)
{

	// execute configuration
	countriesListWidget->setCurrentRow(args.at(8).toInt());
	
	citiesListWidget->setCurrentRow(args.at(9).toInt());
	
	// set Calender
	if( args.at(5) == "UmmAlQuraUniv" )
		calenderComboBox->setCurrentIndex(0);
		
	else if( args.at(5) == "EgytionGeneralAuthorityofSurvey" )
		calenderComboBox->setCurrentIndex(1);
			
	else if( args.at(5) == "UnivOfIslamicScincesKarachi" )
		calenderComboBox->setCurrentIndex(2);
		
	else if( args.at(5) == "IslamicSocietyOfNorthAmerica" )
		calenderComboBox->setCurrentIndex(3);
			
	else if( args.at(5) == "MuslimWorldLeague" )
		calenderComboBox->setCurrentIndex(4);
		

	// set Mazhab.
	if( args.at(6) == "Default" )
		mazhabComboBox->setCurrentIndex(0);
	else
		mazhabComboBox->setCurrentIndex(1);
		
	// set Time.
	if ( args.at(7) == "Winter" )
		winterRadioButton->setChecked(true);
	else
		summerRadioButton->setChecked(true);
		
}




int PlaceDialog::countryNumber()const
{
	
	return countryNo;
}

int PlaceDialog::cityNumber()const
{
	return cityNo;
}
