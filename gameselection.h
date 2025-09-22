#ifndef GAMESELECTION_H
#define GAMESELECTION_H

#include <QDialog>
#include <QMouseEvent>
#include <QPoint>
#include <QTimer>
#include <QIntValidator>

namespace Ui {
	class gameSelection;
}

class gameSelection : public QDialog
{
		Q_OBJECT
		
	public:
		explicit gameSelection(QWidget *parent = nullptr);
		~gameSelection();
		bool boolValue();
		bool gamemode();
		int setTime();
		void resetAll();
		
	private slots:
		void on_buttonTraining_clicked();
		void on_time_stateChanged(int arg1);
		
		void on_close_clicked();
		
	private:
		Ui::gameSelection *ui;
		
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
	
		QPoint current_pos;
		QPoint new_pos;
};

#endif // GAMESELECTION_H
