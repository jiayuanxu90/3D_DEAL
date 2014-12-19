#ifndef OBJHELPER_H
#define OBJHELPER_H

#include <QVector>
#include <QVector3D>
#include <QtOpenGL>

struct Face {
    int point1;
    int point2;
    int point3;
};



class ObjHelper
{

private:
    int readFile(QString str);

    QVector3D addVertex(QString line);
    struct Face addFace(QString line);
    void addFaceVerteices(struct Face face);

public:

    GLfloat x_min;
    GLfloat y_min;
    GLfloat z_min;
    GLfloat x_max;
    GLfloat y_max;
    GLfloat z_max;


    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<Face> faces;

    QVector<QVector3D> vertices_tiangles;

    ObjHelper(QString str);
    ~ObjHelper();
};

#endif // OBJHELPER_H
