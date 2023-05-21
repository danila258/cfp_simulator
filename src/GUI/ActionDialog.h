#ifndef ACTIONDIALOG_H
#define ACTIONDIALOG_H

#include "DefaultObjects.h"
#include "DataTransferInterface.h"
#include "ObjectRowWidget.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVBoxLayout>

#include <memory>


class ActionDialog : public QDialog
{
    Q_OBJECT

public:
    ActionDialog(size_t thread, const UniversalString& className, const std::vector<defaultParam>& defaultActions,
                 size_t minId, size_t maxId, QWidget* parent = nullptr);

    actionContent getAction();

private slots:
    void actionSlot(int index);
    void pauseSlot(int value);
    void idSlot(int value);

private:
    const std::vector<defaultParam>& _defaultActions;

    std::shared_ptr<QVBoxLayout> _actionLayout;
    std::shared_ptr<ObjectRowWidget> _actionRow;

    size_t _thread;
    UniversalString _className;

    size_t _maxId;
    size_t _minId;

    UniversalString _funcName;
    int _pause = 500;
    size_t _id;

    QHBoxLayout* getFormLayout(QLabel* label, QWidget* field, QWidget* parent = nullptr);
};


#endif //ACTIONDIALOG_H
