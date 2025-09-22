#ifndef SIGNIN_H
#define SIGNIN_H

#include <QDialog>
#include <QMouseEvent>
#include <QPoint>

namespace Ui {
class SignIn;
}

class SignIn : public QDialog
{
    Q_OBJECT

public:
    explicit SignIn(QWidget *parent = nullptr);
    ~SignIn();

private slots:
    void on_close_clicked();

private:
    Ui::SignIn *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QPoint current_pos;
    QPoint new_pos;
};

#endif // SIGNIN_H
