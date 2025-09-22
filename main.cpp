#include "vocabeltrainer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    vocabeltrainer w;
	w.setWindowIcon(QIcon(":/icon/images/vocicon.ico"));
    w.show();
    return a.exec();
}
