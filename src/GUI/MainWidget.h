#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ObjectCreatorWidget.h"
#include "ThreadsTreeWidget.h"
#include "ConfigsWidget.h"
#include "ConfigParser.h"
#include "UniversalString.h"
#include "DataTransferInterface.h"
#include "MainLogic.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QVector>
#include <QFileDialog>
#include <QMessageBox>

#include <memory>


class MainWidget : public QWidget
{
Q_OBJECT

public:
    explicit MainWidget(MainLogic& logic);

public slots:
    void changeThreadIndexSlot(int index);
    void addThreadSlot();
    void removeThreadSlot(int index);
    void updateThreadContentSlot(const std::vector<objectContent>& content);

    void changeConfigIndexSlot(int index);
    void addConfigSlot();
    void removeConfigSlot(int index);

signals:
    void updateThreadTreeSignal(const std::vector<objectContent>& content);
    void addConfigSignal(const QString& config);

private:
    MainLogic& _logic;

    std::shared_ptr<ObjectCreatorWidget> _objectCreatorWidget;
    std::shared_ptr<ThreadsTreeWidget> _threadsTreeWidget;
    std::shared_ptr<ConfigsWidget> _configsWidget;

    size_t _threadIndex = 0;
    size_t _configIndex = 0;

    std::vector<std::vector<threadContent>> _configs;

    bool _skipUpdateObjectCreator = false;

private slots:
    void openButtonSlot();
    void saveButtonSlot();
    void runButtonSlot();

};


#endif //MAINWINDOW_H
