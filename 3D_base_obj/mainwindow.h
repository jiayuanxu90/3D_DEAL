#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSlider;

class GLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent * e);

private:
    bool fs;

    GLWidget * glwidget;

    QSlider * createSlider();

    QSlider * xSlider;
    QSlider * ySlider;
    QSlider * zSlider;

};

#endif // MAINWINDOW_H
