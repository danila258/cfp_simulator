#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ObjectCreatorWidget.h"
#include "ThreadsTreeWidget.h"
#include "ConfigsWidget.h"
#include "ConfigParser.h"
#include "UniversalString.h"
#include "DataTransferInterface.h"
#include "MainLogic.h"

#include <QWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QVector>

#include <memory>


class MainWidget : public QWidget
{
Q_OBJECT

public:
    explicit MainWidget(MainLogic& logic);

public slots:
    void changeThreadSlot(int index);

private:
    MainLogic& _logic;

    std::shared_ptr<ObjectCreatorWidget> _objectCreatorWidget;
    std::shared_ptr<ThreadsTreeWidget> _threadsTreeWidget;
    std::shared_ptr<ConfigsWidget> _configsWidget;

    size_t _threadIndex = 0;
    size_t _configIndex = 0;

    std::vector<std::vector<objectContent>> _threadsContent;

private slots:
    void openButtonSlot();
    void saveButtonSlot();
    void runButtonSlot();

};


#endif //MAINWINDOW_H
