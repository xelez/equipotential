#ifndef POINTCHARGE_H
#define POINTCHARGE_H

#include <QGraphicsItem>

class MainWindow;

class PointCharge : public QGraphicsItem
{
public:
    PointCharge(MainWindow *win);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    double charge;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

//    void mousePressEvent(QGraphicsSceneMouseEvent *event);
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    MainWindow* window;
};

#endif // POINTCHARGE_H
