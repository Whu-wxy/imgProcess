#include "imgprocess.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImgProcess w;
    w.show();
//try!
    return a.exec();
}
