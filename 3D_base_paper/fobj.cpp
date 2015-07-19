#include "fobj.h"
#include <QFile>
#include <QDir>
#include <QTextStream>

FObj::FObj(QString str, Model *&model)
{
    read_file(str, model);
}

//FObj::FObj(QString str, HalfEdge *&halfEdge, QVector<Vertex *> &vtx_list)
//{
//    vn_list.append(QVector3D(0.0f, 0.0f, 0.0f));
//    read_file(str, halfEdge, vtx_list);
//}

FObj::~FObj()
{
}

bool FObj::read_file(QString str, Model *&model)
{
    QFile file(str);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "*** Can not open the file! File location is \""
                 << str << "\" ***" << endl;
        return false;
    }
    else {
        QTextStream infile(&file);
        while (!infile.atEnd()) {
            QString line = infile.readLine();
            QTextStream sin(&line);
            QString s;
            sin >> s;
            if (s == "v") {
                deal_with_v_model(sin, model);
            }
            else if (s == "vt") {
                deal_with_vt_model(sin, model);
            }
            else if (s == "vn") {
                deal_with_vn_model(sin, model);
            }
            else if (s == "f") {
                deal_with_f_model(sin, model);
            }
        }
    }
    return true;
}

//bool FObj::read_file(QString str, HalfEdge * &halfEdge, QVector<Vertex *> &vtx_list)
//{
//    QFile file(str);
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        qDebug() << "*** Can not open the file! File location is \""
//                 << str << "\" ***" << endl;
//        return false;
//    }
//    else {
//        QTextStream infile(&file);
//        while (!infile.atEnd()) {
//            QString line = infile.readLine();
//            QTextStream sin(&line);
//            QString s;
//            sin >> s;
//            if (s == "v") {
//                deal_with_v_he(sin, halfEdge, vtx_list);
//            }
////            else if (s == "vt") {
////                // --- do with the vt context
////                //it is not metioned in the research
////                deal_with_vt_he(sin);
////            }
//            else if (s == "vn") {
//                // --- the same as the 'vt'
//                deal_with_vn_he(sin);
//            }
//            else if (s == "f") {
//                deal_with_f_he(sin);
//            }
//        }

//        return true;
//    }
//}


void FObj::deal_with_v_model(QTextStream &ts, Model *&model)
{
    GLfloat x = 0.0f,
            y = 0.0f,
            z = 0.0f;
    ts >> x >> y >> z;
    model->add_vertex(x, y, z);
}

void FObj::deal_with_vt_model(QTextStream &ts, Model *&model)
{
    GLfloat x = 0.0f,
            y = 0.0f;
    ts >> x >> y;
    model->add_vertex_texture(x, y);
}

void FObj::deal_with_vn_model(QTextStream &ts, Model *&model)
{
    GLfloat x = 0.0f,
            y = 0.0f,
            z = 0.0f;
    ts >> x >> y >> z;
    model->add_vertex_normal(x, y, z);
}

void FObj::deal_with_f_model(QTextStream &ts, Model * &model)
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
            model->add_v_index(a);
        }
        else {
            // There is a '/' in str or two
            // but, anyway, there have to be vertex indices
            // in the str, so put it into vertex index list
            unsigned int a = (str.mid(0, index)).toUInt();
            model->add_v_index(a);

            // Cut str form the position of ('/'+1)
            // to get a new str
            str = str.mid(index+1);
            index = str.indexOf('/');

            if (index == -1) {
                // There is no '/' in the new str
                // So, Case 2
                unsigned int b = str.toUInt();
                model->add_vt_index(b);
            }
            else if (index == 0) {
                // The new str is began with '/'
                // So, Case 4
                str = str.mid(index+1);
                unsigned int c = str.toUInt();
                model->add_n_index(c);
            }
            else {
                // Case 3:
                unsigned int b = (str.mid(0, index)).toUInt();
                model->add_vt_index(b);
                str = str.mid(index+1);
                unsigned int c = str.toUInt();
                model->add_n_index(c);
            }

        }
    }
}

//void FObj::deal_with_v_he(QTextStream &ts, HalfEdge* &halfEdge, QVector<Vertex *> &vtx_list)
//{
//    GLfloat x = 0.0f,
//            y = 0.0f,
//            z = 0.0f;
//    ts >> x >> y >> z;
//    Vertex * vtx_new = new Vertex(halfEdge->get_vertex_size()+1, x, y, z);
//    halfEdge->add_vertex(vtx_new);
//    vtx_list.push_back(vtx_new);
//}

////void FObj::deal_with_vt_he(QTextStream &ts)
////{

////}

//void FObj::deal_with_vn_he(QTextStream &ts)
//{
//    GLfloat x = 0.0f,
//            y = 0.0f,
//            z = 0.0f;
//    ts >> x >> y >> z;
//    vn_list.push_back(QVector3D(x, y, z));
//}

//void FObj::deal_with_f_he(QTextStream &ts)
//{
//    // part into 4 following cases
//    // 1. a
//    // 2. a/b
//    // 3. a/b/c
//    // 4. a//c
//    for (int i=0; i<3; ++i) {
//        QString str;
//        ts >> str;
//        int index = str.indexOf('/');

//        if (index == -1) {
//            // There is no '/' in str
//            // so, Case 1
//            unsigned int a = str.toUInt();
//            vertex_indices.push_back(a);
//        }
//        else {
//            // There is a '/' in str or two
//            // but, anyway, there have to be vertex indices
//            // in the str, so put it into vertex index list
//            unsigned int a = (str.mid(0, index)).toUInt();
//            vertex_indices.push_back(a);

//            // Cut str form the position of ('/'+1)
//            // to get a new str
//            str = str.mid(index+1);
//            index = str.indexOf('/');

//            if (index == -1) {
//                // There is no '/' in the new str
//                // So, Case 2
//                unsigned int b = str.toUInt();
//                vt_indices.push_back(b);
//            }
//            else if (index == 0) {
//                // The new str is began with '/'
//                // So, Case 4
//                str = str.mid(index+1);
//                unsigned int c = str.toUInt();
//                normal_indices.push_back(c);
//            }
//            else {
//                // Case 3:
//                unsigned int b = (str.mid(0, index)).toUInt();
//                vt_indices.push_back(b);
//                str = str.mid(index+1);
//                unsigned int c = str.toUInt();
//                normal_indices.push_back(c);
//            }

//        }
//    }
//}
