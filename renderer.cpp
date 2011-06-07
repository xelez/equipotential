#include "renderer.h"

#include <QVector2D>
#include <cmath>

Renderer::Renderer()
{
}

double Renderer::calcElectricField(QGraphicsItem * area, int &x, int &y) {
    QVector2D tmpval(0,0);

    foreach (const QGraphicsItem * tmp, area->childItems()) {
        const PointCharge * p = (PointCharge *) tmp;
        const QVector2D r = QVector2D(p->pos()) - QVector2D(x, y);

        if (qAbs(r.length()) > EPS)
            tmpval += (qK * p->charge / r.lengthSquared()) * r.normalized();
        else
            return 0;
    }

    return tmpval.length();
}

double Renderer::calcPotential(QGraphicsItem * area, int &x, int &y) {
    double value = 0.0;

    foreach (const QGraphicsItem * tmp, area->childItems()) {
        const PointCharge * p = (PointCharge *) tmp;
        const QVector2D r = QVector2D(p->pos()) - QVector2D(x, y);

        if (qAbs(r.length()) > EPS)
            value += qK * p->charge / r.length();
        else
            return 0;
    }

    return value;
}

void Renderer::render(QGraphicsItem *area, CalcFunc calc)
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
            value = calc(area, x, y);
            minVal = qMin(minVal, value);
            maxVal = qMax(maxVal, value);
        }
    }

    const double diff = maxVal - minVal;
    for (int y=0; y<rect.height(); ++y) {
        for (int x=0; x<rect.width(); ++x) {
            double tmp = 2*pow((data[x][y]-minVal)/diff, 1.0/8.0);
            if (tmp!=tmp) tmp=0;
            const double mapped = ( (int)(1e6*tmp) % 1000000 ) / 1e6;
            QColor color = QColor::fromHsvF(mapped, 0.7, 0.7);
            img.setPixel(x,y, color.rgb());
        }
    }
}



