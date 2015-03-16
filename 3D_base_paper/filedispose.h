#ifndef FILEDISPOSE_H
#define FILEDISPOSE_H

#include <QVector>
#include "model.h"

class FileDispose
{
public:
    FileDispose();
    ~FileDispose();

    QVector<unsigned int> vertex_indices, vt_indices, normal_indices, vrgb_indices;

    virtual bool read_file(QString str, Model * &model) = 0;
};

#endif // FILEDISPOSE_H
