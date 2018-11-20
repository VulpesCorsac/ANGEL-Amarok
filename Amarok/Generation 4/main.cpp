#include "Amarok.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Amarok w;

//    QBrush br(Qt::TexturePattern);
//    br.setTextureImage(QImage(":/Amarok.png"));
//    QPalette plt = w.palette();
//    plt.setBrush(QPalette::Background, br);
//    w.setPalette(plt);

    w.show();

    return a.exec();
}
