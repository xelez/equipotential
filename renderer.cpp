#include "renderer.h"

#include <QVector2D>

Renderer::Renderer()
{
}

void Renderer::render(QGraphicsRectItem *area)
{
    QRectF rect = area->rect();
    img = QImage(rect.width(), rect.height(), QImage::Format_RGB32);
    img.fill(QColor(Qt::white).rgb());

    data.clear();
    data.resize(rect.width());

    for (Vector2::iterator v = data.begin(); v != data.end(); ++v)
        v->resize(rect.height());

    double minP=1e20, maxP=-1e20;

    for (int y=0; y<rect.height(); ++y) {
        for (int x=0; x<rect.width(); ++x) {
            foreach (const QGraphicsItem * tmp, area->childItems()) {
                const PointCharge * p = (PointCharge *) tmp;
                double & potential = data[x][y];
                const double r = (QVector2D(p->pos())-QVector2D(x, y)).length();
                if (qAbs(r) > EPS)
                    potential += qK * p->charge / r;
                else {
                    potential = 0;
                    break;
                }
                minP = qMin(minP, data[x][y]);
                maxP = qMax(maxP, data[x][y]);
            }
        }
    }

    const double diff = maxP - minP;
    qDebug("%.6lf %.6lf %.6lf", maxP, minP, diff);
    QColor color;
    for (int y=0; y<rect.height(); ++y) {
        for (int x=0; x<rect.width(); ++x) {
            //qDebug("%.4lf", (255*(data[x][y]-minP)/diff));
            img.setPixel(x,y, qRgb((25500*(data[x][y]-minP)/diff), 255, 255));
        }
    }
}



