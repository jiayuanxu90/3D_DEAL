#include "objhelper.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QtOpenGL>

ObjHelper::ObjHelper(QString str)
{
    vertices.append(QVector3D(0, 0, 0));
    x_min = 0.0f;
    x_max = 0.0f;
    y_min = 0.0f;
    y_max = 0.0f;
    z_min = 0.0f;
    z_max = 0.0f;
    readFile(str);
}


ObjHelper::~ObjHelper()
{
    vertices.clear();
    faces.clear();
    vertices_tiangles.clear();
    normals.clear();
}

int ObjHelper::readFile(QString str)
{
    QFile file(str);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Can not open the file!" << endl;
        return 0;
    }
    else {
        QTextStream infile(&file);
        while (!infile.atEnd()) {
            QString line = infile.readLine();
            if (line.at(0) == 'v') {
                QVector3D qv3 = addVertex(line);
                if (qv3[0] < x_min)
                    x_min = qv3[0];
                if (qv3[0] > x_max)
                    x_max = qv3[0];
                if (qv3[1] < y_min)
                    y_min = qv3[1];
                if (qv3[1] > y_max)
                    y_max = qv3[1];
                if (qv3[2] < z_min)
                    z_min = qv3[2];
                if (qv3[2] > z_max)
                    z_max = qv3[2];

            }
            else if (line.at(0) == 'f') {
                struct Face face = addFace(line);
                addFaceVerteices(face);
            }
        }
        return 1;
    }
}


QVector3D ObjHelper::addVertex(QString line)
{
    QTextStream sin(&line);
    QString s;
    GLfloat x, y, z;
    sin >> s >> x >> y >> z;
    QVector3D m(x, y, z);
    vertices.append(m);
    return m;
}

struct Face ObjHelper::addFace(QString line)
{
    QTextStream sin(&line);
    QString s;
    int p1, p2, p3;
    sin >> s >> p1 >> p2 >> p3;
    struct Face face;
    face.point1 = p1;
    face.point2 = p2;
    face.point3 = p3;
    faces.append(face);
    return face;
}

void ObjHelper::addFaceVerteices(struct Face face)
{
    vertices_tiangles.append(vertices.at(face.point1));
    vertices_tiangles.append(vertices.at(face.point2));
    vertices_tiangles.append(vertices.at(face.point3));
}


















