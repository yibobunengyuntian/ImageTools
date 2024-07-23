#include "jointimagewin.h"

JointImageWin::JointImageWin(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    setAcceptDrops(true);
    initializeMenu();
}

bool JointImageWin::type()
{
    return m_type;
}

QImage JointImageWin::image1()
{
    return m_image1;
}

QImage JointImageWin::image2()
{
    return m_image2;
}

void JointImageWin::onSetImage1(const QString &path)
{
    m_image1.load(path);
}

void JointImageWin::onSetImage2(const QString &path)
{
    m_image2.load(path);
}

void JointImageWin::onSwap()
{
    QImage image = m_image1;
    m_image1 = m_image2;
    m_image2 = image;
    this->update();
}

void JointImageWin::onSetType(const bool &type)
{
    m_type = type;
}

void JointImageWin::onRotate_l()
{
    QMatrix mat;
    mat.rotate(-90);
    if(m_1_or_2)
    {
        m_image1 = m_image1.transformed(mat);
    }
    else
    {
        m_image2 = m_image2.transformed(mat);
    }
    this->update();
}

void JointImageWin::onRotate_r()
{
    QMatrix mat;
    mat.rotate(90);
    if(m_1_or_2)
    {
        m_image1 = m_image1.transformed(mat);
    }
    else
    {
        m_image2 = m_image2.transformed(mat);
    }
    this->update();
}

void JointImageWin::onOverturn_ud()
{
    if(m_1_or_2)
    {
        m_image1 = m_image1.mirrored(false, true);
    }
    else
    {
        m_image2 = m_image2.mirrored(false, true);
    }
    this->update();
}

void JointImageWin::onOverturn_lr()
{
    if(m_1_or_2)
    {
        m_image1 = m_image1.mirrored(true, false);
    }
    else
    {
        m_image2 = m_image2.mirrored(true, false);
    }
    this->update();
}

void JointImageWin::onSwitchover()
{
    emit switchover(m_1_or_2);
}

void JointImageWin::onSave()
{
    if(m_1_or_2)
    {
        emit save(true);
    }
    else
    {
        emit save(false);
    }
}

void JointImageWin::initializeMenu()
{
    m_pMenu = new QMenu;
    auto pSave = m_pMenu->addAction(QIcon(":/res/image/save.png"), "保存");
    auto pSwitchover = m_pMenu->addAction(QIcon(":/res/image/switchover.png"), "切换");
    auto pRotate_l = m_pMenu->addAction(QIcon(":/res/image/rotate_l.png"), "旋转-90°");
    auto pRotate_r = m_pMenu->addAction(QIcon(":/res/image/rotate_r.png"), "旋转+90°");
    auto pOverturn_ud = m_pMenu->addAction(QIcon(":/res/image/overturn_ud.png"), "上下翻转");
    auto pOverturn_lr = m_pMenu->addAction(QIcon(":/res/image/overturn_lr.png"), "左右翻转");
    auto pSend = m_pMenu->addAction(QIcon(":/res/image/send.png"), "发送到");
    QMenu *pSaveMenu = new QMenu();
    QMenu *pSendMenu = new QMenu();
    pSave->setMenu(pSaveMenu);
    pSend->setMenu(pSendMenu);
    auto pSaveAll = pSaveMenu->addAction("整张");
    auto pSaveCurr = pSaveMenu->addAction("当前");

    connect(pSaveAll, SIGNAL(triggered(bool)), this, SIGNAL(save()));
    connect(pSaveCurr, SIGNAL(triggered(bool)), this, SLOT(onSave()));

    connect(pSwitchover, SIGNAL(triggered(bool)), this, SLOT(onSwitchover()));
    connect(pRotate_l, SIGNAL(triggered(bool)), this, SLOT(onRotate_l()));
    connect(pRotate_r, SIGNAL(triggered(bool)), this, SLOT(onRotate_r()));
    connect(pOverturn_lr, SIGNAL(triggered(bool)), this, SLOT(onOverturn_lr()));
    connect(pOverturn_ud, SIGNAL(triggered(bool)), this, SLOT(onOverturn_ud()));
}

void JointImageWin::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    if(!m_image1.isNull())
    {
        int w, h;
        if(m_type)
        {
            if(1.0*m_image1.height()/m_image1.width() > 1.0*this->height()/this->width()/2)
            {
                h = this->height()/2;
                w = 1.0*h * m_image1.width()/m_image1.height();
                m_imageRect1 = QRect((this->width() - w)/2, 0, w, h);
            }
            else
            {
                w = this->width();
                h = 1.0*w * m_image1.height()/m_image1.width();
                m_imageRect1 = QRect(0, (this->height()/2 - h)/2, w, h);
            }
        }
        else
        {
            if(1.0*m_image1.height()/m_image1.width() > 1.0*this->height()/this->width()*2)
            {
                h = this->height();
                w = 1.0*h * m_image1.width()/m_image1.height();
                m_imageRect1 = QRect((this->width()/2 - w)/2, 0, w, h);
            }
            else
            {
                w = this->width()/2;
                h = 1.0*w * m_image1.height()/m_image1.width();
                m_imageRect1 = QRect(0, (this->height() - h)/2, w, h);
            }
        }
        painter.drawImage(m_imageRect1,m_image1);
    }
    else
    {
        QImage image(":/res/image/add.png");
        QRect rect;
        if(m_type)
        {
            rect = QRect((this->width() - image.width())/2,
                         (this->height()/2 - image.height())/2,
                         image.width(), image.height());
        }
        else
        {
            rect = QRect((this->width()/2 - image.width())/2,
                         (this->height() - image.height())/2,
                         image.width(), image.height());
        }
        painter.drawImage(rect, image);
    }
    if(!m_image2.isNull())
    {
        int w, h;
        if(m_type)
        {
            if(1.0*m_image2.height()/m_image2.width() > 1.0*this->height()/this->width()/2)
            {
                h = this->height()/2;
                w = 1.0*h * m_image2.width()/m_image2.height();
                m_imageRect2 = QRect((this->width() - w)/2, this->height()/2, w, h);
            }
            else
            {
                w = this->width();
                h = 1.0*w * m_image2.height()/m_image2.width();
                m_imageRect2 = QRect(0, (this->height()/2 - h)/2 + this->height()/2, w, h);
            }
        }
        else
        {
            if(1.0*m_image2.height()/m_image2.width() > 1.0*this->height()/this->width()*2)
            {
                h = this->height();
                w = 1.0*h * m_image2.width()/m_image2.height();
                m_imageRect2 = QRect((this->width()/2 - w)/2 + this->width()/2, 0, w, h);
            }
            else
            {
                w = this->width()/2;
                h = 1.0*w * m_image2.height()/m_image2.width();
                m_imageRect2 = QRect(this->width()/2, (this->height() - h)/2, w, h);
            }
        }
        painter.drawImage(m_imageRect2,m_image2);
    }
    else
    {
        QImage image(":/res/image/add.png");
        QRect rect;
        if(m_type)
        {
            rect = QRect((this->width() - image.width())/2,
                         (this->height()/2 - image.height())/2 + this->height()/2,
                         image.width(), image.height());
        }
        else
        {
            rect = QRect((this->width()/2 - image.width())/2 + this->width()/2,
                         (this->height() - image.height())/2,
                         image.width(), image.height());
        }
        painter.drawImage(rect, image);
    }
    if(m_type)
    {
        painter.drawLine(QPoint(0, this->height()/2), QPoint(this->width(), this->height()/2));
    }
    else
    {
        painter.drawLine(QPoint(this->width()/2, 0), QPoint(this->width()/2, this->height()));
    }
}

void JointImageWin::mousePressEvent(QMouseEvent *pEvent)
{
    if(pEvent->button() == Qt::LeftButton)
    {
        if((m_type && pEvent->pos().y() < this->height()/2) ||
                (!m_type && pEvent->pos().x() < this->width()/2))
        {
            if(m_image1.isNull())
            {
                emit openImage1();
            }
        }
        else
        {
            if(m_image2.isNull())
            {
                emit openImage2();
            }
        }
    }
    else if(pEvent->button() == Qt::RightButton)
    {
        if(!m_image1.isNull() && m_imageRect1.contains(pEvent->pos()))
        {
            m_1_or_2 = true;
            m_pMenu->exec(QCursor::pos());
        }
        else if(!m_image2.isNull() && m_imageRect2.contains(pEvent->pos()))
        {
            m_1_or_2 = false;
            m_pMenu->exec(QCursor::pos());
        }
    }
}

void JointImageWin::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->urls().count()  == 1
            && QString(IMAGE_FORMAT).split(" *.").contains(event->mimeData()->urls()[0].fileName().split(".").last()))
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();//否则不接受鼠标事件
    }
}

void JointImageWin::dropEvent(QDropEvent *event)
{
    if((m_type && event->pos().y() < this->height()/2) ||
            (!m_type && event->pos().x() < this->width()/2))
    {
        onSetImage1(event->mimeData()->urls()[0].toLocalFile());
    }
    else
    {
        onSetImage2(event->mimeData()->urls()[0].toLocalFile());
    }
    this->update();
}

void JointImageWin::dragMoveEvent(QDragMoveEvent *event)
{

}

