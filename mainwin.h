#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <QMouseEvent>
#include "ui_mainwin.h"
#include "segmentationwin.h"
#include "jointwin.h"
#include "configfile.h"

class MainWin : public QWidget, Ui_MainWin
{
    Q_OBJECT

public:
    MainWin(QWidget *parent = nullptr);
    void addWidget(QWidget *widget);

protected:
    void initializeUI();
    void initializeConnect();
    bool eventFilter(QObject *obj, QEvent *event);
    //覆写closeEvent函数
    void closeEvent(QCloseEvent *event);
    //取消选中
    void deselect(QMouseEvent *event);
protected slots:
    void onItemDoubleClicked();
private:
    QMap<QListWidgetItem*, QWidget*> m_widgets;
};
#endif // MAINWIN_H
