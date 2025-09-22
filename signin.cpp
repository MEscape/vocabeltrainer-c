#include "signin.h"
#include "ui_signin.h"

using namespace std;

SignIn::SignIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignIn)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowType::FramelessWindowHint);
}

SignIn::~SignIn()
{
    delete ui;
}

void SignIn::mousePressEvent(QMouseEvent *event) {
    current_pos = event->globalPosition().toPoint();
}

void SignIn::mouseMoveEvent(QMouseEvent *event) {
    new_pos = QPoint(event->globalPosition().toPoint() - current_pos);
    move(x() + new_pos.x(), y() + new_pos.y());
    current_pos = event->globalPosition().toPoint();
}

void SignIn::on_close_clicked()
{
    this->close();
}

void dirve() {

}
