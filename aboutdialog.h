#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include "ui_aboutdialog.h"

class AboutDialog:public QDialog,private Ui::AboutDialog
{
	public:
		AboutDialog(QWidget* =0);	
};

#endif

