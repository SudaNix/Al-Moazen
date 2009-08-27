/*
The Algorithms in this Code are taken from: http://qasweb.org , and implemented by me.

This file is part of Al-moathen software, and contain the API for calculate prayer times.
and it license under GNU/GPL v2.

Project start in : 9/7/2008
Update start in : 25/3/2009 and stop after one day.
restart in 19/4/2009
I hope this is the last time :).

*/

#ifndef PRAYERTIME_H
#define PRAYERTIME_H

#include <string>
using namespace std;

class PrayerTime
{
	public:
		enum Calender {UmmAlQuraUniv,EgytionGeneralAuthorityofSurvey,UnivOfIslamicScincesKarachi,IslamicSocietyOfNorthAmerica,MuslimWorldLeague};
		enum Mazhab {Default,Hanafi};
		enum Season {Winter,Summer};
		

		PrayerTime(double,double,int,int day,int month,int year,Calender=UmmAlQuraUniv,Mazhab =Default,Season=Winter);

		PrayerTime(double,double,int,int day,int month,int year,std::string calender,std::string mazhab,std::string season);

		PrayerTime();

		// Calculate Prayer times.
		void calculate();
		
		// Prayer Time
		double zuhrTime()const{return zuhr;}
		double asrTime()const{return asr;}
		double fajrTime()const{return fajr;}
		double ishaTime()const{return isha;}
		double maghribTime()const{return maghrib;}
		double shroukTime()const{return shrouk;}

		// getter and setter functions.
		Calender calender()const{return m_calender;}
		Mazhab mazhab()const{return m_mazhab;}
		Season season()const{return m_season;}
		
		double longitude()const{return m_longitude;}
		double latitude()const{return m_latitude;}
		double zone()const {return m_zone;}
		
		int day()const{return m_day;}
		int month()const{return m_month;}
		int year()const{return m_year;}
		
		void setDay(int day);
		void setMonth(int month);
		void setYear(int year);
		void setDate(int day,int month,int year);

		void setData(double,double,int,int day,int month,int year,Calender=UmmAlQuraUniv,Mazhab =Default,Season=Winter);
		
		void setData(double,double,int,int day,int month,int year,std::string calender,std::string mazhab,std::string season);
		
		// conversion functions
		Calender strToCalender(string calender);
		Mazhab strToMazhab(string mazhab);
		Season strToSeason(string season);
		
		string calenderToStr(Calender calender);
		string mazhabToStr(Mazhab mazhab);
		string seasonToStr(Season season);

	private:
		const double DegToRad; 
		const double RadToDeg;
		
		double m_longitude;
		double m_latitude;
		int m_zone;
		
		// shared variable used in calculation
		double dec;
		
		int m_day;
		int m_month;
		int m_year;

		double fajr;
		double shrouk;
		double zuhr;
		double asr;
		double maghrib;
		double isha;
		
		Calender m_calender;
		Mazhab m_mazhab;
		Season m_season;
		
		// just used in calculate() function.
		double equation(double alt);
		double removeDublication(double);
		double abs(double var);
		
};

#endif

