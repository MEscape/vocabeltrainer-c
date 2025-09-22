#include "startwindow.h"
#include "ui_startwindow.h"

using namespace std;

//Golabale Variable die auf AppData verlinkt
QString OutputFolder(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/.vocabeltrainer");

//Main Funktion
startWindow::startWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::startWindow)
{
    ui->setupUi(this);
	
	//Frameless Window
    this->setWindowFlags(Qt::WindowType::FramelessWindowHint);
	//Icons der Sidebar
	icon();
	//Entfernt Rechte des Nutzers QTextEdit zuverändern
	ui->engText->setReadOnly(true);
	ui->gerText->setReadOnly(true);
	//Entfernt Tab von folgenden Elementen
	ui->engText->setFocusPolicy(Qt::NoFocus);
	ui->gerText->setFocusPolicy(Qt::NoFocus);
	ui->go->setFocusPolicy(Qt::NoFocus);
	ui->notEmpty->setFocusPolicy(Qt::NoFocus);
	ui->reset->setFocusPolicy(Qt::NoFocus);
	
	//Boxshadow für QTextEdit
	boxShadow();
	
	//Startet Trainer
	connect(ui->go, SIGNAL(clicked()), this, SLOT(on_answerButton_clicked()));
	//Neue Vokabel wenn Progressbar 100% hat
	connect(ui->reset, SIGNAL(clicked()), this, SLOT(on_answerButton_clicked()));
	
	//Sortiert Werte wenn die Verbindung existiert
	connect(ui->powerGame, SIGNAL(clicked()), this, SLOT(setBack()));
	
	//Standardmäßige einstellung
	ui->stackedWidget_5->setCurrentIndex(0);
}

startWindow::~startWindow() {
    delete ui;
}

//Icons der Sidebar
void startWindow::icon() {
    ui->begin->setIcon(QIcon(":/imagesList/images/home.png"));
    ui->addition->setIcon(QIcon(":/imagesList/images/add.png"));
    ui->visit->setIcon(QIcon(":/imagesList/images/eye.png"));
    ui->power->setIcon(QIcon(":/imagesList/images/power.png"));
}

//Loading Animation
void startWindow::loadingAnimation() {
	ui->stackedWidget->setCurrentIndex(4);
	movie = new QMovie(":/loadingAnimation/gif/Loading_icon.gif");
	ui->vid->setMovie(movie);
	movie->start();
}

//BoxShadow für QTextEdit
void startWindow::boxShadow() {
	QGraphicsDropShadowEffect* effectGer = new QGraphicsDropShadowEffect();
	effectGer->setColor(QColorConstants::Black);
	effectGer->setOffset(5, 5);
	effectGer->setBlurRadius(5);
	ui->gerText->setGraphicsEffect(effectGer);
	
	QGraphicsDropShadowEffect* effectEng = new QGraphicsDropShadowEffect();
	effectEng->setColor(QColorConstants::Black);
	effectEng->setOffset(5, 5);
	effectEng->setBlurRadius(5);
	ui->engText->setGraphicsEffect(effectEng);
}

//Checkt ob berreits Vokabeln hinzugefügt wurden
void startWindow::checkIfBegin() {
	bool exist = fileExists(OutputFolder +"/VocGroup/english-DE.txt");
	
	if(exist) {
		QVector<QString> english = loadFrom(OutputFolder +"/VocGroup/english-DE.txt");
		
		if(english.isEmpty()) {
			ui->stackedWidget_2->setCurrentIndex(0);
			ui->stackedWidget_3->setCurrentIndex(2);
			ui->stackedWidget_4->setCurrentIndex(1);
		} else {
			ui->stackedWidget_2->setCurrentIndex(0);
			ui->stackedWidget_3->setCurrentIndex(0);
			ui->stackedWidget_4->setCurrentIndex(0);
		}
	} else {
		ui->stackedWidget_2->setCurrentIndex(0);
		ui->stackedWidget_3->setCurrentIndex(2);
		ui->stackedWidget_4->setCurrentIndex(1);
	}
}

//Entfernt ENTER Funktion
void startWindow::keyPressEvent(QKeyEvent *evt) {
	if(evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return)
        return;
	QDialog::keyPressEvent(evt);
}

//Checkt ob Datei existiert
bool startWindow::fileExists(QString txtFile) {
    QFileInfo check_file(txtFile);
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

//Ließt Vokabeln von String in Dokument ein
void startWindow::safeIn(QString safe, QString txtFile) {
	QFile file(txtFile);
	
	if(!file.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)) {
	}
	
	QTextStream input(&file);
	input << safe + '\n';
	
	file.close();
}

//Entfernt Datein
void startWindow::removeFile(QString txtFile) {
	QFile file(txtFile);
	
	file.remove();
}

//Ließt Werte von Datei in Vektor ein
QVector<QString> startWindow::loadFrom(QString txtFile) {
	QVector<QString> load;
	QFile file(txtFile);
	
	if(!file.open(QIODevice::ReadOnly)) {
	}
	QTextStream input(&file);
	
	while(!input.atEnd()) {
		QString data = input.readLine();
		load.push_back(data);
	}
			
	file.close();
	
	return load;
}

//Ließt Werte von Datei in Vektor<int> ein
QVector<int> startWindow::loadFromInt(QString txtFile) {
	QVector<int> load;
	QFile file(txtFile);
	
	if(!file.open(QIODevice::ReadOnly)) {
	}
	QTextStream input(&file);
	
	while(!input.atEnd()) {
		QString data = input.readLine();
		load.push_back(data.toInt());
	}
			
	file.close();
	
	return load;
}

//Konvertiert Vektor in Set
QSet<QString> startWindow::convertToSet(QVector<QString> input) {
    QSet<QString> search;
    for (QString element : input) {
        search.insert(element);
    }

    return search;
}

//Konvertiert Vektor in Queue
QQueue<QString> startWindow::convertToQueue(QVector<QString> input) {
    QQueue<QString> duration;
    for (QString element : input) {
        duration.push_back(element);
    }

    return duration;
}

//Liniennummer der eingegebenen Vokabel
int startWindow::line_of_string(QString txtFile, QString searchString) {
	QFile file(txtFile);
			
	if(!file.open(QIODevice::ReadOnly)) {
	}
	int lineCount = 0;

	QTextStream input(&file);

	while(!input.atEnd()) {
		++lineCount;
		QString active_line = input.readLine();
		
		if(active_line == searchString) {
            return lineCount;   
        }
	}
	file.close();
}

//Löschen oder Ersetzen in Dokument
void startWindow::del_rep_line(QString txtFile, QString newValue, int lineCount, QString selection) {
	QVector<QString> lines;	
	QFile file(txtFile);
	
	if(!file.open(QIODevice::ReadOnly)) {
	}
	QTextStream input(&file);

    while(!input.atEnd()) {
		QString line = input.readLine();
        lines.push_back(line);
    }
  
    file.remove();
	QTextStream output(&file);

	if(!file.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)) {
	}

    lineCount--;

    if(selection == "del") {
        for (int i = 0; i < lines.size(); i++) {
            if (i != lineCount) {
            output << lines[i] + '\n';
            }
        }
    } if(selection == "rep") {
        for (int i = 0; i < lines.size(); i++) {
            if (i != lineCount) {
				output << lines[i] + '\n'; 
            } else {
				output << newValue + '\n'; 
			}
        }
    }
    file.close();
}

//Checkt ob das Wort was entfernt werden soll bei ALL, noch in anderen vocGroups existiert
bool startWindow::checkOthers() {
	QVector<QString> summary;
	
	for(int i = 0; i < vocgroup.size(); i++) {
		QVector<QString> temp = loadFrom(OutputFolder +"/VocGroup/"+ vocgroup[i] +"/german-DE.txt");
		for(int t = 0; t < temp.size(); t++) {
			summary.push_back(temp[t]);
		}
	}
	
	sort(summary.begin(), summary.end());
	auto it = unique(summary.begin(), summary.end());
	bool wasUnique = (it == summary.end());
	
	return wasUnique;
}

//Löscht Daten aus den Dokumenten
void startWindow::delete_list(QString language) {
	QString subString = language.right(6);
	
	int lineCount = line_of_string(OutputFolder +"/VocGroup/"+ groupVoc + language, del);
	
	del_rep_line(OutputFolder +"/VocGroup/"+ groupVoc +"/english-"+ subString, "0", lineCount, "del");
    del_rep_line(OutputFolder +"/VocGroup/"+ groupVoc +"/german-"+ subString, "0", lineCount, "del");
	del_rep_line(OutputFolder +"/logs/algorithm/"+ groupVoc +"-"+ subString, "0", lineCount, "del");
	
	bool wasUnique = checkOthers();
	
	if(wasUnique) {
		del_rep_line(OutputFolder +"/VocGroup/german-"+ subString, "0", lineCountAll, "del");
		del_rep_line(OutputFolder +"/VocGroup/english-"+ subString, "0", lineCountAll, "del");
		del_rep_line(OutputFolder +"/logs/algorithm/all-"+ subString, "0", lineCountAll, "del");
	}
}

//Entfernt Inhalt
void startWindow::clearFields() {
	ui->engText->setText("");
	ui->gerText->setText("");
	ui->select_Del->clear();
	ui->select_Group->clear();
	ui->groupGame->clear();
}

//Startbereich
void startWindow::on_begin_clicked() {
	checkIfBegin();
	clearFields();
	
	ui->begin->setFocus();
    ui->stackedWidget->setCurrentIndex(0);
	
	QString vocGroup = ui->vocGroup->text();
	
	QDir dir;
	
	//Checkt ob Ordner existiert
	if (dir.exists(OutputFolder)) {
		QVector<QString> german = loadFrom(OutputFolder +"/VocGroup/"+ vocGroup +"/german-DE.txt");
		int size = german.size();
		QString sum;
		
		//Checkt ob Feld leer ist
		if(vocGroup.isEmpty()){
			ui->sum->setText("");
		} else {
			sum = QString::number(size);
			ui->sum->setText(sum);
			ui->groupforsum->setText(vocGroup);
		}
	}
}

//Bereich für Vokabeln hinzufügen
void startWindow::on_addition_clicked() {
	checkIfBegin();
	clearFields();
	
    ui->stackedWidget->setCurrentIndex(1);
	
	vocgroup = loadFrom(OutputFolder +"/logs/vocGroups.txt");
	englishAll = loadFrom(OutputFolder +"/VocGroup/english-DE.txt");
	
	ui->vocGroup->clear();
	ui->groupforsum->setText("");
}

//Bereich für Vokabeln ansehen
void startWindow::on_visit_clicked() {
	checkIfBegin();
	clearFields();
	
    ui->stackedWidget->setCurrentIndex(2);
	
	vocgroup = loadFrom(OutputFolder +"/logs/vocGroups.txt");
}

//Bereich für Vokabeltrainer
void startWindow::on_power_clicked() {
	checkIfBegin();
	clearFields();
	
    ui->stackedWidget->setCurrentIndex(3);
	
	dataEng = loadFrom(OutputFolder +"/VocGroup/english-DE.txt");
	vocgroup = loadFrom(OutputFolder +"/logs/vocGroups.txt");
}

//Lokalen Trainer beenden
void startWindow::on_end_clicked() {
    emit endClicked();
}

//Error Nachricht -> Nicht alle Felder sind ausgefüllt
void startWindow::errorTextAdditon() {
	ui->textGroup->setText("Lücken ausfüllen.");
	ui->textGroup->setStyleSheet("color: red");
	QTimer::singleShot(3000, this, [this] () { ui->textGroup->setText("Geben sie den Namen ihrer Vokabelgruppe ein und betätigen sie mit OK."); ui->textGroup->setStyleSheet("color: white"); });
}

//Checkt ob VocGroup schon existiert
void startWindow::checkExistVocGroup() {
	QSet<QString> search = convertToSet(vocgroup);
	
	if(!(search.find(lowVocGroup) != search.end())) {
		safeIn(lowVocGroup, OutputFolder +"/logs/vocGroups.txt");
	}
}

//Erstellt VocGroups
void startWindow::createGroup() {
	//Alle VocGroups kleingeschrieben
	lowVocGroup = groupVoc.toLower().trimmed();

	//Erstellt neue Pfade
	QString path1(OutputFolder +"/VocGroup/"+ lowVocGroup);
	QString path2(OutputFolder +"/logs");
	QString path3(OutputFolder +"/logs/algorithm");
	LPCWSTR path2forwin = (const wchar_t*) path2.utf16();
	
	QDir dir;
	
	if (!dir.exists(OutputFolder)) {
		dir.mkpath(OutputFolder);
	}
	if (!dir.exists(path1)) {
		dir.mkpath(path1);
	}
	if (!dir.exists(path2)) {
		dir.mkpath(path2);
		SetFileAttributes(path2forwin, FILE_ATTRIBUTE_HIDDEN);
	}
	if (!dir.exists(path3)) {
		dir.mkpath(path3);
	}
	
	checkExistVocGroup();

	ui->infoGroup->setText(groupVoc);
}

void startWindow::bufferAdd() {
	loadingAnimation();
	
	createGroup();
	
	movie->stop();
	ui->stackedWidget->setCurrentIndex(1);
	ui->stackedWidget_2->setCurrentIndex(1);
}

//Gesammtfunktion um VocGroups hinzufügen zu können (Button)
void startWindow::on_select1_clicked() {
	groupVoc = ui->vocGroup->text().trimmed();
	
	//Checkt ob Lücke ausgefüllt ist
	if(groupVoc.isEmpty()){
		errorTextAdditon();
	} else {
		bufferAdd();
	}
}

//Gesammtfunktion um VocGroups hinzufügen zu können (Eingabefeld)
void startWindow::on_vocGroup_returnPressed() {
	groupVoc = ui->vocGroup->text().trimmed();
	
	//Checkt ob Lücke ausgefüllt ist
	if(groupVoc.isEmpty()){
		errorTextAdditon();
	} else {
		bufferAdd();
	}
}

//Error Nachricht -> Nicht alle Felder sind ausgefüllt
void startWindow::errorTextAdditon2() {
	ui->textAdd->setStyleSheet("color: red");
	ui->textAdd->setText("Alle Lücken ausfüllen.");
	QTimer::singleShot(3000, this, [this] () { ui->textAdd->setText("Geben sie ihre Vokabel ein."); ui->textAdd->setStyleSheet("color: white");});
}

//Error Nachricht -> Vokabel wurde schon hinzugefügt (Checkt Englischen speicher)
void startWindow::errorExistEng() {
	ui->textAdd->setStyleSheet("color: red");
	ui->textAdd->setText("Vokabel wurde berreits hinzugefügt.");
	QTimer::singleShot(3000, this, [this] () { ui->textAdd->setText("Geben sie ihre Vokabel ein."); ui->textAdd->setStyleSheet("color: white"); });
			
	ui->vocGer->clear();
	ui->vocEng->clear();
}

//Läd jeweilige Vokabel ins Dokument
void startWindow::addVoc() {
	search = convertToSet(englishAll);
	
	safeIn(voc_german, OutputFolder +"/VocGroup/"+ groupVoc +"/german-DE.txt");
	safeIn(voc_english, OutputFolder +"/VocGroup/"+ groupVoc +"/english-DE.txt");
	safeIn(voc_german, OutputFolder +"/VocGroup/"+ groupVoc +"/german-EN.txt");
	safeIn(voc_english, OutputFolder +"/VocGroup/"+ groupVoc +"/english-EN.txt");
	safeIn("0", OutputFolder +"/logs/algorithm/"+ groupVoc +"-DE.txt");
	safeIn("0", OutputFolder +"/logs/algorithm/"+ groupVoc +"-EN.txt");
	
	//Überprüft ob Vokabel schon hinzugefügt wurde
	if(!(search.find(voc_english) != search.end())) {
		safeIn(voc_german, OutputFolder +"/VocGroup/german-DE.txt");
		safeIn(voc_english, OutputFolder +"/VocGroup/english-DE.txt");
		safeIn(voc_german, OutputFolder +"/VocGroup/german-EN.txt");
		safeIn(voc_english, OutputFolder +"/VocGroup/english-EN.txt");
		safeIn("0", OutputFolder +"/logs/algorithm/all-DE.txt");
		safeIn("0", OutputFolder +"/logs/algorithm/all-EN.txt");
	}
}

//Checkt ob Vokabel ins Dokument geladen werden kann
void startWindow::vocAddition() {
	QVector<QString> english = loadFrom(OutputFolder +"/VocGroup/"+ groupVoc +"/english-DE.txt");
	QSet<QString> search = convertToSet(english);

	//Überprüft ob englische Vokabel schon hinzugefügt wurde
	if(search.find(voc_english) != search.end()) {
		errorExistEng();
	} else {
		addVoc();
		
		ui->textAdd->setStyleSheet("color: green");
		ui->textAdd->setText("Vokabel wurde erfolgreich hinzugefügt.");
		QTimer::singleShot(3000, this, [this] () { ui->textAdd->setText("Geben sie ihre Vokabel ein."); ui->textAdd->setStyleSheet("color: white"); });

		ui->vocGer->clear();
		ui->vocEng->clear();
	}
}

//Gesammtfunktion um Vokabeln hinzufügen zu können (Button)
void startWindow::on_select2_clicked() {	
	voc_english = ui->vocEng->text().trimmed();
	voc_german = ui->vocGer->text().trimmed();
	
	//Checkt ob das Feld leer ist
	if(voc_english.isEmpty() || voc_german.isEmpty()){
		errorTextAdditon2();
	} else {
		vocAddition();
	}
}

//Gesammtfunktion um Vokabeln hinzufügen zu können (Eingabefeld)
void startWindow::on_vocGer_returnPressed() {
	voc_english = ui->vocEng->text().trimmed();
	voc_german = ui->vocGer->text().trimmed();
	
	//Checkt ob das Feld leer ist
	if(voc_english.isEmpty() || voc_german.isEmpty()){
		errorTextAdditon2();
	} else {
		vocAddition();
	}
}

//Gesammtfunktion um Vokabeln hinzufügen zu können (Eingabefeld)
void startWindow::on_vocEng_returnPressed() {
	voc_english = ui->vocEng->text().trimmed();
	voc_german = ui->vocGer->text().trimmed();
	
	//Checkt ob das Feld leer ist
	if(voc_english.isEmpty() || voc_german.isEmpty()){
		errorTextAdditon2();
	} else {
		vocAddition();
	}
}

//Ändern der VocGroup
void startWindow::on_back_clicked() {
	ui->vocGroup->clear();
	ui->vocGer->clear();
	ui->vocEng->clear();
    ui->stackedWidget_2->setCurrentIndex(0);
}

//Entfernt Inhalt von QTextEdit
void startWindow::clearText() {
	ui->engText->clear();
	ui->gerText->clear();
}

//Error Nachricht -> Feld für VocGroup ist nicht ausgefüllt
void startWindow::errorVisitGroup() {
	ui->errorMsg->setText("Lücke ausfüllen.");
	QTimer::singleShot(3000, this, [this] () { ui->errorMsg->setText(""); });
	
	clearText();
}

//Checkt ob VocGroup dargestellt werden kann
void startWindow::visitEmpty() {
	QVector<QString> english = loadFrom(OutputFolder +"/VocGroup/"+ groupVoc +"/english-DE.txt");
	QVector<QString> german = loadFrom(OutputFolder +"/VocGroup/"+ groupVoc +"/german-DE.txt");
	
	//Checkt ob Datei leer ist
	if(english.isEmpty() || german.isEmpty()) {
		ui->errorMsg->setText("Vokabelgruppe ist leer.");
		QTimer::singleShot(3000, this, [this] () { ui->errorMsg->setText(""); });
		
		clearText();
		ui->select_Group->clear();
	} else {
		clearText();
		ui->engText->append(english.join("\n"));
		ui->gerText->append(german.join("\n"));
	}
}

//Checkt ob Ordner leer ist
void startWindow::folderCheckVisit() {
	if(QDir(OutputFolder +"/VocGroup/"+ groupVoc).entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() == 0) {
		ui->errorMsg->setText("Vokabelgruppe ist leer.");
		QTimer::singleShot(3000, this, [this] () { ui->errorMsg->setText(""); });
		
		clearText();
		ui->select_Group->clear();
	} else {
		visitEmpty();
	}
}

//Checkt ob eingegebene Vokabelgruppe existiert
void startWindow::checkExistVisit() {
	if(search.find(groupVoc) != search.end()) {
		folderCheckVisit();
	} else {
		ui->errorMsg->setText("Vokabelgruppe existiert nicht.");
		QTimer::singleShot(3000, this, [this] () { ui->errorMsg->setText(""); });
		
		clearText();
		ui->select_Group->clear();
	}
}

void startWindow::buffer() {
	loadingAnimation();
	
	checkExistVisit();
	
	movie->stop();
	ui->stackedWidget->setCurrentIndex(2);
	ui->stackedWidget_4->setCurrentIndex(0);
}

//Gesammtfunktion um Vokabeln darstellen zu können (Button)
void startWindow::on_selectGroup_clicked() {
	groupVoc = ui->select_Group->text().trimmed().toLower();
	
	//Checkt ob das Feld leer ist
	if(groupVoc.isEmpty()){
		errorVisitGroup();
	} else {
		search = convertToSet(vocgroup);
		buffer();
	}
}

//Gesammtfunktion um Vokabeln darstellen zu können (Eingabefeld)
void startWindow::on_select_Group_returnPressed() {
	groupVoc = ui->select_Group->text().trimmed().toLower();
	
	//Checkt ob das Feld leer ist
	if(groupVoc.isEmpty()){
		errorVisitGroup();
	} else {
		search = convertToSet(vocgroup);
		buffer();
	}
}

//Nachricht -> Vokabel gelöscht
void startWindow::success() {
	ui->errorMsg2->setStyleSheet("color: green");
	ui->errorMsg2->setText("Vokabel erfolgreich entfernt.");
	QTimer::singleShot(3000, this, [this] () { ui->errorMsg2->setText(""); });
}

//Errornachrichten beim Versuch Vokabeln zu löschen
void startWindow::errorDel() {
	search = convertToSet(vocgroup);
	
	if(search.find(vocGroupFilled) != search.end()) {
		ui->errorMsg2->setStyleSheet("color: red");
		ui->errorMsg2->setText("Vokabel falsch geschrieben");
		QTimer::singleShot(3000, this, [this] () { ui->errorMsg2->setText(""); });
	} else {
		ui->errorMsg2->setStyleSheet("color: red");
		ui->errorMsg2->setText("Vokabelgruppe existiert nicht.");
		QTimer::singleShot(3000, this, [this] () { ui->errorMsg2->setText(""); });
	}
}

//Überprüft ob Vokabel aus dem Dokument gelöscht werden kann
void startWindow::vocDelete() {
	int sibling = 0;
	QVector<QString> german = loadFrom(OutputFolder +"/VocGroup/"+ groupVoc +"/german-DE.txt");
	QVector<QString> english = loadFrom(OutputFolder +"/VocGroup/"+ groupVoc +"/english-DE.txt");
	QSet<QString> searchGer = convertToSet(german);
	QSet<QString> searchEng = convertToSet(english);

	//Checkt ob Vokabel in der jeweiligen Gruppe existiert
		if(searchGer.find(del) != searchGer.end()) {
			sibling = 1;
			lineCountAll = line_of_string(OutputFolder +"/VocGroup/german-DE.txt", del);
			delete_list("/german-DE.txt");
			lineCountAll = line_of_string(OutputFolder +"/VocGroup/german-EN.txt", del);
			delete_list("/german-EN.txt");
			success();
		} if(searchEng.find(del) != searchEng.end()) {
			//Checkt ob Deutsch = Englisch
			if(sibling != 1) {
				lineCountAll = line_of_string(OutputFolder +"/VocGroup/english-DE.txt", del);
				delete_list("/english-DE.txt");
				lineCountAll = line_of_string(OutputFolder +"/VocGroup/english-EN.txt", del);
				delete_list("/english-EN.txt");
				success();
			}
		} if(!(searchGer.find(del) != searchGer.end() || searchEng.find(del) != searchEng.end())) {
			errorDel();
		}
}

//Error Nachricht -> Feld ist nicht ausgefüllt
void startWindow::errorDelete() {
	ui->errorMsg2->setStyleSheet("color: red");
	ui->errorMsg2->setText("Lücke ausfüllen.");
	QTimer::singleShot(3000, this, [this] () { ui->errorMsg2->setText(""); });
}

void startWindow::errorDelete2() {
	ui->errorMsg2->setStyleSheet("color: red");
	ui->errorMsg2->setText("Vokabelgruppe eintragen.");
	QTimer::singleShot(3000, this, [this] () { ui->errorMsg2->setText(""); });
}

//Checkt ob das Feld leer ist
void startWindow::checkDel() {
	vocGroupFilled = ui->select_Group->text();
	del = ui->select_Del->text().trimmed();
	
	if(del.isEmpty()){
		if(vocGroupFilled.isEmpty()) {
			errorDelete2();
		} else {
			errorDelete();
		}
	} else {
		if(vocGroupFilled.isEmpty()) {
			errorDelete2();
		} else {
			loadingAnimation();
			
			vocDelete();
			
			movie->stop();
			ui->stackedWidget->setCurrentIndex(2);
			ui->stackedWidget_3->setCurrentIndex(0);
			checkIfBegin();
		}	
	}
}

//Gesammtfunktion um Vokabeln zu löschen (Button)
void startWindow::on_selectDel_clicked() {
	groupVoc = ui->select_Group->text().trimmed().toLower();
	QVector<QString> english;
	QVector<QString> german;
	
	checkDel();
	//Checkt ob Vokabelgruppe eingetragen wurde
	if(!(groupVoc.isEmpty())) {
		english = loadFrom(OutputFolder +"/VocGroup/"+ groupVoc +"/english-DE.txt");
		german = loadFrom(OutputFolder +"/VocGroup/"+ groupVoc +"/german-DE.txt");
	}
	
	ui->select_Del->clear();
	clearText();
	
	ui->engText->append(english.join("\n"));
	ui->gerText->append(german.join("\n"));
}

//Gesammtfunktion um Vokabeln zu löschen (Eingabefeld)
void startWindow::on_select_Del_returnPressed() {
	groupVoc = ui->select_Group->text().trimmed().toLower();
	QVector<QString> english;
	QVector<QString> german;
	
	checkDel();
	//Checkt ob Vokabelgruppe eingetragen wurde
	if(!(groupVoc.isEmpty())) {
		english = loadFrom(OutputFolder +"/VocGroup/"+ groupVoc +"/english-DE.txt");
		german = loadFrom(OutputFolder +"/VocGroup/"+ groupVoc +"/german-DE.txt");
	}
	
	ui->select_Del->clear();
	clearText();
	
	ui->engText->append(english.join("\n"));
	ui->gerText->append(german.join("\n"));
}

//Resetet VocGroup bei Vokabeln ansehen
void startWindow::on_resetGroup_clicked() {
	ui->engText->setText("");
	ui->gerText->setText("");
	ui->select_Group->clear();
	ui->errorMsg->clear();
	ui->errorMsg2->clear();
}

//Error Nachricht -> Es existieren keine vocGroups
void startWindow::errorGameAll() {
	ui->errorMsgGameAll->setText("Es wurden keine Vokabeln gefunden.");
	QTimer::singleShot(3000, this, [this] () { ui->errorMsgGameAll->setText(""); });
}

//Progressbar ist abgelaufen und User hat nicht in der Zeit Vokabel eingegeben
void startWindow::timeover() {
	setState = 0;
	calcAlgorithm();
	
	ui->result->setStyleSheet("color: red");
	ui->result->setText("Zeit ist vorbei.");
	QTimer::singleShot(3000, this, [this] () { ui->result->setText(""); });
	emit ui->reset->click();
}

//Stellt Progressbar ein
void startWindow::configurateBar() {
	ui->timeLeft->setValue(0);
	ui->timeLeft->setTextVisible(false);
	ui->timeLeft->setStyleSheet("QProgressBar {border: 1px solid white; background-color: transparent; } QProgressBar::chunk { background-color: rgba(2, 65, 118, 255);}");
	ui->timeLeft->setRange(0, 100);
}

//Deaktiviert Progressbar
void startWindow::disableBar() {
	ui->timeLeft->setStyleSheet("background-color: transparent; border: 0;");
	ui->timeLeft->setValue(0);
	ui->timeLeft->setTextVisible(false);
}

//Animation Progressbar
void startWindow::progressBar() {	
	configurateBar();
	
	//Zeitlichbegrenzte ProgressBar
	QTimeLine *timeLine = new QTimeLine(time, this);
	timeLine->setFrameRange(0, 100);
	connect(timeLine, &QTimeLine::frameChanged, ui->timeLeft, &QProgressBar::setValue);
	timeLine->start();
		
	//Zeit abgelaufen
	connect(timeLine, SIGNAL(finished()), this, SLOT(timeover()));
	
	//Eingabe registriert
	connect(ui->notEmpty, &QPushButton::clicked, timeLine, &QTimeLine::stop);
	
	//Resetet Progressbar wenn Modi gewächselt wird
	connect(ui->begin, &QPushButton::clicked, timeLine, &QTimeLine::stop);
	connect(ui->addition, &QPushButton::clicked, timeLine, &QTimeLine::stop);
	connect(ui->visit, &QPushButton::clicked, timeLine, &QTimeLine::stop);
	connect(ui->power, &QPushButton::clicked, timeLine, &QTimeLine::stop);
}

//Deaktiviert Buttons
void startWindow::setButtons() {
	ui->stackedWidget_5->setCurrentIndex(1);
	ui->begin->setEnabled(false);
	ui->visit->setEnabled(false);
	ui->addition->setEnabled(false);
	ui->end->setEnabled(false);
}

//Übersichtsseite
void startWindow::setBack() {
	ui->stackedWidget_3->setCurrentIndex(3);
	ui->stackedWidget_5->setCurrentIndex(0);
	
	ui->begin->setEnabled(true);
	ui->visit->setEnabled(true);
	ui->addition->setEnabled(true);
	ui->end->setEnabled(true);
	
	if(!(placeholder.isEmpty())) {
		sortAlgorithm();
	}
}

//Wählt zwischen EN und DE
void startWindow::modeOfGame() {
	if(check) {
		sign = "DE";
		algorithmPath = "german";
	} else {
		sign = "EN";
		algorithmPath = "english";
	}
}

//Dreht Vektor um und läd in Queue
void startWindow::turnAndLoad() {
	reverse(germanReserve.begin(), germanReserve.end());
	reverse(englishReserve.begin(), englishReserve.end());
	
	QQueue<QString> duration_german = convertToQueue(germanReserve);
	QQueue<QString> duration_english = convertToQueue(englishReserve);
	
	germanReserve.clear();
	englishReserve.clear();
	
	if(check) {
		language1 = duration_german;
		language2 = duration_english;
	} else {
		language1 = duration_english;
		language2 = duration_german;
	}
}

//Läd Vektoren für Trainer
void startWindow::loadAll() {
	modeOfGame();
	
	loadingAnimation();
	
	german = loadFrom(OutputFolder +"/VocGroup/" + groupVoc +"/german-"+ sign +".txt");
	english = loadFrom(OutputFolder +"/VocGroup/" + groupVoc +"/english-"+ sign +".txt");
	germanReserve = german;
	englishReserve = english;
	turnAndLoad();
	
	movie->stop();
}

//Übergang zum Training + Gibt Werte von gameSelection in Settings
void startWindow::occurrenceSelection() {
	gameSelection.setModal(true);
	
	if(gameSelection.exec() == QDialog::Accepted) {
		check = gameSelection.boolValue();
		mode = gameSelection.gamemode();
		time = gameSelection.setTime();
		
		setButtons();
		loadAll();
		
		ui->result->clear();
		emit ui->go->click();
		
		ui->stackedWidget_3->setCurrentIndex(1);
	}
	
	gameSelection.resetAll();
}

//Gesammtfunktion für das Starten aller vocGroups
void startWindow::on_startAll_clicked() {
	groupVoc = "";
	
	if(dataEng.isEmpty()) {
		errorGameAll();
	} else {
		occurrenceSelection();
	}
}

//Error Nachricht -> Feld ausfüllen
void startWindow::errorGame() {
	ui->errorMsgGame->setText("Lücke ausfüllen.");
	QTimer::singleShot(3000, this, [this] () { ui->errorMsgGame->setText(""); });
}

//Check ob Trainer mit jeweiligen vocGroup gestartet werden kann
void startWindow::folderCheckGame() {
	//Checkt ob Ordner leer ist
	if(QDir(OutputFolder +"/VocGroup/"+ groupVoc).entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() == 0) {
		ui->errorMsgGame->setText("Vokabelgruppe ist leer.");
		QTimer::singleShot(3000, this, [this] () { ui->errorMsgGame->setText(""); });
	} else {
		dataEng = loadFrom(OutputFolder +"/VocGroup/"+ groupVoc +"/english-DE.txt");
		//Checkt ob Englische Datei leer ist
		if(dataEng.isEmpty()) {
			ui->errorMsgGame->setText("Vokabelgruppe ist leer.");
			QTimer::singleShot(3000, this, [this] () { ui->errorMsgGame->setText(""); });
		} else {
			occurrenceSelection();
		}
	}
}

//Checkt ob eingegebene Vokabelgruppe existiert
void startWindow::checkExistGame() {
	search = convertToSet(vocgroup);
	
	if(search.find(groupVoc) != search.end()) {
		folderCheckGame();
	} else {
		ui->errorMsgGame->setText("Vokabelgruppe existiert nicht.");
		QTimer::singleShot(3000, this, [this] () { ui->errorMsgGame->setText(""); });
		
		ui->groupGame->clear();
	}
}

//Gesammtfunktion für das Starten einer vocGroup (Button)
void startWindow::on_start_clicked() {
	groupVoc = ui->groupGame->text().trimmed().toLower();
	
	if(groupVoc.isEmpty()){
		errorGame();
	} else {		
		checkExistGame();
	}
}

//Gesammtfunktion für das Starten einer vocGroup (Eingabefeld)
void startWindow::on_groupGame_returnPressed() {
	groupVoc = ui->groupGame->text().trimmed().toLower();
	
	if(groupVoc.isEmpty()){
		errorGame();
	} else {	
		checkExistGame();
	}
}

//Checkt settings ob Progressbar aktiv sein soll oder nicht
void startWindow::setProgressbar() {
	if(mode) {
		progressBar();
	} else {
		disableBar();
	}
}

//Wertet Vokabeln aus
void startWindow::calcAlgorithm() {
			
	if(groupVoc.isEmpty()) {
		placeholder = "all";
	} else {
		placeholder = groupVoc;
	}
	
	int line = line_of_string(OutputFolder +"/VocGroup/"+ groupVoc +"/"+ algorithmPath +"-"+ sign +".txt", word2);
	QVector<int> algorithm = loadFromInt(OutputFolder +"/logs/algorithm/"+ placeholder +"-"+ sign +".txt");
	int num = algorithm[line - 1];

	QString value;
	
	if(setState == 1) {
		num++;
		value = QString::number(num);
	} else {
		value = "1";
	}
	
	del_rep_line(OutputFolder +"/logs/algorithm/"+ placeholder +"-"+ sign +".txt", value, line, "rep");
}

//Verbindet Vektoren
QVector<QPair<int, QPair<QString, QString>>> startWindow::zip(QVector<int> &algorithm, QVector<QString> &english, QVector<QString> &german) {
	QVector<QPair<int, QPair<QString, QString>>> zipped;
	
    for(int i = 0; i < algorithm.size(); ++i) {
        zipped.push_back(qMakePair(algorithm[i], qMakePair(english[i], german[i])));
    }
	
	return zipped;
}

//Beendet Verbindung zwischen Vektoren
void startWindow::unzip(QVector<QPair<int, QPair<QString, QString>>> &zipped, QVector<int> &algorithm, QVector<QString> &english, QVector<QString> &german) {
    for(int i = 0; i < algorithm.size(); i++) {
        algorithm[i] = zipped[i].first;
        english[i] = zipped[i].second.first;
		german[i] = zipped[i].second.second;
    }
}

bool sortbyFirst(const QPair<int, QPair<QString, QString>> &a, const QPair<int, QPair<QString, QString>> &b) {
       return a.first < b.first;
}

//Sortiert Vokabeln
void startWindow::sortAlgorithm() {
	QVector<int> algorithm = loadFromInt(OutputFolder +"/logs/algorithm/"+ placeholder +"-"+ sign +".txt");
	
	QVector<QPair<int, QPair<QString, QString>>> zipped = zip(algorithm, english, german);
	
	random_shuffle(zipped.begin(), zipped.end());
	sort(zipped.rbegin(), zipped.rend(), sortbyFirst);
	
	unzip(zipped, algorithm, english, german);
	
	removeFile(OutputFolder +"/logs/algorithm/"+ placeholder +"-"+ sign +".txt");
	removeFile(OutputFolder +"/VocGroup/"+ groupVoc +"/german-"+ sign +".txt");
	removeFile(OutputFolder +"/VocGroup/"+ groupVoc +"/english-"+ sign +".txt");
	
	for(int i = 0; i < algorithm.size(); i++) {
        safeIn(QString::number(algorithm[i]), OutputFolder +"/logs/algorithm/"+ placeholder +"-"+ sign +".txt");
		safeIn(english[i], OutputFolder +"/VocGroup/"+ groupVoc +"/english-"+ sign +".txt");
		safeIn(german[i], OutputFolder +"/VocGroup/"+ groupVoc +"/german-"+ sign +".txt");
    }
}

void startWindow::newTask() {
	QString english;
	QString german;
	
	if(check) {
		english = word1;
		german = word2;
	} else {
		german = word1;
		english = word2;
	}
	
	englishReserve.push_back(english);
	germanReserve.push_back(german);
}

//Eingabe check des Users
void startWindow::truefalse() {
	if(userInput == word2) {
		ui->result->setStyleSheet("color: green");
		ui->result->setText("Richtig");
		setState = 1;
	} else {
		ui->result->setStyleSheet("color: red");
		ui->result->setText("Falsch, Richtig gewesen wäre " + word2);
		setState = 0;
		
		newTask();
	}
	
	calcAlgorithm();
}

//Error Nachricht -> Feld ausfüllen
void startWindow::errorUser() {
	ui->errorMsgPower->setStyleSheet("color: red");
	ui->errorMsgPower->setText("Lücke ausfüllen.");
	QTimer::singleShot(3000, this, [this] () { ui->errorMsgPower->setText(""); });
}

void startWindow::queue() {
	if(language1.size() > 0) {
		word1 = language2.dequeue();
		word2 = language1.dequeue();
	} else {
		if(englishReserve.size() > 0) {
			loadingAnimation();
			sortAlgorithm();
			turnAndLoad();
			movie->stop();

			emit ui->go->click();
		} else {
			setBack();
		}
	}
}

void startWindow::process() {
	QObject* button = QObject::sender();
	
	//Aller erster durchlauf
	if(button != ui->go) {
		truefalse();
	}
	
	setProgressbar();
	queue();
}

//Gesammtfunktion für den Trainer (Eingabefeld)
void startWindow::on_answer_returnPressed() {	
	QObject* button = QObject::sender();
	userInput = ui->answer->text().trimmed();
	
	//Checkt ob Progressbar voll ist
	if(button != ui->reset) {
		//Checkt ob erster Durchlauf
		if(button == ui->go) {
			ui->stackedWidget->setCurrentIndex(3);
			ui->stackedWidget_3->setCurrentIndex(1);
			process();
		} else {			
			if(userInput.isEmpty()) {
				errorUser();
			} else {
				emit ui->notEmpty->click();
				process();
			} 
		}
	} else {
		progressBar();
		queue();
	}
	
	ui->word->setWordWrap(true);
	ui->word->setText(word1);
	ui->answer->clear();
}

//Gesammtfunktion für den Trainer (Button)
void startWindow::on_answerButton_clicked() {
	QObject* button = QObject::sender();
	userInput = ui->answer->text().trimmed();
	
	//Checkt ob Progressbar voll ist
	if(button != ui->reset) {
		//Checkt ob erster Durchlauf
		if(button == ui->go) {
			ui->stackedWidget->setCurrentIndex(3);
			ui->stackedWidget_3->setCurrentIndex(1);
			setProgressbar();	
			process();
		} else {			
			if(userInput.isEmpty()) {
				errorUser();
			} else {
				emit ui->notEmpty->click();	
				process();
			} 
		}
	} else {
		progressBar();
		queue();
	}
	
	ui->word->setWordWrap(true);
	ui->word->setText(word1);
	ui->answer->clear();
}
