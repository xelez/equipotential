#ifndef RENDERER_H
#define RENDERER_H

#include <QVector>
#include <QImage>
#include <QGraphicsRectItem>
#include "pointcharge.h"

//const double qK = 9*1e9;
const double qK = 9;
const double EPS = 1e-10;

typedef QVector< QVector<double> > Vector2;
typedef double (*CalcFunc)(QGraphicsItem*, int & x, int & y);

class Renderer
{
public:
    Renderer();

    void render(QGraphicsItem *area, CalcFunc calc);
    QImage img;

    static double calcPotential(QGraphicsItem* area, int & x, int & y);
    static double calcElectricField(QGraphicsItem* area, int & x, int & y);

private:
    Vector2 data;
};

#endif // RENDERER_H
