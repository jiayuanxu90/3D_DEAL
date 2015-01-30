#ifndef FDOBJ_H
#define FDOBJ_H

#include "filedispose.h"
#include <QTextStream>
#include "model.h"

class FDObj: public FileDispose
{


public:

    bool read_file(QString str, Model * &model);

    FDObj(QString str, Model * &model);
    ~FDObj();

private:
    void deal_with_v(QTextStream & ts, Model * &model);
    void deal_with_vt(QTextStream & ts, Model * &model);
    void deal_with_vn(QTextStream & ts, Model * &model);
    void deal_with_f(QTextStream & ts, Model * &model);
};

#endif // FDOBJ_H
