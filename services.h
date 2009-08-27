#ifndef SERVICES_H
#define SERVICES_H


#include <QDir>
#include <QStringList>
#include <QFile>

class Services
{
	public:
		Services();
		
		void loadStyleSheet(const QString &);
	
		QStringList soundFiles();

		bool isSignle(); // to be sure there is one instance of program run.
		void deleteLockFile();

	private:
		QDir dir;
		QFile lockFile;

};


#endif
