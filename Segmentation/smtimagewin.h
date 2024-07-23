#ifndef SMTIMAGEWIN_H
#define SMTIMAGEWIN_H

#include <QWidget>
#include <QPainter>
#include <QVector2D>
#include <QMimeData>
#include <QMouseEvent>
#include <QMenu>
#include <QDebug>
#include "ui_smtimagewin.h"
#include "defines.h"

class SmtImageWin : public QWidget, public Ui_SmtImageWin
{
    Q_OBJECT

public:
    explicit SmtImageWin(QWidget *parent = nullptr);

    bool type();
    QVector2D ratio();
    QImage image();

signals:
    void ratioChanged(QVector2D ratio);
    void openImage();
    void saveAll();
    void saveP1();
    void saveP2();
public slots:
    void onSetImage(const QString &path);
    void onSetType(const bool &type);
    void onSetRatio(const QVector2D &ratio);
    void onRotate_l();
    void onRotate_r();
    void onOverturn_ud();
    void onOverturn_lr();

protected:
    void initializeMenu();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *pEvent) override;
    virtual void mouseMoveEvent(QMouseEvent *pEvent) override;
    virtual void mouseReleaseEvent(QMouseEvent *pEvent) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override; // 拖动进入事件
    virtual void dropEvent(QDropEvent *event) override;           // 放下事件
    virtual void dragMoveEvent(QDragMoveEvent *event) override;

private:
    QImage m_image;
    QRect m_imageRect;
    bool m_type = true; //(false:左右，true:上下)
    QVector2D m_ratio = QVector2D(1, 1);
    bool m_isMouseDown = false;
    QMenu *m_pMenu = nullptr;
};

#endif // SMTIMAGEWIN_H
