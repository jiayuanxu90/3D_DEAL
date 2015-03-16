#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QDebug>
#include <QtOpenGL>

#define UNSUBDIVIDED 0
#define SUBDIVIDED 1

#define UNPARTED 2
#define OOD 3
#define EVEN 4

#define OUTPUTED 5
#define UNOUTPUTED 6
//#define IS_SINAR 5
//#define IS_NOT_SIGULAR 6


class Face;
class Edge;

class Vertex {
public:
//    QVector<Edge *> edge_list;
    QVector<Edge *> edge_list_out;
    QVector<Edge *> edge_list_in;
    unsigned int v_index;
    QVector3D coordinate;
    QVector3D coordinate_normalized;
    Vertex * prev;
    Vertex * next;
//    unsigned int flag;
    unsigned int partion;

    Vertex() {
        v_index = 0;
        coordinate.setX(0.0f);
        coordinate.setY(0.0f);
        coordinate.setZ(0.0f);
        coordinate_normalized.setX(0.0f);
        coordinate_normalized.setY(0.0f);
        coordinate_normalized.setZ(0.0f);
        prev = NULL;
        next = NULL;
        partion = UNPARTED;
    }

    Vertex(const unsigned int v_index0) {
        v_index = v_index0;
        coordinate.setX(0.0f);
        coordinate.setY(0.0f);
        coordinate.setZ(0.0f);
        coordinate_normalized.setX(0.0f);
        coordinate_normalized.setY(0.0f);
        coordinate_normalized.setZ(0.0f);
        prev = NULL;
        next = NULL;
        partion = UNPARTED;
    }

    Vertex(const unsigned int v_index0, const GLfloat x, const GLfloat y, const GLfloat z) {
        v_index = v_index0;
        coordinate.setX(x);
        coordinate.setY(y);
        coordinate.setZ(z);
        coordinate_normalized.setX(0.0f);
        coordinate_normalized.setY(0.0f);
        coordinate_normalized.setZ(0.0f);
        prev = NULL;
        next = NULL;
        partion = UNPARTED;
    }

    Vertex(const GLfloat x, const GLfloat y, const GLfloat z) {
        coordinate.setX(x);
        coordinate.setY(y);
        coordinate.setZ(z);
        coordinate_normalized.setX(0.0f);
        coordinate_normalized.setY(0.0f);
        coordinate_normalized.setZ(0.0f);
        prev = NULL;
        next = NULL;
        partion = UNPARTED;
    }

    Vertex(const QVector3D coordinate0) {
        coordinate.setX(coordinate0.x());
        coordinate.setY(coordinate0.y());
        coordinate.setZ(coordinate0.z());
        coordinate_normalized.setX(0.0f);
        coordinate_normalized.setY(0.0f);
        coordinate_normalized.setZ(0.0f);
        prev = NULL;
        next = NULL;
        partion = UNPARTED;
    }

    Vertex(const Vertex & vertex) {
        v_index = vertex.v_index;
        edge_list_in = vertex.edge_list_in;
        edge_list_out = vertex.edge_list_out;
        coordinate = vertex.coordinate;
        coordinate_normalized = vertex.coordinate_normalized;
        prev = vertex.prev;
        next = vertex.next;
        partion = vertex.partion;
    }

    ~Vertex() {
        edge_list_in.clear();
        edge_list_out.clear();
        v_index = 0;
        coordinate.setX(0.0f);
        coordinate.setY(0.0f);
        coordinate.setZ(0.0f);
        coordinate_normalized.setX(0.0f);
        coordinate_normalized.setY(0.0f);
        coordinate_normalized.setZ(0.0f);
        prev = NULL;
        next = NULL;
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
        return (edge_list_in.size() != 6);
    }

//    bool addEdge(Edge * &edge) {
//        edge_list.push_back(edge);
//    }

    void add_edge_in(Edge * edge) {
        edge_list_in.push_back(edge);
    }

    void add_edge_out(Edge * edge) {
        edge_list_out.push_back(edge);
    }

    bool isEqual(Vertex * const &vertex) const {
        return (v_index == vertex->v_index);
    }

    void set_even() {
        partion = EVEN;
    }

    void set_ood() {
        partion = OOD;
    }

    void  set_unparted() {
        partion = UNPARTED;
    }

    bool isZero() const {
        return (coordinate.x()==0.0f && coordinate.y()==0.0f && coordinate.z()==0.0f);
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
    // when reverse-subdivide mesh, two edges will be merged to two, both child edges points to the origin edge.
    Edge * parent;
    QVector3D error;

    // 5. child edges
    // when buttrtfly subdivide mesh, the origin edge points to the two child edges.
    Edge * child1;
    Edge * child2;

    // --- flag whether the edge has been subdivided or not
    unsigned int flag;

    // --- to discribe whether the edge has been outputed
    unsigned int outputed;

    // --- if the edge is be subdivided, the new vertex will be insert into the vertex list, and
    // --- the vertex index will be added into the slot;
//    unsigned int vi;
    Vertex * vtx_in;

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
        outputed = UNOUTPUTED;
        face = NULL;
        parent = NULL;
        child1 = NULL;
        child2 = NULL;
        vtx_in = NULL;
        error.setX(0.0);
        error.setY(0.0);
        error.setZ(0.0);
    }
    Edge(Vertex * &v1, Vertex * &v2) {
        vertex1 = v1;
        vertex2 = v2;
        prev = NULL;
        next = NULL;
        reverse = NULL;
        flag = UNSUBDIVIDED;
        outputed = UNOUTPUTED;
        face = NULL;
        parent = NULL;
        child1 = NULL;
        child2 = NULL;
        vtx_in = NULL;
//        vertex1->edge_list.push_back(this);
        //vertex1->add_edge_out(this);
        //vertex2->add_edge_in(this);
        error.setX(0.0);
        error.setY(0.0);
        error.setZ(0.0);
    }
    Edge(const QVector2D data) {
        vertex1 = new Vertex(data.x());
        vertex2 = new Vertex(data.y());
        prev = NULL;
        next = NULL;
        reverse = NULL;
        flag = UNSUBDIVIDED;
        outputed = UNOUTPUTED;
        face = NULL;
        parent = NULL;
        child1 = NULL;
        child2 = NULL;
        vtx_in = NULL;
//        vertex1->edge_list.push_back(this);
        //vertex1->add_edge_out(this);
        //vertex2->add_edge_in(this);
        error.setX(0.0);
        error.setY(0.0);
        error.setZ(0.0);
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
        vtx_in = edge.vtx_in;
        error = edge.error;
    }
    // [End of Edge()]


    // ---
    // [Begin of ~Edge()]
    ~Edge() {
        prev = NULL;
        next = NULL;
        reverse = NULL;
        flag = UNSUBDIVIDED;
        outputed = UNOUTPUTED;
        face = NULL;
        parent = NULL;
        child1 = NULL;
        child2 = NULL;
        vtx_in = NULL;
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

    void add_edge_to_vtx_list() {
        vertex1->add_edge_out(this);
        vertex2->add_edge_in(this);
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

    Face(Edge * &e01, Edge * &e02, Edge * &e03) {
        e1 = e01;
        e2 = e02;
        e3 = e03;
        e1->next = e2;
        e2->next = e3;
        e3->next = e1;
        e1->prev = e3;
        e2->prev = e1;
        e3->prev = e2;
        e1->face = this;
        e2->face = this;
        e3->face = this;
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
    Face * face_front;
//    QVector<Vertex *> vtx_ptr_list;
    int face_size;
    Vertex * vtx_front;
    int vtx_size;


public:

    GLfloat x_trans;
    GLfloat y_trans;
    GLfloat z_trans;
    GLfloat range;
    int rs_times;

    HalfEdge();
    ~HalfEdge();

    Face * get_face_front() const;
    Face * get_face_first() const;
    Vertex * get_vertex_front() const;
    Vertex * get_vertex_first() const;
    Vertex * get_vertex_extraordinary();
    Vertex * get_unparted() const;
    int get_vertex_size() const;
    int get_face_size() const;
    bool is_empty() const;

    void set_poroperties();
    void normalize_for_paint();

    void clear_edges_of_vertex();

    bool construct_halfedge_sturcture(QVector<unsigned int> &vi_array, QVector<Vertex *> &vtx_list);
    bool add_face(Face *&face);
    bool add_vertex(Vertex * &vertex);
    Vertex * get_vertex(unsigned int vi);
    bool do_with_reverse(QQueue<Edge *> & edge_queue, Edge * &edge, QVector<unsigned int> & vi_array, QVector<Vertex *> &vtx_list);
    bool search_in_queue(QQueue<Edge *> &edge_queue, Edge * &edge);
//    Vertex * search_in_queue(QQueue<Edge *> &edge_queue, unsigned int x, unsigned int y);
    Face * search_in_viarray(QQueue<Edge *> &edge_queue, Edge * &edge, QVector<unsigned int> &vi_array, QVector<Vertex *> &vtx_list);
    Face * search_in_viarray_per(QQueue<Edge *> & edge_queue, Edge * &edge, QVector<unsigned int> &vi_array, int i, QVector<Vertex *> &vtx_list);
    void add_face_when_queue_is_null(QQueue<Edge *> &edge_queue, QVector<unsigned int> &vi_array, QVector<Vertex *> &vtx_list);
    bool delete_face_front();

    bool delete_ood_vtx();
    void update_vtx_index();

    void print_for_debug() const;
};

#endif // HALFEDGE_H
