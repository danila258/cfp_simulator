#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ConfigParser.h"
#include "ObjectCreatorWidget.h"
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
    MainWidget();

private:
    std::shared_ptr<ObjectCreatorWidget> _objectCreatorWidget;

private slots:
    void openButtonSlot();
    void saveButtonSlot();
    void runButtonSlot();
};


#endif //MAINWINDOW_H
