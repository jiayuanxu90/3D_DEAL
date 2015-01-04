#ifndef MODELINTF_H
#define MODELINTF_H

#include "model.h"

class ModelIntf
{

public:

    virtual bool read_file(QString str, Model * &model) = 0;

    ModelIntf();
    ~ModelIntf();

};

#endif // MODELINTF_H
