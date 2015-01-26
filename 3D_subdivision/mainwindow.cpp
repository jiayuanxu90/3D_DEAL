#include "mainwindow.h"

#include <QApplication>
#include <QMenuBar>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QMessageBox>

#include "glwidget.h"



MainWindow::MainWindow()
{
    switch (QMessageBox::information(0,
        "Start in fullscreen?",
        "Would you like to run this program in fullscreen mode?",
        QMessageBox::Yes,
        QMessageBox::No | QMessageBox::Default)) {
    case QMessageBox::Yes:
        fs = true;
        break;
    case QMessageBox::No:
        fs = false;
        break;
    default:
        break;
    }

    glwidget = new GLWidget();
    QLabel * label = new QLabel(this);
    QTimer * timer = new QTimer(this);
    QSlider * slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);

    xSlider = createSlider();
    ySlider = createSlider();
    zSlider = createSlider();

    slider->setRange(0, 100);
    slider->setSliderPosition(50);
    timer->setInterval(10);
    label->setText("3D model deal");
    label->setAlignment(Qt::AlignCenter);

    QGroupBox * groupBox = new QGroupBox(this);
    setCentralWidget(groupBox);
    groupBox->setTitle("OpenGL for 3D");

    QGridLayout * gridLayout = new QGridLayout(groupBox);
    QHBoxLayout * hboxLayout = new QHBoxLayout();
    hboxLayout->addWidget(glwidget);
    hboxLayout->addWidget(xSlider);
    hboxLayout->addWidget(ySlider);
    hboxLayout->addWidget(zSlider);

    gridLayout->addLayout(hboxLayout, 1, 0, 8, 1);
    gridLayout->addWidget(label, 9, 0, 1, 1);
    gridLayout->addWidget(slider, 11, 0, 1, 1);

    groupBox->setLayout(gridLayout);


    // menu
    QMenu * fileMenu = new QMenu("File");
    QMenu * opsMenu = new QMenu("Operations");
    QMenu * helpMenu = new QMenu("Help");

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(opsMenu);
    menuBar()->addMenu(helpMenu);

    QAction * openFile = new QAction("Open", fileMenu);
    QAction * exit = new QAction("Exit", fileMenu);
    QAction * s_butterfly = new QAction("s_butterfly", opsMenu);
    QAction * aboutQt = new QAction("About", helpMenu);

    fileMenu->addAction(openFile);
    fileMenu->addAction(exit);
    opsMenu->addAction(s_butterfly);
    helpMenu->addAction(aboutQt);

    QObject::connect(exit, SIGNAL(triggered(bool)), this, SLOT(close()));
    QObject::connect(aboutQt, SIGNAL(triggered(bool)), this, SLOT(about()));
    QObject::connect(openFile, SIGNAL(triggered(bool)), glwidget, SLOT(openFile()));
    QObject::connect(s_butterfly, SIGNAL(triggered(bool)), glwidget, SLOT(subdivideButterfly()));




    QObject::connect(slider, SIGNAL(valueChanged(int)), glwidget, SLOT(setScaling(int)));

    QObject::connect(xSlider, SIGNAL(valueChanged(int)), glwidget, SLOT(setXRotation(int)));
    QObject::connect(glwidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));
    QObject::connect(ySlider, SIGNAL(valueChanged(int)), glwidget, SLOT(setYRotation(int)));
    QObject::connect(glwidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setValue(int)));
    QObject::connect(zSlider, SIGNAL(valueChanged(int)), glwidget, SLOT(setZRotation(int)));
    QObject::connect(glwidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setValue(int)));

    if (fs) {
        showFullScreen();
    }
}


MainWindow::~MainWindow()
{

}


QSlider * MainWindow::createSlider()
{
    QSlider * slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 360*16);
    slider->setSingleStep(16);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(15 * 16);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_F2:
        fs = !fs;
        if (fs)
            showFullScreen();
        else
            this->showNormal();
        glwidget->update();
        break;
    default:
        break;
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, "About", "This is a Qt app dealing with 3D mesh.\n"
                                      "                      @Author: jiayuan_xu\n");
}
