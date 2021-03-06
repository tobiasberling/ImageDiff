#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QDebug>
#include <QRgb>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->image_diff->setType(MyImageLabelType::Output);

    connect(ui->image_1, &MyImageLabel::changed, this, &MainWindow::onImageChanged);
    connect(ui->image_2, &MyImageLabel::changed, this, &MainWindow::onImageChanged);


    auto iml1 = reinterpret_cast<MyImageLabel*>(ui->image_1);
    auto iml2 = reinterpret_cast<MyImageLabel*>(ui->image_2);

    if (qApp->arguments().size() > 1)
        iml1->loadImage(qApp->arguments()[1]);
    if (qApp->arguments().size() > 2)
        iml2->loadImage(qApp->arguments()[2]);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onImageChanged()
{
    auto iml1 = reinterpret_cast<MyImageLabel*>(ui->image_1);
    auto iml2 = reinterpret_cast<MyImageLabel*>(ui->image_2);
    auto iml_diff = reinterpret_cast<MyImageLabel*>(ui->image_diff);

    auto im1 = iml1->getOriginalImage();
    auto im2 = iml2->getOriginalImage();

    if (im1->isNull() || im2->isNull())
        return;

    const auto diffImageSize = QSize(qMax(im1->width(), im2->width()), qMax(im1->height(), im2->height()));

    auto diffImage = QImage(diffImageSize, im1->format());

    bool identicalImages = true;

    for (int x = 0; x < diffImageSize.width(); x++)
    {
        for (int y = 0; y < diffImageSize.height(); y++)
        {
            if (im1->width() <= x || im2->width() <= x || im1->height() <= y || im2->height() <= y)
            {
                diffImage.setPixel(x, y, qRgb(255, 0, 0));
                identicalImages = false;
            }
            else if (im1->pixel(x, y) != im2->pixel(x, y))
            {
                diffImage.setPixel(x, y, qRgb(0, 255, 0));
                identicalImages = false;
            }
            else
            {
                diffImage.setPixel(x, y, im1->pixel(x, y));
            }
        }
    }
    if (identicalImages)
    {
        ui->image_diff->setText("Images are identical");
    }
    else
    {
        iml_diff->setOriginalImage(diffImage);
    }
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}
