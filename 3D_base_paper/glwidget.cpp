#include "glwidget.h"
//#include "fobj.h"
#include "halfedge.h"

#include <QtWidgets>
#include <QtOpenGL>

#include <gl/GLU.h>

#include <math.h>


GLWidget::GLWidget(QWidget * parent) :
    QGLWidget(parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;

    m_fScale = 1.0f;
    frames = 0;

    bgColor = QColor::fromCmykF(0.0, 0.0, 0.0, 0.0);

    setMinimumSize(400, 400);
    model = NULL;
    halfEdge = NULL;

    lightAmbient[0] = 0.5f;
    lightAmbient[1] = 0.5f;
    lightAmbient[2] = 0.5f;
    lightAmbient[3] = 1.0f;

    lightDiffuse[0] = 1.0f;
    lightDiffuse[1] = 1.0f;
    lightDiffuse[2] = 1.0f;
    lightDiffuse[3] = 1.0f;


    lightPosition[0] = 0.0f;
    lightPosition[1] = 0.0f;
    lightPosition[2] = 2.0f;
    lightPosition[3] = 1.0f;

    draw_flag = 0;
    times = 0;
    write_pretreatment = true;
    cstr_stb_mesh_times = 0;
}

GLWidget::~GLWidget()
{

}

void GLWidget::setScaling(int scale)
{
    if (scale > 50)
        m_fScale = 1.0f + (GLfloat(scale-50))/50*0.5;
    else if (scale < 50)
        m_fScale = 1.0f - (GLfloat(50-scale))/50*0.5;
    else
        m_fScale = 1;
    updateGL();
}

void GLWidget::normalizeAngle(int &angle)
{
    while (angle < 360) {
        angle += 360 * 16;
    }
    while (angle > 360 * 16) {
        angle -= 360 * 16;
    }
}

void GLWidget::setXRotation(int angle)
{
    normalizeAngle(angle);
    if (angle !=  xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
    normalizeAngle(angle);
    if (angle !=  yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    normalizeAngle(angle);
    if (angle !=  zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}


void GLWidget::openFileModel()
{
    QString fileName =
            QFileDialog::getOpenFileName(this,
                    tr("open file"), "C:/Users/Tree/Desktop/M",
                    tr("Objfile(*.obj);;AllFile(*.*)"));
    if (!fileName.endsWith(".obj")) {
        QMessageBox::about(this, "Message", "This format is not supported! Please try again...");
        return;
    }

    file_name = fileName;
    makeObjectModel();

    draw_flag = 0;
    times = 0;

    updateGL();

}


void GLWidget::saveAaPLY()
{
    if (halfEdge->is_empty()) {
        QMessageBox::about(this, "Message", "Please choose a mesh first...");
    }
    else {
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

    name.append("_stb_0");
    path.append(name);

    QString path_stb;
    path_stb += path + ".ply";

    halfEdge->save_as_ply(path_stb);
    }
}

void GLWidget::openFileHE()
{
    QString fileName =
            QFileDialog::getOpenFileName(this,
                    tr("open file"), "C:/Users/Tree/Desktop/M",
                    tr("Objfile(*.obj);;AllFile(*.*)"));
    if (!fileName.endsWith(".obj")) {
        QMessageBox::about(this, "Message", "This format is not supported! Please try again...");
        return;
    }

    file_name = fileName;
    makeObjectHE();

    draw_flag = 0;
    times = 0;
    write_pretreatment = true;

    updateGL();
}


void GLWidget::subdivide_butterfly_he()
{
    if (halfEdge->is_empty()) {
        QMessageBox::about(this, "Message", "Please choose a mesh first...");
    }
    else {
        halfEdge->subdivision_butterfly();
        halfEdge->set_poroperties();
        halfEdge->normalize_for_paint();
        times++;
        updateGL();
    }
}

void GLWidget::subdivide_loop_revision_he()
{
    if (halfEdge->is_empty()) {
        QMessageBox::about(this, "Message", "Please choose a mesh first...");
    }
    else {
        halfEdge->subdivision_loop_revision();
        halfEdge->set_poroperties();
        halfEdge->normalize_for_paint();
        times++;
        updateGL();
    }
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(640, 480);
}

void GLWidget::initializeGL()
{
    qglClearColor(bgColor);

    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_MULTISAMPLE);
    //static GLfloat lightPosition[4] = {0.5, 5.0, 7.0, 1.0};
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT1);
    //glEnable(GL_LIGHTING);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    if (!model->empty()) {
        model->draw(xRot, yRot, zRot, m_fScale);
    }
    if (!halfEdge->is_empty()) {
        halfEdge->draw(xRot, yRot, zRot, m_fScale, draw_flag);
    }
}

void GLWidget::resizeGL(int width, int height)
{
    //int side = qMin(width, height);
    //glViewport((width-side)/2, (height-side)/2, side, side);

    if (height == 0) {
        height = 1;
    }
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0);
    //glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::makeObjectModel()
{

    if (!halfEdge->is_empty()) {
        halfEdge->~HalfEdge();
        halfEdge = NULL;
    }

    if (!model->empty()) {
        model->~Model();
        model = NULL;
    }

    //file_name = filename;
    model = new Model();
    model->construct_form_file(file_name);
    //file_dispose = new FObj(file_name, model);
    model->set_poroperties();
    model->normalize_for_paint();
}

void GLWidget::makeObjectHE()
{

    if (!halfEdge->is_empty()) {
        halfEdge->~HalfEdge();
        halfEdge = NULL;
    }

    if (!model->empty()) {
        model->~Model();
        model = NULL;
    }

    halfEdge = new HalfEdge();
    halfEdge->construct_from_file(file_name);
    halfEdge->set_poroperties();
    halfEdge->normalize_for_paint();
//    file_dispose = new FObj(filename, halfEdge, vtx_list);
//    qDebug() << "--------------------------------";
//    for (int i=0; i<10; i++) {
//        qDebug() << file_dispose->vn_list.at(i).x() << " "
//                 << file_dispose->vn_list.at(i).y() << " "
//                 << file_dispose->vn_list.at(i).z();
//        qDebug() << file_dispose->normal_indices.at(i);

//    }
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->x() - lastPos.x();
    int dy = e->y() - lastPos.y();

    if (e->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (e->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = e->pos();
}

void GLWidget::partition_he()
{
    if (halfEdge->is_empty()) {
        QMessageBox::about(this, "Message", "Please choose a mesh first...");
        return;
    }
    if (times == 0) {
        QMessageBox::about(this, "Message", "Can not part any more...");
        return;
    }
    draw_flag = 1;
    halfEdge->partition();
    updateGL();
}

void GLWidget::reversr_subdivide_he()
{
    if (halfEdge->is_empty()) {
        QMessageBox::about(this, "Message", "Please choose a mesh first...");
        return;
    }
    if (times == 0) {
        QMessageBox::about(this, "Message", "Can not reverse subdivide any more...");
        return;
    }

    draw_flag = 0;

    if (write_pretreatment && halfEdge->write_pretreatment(file_name)) {
        write_pretreatment = false;
        halfEdge->write_file(file_name, times);
    }

    halfEdge->reverse_subdivision();
    halfEdge->set_poroperties();
    halfEdge->normalize_for_paint();
    times--;


    // write name_times.obj mesh
    halfEdge->write_file(file_name, times);

    updateGL();

}

void GLWidget::read_base_mesh()
{
    QString fileName =
            QFileDialog::getOpenFileName(this,
                    tr("choose a base mesh"), "C:/Users/Tree/Desktop/M",
                    tr("Objfile(*.obj);;AllFile(*.*)"));
    if (!fileName.endsWith(".obj")) {
        QMessageBox::about(this, "Message", "This format is not supported! Please try again...");
        return;
    }

    file_name = fileName;
    if (!halfEdge->is_empty()) {
        halfEdge->~HalfEdge();
        halfEdge = NULL;
    }

    cstr_stb_mesh_times = 0;
    halfEdge = new HalfEdge();
    halfEdge->construct_base_mesh(file_name);


    // ------------output-----------------------------
    HalfEdge h;
    h.construct_base_mesh(file_name);

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

    name.append("_stb_0");
    path.append(name);

    QString path_stb;
    path_stb += path + ".obj";

    h.redefine_vtx_index();
    h.write_stb_mesh(path_stb);
    h.~HalfEdge();
    // ------------output-----------------------------
    halfEdge->set_poroperties();
    halfEdge->normalize_for_paint();

    draw_flag = 0;

    updateGL();
}

void GLWidget::cstr_next()
{
    if (halfEdge->is_empty()) {
        QMessageBox::about(this, "Message", "No base mesh constructed...");
        return;
    }

    if (!halfEdge->construct_next_distance(file_name, cstr_stb_mesh_times)) {
        QMessageBox::about(this, "Message", "It is origin mesh, can not be subdivide again...");
        return;
    }

    cstr_stb_mesh_times++;

    halfEdge->set_poroperties();
    halfEdge->normalize_for_paint();

    updateGL();
}

void GLWidget::load_base_mesh()
{
    QString fileName =
            QFileDialog::getOpenFileName(this,
                    tr("choose a base mesh"), "C:/Users/Tree/Desktop/M",
                    tr("Objfile(*.obj);;AllFile(*.*)"));
    if (!fileName.endsWith(".obj")) {
        QMessageBox::about(this, "geBMessage", "This format is not supported! Please try again...");
        return;
    }

    file_name = fileName;
    if (!halfEdge->is_empty()) {
        halfEdge->~HalfEdge();
        halfEdge = NULL;
    }

    cstr_stb_mesh_times = 0;
    halfEdge = new HalfEdge();
    halfEdge->construct_from_file(file_name);
    halfEdge->set_poroperties();
    halfEdge->normalize_for_paint();

    updateGL();
}


void GLWidget::load_next()
{
    if (halfEdge->is_empty()) {
        QMessageBox::about(this, "Message", "No base mesh constructed...");
        return;
    }

    if (!halfEdge->load_next(file_name, cstr_stb_mesh_times)) {
        QMessageBox::about(this, "Message", "It is origin mesh, can not be subdivide again...");
        return;
    }

    cstr_stb_mesh_times++;

    halfEdge->set_poroperties();
    halfEdge->normalize_for_paint();

    updateGL();

}

void GLWidget::z_tree_1over2()
{
    if (halfEdge->is_empty()) {
        QMessageBox::about(this, "Message", "No base mesh chosen...");
        return;
    }

    if (!halfEdge->zero_tree_compress(file_name, 0.5)) {
        return;
    }

    halfEdge->set_poroperties();
    halfEdge->normalize_for_paint();

    updateGL();

}

void GLWidget::z_tree_2over3()
{
    if (halfEdge->is_empty()) {
        QMessageBox::about(this, "Message", "No base mesh chosen...");
        return;
    }

    if (!halfEdge->zero_tree_compress(file_name, 0.66)) {
        return;
    }

    halfEdge->set_poroperties();
    halfEdge->normalize_for_paint();

    updateGL();

}

void GLWidget::z_tree_3over4()
{
    if (halfEdge->is_empty()) {
        QMessageBox::about(this, "Message", "No base mesh chosen...");
        return;
    }

    if (!halfEdge->zero_tree_compress(file_name, 0.875)) {
        return;
    }

    halfEdge->set_poroperties();
    halfEdge->normalize_for_paint();

    updateGL();
}

void GLWidget::z_tree_4over5()
{
    if (halfEdge->is_empty()) {
        QMessageBox::about(this, "Message", "No base mesh chosen...");
        return;
    }

    if (!halfEdge->zero_tree_compress(file_name, 0.9375)) {
        return;
    }

    halfEdge->set_poroperties();
    halfEdge->normalize_for_paint();

    updateGL();
}
