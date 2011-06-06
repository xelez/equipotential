#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pointcharge.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap background(400, 400);
    background.fill(Qt::lightGray);
    QPainter painter;
    painter.begin(&background);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(Qt::lightGray));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRect(0, 0, 400, 400);
    painter.end();

    chargeArea = scene.addPixmap(background);

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

void MainWindow::setChargesVisible(int value)
{
    ui->pbAddCharge->setEnabled(value);
    foreach(QGraphicsItem *charge, chargeArea->childItems())
        charge->setVisible(value);
}

void MainWindow::renderP()
{
    renderer.render(chargeArea);
    chargeArea->setPixmap(QPixmap::fromImage(renderer.img));
}





