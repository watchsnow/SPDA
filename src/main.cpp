#include "src/SPDA.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSPDA w;
    w.show();

    return a.exec();
}
