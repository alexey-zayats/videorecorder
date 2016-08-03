#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class QCamera;
class QMediaPlayer;
class QVideoWidget;
class QMediaRecorder;
class QCameraViewfinder;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void recordButton_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QCamera *camera;
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QMediaRecorder *mediaRecorder;
    QCameraViewfinder *cameraViewFinder;

    int cameraFinderPos;
    int videoWidgetPos;
};

#endif // MAINWINDOW_H
