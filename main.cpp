#include "socialnetwork.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    socialnetwork w;
    w.show();
    return a.exec();
}
