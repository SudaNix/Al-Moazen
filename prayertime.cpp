/*
The Algorithms in this Code are taken from: http://qasweb.org , and implemented by me.

This file is part of Al-moathen software, and contain the API for calculate prayer times.
and it license under GNU/GPL v2.

Project start in : 9/7/2008
Update start in : 25/3/2009
*/


#include <cmath>
//#include <cstring>
#include "prayertime.h"

using namespace std;

// Default Constructor.
PrayerTime::PrayerTime():
DegToRad(0.017453292),RadToDeg(57.29577951)
{
	// FIXME : Calender,Mazhab and Season should be detected automatically.
	PrayerTime::setData(0,0,0,0,0,0,PrayerTime::UmmAlQuraUniv,PrayerTime::Default,PrayerTime::Winter);
}


PrayerTime::PrayerTime(double lot,double lat,int zone,int day,int month,int year,Calender calender,Mazhab mazhab,Season season):
DegToRad(0.017453292),RadToDeg(57.29577951)
{
	PrayerTime::setData(lot,lat,zone,day,month,year,calender,mazhab,season);
}


PrayerTime::PrayerTime(double lot,double lat,int zone,int day,int month,int year,string calender,string mazhab,string season):
DegToRad(0.017453292),RadToDeg(57.29577951)
{
	setData(lot,lat,zone,day,month,year,calender,mazhab,season);
}

// calculate prayer time.
void PrayerTime::calculate()
{
	double julianDay=(367*m_year)-int(((m_year+int((m_month+9)/12))*7)/4)+int(275*m_month/9)+m_day-730531.5;

	
	double sunLength=280.461+0.9856474*julianDay;
	sunLength=removeDublication(sunLength);
	
	
	double middleSun=357.528+0.9856003*julianDay;
	middleSun=removeDublication(middleSun);
	
	double lambda=sunLength+1.915*sin(middleSun*DegToRad)+0.02*sin(2*middleSun*DegToRad);
	lambda=removeDublication(lambda);
	
	double obliquity=23.439-0.0000004*julianDay;
	
	double alpha=RadToDeg*atan(cos(obliquity*DegToRad)*tan(lambda*DegToRad));
	if( lambda > 90 && lambda < 180 )
		alpha+=180;
	
	else if( lambda > 180 && lambda < 360 )
		alpha+=360;
	
	
	double ST=100.46+0.985647352*julianDay;
	ST=removeDublication(ST);
	
	
	dec=RadToDeg*asin(sin(obliquity*DegToRad)*sin(lambda*DegToRad));
	
	double noon=alpha-ST;
	if(noon < 0 )
		noon+=360;
	
	double UTNoon=noon-m_longitude;
	
	double localNoon=(UTNoon/15)+m_zone;

	
	zuhr=localNoon; //Zuhr Time.
	

	maghrib=localNoon+equation(-0.8333)/15;  // Maghrib Time
	shrouk=localNoon-equation(-0.8333)/15;   // Shrouk Time
	
	double fajrAlt;
	double ishaAlt;
	
	if( m_calender == UmmAlQuraUniv )
		fajrAlt=-19;
	
	else if ( m_calender == EgytionGeneralAuthorityofSurvey )
	{
		fajrAlt=-19.5;
		ishaAlt=-17.5;
	}

	else if ( m_calender == MuslimWorldLeague )	
	{
		fajrAlt=-18;
		ishaAlt=-17;
	}
	
	else if ( m_calender == IslamicSocietyOfNorthAmerica )
		fajrAlt=ishaAlt=-15;


	
	else if ( m_calender == UnivOfIslamicScincesKarachi)
		fajrAlt=ishaAlt=-18;
	

	
	fajr=localNoon-equation(fajrAlt)/15;  // Fajr Time
	

	isha=localNoon+equation(ishaAlt)/15;  // Isha Time

	if( m_calender == UmmAlQuraUniv )
		isha=maghrib+1.5;
	
	double asrAlt;
	
	if( m_mazhab == Hanafi)
		asrAlt=90-RadToDeg*atan(2+tan(abs(m_latitude-dec)*DegToRad));
	
	else
		asrAlt=90-RadToDeg*atan(1+tan(abs(m_latitude-dec)*DegToRad));
		
	asr=localNoon+equation(asrAlt)/15;
	
	if( m_season == Summer)
	{
		fajr+=1;
		shrouk+=1;
		zuhr+=1;
		asr+=1;
		maghrib+=1;
		isha+=1;
	}
}


double PrayerTime::removeDublication(double var)
{
	// TODO: use mod operator %.
	if(var > 360 )
	{
		var/=360;
		
		var-=int(var);
		
		var*=360;
		
	}
	
	return var;
}

double PrayerTime::equation(double alt)
{
	return RadToDeg*acos((sin(alt*DegToRad)-sin(dec*DegToRad)*sin(m_latitude*DegToRad))/(cos(dec*DegToRad)*cos(m_latitude*DegToRad)));
}

double PrayerTime::abs(double var)
{
	if( var < 0 )
		return -var;
	else
		return var;
}

void PrayerTime::setDate(int day,int month,int year)
{
	m_day=day;
	m_month=month;
	m_year=year;
}

void PrayerTime::setYear(int year)
{
	m_year=year;
}

void PrayerTime::setMonth(int month)
{
	m_month=month;
}

void PrayerTime::setDay(int day)
{
	m_day=day;
}

void PrayerTime::setData(double lot,double lat,int zone,int day,int month,int year,Calender calender,Mazhab mazhab,Season season)
{
	m_longitude=lot;
	m_latitude=lat;
	m_zone=zone;
		
	m_day=day;
	m_month=month;
	m_year=year;

	m_calender=calender;
	m_mazhab=mazhab;
	m_season=season;
}


void PrayerTime::setData(double lot,double lat,int zone,int day,int month,int year,string calender,string mazhab,string season)
{
	m_longitude=lot;
	m_latitude=lat;
	m_zone=zone;
		

	m_day=day;
	m_month=month;
	m_year=year;

	m_calender = strToCalender(calender);
	m_mazhab = strToMazhab(mazhab);
	m_season = strToSeason(season);
}

PrayerTime::Calender PrayerTime::strToCalender(string calender)
{
	PrayerTime::Calender tmp;
	
	if( calender == "UmmAlQuraUniv")
		tmp=PrayerTime::UmmAlQuraUniv;
		
	else if( calender == "EgytionGeneralAuthorityofSurvey")
		tmp=PrayerTime::EgytionGeneralAuthorityofSurvey;
		
	else if( calender == "UnivOfIslamicScincesKarachi")
		tmp=PrayerTime::UnivOfIslamicScincesKarachi;
		
	else if( calender == "IslamicSocietyOfNorthAmerica")
		tmp=PrayerTime::IslamicSocietyOfNorthAmerica;
		
	else
		tmp=PrayerTime::MuslimWorldLeague;
		
	return tmp;
}

PrayerTime::Mazhab PrayerTime::strToMazhab(string mazhab)
{
	PrayerTime::Mazhab tmp;
	
	if( mazhab == "Default" )
		tmp=PrayerTime::Default;
	else
		tmp=PrayerTime::Hanafi;
		
	return tmp;
}

PrayerTime::Season PrayerTime::strToSeason(string season)
{
	PrayerTime::Season tmp;
	
	if( season == "Winter" )
		tmp=PrayerTime::Winter;
	else
		tmp=PrayerTime::Summer;

	return tmp;
}


string PrayerTime::calenderToStr(Calender calender)
{
	string tmp;
	
	if( calender == PrayerTime::UmmAlQuraUniv )
		tmp="UmmAlQuraUniv";
		
	else if( calender == PrayerTime::EgytionGeneralAuthorityofSurvey )
		tmp="EgytionGeneralAuthorityofSurvey";
		
	else if( calender == PrayerTime::UnivOfIslamicScincesKarachi )
		tmp="UnivOfIslamicScincesKarachi";
		
	else if( calender == PrayerTime::IslamicSocietyOfNorthAmerica )
		tmp="IslamicSocietyOfNorthAmerica";
		
	else
		tmp="MuslimWorldLeague";
		
	return tmp;
}

string PrayerTime::mazhabToStr(Mazhab mazhab)
{
	string tmp;
	
	if( mazhab == PrayerTime::Default )
		tmp="Default";
	else
		tmp="Hanafi";
		
	return tmp;
}

string PrayerTime::seasonToStr(Season season)
{
	string tmp;
	
	if( season == PrayerTime::Winter )
		tmp="Winter";
	else
		tmp="Summer";

	return tmp;
}

