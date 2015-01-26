#ifndef MESHDISPOSE_H
#define MESHDISPOSE_H

#include "model.h"
#include "halfedge.h"

class MeshDispose
{
public:
    MeshDispose();

    bool subdivision_butterfly(HalfEdge * &halfEdge, Model * &model);
    void edge_subdivide(Edge * &current, Model * &model);
    void face_subdivide_to_four(Face *&current, HalfEdge *&halfEdge, Model *&model);
    void clean_parent_faces(HalfEdge * &halfEdge, int origin_size);
};

#endif // MESHDISPOSE_H
