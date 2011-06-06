#include "pointcharge.h"

#include <QGraphicsScene>
#include <QRadialGradient>
#include <QStyleOption>
#include <QPainter>

#include "mainwindow.h"

PointCharge::PointCharge(MainWindow *w) :
        window(w)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1.0);

    charge = 1.0;
}

QRectF PointCharge::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust,
                   20 + adjust, 20 + adjust);
}

QPainterPath PointCharge::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void PointCharge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRadialGradient gradient(-3, -3, 10);
    if (option-> state & QStyle::State_Selected) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
}

QVariant PointCharge::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange && parentItem()) {
        // value is the new position.
        QPointF newPos = value.toPointF();
        QRectF rect = parentItem()->boundingRect();
        if (!rect.contains(newPos)) {
            // Keep the item inside the scene rect.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            window->selectedPosChanged(newPos);
            return newPos;
        }
        window->selectedPosChanged(value.toPointF());
    }
    return QGraphicsItem::itemChange(change, value);
}
