#include "MainWindow.hpp"

#include <QColorSpace>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->resize(640, 480);
    this->setWindowTitle("gls' (bad) ivtc tool");

    this->createMenuBar();
}

MainWindow::~MainWindow()
{
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
    QStringList   mime_types;
    QMimeDatabase database;
    for (const QMimeType &type : database.allMimeTypes())
        if (type.name().startsWith("video/")) mime_types.append(type.name());

    QFileDialog dialog(this, tr("Open Video File"));
    dialog.setMimeTypeFilters(mime_types);
    if (!dialog.exec()) { return; }

    Project *ptr = new Project();
    ptr->set_video_path(dialog.selectedFiles()[0]);
    std::swap(this->m_loaded_project, ptr);

    if (ptr != nullptr)
    {
        // TODO: Throw "Do you want to save?" messagebox
        // if (this->m_loaded_project->has_changed())

        // Clean state
        delete ptr;
    }

    qInfo("New Project Created!");
}