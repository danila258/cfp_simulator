#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ConfigParser.h"
#include "ObjectCreatorWidget.h"
#include "DataTransferInterface.h"

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
    ObjectCreatorWidget* _objectCreatorWidget;
};


#endif //MAINWINDOW_H
