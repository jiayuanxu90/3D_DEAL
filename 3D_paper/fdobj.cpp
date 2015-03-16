#include "fdobj.h"
#include <QFile>
#include <QDir>
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


bool FDObj::write_file(QString file_name, HalfEdge *&halfEdge, int level)
{
    int a = file_name.lastIndexOf('/');
    QString path = file_name.mid(0, a+1);
    QString name = file_name.mid(a+1);
    int b = name.indexOf('_');
    QString suffix;
    if (b == -1) {
        b = name.indexOf(".");
        suffix = name.mid(b+1);
        name = name.mid(0, b);
    }
    else {
        int c = name.indexOf(".");
        suffix = name.mid(c+1);
        name = name.mid(0, b);
    }

    qDebug() << "path: " << path;
    qDebug() << "name: " << name;
    qDebug() << "suffix: " << suffix;
    //name.append(QString::number(level));
    //path = str + name;
    //path.append(".obj");

    if (level == -1) {
        //name.append("_");
        name.append(QString::number(00));
        path.append(name);
        return (write_origin(path.append(".obj"), halfEdge));
    }

    if (level == 0) {
        qDebug() << "ppppppppppppppppppppppppppppppppp";
        name.append("_");
        name.append(QString::number(level));
        path.append(name);
        qDebug() << path;
        QString path_origin;
        path_origin += path + ".obj";
        return (write_origin(path_origin, halfEdge));
    }

    if (level == 1) {
        if (!pretreatment(path, name, suffix, halfEdge))
            return false;
        else {
            name.append("_");
            name.append(QString::number(level));
            path.append(name);

            QString path_origin;
            path_origin += path + ".obj";
            QString path_error;
            path_error += path + ".err";
            if (write_origin(path_origin, halfEdge))
                return (write_error(path_error, halfEdge));
            else
                return false;

        }
    }


    name.append("_");
    name.append(QString::number(level));
    path.append(name);

    QString path_origin;
    path_origin += path + ".obj";
    QString path_error;
    path_error += path + ".err";
    if (write_origin(path_origin, halfEdge))
        return (write_error(path_error, halfEdge));
    else
        return false;

}

bool FDObj::write_origin(QString path, HalfEdge *&halfEdge)
{
    QFile file(path);
    if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
        QTextStream out(&file);
        Vertex * vtx = halfEdge->get_vertex_first();
        while (vtx != halfEdge->get_vertex_front()) {
            out << "v " << QString::number(vtx->coordinate.x()) << " "
                << QString::number(vtx->coordinate.y()) << " " << QString::number(vtx->coordinate.z()) << endl;
            vtx = vtx->next;
        }
        Face * face = halfEdge->get_face_first();
        while (face != halfEdge->get_face_front()) {
            out << "f " << face->e1->vertex1->v_index << " " << face->e2->vertex1->v_index
                << " " << face->e3->vertex1->v_index << endl;
            face = face->next;
        }

        file.close();
        return true;
    }
    else {
        qDebug() << "File open error...";
        return false;
    }

}

bool FDObj::write_error(QString path, HalfEdge *&halfEdge)
{
    QFile file(path);
    if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
        QTextStream out(&file);
        Face * face = halfEdge->get_face_first();
        while (face != halfEdge->get_face_front()) {
            out << face->e1->vertex1->v_index << " " << face->e1->vertex2->v_index << " "
                << face->e1->error.x() << " " << face->e1->error.y() << " " << face->e1->error.z() << endl;
            out << face->e2->vertex1->v_index << " " << face->e2->vertex2->v_index << " "
                << face->e2->error.x() << " " << face->e2->error.y() << " " << face->e2->error.z() << endl;
            out << face->e1->vertex1->v_index << " " << face->e1->vertex2->v_index << " "
                << face->e1->error.x() << " " << face->e1->error.y() << " " << face->e1->error.z() << endl;
            face = face->next;
        }
        file.close();
    }
    else {
        qDebug() << "File open error...";
        return false;
    }
    return true;
}

bool FDObj::pretreatment(QString path, QString name, QString suffix, HalfEdge *&halfEdge)
{
    QDir dir(path);
    if (!dir.exists()) {
        qDebug() << "path not exsits...";
        return false;
    }

    QString name_filter;
    name_filter += name + "_*.*";
    QStringList filters;
    filters << QString(name_filter);

    QDirIterator dir_iterator(path, filters,
                              QDir::Files | QDir::NoSymLinks,
                              QDirIterator::Subdirectories);
    QStringList string_list;
    while (dir_iterator.hasNext()) {
        dir_iterator.next();
        QFileInfo file_info = dir_iterator.fileInfo();
        QString absolute_file_path = file_info.absoluteFilePath();
        string_list.append(absolute_file_path);
    }

    // --- _0 not exist
    QString _path0;
    _path0 += path + name;
    _path0 += "_0.obj";
    qDebug() << _path0;
    QFile _file0(_path0);
    if (!_file0.open(QIODevice::ReadOnly | QIODevice::Text)) {
        write_file(_path0, halfEdge, 0);
    }
    else {
        _file0.close();
    }

    // --- >_1 delete
    for (int i=0; i<string_list.size(); ++i) {
        if (string_list.at(i).compare(_path0) != 0) {
            qDebug() << string_list.at(i);
            QFile f(string_list.at(i));
            f.remove();
        }
    }

    return true;
}

