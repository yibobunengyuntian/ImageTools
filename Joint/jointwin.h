#ifndef JOINTWIN_H
#define JOINTWIN_H

#include <QWidget>
#include <QFileDialog>
#include "ui_jointwin.h"
#include "defines.h"

class JointWin : public QWidget, public Ui_JointWin
{
    Q_OBJECT

public:
    explicit JointWin(QWidget *parent = nullptr);
protected:
    void initializeUI();
    void initializeConnect();

protected slots:
    void onOpen1();
    void onOpen2();
    void onSave();
    void onSave(bool _1_or_2);
    void onTypeChanged();
    void onSwitchover(bool _1_or_2);
private:
    QString m_name1;
    QString m_name2;

private:

};

#endif // JOINTWIN_H
