#include "objimpl.h"
#include <QFile>
#include <QTextStream>
#include "modelintf.h"
#include "model.h"

ObjImpl::ObjImpl(QString str, Model * &model) : ModelIntf()
{
    read_file(str, model);
}

ObjImpl::~ObjImpl()
{
}


bool ObjImpl::read_file(QString str, Model * &model)
{
    QFile file(str);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "*** Can not open the file! File location is \""
                 << str << "\" ***" << endl;
        return false;
    }
    else {
        model = new Model();
        QTextStream infile(&file);
        while (!infile.atEnd()) {
            QString line = infile.readLine();
            QTextStream sin(&line);
            QString s;
            sin >> s;
            if (s == "v") {
                deal_with_v(sin, model);
            }
            else if (s == "vt") {
                deal_with_vt(sin, model);
            }
            else if (s == "vn") {
                deal_with_vn(sin, model);
            }
            else if (s == "f") {
                deal_with_f(sin, model);
            }
        }

        model->set_poroperties();
        model->display_for_test();

        return true;
    }

}



void ObjImpl::deal_with_v(QTextStream &ts, Model * &model)
{
    GLfloat x = 0.0f,
            y = 0.0f,
            z = 0.0f;
    ts >> x >> y >> z;
    model->add_vertex(x, y, z);
}

void ObjImpl::deal_with_vt(QTextStream &ts, Model * &model)
{
    GLfloat x = 0.0f,
            y = 0.0f;
    ts >> x >> y;
    model->add_vertex_texture(x, y);
}

void ObjImpl::deal_with_vn(QTextStream &ts, Model * &model)
{
    GLfloat x = 0.0f,
            y = 0.0f,
            z = 0.0f;
    ts >> x >> y >> z;
    model->add_vertex_normal(x, y, z);

}

void ObjImpl::deal_with_f(QTextStream &ts, Model * &model)
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
