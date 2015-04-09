#ifndef FACERECOGNIZER_H
#define FACERECOGNIZER_H

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <QDebug>

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

//#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

//#include <opencv2/videoio/videoio.hpp>

#include <opencv2/highgui/highgui.hpp>

#include <opencv2/objdetect/objdetect.hpp>


#define FACE_CASCADE_NAME   "../../Data/haarcascades/haarcascade_frontalface_alt.xml"
//#define FACE_CASCADE_NAME   "../../Data/lbpcascades/lbpcascade_frontalface.xml"

#define EYE_CASCADE_NAME    "../../Data/haarcascades/haarcascade_eye_tree_eyeglasses.xml"

#define FILE_NAME           "../../Data/MyFace.3gp"
#define PHOTO_NAME          "../../Data/Face.jpg"

namespace Ui {
class FaceRecognizer;
}

//  using namespace cv;
using namespace std;

class FaceRecognizer : public QWidget
{
    Q_OBJECT

public:
    explicit FaceRecognizer(QWidget *parent = 0);
    ~FaceRecognizer();

private:
    // Непосредственно опеределяет расположение лица
    void detectAndDraw();

private:
    Ui::FaceRecognizer *ui;

    // Экземпляр изображения Qt
    QImage  m_oQtImage;
    // Охватывающий прямоугольник
    QRect   m_oQtFaceLocationRect;
    // Экземпляр таймера
    QTimer* m_pQtTimer;

    // Хранилище с динамически изменяемым размером
    cv::Mat                 m_oCVMat;
    cv::VideoCapture        m_oCVCapture;

    cv::CascadeClassifier   m_pCVFaceCascade;
    cv::CascadeClassifier   m_pCVEyeCascade;

    // Реализация унаследованного класса перерисовки
    void paintEvent(QPaintEvent* event);

public slots:
    void queryFrame();
};

#endif // FACERECOGNIZER_H
