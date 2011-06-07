#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "pointcharge.h"
#include "renderer.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    QPixmap defaultBackground(int w, int h);
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    QGraphicsPixmapItem *chargeArea;
    PointCharge *selectedCharge;

    Renderer renderer;

public slots:
    void selectionChanged();
    void selectedPosChanged(const QPointF & p);
    void addCharge();
    void removeCharge();
    void removeAll();
    void changeChargeX(double value);
    void changeChargeY(double value);
    void changeChargeQ(double value);
    void setChargesVisible(int value);
    void fieldSizeChanged();
    void renderP();
};

#endif // MAINWINDOW_H
