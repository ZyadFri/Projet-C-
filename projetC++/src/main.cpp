#include <QApplication>
#include "GUI/MainWindow.h"

/*!
 * \brief Entry point for the Qt-based blockchain application.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
