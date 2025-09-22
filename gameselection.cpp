#include "gameselection.h"
#include "ui_gameselection.h"

using namespace std;

gameSelection::gameSelection(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::gameSelection)
{
	ui->setupUi(this);
	
	//Frameless Window
    this->setWindowFlags(Qt::WindowType::FramelessWindowHint);
	//Entfernt Tab von folgenden Elementen
	ui->ger->setFocusPolicy(Qt::NoFocus);
	ui->eng->setFocusPolicy(Qt::NoFocus);
	ui->time->setFocusPolicy(Qt::NoFocus);
	
	ui->setTime->setEnabled(false);
}

gameSelection::~gameSelection()
{
	delete ui;
}

//Fenster bewegen
void gameSelection::mousePressEvent(QMouseEvent *event) {
    current_pos = event->globalPosition().toPoint();
}

void gameSelection::mouseMoveEvent(QMouseEvent *event) {
    new_pos = QPoint(event->globalPosition().toPoint() - current_pos);
    move(x() + new_pos.x(), y() + new_pos.y());
    current_pos = event->globalPosition().toPoint();
}

bool gameSelection::boolValue() {
	return ui->ger->isChecked();
}

bool gameSelection::gamemode() {
	return ui->time->isChecked();
}

int gameSelection::setTime() {
	QString time = ui->setTime->text();
	int timeAvailable = time.toInt()*1000;
	
	return timeAvailable;
}

void gameSelection::on_buttonTraining_clicked()
{
	if((ui->eng->isChecked() || ui->ger->isChecked())) {
		if(ui->time->isChecked()) {
			if(ui->setTime->text().isEmpty()) {
				ui->errorMsg_2->setText("Lücke ausfüllen.");
				QTimer::singleShot(3000, this, [this] () { ui->errorMsg_2->setText(""); });
			} else {
				if(ui->setTime->text() == "0") {
					ui->errorMsg_2->setText("0s ist nicht möglich.");
					QTimer::singleShot(3000, this, [this] () { ui->errorMsg_2->setText(""); });
				} else {
					accept();
				}
			}
		} else {
			accept();
		}
	} else {
		if(!(ui->eng->isChecked() || ui->ger->isChecked())) {
			ui->errorMsg->setText("Wähle zuerst die Sprache in die du Übersetzen willst.");
			QTimer::singleShot(3000, this, [this] () { ui->errorMsg->setText(""); });
		}
	}
}

void gameSelection::resetAll() {
	//Uncheck Radiobuttons
	ui->eng->setAutoExclusive(false);
	ui->eng->setChecked(false);
	ui->eng->setAutoExclusive(true);
	ui->ger->setAutoExclusive(false);
	ui->ger->setChecked(false);
	ui->ger->setAutoExclusive(true);
	
	ui->time->setChecked(false);
	ui->setTime->clear();
}

void gameSelection::on_time_stateChanged(int arg1)
{
	if(arg1 != 0) {
		ui->setTime->setValidator(new QIntValidator(0, 100, this));
		ui->setTime->setStyleSheet("background-color: rgba(0, 0, 0, 0); border: 1px solid rgba(0, 0, 0, 0); border-bottom-color: white; color: rgb(255, 255, 255); padding-bottom: 7px; max-width: 110px; margin-right: 20px;");
		ui->setTime->setEnabled(true);
	} else {
		ui->setTime->setStyleSheet("color: transparent; border-bottom-color: transparent; border: 0;");
		ui->setTime->setEnabled(false);
	}
}


void gameSelection::on_close_clicked()
{
    reject();
}

