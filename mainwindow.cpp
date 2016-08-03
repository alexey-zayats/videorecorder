#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCameraViewfinder>
#include <QCamera>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaRecorder>
#include <QCameraInfo>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {
           qDebug() << cameraInfo.deviceName();
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

    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/mp3");
    audioSettings.setQuality(QMultimedia::HighQuality);

    mediaRecorder->setAudioSettings(audioSettings);

    camera->setCaptureMode(QCamera::CaptureVideo);
    camera->start();//8

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
        QString name = QString("xxx") + QDateTime::currentDateTime().toString("dd.MM.yy-h-m-s");
        qDebug() << qApp->applicationDirPath();
        qDebug() << name;

        mediaRecorder->setOutputLocation(QUrl::fromLocalFile(qApp->applicationDirPath() + "/" + name + ".mp4"));
        mediaRecorder->record();
    } else {
        ui->stackedWidget->setCurrentIndex(cameraFinderPos);
        mediaRecorder->stop();
    }
}
