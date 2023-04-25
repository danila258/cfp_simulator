#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ObjectCreatorWidget.h"

#include <QWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QVector>

#include <memory>


class MainWidget : public QWidget
{
public:
    MainWidget();

private:
    ObjectCreatorWidget* _objectCreatorWidget;
};


#endif //MAINWINDOW_H
