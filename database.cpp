#include <QFile>
#include <QMessageBox>
#include <QApplication>

#include "database.h" 

Database::Database()
{
	dir.setPath(QApplication::applicationDirPath());    // application folder
	
	// if the OS is Windows or Mac make , we should change the current dir to parent one.
	#if defined(Q_OS_WIN32)
		if (dir.dirName().toLower() == "debug" || dir.dirName().toLower() == "release")
		{
			dir.cdUp();
		}
			
	#elif defined(Q_OS_MAC)
		if (dir.dirName() == "MacOS")
		{
			dir.cdUp();
			dir.cdUp();
			dir.cdUp();
		}

	#endif

	QDir::setCurrent(dir.absolutePath());
	
	dir.cd("database");
}


bool Database::checkDatabase()
{
	
	if( QFile::exists(dir.filePath("CountriesDB")) )
		return true;
	
	QMessageBox::critical(0, "Cannot Read Database","Unable to Read Database File.\n""Click Cancel to exit.", QMessageBox::Cancel);
	
	
	return false;
}


bool Database::makeConnection()
{
	db=QSqlDatabase::addDatabase("QSQLITE");
	db.setHostName("localhost");
	db.setDatabaseName(dir.filePath("CountriesDB"));
	db.setUserName("");
	db.setPassword("");
	

	if (!db.open()) {
		QMessageBox::critical(0, "Cannot open database", "Unable to establish a database connection.\n""This Application needs SQLITE support.\n\n""Click Cancel to exit.", QMessageBox::Cancel);
		return false;
	}
	
	return true;
}

