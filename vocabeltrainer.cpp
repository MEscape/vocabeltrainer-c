#include "vocabeltrainer.h"
#include "ui_vocabeltrainer.h"
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

void googleVerification() {
	// Erstellen Sie einen neuen API-Client
		google::api_client::ApiClient client;
	
		// Legen Sie die client ID und das client secret fest
		client.set_client_id("CLIENT_ID");
		client.set_client_secret("CLIENT_SECRET");
	
		// Legen Sie den API-Schlüssel fest
		client.set_api_key("API_KEY");
	
		// Erstellen Sie eine neue DriveService-Instanz
		DriveService* service = new DriveService(&client);
	
		// Führen Sie eine Anforderung aus, um die Dateien im Google Drive-Konto des Benutzers abzurufen
		FilesResource::ListRequest request(service);
		request.set_q("mimeType='application/pdf'");
		request.Execute();
	
		// Ausgabe der Ergebnisse
		cout << "Dateien im Google Drive:" << endl;
		for (int i = 0; i < request.result().items().size(); i++) {
			cout << request.result().items()[i].title() << endl;
		}
}
