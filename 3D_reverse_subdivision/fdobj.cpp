#include "fdobj.h"
#include <QFile>
#include <QTextStream>
#include "filedispose.h"
#include "model.h"

FDObj::FDObj(QString str, HalfEdge *&halfEdge, QVector<Vertex *> &vtx_list) : FileDispose()
{
    read_file(str, halfEdge, vtx_list);
}

FDObj::~FDObj()
{
}


bool FDObj::read_file(QString str, HalfEdge * &halfEdge, QVector<Vertex *> &vtx_list)
{
    QFile file(str);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "*** Can not open the file! File location is \""
                 << str << "\" ***" << endl;
        return false;
    }
    else {
        //model = new Model();
        QTextStream infile(&file);
        while (!infile.atEnd()) {
            QString line = infile.readLine();
            QTextStream sin(&line);
            QString s;
            sin >> s;
            if (s == "v") {
                deal_with_v(sin, halfEdge, vtx_list);
            }
//            else if (s == "vt") {
                // --- do with the vt context
                // it is not metioned in the research
                //deal_with_vt(sin, halfEdge);
//            }
//            else if (s == "vn") {
                  // --- the same as the 'vt'
//                deal_with_vn(sin, halfEdge);
//            }
            else if (s == "f") {
                deal_with_f(sin);
            }
        }

        //qDebug() << "load file :" << vertex_indices.size();

//        halfEdge->set_poroperties();
//        halfEdge->normalize_for_paint();

//        qDebug() << "load file: ";
//        qDebug() << "vtx_size" << vtx_list.size();
//        qDebug() << "half_edge vtx size: " << halfEdge->get_vertex_size();
//        Vertex * vtx = halfEdge->get_vertex_first();
//        for (int i=0; i<10; i++) {
//            qDebug() << vtx->v_index;
//            vtx = vtx->next;
//        }

//        halfEdge->construct_halfedge_sturcture(vertex_indices, vtx_list);

        return true;
    }

}



void FDObj::deal_with_v(QTextStream &ts, HalfEdge* &halfEdge, QVector<Vertex *> &vtx_list)
{
    GLfloat x = 0.0f,
            y = 0.0f,
            z = 0.0f;
    ts >> x >> y >> z;
    Vertex * vtx_new = new Vertex(halfEdge->get_vertex_size()+1, x, y, z);
    halfEdge->add_vertex(vtx_new);
    vtx_list.push_back(vtx_new);

}

//void FDObj::deal_with_vt(QTextStream &ts, HalfEdge *&halfEdge)
//{
//    GLfloat x = 0.0f,
//            y = 0.0f;
//    ts >> x >> y;
//    halfEdge->add_vertex_texture(x, y);
//}

//void FDObj::deal_with_vn(QTextStream &ts, HalfEdge *&halfEdge)
//{
//    GLfloat x = 0.0f,
//            y = 0.0f,
//            z = 0.0f;
//    ts >> x >> y >> z;
//    halfEdge->add_vertex_normal(x, y, z);

//}

void FDObj::deal_with_f(QTextStream &ts)
{
    // part into 4 following cases
    // 1. a
    // 2. a/b
    // 3. a/b/c
    // 4. a//c
    for (int i=0; i<3; ++i) {
        QString str;
        ts >> str;
        int index = str.indexOf('/');

        if (index == -1) {
            // There is no '/' in str
            // so, Case 1
            unsigned int a = str.toUInt();
            vertex_indices.push_back(a);
        }
        else {
            // There is a '/' in str or two
            // but, anyway, there have to be vertex indices
            // in the str, so put it into vertex index list
            unsigned int a = (str.mid(0, index)).toUInt();
            vertex_indices.push_back(a);

            // Cut str form the position of ('/'+1)
            // to get a new str
            str = str.mid(index+1);
            index = str.indexOf('/');

            if (index == -1) {
                // There is no '/' in the new str
                // So, Case 2
                unsigned int b = str.toUInt();
                vt_indices.push_back(b);
            }
            else if (index == 0) {
                // The new str is began with '/'
                // So, Case 4
                str = str.mid(index+1);
                unsigned int c = str.toUInt();
                normal_indices.push_back(c);
            }
            else {
                // Case 3:
                unsigned int b = (str.mid(0, index)).toUInt();
                vt_indices.push_back(b);
                str = str.mid(index+1);
                unsigned int c = str.toUInt();
                normal_indices.push_back(c);
            }

        }
    }
}
