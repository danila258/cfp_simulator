#include "MainWidget.h"
#include "MainLogic.h"

#include <QApplication>

#include <iostream>


int main(int argc, char* argv[]) try
{
    MainLogic logic(argc, argv);

    if ( logic.hasGUI() )
    {
        QApplication app(argc, argv);

        auto* gui = new MainWidget(logic);
        gui->show();

        return QApplication::exec();
    }
    else
    {
        logic.runFromConsole();
    }
}
catch (std::exception& e)
{
    std::cerr << e.what() << std::endl;
}