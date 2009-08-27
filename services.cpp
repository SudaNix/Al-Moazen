#include <QtGui>
#include <QApplication>

#include "services.h"


Services::Services()
{	
	dir.setPath(QApplication::applicationDirPath());    // application folder
	
	// if the OS is Windows or Mac , we should change the current dir to parent one.
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

	//create lock file
	QDir currentDir = dir;
	currentDir.cd("configures");

	lockFile.setFileName(currentDir.filePath("run.dat"));

}

void Services::loadStyleSheet(const QString& sheetName)
{
	QFile file(":/qss/" + sheetName.toLower() + ".qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	
	qApp->setStyleSheet(styleSheet);

	file.close();
}


bool Services::isSignle()
{

	if( lockFile.exists() )
	{
		QMessageBox::critical(0,QObject::trUtf8("خطأ في التشغيل"),QObject::trUtf8("يوجد نسخة من البرنامج تعمل حاليا ، لا تستطيع تشغيل اكثر من نسخة واحدة"),QMessageBox::Close);
		return false;
	}

	
	else
	{
		lockFile.open(QFile::WriteOnly|QFile::Truncate);
		QTextStream out(&lockFile);
		out << "running" << endl;
		lockFile.close();
		return true;
	}
}

void Services::deleteLockFile()
{
	QDir currentDir = dir;
	currentDir.cd("configures");
	
	currentDir.remove("run.dat");
}

QStringList Services::soundFiles()
{
	QDir currentDir = dir;
	currentDir.cd("sounds");

	QStringList filter;
	filter << "*.mp3";
	
	currentDir.setNameFilters(filter);
	return currentDir.entryList(QDir::Files,QDir::Name);
}

