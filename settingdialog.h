#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include "ui_settingdialog.h"

class SettingDialog:public QDialog,private Ui::SettingDialog
{
	Q_OBJECT
	public:
		SettingDialog(const QStringList& list,QWidget* =0);
		int count()const;
		
		QString beforeState();
		QString afterState();
		QString soundName();
		
	public slots:
		void testClicked();
		
	private:
		int m_count;
		QStringList list;
		
		void init();
};

#endif
