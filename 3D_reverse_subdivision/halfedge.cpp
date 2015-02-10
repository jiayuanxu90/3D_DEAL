#include "halfedge.h"
#include <QVector2D>
#include <QQueue>
#include <QDebug>
//#include <new>





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
}

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



Vertex * HalfEdge::get_vertex_extraordinary()
{
    Vertex * vtx = NULL;
    Vertex * current = get_vertex_first();
    while (current != get_vertex_front()) {
        if (current->edge_list_in.size()!=6 && current->edge_list_out.size()!=6 && (!current->isParted())) {
            vtx = current;
            qDebug() << "current vtx edge_list size in: " << vtx->edge_list_in.size() << ", out: " <<
                        vtx->edge_list_out.size();
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

bool HalfEdge::construct_halfedge_sturcture(QVector<unsigned int> &vi_array, QVector<Vertex *> &vtx_list)
{

    QQueue<Edge *> edge_queue;
//    for (int i=0; i<v_size; ++i) {
//        vtx_ptr_list.push_back(NULL);
//    }

    add_face_when_queue_is_null(edge_queue, vi_array, vtx_list);

    Edge * edge;
    while ((!edge_queue.isEmpty()) || vi_array.size()>0) {
        if (edge_queue.isEmpty()) {
            add_face_when_queue_is_null(edge_queue, vi_array, vtx_list);
        }
        else {
            edge = edge_queue.dequeue();
            //qDebug() << edge->vertex1->v_index << ", " << edge->vertex2->v_index;
            do_with_reverse(edge_queue, edge, vi_array, vtx_list);
        }
    }


//    Vertex * vtx_current = get_vertex_first();
//    while (vtx_current != get_vertex_front()) {
//        //vtx_current->edge_list_in.clear();
//        qDebug() << vtx_current->v_index << "  in size: " << vtx_current->edge_list_in.size();
//        //vtx_current->edge_list_out.clear();
//        qDebug() << vtx_current->v_index << "  out size: " << vtx_current->edge_list_out.size();
//        vtx_current = vtx_current->next;
//    }

    return true;
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

//Vertex * HalfEdge::search_in_queue(QQueue<Edge *> &edge_queue, unsigned int x, unsigned int y)
//{
//    Vertex * vertex = NULL;
//    for (int i=0; i<edge_queue.size(); i++) {
//        if (edge_queue.at(i)->getX()==y &&
//                edge_queue.at(i)->getY()==x) {
//            vertex = edge_queue.at(i)->vertex1;
//            break;
//        }
//    }
//    return vertex;
//}

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

bool HalfEdge::delete_face_front()
{
    if (face_size == 0) {
        return false;
    }
    else {
        Face * temp_face = face_front->next;
        face_front->next = face_front->next->next;
        face_front->next->prev = face_front;
        delete temp_face;
        face_size--;
        return true;
    }
}

//Vertex * HalfEdge::get_vertex(unsigned int vi)
//{
//    Vertex * vertex = NULL;
//    if (vtx_ptr_list.at(vi) == NULL) {
//        vertex = new Vertex(vi);
//        vtx_ptr_list[vi] = vertex;
////        vertex_list.push_back(vertex);
//    }

//    vertex = vtx_ptr_list.at(vi);
//    return vertex;
//}

void HalfEdge::print_for_debug() const
{
//    for (int i=1; i<vtx_ptr_list.size(); ++i) {
//        if (!vtx_ptr_list.at(i)->edge_list.isEmpty()) {
//            qDebug() << vtx_ptr_list.at(i)->edge_list.size();
//        }
//    }
}


void HalfEdge::clear_edges_of_vertex()
{
    Vertex * vtx_current = get_vertex_first();
    while (vtx_current != get_vertex_front()) {
        vtx_current->edge_list_in.clear();
//        qDebug() << vtx_current->v_index << "  in size: " << vtx_current->edge_list_in.size();
        vtx_current->edge_list_out.clear();
//        qDebug() << vtx_current->v_index << "  out size: " << vtx_current->edge_list_out.size();
        vtx_current = vtx_current->next;
    }

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

void HalfEdge::update_vtx_index()
{
    Vertex * vtx_current = get_vertex_first();
    unsigned int i = 1;
    while (vtx_current != get_vertex_front()) {
        vtx_current->v_index = i;
        ++i;
        vtx_current = vtx_current->next;
    }
}






















