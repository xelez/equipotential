#include "renderer.h"

#include <QVector2D>

Renderer::Renderer()
{
}

void Renderer::render(QGraphicsItem *area)
{
    QRectF rect = area->boundingRect();
    img = QImage(rect.width(), rect.height(), QImage::Format_RGB32);
    img.fill(QColor(Qt::white).rgb());

    data.clear();
    data.resize(rect.width());

    for (Vector2::iterator v = data.begin(); v != data.end(); ++v)
        v->resize(rect.height());

    double minP=1e20, maxP=-1e20;

    for (int y=0; y<rect.height(); ++y) {
        for (int x=0; x<rect.width(); ++x) {
            double & potential = data[x][y];

            foreach (const QGraphicsItem * tmp, area->childItems()) {
                const PointCharge * p = (PointCharge *) tmp;
                const double r = (QVector2D(p->pos())-QVector2D(x, y)).length();
                if (qAbs(r) > EPS)
                    potential += qK * p->charge / r;
                else {
                    potential = 0;
                    break;
                }
            }

            minP = qMin(minP, potential);
            maxP = qMax(maxP, potential);
        }
    }

    const double diff = maxP - minP;
    QColor color;
    color.setRgb(0, 24, 241);
    for (int y=0; y<rect.height(); ++y) {
        for (int x=0; x<rect.width(); ++x) {
            //qDebug("%.4lf", (255*(data[x][y]-minP)/diff));
            if ( data[x][y]-minP < 0) qDebug("%d %d %.5lf %.5lf", x, y, data[x][y], minP);
            //color.setRed( (int)(256*800*(data[x][y]-minP)/diff) % 256 );
            color = QColor::fromHsv((int)(180+360*20*(data[x][y]-minP)/diff) % 360, 200, 200);
            img.setPixel(x,y, color.rgb());
        }
    }
}



