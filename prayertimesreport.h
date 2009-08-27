#ifndef PRAYERTIMESREPORT_H
#define PRAYERTIMESREPORT_H

#include <QMainWindow>
#include <QList>
#include <QTextTableFormat>
#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QTextFrameFormat>
#include <QTextCursor>
#include <QVector>
#include <QTextLength>
#include <QDate>

class QTextEdit;
class QMenu;
class QAction;
class QTextFrame;
class QTextTable;

class PrayerTimesReport:public QMainWindow
{
	Q_OBJECT
	public:
		PrayerTimesReport(const QStringList&,QWidget* parent=0);
		
	
	public slots:
		void printClicked();

	
	private:
		QString convertIntoTime(double var);
		
		void createMenus();
		void createActions();
		void createList();

		void displayHeader();

		void init();
		void displayReport();

		QTextEdit* textEdit;
		
		QMenu* fileMenu;
		
		QAction* printAct;
		QAction* closeAct;


		QTextCursor cursor;
		
		QTextFrame* rootFrame;
		QTextFrameFormat rootFrameFormat;
		
		QTextCharFormat boldCharFormat;
		QTextCharFormat titleCharFormat;
		QTextCharFormat defaultCharFormat;
		QTextCharFormat cellBackgroundFormat;
		QTextCharFormat redBoldCharFormat;
		QTextCharFormat greenTitleCharFormat;
		
		QTextBlockFormat blockFormat;
		QTextBlockFormat centerBlockFormat;
		
		QTextTable* table;
		QTextTableFormat tableFormat;
		
		QStringList headerList;
		
		QVector<QTextLength> constraints;

		QList<QStringList> list;
		
		QStringList passedList;
		
};

#endif
