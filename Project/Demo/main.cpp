#include "widget.h"
#include <QApplication>
#include <QIcon>
#include <QSplashScreen>
#include <QLabel>
#include <QMovie>
#include <synchapi.h>
class DataBase
{
public:
    void readData()
    {
        for (int i = 0; i < 100000; ++i)
        {
            qDebug("reading data");
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pix("../images/video.jpg");
          QSplashScreen splash(pix);
    //      splash.showMessage(QStringLiteral("正在初始化..."));
          splash.setWindowOpacity(1);// 设置窗口透明度
          QLabel label(&splash);
          QMovie mv("../images/Calater5.gif");

          mv.setScaledSize(splash.size());
          label.setMovie(&mv);
          mv.start();
          a.processEvents();
          splash.show();
          splash.setCursor(Qt::BlankCursor);
          for(int i=0; i<3300; i+=mv.speed())
           {
               QCoreApplication::processEvents();
               Sleep(mv.speed());
           }

    Widget w;
    splash.finish(&w);

    w.show();
    return a.exec();
}
