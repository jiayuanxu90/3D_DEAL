#include "fdply.h"
#include <QFile>
#include <QTextStream>

#include "filedispose.h"
#include "model.h"

FDPly::FDPly(QString str, Model * &model) : FileDispose()
{
    property_cursor = 1;

    v_cnt = 0;
    vertex_v_on = true;
    vertex_rgb_on = false;
    vertex_vn_on = false;
    vertex_intensity_on = false;
    vertex_confidence_on = false;

    p_cnt = 0;
    point_v_on = false;
    point_rgb_on = false;
    point_vn_on = false;
    point_intensity_on = false;
    point_confidence_on = false;

    f_cnt = 0;

    read_file(str, model);
}

FDPly::~FDPly()
{

}

bool FDPly::read_file(QString str, Model * &model)
{
    QFile file(str);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "*** Can not open the file! File location is \""
                 << str << "\" ***" << endl;
        return false;
    }
    else {
        model = new Model();
        QTextStream infile(&file);

        deal_with_head(infile);

        deal_with_vertices(infile, model);

        deal_with_faces(infile, model);

        model->set_poroperties();
        model->normalize_for_paint();

        return true;
    }
}

void FDPly::deal_with_head(QTextStream &infile)
{
    while (!infile.atEnd()) {
        QString line = infile.readLine();
        if (line == "end_header")
            break;
        else if (line.startsWith("element")) {
            QTextStream sin(&line);
            QString x, y, z;
            sin >> x >> y >> z;
            if (y == "vertex") {
                v_cnt  = z.toUInt();
                property_cursor = 1;
            }
            else if (y == "point") {
                p_cnt = z.toUInt();
                property_cursor = 2;
            }
            else if (y == "face") {
                f_cnt = z.toUInt();
                property_cursor = 3;
            }
        }
        else if (line.startsWith("property")) {
            if (property_cursor == 1) {
                QTextStream sin(&line);
                QString x, y, z;
                sin >> x >> y >> z;
                if (z!=NULL && z=="x") {
                    vertex_v_on = true;
                }
                else if (z!=NULL && z=="nx") {
                    vertex_vn_on = true;
                }
                else if (z!=NULL && z=="red") {
                    vertex_rgb_on = true;
                }
                else if (z!=NULL && z=="intensity") {
                    vertex_intensity_on = true;
                }
                else if (z!=NULL && z=="confidence") {
                    vertex_confidence_on = true;
                }
            }
            else if (property_cursor == 2) {
                QTextStream sin(&line);
                QString x, y, z;
                sin >> x >> y >> z;
                if (z!=NULL && z=="x") {
                    point_v_on = true;
                }
                else if (z!=NULL && z=="nx") {
                    point_vn_on = true;
                }
                else if (z!=NULL && z=="red") {
                    point_rgb_on = true;
                }
                else if (z!=NULL && z=="intensity") {
                    point_intensity_on = true;
                }
                else if (z!=NULL && z=="confidence") {
                    point_confidence_on = true;
                }
            }
        }
    }

}

void FDPly::deal_with_vertices(QTextStream &infile, Model * &model)
{
    for (unsigned int i=0; i<v_cnt; ++i) {
        QString line = infile.readLine();
        QTextStream sin(&line);
        if (vertex_v_on) {
            GLfloat x = 0.0f,
                    y = 0.0f,
                    z = 0.0f;
            sin >> x >> y >> z;
            model->add_vertex(x, y, z);
        }
        if (vertex_rgb_on) {
            GLint red = 255,
                green = 255,
                blue = 255;
            sin >> red >> green >> blue;
            model->add_vertex_rgb(red, green, blue);
        }
        if (vertex_vn_on) {
            GLfloat x = 0.0f,
                    y = 0.0f,
                    z = 0.0f;
            sin >> x >> y >> z;
            model->add_vertex_normal(x, y, z);
        }

        // -----------------------------------------
        // intensity and confidence will be add here
        // -----------------------------------------

    }
}

void FDPly::deal_with_faces(QTextStream &infile, Model *&model)
{
    for (unsigned int i=0; i<f_cnt; ++i) {
        QString line = infile.readLine();
        QTextStream sin(&line);
        unsigned int count = 0;
        sin >> count;
        if (count != 3) {
            QMessageBox::warning(NULL, "Warning!", "Triangle mesh only!", QMessageBox::Ok, QMessageBox::Ok);
            break;
        }
        else {
            for (int j=0; j<3; ++j) {
                unsigned int a = 0;
                sin >> a;
                if (vertex_v_on)
                    model->add_v_index(a+1);
                if (vertex_rgb_on)
                    model->add_vrgb_index(a+1);
                if (vertex_vn_on)
                    model->add_n_index(a+1);
            }
        }
    }
}
