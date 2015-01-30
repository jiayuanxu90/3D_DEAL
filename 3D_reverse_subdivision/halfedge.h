#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <QVector>
#include <QVector2D>
#include <QDebug>

#define UNSUBDIVIDED 0
#define SUBDIVIDED 1

#define UNPARTED 2
#define OOD 3
#define EVEN 4
//#define IS_SINAR 5
//#define IS_NOT_SIGULAR 6


class Face;
class Edge;

class Vertex {
public:
    QVector<Edge *> edge_list;
    unsigned int v_index;
//    unsigned int flag;
    unsigned int partion;

    Vertex() {
        v_index = 0;
//        flag = IS_NOT_SIGULAR;
        partion = UNPARTED;
    }

    Vertex(const unsigned int v_index0) {
        v_index = v_index0;
        partion = UNPARTED;
    }

    Vertex(const Vertex & vertex) {
        v_index = vertex.v_index;
        edge_list = vertex.edge_list;
        partion = vertex.partion;
    }

    ~Vertex() {
        edge_list.clear();
        v_index = 0;
        partion = UNPARTED;
    }

    bool isParted() const {
        if (partion == UNPARTED)
            return false;
        else
            return true;
    }

    bool isOod() const {
        return (partion == OOD);
    }

    bool isEven() const {
        return (partion == EVEN);
    }

    bool isSingular() const {
        return (edge_list.size() != 6);
    }

    bool addEdge(Edge * &edge) {
        edge_list.push_back(edge);
    }

    bool isEqual(Vertex * const &vertex) const {
        return (v_index == vertex->v_index);
    }
};

class Edge {
public:
    // data: a vector data.x()-->data.y()
    //QVector2D data;

    Vertex * vertex1;
    Vertex * vertex2;

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
        vertex1 = NULL;
        vertex2 = NULL;
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
    Edge(Vertex * &v1, Vertex * &v2) {
        vertex1 = v1;
        vertex2 = v2;
        prev = NULL;
        next = NULL;
        reverse = NULL;
        flag = UNSUBDIVIDED;
        face = NULL;
        parent = NULL;
        child1 = NULL;
        child2 = NULL;
        vi = 0;
        vertex1->edge_list.push_back(this);
    }
    Edge(const QVector2D data) {
        vertex1 = new Vertex(data.x());
        vertex2 = new Vertex(data.y());
        vertex1->edge_list.push_back(this);
    }

    Edge(const Edge & edge) {
        vertex1 = edge.vertex1;
        vertex2 = edge.vertex2;
        prev = edge.prev;
        next = edge.next;
        reverse = edge.reverse;
        flag = edge.flag;
        face = edge.face;
        parent = edge.parent;
        child1 = edge.child1;
        child2 = edge.child2;
        vi = edge.vi;
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
        return (vertex1->v_index==edge->getX() && vertex2->v_index==edge->getY());
    }
    // [End of isEqual()]

    // ---
    unsigned int getX() const {
        if (vertex1 == NULL)
            return 0;
        else
            return vertex1->v_index;
    }
    unsigned int getY() const {
        if (vertex2 == NULL)
            return 0;
        else
            return vertex2->v_index;
    }


    // ---
    // [Begin of print_for_debug()]
    void print_for_debug() const {
        qDebug() << "(" << getX() << ", " << getY() << ")" << endl;
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
        qDebug() << "e1: (" << e1->getX() << ", " << e1->getY() << ")" << endl
                 << "e2: (" << e2->getX() << ", " << e2->getY() << ")" << endl
                 << "e3: (" << e3->getX() << ", " << e3->getY() << ")" << endl;
    }
    // [End of print()]
};
class HalfEdge
{
    Face * front;
    QVector<Vertex *> vertex_list;
    int size;

public:
    HalfEdge();
    ~HalfEdge();

    Face * get_front() const;
    Face * get_first() const;
    int get_size() const;
    int get_vertex_size() const;
    int get_face_size() const;
    bool is_empty() const;

    bool construct_halfedge_sturcture(QVector<unsigned int> vi_array);
    bool add_face(Face *&face);
    bool add_vertex(Vertex * &vertex);
    bool do_with_reverse(QQueue<Edge *> & edge_queue, Edge * &edge, QVector<unsigned int> & vi_array);
    bool search_in_queue(QQueue<Edge *> &edge_queue, Edge * &edge);
    Face * search_in_viarray(QQueue<Edge *> &edge_queue, Edge * &edge, QVector<unsigned int> &vi_array);
    Face * search_in_viarray_per(QQueue<Edge *> & edge_queue, Edge * &edge, QVector<unsigned int> &vi_array, int i);
    void add_face_when_queue_is_null(QQueue<Edge *> &edge_queue, QVector<unsigned int> &vi_array);
    bool delete_face_front();

    void print_for_debug() const;
};

#endif // HALFEDGE_H
