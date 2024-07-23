#ifndef SEGMENTATIONWIN_H
#define SEGMENTATIONWIN_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include "ui_segmentationwin.h"

class SegmentationWin : public QWidget, public Ui_SegmentationWin
{
    Q_OBJECT

public:
    explicit SegmentationWin(QWidget *parent = nullptr);

protected:
    void initializeUI();
    void initializeConnect();

protected slots:
    void onOpen();
    void onSave();
    void onRatioChanged();
    void onTypeChanged();
    void onSaveAll();
    void onSaveP1();
    void onSaveP2();
private:
    QString m_name;
};

#endif // SEGMENTATIONWIN_H
