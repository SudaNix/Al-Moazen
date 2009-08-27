#ifndef AZKARDIALOG_H
#define AZKARDIALOG_H

#include <QDialog>
#include "ui_azkardialog.h"

class AzkarDialog:public QDialog,private Ui::AzkarDialog
{
	//Q_OBJECT
	public:
		AzkarDialog(QWidget* =0);
		
		void setAzkarEnable(QString);
		void setAutomaticClose(QString);
		void setPlayAzkarAfter(int);
		void setCloseAzkarAfter(int);
		
		QString azkarEnabled()const;
		QString automaticClose()const;
		int playAzkarAfter()const;
		int closeAzkrAfter()const;
		
		
	//public slots:
		
		
};

#endif  // AZKARDIALOG_H
