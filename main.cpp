#include "facerecognizer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FaceRecognizer w;
    w.show();

    return a.exec();
}
