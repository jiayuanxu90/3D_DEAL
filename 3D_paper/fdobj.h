#ifndef FDOBJ_H
#define FDOBJ_H

#include "filedispose.h"
#include <QTextStream>
//#include "model.h"
#include "halfedge.h"

class FDObj: public FileDispose
{


public:

    bool read_file(QString str, HalfEdge *&halfEdge, QVector<Vertex *> &vtx_list);
    // write file, save as .obj
    bool write_file(QString file_name, HalfEdge *&halfEdge, int level);


//    FDObj(QString str, Model * &model);
    FDObj(QString str, HalfEdge * &halfEdge, QVector<Vertex *> &vtx_list);
    ~FDObj();

private:
    void deal_with_v(QTextStream & ts, HalfEdge * &halfEdge, QVector<Vertex *> &vtx_list);
//    void deal_with_vt(QTextStream & ts, HalfEdge * &halfEdge);
//    void deal_with_vn(QTextStream & ts, HalfEdge * &halfEdge);
    void deal_with_f(QTextStream & ts);
    bool write_origin(QString path, HalfEdge * &halfEdge);
    bool write_error(QString path, HalfEdge * &halfEdge);
    bool pretreatment(QString path, QString name, QString suffix, HalfEdge * &halfEdge);
};

#endif // FDOBJ_H
