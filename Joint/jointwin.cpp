#include "jointwin.h"
#include "configfile.h"
#include "defines.h"

JointWin::JointWin(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    initializeUI();
    initializeConnect();
}

void JointWin::initializeUI()
{
    this->setWindowIcon(QIcon(":/res/image/joint.png"));
    this->setWindowTitle("图片合并");
    m_pType->clear();
    m_pType->addItems(QStringList() << "上下" << "左右");
}

void JointWin::initializeConnect()
{
    connect(m_pBtnOpen1, SIGNAL(clicked(bool)), this, SLOT(onOpen1()));
    connect(m_pBtnOpen2, SIGNAL(clicked(bool)), this, SLOT(onOpen2()));
    connect(m_pBtnSave, SIGNAL(clicked(bool)), this, SLOT(onSave()));
    connect(m_pType, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeChanged()));
    connect(m_pJointImageWin, SIGNAL(openImage1()), this, SLOT(onOpen1()));
    connect(m_pJointImageWin, SIGNAL(openImage2()), this, SLOT(onOpen2()));
    connect(m_pJointImageWin, SIGNAL(save()), this, SLOT(onSave()));
    connect(m_pJointImageWin, SIGNAL(save(bool)), this, SLOT(onSave(bool)));
    connect(m_pJointImageWin, SIGNAL(switchover(bool)), this, SLOT(onSwitchover(bool)));
    connect(m_pBtnSwap, SIGNAL(clicked(bool)), m_pJointImageWin, SLOT(onSwap()));
}

void JointWin::onOpen1()
{
    QString dirPath = g_Config->Get(metaObject()->className(), OPEN_PATH).toString();
    QString path = QFileDialog::getOpenFileName(nullptr,tr("Open Project"),
                                                dirPath, tr("Files (%1)").arg(IMAGE_FORMAT));
    if(path.isEmpty()){
        return;
    }
    QString savePath = path;
    m_name1 = savePath.split("/").last();
    m_name1.chop(m_name1.split(".").last().count() + 1);
    savePath.chop(savePath.split("/").last().count() + 1);
    g_Config->Set(metaObject()->className(), OPEN_PATH, savePath);
    m_pJointImageWin->onSetImage1(path);
}

void JointWin::onOpen2()
{
    QString dirPath = g_Config->Get(metaObject()->className(), OPEN_PATH).toString();
    QString path = QFileDialog::getOpenFileName(nullptr,tr("Open Project"),
                                                dirPath, tr("Files (%1)").arg(IMAGE_FORMAT));
    if(path.isEmpty()){
        return;
    }
    QString savePath = path;
    m_name2 = savePath.split("/").last();
    m_name2.chop(m_name2.split(".").last().count() + 1);
    savePath.chop(savePath.split("/").last().count() + 1);
    g_Config->Set(metaObject()->className(), OPEN_PATH, savePath);
    m_pJointImageWin->onSetImage2(path);
}

void JointWin::onSave()
{
    QImage image1 = m_pJointImageWin->image1();
    QImage image2 = m_pJointImageWin->image2();
    if(image1.isNull() || image2.isNull())
    {
        return;
    }
    QString dirPath = g_Config->Get(metaObject()->className(), SAVE_PATH).toString();
    auto path = QFileDialog::getSaveFileName(nullptr,tr("Save Image"),
                                                dirPath + "/" + m_name1 + "And" + m_name2 + ".png", tr("Image Files (*.png)"));
    if(path.isEmpty()){
        return;
    }
    QMatrix matrix;
    if(m_pJointImageWin->type())
    {
        if(image1.width() > image2.width())
        {
            matrix.scale(1.0*image2.width()/image1.width(), 1.0*image2.width()/image1.width());
            image1 = image1.transformed(matrix);
        }
        else if(image1.width() < image2.width())
        {
            matrix.scale(1.0*image1.width()/image2.width(), 1.0*image1.width()/image2.width());
            image2 = image2.transformed(matrix);
        }
    }
    else
    {
        if(image1.height() > image2.height())
        {
            matrix.scale(1.0*image2.height()/image1.height(), 1.0*image2.height()/image1.height());
            image1 = image1.transformed(matrix);
        }
        else if(image1.height() < image2.height())
        {
            matrix.scale(1.0*image1.height()/image2.height(), 1.0*image1.height()/image2.height());
            image2 = image2.transformed(matrix);
        }
    }

    QPixmap srcPix1 = QPixmap::fromImage(image1);
    QPixmap srcPix2 = QPixmap::fromImage(image2);

    QPixmap stitchPix;
    if(m_pJointImageWin->type())
    {
        stitchPix = QPixmap(srcPix1.width(), srcPix1.height() + srcPix2.height());
    }
    else
    {
        stitchPix = QPixmap(srcPix1.width() + srcPix2.width(), srcPix1.height());
    }
    QPainter p(&stitchPix);

    //绘制两幅小图到QPixmap上
    if(m_pJointImageWin->type())
    {
        p.drawPixmap(0, 0, srcPix1.width(), srcPix1.height(), srcPix1);
        p.drawPixmap(0, srcPix1.height(), srcPix2.width(), srcPix2.height(), srcPix2);
    }
    else
    {
        p.drawPixmap(0, 0, srcPix1.width(), srcPix1.height(), srcPix1);
        p.drawPixmap(srcPix1.width(), 0, srcPix2.width(), srcPix2.height(), srcPix2);
    }
    stitchPix.toImage().save(path);
    QString savePath = path;
    savePath.chop(savePath.split("/").last().count() + 1);
    g_Config->Set(metaObject()->className(), SAVE_PATH, savePath);
}

void JointWin::onSave(bool _1_or_2)
{
    QImage image = _1_or_2?m_pJointImageWin->image1():m_pJointImageWin->image2();
    QString name = _1_or_2?m_name1:m_name2;
    if(image.isNull())
    {
        return;
    }
    QString dirPath = g_Config->Get(metaObject()->className(), SAVE_PATH).toString();
    auto path = QFileDialog::getSaveFileName(nullptr,tr("Save Image"),
                                                dirPath + "/" + name, tr("Image Files (*.png)"));
    if(path.isEmpty()){
        return;
    }
    image.save(path);
    QString savePath = path;
    savePath.chop(savePath.split("/").last().count() + 1);
    g_Config->Set(metaObject()->className(), SAVE_PATH, savePath);
}

void JointWin::onTypeChanged()
{
    m_pJointImageWin->onSetType(m_pType->currentIndex() == 0);
    m_pJointImageWin->update();
}

void JointWin::onSwitchover(bool _1_or_2)
{
    if(_1_or_2)
    {
        onOpen1();
    }
    else
    {
        onOpen2();
    }
}


