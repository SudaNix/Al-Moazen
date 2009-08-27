#ifndef CHANGETIMEDIALOG_H
#define CHANGETIMEDIALOG_H

#include <QDialog>
#include "ui_changetimedialog.h"

class ChangeTimeDialog:public QDialog,private Ui::ChangeTimeDialog
{
	Q_OBJECT
	public:
		ChangeTimeDialog(const QStringList&,QWidget* =0);
		
};

#endif // CHANGETIMEDIALOG_H
