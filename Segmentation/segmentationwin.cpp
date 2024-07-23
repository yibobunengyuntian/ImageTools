#include "segmentationwin.h"
#include "configfile.h"
#include "defines.h"

SegmentationWin::SegmentationWin(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    initializeUI();
    initializeConnect();
}

void SegmentationWin::initializeUI()
{
    this->setWindowIcon(QIcon(":/res/image/SMT.png"));
    this->setWindowTitle("图片分割");
    m_pType->clear();
    m_pType->addItems(QStringList() << "上下" << "左右");
}

void SegmentationWin::initializeConnect()
{
    connect(m_pBtnOpen, SIGNAL(clicked(bool)), this, SLOT(onOpen()));
    connect(m_pBtnSave, SIGNAL(clicked(bool)), this, SLOT(onSave()));
    connect(m_pType, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeChanged()));
    connect(m_pRatio1, SIGNAL(valueChanged(int)), this, SLOT(onRatioChanged()));
    connect(m_pRatio2, SIGNAL(valueChanged(int)), this, SLOT(onRatioChanged()));
    connect(m_pSMTImageWin, SIGNAL(openImage()), this, SLOT(onOpen()));
    connect(m_pSMTImageWin, SIGNAL(saveAll()), this, SLOT(onSaveAll()));
    connect(m_pSMTImageWin, SIGNAL(saveP1()), this, SLOT(onSaveP1()));
    connect(m_pSMTImageWin, SIGNAL(saveP2()), this, SLOT(onSaveP2()));
    connect(m_pSMTImageWin, &SmtImageWin::ratioChanged, this, [=](QVector2D ratio){
        m_pRatio1->setValue(ratio.x()/(ratio.x() + ratio.y())*100);
        m_pRatio2->setValue(ratio.y()/(ratio.x() + ratio.y())*100);
    });
}

void SegmentationWin::onOpen()
{
    QString dirPath = g_Config->Get(metaObject()->className(), OPEN_PATH).toString();
    QString path = QFileDialog::getOpenFileName(nullptr,tr("Open Project"),
                                                dirPath, tr("Files (%1)").arg(IMAGE_FORMAT));
    if(path.isEmpty()){
        return;
    }
    QString savePath = path;
    m_name = savePath.split("/").last();
    m_name = m_name.split(".").first();
    savePath.chop(savePath.split("/").last().count() + 1);
    g_Config->Set(metaObject()->className(), OPEN_PATH, savePath);
    m_pSMTImageWin->onSetImage(path);
}

void SegmentationWin::onSave()
{
    QImage image = m_pSMTImageWin->image();
    if(image.isNull())
    {
        return;
    }
    QString dirPath = g_Config->Get(metaObject()->className(), SAVE_PATH).toString();
    QString proDir = QFileDialog::getExistingDirectory(nullptr,QStringLiteral("选择路径"),dirPath);
    if(proDir.isEmpty()){
        return;
    }
    // 切割并保存图片
    QImage p1, p2;
    float ratio = m_pSMTImageWin->ratio().x()/(m_pSMTImageWin->ratio().x() + m_pSMTImageWin->ratio().y());
    qDebug() << ratio;
    if(m_pSMTImageWin->type())
    {
        p1 = image.copy(0, 0, image.width(), int(ratio*image.height()));
        p2 = image.copy(0, int(ratio*image.height()) + 1, image.width(), image.height() - int(ratio*image.height()));
    }
    else
    {
        p1 = image.copy(0, 0, int(ratio*image.width()), image.height());
        p2 = image.copy(int(ratio*image.width()) + 1, 0, image.width() - int(ratio*image.width()), image.height());
    }
    p1.save(proDir + "/" + m_name + "_p1" + ".png");
    p2.save(proDir + "/" + m_name + "_p2" + ".png");
    g_Config->Set(metaObject()->className(), SAVE_PATH, proDir);
}

void SegmentationWin::onRatioChanged()
{
    m_pSMTImageWin->onSetRatio(QVector2D(m_pRatio1->value(), m_pRatio2->value()));
    m_pSMTImageWin->update();
}

void SegmentationWin::onTypeChanged()
{
    m_pSMTImageWin->onSetType(m_pType->currentIndex() == 0);
    m_pSMTImageWin->update();
}

void SegmentationWin::onSaveAll()
{
    if(m_pSMTImageWin->image().isNull())
    {
        return;
    }
    QString dirPath = g_Config->Get(metaObject()->className(), SAVE_PATH).toString();
    auto path = QFileDialog::getSaveFileName(nullptr,tr("Save Image"),
                                                dirPath + "/" + m_name, tr("Image Files (*.png)"));
    if(path.isEmpty()){
        return;
    }
    m_pSMTImageWin->image().save(path);
    QString savePath = path;
    savePath.chop(savePath.split("/").last().count() + 1);
    g_Config->Set(metaObject()->className(), SAVE_PATH, savePath);
}

void SegmentationWin::onSaveP1()
{
    QImage image = m_pSMTImageWin->image();
    if(image.isNull())
    {
        return;
    }
    QString dirPath = g_Config->Get(metaObject()->className(), SAVE_PATH).toString();
    auto path = QFileDialog::getSaveFileName(nullptr,tr("Save Image"),
                                                dirPath + "/" + m_name + "_p1", tr("Image Files (*.png)"));
    if(path.isEmpty()){
        return;
    }
    // 切割并保存图片
    QImage p1;
    float ratio = m_pSMTImageWin->ratio().x()/(m_pSMTImageWin->ratio().x() + m_pSMTImageWin->ratio().y());
    if(m_pSMTImageWin->type())
    {
        p1 = image.copy(0, 0, image.width(), int(ratio*image.height()));
    }
    else
    {
        p1 = image.copy(0, 0, int(ratio*image.width()), image.height());
    }
    p1.save(path);
    QString savePath = path;
    savePath.chop(savePath.split("/").last().count() + 1);
    g_Config->Set(metaObject()->className(), SAVE_PATH, savePath);
}

void SegmentationWin::onSaveP2()
{
    QImage image = m_pSMTImageWin->image();
    if(image.isNull())
    {
        return;
    }
    QString dirPath = g_Config->Get(metaObject()->className(), SAVE_PATH).toString();
    auto path = QFileDialog::getSaveFileName(nullptr,tr("Save Image"),
                                                dirPath + "/" + m_name + "_p2", tr("Image Files (*.png)"));
    if(path.isEmpty()){
        return;
    }
    // 切割并保存图片
    QImage p2;
    float ratio = m_pSMTImageWin->ratio().x()/(m_pSMTImageWin->ratio().x() + m_pSMTImageWin->ratio().y());
    if(m_pSMTImageWin->type())
    {
        p2 = image.copy(0, int(ratio*image.height()) + 1, image.width(), image.height() - int(ratio*image.height()));
    }
    else
    {
        p2 = image.copy(int(ratio*image.width()) + 1, 0, image.width() - int(ratio*image.width()), image.height());
    }
    p2.save(path);
    QString savePath = path;
    savePath.chop(savePath.split("/").last().count() + 1);
    g_Config->Set(metaObject()->className(), SAVE_PATH, savePath);
}
