#include "MainWindow.hpp"

#include <QColorSpace>

template<std::size_t length>
std::array<uint16_t, length> create_rec601_transfer()
{
    std::array<uint16_t, length> transfer {};
    double                       luma_scale = static_cast<double>(length) - 1.0;

    for (int i = 0; i < length; i++)
    {
        double luma = static_cast<double>(i) / luma_scale;

        double encoded = 0.0;
        if (luma < 0.018) { encoded = luma * 4.5; }
        else
        {
            encoded = 1.099 * std::pow(luma, 0.45) - 0.099;
        }

        transfer[i] = static_cast<uint16_t>(encoded * 65535.0);
    }

    return transfer;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->resize(640, 480);
    this->setWindowTitle("gls' (bad) ivtc tool");

    this->createMenuBar();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenuBar()
{
    QAction *newAct = new QAction(this);
    newAct->setShortcut(QKeySequence::New);
    newAct->setText(tr("Create a New Project"));

    connect(newAct, &QAction::triggered, this, &MainWindow::newProject);

    auto menuBar = this->menuBar()->addMenu(QString("File"));
    menuBar->addAction(newAct);
}

void MainWindow::newProject()
{ qInfo("get fucked ha lol"); }