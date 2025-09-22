#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QDialog>
#include <QMouseEvent>
#include <QPoint>
#include <QDir>
#include <QString>
#include <direct.h>
#include <QSet>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QFileInfo>
#include <windows.h>
#include <QStandardPaths>
#include <QProgressBar>
#include <QTimeLine>
#include <QStringList>
#include <algorithm>
#include <QPair>
#include <QQueue>
#include <QDebug>
#include <QMovie>
#include <QStackedWidget>
#include <QGraphicsView>

#include <gameselection.h>

using namespace std;

namespace Ui {
class startWindow;
}

extern QString OutputFolder;

class startWindow : public QDialog
{
    Q_OBJECT

public:
    explicit startWindow(QWidget *parent = nullptr);
    ~startWindow();
	
	gameSelection gameSelection;

private slots:

    void on_begin_clicked();
	
	void on_addition_clicked();
	
    void on_visit_clicked();
	
    void on_power_clicked();
	
    void on_end_clicked();

    void on_select1_clicked();

    void on_select2_clicked();

	void on_vocGroup_returnPressed();
	
	void on_vocGer_returnPressed();
	
	void on_vocEng_returnPressed();
	
	void on_back_clicked();
	
	void on_select_Group_returnPressed();
	
	void on_selectGroup_clicked();
	
	void on_selectDel_clicked();
	
	void on_select_Del_returnPressed();
	
	void on_resetGroup_clicked();
	
	void on_answer_returnPressed();
	
	void on_answerButton_clicked();
	
	void on_startAll_clicked();
	
	void on_start_clicked();
	
	void on_groupGame_returnPressed();
	
	void checkIfBegin();
	
	void timeover();
	
	void setBack();
	
	private:
    Ui::startWindow *ui;
	
	QQueue<QString> language1;
	QQueue<QString> language2;
	QVector<QString> vocgroup;
	QVector<QString> englishAll;
	QVector<QString> dataEng;
	QVector<QString> dataGer;
	QVector<QString> german;
	QVector<QString> germanReserve;
	QVector<QString> english;
	QVector<QString> englishReserve;
	QSet<QString> search;
	QString groupVoc;
	QString voc_english;
	QString voc_german;
	QString del;
	QString userInput;
	QString lowVocGroup;
	QString word1;
	QString word2;
	QString algorithmPath;
	QString vocGroupFilled;
	QString placeholder;
	QString sign;
	int lineCountAll;
	int time;
	int setState;
	bool check;
	bool mode;
	QMovie *movie;
	
	void icon();
	void boxShadow();
	void keyPressEvent(QKeyEvent *evt);
	void returnBool();
	void delete_list(QString language);
	void del_rep_line(QString txtFile, QString newValue, int lineCount, QString selection);
	void safeIn(QString safe, QString txtFile);
	void errorTextAdditon();
	void createGroup();
	void errorTextAdditon2();
	void checkExistVocGroup();
	void vocAddition();
	void errorExistEng();
	void addVoc();
	void errorDel();
	void errorVisitGroup();
	void checkExistVisit();
	void folderCheckVisit();
	void visitEmpty();
	void vocDelete();
	void errorDelete();
	void errorDelete2();
	void errorGameAll();
	void errorGame();
	void checkExistGame();
	void occurrenceSelection();
	void folderCheckGame();
	void clearFields();
	void truefalse();
	void errorUser();
	void disableBar();
	void progressBar();
	void configurateBar();
	void clearText();
	void checkDel();
	void colorOnWhich();
	void calcAlgorithm();
	void process();
	void queue();
	void removeFile(QString txtFile);
	void sortAlgorithm();
	void setButtons();
	void setProgressbar();
	void loadAll();
	void success();
	void modeOfGame();
	void newTask();
	void turnAndLoad();
	void loadingAnimation();
	void buffer();
	void bufferAdd();
	void unzip(QVector<QPair<int, QPair<QString, QString>>> &zipped, QVector<int> &algorithm, QVector<QString> &english, QVector<QString> &german);
	int line_of_string(QString txtFile, QString searchString);
	bool fileExists(QString txtFile);
	bool checkOthers();
	QSet<QString> convertToSet(QVector<QString> input);
	QVector<QString> loadFrom(QString txtFile);
	QVector<int> loadFromInt(QString txtFile);
	QQueue<QString> convertToQueue(QVector<QString> input);
	QVector<QPair<int, QPair<QString, QString>>> zip(QVector<int> &algorithm, QVector<QString> &english, QVector<QString> &german);
	
signals:
	void endClicked();
};

#endif // STARTWINDOW_H
