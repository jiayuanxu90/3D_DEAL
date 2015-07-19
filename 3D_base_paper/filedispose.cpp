#include "filedispose.h"


FileDispose::FileDispose()
{

}

FileDispose::~FileDispose()
{
    vertex_indices.clear();
    vt_indices.clear();
    normal_indices.clear();
    vrgb_indices.clear();
    vn_list.clear();

    qDebug() << "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh";
}
