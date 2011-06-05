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

class Renderer
{
public:
    Renderer();

    void render(QGraphicsRectItem *area);
    QImage img;
private:
    Vector2 data;
};

#endif // RENDERER_H
