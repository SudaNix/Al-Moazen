#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget* parent):
QDialog(parent)
{
	setupUi(this);
	
	QPixmap map(":/images/home0.png");
	label->setPixmap(map);
	
	okButton->setIcon(QIcon(":/images/ok.png"));
}

