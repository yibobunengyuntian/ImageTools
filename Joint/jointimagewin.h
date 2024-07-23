#ifndef JOINTIMAGEWIN_H
#define JOINTIMAGEWIN_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMimeData>
#include <QMouseEvent>
#include <QMenu>
#include <QStyle>
#include "ui_jointimagewin.h"
#include "defines.h"

class JointImageWin : public QWidget, public Ui_JointImageWin
{
    Q_OBJECT

public:
    explicit JointImageWin(QWidget *parent = nullptr);

    bool type();
    QImage image1();
    QImage image2();

signals:
    void openImage1();
    void openImage2();

    void save();
    void save(bool _1_or_2);
    void switchover(bool _1_or_2);

public slots:
    void onSetImage1(const QString &path);
    void onSetImage2(const QString &path);
    void onSwap();
    void onSetType(const bool &type);

    void onRotate_l();
    void onRotate_r();
    void onOverturn_ud();
    void onOverturn_lr();
    void onSwitchover();
    void onSave();

protected:
    void initializeMenu();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *pEvent) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override; // 拖动进入事件
    virtual void dropEvent(QDropEvent *event) override;           // 放下事件
    virtual void dragMoveEvent(QDragMoveEvent *event) override;

private:
    QImage m_image1;
    QImage m_image2;
    QRect m_imageRect1;
    QRect m_imageRect2;
    bool m_type = true; //(false:左右，true:上下)
    QMenu *m_pMenu = nullptr;
    bool m_1_or_2 = true;
};

#endif // JOINTIMAGEWIN_H
