#include "glwidget.h"
#include "objhelper.h"

#include <QtWidgets>
#include <QtOpenGL>

#include <gl/GLU.h>

#include <math.h>

#include <iostream>
using namespace std;

GLWidget::GLWidget(QWidget * parent) :
    QGLWidget(parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;

    m_fScale = 1.0f;
    frames = 0;

    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

    setMinimumSize(400, 400);
    oh = NULL;
    //makeObject("C:/Users/Tree/Desktop/models_oo/bunny.obj");
}

GLWidget::~GLWidget()
{

}

void GLWidget::setScaling(int scale)
{
    if (scale > 50)
        m_fScale = 1.0f + (GLfloat(scale-50))/50*1.0;
    else if (scale < 50)
        m_fScale = 1.0f - (GLfloat(50-scale))/50*1.0;
    else
        m_fScale = 1;
    updateGL();
}

void GLWidget::normalizeAngle(int &angle)
{
    while (angle < 360) {
        angle += 360 * 16;
    }
    while (angle > 360 * 16) {
        angle -= 360 * 16;
    }
}

void GLWidget::setXRotation(int angle)
{
    normalizeAngle(angle);
    if (angle !=  xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
    normalizeAngle(angle);
    if (angle !=  yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    normalizeAngle(angle);
    if (angle !=  zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::openFile()
{
    //QTextStream qsout(stdout, QIODevice::WriteOnly);
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), "../../models/", tr("Allfile(*.*);;objfile(*.obj)"));
    makeObject(fileName);

}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(640, 480);
}

void GLWidget::initializeGL()
{
    qglClearColor(qtPurple.dark());

    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_MULTISAMPLE);
    //static GLfloat lightPosition[4] = {0.5, 5.0, 7.0, 1.0};
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -1.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    glScalef(m_fScale, m_fScale, m_fScale);

    if (oh!=NULL && !oh->faces.empty()) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_TRIANGLES);
        for (int i=0; i<oh->faces.size(); ++i) {
            glVertex3f(oh->vertices.at(oh->faces.at(i).point1)[0],
                    oh->vertices.at(oh->faces.at(i).point1)[1],
                    oh->vertices.at(oh->faces.at(i).point1)[2]);
            glVertex3f(oh->vertices.at(oh->faces.at(i).point2)[0],
                    oh->vertices.at(oh->faces.at(i).point2)[1],
                    oh->vertices.at(oh->faces.at(i).point2)[2]);
            glVertex3f(oh->vertices.at(oh->faces.at(i).point3)[0],
                    oh->vertices.at(oh->faces.at(i).point3)[1],
                    oh->vertices.at(oh->faces.at(i).point3)[2]);
        }
        glEnd();
    }
}

void GLWidget::resizeGL(int width, int height)
{
    //int side = qMin(width, height);
    //glViewport((width-side)/2, (height-side)/2, side, side);

    if (height == 0) {
        height = 1;
    }
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0);
    //glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::makeObject(QString filename)
{
    if (oh != NULL) {
        oh->~ObjHelper();
        oh = NULL;
    }
    oh = new ObjHelper(filename);
}


void GLWidget::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->x() - lastPos.x();
    int dy = e->y() - lastPos.y();

    if (e->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (e->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = e->pos();
}
