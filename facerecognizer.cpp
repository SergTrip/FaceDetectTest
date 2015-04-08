#include "facerecognizer.h"
#include "ui_facerecognizer.h"

FaceRecognizer::FaceRecognizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaceRecognizer)
{
    ui->setupUi(this);

    //====================== Нало полезного кода ===============================
    // Открыть файл
    // m_pOpenCVCapture = cvCaptureFromAVI( "../../../Data/video.avi" );
    m_pOpenCVCapture = cvCaptureFromFile( "../../../Data/MyFace.3gp" );

    //grab one frame to get width and height
    IplImage* frame = cvQueryFrame( m_pOpenCVCapture );

    // Создать мзображение Qt c таким же размером
    m_oQtImage = QImage( QSize( frame->width, frame->height ), QImage::Format_RGB888 );

    // Установить размеры окна по размеру изображения
    ((QWidget*)ui)->setMinimumSize( m_oQtImage.width(), m_oQtImage.height());
    // Максимальные равны минимальным
    ((QWidget*)ui)->setMaximumSize( ((QWidget*)ui)->minimumSize() );

    // Копирование изображения в OpenCV Image
    // Сreate only the header, as the data buffer is shared, and was allocated by QImage
    m_pOpenCVImage = cvCreateImageHeader( cvSize( m_oQtImage.width(), m_oQtImage.height() ), 8, 3 );
    // Share buffers
    m_pOpenCVImage->imageData = (char*)m_oQtImage.bits();

    // Двойная буфферизация ???
    if( frame->origin == IPL_ORIGIN_TL )
        cvCopy( frame, m_pOpenCVImage, 0 );
    else
        cvFlip( frame, m_pOpenCVImage, 0 );

    // Images from cvQueryFrame come in BGR form and not what Qt expects - RGB
    // and since the buffers are shared - format should be consistent
    cvCvtColor( m_pOpenCVImage, m_pOpenCVImage, CV_BGR2RGB);

    //we need memstorage and a cascade
    m_pCvMemoryStorage  = cvCreateMemStorage( 0 );
    m_pOpenCVCascade    = (CvHaarClassifierCascade*)cvLoad( CASCADE_NAME, 0, 0, 0 );

    // Set timer for 50ms intervals
    m_pQtTimer = new QTimer(this);
    // Подключить слот
    connect( m_pQtTimer, SIGNAL(timeout()), this, SLOT( queryFrame() ) );
    // Запустить таймер
    m_pQtTimer->start(50);
}

FaceRecognizer::~FaceRecognizer()
{
    delete ui;
}

CvRect FaceRecognizer::detectAndDraw(IplImage *img, CvMemStorage *storage, CvHaarClassifierCascade *cascade)
{
    // Пдготовить возвращаемое значение
    CvRect res;
    res = cvRect( -1, -1, 0, 0 );

    // Указатели на экземпляры изображений
    IplImage *pGrayImage, *pSmallImage;
    // Пока не понятно
    int i = 0;

    // Создаем серое мзображение заданных размеров
    pGrayImage = cvCreateImage( cvSize( img->width, img->height ), 8, 1 );

    int scale = 5;
    // Создаем уменьшенное изображение ( Откуд взялся Scale?? )
    pSmallImage = cvCreateImage( cvSize( cvRound (img->width/scale), cvRound (img->height/scale)), 8, 1 );

    // Копируем исходное изображение с преобразовением цвета
    cvCvtColor      ( img, pGrayImage, CV_RGB2GRAY );
    // Уменьшить серый экземпляр до заданного размера
    cvResize        ( pGrayImage, pSmallImage, CV_INTER_LINEAR );
    // Корректируем гистограмму
    cvEqualizeHist  ( pSmallImage, pSmallImage );

    // Чистим хранилище
    cvClearMemStorage( storage );

    // Экземпляр прямоугольника
    CvRect* r = NULL;

    // Если объект существует
    if( cascade )
    {
        double t;

        // Захватить текущее количество тиков
        t = (double)cvGetTickCount();

        // Вызвать функцию обнаружения объектов
        CvSeq* faces = cvHaarDetectObjects( pSmallImage, cascade, storage, 1.1, 2,
                                            0  | CV_HAAR_FIND_BIGGEST_OBJECT
                                                //|CV_HAAR_DO_ROUGH_SEARCH
                                                //|CV_HAAR_DO_CANNY_PRUNING
                                                //|CV_HAAR_SCALE_IMAGE
                                                ,
                                                cvSize(30, 30) );

        // Получить разницу в тиках (время затраченное на алгоритм)
        t = (double)cvGetTickCount() - t;

        // Вывисти время в миллисекундах
        qDebug() << ( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );

        r = (CvRect*)cvGetSeqElem( faces, i );

        cvReleaseImage( &pGrayImage     );
        cvReleaseImage( &pSmallImage    );

        if( r )
        {
            res = cvRect(r->x,r->y,r->width,r->height);
        }
    }

    // Вернуть результат
    return res;
}

void FaceRecognizer::queryFrame()
{
    IplImage* frame = cvQueryFrame( m_pOpenCVCapture );

    if( frame->origin == IPL_ORIGIN_TL )
        cvCopy( frame, m_pOpenCVImage, 0 );
    else
        cvFlip( frame, m_pOpenCVImage, 0 );

    cvCvtColor( m_pOpenCVImage, m_pOpenCVImage, CV_BGR2RGB );

    CvRect r = detectAndDraw( m_pOpenCVImage, m_pCvMemoryStorage, m_pOpenCVCascade);

    m_oQtFaceLocationRect = QRect( QPoint( r.x, r.y ), QSize( r.width, r.height ));

    this->update();
}

// Finally - painting, which is easy:
void FaceRecognizer::paintEvent(QPaintEvent* event)
{
    QPainter painter( (QWidget*)this );

    painter.drawImage( QPoint( ((QWidget*)ui)->x(), ((QWidget*)ui)->y() ), m_oQtImage);

    if ( m_oQtFaceLocationRect.x() > 0 && m_oQtFaceLocationRect.y() > 0 )
    {
        painter.setBrush    ( Qt::NoBrush               );
        painter.setPen      ( QColor    ( 255, 0, 0)    );

        painter.drawRect    ( QRect     ( m_oQtFaceLocationRect.x() + ((QWidget*)ui)->x(),
                                          m_oQtFaceLocationRect.y() + ((QWidget*)ui)->y(),
                                          m_oQtFaceLocationRect.width (),
                                          m_oQtFaceLocationRect.height()  ) );
    }
}
