#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QDebug>
#include <QtOpenGL>
#include <QTextStream>


#define UNSUBDIVIDED 0
#define SUBDIVIDED 1

#define UNPARTED 2
#define OOD 3
#define EVEN 4

#define OUTPUTED 5
#define UNOUTPUTED 6

#define LOADED 7
#define UNLOADED 8

#define SETTED 9
#define UNSETTED 10


class Face;
class Edge;

class Vertex {
public:
    QVector<Edge *> edge_list_out;
    QVector<Edge *> edge_list_in;
    unsigned int v_index;
    QVector3D coordinate;
    QVector3D coordinate_normalized;
    QVector3D normal_coordinate;
    QVector3D normal_coordinate_normalized;

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
        normal_coordinate.setX(0.0f);
        normal_coordinate.setY(0.0f);
        normal_coordinate.setZ(0.0f);
        normal_coordinate_normalized.setX(0.0f);
        normal_coordinate_normalized.setY(0.0f);
        normal_coordinate_normalized.setZ(0.0f);
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
        normal_coordinate.setX(0.0f);
        normal_coordinate.setY(0.0f);
        normal_coordinate.setZ(0.0f);
        normal_coordinate_normalized.setX(0.0f);
        normal_coordinate_normalized.setY(0.0f);
        normal_coordinate_normalized.setZ(0.0f);
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
        normal_coordinate.setX(0.0f);
        normal_coordinate.setY(0.0f);
        normal_coordinate.setZ(0.0f);
        normal_coordinate_normalized.setX(0.0f);
        normal_coordinate_normalized.setY(0.0f);
        normal_coordinate_normalized.setZ(0.0f);
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
        normal_coordinate.setX(0.0f);
        normal_coordinate.setY(0.0f);
        normal_coordinate.setZ(0.0f);
        normal_coordinate_normalized.setX(0.0f);
        normal_coordinate_normalized.setY(0.0f);
        normal_coordinate_normalized.setZ(0.0f);
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
        normal_coordinate.setX(0.0f);
        normal_coordinate.setY(0.0f);
        normal_coordinate.setZ(0.0f);
        normal_coordinate_normalized.setX(0.0f);
        normal_coordinate_normalized.setY(0.0f);
        normal_coordinate_normalized.setZ(0.0f);
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
        normal_coordinate = vertex.normal_coordinate;
        normal_coordinate_normalized = vertex.normal_coordinate_normalized;
//        prev = vertex.prev;
//        next = vertex.next;
        partion = vertex.partion;
    }

    Vertex & operator = (const Vertex &vertex) {
        v_index = vertex.v_index;
        edge_list_in = vertex.edge_list_in;
        edge_list_out = vertex.edge_list_out;
        coordinate = vertex.coordinate;
        coordinate_normalized = vertex.coordinate_normalized;
        normal_coordinate = vertex.normal_coordinate;
        normal_coordinate_normalized = vertex.normal_coordinate_normalized;
        prev = NULL;
        next = NULL;
        partion = vertex.partion;
        return *this;
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
        normal_coordinate.setX(0.0f);
        normal_coordinate.setY(0.0f);
        normal_coordinate.setZ(0.0f);
        normal_coordinate_normalized.setX(0.0f);
        normal_coordinate_normalized.setY(0.0f);
        normal_coordinate_normalized.setZ(0.0f);
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

    void print() const {
        qDebug() << "v: " << v_index;
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

    unsigned int v_index_rvsbd;

    // --- flag whether the edge has been subdivided or not
    unsigned int flag;

    // --- to discribe whether the edge has been outputed
    unsigned int outputed;

    unsigned int loaded;

    unsigned int flag_zero_tree;

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
        loaded = UNLOADED;
        flag_zero_tree = UNSETTED;
        face = NULL;
        parent = NULL;
        child1 = NULL;
        child2 = NULL;
        vtx_in = NULL;
        error.setX(0.0);
        error.setY(0.0);
        error.setZ(0.0);
        v_index_rvsbd = 0;
    }
    Edge(Vertex * &v1, Vertex * &v2) {
        vertex1 = v1;
        vertex2 = v2;
        prev = NULL;
        next = NULL;
        reverse = NULL;
        flag = UNSUBDIVIDED;
        outputed = UNOUTPUTED;
        loaded = UNLOADED;
        flag_zero_tree = UNSETTED;
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
        v_index_rvsbd = 0;
    }
    Edge(const QVector2D data) {
        vertex1 = new Vertex(data.x());
        vertex2 = new Vertex(data.y());
        prev = NULL;
        next = NULL;
        reverse = NULL;
        flag = UNSUBDIVIDED;
        outputed = UNOUTPUTED;
        loaded = UNLOADED;
        flag_zero_tree = UNSETTED;
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
        v_index_rvsbd = 0;
    }

    Edge(const Edge & edge) {
        vertex1 = edge.vertex1;
        vertex2 = edge.vertex2;
//        Vertex vertex_1 = (*edge.vertex1);
//        vertex1 = &vertex_1;
//        Vertex vertex_2 = (*edge.vertex2);
//        vertex2 = &vertex_2;
        prev = edge.prev;
        next = edge.next;
        reverse = edge.reverse;
        face = edge.face;
        parent = edge.parent;
        child1 = edge.child1;
        child2 = edge.child2;
        vtx_in = edge.vtx_in;
        outputed = edge.outputed;
        loaded = edge.loaded;
        flag_zero_tree = edge.flag_zero_tree;
        flag = edge.flag;
        error = edge.error;
        v_index_rvsbd = edge.v_index_rvsbd;
    }
    // [End of Edge()]

    Edge & operator = (const Edge & edge) {
//        Vertex vertex_1 = (*edge.vertex1);
//        vertex1 = &vertex_1;
//        Vertex vertex_2 = (*edge.vertex2);
//        vertex2 = &vertex_2;
        vertex1 = edge.vertex1;
        vertex2 = edge.vertex2;
        prev = edge.prev;
        next = edge.next;
        reverse = edge.reverse;
        face = edge.face;
        parent = edge.parent;
        child1 = edge.child1;
        child2 = edge.child2;
        vtx_in = edge.vtx_in;
        outputed = edge.outputed;
        flag_zero_tree = edge.flag_zero_tree;
        loaded = edge.loaded;
        flag = edge.flag;
        vtx_in = edge.vtx_in;
        error = edge.error;
        v_index_rvsbd = edge.v_index_rvsbd;
        return *this;
    }

    // ---
    // [Begin of ~Edge()]
    ~Edge() {
        prev = NULL;
        next = NULL;
        reverse = NULL;
        flag = UNSUBDIVIDED;
        outputed = UNOUTPUTED;
        loaded = UNLOADED;
        flag_zero_tree = UNSETTED;
        face = NULL;
        parent = NULL;
        child1 = NULL;
        child2 = NULL;
        vtx_in = NULL;
        v_index_rvsbd = 0;
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

    void set_loaded() {
        loaded = LOADED;
    }

    bool is_zero() {
        return (error.x()==0.0f && error.y()==0.0f && error.z()==0.0f);
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

    int face_color;

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
        face_color = 0;
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
        face_color = 0;
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

    Face(const Face &face) {
//        Edge e_1 = (*face.e1);
//        e1 = &e_1;
//        Edge e_2 = (*face.e2);
//        e2 = &e_2;
//        Edge e_3 = (*face.e3);
//        e3 = &e_3;
        e1 = face.e1;
        e2 = face.e2;
        e3 = face.e3;
        flag = face.flag;
        face_color = face.face_color;
        prev = face.prev;
        next = face.next;
    }

    Face & operator = (const Face & face) {
        e1 = face.e1;
        e2 = face.e2;
        e3 = face.e3;
        flag = face.flag;
        face_color = face.face_color;
        prev = face.prev;
        next = face.next;
        return *this;
    }
};


class Distance
{
public:
    unsigned int v_index;
    unsigned int a;
    unsigned int b;
    QVector3D distance;

    Distance(unsigned int v_index0, unsigned int a0, unsigned int b0,
             GLfloat x, GLfloat y, GLfloat z) {
        v_index = v_index0;
        a = a0;
        b = b0;
        distance.setX(x);
        distance.setY(y);
        distance.setZ(z);
    }
};


class HalfEdge
{
private:
    Face * face_front;
    int face_size;
    Vertex * vtx_front;
    int vtx_size;

    QVector<QVector3D> list;


public:

    GLfloat x_trans;
    GLfloat y_trans;
    GLfloat z_trans;
    GLfloat range;
    int rs_times;

    HalfEdge();
//    HalfEdge(const HalfEdge &rightside);
    ~HalfEdge();

    Face * get_face_front() const;
    Face * get_face_first() const;
    Vertex * get_vertex_front() const;
    Vertex * get_vertex_first() const;
    int get_vertex_size() const;
    int get_face_size() const;
    bool is_empty() const;

    bool add_face(Face *&face);
    bool add_vertex(Vertex * &vertex);


    void set_poroperties();
    void normalize_for_paint();
    void clear_edges_of_vertex();


    // --- construct half-edge from file
    bool construct_from_file(QString file_name);


    // --- subdivision begin

    // butterfly subdivision
    bool subdivision_butterfly();
    bool subdivision_loop_revision();

    // --- subdivision end


    // --- reverse subdivision
    bool partition();
    bool reverse_subdivision();


    // --- save processiong mesh as old method
    bool write_file(QString file_name, int times);
    bool write_pretreatment(QString file_name);

    // --- construct stable porcessing mesh
    bool construct_base_mesh(QString file_name);
    bool construct_next_distance(QString file_name, int &times);


    // --- load next mesh
    bool load_next(QString file_name, int &times);

//    void draw();
    void draw_normal() const;
    void draw_partition() const;
    void draw(int xRot, int yRot, int zRot, GLfloat m_fScale, int draw_flag) const;

    void redefine_vtx_index();
    bool write_stb_mesh(QString path);
    bool write_stb_distance(QString path);

    // --- zero tree compression
    bool zero_tree_compress(QString file_name, double x);


    // --- save as ply format
    bool save_as_ply(QString path) const;


private:

    // --- for construct halfedge begin

    // --- for read file begin
    void deal_with_v(QTextStream &sin, QVector<Vertex *> &vtx_list);
    void deal_with_vt(QTextStream &sin, QVector<QVector2D> &vt_list);
    void deal_with_vn(QTextStream &sin, QVector<QVector3D> &vn_list);
    void deal_with_f(QTextStream &sin, QVector<unsigned int> & vi_list,
                     QVector<unsigned int> &vti_list, QVector<unsigned int> &ni_list);
    // --- for read file end


    bool do_with_reverse(QQueue<Edge *> & edge_queue, Edge * &edge, QVector<unsigned int> & vi_array, QVector<Vertex *> &vtx_list);
    bool search_in_queue(QQueue<Edge *> &edge_queue, Edge * &edge);
    Face * search_in_viarray(QQueue<Edge *> &edge_queue, Edge * &edge, QVector<unsigned int> &vi_array, QVector<Vertex *> &vtx_list);
    Face * search_in_viarray_per(QQueue<Edge *> & edge_queue, Edge * &edge, QVector<unsigned int> &vi_array, int i, QVector<Vertex *> &vtx_list);
    void add_face_when_queue_is_null(QQueue<Edge *> &edge_queue, QVector<unsigned int> &vi_array, QVector<Vertex *> &vtx_list);

    // --- for construct halfedge end



    // --- subdivision begin

    void edge_subdivision_butterfly(Edge * &current);
    void edge_subdivision_loop_revision(Edge * &current);
    void face_subdivide_to_four(Face * &current);
    void clean_parent_faces(int origin_size);
    bool delete_face_front();

    // --- subdivision end


    // --- reverse subdivision begin

    //bool reverse_subdivision_loop();

    void unpart_vtx();
    Vertex * get_vertex_extraordinary() const;
    Vertex * get_unparted() const;
    bool deal_with_odd(Vertex * &vertex, QQueue<Vertex *> &vtx_queue);
    bool set_even(Vertex * &vertex, QQueue<Vertex *> &vtx_queue);

    bool face_dispose_rs(Face * &current_face);
    QVector3D get_error(Vertex * &v0, Vertex * &v1, Vertex * &v2, Vertex * &v3, Vertex * &v4);
    Edge * get_edge(Edge * &edge, Vertex * &v1, Vertex * &v2, Vertex * &v3);

    bool delete_ood_vtx();

    // --- reverse subdivision end


    // --- save processiong mesh as old method begin

    bool write_mesh(QString path);
    bool write_error(QString path);

    // --- save processing mesh as old method end


    // --- construct stable mesh begin
    // 1. base mesh construct
    bool do_with_reverse(QQueue<Edge *> & edge_queue, Edge * &edge, QVector<unsigned int> & vi_array, QHash<int, Vertex *> &vtx_hash);
    //bool search_in_queue(QQueue<Edge *> &edge_queue, Edge * &edge);
    Face * search_in_viarray(QQueue<Edge *> &edge_queue, Edge * &edge, QVector<unsigned int> &vi_array, QHash<int, Vertex *> &vtx_hash);
    Face * search_in_viarray_per(QQueue<Edge *> & edge_queue, Edge * &edge, QVector<unsigned int> &vi_array, int i, QHash<int, Vertex *> &vtx_hash);
    void add_face_when_queue_is_null(QQueue<Edge *> &edge_queue, QVector<unsigned int> &vi_array, QHash<int, Vertex *> &vtx_hash);
    void set_distance(Edge * &e, QVector<Distance *> &d_list);
    void subdivide_cnstr();
    void edge_subdivide_cnstr(Edge * &current);

    // --- construct stable mesh end


    // --- load stable mesh
    void set_distance(Edge *&edge, QQueue<QVector3D> &d_list);

    GLfloat max(QQueue<QVector3D> &d_list);
    GLfloat mid(QQueue<QVector3D> &d_list);
    void add_distances_zt(QQueue<QVector3D> &d_list, GLfloat epsilon);
    void set_distance_zt(Edge * &edge, QQueue<QVector3D> &d_list, GLfloat epsilon);
    void subdivide_zero_tree();

};

#endif // HALFEDGE_H
