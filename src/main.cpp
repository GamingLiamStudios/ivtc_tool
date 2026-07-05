#include <QtLogging>
#include <QtTranslation>
#include <QtWidgets>
#include <qcoreapplication.h>

#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    qInfo("Hello World!");

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("GLStudios");
    QCoreApplication::setOrganizationDomain("glstudios.org");
    QCoreApplication::setApplicationName("ivtc_tool");

    MainWindow window;
    window.show();

    return app.exec();
}

#include "moc_ProjectManager.cpp"