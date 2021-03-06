#include "model.h"
#include <QDebug>
#include <QtOpenGL>
#include <QTime>

//#include "fobj.h"
#include <QFile>
#include <QTextStream>

Model::Model()
{
    vertices.append(QVector3D(0, 0, 0));
    vertices_normals.append(QVector3D(0, 0, 0));
    vertices_texture.append(QVector2D(0, 0));
    vertices_rgbs.append(QVector3D(0, 0, 0));
    range = 0.0f;
    x_trans = 0.0f;
    y_trans = 0.0f;
    z_trans = 0.0f;

    list.push_back(QVector3D(1.0f, 0.0f, 0.0f));
    list.push_back(QVector3D(0.0f, 1.0f, 0.0f));
    list.push_back(QVector3D(0.0f, 0.0f, 1.0f));
    list.push_back(QVector3D(1.0f, 0.5f, 0.0f));
    list.push_back(QVector3D(0.9f, 0.0f, 0.4f));
    list.push_back(QVector3D(0.0f, 0.8f, 0.8f));
    list.push_back(QVector3D(1.0f, 1.0f, 1.0f));

}

//Model::Model()
//{
//    vertices.append(QVector3D(0, 0, 0));
//    vertices_normals.append(QVector3D(0, 0, 0));
//    vertices_texture.append(QVector2D(0, 0));
//    vertices_rgbs.append(QVector3D(0, 0, 0));
//    range = 0.0f;
//    x_trans = 0.0f;
//    y_trans = 0.0f;
//    z_trans = 0.0f;

//    list.push_back(QVector3D(1.0f, 0.0f, 0.0f));
//    list.push_back(QVector3D(0.0f, 1.0f, 0.0f));
//    list.push_back(QVector3D(0.0f, 0.0f, 1.0f));
//    list.push_back(QVector3D(1.0f, 0.5f, 0.0f));
//    list.push_back(QVector3D(0.9f, 0.0f, 0.4f));
//    list.push_back(QVector3D(0.0f, 0.8f, 0.8f));
//    list.push_back(QVector3D(1.0f, 1.0f, 1.0f));

////    file_dispose = new FObj(file_mame, &this);
////    file_dispose->~FObj();
//}

Model::~Model()
{
    vertices.clear();
    vertices_texture.clear();
    vertices_normals.clear();
    vertex_indices.clear();
    vt_indices.clear();
    normal_indices.clear();
    vrgb_indices.clear();
    list.clear();
}

bool Model::empty() const
{
    if (this==NULL || vertices.size()==1) {
        return true;
    }
    return false;
}

void Model::construct_form_file(QString file_name)
{
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "*** Can not open the file! File location is \""
                 << file_name << "\" ***" << endl;
        return;
    }
    else {
        QTextStream infile(&file);
        while (!infile.atEnd()) {
            QString line = infile.readLine();
            QTextStream sin(&line);
            QString s;
            sin >> s;
            if (s == "v") {
                deal_with_v_model(sin);
            }
            else if (s == "vt") {
                deal_with_vt_model(sin);
            }
            else if (s == "vn") {
                deal_with_vn_model(sin);
            }
            else if (s == "f") {
                deal_with_f_model(sin);
            }
        }
    }
}

int Model::node_size() const
{
    if (empty())
        return 0;
    else
        return vertex_indices.size();
}

QVector3D Model::add_vertex(GLfloat x, GLfloat y, GLfloat z)
{
    vertices.push_back(QVector3D(x, y, z));
    return vertices.at(vertices.size()-1);
}

QVector2D Model::add_vertex_texture(GLfloat x, GLfloat y)
{
    vertices_texture.push_back(QVector2D(x, y));
    return vertices_texture.at(vertices_texture.size()-1);
}

QVector3D Model::add_vertex_normal(GLfloat x, GLfloat y, GLfloat z)
{
    vertices_normals.push_back(QVector3D(x, y, z));
    return vertices_normals.at(vertices_normals.size()-1);
}

QVector3D Model::add_vertex_rgb(GLint x, GLint y, GLint z)
{
    vertices_rgbs.push_back(QVector3D(x, y, z));
    return vertices_rgbs.at(vertices_rgbs.size()-1);
}


void Model::add_v_index(unsigned int vi)
{
    vertex_indices.push_back(vi);
}

void Model::add_vt_index(unsigned int vti)
{
    vt_indices.push_back(vti);
}

void Model::add_n_index(unsigned int ni)
{
    normal_indices.push_back(ni);
}

void Model::add_vrgb_index(unsigned int rgbi)
{
    vrgb_indices.push_back(rgbi);
}

void Model::set_poroperties()
{
    // vertex
    GLfloat x_min = 0.0f;
    GLfloat y_min = 0.0f;
    GLfloat z_min = 0.0f;
    GLfloat x_max = 0.0f;
    GLfloat y_max = 0.0f;
    GLfloat z_max = 0.0f;
    for (int i=0; i<vertices.size(); ++i) {
        if (x_max < vertices.at(i).x())
            x_max = vertices.at(i).x();
        if (x_min > vertices.at(i).x())
            x_min = vertices.at(i).x();
        if (y_max < vertices.at(i).y())
            y_max = vertices.at(i).y();
        if (y_min > vertices.at(i).y())
            y_min = vertices.at(i).y();
        if (z_max < vertices.at(i).z())
            z_max = vertices.at(i).z();
        if (z_min > vertices.at(i).z())
            z_min = vertices.at(i).z();
    }
    GLfloat max = x_max;
    if (max < y_max)
        max = y_max;
    if (max < z_max)
        max = z_max;
    GLfloat min = x_min;
    if (min > y_min)
        min = y_min;
    if (min > z_min)
        min = z_min;

    range = max - min;

    x_trans = (x_min + x_max) / 2;
    y_trans = (y_min + y_max) / 2;
    z_trans = (z_min + z_max) / 2;

    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    for (int i=0; i<node_size()/3; ++i) {
        int random = qrand() % 7;
        face_color_list.push_back(random);
    }
}

void Model::normalize_for_paint()
{
    for (int i=0; i<vertices.size(); ++i) {
        v_array.push_back(QVector3D(vertices.at(i).x()/range,
                                    vertices.at(i).y()/range,
                                    vertices.at(i).z()/range));
    }
    for (int i=0; i<vertices_normals.size(); ++i) {
        vn_array.push_back(QVector3D(vertices_normals.at(i).x()/range,
                                     vertices_normals.at(i).y()/range,
                                     vertices_normals.at(i).z()/range));
    }
}

void Model::display_for_test() const
{
    qDebug() << "v: --------------" << endl;
    for (int i=0; i<vertices.size() && i<10; ++i) {
        qDebug() << vertices.at(i).x() << " " << vertices.at(i).y() << " "
                 << vertices.at(i).z() << endl;
    }
    qDebug() << "v_array: --------------" << endl;
    for (int i=0; i<v_array.size() && i<10; ++i) {
        qDebug() << v_array.at(i).x() << " " << v_array.at(i).y() << " "
                 << v_array.at(i).z() << endl;
    }

    qDebug() << "vt: --------------" << endl;
    for (int i=0; i<vertices_texture.size() && i<10; ++i) {
        qDebug() << vertices_texture.at(i).x() << " " <<
                    vertices_texture.at(i).y() << endl;
    }

    qDebug() << "vn: -----------------" << endl;
    for (int i=0; i<vertices_normals.size() && i<10; ++i) {
        qDebug() << vertices_normals.at(i).x() << " "
                 << vertices_normals.at(i).y() << " "
                 << vertices_normals.at(i).z() << endl;
    }
    qDebug() << "vn_array: -----------------" << endl;
    for (int i=0; i<vn_array.size() && i<10; ++i) {
        qDebug() << vn_array.at(i).x() << " "
                 << vn_array.at(i).y() << " "
                 << vn_array.at(i).z() << endl;
    }

    qDebug() << "v_rgb: -----------------" << endl;
    for (int i=0; i<vertices_rgbs.size() && i<10; ++i) {
        qDebug() << vertices_rgbs.at(i).x() << " "
                 << vertices_rgbs.at(i).y() << " "
                 << vertices_rgbs.at(i).z() << endl;
    }

    qDebug() << "index of v: --------------------" << endl;
    for (int i=0; i<vertex_indices.size() && i<30;) {
        qDebug() << vertex_indices.at(i) << " "
                 << vertex_indices.at(i+1) << " "
                 << vertex_indices.at(i+2) << endl;
        i += 3;
    }

    qDebug() << "index of uv: -------------------" << endl;
    for (int i=0; i<vt_indices.size() && i<30;) {
        qDebug() << vt_indices.at(i) << " "
                 << vt_indices.at(i+1) << " "
                 << vt_indices.at(i+2) << endl;
        i += 3;
    }

    qDebug() << "index of vn: ------------------" << endl;
    for (int i=0; i<normal_indices.size() && i<30;) {
        qDebug() << normal_indices.at(i) << " "
                 << normal_indices.at(i+1) << " "
                 << normal_indices.at(1+2) << endl;
        i += 3;
    }

    qDebug() << "index of v_rgb: ------------------" << endl;
    for (int i=0; i<vrgb_indices.size() && i<30;) {
        qDebug() << vrgb_indices.at(i) << " "
                 << vrgb_indices.at(i+1) << " "
                 << vrgb_indices.at(1+2) << endl;
        i += 3;
    }

    qDebug() << "properties: ---------------------------" << endl;
    qDebug() << "x_trans: " << x_trans << " | "
             << "y_trans: " << y_trans << " | "
             << "z_trans: " << z_trans << endl;
    qDebug() << "range: " << range << endl;
}

void Model::draw(int xRot, int yRot, int zRot, GLfloat m_fScale) const
{
    if (empty()) {
        glTranslatef(0.0f, 0.0f, -1.0f);
    } else {
        glTranslatef(-x_trans/range, -y_trans/range, -z_trans/range-1.0);
    }

    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    glScalef(m_fScale, m_fScale, m_fScale);

    if (!empty()) {


        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glColor4f(1.0f, 0.0f, 1.0f, 0.0f);

        int j = 0;
        for (int i=0; i<node_size(); i=i+3) {
            //qDebug() << i << " " << rand_x << " " << rand_y << " " << rand_z;
            glColor4f(list.at(face_color_list.at(j)).x(),
                      list.at(face_color_list.at(j)).y(),
                      list.at(face_color_list.at(j)).z(), 0.0f);
            j++;

            glBegin(GL_TRIANGLES);

            for (int k=0; k<3; ++k) {
                if (!normal_indices.empty() && normal_indices.size()!=0) {
                    glNormal3f(vn_array.at(normal_indices.at(i+k)).x(),
                           vn_array.at(normal_indices.at(i+k)).y(),
                           vn_array.at(normal_indices.at(i+k)).z());
                }

                glVertex3f(v_array.at(vertex_indices.at(i+k)).x(),
                       v_array.at(vertex_indices.at(i+k)).y(),
                       v_array.at(vertex_indices.at(i+k)).z());
            }

            glEnd();
        }


        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor4f(0.6f, 0.6f, 0.6f, 0.0f);

        for (int i=0; i<node_size(); i+=3) {
            glBegin(GL_LINE_LOOP);
            glVertex3f(v_array.at(vertex_indices.at(i)).x(),
                       v_array.at(vertex_indices.at(i)).y(),
                       v_array.at(vertex_indices.at(i)).z());
            glVertex3f(v_array.at(vertex_indices.at(i+1)).x(),
                       v_array.at(vertex_indices.at(i+1)).y(),
                       v_array.at(vertex_indices.at(i+1)).z());
            glVertex3f(v_array.at(vertex_indices.at(i+2)).x(),
                       v_array.at(vertex_indices.at(i+2)).y(),
                       v_array.at(vertex_indices.at(i+2)).z());
            glEnd();
        }

    }

}

void Model::deal_with_v_model(QTextStream &ts)
{
    GLfloat x = 0.0f,
            y = 0.0f,
            z = 0.0f;
    ts >> x >> y >> z;
    add_vertex(x, y, z);
}

void Model::deal_with_vt_model(QTextStream &ts)
{
    GLfloat x = 0.0f,
            y = 0.0f;
    ts >> x >> y;
    add_vertex_texture(x, y);
}

void Model::deal_with_vn_model(QTextStream &ts)
{
    GLfloat x = 0.0f,
            y = 0.0f,
            z = 0.0f;
    ts >> x >> y >> z;
    add_vertex_normal(x, y, z);
}

void Model::deal_with_f_model(QTextStream &ts)
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
            add_v_index(a);
        }
        else {
            // There is a '/' in str or two
            // but, anyway, there have to be vertex indices
            // in the str, so put it into vertex index list
            unsigned int a = (str.mid(0, index)).toUInt();
            add_v_index(a);

            // Cut str form the position of ('/'+1)
            // to get a new str
            str = str.mid(index+1);
            index = str.indexOf('/');

            if (index == -1) {
                // There is no '/' in the new str
                // So, Case 2
                unsigned int b = str.toUInt();
                add_vt_index(b);
            }
            else if (index == 0) {
                // The new str is began with '/'
                // So, Case 4
                str = str.mid(index+1);
                unsigned int c = str.toUInt();
                add_n_index(c);
            }
            else {
                // Case 3:
                unsigned int b = (str.mid(0, index)).toUInt();
                add_vt_index(b);
                str = str.mid(index+1);
                unsigned int c = str.toUInt();
                add_n_index(c);
            }

        }
    }
}
