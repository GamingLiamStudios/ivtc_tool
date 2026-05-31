#pragma once

#include <QtWidgets>
#include "Project.hpp"

class MainWindow : public QMainWindow
{
    // Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void createMenuBar();

    void newProject();

private:
    Project *m_loaded_project = nullptr;
};