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

    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

private:

    //All kinds of situations
    enum situation{
        BAD_IMG,
        VERYBAD_IMG,
        OMG
    };

    Ui::MainWindow *ui;
    QTimer timer;
    QVector<int> frameBoxLife;
    QVector<QPushButton*> frameBox;
    QVector<QLabel*> frameImg;

    int addSnapshot(situation s);
    int delSnapshot(int ind);
    cv::VideoCapture video;
    QImage mainVideo;
    bool flagPlay;
};

#endif // MAINWINDOW_H
