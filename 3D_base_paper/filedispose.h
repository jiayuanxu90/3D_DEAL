#ifndef FILEDISPOSE_H
#define FILEDISPOSE_H

#include <QVector>
#include <QVector3D>

#include "model.h"
//#include "halfedge.h"

class FileDispose
{
public:
    FileDispose();
    ~FileDispose();

    QVector<unsigned int> vertex_indices, vt_indices, normal_indices, vrgb_indices;
    QVector<QVector3D> vn_list;

    virtual bool read_file(QString str, Model * &model) = 0;
//    virtual bool read_file(QString str, HalfEdge * &halfEdge, QVector<Vertex *> &vtx_list) = 0;
};

#endif // FILEDISPOSE_H
