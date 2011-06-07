#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pointcharge.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    chargeArea = new QGraphicsPixmapItem(0, &scene);
    ui->graphicsView->setScene(&scene);

    ui->sbFieldWidth->setValue(600);
    ui->sbFieldHeight->setValue(600);

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

QPixmap MainWindow::defaultBackground(int w, int h)
{
    QPixmap background(w, h);
    background.fill(Qt::lightGray);
    QPainter painter;
    painter.begin(&background);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(Qt::lightGray));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRect(0, 0, w, h);
    painter.end();
    return background;
}

void MainWindow::fieldSizeChanged()
{
    removeAll();

    const int w = ui->sbFieldWidth->value();
    const int h = ui->sbFieldHeight->value();

    ui->sbX->setMaximum(w);
    ui->sbY->setMaximum(h);
    chargeArea->setPixmap(defaultBackground(w,h));
    scene.setSceneRect(0,0, w, h);    
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
    renderer.render(chargeArea, Renderer::calcPotential);
    chargeArea->setPixmap(QPixmap::fromImage(renderer.img));
}

void MainWindow::renderE()
{
    renderer.render(chargeArea, Renderer::calcElectricField);
    chargeArea->setPixmap(QPixmap::fromImage(renderer.img));
}




