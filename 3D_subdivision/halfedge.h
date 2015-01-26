#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <QVector>
#include <QVector2D>
#include <QDebug>

#define UNSUBDIVIDED 0
#define SUBDIVIDED 1


class Face;

class Edge {
public:
    // data: a vector data.x()-->data.y()
    QVector2D data;

    // Pointers
    // 1. point to prev
    Edge * prev;
    // 2. point to next
    Edge * next;
    // 3. point to reverse
    Edge * reverse;

    // 4. parent edge
    // when butterfly subdivide mesh, one edge will be divided to two, both parent edge points to the origin edge.
    Edge * parent;

    // 5. child edges
    // when buttrtfly subdivide mesh, the origin edge points to the two child edges.
    Edge * child1;
    Edge * child2;

    // --- flag whether the edge is be subdivided or not
    unsigned int flag;

    // --- if the edge is be subdivided, the new vertex will be insert into the vertex list, and
    // --- the vertex index will be added into the slot;
    unsigned int vi;

    // Pointer point to the Face
    Face * face;

    // ----
    // constructor of  Edge
    // initialize these properties
    // [begin of Edge()]
    Edge() {
        prev = NULL;
        next = NULL;
        reverse = NULL;
        flag = UNSUBDIVIDED;
        face = NULL;
        parent = NULL;
        child1 = NULL;
        child2 = NULL;
        vi = 0;
    }
    Edge(QVector2D d) {
        prev = NULL;
        next = NULL;
        reverse = NULL;
        flag = UNSUBDIVIDED;
        face = NULL;
        parent = NULL;
        child1 = NULL;
        child2 = NULL;
        vi = 0;
        data = d;
    }
    Edge(const Edge & edge) {
        data = edge.data;
        prev = edge.prev;
        next = edge.next;
        reverse = edge.reverse;
        flag = edge.flag;
        face = edge.face;
    }
    // [End of Edge()]


    // ---
    // [Begin of ~Edge()]
    ~Edge() {
        prev = NULL;
        next = NULL;
        reverse = NULL;
        flag = UNSUBDIVIDED;
        face = NULL;
        parent = NULL;
        child1 = NULL;
        child2 = NULL;
    }

    // [End of ~Edge()]

    // ---
    // [begin of isEqual()]
    bool isEqual(const Edge * & edge) {
        return (data.x()==edge->data.x() && data.y()==edge->data.y());
    }
    // [End of isEqual()]

    // ---
    // [Begin of print_for_debug()]
    void print_for_debug() const {
        qDebug() << "(" << data.x() << ", " << data.y() << ")" << endl;
    }
};

class Face {
public:
    Edge * e1;
    Edge * e2;
    Edge * e3;

    unsigned int flag;

    Face * prev;
    Face * next;

    // ---
    // constructor of Face
    // initialize these properties
    // [Begin of Face()]
    Face() {
        e1 = NULL;
        e2 = NULL;
        e3 = NULL;
        flag = UNSUBDIVIDED;
        prev = NULL;
        next = NULL;
    }
    // [End of Face()]

    // ---
    // [Begin of ~Face()]
    ~Face() {
        e1->~Edge();
        delete e1;
        e2->~Edge();
        delete e2;
        e3->~Edge();
        delete e3;
        e1 = NULL;
        e2 = NULL;
        e3 = NULL;
        prev = NULL;
        next = NULL;
    }
    // [End of ~Face()]

    // ---
    // [Begin of print()]
    void print() const {
        qDebug() << "e1: (" << e1->data.x() << ", " << e1->data.y() << ")" << endl
                 << "e2: (" << e2->data.x() << ", " << e2->data.y() << ")" << endl
                 << "e3: (" << e3->data.x() << ", " << e3->data.y() << ")" << endl;
    }
    // [End of print()]
};
class HalfEdge
{
    Face * front;
    int size;

public:
    HalfEdge();
    ~HalfEdge();

    Face * get_front() const;
    Face * get_first() const;
    int get_size() const;
    bool is_empty() const;

    bool construct_halfedge_sturcture(QVector<unsigned int> vi_array);
    bool add_face(Face *&face);
    bool do_with_reverse(QQueue<Edge *> & edge_queue, Edge * &edge, QVector<unsigned int> & vi_array);
    bool search_in_queue(QQueue<Edge *> &edge_queue, Edge * &edge);
    Face * search_in_viarray(QQueue<Edge *> &edge_queue, Edge * &edge, QVector<unsigned int> &vi_array);
    Face * search_in_viarray_per(QQueue<Edge *> & edge_queue, Edge * &edge, QVector<unsigned int> &vi_array, int i);
    void add_face_when_queue_is_null(QQueue<Edge *> &edge_queue, QVector<unsigned int> &vi_array);
    bool delete_face_front();

    void print_for_debug() const;
};

#endif // HALFEDGE_H
