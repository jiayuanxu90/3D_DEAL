#ifndef FILEDISPOSE_H
#define FILEDISPOSE_H

//#include "model.h"
#include "halfedge.h"
#include <QVector>


class FileDispose
{


public:
    QVector<unsigned int> vertex_indices, vt_indices, normal_indices, vrgb_indices;

    //virtual bool read_file(QString str, Model * &model) = 0;
    virtual bool read_file(QString str, HalfEdge * &halfEdge, QVector<Vertex *> &vtx_list) = 0;

    FileDispose();
    ~FileDispose();

};

#endif // FILEDISPOSE_H
