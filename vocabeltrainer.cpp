#include "vocabeltrainer.h"
#include "ui_vocabeltrainer.h"
#include "configmanager.h"
#include <QMessageBox>
#include "google/cloud/storage/client.h"

using namespace std;

vocabeltrainer::vocabeltrainer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::vocabeltrainer)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowType::FramelessWindowHint);
	
	ui->stackedWidget->insertWidget(1, &startWindow);
	
	//Wenn im startWindow der Beenden-Button gedrückt wurde, springt das stackedWidget vom vocabeltrainer auf Index 0
	connect(&startWindow, SIGNAL(endClicked()), this, SLOT(moveHome()));
	
	//Wenn Local-Button gedrückt wurde, springen alle stackedWidgets von startWindow auf Index 0
	connect(this, SIGNAL(startClicked()), &startWindow, SLOT(on_begin_clicked()));
	
	//Wenn Local-Button gedrückt wurde, wird im startWindow überprüft ob berreitsVokabeln hinzugefügt wurden
	connect(this, SIGNAL(startClicked()), &startWindow, SLOT(checkIfBegin()));
}

vocabeltrainer::~vocabeltrainer()
{
    delete ui;
}

void vocabeltrainer::mousePressEvent(QMouseEvent *event) {
    current_pos = event->globalPosition().toPoint();
}

void vocabeltrainer::mouseMoveEvent(QMouseEvent *event) {
    new_pos = QPoint(event->globalPosition().toPoint() - current_pos);
    move(x() + new_pos.x(), y() + new_pos.y());
    current_pos = event->globalPosition().toPoint();
}

void vocabeltrainer::on_SignIn_clicked()
{
    signIn.setModal(true);
    signIn.exec();
}


void vocabeltrainer::on_SignUp_clicked()
{
    signUp.setModal(true);
    signUp.exec();
}


void vocabeltrainer::on_local_clicked()
{
	emit startClicked();
	ui->stackedWidget->setCurrentIndex(1);
}


void vocabeltrainer::on_powerOff_clicked()
{
	this->close();
}

void vocabeltrainer::moveHome()
{
	ui->stackedWidget->setCurrentIndex(0);
}

void VocabelTrainer::googleVerification()
{
    ConfigManager& config = ConfigManager::getInstance();
    
    // Versuche die Konfiguration zu laden, falls noch nicht geschehen
    if (!config.isConfigLoaded()) {
        if (!config.loadConfig()) {
            qDebug() << "Fehler: Kann Konfigurationsdatei nicht laden!";
            QMessageBox::critical(this, "Konfigurationsfehler", 
                "Die Konfigurationsdatei konnte nicht geladen werden.\n"
                "Bitte stellen Sie sicher, dass eine 'config.json' Datei mit Ihren Google OAuth Credentials existiert.\n"
                "Verwenden Sie 'config_example.json' als Vorlage.");
            return;
        }
    }
    
    // Lade die Credentials aus der Konfiguration
    QString clientId = config.getGoogleClientId();
    QString clientSecret = config.getGoogleClientSecret();
    QString apiKey = config.getGoogleApiKey();
    
    // Prüfe ob alle erforderlichen Credentials vorhanden sind
    if (clientId.isEmpty() || clientSecret.isEmpty() || apiKey.isEmpty()) {
        qDebug() << "Fehler: Unvollständige Google OAuth Credentials in der Konfiguration!";
        QMessageBox::critical(this, "Konfigurationsfehler", 
            "Die Google OAuth Credentials sind unvollständig.\n"
            "Bitte überprüfen Sie Ihre config.json Datei.");
        return;
    }

    google::api_client::ApiClient client;
    client.set_client_id(clientId.toStdString());
    client.set_client_secret(clientSecret.toStdString());
    client.set_api_key(apiKey.toStdString());

    google::api_client::DriveService drive_service(&client);

    google::api_client::DriveService::ListRequest request;
    request.set_q("mimeType='application/pdf'");

    auto response = drive_service.List(request);
    if (response.ok()) {
        qDebug() << "Google Drive files listed successfully";
        // Process the response
    } else {
        qDebug() << "Error listing Google Drive files:" << response.status().message().c_str();
    }
}
