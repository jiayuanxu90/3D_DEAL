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
bool MeshDispose::subdivision_butterfly(HalfEdge *&halfEdge, Model *&model)
{
    if (halfEdge->is_empty() || model->empty()) {
        return false;
    }

    Face * current = halfEdge->get_first();
    int origin_size = halfEdge->get_face_size();
    model->vertex_indices.clear();

    for (int i=0; i<origin_size; ++i) {
        edge_subdivide_butterfly(current->e1, model, halfEdge);
        edge_subdivide_butterfly(current->e2, model, halfEdge);
        edge_subdivide_butterfly(current->e3, model, halfEdge);

        face_subdivide_to_four(current, halfEdge, model);

        current = current->next;
    }

    return true;
}

bool MeshDispose::subdivision_loop_revision(HalfEdge *&halfEdge, Model *&model)
{
    if (halfEdge->is_empty() || model->empty()) {
        return false;
    }

    Face * current = halfEdge->get_first();
    int origin_size = halfEdge->get_size();
    model->vertex_indices.clear();

    for (int i=0; i<origin_size; ++i) {
        edge_subdivide_loop_revision(current->e1, model, halfEdge);
        edge_subdivide_loop_revision(current->e2, model, halfEdge);
        edge_subdivide_loop_revision(current->e3, model, halfEdge);

        face_subdivide_to_four(current, halfEdge, model);

        current = current->next;
    }

    return true;
}


void MeshDispose::edge_subdivide_butterfly(Edge *&current, Model *&model, HalfEdge * &halfEdge)
{
    if (current->flag == UNSUBDIVIDED) {
        QVector3D v1, v2, v3, v4, q1, q2, q3, q4;
        unsigned int i_v1 = current->getX();
        v1.setX(model->v_array.at(i_v1).x());
        v1.setY(model->v_array.at(i_v1).y());
        v1.setZ(model->v_array.at(i_v1).z());

        unsigned int i_v2 = current->getY();
        v2.setX(model->v_array.at(i_v2).x());
        v2.setY(model->v_array.at(i_v2).y());
        v2.setZ(model->v_array.at(i_v2).z());

        unsigned int i_v3 = current->next->getY();
        v3.setX(model->v_array.at(i_v3).x());
        v3.setY(model->v_array.at(i_v3).y());
        v3.setZ(model->v_array.at(i_v3).z());
        Edge * edge_Q1 = current->prev->reverse;
        if (edge_Q1 == NULL) {
            q1.setX(0.0f);
            q1.setY(0.0f);
            q1.setZ(0.0f);
        }
        else {
            unsigned int i_q1 = edge_Q1->next->getY();
            q1.setX(model->v_array.at(i_q1).x());
            q1.setY(model->v_array.at(i_q1).y());
            q1.setZ(model->v_array.at(i_q1).z());
        }
        Edge * edge_Q2 = current->next->reverse;
        if (edge_Q2 == NULL) {
            q2.setX(0.0f);
            q2.setY(0.0f);
            q2.setZ(0.0f);
        }
        else {
            unsigned int i_q2 = edge_Q2->next->getY();
            q2.setX(model->v_array.at(i_q2).x());
            q2.setY(model->v_array.at(i_q2).y());
            q2.setZ(model->v_array.at(i_q2).z());
        }
        Edge * edge_rev = current->reverse;
        if (edge_rev != NULL) {
            unsigned int i_v4 = edge_rev->next->getY();
            v4.setX(model->v_array.at(i_v4).x());
            v4.setY(model->v_array.at(i_v4).y());
            v4.setZ(model->v_array.at(i_v4).z());

            if (edge_rev->prev->reverse != NULL) {
                unsigned int i_q3 = edge_rev->prev->reverse->next->getY();
                q3.setX(model->v_array.at(i_q3).x());
                q3.setY(model->v_array.at(i_q3).y());
                q3.setZ(model->v_array.at(i_q3).z());
            }
            else {
                q3.setX(0.0f);
                q3.setY(0.0f);
                q3.setZ(0.0f);
            }

            if (edge_rev->next->reverse != NULL) {
                unsigned int i_q4 = edge_rev->next->reverse->next->getY();
                q4.setX(model->v_array.at(i_q4).x());
                q4.setY(model->v_array.at(i_q4).y());
                q4.setZ(model->v_array.at(i_q4).z());
            }
            else {
                q4.setX(0.0f);
                q4.setY(0.0f);
                q4.setZ(0.0f);
            }
        }
        else {
            v4.setX(0.0f);
            v4.setY(0.0f);
            v4.setZ(0.0f);
        }

        QVector3D v_new = (8*(v1+v2) + 2*(v3+v4) - (q1+q2+q3+q4)) / 16;

        model->v_array.push_back(v_new);
        unsigned int i_v_new = model->v_array.size()-1;
        Vertex * vertex = new Vertex(i_v_new);
        halfEdge->add_vertex(vertex);
        current->child1 = new Edge(current->vertex1, vertex);
        current->child2 = new Edge(vertex, current->vertex2);
        current->child1->parent = current;
        current->child2->parent = current;
        current->vi = i_v_new;
        current->flag = SUBDIVIDED;

        if (current->reverse != NULL) {

            current->reverse->child1 = new Edge(current->reverse->vertex1, vertex);
            current->reverse->child2 = new Edge(vertex, current->reverse->vertex2);
            current->child1->reverse = current->reverse->child2;
            current->reverse->child2->reverse = current->child1;
            current->child2->reverse = current->reverse->child1;
            current->reverse->child1->reverse = current->child2;
            current->reverse->child1->parent = current->reverse;
            current->reverse->child2->parent = current->reverse;
            current->reverse->flag = SUBDIVIDED;
        }
    }



}

void MeshDispose::edge_subdivide_loop_revision(Edge *&current, Model *&model, HalfEdge * &halfEdge)
{
    if (current->flag == UNSUBDIVIDED) {
        QVector3D v1, v2, v3, v4;
        unsigned int i_v1 = current->getX();
        v1.setX(model->v_array.at(i_v1).x());
        v1.setY(model->v_array.at(i_v1).y());
        v1.setZ(model->v_array.at(i_v1).z());

        unsigned int i_v2 = current->getY();
        v2.setX(model->v_array.at(i_v2).x());
        v2.setY(model->v_array.at(i_v2).y());
        v2.setZ(model->v_array.at(i_v2).z());

        unsigned int i_v3 = current->next->getY();
        v3.setX(model->v_array.at(i_v3).x());
        v3.setY(model->v_array.at(i_v3).y());
        v3.setZ(model->v_array.at(i_v3).z());
        Edge * edge_rev = current->reverse;
        if (edge_rev != NULL) {
            unsigned int i_v4 = edge_rev->next->getY();
            v4.setX(model->v_array.at(i_v4).x());
            v4.setY(model->v_array.at(i_v4).y());
            v4.setZ(model->v_array.at(i_v4).z());
        }
        else {
            v4.setX(0.0f);
            v4.setY(0.0f);
            v4.setZ(0.0f);
        }

        QVector3D v_new = (3*(v1+v2) + 1*(v3+v4)) / 8;

        model->v_array.push_back(v_new);
        unsigned int i_v_new = model->v_array.size()-1;
        Vertex * vertex = new Vertex(i_v_new);
        halfEdge->add_vertex(vertex);
        current->child1 = new Edge(current->vertex1, vertex);
        current->child2 = new Edge(vertex, current->vertex2);
        current->child1->parent = current;
        current->child2->parent = current;
        current->vi = i_v_new;
        current->flag = SUBDIVIDED;

        if (current->reverse != NULL) {
            current->reverse->child1 = new Edge(current->reverse->vertex1, vertex);
            current->reverse->child2 = new Edge(vertex, current->reverse->vertex2);
            current->child1->reverse = current->reverse->child2;
            current->reverse->child2->reverse = current->child1;
            current->child2->reverse = current->reverse->child1;
            current->reverse->child1->reverse = current->child2;
            current->reverse->child1->parent = current->reverse;
            current->reverse->child2->parent = current->reverse;
            current->reverse->flag = SUBDIVIDED;
        }
    }
}

void MeshDispose::face_subdivide_to_four(Face * &current, HalfEdge * &halfEdge, Model * &model)
{
    Face * face_new_1 = new Face();
    face_new_1->e1 = current->e1->child2;
    face_new_1->e2 = current->e2->child1;
    Edge * edge_new_1 = new Edge(face_new_1->e2->vertex2, face_new_1->e1->vertex1);
    face_new_1->e3 = edge_new_1;
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
    model->add_v_index(face_new_1->e1->getX());
    model->add_v_index(face_new_1->e2->getX());
    model->add_v_index(face_new_1->e3->getX());

    Face * face_new_2 = new Face();
    face_new_2->e1 = current->e2->child2;
    face_new_2->e2 = current->e3->child1;
    Edge * edge_new_2 = new Edge(face_new_2->e2->vertex2, face_new_2->e1->vertex1);
    face_new_2->e3 = edge_new_2;
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
    model->add_v_index(face_new_2->e1->getX());
    model->add_v_index(face_new_2->e2->getX());
    model->add_v_index(face_new_2->e3->getX());

    Face * face_new_3 = new Face();
    face_new_3->e1 = current->e3->child2;
    face_new_3->e2 = current->e1->child1;
    Edge * edge_new_3 = new Edge(face_new_3->e2->vertex2, face_new_3->e1->vertex1);
    face_new_3->e3 = edge_new_3;
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
    model->add_v_index(face_new_3->e1->getX());
    model->add_v_index(face_new_3->e2->getX());
    model->add_v_index(face_new_3->e3->getX());

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
    model->add_v_index(face_new_4->e1->getX());
    model->add_v_index(face_new_4->e2->getX());
    model->add_v_index(face_new_4->e3->getX());
}

void MeshDispose::clean_parent_faces(HalfEdge * &halfEdge, int origin_size)
{
    for (int i=0; i<origin_size; ++i) {
        halfEdge->delete_face_front();
    }
}
