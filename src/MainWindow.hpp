#pragma once

#include <QtWidgets>
#include <qtmetamacros.h>
#include "Project.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    // Q_PROPERTY(Project *loadedProject MEMBER m_loaded_project NOTIFY projectChanged)
    Q_PROPERTY(QImage visibleFrame MEMBER m_visible_frame NOTIFY frameChanged)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void projectChanged(const Project *newProject);
    void frameChanged();

protected:
    void closeEvent(QCloseEvent *event) override;

protected slots:
    void updateScene();

private:
    void createMenuBar();
    void newProject();

    void loadSettings();
    void saveSettings();

    void paintColorBars();

private:
    Project *m_loaded_project = nullptr;

    QGraphicsScene *m_scene;
    QImage          m_visible_frame;
};
