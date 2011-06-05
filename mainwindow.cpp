#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pointcharge.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    chargeArea = scene.addRect(0, 0, 400, 400, QPen(Qt::black), QBrush(Qt::lightGray));
    pixmap = new QGraphicsPixmapItem(chargeArea, &scene);
    scene.setSceneRect(0,0, 400, 400);
    ui->graphicsView->setScene(&scene);

    connect(&scene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::selectionChanged()
{
    QList<QGraphicsItem *> items = scene.selectedItems();
    if (items.count()==1) {
        selectedCharge = (PointCharge*) items.first();
        selectedPosChanged(selectedCharge->pos());
        ui->sbCharge->setValue( selectedCharge->charge );
        ui->gbCharge->setEnabled(true);
    } else {
        ui->gbCharge->setEnabled(false);
        selectedCharge = NULL;
    }
}

void MainWindow::selectedPosChanged(const QPointF & p)
{
    ui->sbX->setValue( p.x() );
    ui->sbY->setValue( p.y() );
}

void MainWindow::addCharge()
{
    PointCharge *p = new PointCharge(this);
    p->setParentItem(chargeArea);
}

void MainWindow::removeCharge()
{
    if (selectedCharge) {
        delete selectedCharge;
        selectedCharge = NULL;
    }
}

void MainWindow::removeAll()
{
    qDeleteAll(chargeArea->childItems());
}

void MainWindow::changeChargeX(double value)
{
    selectedCharge->setX(value);
}

void MainWindow::changeChargeY(double value)
{
    selectedCharge->setY(value);
}

void MainWindow::changeChargeQ(double value)
{
    selectedCharge->charge = value;
}

void MainWindow::renderP()
{
    renderer.render(chargeArea);
    pixmap->setPixmap(QPixmap::fromImage(renderer.img));
}





