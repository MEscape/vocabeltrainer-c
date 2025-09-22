#include "signup.h"
#include "ui_signup.h"

using namespace std;

SignUp::SignUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignUp)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowType::FramelessWindowHint);
}

SignUp::~SignUp()
{
    delete ui;
}

void SignUp::mousePressEvent(QMouseEvent *event) {
    current_pos = event->globalPosition().toPoint();
}

void SignUp::mouseMoveEvent(QMouseEvent *event) {
    new_pos = QPoint(event->globalPosition().toPoint() - current_pos);
    move(x() + new_pos.x(), y() + new_pos.y());
    current_pos = event->globalPosition().toPoint();
}

void SignUp::on_close_clicked()
{
    this->close();
}

