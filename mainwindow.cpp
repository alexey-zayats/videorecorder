#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCameraViewfinder>
#include <QCamera>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaRecorder>
#include <QCameraInfo>
#include <QDateTime>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {
           qDebug() << cameraInfo.description();
    }

    // return;

    cameraViewFinder = new QCameraViewfinder(this);

    camera = new QCamera(this);
    camera->setViewfinder(cameraViewFinder);

    player = new QMediaPlayer(this);

    videoWidget = new QVideoWidget(this);

    player->setVideoOutput(videoWidget);

    cameraFinderPos = ui->stackedWidget->addWidget(cameraViewFinder);
    videoWidgetPos = ui->stackedWidget->addWidget(videoWidget);

    mediaRecorder = new QMediaRecorder(camera,this);

    QVideoEncoderSettings videoSettings = mediaRecorder->videoSettings();
     videoSettings.setResolution(640, 480);
     videoSettings.setQuality(QMultimedia::VeryHighQuality);
    mediaRecorder->setVideoSettings(videoSettings);

    QVariantMap opts = videoSettings.encodingOptions();
    foreach ( QString key, opts.keys() ) {
        qDebug() << key << ": " << opts[key];

    }

    QAudioEncoderSettings audioSettings = mediaRecorder->audioSettings();
     audioSettings.setCodec("audio/mp3");
     audioSettings.setQuality(QMultimedia::HighQuality);

    mediaRecorder->setAudioSettings(audioSettings);

    qDebug() << "Supported audio codecs: " << mediaRecorder->supportedAudioCodecs();
    qDebug() << "Supported video codecs: " <<  mediaRecorder->supportedVideoCodecs();

    camera->setCaptureMode(QCamera::CaptureVideo);
    camera->start();

    connect(ui->recordButton, SIGNAL(toggled(bool)), this, SLOT(recordButton_toggled(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recordButton_toggled(bool checked)
{
    if ( checked ) {
        ui->stackedWidget->setCurrentIndex(cameraFinderPos);

        QDir appDir = qApp->applicationDirPath();

#ifdef Q_OS_MAC
        appDir.cdUp();
        appDir.cdUp();
        appDir.cdUp();
#endif

        QString fileName = appDir.absolutePath() + QString("/xxx-") + QDateTime::currentDateTime().toString("dd.MM.yy-h-m-s") + QString(".avi");

        qDebug() << fileName;


        mediaRecorder->setOutputLocation(fileName);
        mediaRecorder->record();
        ui->recordButton->setText(QString("Recording is ON"));
    } else {
        ui->stackedWidget->setCurrentIndex(cameraFinderPos);
        mediaRecorder->stop();
        ui->recordButton->setText(QString("Recording is Off"));
    }
}
