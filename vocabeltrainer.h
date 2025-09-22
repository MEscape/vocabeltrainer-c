#ifndef VOCABELTRAINER_H
#define VOCABELTRAINER_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPoint>
#include <startwindow.h>
#include <signin.h>
#include <signup.h>
#include <iostream>
#include <google/drive_api.h>

using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class vocabeltrainer; }
QT_END_NAMESPACE

class vocabeltrainer : public QMainWindow
{
    Q_OBJECT

public:
    vocabeltrainer(QWidget *parent = nullptr);
    ~vocabeltrainer();

private slots:
    void on_SignIn_clicked();

    void on_SignUp_clicked();

    void on_local_clicked();

    void on_powerOff_clicked();
	
	void moveHome();

private:
    Ui::vocabeltrainer *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QPoint current_pos;
    QPoint new_pos;
	
	startWindow startWindow;
	SignUp signUp;
	SignIn signIn;
	
signals:
	void startClicked();
};
#endif // VOCABELTRAINER_H
