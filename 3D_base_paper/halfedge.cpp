#include "halfedge.h"

#include <QVector2D>
#include <QQueue>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QHash>
#include <QtGlobal>



HalfEdge::HalfEdge()
{
    face_front = new Face();
    face_front->next = face_front;
    face_front->prev = face_front;
    face_size = 0;

    vtx_front = new Vertex();
    vtx_front->next = vtx_front;
    vtx_front->prev = vtx_front;
    vtx_size = 0;

    range = 0.0f;
    x_trans = 0.0f;
    y_trans = 0.0f;
    z_trans = 0.0f;

    rs_times = 0;

    list.push_back(QVector3D(1.0f, 0.0f, 0.0f));
    list.push_back(QVector3D(0.0f, 1.0f, 0.0f));
    list.push_back(QVector3D(0.0f, 0.0f, 1.0f));
    list.push_back(QVector3D(1.0f, 0.5f, 0.0f));
    list.push_back(QVector3D(0.9f, 0.0f, 0.4f));
    list.push_back(QVector3D(0.0f, 0.8f, 0.8f));
    list.push_back(QVector3D(1.0f, 1.0f, 1.0f));
}

//HalfEdge::HalfEdge(const HalfEdge &rightside)
//{
////    face_front = new Face();
////    face_front->next = face_front;
////    face_front->prev = face_front;
////    face_size = 0;

////    vtx_front = new Vertex();
////    vtx_front->next = vtx_front;
////    vtx_front->prev = vtx_front;
////    vtx_size = 0;

////    range = 0.0f;
////    x_trans = 0.0f;
////    y_trans = 0.0f;
////    z_trans = 0.0f;

////    rs_times = 0;

////    list.push_back(QVector3D(1.0f, 0.0f, 0.0f));
////    list.push_back(QVector3D(0.0f, 1.0f, 0.0f));
////    list.push_back(QVector3D(0.0f, 0.0f, 1.0f));
////    list.push_back(QVector3D(1.0f, 0.5f, 0.0f));
////    list.push_back(QVector3D(0.9f, 0.0f, 0.4f));
////    list.push_back(QVector3D(0.0f, 0.8f, 0.8f));
////    list.push_back(QVector3D(1.0f, 1.0f, 1.0f));

////    QVector<Vertex *> vtx_vector;
////    vtx_vector.append(get_vertex_front());
////    Vertex * vtx_current = rightside.get_vertex_first();
////    while (vtx_current != rightside.get_vertex_front()) {
////        Vertex * v = vtx_current;
////        add_vertex(v);
////        vtx_vector.push_back(v);

////        vtx_current = vtx_current->next;
////    }

////    Face * face = rightside.get_face_first();
////    while (face != rightside.get_face_front()) {

////        face = face->next;
////    }

//}

HalfEdge::~HalfEdge()
{
    while (face_size > 0) {
        Face * temp_face = face_front->next;
        face_front->next = face_front->next->next;
        face_front->next->prev = face_front;
        delete temp_face;
        face_size--;
    }
    face_front = NULL;


    while (vtx_size > 0) {
        Vertex * temp_vtx = vtx_front->next;
        vtx_front->next = vtx_front->next->next;
        vtx_front->next->prev = vtx_front;
        delete temp_vtx;
        vtx_size--;
    }
    vtx_front = NULL;
    rs_times = 0;
}

Face * HalfEdge::get_face_front() const
{
    return face_front;
}

Face * HalfEdge::get_face_first() const
{
    return face_front->next;
}

Vertex * HalfEdge::get_vertex_front() const
{
    return vtx_front;
}

Vertex * HalfEdge::get_vertex_first() const
{
    return vtx_front->next;
}

int HalfEdge::get_face_size() const
{
    return face_size;
}

int HalfEdge::get_vertex_size() const
{
    return vtx_size;
}

bool HalfEdge::is_empty() const
{
    return (this==NULL || face_size==0 || vtx_size==0);
}

bool HalfEdge::add_face(Face * &face)
{
    if (face == NULL) {
        return false;
    }
    else {
        face->prev = face_front->prev;
        face->next = face_front;
        face_front->prev->next = face;
        face_front->prev = face;
        face_size++;

        //qDebug() << face_size;
        //qDebug() << face->e1->vertex1->v_index << ", " << face->e2->vertex1->v_index << ", " << face->e3->vertex1->v_index;

        return true;
    }

}

bool HalfEdge::add_vertex(Vertex *&vertex)
{
    if (vertex == NULL) {
        return false;
    }
    else {
        vertex->prev = vtx_front->prev;
        vertex->next = vtx_front;
        vtx_front->prev->next = vertex;
        vtx_front->prev = vertex;
        vtx_size++;
        return true;
    }
}

void HalfEdge::set_poroperties()
{
    GLfloat x_min = 0.0f;
    GLfloat y_min = 0.0f;
    GLfloat z_min = 0.0f;
    GLfloat x_max = 0.0f;
    GLfloat y_max = 0.0f;
    GLfloat z_max = 0.0f;
    Vertex * vtx_current = get_vertex_first();
    while (vtx_current != get_vertex_front()) {
        if (x_max < vtx_current->coordinate.x())
            x_max = vtx_current->coordinate.x();
        if (x_min > vtx_current->coordinate.x())
            x_min = vtx_current->coordinate.x();
        if (y_max < vtx_current->coordinate.y())
            y_max = vtx_current->coordinate.y();
        if (y_min > vtx_current->coordinate.y())
            y_min = vtx_current->coordinate.y();
        if (z_max < vtx_current->coordinate.z())
            z_max = vtx_current->coordinate.z();
        if (z_min > vtx_current->coordinate.z())
            z_min = vtx_current->coordinate.z();

        vtx_current = vtx_current->next;
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

    // --- set faces colors
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    Face * face_current = get_face_first();
    while (face_current != get_face_front()) {
        int random = qrand() % 7;
        face_current->face_color = random;

        face_current = face_current->next;
    }
}

void HalfEdge::normalize_for_paint()
{
    Vertex * vtx_current = get_vertex_first();
    while (vtx_current != get_vertex_front()) {
        vtx_current->coordinate_normalized.setX(vtx_current->coordinate.x()/range);
        vtx_current->coordinate_normalized.setY(vtx_current->coordinate.y()/range);
        vtx_current->coordinate_normalized.setZ(vtx_current->coordinate.z()/range);
        vtx_current = vtx_current->next;
    }
}

void HalfEdge::clear_edges_of_vertex()
{
    Vertex * vtx_current = get_vertex_first();
    while (vtx_current != get_vertex_front()) {
        vtx_current->edge_list_in.clear();
        vtx_current->edge_list_out.clear();

        vtx_current = vtx_current->next;
    }
}

bool HalfEdge::construct_from_file(QString file_name)
{
    // ------------- read file begin -------------------------

    QTime time_begin = QTime::currentTime();
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "*** Can not open the file! File location is \""
                 << file_name << "\" ***" << endl;
        return false;
    }

    QVector<unsigned int> vi_list, ni_list, vti_list;
    QVector<QVector2D> vt_list;
    QVector<QVector3D> vn_list;
    vn_list.append(QVector3D(0.0f, 0.0f, 0.0f));
    QVector<Vertex *> vtx_list;
    vtx_list.push_back(get_vertex_front());

    QTextStream infile(&file);
    while (!infile.atEnd()) {
        QString line = infile.readLine();
        QTextStream sin(&line);
        QString s;
        sin >> s;
        if (s == "v") {
            deal_with_v(sin, vtx_list);
        }
        else if (s == "vt") {
            // --- do with the vt context
            //it is not metioned in the research
            deal_with_vt(sin, vt_list);
        }
        else if (s == "vn") {
            // --- the same as the 'vt'
            deal_with_vn(sin, vn_list);
        }
        else if (s == "f") {
            deal_with_f(sin, vi_list, vti_list, ni_list);
        }
    }

    // ------------- read file end -------------------------



    // ----------------- debug -------------------------------
//    if (!vi_list.empty()) {
//        for (int i=0; i<10; i++) {
//            qDebug() << vi_list.at(i);
//        }
//    }
//    if (!ni_list.empty()) {
//    qDebug() << "aaaaaaaaaaaaaadddddddddddddddddaaaaaaaaaa";
//        for (int i=0; i<10; i++) {
//            qDebug() << ni_list.at(i);
//        }
//    }
//    int j = 0;
//    Vertex * vtx = get_vertex_first();
//    qDebug() << "aaaaaaaaaaaaaaaaaaaaaaaa";
//    while (j < 10) {
//        qDebug() << vtx->v_index;
//        vtx = vtx->next;
//        j++;
//    }
    // ----------------- debug -------------------------------



    // ---------------- write normals to vertex begin --------------

    for (int i=0; i<ni_list.size(); ++i) {
        vtx_list.at(vi_list.at(i))->normal_coordinate = vn_list.at(ni_list.at(i));
    }

    // ---------------- write normals to vertex end   --------------

    // ------- construct half edge structure begin

    QQueue<Edge *> edge_queue;
    add_face_when_queue_is_null(edge_queue, vi_list, vtx_list);

    Edge * edge;
    while ((!edge_queue.isEmpty()) || vi_list.size()>0) {
        if (edge_queue.isEmpty()) {
            add_face_when_queue_is_null(edge_queue, vi_list, vtx_list);
        }
        else {
            edge = edge_queue.dequeue();
            //qDebug() << edge->vertex1->v_index << ", " << edge->vertex2->v_index;
            do_with_reverse(edge_queue, edge, vi_list, vtx_list);
        }
    }

    // ------- construct half edge structure end

    QTime time_end = QTime::currentTime();
    int time_sum = time_begin.msecsTo(time_end);

    qDebug("construct stable base mesh needs %d msecs", time_sum);

    return true;

}

void HalfEdge::deal_with_v(QTextStream &sin, QVector<Vertex *> &vtx_list)
{
    GLfloat x = 0.0f,
            y = 0.0f,
            z = 0.0f;
    sin >> x >> y >> z;
    Vertex * vtx_new = new Vertex(get_vertex_size()+1, x, y, z);
    add_vertex(vtx_new);
    vtx_list.push_back(vtx_new);
}

void HalfEdge::deal_with_vt(QTextStream &sin, QVector<QVector2D> &vt_list)
{
    GLfloat x = 0.0f,
            y = 0.0f;
    sin >> x >> y;
    vt_list.push_back(QVector2D(x, y));
}


void HalfEdge::deal_with_vn(QTextStream &sin, QVector<QVector3D> &vn_list)
{
    GLfloat x = 0.0f,
            y = 0.0f,
            z = 0.0f;
    sin >> x >> y >> z;
    vn_list.push_back(QVector3D(x, y, z));
}

void HalfEdge::deal_with_f(QTextStream &sin, QVector<unsigned int> &vi_list, QVector<unsigned int> &vti_list, QVector<unsigned int> &ni_list)
{
    // part into 4 following cases
    // 1. a
    // 2. a/b
    // 3. a/b/c
    // 4. a//c
    for (int i=0; i<3; ++i) {
        QString str;
        sin >> str;
        int index = str.indexOf('/');

        if (index == -1) {
            // There is no '/' in str
            // so, Case 1
            unsigned int a = str.toUInt();
            vi_list.push_back(a);
        }
        else {
            // There is a '/' in str or two
            // but, anyway, there have to be vertex indices
            // in the str, so put it into vertex index list
            unsigned int a = (str.mid(0, index)).toUInt();
            vi_list.push_back(a);

            // Cut str form the position of ('/'+1)
            // to get a new str
            str = str.mid(index+1);
            index = str.indexOf('/');

            if (index == -1) {
                // There is no '/' in the new str
                // So, Case 2
                unsigned int b = str.toUInt();
                vti_list.push_back(b);
            }
            else if (index == 0) {
                // The new str is began with '/'
                // So, Case 4
                str = str.mid(index+1);
                unsigned int c = str.toUInt();
                ni_list.push_back(c);
            }
            else {
                // Case 3:
                unsigned int b = (str.mid(0, index)).toUInt();
                vti_list.push_back(b);
                str = str.mid(index+1);
                unsigned int c = str.toUInt();
                ni_list.push_back(c);
            }

        }
    }

}

bool HalfEdge::do_with_reverse(QQueue<Edge *> &edge_queue, Edge * &edge, QVector<unsigned int> & vi_array, QVector<Vertex *> &vtx_list)
{
    if (!search_in_queue(edge_queue, edge)) {
        Face * new_face = search_in_viarray(edge_queue, edge, vi_array, vtx_list);
        if (new_face != NULL) {
            add_face(new_face);
        }
    }
    return true;
}

bool HalfEdge::search_in_queue(QQueue<Edge *> &edge_queue, Edge *&edge)
{
    bool flag = false;
    for (int i=0; i<edge_queue.size(); i++) {
        if (edge_queue.at(i)->getX()==edge->getY() &&
                edge_queue.at(i)->getY()==edge->getX()) {
            edge->reverse = edge_queue.at(i);
            edge_queue.at(i)->reverse = edge;
            edge_queue.removeAt(i);
            flag = true;
            break;
        }
    }
    return flag;
}

Face * HalfEdge::search_in_viarray(QQueue<Edge *> &edge_queue, Edge *&edge, QVector<unsigned int> &vi_array, QVector<Vertex *> &vtx_list)
{
    Face * new_face = NULL;
    int i=0;
    while (!vi_array.isEmpty() && i<vi_array.size()) {
        new_face = search_in_viarray_per(edge_queue, edge, vi_array, i, vtx_list);
        if (new_face != NULL) {

            //qDebug() << vi_array.at(i) << " " << vi_array.at(i+1) << " " << vi_array.at(i+2);
            vi_array.removeAt(i);
            vi_array.removeAt(i);
            vi_array.removeAt(i);
            break;
        }
        i += 3;
    }
    return new_face;

}

Face * HalfEdge::search_in_viarray_per(QQueue<Edge *> &edge_queue, Edge *&edge, QVector<unsigned int> &vi_array, int i, QVector<Vertex *> &vtx_list)
{
    Face * new_face = NULL;
    unsigned int x = vi_array.at(i);
    unsigned int y = vi_array.at(i+1);
    unsigned int z = vi_array.at(i+2);


    if ((edge->getX()==x&&edge->getY()==y) || (edge->getY()==x&&edge->getX()==y)) {
        Edge * e1 = new Edge(edge->vertex2, edge->vertex1);
        Vertex * vertex = vtx_list.at(z);
        Edge * e2 = new Edge(e1->vertex2, vertex);
        Edge * e3 = new Edge(vertex, e1->vertex1);

        e1->add_edge_to_vtx_list();
        e2->add_edge_to_vtx_list();
        e3->add_edge_to_vtx_list();

        e1->reverse = edge;
        e1->reverse->reverse = e1;
        e1->next = e2;
        e2->next = e3;
        e3->next = e1;
        e1->prev = e3;
        e2->prev = e1;
        e3->prev = e2;
        new_face = new Face();
        new_face->e1 = e1;
        new_face->e2 = e2;
        new_face->e3 = e3;
        e1->face = new_face;
        e2->face = new_face;
        e3->face = new_face;
        edge_queue.enqueue(new_face->e2);
        edge_queue.enqueue(new_face->e3);
        return new_face;
    }

    if ((edge->getX()==y && edge->getY()==z) || (edge->getY()==y && edge->getX()==z)) {
        Edge * e1 = new Edge(edge->vertex2, edge->vertex1);
        Vertex * vertex = vtx_list.at(x);
        Edge * e2 = new Edge(e1->vertex2, vertex);
        Edge * e3 = new Edge(vertex, e1->vertex1);

        e1->add_edge_to_vtx_list();
        e2->add_edge_to_vtx_list();
        e3->add_edge_to_vtx_list();

        e1->reverse = edge;
        e1->reverse->reverse = e1;
        e1->next = e2;
        e2->next = e3;
        e3->next = e1;
        e1->prev = e3;
        e2->prev = e1;
        e3->prev = e2;
        new_face = new Face();
        new_face->e1 = e1;
        new_face->e2 = e2;
        new_face->e3 = e3;
        e1->face = new_face;
        e2->face = new_face;
        e3->face = new_face;
        edge_queue.enqueue(new_face->e2);
        edge_queue.enqueue(new_face->e3);
        return new_face;
    }

    if ((edge->getX()==z && edge->getY()==x) || (edge->getY()==z && edge->getX()==x)) {
        Edge * e1 = new Edge(edge->vertex2, edge->vertex1);
        Vertex * vertex = vtx_list.at(y);
        Edge * e2 = new Edge(e1->vertex2, vertex);
        Edge * e3 = new Edge(vertex, e1->vertex1);

        e1->add_edge_to_vtx_list();
        e2->add_edge_to_vtx_list();
        e3->add_edge_to_vtx_list();

        e1->reverse = edge;
        e1->reverse->reverse = e1;
        e1->next = e2;
        e2->next = e3;
        e3->next = e1;
        e1->prev = e3;
        e2->prev = e1;
        e3->prev = e2;
        new_face = new Face();
        new_face->e1 = e1;
        new_face->e2 = e2;
        new_face->e3 = e3;
        e1->face = new_face;
        e2->face = new_face;
        e3->face = new_face;
        edge_queue.enqueue(new_face->e2);
        edge_queue.enqueue(new_face->e3);
        return new_face;
    }

    return NULL;
}

void HalfEdge::add_face_when_queue_is_null(QQueue<Edge *> &edge_queue, QVector<unsigned int> &vi_array, QVector<Vertex *> &vtx_list)
{

    Face * first_face = new Face();

    Vertex * v1 = vtx_list.at(vi_array.at(0));
    Vertex * v2 = vtx_list.at(vi_array.at(1));
    Vertex * v3 = vtx_list.at(vi_array.at(2));

//    qDebug() << "(" << v1->v_index << ", " << v2->v_index << ", " << v3->v_index;
   /*
    Vertex * v1 = get_vertex(vi_array.at(0));
    Vertex * v2 = get_vertex(vi_array.at(1));
    Vertex * v3 = get_vertex(vi_array.at(2));*/

    Edge * e1 = new Edge(v1, v2);
    Edge * e2 = new Edge(v2, v3);
    Edge * e3 = new Edge(v3, v1);
    e1->add_edge_to_vtx_list();
    e2->add_edge_to_vtx_list();
    e3->add_edge_to_vtx_list();

    vi_array.pop_front();
    vi_array.pop_front();
    vi_array.pop_front();
    e1->next = e2;
    e2->next = e3;
    e3->next = e1;
    e1->prev = e3;
    e2->prev = e1;
    e3->prev = e2;
    e1->face = first_face;
    e2->face = first_face;
    e3->face = first_face;
    first_face->e1 = e1;
    first_face->e2 = e2;
    first_face->e3 = e3;
    add_face(first_face);

    edge_queue.enqueue(e1);
    edge_queue.enqueue(e2);
    edge_queue.enqueue(e3);
}


void HalfEdge::draw(int xRot, int yRot, int zRot, GLfloat m_fScale, int draw_flag) const
{
    glTranslatef(-x_trans/range, -y_trans/range, -z_trans/range-1.0);

    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    glScalef(m_fScale, m_fScale, m_fScale);

    if (draw_flag == 0) {
        draw_normal();
    }
    else {
        draw_partition();
    }
}

void HalfEdge::draw_normal() const
{
    Face * face_current = get_face_first();
    glColor3f(1.0f, 1.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    while (face_current != get_face_front()) {
        glColor4f(list.at(face_current->face_color).x(),
                    list.at(face_current->face_color).y(),
                    list.at(face_current->face_color).z(), 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex3f(face_current->e1->vertex1->coordinate_normalized.x(),
                    face_current->e1->vertex1->coordinate_normalized.y(),
                   face_current->e1->vertex1->coordinate_normalized.z());
        glVertex3f(face_current->e2->vertex1->coordinate_normalized.x(),
                   face_current->e2->vertex1->coordinate_normalized.y(),
                   face_current->e2->vertex1->coordinate_normalized.z());
        glVertex3f(face_current->e3->vertex1->coordinate_normalized.x(),
                    face_current->e3->vertex1->coordinate_normalized.y(),
                   face_current->e3->vertex1->coordinate_normalized.z());
        glEnd();

        face_current = face_current->next;
    }


    glColor4f(0.6f, 0.6f, 0.6f, 0.0f);
    face_current = get_face_first();
    while (face_current != get_face_front()) {
        glBegin(GL_LINE_LOOP);
        glVertex3f(face_current->e1->vertex1->coordinate_normalized.x(),
                    face_current->e1->vertex1->coordinate_normalized.y(),
                   face_current->e1->vertex1->coordinate_normalized.z());
        glVertex3f(face_current->e2->vertex1->coordinate_normalized.x(),
                   face_current->e2->vertex1->coordinate_normalized.y(),
                   face_current->e2->vertex1->coordinate_normalized.z());
        glVertex3f(face_current->e3->vertex1->coordinate_normalized.x(),
                    face_current->e3->vertex1->coordinate_normalized.y(),
                   face_current->e3->vertex1->coordinate_normalized.z());
        glEnd();

        face_current = face_current->next;
    }
}

void HalfEdge::draw_partition() const
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLES);
    Face * face_current = get_face_first();
    glColor3f(0.0f, 0.0f, 0.0f);
    while (face_current != get_face_front()) {
        glVertex3f(face_current->e1->vertex1->coordinate_normalized.x(),
                   face_current->e1->vertex1->coordinate_normalized.y(),
                   face_current->e1->vertex1->coordinate_normalized.z());
        glVertex3f(face_current->e2->vertex1->coordinate_normalized.x(),
                   face_current->e2->vertex1->coordinate_normalized.y(),
                   face_current->e2->vertex1->coordinate_normalized.z());
        glVertex3f(face_current->e3->vertex1->coordinate_normalized.x(),
                   face_current->e3->vertex1->coordinate_normalized.y(),
                   face_current->e3->vertex1->coordinate_normalized.z());

        face_current = face_current->next;
    }
    glEnd();

    glPointSize(5.0f);
    glBegin(GL_POINTS);
    Vertex * vtx_current = get_vertex_first();
    while (vtx_current != get_vertex_front()) {
        if (vtx_current->isOod()) {
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(vtx_current->coordinate_normalized.x(),
                       vtx_current->coordinate_normalized.y(),
                       vtx_current->coordinate_normalized.z());
        }
        else if (vtx_current->isEven()) {
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(vtx_current->coordinate_normalized.x(),
                       vtx_current->coordinate_normalized.y(),
                       vtx_current->coordinate_normalized.z());
        }
        vtx_current = vtx_current->next;
    }
    glEnd();

}


bool HalfEdge::subdivision_butterfly()
{
    if (is_empty()) {
        return false;
    }

    clear_edges_of_vertex();

    Face * current = get_face_first();
    int origin_size = get_face_size();

    for (int i=0; i<origin_size; ++i) {
        edge_subdivision_butterfly(current->e1);
        edge_subdivision_butterfly(current->e2);
        edge_subdivision_butterfly(current->e3);

        face_subdivide_to_four(current);

        current = current->next;
    }

    for (int i=0; i<origin_size; ++i) {
        delete_face_front();
    }

    return true;
}

bool HalfEdge::subdivision_loop_revision()
{
    if (is_empty()) {
        return false;
    }

    clear_edges_of_vertex();

    Face * current = get_face_first();
    int origin_size = get_face_size();

    for (int i=0; i<origin_size; ++i) {
        edge_subdivision_loop_revision(current->e1);
        edge_subdivision_loop_revision(current->e2);
        edge_subdivision_loop_revision(current->e3);

        face_subdivide_to_four(current);

        current = current->next;
    }

    for (int i=0; i<origin_size; ++i) {
        delete_face_front();
    }

    return true;
}

void HalfEdge::edge_subdivision_butterfly(Edge *&current)
{
    if (current->flag == UNSUBDIVIDED) {
        Vertex *v1, *v2, *v3, *v4, *q1, *q2, *q3, *q4;
        v1 = current->vertex1;
        v2 = current->vertex2;
        v3 = current->next->vertex2;

        Edge * edge_Q1 = current->prev->reverse;
        if (edge_Q1 == NULL) {
            q1 = new Vertex(0.0f, 0.0f, 0.0f);
        }
        else {
            q1 = edge_Q1->next->vertex2;
        }
        Edge * edge_Q2 = current->next->reverse;
        if (edge_Q2 == NULL) {
            q2 = new Vertex(0.0f, 0.0f, 0.0f);
        }
        else {
            q2 = edge_Q2->next->vertex2;
        }
        Edge * edge_rev = current->reverse;
        if (edge_rev != NULL) {
            v4 = edge_rev->next->vertex2;

            if (edge_rev->prev->reverse != NULL) {
                q3 = edge_rev->prev->reverse->next->vertex2;
            }
            else {
                q3 = new Vertex(0.0f, 0.0f, 0.0f);
            }

            if (edge_rev->next->reverse != NULL) {
                q4 = edge_rev->next->reverse->next->vertex2;
            }
            else {
                q4 = new Vertex(0.0f, 0.0f, 0.0f);
            }
        }
        else {
            v4 = new Vertex(0.0f, 0.0f, 0.0f);
            q3 = new Vertex(0.0f, 0.0f, 0.0f);
            q4 = new Vertex(0.0f, 0.0f, 0.0f);
        }

        QVector3D v_new_coordinate;
        if ((!v4->isZero()) && (!q1->isZero()) && (!q2->isZero()) && (!q3->isZero()) && (!q4->isZero())) {
            v_new_coordinate = (8*(v1->coordinate+v2->coordinate) + 2*(v3->coordinate+v4->coordinate) -
                (q1->coordinate+q2->coordinate+q3->coordinate+q4->coordinate)) / 16;
        }
        else{
            if (!v4->isZero())
                v_new_coordinate = (3*(v1->coordinate+v2->coordinate) + 1*(v3->coordinate+v4->coordinate)) / 8;
            else
                v_new_coordinate = (v1->coordinate + v2->coordinate) / 2;
        }
        Vertex * vtx_new = new Vertex(v_new_coordinate);
        vtx_new->v_index = get_vertex_size() + 1;
        add_vertex(vtx_new);
        current->child1 = new Edge(current->vertex1, vtx_new);
        current->child2 = new Edge(vtx_new, current->vertex2);
        //current->child1->parent = current;
        //current->child2->parent = current;
        current->vtx_in = vtx_new;
        current->flag = SUBDIVIDED;

        if (current->reverse != NULL) {

            current->reverse->child1 = new Edge(current->reverse->vertex1, vtx_new);
            current->reverse->child2 = new Edge(vtx_new, current->reverse->vertex2);
            current->child1->reverse = current->reverse->child2;
            current->reverse->child2->reverse = current->child1;
            current->child2->reverse = current->reverse->child1;
            current->reverse->child1->reverse = current->child2;
            current->reverse->vtx_in = vtx_new;
            //current->reverse->child1->parent = current->reverse;
            //current->reverse->child2->parent = current->reverse;
            current->reverse->flag = SUBDIVIDED;
        }

    }
}

void HalfEdge::edge_subdivision_loop_revision(Edge *&current)
{
    if (current->flag == UNSUBDIVIDED) {
        Vertex *v1, *v2, *v3, *v4;
        v1 = current->vertex1;
        v2 = current->vertex2;
        v3 = current->next->vertex2;

        Edge * edge_rev = current->reverse;
        if (edge_rev != NULL) {
            v4 = edge_rev->next->vertex2;
        }
        else {
            v4 = new Vertex(0.0f, 0.0f, 0.0f);
        }

        QVector3D v_new_coordinate;
        if (v4->isZero()) {
            v_new_coordinate = (v1->coordinate + v2->coordinate) / 2;
        }
        else {
            v_new_coordinate = (3*(v1->coordinate+v2->coordinate) + v3->coordinate + v4->coordinate) / 8;
        }

        Vertex * vtx_new = new Vertex(v_new_coordinate);
        vtx_new->v_index = get_vertex_size() + 1;
        add_vertex(vtx_new);
        current->child1 = new Edge(current->vertex1, vtx_new);
        current->child2 = new Edge(vtx_new, current->vertex2);
        current->vtx_in = vtx_new;
        current->flag = SUBDIVIDED;

        if (current->reverse != NULL) {
            current->reverse->child1 = new Edge(current->reverse->vertex1, vtx_new);
            current->reverse->child2 = new Edge(vtx_new, current->reverse->vertex2);
            current->child1->reverse = current->reverse->child2;
            current->reverse->child2->reverse = current->child1;
            current->child2->reverse = current->reverse->child1;
            current->reverse->child1->reverse = current->child2;
            current->reverse->vtx_in = vtx_new;
            current->reverse->flag = SUBDIVIDED;
        }
    }
}

void HalfEdge::face_subdivide_to_four(Face *&current)
{
    Face * face_new_1 = new Face();
    face_new_1->e1 = current->e1->child2;
    face_new_1->e2 = current->e2->child1;
    Edge * edge_new_1 = new Edge(face_new_1->e2->vertex2, face_new_1->e1->vertex1);
    face_new_1->e3 = edge_new_1;

    face_new_1->e1->add_edge_to_vtx_list();
    face_new_1->e2->add_edge_to_vtx_list();
    face_new_1->e3->add_edge_to_vtx_list();

    face_new_1->e1->next = face_new_1->e2;
    face_new_1->e2->next = face_new_1->e3;
    face_new_1->e3->next = face_new_1->e1;
    face_new_1->e1->prev = face_new_1->e3;
    face_new_1->e2->prev = face_new_1->e1;
    face_new_1->e3->prev = face_new_1->e2;
    face_new_1->e1->face = face_new_1;
    face_new_1->e2->face = face_new_1;
    face_new_1->e3->face = face_new_1;
    add_face(face_new_1);

    Face * face_new_2 = new Face();
    face_new_2->e1 = current->e2->child2;
    face_new_2->e2 = current->e3->child1;
    Edge * edge_new_2 = new Edge(face_new_2->e2->vertex2, face_new_2->e1->vertex1);
    face_new_2->e3 = edge_new_2;

    face_new_2->e1->add_edge_to_vtx_list();
    face_new_2->e2->add_edge_to_vtx_list();
    face_new_2->e3->add_edge_to_vtx_list();

    face_new_2->e1->next = face_new_2->e2;
    face_new_2->e2->next = face_new_2->e3;
    face_new_2->e3->next = face_new_2->e1;
    face_new_2->e1->prev = face_new_2->e3;
    face_new_2->e2->prev = face_new_2->e1;
    face_new_2->e3->prev = face_new_2->e2;
    face_new_2->e1->face = face_new_2;
    face_new_2->e2->face = face_new_2;
    face_new_2->e3->face = face_new_2;
    add_face(face_new_2);

    Face * face_new_3 = new Face();
    face_new_3->e1 = current->e3->child2;
    face_new_3->e2 = current->e1->child1;
    Edge * edge_new_3 = new Edge(face_new_3->e2->vertex2, face_new_3->e1->vertex1);
    face_new_3->e3 = edge_new_3;

    face_new_3->e1->add_edge_to_vtx_list();
    face_new_3->e2->add_edge_to_vtx_list();
    face_new_3->e3->add_edge_to_vtx_list();

    face_new_3->e1->next = face_new_3->e2;
    face_new_3->e2->next = face_new_3->e3;
    face_new_3->e3->next = face_new_3->e1;
    face_new_3->e1->prev = face_new_3->e3;
    face_new_3->e2->prev = face_new_3->e1;
    face_new_3->e3->prev = face_new_3->e2;
    face_new_3->e1->face = face_new_3;
    face_new_3->e2->face = face_new_3;
    face_new_3->e3->face = face_new_3;
    add_face(face_new_3);

    Edge * edge_rev_1 = new Edge(face_new_1->e3->vertex2, face_new_1->e3->vertex1);
    Edge * edge_rev_2 = new Edge(face_new_2->e3->vertex2, face_new_2->e3->vertex1);
    Edge * edge_rev_3 = new Edge(face_new_3->e3->vertex2, face_new_3->e3->vertex1);


    face_new_1->e3->reverse = edge_rev_1;
    face_new_2->e3->reverse = edge_rev_2;
    face_new_3->e3->reverse = edge_rev_3;
    edge_rev_1->reverse = face_new_1->e3;
    edge_rev_2->reverse = face_new_2->e3;
    edge_rev_3->reverse = face_new_3->e3;


    Face * face_new_4 = new Face();
    face_new_4->e1 = edge_rev_1;
    face_new_4->e2 = edge_rev_2;
    face_new_4->e3 = edge_rev_3;
    face_new_4->e1->add_edge_to_vtx_list();
    face_new_4->e2->add_edge_to_vtx_list();
    face_new_4->e3->add_edge_to_vtx_list();
    edge_rev_1->next = edge_rev_2;
    edge_rev_2->next = edge_rev_3;
    edge_rev_3->next = edge_rev_1;
    edge_rev_1->prev = edge_rev_3;
    edge_rev_2->prev = edge_rev_1;
    edge_rev_3->prev = edge_rev_2;
    edge_rev_1->face = face_new_4;
    edge_rev_2->face = face_new_4;
    edge_rev_3->face = face_new_4;
    add_face(face_new_4);

}

void HalfEdge::clean_parent_faces(int origin_size)
{
    for (int i=0; i<origin_size; ++i) {
        delete_face_front();
    }
}

bool HalfEdge::delete_face_front()
{
    if (get_face_size() == 0) {
        return false;
    }

    Face * temp_face = get_face_first();
    face_front->next = face_front->next->next;
    face_front->next->prev = face_front;
    delete temp_face;
    face_size--;
    return true;
}

bool HalfEdge::reverse_subdivision()
{
    bool flag = true;

    //unpart_vtx();

    if (partition()) {
        clear_edges_of_vertex();

        Face * current_face = get_face_first();
        int face_size_origin = get_face_size();
        int i = 0;
        for (i=0; i<face_size_origin; ++i) {
            face_dispose_rs(current_face);
            current_face = current_face->next;
        }

        // delete the origin faces
        for (i=0; i<face_size_origin; ++i) {
            delete_face_front();
        }

        // delete the odd vertices
        delete_ood_vtx();

        //set_poroperties();
        //normalize_for_paint();
    }
    else {
        flag = false;
    }

    return flag;
}

void HalfEdge::unpart_vtx()
{
    Vertex * vtx_current = get_vertex_first();
    while (vtx_current != get_vertex_front()) {
        vtx_current->set_unparted();

        vtx_current = vtx_current->next;
    }
}

bool HalfEdge::partition()
{

    unpart_vtx();
    Vertex * start_vertex = get_vertex_extraordinary();

    QQueue<Vertex *> vtx_queue;
    bool flag = true;
    while (start_vertex != NULL && flag) {
        set_even(start_vertex, vtx_queue);
        while (!vtx_queue.empty() && vtx_queue.size()!=0 && flag) {
            Vertex * vertex = vtx_queue.dequeue();
            if (!deal_with_odd(vertex, vtx_queue))
                flag = false;
        }
        start_vertex  = get_vertex_extraordinary();
    }

    return flag;
}

Vertex * HalfEdge::get_vertex_extraordinary() const
{
    Vertex * vtx = NULL;
    Vertex * current = get_vertex_first();
    while (current != get_vertex_front()) {
        if (current->edge_list_in.size()!=6 && current->edge_list_out.size()!=6 && (!current->isParted())) {
            vtx = current;
            break;
        }
        current = current->next;
    }
    if (vtx == NULL) {
        vtx = get_unparted();
    }
    return vtx;

}

Vertex * HalfEdge::get_unparted() const
{
    Vertex * vtx = NULL;
    Vertex * current = get_vertex_first();
    while (current != get_vertex_front()) {
        if (!current->isParted()) {
            vtx = current;
            break;
        }
        current = current->next;
    }

    return vtx;
}

bool HalfEdge::face_dispose_rs(Face *&current_face)
{
    // --- judge whether the three vertices are all ood
    if (current_face->e1->vertex1->isOod() && current_face->e2->vertex1->isOod() && current_face->e3->vertex1->isOod()) {
        //Vertex * v1 = current_face->e1->vertex1;

//        qDebug() << "++++++++++++++++++++++++++++++++++++++++";

        Vertex * v1 = current_face->e1->reverse->next->vertex2;
        Vertex * v2 = current_face->e2->reverse->next->vertex2;
        Vertex * v3 = current_face->e3->reverse->next->vertex2;
        Edge * e1 = get_edge(current_face->e1, v3, v1, v2);
        Edge * e2 = get_edge(current_face->e2, v1, v2, v3);
        Edge * e3 = get_edge(current_face->e3, v2, v3, v1);
        e1->add_edge_to_vtx_list();
        e2->add_edge_to_vtx_list();
        e3->add_edge_to_vtx_list();

        Face * face_new = new Face(e1, e2, e3);
        add_face(face_new);

        //QVector3D error1 = get_error(v1);
        // --- make sure the direction is correct

        return true;
    }

    return false;
}

bool HalfEdge::delete_ood_vtx()
{
    if (get_vertex_size() == 0)
        return false;
    Vertex * vtx_current = get_vertex_front();
    while (vtx_current->next != get_vertex_front()) {
        if (vtx_current->next->isOod()) {
            Vertex * vtx_temp = vtx_current->next;
            vtx_current->next  = vtx_current->next->next;
            vtx_current->next->prev = vtx_current;
            delete vtx_temp;
        }
        else {
            vtx_current = vtx_current->next;
        }
    }
    return true;
}

bool HalfEdge::deal_with_odd(Vertex *&vertex, QQueue<Vertex *> &vtx_queue)
{
    if (vertex->edge_list_in.size()==6 && vertex->edge_list_out.size()==6) {
        for (int j=0; j<vertex->edge_list_out.size(); ++j) {
            if (vertex->edge_list_out.at(j)->vertex2->isEven()) {
                Vertex * vertex_even = vertex->edge_list_out.at(j)->next->next->reverse->next->next->reverse->next->next->vertex1;
                set_even(vertex_even, vtx_queue);
            }
        }
        return true;
    }
//    Vertex * vtx_out = NULL;
//    Vertex * vtx_in = NULL;
    Edge * edge_out = NULL;
    Edge * edge_in = NULL;
    for (int i=0; i<vertex->edge_list_out.size(); ++i) {
        if (vertex->edge_list_out.at(i)->vertex2->isEven()) {
//            vtx_out = vertex->edge_list_out.at(i)->vertex2;
            edge_out = vertex->edge_list_out.at(i);
            break;
        }
    }
    for (int i=0; i<vertex->edge_list_in.size(); ++i) {
        if (vertex->edge_list_in.at(i)->vertex1->isEven()) {
//            vtx_in = vertex->edge_list_in.at(i)->vertex1;
            edge_in = vertex->edge_list_in.at(i);
            break;
        }
    }
    if (edge_in!=NULL && edge_out!=NULL) {
        return true;
    }
    else {


        // ----------------------------
        // --- for more code
        // -----------------------------

        Vertex * another_even = NULL;
        if (edge_in == NULL) {
            another_even = edge_out->prev->reverse->prev->reverse->prev->vertex1;
        }
        else {
            another_even = edge_in->next->reverse->next->reverse->next->vertex2;
        }
        return set_even(another_even, vtx_queue);
    }

}

bool HalfEdge::set_even(Vertex *&vertex, QQueue<Vertex *> &vtx_queue)
{
    if (vertex->isOod()) {
        qDebug() << "Oh my god!!!!";
        return false;
    }
    if (vertex->isEven())
        return true;
    vertex->set_even();
    bool flag = true;


    for (int i=0; i<vertex->edge_list_out.size(); ++i) {
        if (!vertex->edge_list_out[i]->vertex2->isEven()) {
            if (!vertex->edge_list_out[i]->vertex2->isOod()) {
                vertex->edge_list_out[i]->vertex2->set_ood();
                vtx_queue.enqueue(vertex->edge_list_out[i]->vertex2);
            }
        }
        else {
            flag = false;
            break;
        }
    }
    for (int i=0; i<vertex->edge_list_in.size(); ++i) {
        if (!vertex->edge_list_in[i]->vertex1->isEven()) {
            if (!vertex->edge_list_in[i]->vertex1->isOod()) {
                vertex->edge_list_in[i]->vertex1->set_ood();
                vtx_queue.enqueue(vertex->edge_list_in[i]->vertex1);
            }
        }
        else {
            flag = false;
            break;
        }
    }
    return flag;

}


Edge * HalfEdge::get_edge(Edge *&edge, Vertex *&v1, Vertex *&v2, Vertex *&v3)
{

    if (edge->reverse->next->parent != NULL)
        return edge->reverse->next->parent;
    else {
        Vertex * v4 = NULL;
        if (edge->reverse->next->reverse != NULL) {
            v4 = edge->reverse->next->reverse->next->reverse->prev->reverse->next->vertex2;
        }

        QVector3D error = get_error(edge->vertex1, v1, v2, v3, v4);
        // edge->v_index_rvsbd = edge->vertex1->v_index;
        Edge * child2 = edge->reverse->next;
        Edge * child1 = edge->prev->reverse->prev;
        Edge * edge_new = new Edge(child1->vertex1, child2->vertex2);
        //edge_new->add_edge_to_vtx_list();
        edge_new->error = error;
        child1->parent = edge_new;
        child2->parent = edge_new;
        if (v4 != NULL) {
            Edge * edge_new_reverse = new Edge(child2->reverse->vertex1, child1->reverse->vertex2);
            edge_new_reverse->error = error;
            child1->reverse->parent = edge_new_reverse;
            child2->reverse->parent = edge_new_reverse;
            edge_new->reverse = edge_new_reverse;
            edge_new_reverse->reverse = edge_new;
            edge_new_reverse->v_index_rvsbd = edge->vertex1->v_index;
            //edge_new_reverse->add_edge_to_vtx_list();
        }
        edge_new->v_index_rvsbd = edge->vertex1->v_index;
        return edge_new;
    }
}


QVector3D HalfEdge::get_error(Vertex *&v0, Vertex *&v1, Vertex *&v2, Vertex *&v3, Vertex *&v4)
{
    QVector3D error;
    error.setX(0.0f);
    error.setY(0.0f);
    error.setZ(0.0f);
    QVector3D predict_coordinate;

    if (v4 == NULL) {
        predict_coordinate = (v1->coordinate + v2->coordinate) / 2;
    }
    else {
        predict_coordinate = (3*(v1->coordinate+v2->coordinate) + v3->coordinate + v4->coordinate) / 8;
    }

    error = v0->coordinate - predict_coordinate;

    return error;
}

bool HalfEdge::write_file(QString file_name, int times)
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

    name.append("_");
    name.append(QString::number(times));
    path.append(name);

    QString path_origin;
    path_origin += path + ".obj";
    QString path_error;
    path_error += path + ".err";
    if (write_mesh(path_origin))
        return (write_error(path_error));
    else
        return false;

}

bool HalfEdge::write_pretreatment(QString file_name)
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

    QDir dir(path);
    if (!dir.exists()) {
        qDebug() << "Path does not exist...";
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

    // --- _00 does not exist
    QString _path00;
    _path00 += path + name;
    _path00 += "_00.obj";
    qDebug() << _path00;
    QFile _file00(_path00);
    if (!_file00.open(QIODevice::ReadOnly | QIODevice::Text)) {
        write_mesh(_path00);
    }
    else {
        _file00.close();
    }

    for (int i=0; i<string_list.size(); ++i) {
        if (string_list.at(i).compare(_path00) != 0) {
            qDebug() << string_list.at(i);
            QFile f(string_list.at(i));
            f.remove();
        }
    }

    return true;

}

bool HalfEdge::write_mesh(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
        QTextStream out(&file);
        Vertex * vtx = get_vertex_first();
        while (vtx != get_vertex_front()) {
            out << "v " << vtx->v_index << " " << QString::number(vtx->coordinate.x()) << " "
                << QString::number(vtx->coordinate.y()) << " " << QString::number(vtx->coordinate.z()) << endl;
            vtx = vtx->next;
        }
        Face * face = get_face_first();
        while (face != get_face_front()) {
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

bool HalfEdge::write_error(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
        QTextStream out(&file);
        Face * face = get_face_first();
        while (face != get_face_front()) {
            if (face->e1->outputed == UNOUTPUTED) {
                out << face->e1->v_index_rvsbd << " "
                    << face->e1->vertex1->v_index << " " << face->e1->vertex2->v_index << " "
                    << face->e1->error.x() << " " << face->e1->error.y() << " " << face->e1->error.z() << endl;
                face->e1->outputed = OUTPUTED;
                if (face->e1->reverse != NULL) {
                    face->e1->reverse->outputed = OUTPUTED;
                }
            }
            if (face->e2->outputed == UNOUTPUTED) {
                out << face->e2->v_index_rvsbd << " "
                    << face->e2->vertex1->v_index << " " << face->e2->vertex2->v_index << " "
                    << face->e2->error.x() << " " << face->e2->error.y() << " " << face->e2->error.z() << endl;
                face->e2->outputed = OUTPUTED;
                if (face->e2->reverse != NULL) {
                    face->e2->reverse->outputed = OUTPUTED;
                }
            }
            if (face->e3->outputed == UNOUTPUTED) {
                out << face->e3->v_index_rvsbd << " "
                    << face->e3->vertex1->v_index << " " << face->e3->vertex2->v_index << " "
                    << face->e3->error.x() << " " << face->e3->error.y() << " " << face->e3->error.z() << endl;
                face->e3->outputed = OUTPUTED;
                if (face->e3->reverse != NULL) {
                    face->e3->reverse->outputed = OUTPUTED;
                }
            }
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

bool HalfEdge::construct_base_mesh(QString file_name)
{

    QTime time_begin = QTime::currentTime();

    QFile file(file_name);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "*** Can not open the file! File location is \""
                 << file_name << "\" ***" << endl;
        return false;
    }

    QVector<unsigned int> vi_list, ni_list, vti_list;
    QVector<QVector2D> vt_list;
    QVector<QVector3D> vn_list;
    vn_list.append(QVector3D(0.0f, 0.0f, 0.0f));
    QHash<int, Vertex *> vtx_hash;

    QTextStream infile(&file);
    while (!infile.atEnd()) {
        QString line = infile.readLine();
        QTextStream sin(&line);
        QString s;
        sin >> s;
        if (s == "v") {
            unsigned int vi = 0;
            GLfloat x = 0.0f,
                    y = 0.0f,
                    z = 0.0f;
            sin >> vi >> x >> y >> z;
            Vertex * vtx_new = new Vertex(vi, x, y, z);
            add_vertex(vtx_new);
            // -------------------------
            vtx_hash.insert(vi, vtx_new);
        }
        else if (s == "f") {
            unsigned int a1 = 0,
                    a2 = 0,
                    a3 = 0;
            sin >> a1 >> a2 >> a3;
            vi_list.push_back(a1);
            vi_list.push_back(a2);
            vi_list.push_back(a3);
        }
    }

    QQueue<Edge *> edge_queue;
    add_face_when_queue_is_null(edge_queue, vi_list, vtx_hash);

    Edge * edge;
    while ((!edge_queue.isEmpty()) || vi_list.size()>0) {
        if (edge_queue.isEmpty()) {
            add_face_when_queue_is_null(edge_queue, vi_list, vtx_hash);
        }
        else {
            edge = edge_queue.dequeue();
            do_with_reverse(edge_queue, edge, vi_list, vtx_hash);
        }
    }

    QTime time_end = QTime::currentTime();
    int msecs = time_begin.msecsTo(time_end);
    qDebug("construct base mesh need %d msecs", msecs);

    return true;

}

void HalfEdge::redefine_vtx_index() {
    Vertex * v = get_vertex_first();
    int i = 1;
    while (v != get_vertex_front()) {
        v->v_index = i;
        ++i;
        v = v->next;
    }
}

bool HalfEdge::write_stb_mesh(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
        QTextStream out(&file);
        Vertex * vtx = get_vertex_first();
        while (vtx != get_vertex_front()) {
            out << "v " << QString::number(vtx->coordinate.x()) << " "
                << QString::number(vtx->coordinate.y()) << " " << QString::number(vtx->coordinate.z()) << endl;
            vtx = vtx->next;
        }
        Face * face = get_face_first();
        while (face != get_face_front()) {
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

void HalfEdge::add_face_when_queue_is_null(QQueue<Edge *> &edge_queue, QVector<unsigned int> &vi_array, QHash<int, Vertex *> &vtx_hash)
{
    Face * first_face = new Face();

    Vertex * v1 = vtx_hash.value(vi_array.at(0));
    Vertex * v2 = vtx_hash.value(vi_array.at(1));
    Vertex * v3 = vtx_hash.value(vi_array.at(2));

    Edge * e1 = new Edge(v1, v2);
    Edge * e2 = new Edge(v2, v3);
    Edge * e3 = new Edge(v3, v1);
    e1->add_edge_to_vtx_list();
    e2->add_edge_to_vtx_list();
    e3->add_edge_to_vtx_list();

    vi_array.pop_front();
    vi_array.pop_front();
    vi_array.pop_front();
    e1->next = e2;
    e2->next = e3;
    e3->next = e1;
    e1->prev = e3;
    e2->prev = e1;
    e3->prev = e2;
    e1->face = first_face;
    e2->face = first_face;
    e3->face = first_face;
    first_face->e1 = e1;
    first_face->e2 = e2;
    first_face->e3 = e3;
    add_face(first_face);

    edge_queue.enqueue(e1);
    edge_queue.enqueue(e2);
    edge_queue.enqueue(e3);


}

bool HalfEdge::do_with_reverse(QQueue<Edge *> &edge_queue, Edge *&edge, QVector<unsigned int> &vi_array, QHash<int, Vertex *> &vtx_hash)
{
    if (!search_in_queue(edge_queue, edge)) {
        Face * new_face = search_in_viarray(edge_queue, edge, vi_array, vtx_hash);
        if (new_face != NULL) {
            add_face(new_face);
        }
    }
    return true;
}

Face * HalfEdge::search_in_viarray(QQueue<Edge *> &edge_queue, Edge *&edge, QVector<unsigned int> &vi_array, QHash<int, Vertex *> &vtx_hash)
{
    Face * new_face = NULL;
    int i=0;
    while (!vi_array.isEmpty() && i<vi_array.size()) {
        new_face = search_in_viarray_per(edge_queue, edge, vi_array, i, vtx_hash);
        if (new_face != NULL) {

            //qDebug() << vi_array.at(i) << " " << vi_array.at(i+1) << " " << vi_array.at(i+2);
            vi_array.removeAt(i);
            vi_array.removeAt(i);
            vi_array.removeAt(i);
            break;
        }
        i += 3;
    }
    return new_face;

}

Face * HalfEdge::search_in_viarray_per(QQueue<Edge *> &edge_queue, Edge *&edge, QVector<unsigned int> &vi_array, int i, QHash<int, Vertex *> &vtx_hash)
{
    Face * new_face = NULL;
    unsigned int x = vi_array.at(i);
    unsigned int y = vi_array.at(i+1);
    unsigned int z = vi_array.at(i+2);


    if ((edge->getX()==x&&edge->getY()==y) || (edge->getY()==x&&edge->getX()==y)) {
        Edge * e1 = new Edge(edge->vertex2, edge->vertex1);
        Vertex * vertex = vtx_hash.value(z);
        //Vertex * vertex = vtx_list.at(z);
        Edge * e2 = new Edge(e1->vertex2, vertex);
        Edge * e3 = new Edge(vertex, e1->vertex1);

        e1->add_edge_to_vtx_list();
        e2->add_edge_to_vtx_list();
        e3->add_edge_to_vtx_list();

        e1->reverse = edge;
        e1->reverse->reverse = e1;
        e1->next = e2;
        e2->next = e3;
        e3->next = e1;
        e1->prev = e3;
        e2->prev = e1;
        e3->prev = e2;
        new_face = new Face();
        new_face->e1 = e1;
        new_face->e2 = e2;
        new_face->e3 = e3;
        e1->face = new_face;
        e2->face = new_face;
        e3->face = new_face;
        edge_queue.enqueue(new_face->e2);
        edge_queue.enqueue(new_face->e3);
        return new_face;
    }

    if ((edge->getX()==y && edge->getY()==z) || (edge->getY()==y && edge->getX()==z)) {
        Edge * e1 = new Edge(edge->vertex2, edge->vertex1);
        Vertex * vertex = vtx_hash.value(x);
//        Vertex * vertex = vtx_list.at(x);
        Edge * e2 = new Edge(e1->vertex2, vertex);
        Edge * e3 = new Edge(vertex, e1->vertex1);

        e1->add_edge_to_vtx_list();
        e2->add_edge_to_vtx_list();
        e3->add_edge_to_vtx_list();

        e1->reverse = edge;
        e1->reverse->reverse = e1;
        e1->next = e2;
        e2->next = e3;
        e3->next = e1;
        e1->prev = e3;
        e2->prev = e1;
        e3->prev = e2;
        new_face = new Face();
        new_face->e1 = e1;
        new_face->e2 = e2;
        new_face->e3 = e3;
        e1->face = new_face;
        e2->face = new_face;
        e3->face = new_face;
        edge_queue.enqueue(new_face->e2);
        edge_queue.enqueue(new_face->e3);
        return new_face;
    }

    if ((edge->getX()==z && edge->getY()==x) || (edge->getY()==z && edge->getX()==x)) {
        Edge * e1 = new Edge(edge->vertex2, edge->vertex1);
//        Vertex * vertex = vtx_list.at(y);
        Vertex * vertex = vtx_hash.value(y);
        Edge * e2 = new Edge(e1->vertex2, vertex);
        Edge * e3 = new Edge(vertex, e1->vertex1);

        e1->add_edge_to_vtx_list();
        e2->add_edge_to_vtx_list();
        e3->add_edge_to_vtx_list();

        e1->reverse = edge;
        e1->reverse->reverse = e1;
        e1->next = e2;
        e2->next = e3;
        e3->next = e1;
        e1->prev = e3;
        e2->prev = e1;
        e3->prev = e2;
        new_face = new Face();
        new_face->e1 = e1;
        new_face->e2 = e2;
        new_face->e3 = e3;
        e1->face = new_face;
        e2->face = new_face;
        e3->face = new_face;
        edge_queue.enqueue(new_face->e2);
        edge_queue.enqueue(new_face->e3);
        return new_face;
    }

    return NULL;

}

bool HalfEdge::construct_next_distance(QString file_name, int &times)
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

    QString name_test;
    name_test += name + "_";
    name_test.append(QString::number(times+1));
    QString path_test;
    path_test.append(path);
    path_test.append(name_test);
    path_test.append(".err");

    QString name_distance;
    name_distance += name + "_stb_";
    name_distance.append(QString::number(times));
    QString path_distance;
    path_distance.append(path);
    path_distance.append(name_distance);
    path_distance.append(".distance");

    QFile file_test(path_test);
    if (!file_test.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "*** Can not open the file! File location is \""
                 << path_test << "\" ***" << endl;
        return false;
    }

//    qDebug() << path;
//    qDebug() << name;
//    qDebug() << suffix;

    name.append("_");
    name.append(QString::number(times));
    path.append(name);

    QString path_error;
    path_error += path + ".err";

    qDebug() << "path_error: " << path_error;
    qDebug() << "times: " << times;


    QFile file(path_error);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "*** Can not open the file! File location is \""
                 << file_name << "\" ***" << endl;
        return false;
    }

    /// ---- time begin
    QDateTime time_begin = QDateTime::currentDateTime();
    qDebug() << time_begin;

    QTime begin_time = QTime::currentTime();

    QTextStream infile(&file);
    QVector<Distance *> d_list;
    while (!infile.atEnd()) {
        QString line = infile.readLine();
        QTextStream sin(&line);
        unsigned int v_index;
        unsigned int a, b;
        GLfloat x, y, z;
        sin >> v_index >> a >> b >> x >> y >> z;
        // -----------------------
        Distance * d = new Distance(v_index, a, b, x, y, z);
        d_list.push_back(d);
    }

    file.close();
//    qDebug() << "d_list_size: " << d_list.size();
//    for (int i=0; i<5; i++) {
//        qDebug() << "d:" << d_list.at(i)->v_index << d_list.at(i)->a << d_list.at(i)->b;
//    }

    // --- dispose edges:
    //// add error informations to edge
    Face * face_current = get_face_first();
    while (face_current != get_face_front()) {
//        for (int i=0; i<d_list; ++i) {
//            if ((face_current->e1->vertex1->v_index==d_list.at(i)->a &&
//                 face_current->e1->vertex1->v_index==d_list.at(i)->b) ||
//                    (face_current->e1->vertex1->v_index==d_list.at(i)->b &&
//                     face_current->e1->vertex2->v_index==d_list.at(i)->a)) {
//                face_current->e1->error = d_list.at(i)->distance;
//            }
//        }
        set_distance(face_current->e1, d_list);
        set_distance(face_current->e2, d_list);
        set_distance(face_current->e3, d_list);

        face_current = face_current->next;
    }

    QDateTime time_end = QDateTime::currentDateTime();
    QTime end_time = QTime::currentTime();

    int time_sum = time_begin.secsTo(time_end);
    int time_sum_msec = begin_time.msecsTo(end_time);

    write_stb_distance(path_distance);

    time_begin = QDateTime::currentDateTime();
    begin_time = QTime::currentTime();
    subdivide_cnstr();
    time_end = QDateTime::currentDateTime();
    end_time = QTime::currentTime();
    time_sum = time_sum + time_begin.secsTo(time_end);
    time_sum_msec = time_sum_msec + begin_time.msecsTo(end_time);
    qDebug( "There are %d seconds to next", time_sum);
    qDebug("construct next mesh need %d msces", time_sum_msec);
    d_list.clear();

    /// time end

//    if (times == 0) {

//    }



    return true;
}

void HalfEdge::set_distance(Edge *&e, QVector<Distance *> &d_list)
{
    for (int i=0; i<d_list.size(); ++i) {
        if ((e->vertex1->v_index==d_list.at(i)->a &&
             e->vertex2->v_index==d_list.at(i)->b) ||
                (e->vertex1->v_index==d_list.at(i)->b &&
                 e->vertex2->v_index==d_list.at(i)->a)) {
            e->error = d_list.at(i)->distance;
            e->v_index_rvsbd = d_list.at(i)->v_index;
        }
    }

}

void HalfEdge::subdivide_cnstr()
{
    if (is_empty()) {
        return;
    }

    clear_edges_of_vertex();

    Face * current = get_face_first();
    int origin_size = get_face_size();

    for (int i=0; i<origin_size; ++i) {
        edge_subdivide_cnstr(current->e1);
        edge_subdivide_cnstr(current->e2);
        edge_subdivide_cnstr(current->e3);

        face_subdivide_to_four(current);

        current = current->next;
    }

    for (int i=0; i<origin_size; ++i) {
        delete_face_front();
    }

}

void HalfEdge::edge_subdivide_cnstr(Edge *&current)
{
    if (current->flag == UNSUBDIVIDED) {
        Vertex *v1, *v2, *v3, *v4;
        v1 = current->vertex1;
        v2 = current->vertex2;
        v3 = current->next->vertex2;

        Edge * edge_rev = current->reverse;
        if (edge_rev != NULL) {
            v4 = edge_rev->next->vertex2;
        }
        else {
            v4 = new Vertex(0.0f, 0.0f, 0.0f);
        }

        QVector3D v_new_coordinate;
        if (v4->isZero()) {
            v_new_coordinate = (v1->coordinate + v2->coordinate) / 2;
        }
        else {
            v_new_coordinate = (3*(v1->coordinate+v2->coordinate) + v3->coordinate + v4->coordinate) / 8;
        }

//        qDebug() << "predict: " << v_new_coordinate.x() << " "
//                 << v_new_coordinate.y() << " " << v_new_coordinate.z();


//        qDebug() << "error: " << current->error.x() << " "
//                 << current->error.y() << " " << current->error.z();

        v_new_coordinate += current->error;

//        qDebug() << "back: " << v_new_coordinate.x() << " "
//                 << v_new_coordinate.y() << " " << v_new_coordinate.z();

        Vertex * vtx_new = new Vertex(v_new_coordinate);
        vtx_new->v_index = current->v_index_rvsbd;
        add_vertex(vtx_new);
        current->child1 = new Edge(current->vertex1, vtx_new);
        current->child2 = new Edge(vtx_new, current->vertex2);
        current->vtx_in = vtx_new;
        current->flag = SUBDIVIDED;

        if (current->reverse != NULL) {
            current->reverse->child1 = new Edge(current->reverse->vertex1, vtx_new);
            current->reverse->child2 = new Edge(vtx_new, current->reverse->vertex2);
            current->child1->reverse = current->reverse->child2;
            current->reverse->child2->reverse = current->child1;
            current->child2->reverse = current->reverse->child1;
            current->reverse->child1->reverse = current->child2;
            current->reverse->vtx_in = vtx_new;
            current->reverse->flag = SUBDIVIDED;
        }
    }

}

bool HalfEdge::write_stb_distance(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
        QTextStream out(&file);
        Face * face = get_face_first();
        while (face != get_face_front()) {
            if (face->e1->outputed == UNOUTPUTED) {
                out << face->e1->error.x() << " " << face->e1->error.y() << " " << face->e1->error.z() << endl;
                face->e1->outputed = OUTPUTED;
                if (face->e1->reverse != NULL) {
                    face->e1->reverse->outputed = OUTPUTED;
                }
            }
            if (face->e2->outputed == UNOUTPUTED) {
                out << face->e2->error.x() << " " << face->e2->error.y() << " " << face->e2->error.z() << endl;
                face->e2->outputed = OUTPUTED;
                if (face->e2->reverse != NULL) {
                    face->e2->reverse->outputed = OUTPUTED;
                }
            }
            if (face->e3->outputed == UNOUTPUTED) {
                out << face->e3->error.x() << " " << face->e3->error.y() << " " << face->e3->error.z() << endl;
                face->e3->outputed = OUTPUTED;
                if (face->e3->reverse != NULL) {
                    face->e3->reverse->outputed = OUTPUTED;
                }
            }
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

bool HalfEdge::load_next(QString file_name, int &times)
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


    name.append("_stb_");
    name.append(QString::number(times));
    path.append(name);

    QString path_distance;
    path_distance += path + ".distance";

    qDebug() << "path_distance: " << path_distance;
    qDebug() << "times: " << times;


    /// ---- time begin
    QDateTime time_begin = QDateTime::currentDateTime();
    qDebug() << time_begin;

    QTime begin_time = QTime::currentTime();
    QFile file(path_distance);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "*** Can not open the file! File location is \""
                 << file_name << "\" ***" << endl;
        return false;
    }

    QTextStream infile(&file);
    QQueue<QVector3D> d_list;
    while (!infile.atEnd()) {
        QString line = infile.readLine();
        QTextStream sin(&line);
        GLfloat x, y, z;
        sin >> x >> y >> z;
        // -----------------------
        QVector3D d(x, y, z);
        d_list.push_back(d);
    }

    file.close();
//    qDebug() << "d_list_size: " << d_list.size();
//    for (int i=0; i<5; i++) {
//        qDebug() << "d:" << d_list.at(i)->v_index << d_list.at(i)->a << d_list.at(i)->b;
//    }

    // --- dispose edges:
    //// add error informations to edge
    Face * face_current = get_face_first();
    while (face_current != get_face_front()) {
        set_distance(face_current->e1, d_list);
        set_distance(face_current->e2, d_list);
        set_distance(face_current->e3, d_list);

        face_current = face_current->next;
    }


    subdivide_cnstr();

    d_list.clear();

    QDateTime time_end = QDateTime::currentDateTime();
    QTime end_time = QTime::currentTime();

    int time_sum = time_begin.secsTo(time_end);
    int time_sum_msec = begin_time.msecsTo(end_time);

    qDebug("construct next level need %d secs (%d msecs)", time_sum, time_sum_msec);

    redefine_vtx_index();

    return true;
}

void HalfEdge::set_distance(Edge *&edge, QQueue<QVector3D> &d_list)
{
    if (edge->loaded == UNLOADED) {
        edge->error = d_list.dequeue();
        edge->v_index_rvsbd = 0;
        edge->loaded = LOADED;
        if (edge->reverse != NULL) {
            edge->reverse->error = edge->error;
            edge->reverse->v_index_rvsbd = 0;
            edge->reverse->loaded = LOADED;
        }
    }

}

bool HalfEdge::zero_tree_compress(QString file_name, double x)
{
    x = 0.99;
    int a = file_name.lastIndexOf('/');
    QString path = file_name.mid(0, a+1);
    QString name = file_name.mid(a+1);
    int b = name.lastIndexOf('_');
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

    QString path_new;
    path_new += path + name + "_" + QString::number(x) + "/";
    QDir dir(path_new);
    if (!dir.exists()) {
        //qDebug() << "--------------------";
        if (!dir.mkdir(path_new))
            return false;

    }

//    qDebug() << name;

    QString file_name_0;
    file_name_0 += name + "_0.obj";
    QString file_to;
    file_to += path_new + file_name_0;
    //qDebug() << "file_source: " << file_name;
    //qDebug() << "file_to: " << file_to;
    if (!QFile::copy(file_name, file_to)) {
        qDebug() << "file: *** " << file_to << " *** already exist";
//        return false;
    }

    QString name_filter;
    name_filter += name + "_*.distance";

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

    if (string_list.isEmpty() || string_list.size()==0) {
        return false;
    }

    //qDebug() << "range: " << range;

    GLfloat epsilon = 0.0f;


    // --- copy to amother file
    for (int i=0; i<string_list.size(); ++i) {
//        qDebug() << string_list.at(i);
        QString distance_filesource;
        distance_filesource += path + name + "_" + QString::number(i) + ".distance";
//        qDebug() << distance_filesource;

        QFile file(distance_filesource);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "*** Can not open the file! File location is \""
                 << distance_filesource << "\" ***" << endl;
            return false;
        }

        QTextStream infile(&file);
        QQueue<QVector3D> d_list;
        while (!infile.atEnd()) {
            QString line = infile.readLine();
            QTextStream sin(&line);
            GLfloat x, y, z;
            sin >> x >> y >> z;
            // -----------------------
            QVector3D d(x, y, z);
            d_list.push_back(d);
        }

        file.close();

        if (epsilon == 0.0f) {
            epsilon = mid(d_list);
            qDebug() << "epsilon: " << epsilon;
        }
        else {
            epsilon = epsilon / 2;
        }

        // --------------------------------
        add_distances_zt(d_list, epsilon);


        QString distance_fileto;
        distance_fileto += path_new + name + "_" + QString::number(i) + ".distance";
//        qDebug() << distance_fileto;
//        if (!QFile::copy(distance_filesource, distance_fileto)) {
//            qDebug() << "file: *** " << distance_fileto << " *** already exist";
//        }

        write_stb_distance(distance_fileto);

        // --- next level subdivision
        subdivide_zero_tree();


    }

    return true;
}

GLfloat HalfEdge::max(QQueue<QVector3D> &d_list)
{
    GLfloat a = 0.0f;
    for (int i=0; i<d_list.size(); ++i) {
        if (a < qAbs(d_list.at(i).x())) {
            a = qAbs(d_list.at(i).x());
        }
        if (a < qAbs(d_list.at(i).y())) {
            a = qAbs(d_list.at(i).y());
        }
        if (a < qAbs(d_list.at(i).z())) {
            a = qAbs(d_list.at(i).z());
        }
    }

    qDebug() << "a : " << a;

    return a;
}

GLfloat HalfEdge::mid(QQueue<QVector3D> &d_list)
{
    QVector<GLfloat> list;
    for (int i=0; i<d_list.size(); ++i) {
        list.push_back(d_list.at(i).x());
        list.push_back(d_list.at(i).y());
        list.push_back(d_list.at(i).z());
    }
    qSort(list.begin(), list.end());



    int a = d_list.size()*3;

//    for (int i=0; i<a; ++i) {
//        qDebug() << "i: " << list.at(i);
//    }

//    qDebug() << "a: " << a;
    return qAbs(list.at(a/16));
}

void HalfEdge::add_distances_zt(QQueue<QVector3D> &d_list, GLfloat epsilon)
{
    Face * face_current = get_face_first();
    while (face_current != get_face_front()) {
        set_distance_zt(face_current->e1, d_list, epsilon);
        set_distance_zt(face_current->e2, d_list, epsilon);
        set_distance_zt(face_current->e3, d_list, epsilon);

        face_current = face_current->next;
    }

}

void HalfEdge::set_distance_zt(Edge *&edge, QQueue<QVector3D> &d_list, GLfloat epsilon)
{
    if (edge->loaded == UNLOADED) {
        if (edge->flag_zero_tree == SETTED) {
            d_list.dequeue();
        }
        else {
            edge->error = d_list.dequeue();
            if (qAbs(edge->error.x())<epsilon && qAbs(edge->error.y())<epsilon && qAbs(edge->error.z())<epsilon) {
                edge->error.setX(0.0f);
                edge->error.setY(0.0f);
                edge->error.setZ(0.0f);
            }
        }
        edge->v_index_rvsbd = 0;
        edge->loaded = LOADED;
        if (edge->reverse != NULL) {
            edge->reverse->error = edge->error;
            edge->reverse->v_index_rvsbd = 0;
            edge->reverse->loaded = LOADED;
        }
    }
}

void HalfEdge::subdivide_zero_tree()
{
    clear_edges_of_vertex();

    Face * current = get_face_first();

    int origin_size = get_face_size();

    for (int i=0; i<origin_size; ++i) {
        edge_subdivide_cnstr(current->e1);
        edge_subdivide_cnstr(current->e2);
        edge_subdivide_cnstr(current->e3);

        face_subdivide_to_four(current);

        current = current->next;
    }


    current = get_face_first();
    for (int i=0; i<origin_size; ++i) {
        if (current->e1->is_zero()) {
            current->e1->child1->flag_zero_tree = SETTED;
            current->e1->child2->flag_zero_tree = SETTED;
            current->e1->child1->next->reverse->prev->flag_zero_tree = SETTED;
            current->e1->child1->next->reverse->prev->reverse->flag_zero_tree = SETTED;
            if (current->e1->reverse != NULL) {
                current->e1->reverse->child1->flag_zero_tree = SETTED;
                current->e1->reverse->child2->flag_zero_tree = SETTED;
                current->e1->reverse->child1->next->reverse->prev->flag_zero_tree = SETTED;
                current->e1->reverse->child1->next->reverse->prev->reverse->flag_zero_tree = SETTED;
            }
        }

        if (current->e2->is_zero()) {
            current->e2->child1->flag_zero_tree = SETTED;
            current->e2->child2->flag_zero_tree = SETTED;
            current->e2->child1->next->reverse->prev->flag_zero_tree = SETTED;
            current->e2->child1->next->reverse->prev->reverse->flag_zero_tree = SETTED;
            if (current->e2->reverse != NULL) {
                current->e2->reverse->child1->flag_zero_tree = SETTED;
                current->e2->reverse->child2->flag_zero_tree = SETTED;
                current->e2->reverse->child1->next->reverse->prev->flag_zero_tree = SETTED;
                current->e2->reverse->child1->next->reverse->prev->reverse->flag_zero_tree = SETTED;
            }
        }

        if (current->e3->is_zero()) {
            current->e3->child1->flag_zero_tree = SETTED;
            current->e3->child2->flag_zero_tree = SETTED;
            current->e3->child1->next->reverse->prev->flag_zero_tree = SETTED;
            current->e3->child1->next->reverse->prev->reverse->flag_zero_tree = SETTED;
            if (current->e3->reverse != NULL) {
                current->e3->reverse->child1->flag_zero_tree = SETTED;
                current->e3->reverse->child2->flag_zero_tree = SETTED;
                current->e3->reverse->child1->next->reverse->prev->flag_zero_tree = SETTED;
                current->e3->reverse->child1->next->reverse->prev->reverse->flag_zero_tree = SETTED;
            }
        }

        current = current->next;
    }

    for (int i=0; i<origin_size; ++i) {
        delete_face_front();
    }

}


bool HalfEdge::save_as_ply(QString path) const
{
    QFile file(path);
    if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
        QTextStream out(&file);

        out << "ply" << endl;
        out << "format ascii 1.0" << endl;
        out << "comment generated by xujy" << endl;
        out << "element vertex " << get_vertex_size() << endl;
        out << "property float x" << endl;
        out << "property float y" << endl;
        out << "property float z" << endl;
        out << "element face " << get_face_size() << endl;
        out << "property list uchar int vertex_indices" << endl;
        out << "end_header" << endl;

        Vertex * vtx = get_vertex_first();
        while (vtx != get_vertex_front()) {
            out << vtx->coordinate.x() << " " << vtx->coordinate.y() << " " <<
                   vtx->coordinate.z() << endl;

            vtx = vtx->next;
        }

        Face * face = get_face_first();
        while (face != get_face_front()) {
            out << "3 "
                << face->e1->getX()-1 << " "
                << face->e2->getX()-1 << " "
                << face->e3->getX()-1 << endl;

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
