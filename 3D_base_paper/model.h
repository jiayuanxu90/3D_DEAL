#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include <QtOpenGL>

//#include"fobj.h"

class Model
{
public:

    GLfloat x_trans;
    GLfloat y_trans;
    GLfloat z_trans;
    GLfloat range;

    QVector<QVector3D> vertices;
    QVector<QVector2D> vertices_texture;
    QVector<QVector3D> vertices_normals;
    QVector<QVector3D> vertices_rgbs;

    QVector<QVector3D> v_array;
//    QVector<QVector2D> vt_array;
    QVector<QVector3D> vn_array;

    QVector<int> face_color_list;

    QVector<unsigned int> vertex_indices, vt_indices, normal_indices, vrgb_indices;



    Model();
//    Model(QString file_mame);
    ~Model();

    bool empty() const;
    int node_size() const;

    QVector<QVector3D> list;

    QVector3D add_vertex(GLfloat x, GLfloat y, GLfloat z);
    QVector2D add_vertex_texture(GLfloat x, GLfloat y);
    QVector3D add_vertex_normal(GLfloat x, GLfloat y, GLfloat z);
    QVector3D add_vertex_rgb(GLint x, GLint y, GLint z);

    void construct_form_file(QString file_name);

    void set_poroperties();
    void normalize_for_paint();

    void add_v_index(unsigned int vi);
    void add_vt_index(unsigned int vti);
    void add_n_index(unsigned int ni);
    void add_vrgb_index(unsigned int rgbi);

    void display_for_test() const;
    void draw(int xRot, int yRot, int zRot, GLfloat m_fScale) const;

private:
    void deal_with_v_model(QTextStream &ts);
    void deal_with_vt_model(QTextStream &ts);
    void deal_with_vn_model(QTextStream &ts);
    void deal_with_f_model(QTextStream &ts);
};

#endif // MODEL_H
