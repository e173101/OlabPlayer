#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void refresh(void);

    void on_pushButton_snapshot_clicked();

    void on_pushButton_openVideo_clicked();

private:
    Ui::MainWindow *ui;
    QTimer timer;
    QVector<int> frameBoxLife;
    QVector<QLabel*> frameBox;
    QVector<QLabel*> frameImg;

    cv::VideoCapture video;
    QImage mainVideo;
};

#endif // MAINWINDOW_H
