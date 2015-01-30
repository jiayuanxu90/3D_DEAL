#ifndef FILEDISPOSE_H
#define FILEDISPOSE_H

#include "model.h"

class FileDispose
{

public:

    virtual bool read_file(QString str, Model * &model) = 0;

    FileDispose();
    ~FileDispose();

};

#endif // FILEDISPOSE_H
