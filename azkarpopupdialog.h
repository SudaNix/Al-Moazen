#ifndef AZKARPOPUPDIALOG_H
#define AZKARPOPUPDIALOG_H

#include <QDialog>
#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>

class QTextEdit;

class AzkarPopupDialog:public QDialog
{
	Q_OBJECT
	public:
		AzkarPopupDialog(QWidget* =0);
		~AzkarPopupDialog();

		void display();
		void displayAzkarAlsabah();
		void displayAzkarAlmasa();
		
		void displayAzkarAlmasaSound();
		void displayAzkarAlsabahSound();

		void setCloseAfter(int value);
		
	protected:
		void timerEvent(QTimerEvent*);
		
	private:
		QTextEdit* textEdit;
		int timer;
		int closeAfter;
		
		Phonon::MediaObject* player;
		Phonon::AudioOutput* audioOutput;
		
};
#endif  // AZKARPOPUPDIALOG_H