#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "objhelper.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget * parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

public slots:
    void setScaling(int scale);
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

    void openFile();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);

private:
    GLfloat m_fScale;
    int frames;

    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;

    ObjHelper * oh;

    void normalizeAngle(int & angle);
    void makeObject(QString filename);

};

#endif // GLWIDGET_H
