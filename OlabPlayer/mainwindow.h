#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

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

private:
    Ui::MainWindow *ui;
    QTimer timer;
    QVector<int> frameBoxLife;
};

#endif // MAINWINDOW_H
