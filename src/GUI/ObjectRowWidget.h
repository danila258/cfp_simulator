#ifndef OBJECTROW_H
#define OBJECTROW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>

#include <QDebug>


struct RTLibObject
{
    int id;
    QString varName;
    QString className;
    QVector<QPair<QString, QString>> argVal;
};


class ObjectRowWidget : public QWidget
{
Q_OBJECT

public:
    ObjectRowWidget(RTLibObject defaultObject, QWidget* parent = nullptr);

    ~ObjectRowWidget() override;

    QString getClassName() const;
    RTLibObject getUserInput() const;

    void setValues(const QVector<QString>& values);
    void setId(int id);

private slots:
    void varNameChanged(const QString& varName);

private:
    RTLibObject _defaultObject;

    QLineEdit* _idLineEdit;

    QString _varName;
    QLineEdit* _varNameLineEdit;

    QVector<QLineEdit*> _lineEdits;
};


#endif //OBJECTROW_H
