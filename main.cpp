/**********************************************************
******************  AL-Moazen.   *********************
Aim to calculate prayers time and alarm you to the current prayer.
O.S: Cross Platform.
Project Developer: Ahmad Essam "SudaNix".
Project License: GPL v2.
feedback: suda.nix[at]hotmail.com
www.sudancs.com || www.qt-ar.org
changelog:
9/7/2008 project Start
25/3/2009 update project.
***********************************************************/

#include <QApplication>
#include <QtSql>

#include "almoazendialog.h"
#include "services.h"
#include "database.h"
#include <stdlib.h>


int main(int argc,char* argv[])
{
	Q_INIT_RESOURCE(gui);
	
	QApplication app(argc,argv);
	app.setQuitOnLastWindowClosed(false);
	app.setApplicationName("Al-Moazen");
	
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	
	Services services;
	Database db;

	// check if another instance of program run in memory.
	// TODO: insert process id in file and check it when another process try to execute the program.
	if( !services.isSignle() )
		return false;
	
	// load style sheet
	QFile file(":/qss/style.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	app.setStyleSheet(styleSheet);
	//services.loadStyleSheet("style");
	
		
	// check availability of Database file.
	if( !db.checkDatabase() )
		return false;

	// connect to SQLite database.
	if( !db.makeConnection() )
		return false;

	qDebug() << "Size of : " <<sizeof(AlMoazenDialog);
	AlMoazenDialog AlmoazenDlg;
	AlmoazenDlg.show();
	
	AlmoazenDlg.setFixedSize(QSize(329,366));
	
	return app.exec();
}


