#include "MainWindow.hpp"
#include "ColorSpace.hpp"

#include <QColorSpace>
#include <QSettings>
#include <QRgb>
#include <QtCore>
#include <qrgb.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    loadSettings();

    this->setWindowTitle("gls' (bad) ivtc tool");
    this->createMenuBar();

    this->m_scene      = new QGraphicsScene();
    auto graphics_view = new QGraphicsView();
    graphics_view->setScene(this->m_scene);
    this->setCentralWidget(graphics_view);
    connect(this, &MainWindow::frameChanged, &MainWindow::updateScene);

    this->paintColorBars();
}

MainWindow::~MainWindow()
{
    saveSettings();

    if (this->m_loaded_project != nullptr)
    {
        // Clean state
        delete this->m_loaded_project;
    }
}

void MainWindow::updateScene()
{
    this->m_scene->clear();
    this->m_scene->setSceneRect(this->m_visible_frame.rect());
    this->m_scene->addPixmap(QPixmap::fromImage(this->m_visible_frame, Qt::ThresholdDither));
}

void MainWindow::loadSettings()
{
    QSettings settings;

    auto window_size = settings.value("window/size", QSize(640, 480)).toSize();
    auto window_loc  = settings.value("window/location", QPoint(0, 0)).toPoint();
    this->resize(window_size);
    this->move(window_loc);
}
void MainWindow::saveSettings()
{
    QSettings settings;

    settings.setValue("window/size", this->size());
    settings.setValue("window/location", this->pos());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (this->m_loaded_project != nullptr && this->m_loaded_project->has_changed())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
          this,
          "Save?",
          "Would you like to save your changes before exiting?",
          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
        if (reply == QMessageBox::Yes) { this->m_loaded_project->save(); }
    }
    event->accept();
}

void MainWindow::createMenuBar()
{
    QAction *newAct = new QAction(this);
    newAct->setText(tr("Create a New Project"));

    connect(newAct, &QAction::triggered, this, &MainWindow::newProject);

    auto menuBar = this->menuBar()->addMenu(QString("File"));
    menuBar->addAction(newAct);
}

void MainWindow::newProject()
{
    if (this->m_loaded_project != nullptr && this->m_loaded_project->has_changed())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
          this,
          "Save?",
          "Would you like to save your changes?",
          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Cancel) { return; }
        if (reply == QMessageBox::Yes) { this->m_loaded_project->save(); }
    }

    QSettings settings;

    QStringList   mime_types;
    QMimeDatabase database;
    for (const QMimeType &type : database.allMimeTypes())
        if (type.name().startsWith("video/")) mime_types.append(type.name());

    QFileDialog dialog(this, tr("Open Video File"));
    dialog.setMimeTypeFilters(mime_types);
    dialog.setFileMode(QFileDialog::FileMode::ExistingFile);
    dialog.setDirectory(settings.value("project/videoDirectory").toString());
    if (!dialog.exec()) { return; }

    auto     video_path = dialog.selectedFiles()[0];
    Project *ptr        = new Project(video_path);
    std::swap(this->m_loaded_project, ptr);

    settings.setValue("project/videoDirectory", dialog.directory().absolutePath());

    if (ptr != nullptr)
    {
        // Clean state
        delete ptr;
    }

    qInfo("New Project Created!");
    emit projectChanged(this->m_loaded_project);
}

void MainWindow::paintColorBars()
{
    QImage bars(854, 480, QImage::Format_RGBX8888);
    bars.setColorSpace(QColorSpace::NamedColorSpace::SRgb);
    QPainter painter(&bars);

    painter.fillRect(QRect(0, 0, 854, 480), QColor::fromRgb(255, 0, 0));

    int a = 854, b = 480, c = (3 * a) / (4 * 7), d = a / 8;

    // Colors
    YCbCr colors[] = {
        YCbCr(721, 512, 512),    // 75% White,
        YCbCr(674, 176, 543),    // 75% Yellow
        YCbCr(581, 589, 176),    // 75% Cyan
        YCbCr(534, 253, 207),    // 75% Green
        YCbCr(251, 771, 817),    // 75% Magenta
        YCbCr(204, 435, 848),    // 75% Red
        YCbCr(111, 848, 481),    // 75% Blue
        YCbCr(64, 512, 512),     // 75% Black,
        YCbCr(940, 512, 512),    // 100% White,
        YCbCr(141, 697, 606),    // +Q
        YCbCr(245, 412, 629),    // +I
        YCbCr(244, 612, 395),    // -I
    };

    // 40% Grey bars
    auto grey40 = YCbCr(414, 512, 512).toQColor(Matrix::BT709);
    painter.fillRect(QRect(0, 0, a, b / 12 * 7), grey40);

    // 75% Color bars
    for (int i = 0; i < 7; i++)
    {
        auto color = colors[i].toQColor(Matrix::BT709);
        painter.fillRect(QRect(d + i * c, 0, c, b / 12 * 7), color);
    }

    painter.end();
    this->m_visible_frame = bars;
    emit frameChanged();
}
