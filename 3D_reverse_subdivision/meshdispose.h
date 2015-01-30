#ifndef MESHDISPOSE_H
#define MESHDISPOSE_H

#include "model.h"
#include "halfedge.h"

class MeshDispose
{
public:
    MeshDispose();

    bool subdivision_butterfly(HalfEdge * &halfEdge, Model * &model);
    bool subdivision_loop_revision(HalfEdge * &halfEdge, Model * &model);

private:
    void edge_subdivide_butterfly(Edge * &current, Model * &model, HalfEdge *&halfEdge);
    void edge_subdivide_loop_revision(Edge * &current, Model * &model, HalfEdge *&halfEdge);
    void face_subdivide_to_four(Face *&current, HalfEdge *&halfEdge, Model *&model);
    void clean_parent_faces(HalfEdge * &halfEdge, int origin_size);
};

#endif // MESHDISPOSE_H
