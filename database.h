#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QDir>

class Database
{
	public:
		Database();
		
		bool checkDatabase();
		bool makeConnection();

	private:
		QSqlDatabase db;
		QDir dir;		
};

#endif


