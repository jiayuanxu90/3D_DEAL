#include "halfedge.h"
#include <QVector2D>
#include <QQueue>
#include <QDebug>
#include <new>





HalfEdge::HalfEdge()
{
    front = new Face();
    front->next = front;
    front->prev = front;
    size = 0;
}

HalfEdge::~HalfEdge()
{
    while (size > 0) {
        Face * temp_face = front->next;
        front->next = front->next->next;
        front->next->prev = front;
        delete temp_face;
        size--;
    }
    front = NULL;
}


Face * HalfEdge::get_front() const
{
    return front;
}

Face * HalfEdge::get_first() const
{
    return front->next;
}

int HalfEdge::get_size() const
{
    return size;
}

bool HalfEdge::is_empty() const
{
    return (size==0);
}

bool HalfEdge::construct_halfedge_sturcture(QVector<unsigned int> vi_array)
{

    QQueue<Edge *> edge_queue;

    add_face_when_queue_is_null(edge_queue, vi_array);

    Edge * edge;
    while ((!edge_queue.isEmpty()) || vi_array.size()>0) {
        qDebug() << "EDGE_QUEUE_SIZE: " << edge_queue.size();
        if (edge_queue.isEmpty()) {
            add_face_when_queue_is_null(edge_queue, vi_array);
        }
        else {
            edge = edge_queue.dequeue();
            do_with_reverse(edge_queue, edge, vi_array);
        }
    }


    return true;
}

bool HalfEdge::do_with_reverse(QQueue<Edge *> &edge_queue, Edge * &edge, QVector<unsigned int> & vi_array)
{
    if (!search_in_queue(edge_queue, edge)) {
        Face * new_face = search_in_viarray(edge_queue, edge, vi_array);
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
        if (edge_queue.at(i)->data.x()==edge->data.y() &&
                edge_queue.at(i)->data.y()==edge->data.x()) {
            edge->reverse = edge_queue.at(i);
            edge_queue.at(i)->reverse = edge;
            edge_queue.removeAt(i);
            flag = true;
            break;
        }
    }
    return flag;
}

Face * HalfEdge::search_in_viarray(QQueue<Edge *> &edge_queue, Edge *&edge, QVector<unsigned int> &vi_array)
{
    Face * new_face = NULL;
    int i=0;
    while (!vi_array.isEmpty() && i<vi_array.size()) {
        new_face = search_in_viarray_per(edge_queue, edge, vi_array, i);
        if (new_face != NULL) {
            vi_array.removeAt(i);
            vi_array.removeAt(i);
            vi_array.removeAt(i);
            break;
        }
        i += 3;
    }
    return new_face;

}

Face * HalfEdge::search_in_viarray_per(QQueue<Edge *> &edge_queue, Edge *&edge, QVector<unsigned int> &vi_array, int i)
{
    Face * new_face = NULL;
    unsigned int x = vi_array.at(i);
    unsigned int y = vi_array.at(i+1);
    unsigned int z = vi_array.at(i+2);


    if ((edge->data.x()==x&&edge->data.y()==y) || (edge->data.y()==x&&edge->data.x()==y)) {
        Edge * e1 = new Edge(QVector2D(edge->data.y(), edge->data.x()));
        Edge * e2 = new Edge(QVector2D(e1->data.y(), z));
        Edge * e3 = new Edge(QVector2D(z, e1->data.x()));
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
        edge_queue.enqueue(e2);
        edge_queue.enqueue(e3);
    }

    if ((edge->data.x()==y && edge->data.y()==z) || (edge->data.y()==y && edge->data.x()==z)) {
        Edge * e1 = new Edge(QVector2D(edge->data.y(), edge->data.x()));
        Edge * e2 = new Edge(QVector2D(e1->data.y(), x));
        Edge * e3 = new Edge(QVector2D(x, e1->data.x()));
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
        edge_queue.enqueue(e2);
        edge_queue.enqueue(e3);
    }

    if ((edge->data.x()==x && edge->data.y()==z) || (edge->data.y()==x && edge->data.x()==z)) {
        Edge * e1 = new Edge(QVector2D(edge->data.y(), edge->data.x()));
        Edge * e2 = new Edge(QVector2D(e1->data.y(), y));
        Edge * e3 = new Edge(QVector2D(y, e1->data.x()));
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
        edge_queue.enqueue(e2);
        edge_queue.enqueue(e3);
    }

    return new_face;
}


bool HalfEdge::add_face(Face * &face)
{
    if (face == NULL) {
        return false;
    }
    else {
        face->prev = front->prev;
        face->next = front;
        front->prev->next = face;
        front->prev = face;
        size++;
        return true;
    }

}

void HalfEdge::add_face_when_queue_is_null(QQueue<Edge *> &edge_queue, QVector<unsigned int> &vi_array)
{
    Face * first_face = new Face();
    Edge * e1 = new Edge(QVector2D(vi_array.at(0), vi_array.at(1)));
    Edge * e2 = new Edge(QVector2D(vi_array.at(1), vi_array.at(2)));
    Edge * e3 = new Edge(QVector2D(vi_array.at(2), vi_array.at(0)));
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
    if (size == 0) {
        return false;
    }
    else {
        Face * temp_face = front->next;
        front->next = front->next->next;
        front->next->prev = front;
        delete temp_face;
        size--;
        return true;
    }
}

void HalfEdge::print_for_debug() const
{
//    Face * current = front->next;
//    for (int i=0; i<size; i++) {
//        current->print();
//        current = current->next;
//    }

}



























