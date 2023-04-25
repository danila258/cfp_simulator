#include "MainWidget.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    auto* a = new MainWidget();
    a->show();

    return app.exec();
}
