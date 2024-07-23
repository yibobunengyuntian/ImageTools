#include "smtimagewin.h"

SmtImageWin::SmtImageWin(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    setAcceptDrops(true);
    initializeMenu();
}

bool SmtImageWin::type()
{
    return m_type;
}

QVector2D SmtImageWin::ratio()
{
    return m_ratio;
}

QImage SmtImageWin::image()
{
    return m_image;
}

void SmtImageWin::onSetImage(const QString &path)
{
    m_image.load(path);
}

void SmtImageWin::onSetType(const bool &type)
{
    m_type = type;
}

void SmtImageWin::onSetRatio(const QVector2D &ratio)
{
    m_ratio = ratio;
}

void SmtImageWin::onRotate_l()
{
    QMatrix mat;
    mat.rotate(-90);
    m_image = m_image.transformed(mat);
    this->update();
}

void SmtImageWin::onRotate_r()
{
    QMatrix mat;
    mat.rotate(90);
    m_image = m_image.transformed(mat);
    this->update();
}

void SmtImageWin::onOverturn_ud()
{
    m_image = m_image.mirrored(false, true);
    this->update();
}

void SmtImageWin::onOverturn_lr()
{
    m_image = m_image.mirrored(true, false);
    this->update();
}

void SmtImageWin::initializeMenu()
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
    auto pSaveP1 = pSaveMenu->addAction("图一");
    auto pSaveP2 = pSaveMenu->addAction("图二");

    connect(pSaveAll, SIGNAL(triggered(bool)), this, SIGNAL(saveAll()));
    connect(pSaveP1, SIGNAL(triggered(bool)), this, SIGNAL(saveP1()));
    connect(pSaveP2, SIGNAL(triggered(bool)), this, SIGNAL(saveP2()));

    connect(pSwitchover, SIGNAL(triggered(bool)), this, SIGNAL(openImage()));
    connect(pRotate_l, SIGNAL(triggered(bool)), this, SLOT(onRotate_l()));
    connect(pRotate_r, SIGNAL(triggered(bool)), this, SLOT(onRotate_r()));
    connect(pOverturn_lr, SIGNAL(triggered(bool)), this, SLOT(onOverturn_lr()));
    connect(pOverturn_ud, SIGNAL(triggered(bool)), this, SLOT(onOverturn_ud()));
}

void SmtImageWin::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if(!m_image.isNull())
    {
        int w, h;
        if(1.0*m_image.height()/m_image.width() > 1.0*this->height()/this->width())
        {
            h = this->height();
            w = h * m_image.width()/m_image.height();
            m_imageRect = QRect((this->width() - w)/2, 0, w, h);
        }
        else
        {
            w = this->width();
            h = w * m_image.height()/m_image.width();
            m_imageRect = QRect(0, (this->height() - h)/2, w, h);
        }
        QPainter painter(this);
        painter.drawImage(m_imageRect,m_image);
        QPoint p1, p2;
        if(m_type)
        {
            p1 = QPoint(0, m_imageRect.y() + h*(m_ratio.x()/(m_ratio.x() + m_ratio.y())));
            p2 = QPoint(this->width() + w, p1.y());
        }
        else
        {
            p1 = QPoint(m_imageRect.x() + w*(m_ratio.x()/(m_ratio.x() + m_ratio.y())), 0);
            p2 = QPoint(p1.x(), this->height());
        }
        painter.drawLine(p1, p2);
    }
    else
    {
        QImage image(":/res/image/add.png");
        QPainter painter(this);
        painter.drawImage(QRect((this->width() - image.width())/2,
                                (this->height() - image.height())/2,
                                image.width(), image.height()),
                          image);
    }
}

void SmtImageWin::mousePressEvent(QMouseEvent *pEvent)
{
    if(pEvent->button() == Qt::LeftButton)
    {
        if(m_image.isNull())
        {
            emit openImage();
            return;
        }
        if(!m_imageRect.contains(pEvent->pos()))
        {
            return;
        }
        QVector2D ratio;
        QPoint pos = pEvent->pos();
        if(m_type)
        {
            ratio = QVector2D(pos.y() - m_imageRect.y(), m_imageRect.height() - (pos.y() - m_imageRect.y()));
        }
        else
        {
            ratio = QVector2D(pos.x() - m_imageRect.x(), m_imageRect.width() - (pos.x() - m_imageRect.x()));
        }
        emit ratioChanged(ratio);
        m_isMouseDown = true;
    }
    else if(pEvent->button() == Qt::RightButton)
    {
        if(!m_image.isNull() && m_imageRect.contains(pEvent->pos()))
        {
            m_pMenu->exec(QCursor::pos());
        }
    }
}

void SmtImageWin::mouseMoveEvent(QMouseEvent *pEvent)
{
    if(m_image.isNull()
            || !m_imageRect.contains(pEvent->pos())
            || !m_isMouseDown)
    {
        return;
    }
    QVector2D ratio;
    QPoint pos = pEvent->pos();
    if(m_type)
    {
        ratio = QVector2D(pos.y() - m_imageRect.y(), m_imageRect.height() - (pos.y() - m_imageRect.y()));
    }
    else
    {
        ratio = QVector2D(pos.x() - m_imageRect.x(), m_imageRect.width() - (pos.x() - m_imageRect.x()));
    }
    emit ratioChanged(ratio);
}

void SmtImageWin::mouseReleaseEvent(QMouseEvent *pEvent)
{
    if(m_image.isNull()
            || !m_imageRect.contains(pEvent->pos()))
    {
        return;
    }
    if(pEvent->button() == Qt::LeftButton)
    {
        m_isMouseDown = false;
    }
}

void SmtImageWin::dragEnterEvent(QDragEnterEvent *event)
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

void SmtImageWin::dropEvent(QDropEvent *event)
{
    onSetImage(event->mimeData()->urls()[0].toLocalFile());
    this->update();
}

void SmtImageWin::dragMoveEvent(QDragMoveEvent *event)
{

}
