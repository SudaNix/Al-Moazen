#include "azkardialog.h" 

AzkarDialog::AzkarDialog(QWidget* parent):
QDialog(parent)
{
	setupUi(this);
}

void AzkarDialog::setAzkarEnable(QString value)
{
	value == "yes" ? azkarEnableCheckBox->setChecked(true):azkarEnableCheckBox->setChecked(false);
}

void AzkarDialog::setAutomaticClose(QString value)
{
	value == "yes" ? automaticCloseCheckBox->setChecked(true):automaticCloseCheckBox->setChecked(false);

}

void AzkarDialog::setPlayAzkarAfter(int value)
{
	minuteSpinBox->setValue(value);
}

void AzkarDialog::setCloseAzkarAfter(int value)
{
	secondSpinBox->setValue(value);
}

QString AzkarDialog::azkarEnabled()const
{
	return azkarEnableCheckBox->isChecked()?"yes":"no";
}

QString AzkarDialog::automaticClose()const
{
	return automaticCloseCheckBox->isChecked()?"yes":"no";
}

int AzkarDialog::playAzkarAfter()const
{
	return minuteSpinBox->value();
}

int AzkarDialog::closeAzkrAfter()const
{
	return secondSpinBox->value();
}

