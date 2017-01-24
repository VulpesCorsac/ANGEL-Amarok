#include "Amarok.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Amarok w;
    w.show();

    return a.exec();
}
