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
    //fs = false;
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
    label->setText("3D model dispose");
    label->setAlignment(Qt::AlignCenter);

    QGroupBox * groupBox = new QGroupBox(this);
    setCentralWidget(groupBox);
    groupBox->setTitle("3D mesh display");

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
//    QMenu * opsMenu = new QMenu("Operations");
    QMenu * opsMMenu = new QMenu("Model_Ops");
    QMenu * opsHEMenu = new QMenu("HalfEdge_Ops");
    QMenu * cstr_stable_mesh = new QMenu("cstr_stable_mesh");
    QMenu * reconstrcust = new QMenu("reconstruct");
    QMenu * helpMenu = new QMenu("Help");

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(opsMMenu);
    menuBar()->addMenu(opsHEMenu);
    menuBar()->addMenu(cstr_stable_mesh);
    menuBar()->addMenu(reconstrcust);
    menuBar()->addMenu(helpMenu);

    //QAction * openFile = new QAction("Open", fileMenu);
    QAction * saveAsPly = new QAction("SAVE_PLY", fileMenu);
    QAction * exit = new QAction("Exit", fileMenu);


    QAction * openFileModel = new QAction("Open", opsMMenu);

    // --------------------------- halfedge ops begin ------------------------------
    QAction * openFileHE = new QAction("Open", opsMMenu);
    QAction * subdivision_butterfly_he = new QAction("sub_btfl", opsHEMenu);
    QAction * subdivision_looprev_he = new QAction("sub_lprv", opsHEMenu);
    QAction * partition_he = new QAction("partition", opsHEMenu);
    QAction * reverse_subdivision_he = new QAction("rv_sub", opsHEMenu);
    //QAction * construct_stable_mesh = new QAction("constr_s_m", opsHEMenu);
    // --------------------------- halfedge ops end ------------------------------


    // ---------------------------------- construct stable mesh begin ---------------------------------
    QAction * read_base_mesh = new QAction("read_base_mesh", cstr_stable_mesh);
    QAction * cstr_next = new QAction("cstr_next", cstr_stable_mesh);
    // ---------------------------------- construct stable mesh end ---------------------------------

    QAction * load_base_mesh = new QAction("load_base_mesh", reconstrcust);
    QAction * load_next = new QAction("next", reconstrcust);
    QAction * z_tree_1over2 = new QAction("zero_tree_1/2", reconstrcust);
    QAction * z_tree_2over3 = new QAction("zero_tree_2/3", reconstrcust);
    QAction * z_tree_3over4 = new QAction("zero_tree_3/4", reconstrcust);
    QAction * z_tree_4over5 = new QAction("zero_tree_4/5", reconstrcust);


    QAction * aboutQt = new QAction("About", helpMenu);

    //fileMenu->addAction(openFile);
    fileMenu->addAction(saveAsPly);
    fileMenu->addAction(exit);

    opsMMenu->addAction(openFileModel);

    opsHEMenu->addAction(openFileHE);
    opsHEMenu->addAction(subdivision_butterfly_he);
    opsHEMenu->addAction(subdivision_looprev_he);
    opsHEMenu->addAction(partition_he);
    opsHEMenu->addAction(reverse_subdivision_he);
    //opsHEMenu->addAction(construct_stable_mesh);

    cstr_stable_mesh->addAction(read_base_mesh);
    cstr_stable_mesh->addAction(cstr_next);

    reconstrcust->addAction(load_base_mesh);
    reconstrcust->addAction(load_next);
    reconstrcust->addAction(z_tree_1over2);
    reconstrcust->addAction(z_tree_2over3);
    reconstrcust->addAction(z_tree_3over4);
    reconstrcust->addAction(z_tree_4over5);

    helpMenu->addAction(aboutQt);

    QObject::connect(saveAsPly, SIGNAL(triggered(bool)), glwidget, SLOT(saveAaPLY()));
    QObject::connect(exit, SIGNAL(triggered(bool)), this, SLOT(close()));

    QObject::connect(aboutQt, SIGNAL(triggered(bool)), this, SLOT(about()));
    QObject::connect(openFileModel, SIGNAL(triggered(bool)), glwidget, SLOT(openFileModel()));

    QObject::connect(openFileHE, SIGNAL(triggered(bool)), glwidget, SLOT(openFileHE()));
    QObject::connect(subdivision_butterfly_he, SIGNAL(triggered(bool)), glwidget, SLOT(subdivide_butterfly_he()));
    QObject::connect(subdivision_looprev_he, SIGNAL(triggered(bool)), glwidget, SLOT(subdivide_loop_revision_he()));
    QObject::connect(partition_he, SIGNAL(triggered(bool)), glwidget, SLOT(partition_he()));
    QObject::connect(reverse_subdivision_he, SIGNAL(triggered(bool)), glwidget, SLOT(reversr_subdivide_he()));
    QObject::connect(read_base_mesh, SIGNAL(triggered(bool)), glwidget, SLOT(read_base_mesh()));
    QObject::connect(cstr_next, SIGNAL(triggered(bool)), glwidget, SLOT(cstr_next()));
    QObject::connect(load_base_mesh, SIGNAL(triggered(bool)), glwidget, SLOT(load_base_mesh()));
    QObject::connect(load_next, SIGNAL(triggered(bool)), glwidget, SLOT(load_next()));
    QObject::connect(z_tree_1over2, SIGNAL(triggered(bool)), glwidget, SLOT(z_tree_1over2()));
    QObject::connect(z_tree_2over3, SIGNAL(triggered(bool)), glwidget, SLOT(z_tree_2over3()));
    QObject::connect(z_tree_3over4, SIGNAL(triggered(bool)), glwidget, SLOT(z_tree_3over4()));
    QObject::connect(z_tree_4over5, SIGNAL(triggered(bool)), glwidget, SLOT(z_tree_4over5()));


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
                                    "                  @Author: jiayuan_xu\n");
}
