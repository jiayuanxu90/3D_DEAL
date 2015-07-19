#ifndef FOBJ_H
#define FOBJ_H

#include "filedispose.h"
#include <QTextStream>

#include "model.h"
//#include "halfedge.h"

class FObj : public FileDispose
{
public:
    bool read_file(QString str, Model * &model);
//    bool read_file(QString str, HalfEdge *&halfEdge, QVector<Vertex *> &vtx_list);

    FObj(QString str, Model * &model);
//    FObj(QString str, HalfEdge * &halfEdge, QVector<Vertex *> &vtx_list);
    ~FObj();

private:
    // read file
    void deal_with_v_model(QTextStream &ts, Model * &model);
    void deal_with_vt_model(QTextStream &ts, Model * &model);
    void deal_with_vn_model(QTextStream &ts, Model * &model);
    void deal_with_f_model(QTextStream &ts, Model * &model);


//    void deal_with_v_he(QTextStream &ts, HalfEdge* &halfEdge, QVector<Vertex *> &vtx_list);
//    void deal_with_vt_he(QTextStream &ts);
//    void deal_with_vn_he(QTextStream &ts);
//    void deal_with_f_he(QTextStream &ts);

    // write file

};

#endif // FOBJ_H
