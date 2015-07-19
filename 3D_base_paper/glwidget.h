#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>


#include "model.h"
//#include "filedispose.h"
#include "halfedge.h"



class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget * parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

public slots:
    void setScaling(int scale);
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);


    // === save as ply
    void saveAaPLY();

    //void openFile();
    void openFileModel();
    void openFileHE();

    // --- subdivision
    // --- 1.butterfly
    void subdivide_butterfly_he();
    // --- 2.loop
    void subdivide_loop_revision_he();

    // --- reverse_subdivision
    void partition_he();
    void reversr_subdivide_he();

    // --- construct stable processing mesh
    void read_base_mesh();
    void cstr_next();

    // --- load stable processing mesh
    void load_base_mesh();
    void load_next();

    // --- zero tree compression
    void z_tree_1over2();
    void z_tree_2over3();
    void z_tree_3over4();
    void z_tree_4over5();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);

private:
    GLfloat m_fScale;
    int frames;

    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    QColor bgColor;

    int draw_flag;
    int times;
    bool write_pretreatment;

    int cstr_stb_mesh_times;

    Model * model;

//    QVector<Vertex *> vtx_list;
    HalfEdge * halfEdge;

//    FileDispose * file_dispose;
    QString file_name;

    void normalizeAngle(int & angle);
    void makeObjectModel();
    void makeObjectHE();

    GLfloat lightAmbient[4];
    GLfloat lightDiffuse[4];
    GLfloat lightPosition[4];
};

#endif // GLWIDGET_H
