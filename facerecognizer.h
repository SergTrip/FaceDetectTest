#ifndef FACERECOGNIZER_H
#define FACERECOGNIZER_H

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <QDebug>

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

#include <opencv2/objdetect.hpp>

//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"

#define CASCADE_NAME    "../../Data/haarcascades/haarcascade_eye_tree_eyeglasses.xml"
#define FILE_NAME       "../../Data/MyFace.3gp"

namespace Ui {
class FaceRecognizer;
}

class FaceRecognizer : public QWidget
{
    Q_OBJECT

public:
    explicit FaceRecognizer(QWidget *parent = 0);
    ~FaceRecognizer();

private:
    // Непосредственно опеределяет расположение лица
    CvRect detectAndDraw( IplImage* img, CvMemStorage* storage, CvHaarClassifierCascade* cascade );

private:
    Ui::FaceRecognizer *ui;

    // Экземпляр изображения Qt
    QImage  m_oQtImage;
    // Охватывающий прямоугольник
    QRect   m_oQtFaceLocationRect;
    // Экземпляр таймера
    QTimer* m_pQtTimer;

    // Хранилище с динамически изменяемым размером
    CvMemStorage*               m_pCvMemoryStorage;
    CvHaarClassifierCascade*    m_pOpenCVCascade;
    // IplImage*                   m_pOpenCVImage;

    cv::Mat m_oCVMat;

    // CvCapture*                  m_pOpenCVCapture;
    cv::VideoCapture            m_oCVCapture;

    // Реализация унаследованного класса перерисовки
    void paintEvent(QPaintEvent* event);

public slots:
    void queryFrame();
};

#endif // FACERECOGNIZER_H
