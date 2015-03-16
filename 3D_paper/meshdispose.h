#ifndef MESHDISPOSE_H
#define MESHDISPOSE_H

#include "model.h"
#include "halfedge.h"
#include <QQueue>

class MeshDispose
{
public:
    MeshDispose();

    // subdivision
    bool subdivision_butterfly(HalfEdge * &halfEdge);
    bool subdivision_loop_revision(HalfEdge * &halfEdge);


    // reverse subdivision
    bool reverse_subdivision(HalfEdge * &halfEdge, int flag);

    bool partition(HalfEdge * &halfEdge);

private:
    // subdivision
    void edge_subdivide_butterfly(Edge * &current, HalfEdge *&halfEdge);
    void edge_subdivide_loop_revision(Edge * &current, HalfEdge *&halfEdge);
    void face_subdivide_to_four(Face *&current, HalfEdge *&halfEdge);
    void clean_parent_faces(HalfEdge * &halfEdge, int origin_size);



    // reverse subdivision
    bool reverse_subdivision_loop(HalfEdge * &halfEdge);

    void unpart_vtx(HalfEdge * &halfEdge);
    bool deal_with_odd(Vertex * &vertex, QQueue<Vertex *> &vtx_queue);
    bool set_even(Vertex * &vertex, QQueue<Vertex *> &vtx_queue);

    bool face_dispose_rs(Face * &current_face, HalfEdge * &halfEdge);
    QVector3D get_error(Vertex * &v0, Vertex * &v1, Vertex * &v2, Vertex * &v3, Vertex * &v4);
    Edge * get_edge(Edge * &edge, Vertex * &v1, Vertex * &v2, Vertex * &v3);
};

#endif // MESHDISPOSE_H
