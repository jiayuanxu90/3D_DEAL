#ifndef OBJIMPL_H
#define OBJIMPL_H

#include "modelintf.h"
#include <QTextStream>
#include "model.h"

class ObjImpl : public ModelIntf
{


public:

    bool read_file(QString str, Model * &model);

    ObjImpl(QString str, Model * &model);
    ~ObjImpl();

private:
    void deal_with_v(QTextStream & ts, Model * &model);
    void deal_with_vt(QTextStream & ts, Model * &model);
    void deal_with_vn(QTextStream & ts, Model * &model);
    void deal_with_f(QTextStream & ts, Model * &model);
};

#endif // OBJIMPL_H
