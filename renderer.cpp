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

    double minVal=1e20, maxVal=-1e20;

    for (int y=0; y<rect.height(); ++y) {
        for (int x=0; x<rect.width(); ++x) {
            double & value = data[x][y];
            QVector2D tmpval(0,0);

            foreach (const QGraphicsItem * tmp, area->childItems()) {
                const PointCharge * p = (PointCharge *) tmp;

                const QVector2D r = QVector2D(p->pos()) - QVector2D(x, y);
                if (qAbs(r.length()) > EPS)
                    //value += qK * p->charge / r.length(); - потенциал

                    tmpval += (qK * p->charge / r.lengthSquared()) * r.normalized(); //напряженность
                else {
                    value = 0;
                    break;
                }
            }
            value = tmpval.length(); //напряженность

            minVal = qMin(minVal, value);
            maxVal = qMax(maxVal, value);
        }
    }

    const double diff = maxVal - minVal;
    QColor color;
    color.setRgb(0, 24, 241);
    for (int y=0; y<rect.height(); ++y) {
        for (int x=0; x<rect.width(); ++x) {
            //qDebug("%.4lf", (255*(data[x][y]-minVal)/diff));
            if ( data[x][y]-minVal < 0) qDebug("%d %d %.5lf %.5lf", x, y, data[x][y], minVal);
            color.setRed( (int)(256*80000*(data[x][y]-minVal)/diff) % 256 );
            //color = QColor::fromHsv((int)(180+360*20*(data[x][y]-minVal)/diff) % 360, 200, 200);
            img.setPixel(x,y, color.rgb());
        }
    }
}



