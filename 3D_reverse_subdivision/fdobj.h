#ifndef FDOBJ_H
#define FDOBJ_H

#include "filedispose.h"
#include <QTextStream>
//#include "model.h"
#include "halfedge.h"

class FDObj: public FileDispose
{


public:

    //bool read_file(QString str, Model * &model);
    bool read_file(QString str, HalfEdge *&halfEdge, QVector<Vertex *> &vtx_list);

//    FDObj(QString str, Model * &model);
    FDObj(QString str, HalfEdge * &halfEdge, QVector<Vertex *> &vtx_list);
    ~FDObj();

private:
    void deal_with_v(QTextStream & ts, HalfEdge * &halfEdge, QVector<Vertex *> &vtx_list);
//    void deal_with_vt(QTextStream & ts, HalfEdge * &halfEdge);
//    void deal_with_vn(QTextStream & ts, HalfEdge * &halfEdge);
    void deal_with_f(QTextStream & ts);
};

#endif // FDOBJ_H
