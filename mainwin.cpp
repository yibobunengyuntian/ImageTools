#include "mainwin.h"

MainWin::MainWin(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    initializeUI();
    initializeConnect();
}

void MainWin::addWidget(QWidget *widget)
{
    QIcon icon = widget->windowIcon();
    if(icon.isNull())
    {
        icon = QIcon(":/res/image/exe.png");
    }

    QListWidgetItem *newitem = new QListWidgetItem(          //自适应列表项中的图片大小
                icon,widget->windowTitle());

    //设置列表项整体的尺寸
    newitem->setSizeHint(QSize(70,70));
    //设置列表项中的文字对齐方式：水平居中
    newitem->setTextAlignment(Qt::AlignHCenter);
//    newitem->setToolTip(path);
    //在列表框中添加列表项
    m_pListWidget->addItem(newitem);
    m_widgets[newitem] = widget;
}

void MainWin::initializeUI()
{
    this->setWindowIcon(QIcon(":/res/image/tool.png"));
    this->setWindowTitle("图片工具");
    //注册事件过滤
    m_pListWidget->viewport()->installEventFilter(this);
    m_pListWidget->setViewMode(QListWidget::IconMode);//显示模式
    m_pListWidget->setIconSize(QSize(50, 50));//设置图片大小
    m_pListWidget->setSpacing(3);//间距
    m_pListWidget->setResizeMode(QListView::Adjust);//适应布局调整
    m_pListWidget->setMovement(QListView::Static);//不能移动
    m_pListWidget->setWrapping(true);  //自动换行
//    m_pListWidget->setDragDropMode(QAbstractItemView::InternalMove);
    addWidget(new SegmentationWin());
    addWidget(new JointWin());
}

void MainWin::initializeConnect()
{
    connect(m_pListWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onItemDoubleClicked()));
}

bool MainWin::eventFilter(QObject *obj, QEvent *event)
{
    //类型转换
    QMouseEvent *e = static_cast<QMouseEvent *>(event);
    //判空
    if (e == nullptr)
    {
        return QWidget::eventFilter(obj, event);
    }
    //判断事件类型为鼠标点击
    if (e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonRelease)
    {
        //点击空白处取消选中
        deselect(e);
    }

    return QWidget::eventFilter(obj, event);
}

void MainWin::closeEvent(QCloseEvent *event)
{
    for(auto widget:m_widgets.values())
    {
        widget->close();
    }
    QWidget::closeEvent(event);
}

void MainWin::deselect(QMouseEvent *event)
{
    // 获取当前鼠标坐标
    QPoint point = event->pos();
    // 判断该单元格是否是空单元格
    if (m_pListWidget->itemAt(point) == nullptr)
    {
        //取消选中
        m_pListWidget->setCurrentItem(NULL);
    }
}

void MainWin::onItemDoubleClicked()
{
    QListWidgetItem *currListWidgetItem = m_pListWidget->currentItem();
    QWidget *w = m_widgets[currListWidgetItem];
    w->activateWindow();
    w->setWindowState((w->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    w->showNormal();
    w->move(QCursor::pos() + QPoint(10, 10));
}
