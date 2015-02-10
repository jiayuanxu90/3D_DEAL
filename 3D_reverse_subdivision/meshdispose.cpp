#include "meshdispose.h"
#include "halfedge.h"
#include <QVector2D>
#include <QVector3D>
#include <QDebug>


MeshDispose::MeshDispose()
{
}


// --- We implement butterfly subdivision in the function.
// 1. check if the model and the half_edge both have been constructed
// 2. travel by faces
// 2.a. look for three adjacent faces
bool MeshDispose::subdivision_butterfly(HalfEdge *&halfEdge)
{
    if (halfEdge->is_empty()) {
        return false;
    }

    halfEdge->clear_edges_of_vertex();
//    Vertex * vtx_current = halfEdge->get_vertex_first();
//    while (vtx_current != halfEdge->get_vertex_front()) {
//        //vtx_current->edge_list_in.clear();
//        qDebug() << vtx_current->v_index << "  in size: " << vtx_current->edge_list_in.size();
//        //vtx_current->edge_list_out.clear();
//        qDebug() << vtx_current->v_index << "  out size: " << vtx_current->edge_list_out.size();
//        vtx_current = vtx_current->next;
//    }
    Face * current = halfEdge->get_face_first();
    int origin_size = halfEdge->get_face_size();
    //model->vertex_indices.clear();

    for (int i=0; i<origin_size; ++i) {
        edge_subdivide_butterfly(current->e1, halfEdge);
        edge_subdivide_butterfly(current->e2, halfEdge);
        edge_subdivide_butterfly(current->e3, halfEdge);

        face_subdivide_to_four(current, halfEdge);

        current = current->next;
    }

    for (int i=0; i<origin_size; ++i) {
        halfEdge->delete_face_front();
    }


//    vtx_current = halfEdge->get_vertex_first();
//    while (vtx_current != halfEdge->get_vertex_front()) {
//        //vtx_current->edge_list_in.clear();
//        qDebug() << vtx_current->v_index << "  in size: " << vtx_current->edge_list_in.size();
//        //vtx_current->edge_list_out.clear();
//        qDebug() << vtx_current->v_index << "  out size: " << vtx_current->edge_list_out.size();
//        vtx_current = vtx_current->next;
//    }

    return true;
}

bool MeshDispose::subdivision_loop_revision(HalfEdge *&halfEdge)
{
    if (halfEdge->is_empty()) {
        return false;
    }

    halfEdge->clear_edges_of_vertex();
    Face * current = halfEdge->get_face_first();
    int origin_size = halfEdge->get_face_size();

    for (int i=0; i<origin_size; ++i) {
        edge_subdivide_loop_revision(current->e1, halfEdge);
        edge_subdivide_loop_revision(current->e2, halfEdge);
        edge_subdivide_loop_revision(current->e3, halfEdge);

        face_subdivide_to_four(current, halfEdge);

        current = current->next;
    }

    for (int i=0; i<origin_size; ++i) {
        halfEdge->delete_face_front();
    }

    return true;
}


void MeshDispose::edge_subdivide_butterfly(Edge *&current, HalfEdge * &halfEdge)
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
        vtx_new->v_index = halfEdge->get_vertex_size() + 1;
        halfEdge->add_vertex(vtx_new);
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


bool MeshDispose::reverse_subdivision(HalfEdge *&halfEdge, int flag)
{
    bool resault = false;

    switch (flag) {
    case 0:
        resault = reverse_subdivision_loop(halfEdge);
        break;
    default:
        qDebug() << "Wrong choice...";
    }
    return resault;
}



void MeshDispose::edge_subdivide_loop_revision(Edge *&current, HalfEdge * &halfEdge)
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
        vtx_new->v_index = halfEdge->get_vertex_size() + 1;
        halfEdge->add_vertex(vtx_new);
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

void MeshDispose::face_subdivide_to_four(Face * &current, HalfEdge * &halfEdge)
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
    halfEdge->add_face(face_new_1);

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
    halfEdge->add_face(face_new_2);

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
    halfEdge->add_face(face_new_3);

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
    halfEdge->add_face(face_new_4);
}

void MeshDispose::clean_parent_faces(HalfEdge * &halfEdge, int origin_size)
{
    for (int i=0; i<origin_size; ++i) {
        halfEdge->delete_face_front();
    }
}

bool MeshDispose::reverse_subdivision_loop(HalfEdge *&halfEdge)
{
    bool flag = true;

    unpart_vtx(halfEdge);

    if (partition(halfEdge)) {

        qDebug() << "Partition successful ...!!!!";

        halfEdge->clear_edges_of_vertex();

        Face * current_face = halfEdge->get_face_first();
        int face_size_origin = halfEdge->get_face_size();
        int i = 0;
        for (i=0; i<face_size_origin; ++i) {
            face_dispose_rs(current_face, halfEdge);
            current_face = current_face->next;
        }

        // delete the origin faces
        for (i=0; i<face_size_origin; ++i) {
            halfEdge->delete_face_front();
        }

        // delete the odd vertices
        halfEdge->delete_ood_vtx();

        // update the vertices' v_index
        halfEdge->update_vtx_index();
    }
    else {
        flag = false;
    }
    return flag;
}

void MeshDispose::unpart_vtx(HalfEdge *&halfEdge)
{
    Vertex * vtx_current = halfEdge->get_vertex_first();
    while (vtx_current != halfEdge->get_vertex_front()) {
        vtx_current->set_unparted();
        vtx_current = vtx_current->next;
    }
}

bool MeshDispose::partition(HalfEdge *&halfEdge)
{
                    //     ____
                    //    /\  /\
                    //   /__\/__\
                    //   \  /\  /\
                    //    \/__\/  \


    unpart_vtx(halfEdge);
    Vertex * start_vertex = halfEdge->get_vertex_extraordinary();

    QQueue<Vertex *> vtx_queue;
    bool flag = true;
    while (start_vertex != NULL && flag) {
        set_even(start_vertex, vtx_queue);
        while (!vtx_queue.empty() && vtx_queue.size()!=0 && flag) {
            Vertex * vertex = vtx_queue.dequeue();
            if (!deal_with_odd(vertex, vtx_queue))
                flag = false;
        }
        start_vertex = halfEdge->get_vertex_extraordinary();
    }


    return flag;
}

bool MeshDispose::deal_with_odd(Vertex * &vertex, QQueue<Vertex *> &vtx_queue)
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

bool MeshDispose::set_even(Vertex *&vertex, QQueue<Vertex *> &vtx_queue)
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

bool MeshDispose::face_dispose_rs(Face *&current_face, HalfEdge *&halfEdge)
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
        halfEdge->add_face(face_new);

        //QVector3D error1 = get_error(v1);
        // --- make sure the direction is correct

        return true;
    }

    return false;
}

Edge * MeshDispose::get_edge(Edge *&edge, Vertex *&v1, Vertex *&v2, Vertex *&v3)
{
    if (edge->reverse->next->parent != NULL)
        return edge->reverse->next->parent;
    else {
        Vertex * v4 = NULL;
        if (edge->reverse->next->reverse != NULL) {
            v4 = edge->reverse->next->reverse->next->reverse->prev->reverse->next->vertex2;
        }

        QVector3D error = get_error(edge->vertex1, v1, v2, v3, v4);
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
            //edge_new_reverse->add_edge_to_vtx_list();
        }
        return edge_new;
    }

}

QVector3D MeshDispose::get_error(Vertex *&v0, Vertex *&v1, Vertex *&v2, Vertex *&v3, Vertex *&v4)
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
