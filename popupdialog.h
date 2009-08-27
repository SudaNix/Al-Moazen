#ifndef POPUPDIALOG_H
#define POPUPDIALOG_H

#include <QDialog>

#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>

//class QSound;
class QCloseEvent;
class QTextEdit;

class PopupDialog:public QDialog
{
	//Q_OBJECT
	public:
		PopupDialog(QWidget* =0);
		~PopupDialog();

		void playSound(const QString&);

		void display(const QString&,const QString&);
		void display(const QString&,bool);
		
	//protected:
	//	void closeEvent(QCloseEvent* event);
	private:
		QTextEdit* textEdit;
		//QSound* sound;
		
		Phonon::MediaObject* player;
		Phonon::AudioOutput* audioOutput;
};

#endif   // POPUPDIALOG_H
