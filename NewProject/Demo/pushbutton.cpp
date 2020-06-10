#include "pushbutton.h"
PushButton::PushButton(QWidget *parent)
    :QPushButton(parent)
{
    status = NORMAL;
    mouse_press = false;
}
PushButton::~PushButton()
{
}
void PushButton::getIconPath(QString Icon_path)
{
    this->icon_path = Icon_path;
    setFixedSize(QPixmap(Icon_path).size());//设置按钮与图片一样的大小
}

void PushButton::enterEvent(QEvent *)
{
    status = HOVER;
    update();
}
void PushButton::mousePressEvent(QMouseEvent *event)
{
    //若点击鼠标左键
    if(event->button() == Qt::LeftButton)
    {
        mouse_press = true;
        status = PRESS;
        HelpFrame *p1 = new HelpFrame();
        p1->setWindowTitle("帮助文档");
        p1->move(1200,0);
        p1->show();
        update();
    }

}
void PushButton::mouseReleaseEvent(QMouseEvent *event)
{
    //若点击鼠标左键响应
    if(mouse_press&&this->rect().contains(event->pos()))
    {
        mouse_press = false;
        status = HOVER;
        update();
        emit clicked();
    }
}
void PushButton::leaveEvent(QEvent *)
{
    status = NORMAL;
    update();
}
void PushButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap;
    switch(status)
    {
    case NORMAL:
        {
            pixmap.load(icon_path);
            break;
        }
    case HOVER:
        {
            pixmap.load(QString("../Icons/WenHao2.png"));
            break;
        }
    case PRESS:
        {
            pixmap.load(QString("../Icons/WenHao.png"));
            break;
        }
    default:
        pixmap.load(icon_path);
    }
    painter.drawPixmap(rect(), pixmap);
    painter.drawText(this->rect(), Qt::AlignCenter, this->text());
}
