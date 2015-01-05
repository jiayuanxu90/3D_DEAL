#ifndef PLYIMPL_H
#define PLYIMPL_H

#include <QTextStream>

#include "modelintf.h"
#include "model.h"

/**
 * @brief The PlyImpl class
 * ply format overview:
 *  ELEMENT vertex
    PROPERTY              x (float)
    PROPERTY              y (float)
    PROPERTY              z (float)
    PROPERTY            red (unsigned char)
    PROPERTY          green (unsigned char)
    PROPERTY           blue (unsigned char)
    PROPERTY             nx (float)
    PROPERTY             ny (float)
    PROPERTY             nz (float)
    PROPERTY      intensity (float)
    PROPERTY     confidence (float)
 ELEMENT point
    PROPERTY              x (float)
    PROPERTY              y (float)
    PROPERTY              z (float)
    PROPERTY            red (unsigned char)
    PROPERTY          green (unsigned char)
    PROPERTY           blue (unsigned char)
    PROPERTY             nx (float)
    PROPERTY             ny (float)
    PROPERTY             nz (float)
    PROPERTY      intensity (float)
    PROPERTY     confidence (float)
 ELEMENT face
    PROPERTY vertex_indices (LIST uchar int)
    PROPERTY   vertex_index (LIST uchar int)  <<  Only read for compatibility reasons. Never written.
 */

class PlyImpl : public ModelIntf
{
public:

    bool read_file(QString str, Model *&model);

    PlyImpl(QString str, Model * &model);
    ~PlyImpl();

private:
    int property_cursor;

    unsigned int v_cnt;
    bool vertex_v_on;
    bool vertex_rgb_on;
    bool vertex_vn_on;
    bool vertex_intensity_on;
    bool vertex_confidence_on;

    unsigned int p_cnt;
    bool point_v_on;
    bool point_rgb_on;
    bool point_vn_on;
    bool point_intensity_on;
    bool point_confidence_on;

    unsigned int f_cnt;

    void deal_with_head(QTextStream & infile);
    void deal_with_vertices(QTextStream & infile, Model * &model);
    void deal_with_faces(QTextStream & infile, Model * &model);


};

#endif // PLYIMPL_H
