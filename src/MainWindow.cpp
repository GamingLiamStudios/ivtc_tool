#include "MainWindow.hpp"
#include "ColorSpace.hpp"

#include <QColorSpace>
#include <QSettings>
#include <QRgb>
#include <QtLogging>
#include <QGraphicsVideoItem>

/// Generates a SMPTE HD Color Test Pattern according to RP.219-1
QVideoFrame generateColorBars(QSize frame_size)
{
    if (frame_size.isEmpty() || !frame_size.isValid())
    {
        frame_size = QSize(864, 480);    // Fallback to a default resolution
    }
    qDebug("Generating ColorBars for resolution %dx%d", frame_size.width(), frame_size.height());

    QImage canvas(frame_size, QImage::Format_RGBX8888);
    canvas.setColorSpace(generateColorSpace(Primaries::BT709, Transfer::BT709));

    QPainter painter(&canvas);
    painter.setRenderHint(QPainter::Antialiasing, false);

    painter.fillRect(QRect(QPoint(0, 0), frame_size), QColor::fromRgb(255, 0, 0));

    auto a = (double) frame_size.width();
    auto b = (double) frame_size.height() / 12.0;
    auto d = a / 8.0;
    auto c = (a - d * 2.0) / 7.0;

    // Colors
    YCbCr pattern1[] = {
        YCbCr(721, 512, 512),    // 75% White,
        YCbCr(674, 176, 543),    // 75% Yellow
        YCbCr(581, 589, 176),    // 75% Cyan
        YCbCr(534, 253, 207),    // 75% Green
        YCbCr(251, 771, 817),    // 75% Magenta
        YCbCr(204, 435, 848),    // 75% Red
        YCbCr(111, 848, 481),    // 75% Blue
    };

    /// Pattern 1
    auto y  = 0.0;
    auto ny = b * 7.0;
#define h ((int) ny - (int) y)

    auto x  = 0.0;
    auto nx = d;
#define w ((int) nx - (int) x)
    auto inc_x = [&x, &nx](double inc)
    {
        x = nx;
        nx += inc;
    };

    painter.fillRect(QRect(x, y, w, h), YCbCr(414, 512, 512).toQColor(Matrix::BT709));

    // 75% Color bars
    for (int i = 0; i < 7; i++)
    {
        inc_x(c);
        auto color = pattern1[i].toQColor(Matrix::BT709);
        painter.fillRect(QRect(x, y, w, h), color);
    }

    inc_x(d);
    painter.fillRect(QRect(x, y, w, h), YCbCr(414, 512, 512).toQColor(Matrix::BT709));

    /// Pattern 2
    y  = ny;
    ny = b * 8.0;

    nx = 0.0;

    inc_x(d);
    painter.fillRect(QRect(x, y, w, h), YCbCr(754, 615, 64).toQColor(Matrix::BT709));
    inc_x(c);
    painter.fillRect(QRect(x, y, w, h), YCbCr(940, 512, 512).toQColor(Matrix::BT709));
    inc_x(c * 6.0);
    painter.fillRect(QRect(x, y, w, h), YCbCr(721, 512, 512).toQColor(Matrix::BT709));
    inc_x(d);
    painter.fillRect(QRect(x, y, w, h), YCbCr(127, 960, 471).toQColor(Matrix::BT709));

    /// Pattern 3
    y  = ny;
    ny = b * 9.0;

    x  = 0.0;
    nx = d;

    painter.fillRect(QRect(x, y, w, h), YCbCr(877, 64, 553).toQColor(Matrix::BT709));
    inc_x(c);
    painter.fillRect(QRect(x, y, w, h), YCbCr(141, 697, 606).toQColor(Matrix::BT709));

    x = std::trunc(nx);
    for (int i = 0; i < c * 5.0; i++)
    {
        painter.fillRect(
          QRect(x + i, y, 1, h),
          YCbCr(64.0 + (double(i) * (940.0 - 64.0)) / (c * 5.0), 512, 512).toQColor(Matrix::BT709));
    }
    nx += c * 5.0;
    inc_x(c);
    painter.fillRect(QRect(x, y, w, h), YCbCr(940, 512, 512).toQColor(Matrix::BT709));
    inc_x(d);
    painter.fillRect(QRect(x, y, w, h), YCbCr(250, 409, 960).toQColor(Matrix::BT709));

    /// Pattern 4

    // 1/3 height slices
    for (int i = 0; i < 3; i++)
    {
        y = ny;
        ny += b;

        nx = d;
        inc_x(3.0 / 2.0 * c);
        painter.fillRect(
          QRect(x, y, w, h),
          YCbCr(i == 1 ? 4 : 64, 512, 512).toQColor(Matrix::BT709));

        inc_x(2.0 * c);
        painter.fillRect(
          QRect(x, y, w, h),
          YCbCr(i == 1 ? 1019 : 940, 512, 512).toQColor(Matrix::BT709));
    }

    // full height slices
    y  = b * 9.0;
    ny = b * 12.0;

    x  = 0.0;
    nx = d;
    painter.fillRect(QRect(x, y, w, h), YCbCr(195, 512, 512).toQColor(Matrix::BT709));

    x  = d + 7.0 / 2.0 * c;
    nx = x + 5.0 / 6.0 * c;
    painter.fillRect(QRect(x, y, w, h), YCbCr(64, 512, 512).toQColor(Matrix::BT709));

    inc_x(c / 3.0);
    painter.fillRect(QRect(x, y, w, h), YCbCr(46, 512, 512).toQColor(Matrix::BT709));
    inc_x(c / 3.0);
    painter.fillRect(QRect(x, y, w, h), YCbCr(64, 512, 512).toQColor(Matrix::BT709));
    inc_x(c / 3.0);
    painter.fillRect(QRect(x, y, w, h), YCbCr(82, 512, 512).toQColor(Matrix::BT709));
    inc_x(c / 3.0);
    painter.fillRect(QRect(x, y, w, h), YCbCr(64, 512, 512).toQColor(Matrix::BT709));
    inc_x(c / 3.0);
    painter.fillRect(QRect(x, y, w, h), YCbCr(99, 512, 512).toQColor(Matrix::BT709));

    inc_x(c);
    painter.fillRect(QRect(x, y, w, h), YCbCr(64, 512, 512).toQColor(Matrix::BT709));
    inc_x(d);
    painter.fillRect(QRect(x, y, w, h), YCbCr(195, 512, 512).toQColor(Matrix::BT709));

    painter.end();

    QVideoFrameFormat format(
      frame_size,
      QVideoFrameFormat::pixelFormatFromImageFormat(canvas.format()));
    format.setColorRange(QVideoFrameFormat::ColorRange_Video);

    QVideoFrame video_frame(format);
    if (video_frame.map(QVideoFrame::MapMode::WriteOnly))
    {
        Q_ASSERT(video_frame.size() == canvas.size());

        uchar *dst       = video_frame.bits(0);
        auto   dstStride = video_frame.bytesPerLine(0);

        const uchar *src       = canvas.constBits();
        auto         srcStride = canvas.bytesPerLine();

        auto linesToCopy = video_frame.height();
        auto stride      = qMin(srcStride, dstStride);    // Should be the same anyway
        // qDebug("%lld, %d", srcStride, dstStride);
        for (auto y = 0; y < linesToCopy; y++)
        {
            memcpy(dst + dstStride * y, src + srcStride * y, stride);
        }
    }

    video_frame.unmap();
    return video_frame;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    loadSettings();

    this->setWindowTitle("gls' (bad) ivtc tool");
    this->createMenuBar();

    QGraphicsView *view = new QGraphicsView(this);
    view->setAlignment(Qt::AlignCenter);
    view->setContentsMargins(0, 0, 0, 0);
    view->setFrameStyle(QFrame::NoFrame);
    this->setCentralWidget(view);

    QGraphicsScene *scene = new QGraphicsScene(view);
    view->setScene(scene);

    QGraphicsVideoItem *videoItem = new QGraphicsVideoItem();
    scene->addItem(videoItem);

    videoItem->setSize(QSizeF(864.0, 480.0));
    //  scene->setSceneRect(0, 0, 1280.0, 720.0);

    this->m_videoSink = videoItem->videoSink();
    this->m_videoSink->setVideoFrame(generateColorBars(QSize(864, 480)));

    connect(
      this->m_videoSink,
      &QVideoSink::videoSizeChanged,
      [videoItem]()
      {
          auto newSize = videoItem->videoSink()->videoSize();
          videoItem->setSize(QSizeF(newSize));
      });
    connect(&this->m_projectManager, &ProjectManager::pixelAspectChanged, [](const qreal pixelAspectRatio) {

    });
}

MainWindow::~MainWindow()
{
    saveSettings();
}

void MainWindow::updateScene()
{
    // this->m_scene->clear();
    //  this->m_scene->setSceneRect(this->m_visible_frame.);
    //  this->m_scene->addPixmap(QPixmap::fromImage(this->m_visible_frame, Qt::ThresholdDither));
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
    auto activeProject = m_projectManager.activeProject();
    if (activeProject != nullptr && activeProject->has_changed())
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
        if (reply == QMessageBox::Yes) { activeProject->save(); }
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
    auto activeProject = m_projectManager.activeProject();
    if (activeProject != nullptr && activeProject->has_changed())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
          this,
          "Save?",
          "Would you like to save your changes?",
          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Cancel) { return; }
        if (reply == QMessageBox::Yes) { activeProject->save(); }
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
    Project *project    = new Project(video_path);
    this->m_projectManager.setActiveProject(project);

    settings.setValue("project/videoDirectory", dialog.directory().absolutePath());

    qInfo("New Project Created!");
}