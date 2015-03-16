#ifndef FOBJ_H
#define FOBJ_H

#include "filedispose.h"
#include <QTextStream>
#include "model.h"

class FObj : public FileDispose
{
public:
    bool read_file(QString str, Model * &model);

    FObj(QString str, Model * &model);
    ~FObj();

private:
    // read file
    void deal_with_v(QTextStream &ts, Model * &model);
    void deal_with_vt(QTextStream &ts, Model * &model);
    void deal_with_vn(QTextStream &ts, Model * &model);
    void deal_with_f(QTextStream &ts, Model * &model);

    // write file

};

#endif // FOBJ_H
