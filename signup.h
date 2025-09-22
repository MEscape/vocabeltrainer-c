#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include <QMouseEvent>
#include <QPoint>

namespace Ui {
class SignUp;
}

class SignUp : public QDialog
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);
    ~SignUp();

private slots:
    void on_close_clicked();

private:
    Ui::SignUp *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QPoint current_pos;
    QPoint new_pos;
};

#endif // SIGNUP_H
