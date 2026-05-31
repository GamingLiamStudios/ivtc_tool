#pragma once

#include <QtWidgets>

class MainWindow : public QMainWindow
{
    // Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createMenuBar();

    void newProject();

private:
};