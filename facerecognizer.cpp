#include "facerecognizer.h"
#include "ui_facerecognizer.h"

FaceRecognizer::FaceRecognizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaceRecognizer)
{
    ui->setupUi(this);

    //====================== Нало полезного кода ===============================
    // Пробуем открыть файл
    m_oCVCapture.open( FILE_NAME );
    // Пробуем открыть камеру
    //m_oCVCapture.open( 0 );

    // Если неудалось
    if( !m_oCVCapture.isOpened() )
    {
        // Сообщить о неудаче
        qDebug() << "Не удалось открыть файл или камеру!";
        return;
    }
    // Вычитываем первый кадр
    m_oCVCapture >> m_oCVMat;

    //m_oCVMat = cv::imread( PHOTO_NAME, CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! m_oCVMat.data )                              // Check for invalid input
    {
        qDebug() <<  "Could not open or find the image" ;
        return;
    }

    // Создать мзображение Qt c таким же размером
    m_oQtImage = QImage( QSize( m_oCVMat.cols, m_oCVMat.rows ), QImage::Format_RGB888 );

    // Установить размеры окна по размеру изображения
    this->setMinimumSize( m_oQtImage.width(), m_oQtImage.height() );
    // Максимальные равны минимальным
    this->setMaximumSize( this->minimumSize() );

    // Привязываем данные фрейма к озображнию
    m_oCVMat.data = (uchar*)m_oQtImage.bits();

    // Редпетируем цветовую схему
    cv::cvtColor( m_oCVMat, m_oCVMat, CV_BGR2RGB );

    // Load the cascades
    if( !m_pCVFaceCascade.load( FACE_CASCADE_NAME ) )
    {
        qDebug() << ("--(!)Error loading face cascade\n");
        return;
    }

    if( !m_pCVEyeCascade.load( EYE_CASCADE_NAME ) )
    {
        qDebug() << ("--(!)Error loading eyes cascade\n");
        return;
    }

    // Set timer for 50ms intervals
    m_pQtTimer = new QTimer(this);
    // Подключить слот
    connect( m_pQtTimer, SIGNAL(timeout()), this, SLOT( queryFrame() ) );
    // Запустить таймер
    m_pQtTimer->setSingleShot( true );
    m_pQtTimer->start(50);
}

FaceRecognizer::~FaceRecognizer()
{
    delete ui;
}

void FaceRecognizer::detectAndDraw()
{
    // Список прямоугольников для лиц
    std::vector < cv::Rect >   faces;
    // Временный фрейм
    cv::Mat             frame_gray;

    // Преобразовать цвета
    cv::cvtColor( m_oCVMat, frame_gray, CV_RGB2GRAY  );

    // Выровнять гистограмму
    cv::equalizeHist( frame_gray, frame_gray );

    std::vector < cv::Rect > ::size_type    temp_size;
    cv::Rect                                temp_rect(1,1,1,1);

    faces.clear();

    // Определить список лиц
    // m_pCVFaceCascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0/* | CV_HAAR_SCALE_IMAGE*/,  cv::Size(100, 100));

    temp_size = faces.size();

   // if( faces.size() > 5 )
/*
    // Для каждого лица в списке
    for( size_t i = 0; i < faces.size(); i++ )
    {
        // Скопировать само лицо
        cv::Mat faceROI = frame_gray( faces[i] );

        // Зарезервировать массив глаз
        std::vector<cv::Rect> eyes;
        // На каждом лице найти глаза
        m_pCVEyeCascade.detectMultiScale( faceROI, eyes, 1.1, 2, ( 0 | CV_HAAR_SCALE_IMAGE ) );
        // Если в списке 2 глаза
        if( eyes.size() == 2)
        {
            // Координаты центра лица
            cv::Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );

            // Нарисовать овал на изображении
            cv::ellipse( m_oCVMat, center, cv::Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, cv::Scalar( 255, 0, 0 ), 2, 8, 0 );

            // Для каждого глаза в списке
            for( size_t j = 0; j < eyes.size(); j++ )
            {
                // Координты центра глаза
                cv::Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2,
                                      faces[i].y + eyes[j].y + eyes[j].height/2 );

                // Расчитать радиус
                int radius = cvRound( (eyes[j].width + eyes[j].height) * 0.25 );
                cv::circle( m_oCVMat, eye_center, radius, cv::Scalar( 255, 0, 255 ), 3, 8, 0 );
            }
        }
    }
    */
}

void FaceRecognizer::queryFrame()
{
    // Вычитываем следующий кадр
    m_oCVCapture >> m_oCVMat;
    // m_oCVMat = cv::imread( PHOTO_NAME, CV_LOAD_IMAGE_COLOR);   // Read the file


    // Преобразовать цвета
    cv::cvtColor( m_oCVMat, m_oCVMat, CV_BGR2RGB  );

    // Если кадр не пуст
    if ( !m_oCVMat.empty() )
    {
        // Определить расположение лица
        this->detectAndDraw();
    }

    // Обновить окно
    this->update();

    // Перезапустить таймер
    m_pQtTimer->start(50);
}

// Finally - painting, which is easy:
void FaceRecognizer::paintEvent(QPaintEvent* event)
{
    // Класс для рисования
    QPainter painter( (QWidget*)this );
    // Выводим изображение
    painter.drawImage( 0, 0, m_oQtImage);
}
