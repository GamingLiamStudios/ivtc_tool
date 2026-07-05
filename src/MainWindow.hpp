#pragma once

#include <QtCore>
#include <QtWidgets>
#include <QtMultimedia>

#include "ProjectManager.hpp"
#include <QVideoWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    // Q_PROPERTY(Project *loadedProject MEMBER m_loaded_project NOTIFY projectChanged)
    // Q_PROPERTY(QImage visibleFrame MEMBER m_visible_frame NOTIFY frameChanged)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // void frameChanged();

protected:
    void closeEvent(QCloseEvent *event) override;

protected slots:
    void updateScene();

private:
    void createMenuBar();
    void newProject();

    void loadSettings();
    void saveSettings();

private:
    ProjectManager m_projectManager;

    // QVideoWidget *m_video_widget;
    QVideoSink *m_videoSink;
};
