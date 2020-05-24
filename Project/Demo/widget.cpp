#include "widget.h"
#include "ui_widget.h"
#include "house_loan.h"
#include "commercial_loan.h"
#include "titlebar.h"
#include <iostream>
#include <QEvent>
#include <QDebug>
#include <QLineEdit>
#include <cmath>
#include <QMessageBox>
#include <QCloseEvent>
#include <QChar>
#include <QMovie>
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <Windowsx.h>
#endif
using namespace std;
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //Qt::FramelessWindowHint设置窗口标志为无边框，而Qt::WindowStaysOnTopHint使窗口位于所有界面之上
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    //背景透明
    //setAttribute(Qt::WA_TranslucentBackground, true);

    //定义自定义标题栏对象
    titleBar *pTitleBar = new titleBar(this);
    installEventFilter(pTitleBar);

    setWindowTitle("房贷计算器"); //设置窗口名称，会发生窗口标题栏改变事件，随之自定义标题栏的标题会更新
    setWindowIcon(QIcon("../Images/logo.png")); //设置窗口图标，会发生窗口图标改变事件，随之自定义标题栏的图标会更新

    //使用调色板设置窗口的背景色
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(255, 240, 221));
    setAutoFillBackground(true);
    setPalette(pal);

    //窗口布局中加入标题栏
    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addStretch();
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);

    //m_nBorder表示鼠标位于边框缩放范围的宽度，可以设置为5
    m_nBorderWidth=5;
    ui->stackedWidget->setCurrentWidget(ui->Calater_1);
    QMovie *p1 = new QMovie("../images/shuji.gif");
    QMovie *p2 = new QMovie("../images/shuji.gif");
    QMovie *p3 = new QMovie("../images/shuji.gif");
    ui->label_logo_2->setMovie(p2);
    ui->label_logo->setScaledContents(true);
    p2->start();

    ui->label_logo_3->setMovie(p3);
    ui->label_logo_3->setScaledContents(true);
    p3->start();

    ui->label_logo->setMovie(p1);
    ui->label_logo->setScaledContents(true);
    p1->start();

}
void Widget::closeEvent(QCloseEvent *ev)
{

    int ret = QMessageBox::question(this,"Question","是否需要关闭房贷计算器？");
    if(ret == QMessageBox::Yes)
    {
        //关闭窗口;
        //处理关闭窗口事件，接受事件，事件就不会往下传递
        ev->accept();
    }
    else if(ret == QMessageBox::No)
    {
        //保持不动
        //忽略事件，事件继续给父组件传递
        ev->ignore();
    }
}
bool Widget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)

    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
    case WM_NCHITTEST:
    {
        int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
        int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

        // 如果鼠标位于子控件上，则不进行处理
        if (childAt(nX, nY) != nullptr)
            return QWidget::nativeEvent(eventType, message, result);

        *result = HTCAPTION;

        // 鼠标区域位于窗体边框，进行缩放
        if ((nX > 0) && (nX < m_nBorderWidth))
            *result = HTLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width()))
            *result = HTRIGHT;

        if ((nY > 0) && (nY < m_nBorderWidth))
            *result = HTTOP;

        if ((nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOM;

        if ((nX > 0) && (nX < m_nBorderWidth) && (nY > 0)
                && (nY < m_nBorderWidth))
            *result = HTTOPLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
                && (nY > 0) && (nY < m_nBorderWidth))
            *result = HTTOPRIGHT;

        if ((nX > 0) && (nX < m_nBorderWidth)
                && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOMLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
                && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOMRIGHT;

        return true;
    }
    }

    return QWidget::nativeEvent(eventType, message, result);
}
void Widget::keyPressEvent(QKeyEvent *ev)
{

    if(ev->key()==Qt::Key_Return)
    {
        switch (ui->stackedWidget->currentIndex())
        {
            case 1:ui->pushButton_start->clicked();break;
            case 2:ui->pushButton_Restart_1->clicked();break;
            case 3:ui->pushButton_start_2->clicked();break;
            case 4:ui->pushButton_start_4->clicked();break;
            case 5:ui->pushButton_start_3->clicked();break;
            case 6:ui->pushButton_start_6->clicked();break;
        }

    }
}

Widget::~Widget()
{
    delete ui;
}

/******************************************-----Begin_Calater_1-----*************************************************************/
void Widget::on_pushButton_start_clicked()
{
    // 第一个页面处理
/******************************************-----Page.NO.1.-----输入判断*******************************************************************/
    QString checkValue = ui->lineEdit_value->text();
    QString checkArea = ui->lineEdit_area->text();
    QString checkRate = ui->lineEdit_rate->text();
    bool Justise = 0;
    double rate = 0;
    //判断是否为空
    if(!checkValue.isEmpty()&&!checkArea.isEmpty())
    {
        int checkPoint = 0;
        bool flag1 = 0, flag2 = 0,flag3 = 0;
        //判断输入单价
        for(int i= 0 ;i<checkValue.length();i++)
        {
            if((checkValue[i]<='9'&&checkValue[i]>='0')||checkValue[i]=='.')
            {
                if(checkValue[i]=='.')
                    checkPoint++;
                if(checkPoint>1){flag1=1;break;}
            }
            else{flag1 = 1;break;}
        }

        //判断输入面积
        checkPoint = 0;
        for(int i= 0 ;i<checkArea.length();i++)
        {
            if((checkArea[i]<='9'&&checkArea[i]>='0')||checkArea[i]=='.')
            {
                if(checkArea[i]=='.')
                    checkPoint++;
                if(checkPoint>1){flag2=1;break;}
            }
            else{flag2 = 1;break;}
        }

        //判断输入利率
        checkPoint = 0;
        int n = 0;
        for(int i= 0 ;i<checkRate.length()-1;i++)
        {
            if((checkRate[i]<='9'&&checkRate[i]>='0')||checkRate[i]=='.')
            {
                QChar te = checkRate.at(i);
                if(checkRate[i]=='.')
                {
                    checkPoint++;
                }
                if(checkPoint>=2){flag3=1;break;}
                if(checkRate[i]!='.')
                {
                    rate = rate*10+(te.toLatin1()-'0');
                    n++;
                }
            }
            else{flag3 = 1;break;}
        }
        rate /= pow(10,n+1);
        if(rate == 0)
        {
            flag3 =1;
        }

        if(flag1 == 1 && flag2 == 1 && flag3 == 1)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入的房屋单价、面积、利率有误",QMessageBox::Ok);
//            ui->stackedWidget->setCurrentWidget(ui->Calater_1);
        }
        else if(flag1 == 1 && flag2 == 0 && flag3 == 0)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋单价有误",QMessageBox::Ok);
//            ui->stackedWidget->setCurrentWidget(ui->Calater_1);
        }
        else if(flag1 == 0 && flag2 == 1 && flag3 == 0)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋面积有误",QMessageBox::Ok);
        }
        else if(flag1 == 0 && flag2 == 0 && flag3 == 1)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋利率不合法，请重新输入",QMessageBox::Ok);
        }
        else if(flag1 == 1 && flag2 == 0 && flag3 == 1)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋单价、利率有误",QMessageBox::Ok);
        }
        else if(flag1 == 0 && flag2 == 1 && flag3 == 1)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋面积、利率有误",QMessageBox::Ok);
        }
        else if(flag1 == 1 && flag2 == 1 && flag3 == 0)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋单价、面积有误",QMessageBox::Ok);
        }
        else
        {
            Justise = 1;
        }
    }
    //如果有输入有空
    else
    {
        if(checkValue.isEmpty()&&checkArea.isEmpty()&&checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋单价、面积、利率",QMessageBox::Ok);
        }
        else if(checkValue.isEmpty()&&!checkArea.isEmpty()&&!checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋单价",QMessageBox::Ok);
        }
        else if(!checkValue.isEmpty()&&checkArea.isEmpty()&&!checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋面积",QMessageBox::Ok);
        }
        else if(!checkValue.isEmpty()&&!checkArea.isEmpty()&&checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋利率",QMessageBox::Ok);
        }
        else if(!checkValue.isEmpty()&&checkArea.isEmpty()&&checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋面积、利率",QMessageBox::Ok);
        }
        else if(checkValue.isEmpty()&&!checkArea.isEmpty()&&checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋单价、利率",QMessageBox::Ok);
        }
        else if(checkValue.isEmpty()&&checkArea.isEmpty()&&!checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋面积、单价",QMessageBox::Ok);
        }
    }

    if(Justise ==1 )
    {
 /******************************************-----Page.NO.1.-----按揭成数***********************************************************/
        double Percentage = 0;
        switch (ui->comboBox_percent->currentIndex())
        {
        case 0:Percentage = 0.9;break;case 1:Percentage = 0.85;break;
        case 2:Percentage = 0.8;break;case 3:Percentage = 0.75;break;
        case 4:Percentage = 0.7;break;case 5:Percentage = 0.65;break;
        case 6:Percentage = 0.6;break;case 7:Percentage = 0.55;break;
        case 8:Percentage = 0.5;break;case 9:Percentage = 0.45;break;
        case 10:Percentage = 0.4;break;case 11:Percentage = 0.35;break;
        case 12:Percentage = 0.3;break;case 13:Percentage = 0.25;break;
        case 14:Percentage = 0.2;break;
        }

/******************************************-----Page.NO.1.-----已有值连接***********************************************************/
        //房屋单价值
        double value = ui->lineEdit_value->text().toDouble();
        ui->lineEdit_Revalue_1->setText(ui->lineEdit_value->text());

        //房屋面积值
        double area = ui->lineEdit_area->text().toDouble();

        ui->lineEdit_Rearea_1->setText(ui->lineEdit_area->text());
        short Deadline =on_comboBox_deadline_currentIndexChanged(ui->comboBox_deadline->currentIndex());

//        还款日期下拉框
        ui->comboBox_Redeadline_1->setCurrentIndex(ui->comboBox_deadline->currentIndex());

//        利率下拉框
        ui->comboBox_Rerate_1->setCurrentIndex(ui->comboBox_rate->currentIndex());
        ui->lineEdit_Rerate_1->setText(QString("%1%").arg(rate*100));

//        按揭成数下拉框
        ui->comboBox_Repercent_1->setCurrentIndex(ui->comboBox_percent->currentIndex());

//        贷款类型下拉框
        ui->comboBox_Retype_1->setCurrentIndex(ui->comboBox_type->currentIndex());

        //commercial_loan(double value,double area,double percentage,short deadline,double rate);

        commercial_loan A(value,area,Percentage,Deadline,rate);

//            qDebug()<<value<<' '<<area<<' '<<Deadline<<' '<<rate;
/******************************************-----Page.NO.1.-----计算方式选择********************************************************/
        //等额本息
        if(ui->radioButton_benxi->isChecked())
        {

            ui->radioButton_Rebenxi_1->clicked();
            ui->label_Remonthpay_change->setText("每月月供:");
            ui->widget_Redecline_1->hide();
            //首付
            A.Average_Capital_Plus_Interest();
            //首付
            ui->label_ReFirst_1->setText(QString::number(A.Getfirstpayment(), 'f', 1));
            //月供
            ui->label_Remonthpay_1->setText(QString::number(A.Getmonthpayment(), 'f', 1));
            //贷款总额
            ui->label_ReLoan_1->setText(QString::number(A.Getloan(), 'f', 1));
            //支付利息
            ui->label_Reinterest_1->setText(QString::number(A.Getinterest(), 'f', 1));
            //还款总额
            ui->label_Retotalpay_1->setText(QString::number(A.Getendpayment(), 'f', 1));
            //还款月数
            ui->label_Redate_1->setText(QString::number(A.Getdeadline(), 'f', 1));
        }
        //等额本金
        else if(ui->radioButton_benjin->isChecked())
        {
            ui->radioButton_Rebenjin_1->clicked();
            ui->widget_Redecline_1->show();
            ui->label_Remonthpay_change->setText("首月月供:");
            A.Average_capital();
            //首付
            ui->label_ReFirst_1->setText(QString::number(A.Getfirstpayment(), 'f', 1));
            //月供
            ui->label_Remonthpay_1->setText(QString::number(A.Getmonthpayment(), 'f', 1));
            //每月递减
            ui->label_Redecline_1->setText(QString::number(A.Getmonth_cut(), 'f', 1));
            //贷款总额
            ui->label_ReLoan_1->setText(QString::number(A.Getloan(), 'f', 1));
            //支付利息
            ui->label_Reinterest_1->setText(QString::number(A.Getinterest(), 'f', 1));
            //还款总额
            ui->label_Retotalpay_1->setText(QString::number(A.Getendpayment(), 'f', 1));
            //还款月数
            ui->label_Redate_1->setText(QString::number(A.Getdeadline(), 'f', 1));

        }
        ui->stackedWidget->setCurrentWidget(ui->Calater_1_Result);
    }
/******************************************-----Page.NO.1.-----计算值连接***********************************************************/
}
void Widget::on_comboBox_type_currentIndexChanged(int index)
{
/**************************************-----Page.NO.1.-----贷款种类页面切换**********************************************/
/**************************************-----Page.NO.1.-----商代值改变**********************************************/
    if(index == 0)
    {
        if(ui->comboBox_deadline->currentIndex() <20)
        {
            switch (ui->comboBox_rate->currentIndex())
            {
            //2020.4.20
            case 0:ui->lineEdit_rate->setText("4.65%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate->setText("4.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate->setText("4.85%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate->setText("5.39%");break;
            case 4:ui->lineEdit_rate->setText("5.15%");break;case 5:ui->lineEdit_rate->setText("4.17%");break;
            case 6:ui->lineEdit_rate->setText("3.43%");break;case 7:ui->lineEdit_rate->setText("4.9%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate->setText("5.67%");break;case 9:ui->lineEdit_rate->setText("4.38%");break;
            case 10:ui->lineEdit_rate->setText("3.61%");break;case 11:ui->lineEdit_rate->setText("5.15%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate->setText("5.94%");break;case 13:ui->lineEdit_rate->setText("4.59%");break;
            case 14:ui->lineEdit_rate->setText("3.78%");break;case 15:ui->lineEdit_rate->setText("5.4%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate->setText("6.22%");break;case 17:ui->lineEdit_rate->setText("4.8%");break;
            case 18:ui->lineEdit_rate->setText("3.96%");break;case 19:ui->lineEdit_rate->setText("5.65%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate->setText("6.49%");break;case 21:ui->lineEdit_rate->setText("5.01%");break;
            case 22:ui->lineEdit_rate->setText("4.13%");break;case 23:ui->lineEdit_rate->setText("5.9%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate->setText("6.7%");break;case 25:ui->lineEdit_rate->setText("5.2%");break;
            case 26:ui->lineEdit_rate->setText("4.3%");break;case 27:ui->lineEdit_rate->setText("6.15%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate->setText("7.21%");break;case 29:ui->lineEdit_rate->setText("5.57%");break;
            case 30:ui->lineEdit_rate->setText("4.59%");break;case 31:ui->lineEdit_rate->setText("6.55%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate->setText("7.48%");break;case 33:ui->lineEdit_rate->setText("5.78%");break;
            case 34:ui->lineEdit_rate->setText("4.76%");break;case 35:ui->lineEdit_rate->setText("6.8%");break;
            }
        }
        else
        {
            switch (ui->comboBox_rate->currentIndex())
            {

            //2020.4.20
            case 0:ui->lineEdit_rate->setText("3.85%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate->setText("4.05%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate->setText("4.2%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate->setText("5.23%");break;
            case 4:ui->lineEdit_rate->setText("4.99%");break;case 5:ui->lineEdit_rate->setText("4.04%");break;
            case 6:ui->lineEdit_rate->setText("3.33%");break;case 7:ui->lineEdit_rate->setText("4.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate->setText("5.5%");break;case 9:ui->lineEdit_rate->setText("4.25%");break;
            case 10:ui->lineEdit_rate->setText("3.5%");break;case 11:ui->lineEdit_rate->setText("5%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate->setText("5.78%");break;case 13:ui->lineEdit_rate->setText("4.46%");break;
            case 14:ui->lineEdit_rate->setText("3.68%");break;case 15:ui->lineEdit_rate->setText("5.25%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate->setText("6.05%");break;case 17:ui->lineEdit_rate->setText("4.68%");break;
            case 18:ui->lineEdit_rate->setText("3.85%");break;case 19:ui->lineEdit_rate->setText("5.5%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate->setText("6.33%");break;case 21:ui->lineEdit_rate->setText("4.89%");break;
            case 22:ui->lineEdit_rate->setText("4.03%");break;case 23:ui->lineEdit_rate->setText("5.75%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate->setText("6.6%");break;case 25:ui->lineEdit_rate->setText("5.1%");break;
            case 26:ui->lineEdit_rate->setText("4.2%");break;case 27:ui->lineEdit_rate->setText("6%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate->setText("6.77%");break;case 29:ui->lineEdit_rate->setText("5.23%");break;
            case 30:ui->lineEdit_rate->setText("4.31%");break;case 31:ui->lineEdit_rate->setText("6.15%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate->setText("7.04%");break;case 33:ui->lineEdit_rate->setText("5.44%");break;
            case 34:ui->lineEdit_rate->setText("4.48%");break;case 35:ui->lineEdit_rate->setText("6.4%");break;
            }
        }

    }
/**************************************切换为 组合型贷款*******************************************************************/
    else if(index == 2)
    {
        ui->stackedWidget->setCurrentWidget(ui->Calater_3);
        ui->comboBox_type_3->setCurrentIndex(0);
    }

/**************************************公积金贷款 值改变*******************************************************************/

    else
    {
        if(ui->comboBox_deadline->currentIndex() <20)
        {
            switch (ui->comboBox_rate->currentIndex())
            {
            //2020.4.20
            case 0:ui->lineEdit_rate->setText("3.25%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate->setText("3.25%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate->setText("3.25%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate->setText("3.57%");break;
            case 4:ui->lineEdit_rate->setText("3.41%");break;case 5:ui->lineEdit_rate->setText("2.76%");break;
            case 6:ui->lineEdit_rate->setText("2.28%");break;case 7:ui->lineEdit_rate->setText("3.25%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate->setText("3.57%");break;case 9:ui->lineEdit_rate->setText("2.76%");break;
            case 10:ui->lineEdit_rate->setText("2.28%");break;case 11:ui->lineEdit_rate->setText("3.25%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate->setText("3.57%");break;case 13:ui->lineEdit_rate->setText("2.98%");break;
            case 14:ui->lineEdit_rate->setText("2.45%");break;case 15:ui->lineEdit_rate->setText("3.5%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate->setText("4.13%");break;case 17:ui->lineEdit_rate->setText("3.19%");break;
            case 18:ui->lineEdit_rate->setText("2.63%");break;case 19:ui->lineEdit_rate->setText("3.75%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate->setText("4.4%");break;case 21:ui->lineEdit_rate->setText("3.4%");break;
            case 22:ui->lineEdit_rate->setText("2.8%");break;case 23:ui->lineEdit_rate->setText("4%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate->setText("4.25%");break;case 25:ui->lineEdit_rate->setText("4.25%");break;
            case 26:ui->lineEdit_rate->setText("4.25%");break;case 27:ui->lineEdit_rate->setText("4.25%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate->setText("4.5%");break;case 29:ui->lineEdit_rate->setText("4.5%");break;
            case 30:ui->lineEdit_rate->setText("4.5%");break;case 31:ui->lineEdit_rate->setText("4.5%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate->setText("4.7%");break;case 33:ui->lineEdit_rate->setText("4.7%");break;
            case 34:ui->lineEdit_rate->setText("4.7%");break;case 35:ui->lineEdit_rate->setText("4.7%");break;

            }
        }
        else
        {
            switch (ui->comboBox_rate->currentIndex())
            {

            //2020.4.20
            case 0:ui->lineEdit_rate->setText("2.75%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate->setText("2.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate->setText("2.75%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate->setText("3.03%");break;
            case 4:ui->lineEdit_rate->setText("2.89%");break;case 5:ui->lineEdit_rate->setText("2.34%");break;
            case 6:ui->lineEdit_rate->setText("1.93%");break;case 7:ui->lineEdit_rate->setText("2.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate->setText("3.03%");break;case 9:ui->lineEdit_rate->setText("2.34%");break;
            case 10:ui->lineEdit_rate->setText("1.93%");break;case 11:ui->lineEdit_rate->setText("2.75%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate->setText("3.3%");break;case 13:ui->lineEdit_rate->setText("2.55%");break;
            case 14:ui->lineEdit_rate->setText("2.1%");break;case 15:ui->lineEdit_rate->setText("3%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate->setText("3.57%");break;case 17:ui->lineEdit_rate->setText("2.76%");break;
            case 18:ui->lineEdit_rate->setText("2.28%");break;case 19:ui->lineEdit_rate->setText("3.25%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate->setText("3.85%");break;case 21:ui->lineEdit_rate->setText("2.97%");break;
            case 22:ui->lineEdit_rate->setText("2.45%");break;case 23:ui->lineEdit_rate->setText("3.5%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate->setText("3.75%");break;case 25:ui->lineEdit_rate->setText("3.75%");break;
            case 26:ui->lineEdit_rate->setText("3.75%");break;case 27:ui->lineEdit_rate->setText("3.75%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate->setText("4%");break;case 29:ui->lineEdit_rate->setText("4%");break;
            case 30:ui->lineEdit_rate->setText("4%");break;case 31:ui->lineEdit_rate->setText("4%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate->setText("4.2%");break;case 33:ui->lineEdit_rate->setText("4.2%");break;
            case 34:ui->lineEdit_rate->setText("4.2%");break;case 35:ui->lineEdit_rate->setText("4.2%");break;
            }
        }
    }
}
void Widget::on_comboBox_calaterWay_currentIndexChanged(int index)
{
/**************************************切换为 以贷款总额计算*******************************************************************/
   if(index == 1)
   {
       ui->stackedWidget->setCurrentWidget(ui->Calater_2);
       ui->comboBox_calaterWay_2->setCurrentIndex(0);
       switch (ui->comboBox_type->currentIndex())
       {
            case 0:ui->comboBox_type_2->setCurrentIndex(0);break;
            case 1:ui->comboBox_type_2->setCurrentIndex(1);break;
            default:
                QMessageBox *eorr = new QMessageBox(this);
                eorr->warning(this,"错误","你的选择有问题！",QMessageBox::Ok);
            break;
       }
   }
}
void Widget::on_comboBox_rate_currentIndexChanged(int index)
{
/**************************************-----Page.NO.1.-----利率 下拉框改变事件********************************************/
    //贷款利率
    //商贷
    if(ui->comboBox_type->currentIndex() == 0)
    {
        if(ui->comboBox_deadline->currentIndex() <20)
        {
            switch (index)
            {
            //2020.4.20
            case 0:ui->lineEdit_rate->setText("4.65%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate->setText("4.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate->setText("4.85%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate->setText("5.39%");break;
            case 4:ui->lineEdit_rate->setText("5.15%");break;case 5:ui->lineEdit_rate->setText("4.17%");break;
            case 6:ui->lineEdit_rate->setText("3.43%");break;case 7:ui->lineEdit_rate->setText("4.9%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate->setText("5.67%");break;case 9:ui->lineEdit_rate->setText("4.38%");break;
            case 10:ui->lineEdit_rate->setText("3.61%");break;case 11:ui->lineEdit_rate->setText("5.15%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate->setText("5.94%");break;case 13:ui->lineEdit_rate->setText("4.59%");break;
            case 14:ui->lineEdit_rate->setText("3.78%");break;case 15:ui->lineEdit_rate->setText("5.4%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate->setText("6.22%");break;case 17:ui->lineEdit_rate->setText("4.8%");break;
            case 18:ui->lineEdit_rate->setText("3.96%");break;case 19:ui->lineEdit_rate->setText("5.65%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate->setText("6.49%");break;case 21:ui->lineEdit_rate->setText("5.01%");break;
            case 22:ui->lineEdit_rate->setText("4.13%");break;case 23:ui->lineEdit_rate->setText("5.9%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate->setText("6.7%");break;case 25:ui->lineEdit_rate->setText("5.2%");break;
            case 26:ui->lineEdit_rate->setText("4.3%");break;case 27:ui->lineEdit_rate->setText("6.15%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate->setText("7.21%");break;case 29:ui->lineEdit_rate->setText("5.57%");break;
            case 30:ui->lineEdit_rate->setText("4.59%");break;case 31:ui->lineEdit_rate->setText("6.55%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate->setText("7.48%");break;case 33:ui->lineEdit_rate->setText("5.78%");break;
            case 34:ui->lineEdit_rate->setText("4.76%");break;case 35:ui->lineEdit_rate->setText("6.8%");break;

            }
        }
        else
        {
            switch (index)
            {

            //2020.4.20
            case 0:ui->lineEdit_rate->setText("3.85%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate->setText("4.05%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate->setText("4.2%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate->setText("5.23%");break;
            case 4:ui->lineEdit_rate->setText("4.99%");break;case 5:ui->lineEdit_rate->setText("4.04%");break;
            case 6:ui->lineEdit_rate->setText("3.33%");break;case 7:ui->lineEdit_rate->setText("4.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate->setText("5.5%");break;case 9:ui->lineEdit_rate->setText("4.25%");break;
            case 10:ui->lineEdit_rate->setText("3.5%");break;case 11:ui->lineEdit_rate->setText("5%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate->setText("5.78%");break;case 13:ui->lineEdit_rate->setText("4.46%");break;
            case 14:ui->lineEdit_rate->setText("3.68%");break;case 15:ui->lineEdit_rate->setText("5.25%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate->setText("6.05%");break;case 17:ui->lineEdit_rate->setText("4.68%");break;
            case 18:ui->lineEdit_rate->setText("3.85%");break;case 19:ui->lineEdit_rate->setText("5.5%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate->setText("6.33%");break;case 21:ui->lineEdit_rate->setText("4.89%");break;
            case 22:ui->lineEdit_rate->setText("4.03%");break;case 23:ui->lineEdit_rate->setText("5.75%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate->setText("6.6%");break;case 25:ui->lineEdit_rate->setText("5.1%");break;
            case 26:ui->lineEdit_rate->setText("4.2%");break;case 27:ui->lineEdit_rate->setText("6%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate->setText("6.77%");break;case 29:ui->lineEdit_rate->setText("5.23%");break;
            case 30:ui->lineEdit_rate->setText("4.31%");break;case 31:ui->lineEdit_rate->setText("6.15%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate->setText("7.04%");break;case 33:ui->lineEdit_rate->setText("5.44%");break;
            case 34:ui->lineEdit_rate->setText("4.48%");break;case 35:ui->lineEdit_rate->setText("6.4%");break;

            }
        }
    }
    else//公积金贷款
    {
        if(ui->comboBox_deadline->currentIndex() <20)
        {
            switch (index)
            {
            //2020.4.20
            case 0:ui->lineEdit_rate->setText("3.25%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate->setText("3.25%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate->setText("3.25%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate->setText("3.57%");break;
            case 4:ui->lineEdit_rate->setText("3.41%");break;case 5:ui->lineEdit_rate->setText("2.76%");break;
            case 6:ui->lineEdit_rate->setText("2.28%");break;case 7:ui->lineEdit_rate->setText("3.25%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate->setText("3.57%");break;case 9:ui->lineEdit_rate->setText("2.76%");break;
            case 10:ui->lineEdit_rate->setText("2.28%");break;case 11:ui->lineEdit_rate->setText("3.25%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate->setText("3.57%");break;case 13:ui->lineEdit_rate->setText("2.98%");break;
            case 14:ui->lineEdit_rate->setText("2.45%");break;case 15:ui->lineEdit_rate->setText("3.5%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate->setText("4.13%");break;case 17:ui->lineEdit_rate->setText("3.19%");break;
            case 18:ui->lineEdit_rate->setText("2.63%");break;case 19:ui->lineEdit_rate->setText("3.75%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate->setText("4.4%");break;case 21:ui->lineEdit_rate->setText("3.4%");break;
            case 22:ui->lineEdit_rate->setText("2.8%");break;case 23:ui->lineEdit_rate->setText("4%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate->setText("4.25%");break;case 25:ui->lineEdit_rate->setText("4.25%");break;
            case 26:ui->lineEdit_rate->setText("4.25%");break;case 27:ui->lineEdit_rate->setText("4.25%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate->setText("4.5%");break;case 29:ui->lineEdit_rate->setText("4.5%");break;
            case 30:ui->lineEdit_rate->setText("4.5%");break;case 31:ui->lineEdit_rate->setText("4.5%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate->setText("4.7%");break;case 33:ui->lineEdit_rate->setText("4.7%");break;
            case 34:ui->lineEdit_rate->setText("4.7%");break;case 35:ui->lineEdit_rate->setText("4.7%");break;

            }
        }
        else
        {
            switch (index)
            {

            //2020.4.20
            case 0:ui->lineEdit_rate->setText("2.75%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate->setText("2.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate->setText("2.75%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate->setText("3.03%");break;
            case 4:ui->lineEdit_rate->setText("2.89%");break;case 5:ui->lineEdit_rate->setText("2.34%");break;
            case 6:ui->lineEdit_rate->setText("1.93%");break;case 7:ui->lineEdit_rate->setText("2.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate->setText("3.03%");break;case 9:ui->lineEdit_rate->setText("2.34%");break;
            case 10:ui->lineEdit_rate->setText("1.93%");break;case 11:ui->lineEdit_rate->setText("2.75%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate->setText("3.3%");break;case 13:ui->lineEdit_rate->setText("2.55%");break;
            case 14:ui->lineEdit_rate->setText("2.1%");break;case 15:ui->lineEdit_rate->setText("3%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate->setText("3.57%");break;case 17:ui->lineEdit_rate->setText("2.76%");break;
            case 18:ui->lineEdit_rate->setText("2.28%");break;case 19:ui->lineEdit_rate->setText("3.25%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate->setText("3.85%");break;case 21:ui->lineEdit_rate->setText("2.97%");break;
            case 22:ui->lineEdit_rate->setText("2.45%");break;case 23:ui->lineEdit_rate->setText("3.5%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate->setText("3.75%");break;case 25:ui->lineEdit_rate->setText("3.75%");break;
            case 26:ui->lineEdit_rate->setText("3.75%");break;case 27:ui->lineEdit_rate->setText("3.75%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate->setText("4%");break;case 29:ui->lineEdit_rate->setText("4%");break;
            case 30:ui->lineEdit_rate->setText("4%");break;case 31:ui->lineEdit_rate->setText("4%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate->setText("4.2%");break;case 33:ui->lineEdit_rate->setText("4.2%");break;
            case 34:ui->lineEdit_rate->setText("4.2%");break;case 35:ui->lineEdit_rate->setText("4.2%");break;

            }
        }
    }
}
short Widget::on_comboBox_deadline_currentIndexChanged(int index)
{
/**************************************-----Page.NO.1.-----还款时间 下拉框改变事件*******************************************************************/
    //按揭年数
    short Deadline = 0;
    switch (index)
    {
    case 0:Deadline = 300;break;case 1:Deadline = 288;break;
    case 2:Deadline = 276;break;case 3:Deadline = 264;break;
    case 4:Deadline = 252;break;case 5:Deadline = 240;break;
    case 6:Deadline = 228;break;case 7:Deadline = 216;break;
    case 8:Deadline = 204;break;case 9:Deadline = 192;break;
    case 10:Deadline = 180;break;case 11:Deadline = 168;break;
    case 12:Deadline = 156;break;case 13:Deadline = 144;break;
    case 14:Deadline = 132;break;case 15:Deadline = 120;break;
    case 16:Deadline = 108;break;case 17:Deadline = 96;break;
    case 18:Deadline = 84;break;case 19:Deadline = 72;break;
    case 20:Deadline = 60;break;case 21:Deadline = 48;break;
    case 22:Deadline = 36;break;case 23:Deadline = 24;break;
    case 24:Deadline = 12;break;
    }

    //商业贷款
    if(ui->comboBox_type->currentIndex() == 0)
    {
        if(index <20)//五年以上
        {
            switch (ui->comboBox_rate->currentIndex())
            {
            //2020.4.20
            case 0:ui->lineEdit_rate->setText("4.65%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate->setText("4.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate->setText("4.85%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate->setText("5.39%");break;
            case 4:ui->lineEdit_rate->setText("5.15%");break;case 5:ui->lineEdit_rate->setText("4.17%");break;
            case 6:ui->lineEdit_rate->setText("3.43%");break;case 7:ui->lineEdit_rate->setText("4.9%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate->setText("5.67%");break;case 9:ui->lineEdit_rate->setText("4.38%");break;
            case 10:ui->lineEdit_rate->setText("3.61%");break;case 11:ui->lineEdit_rate->setText("5.15%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate->setText("5.94%");break;case 13:ui->lineEdit_rate->setText("4.59%");break;
            case 14:ui->lineEdit_rate->setText("3.78%");break;case 15:ui->lineEdit_rate->setText("5.4%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate->setText("6.22%");break;case 17:ui->lineEdit_rate->setText("4.8%");break;
            case 18:ui->lineEdit_rate->setText("3.96%");break;case 19:ui->lineEdit_rate->setText("5.65%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate->setText("6.49%");break;case 21:ui->lineEdit_rate->setText("5.01%");break;
            case 22:ui->lineEdit_rate->setText("4.13%");break;case 23:ui->lineEdit_rate->setText("5.9%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate->setText("6.7%");break;case 25:ui->lineEdit_rate->setText("5.2%");break;
            case 26:ui->lineEdit_rate->setText("4.3%");break;case 27:ui->lineEdit_rate->setText("6.15%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate->setText("7.21%");break;case 29:ui->lineEdit_rate->setText("5.57%");break;
            case 30:ui->lineEdit_rate->setText("4.59%");break;case 31:ui->lineEdit_rate->setText("6.55%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate->setText("7.48%");break;case 33:ui->lineEdit_rate->setText("5.78%");break;
            case 34:ui->lineEdit_rate->setText("4.76%");break;case 35:ui->lineEdit_rate->setText("6.8%");break;
            }
        }
        else//五年以下
        {
            switch (ui->comboBox_rate->currentIndex())
            {
                //2020.4.20
                case 0:ui->lineEdit_rate->setText("3.85%");break;
                //2020.3.20
                case 1:ui->lineEdit_rate->setText("4.05%");break;
                //2019.9.20
            case 2:ui->lineEdit_rate->setText("4.2%");break;
                //2015.10.24
                case 3:ui->lineEdit_rate->setText("5.23%");break;
                case 4:ui->lineEdit_rate->setText("4.99%");break;case 5:ui->lineEdit_rate->setText("4.04%");break;
                case 6:ui->lineEdit_rate->setText("3.33%");break;case 7:ui->lineEdit_rate->setText("4.75%");break;
                //2015.8.26
                case 8:ui->lineEdit_rate->setText("5.5%");break;case 9:ui->lineEdit_rate->setText("4.25%");break;
                case 10:ui->lineEdit_rate->setText("3.5%");break;case 11:ui->lineEdit_rate->setText("5%");break;
                //2015.6.28
                case 12:ui->lineEdit_rate->setText("5.78%");break;case 13:ui->lineEdit_rate->setText("4.46%");break;
                case 14:ui->lineEdit_rate->setText("3.68%");break;case 15:ui->lineEdit_rate->setText("5.25%");break;
                //2015.5.11
                case 16:ui->lineEdit_rate->setText("6.05%");break;case 17:ui->lineEdit_rate->setText("4.68%");break;
                case 18:ui->lineEdit_rate->setText("3.85%");break;case 19:ui->lineEdit_rate->setText("5.5%");break;
                //2015.3.1
                case 20:ui->lineEdit_rate->setText("6.33%");break;case 21:ui->lineEdit_rate->setText("4.89%");break;
                case 22:ui->lineEdit_rate->setText("4.03%");break;case 23:ui->lineEdit_rate->setText("5.75%");break;
                //2014.11.22
                case 24:ui->lineEdit_rate->setText("6.6%");break;case 25:ui->lineEdit_rate->setText("5.1%");break;
                case 26:ui->lineEdit_rate->setText("4.2%");break;case 27:ui->lineEdit_rate->setText("6%");break;
                //2012.7.6
                case 28:ui->lineEdit_rate->setText("6.77%");break;case 29:ui->lineEdit_rate->setText("5.23%");break;
                case 30:ui->lineEdit_rate->setText("4.31%");break;case 31:ui->lineEdit_rate->setText("6.15%");break;
                //2012.6.8
                case 32:ui->lineEdit_rate->setText("7.04%");break;case 33:ui->lineEdit_rate->setText("5.44%");break;
                case 34:ui->lineEdit_rate->setText("4.48%");break;case 35:ui->lineEdit_rate->setText("6.4%");break;
            }
        }
    }
    else //公积金贷款
    {
        if(index <20)
        {
            switch (ui->comboBox_rate->currentIndex())
            {
            //2020.4.20
            case 0:ui->lineEdit_rate->setText("3.25%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate->setText("3.25%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate->setText("3.25%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate->setText("3.57%");break;
            case 4:ui->lineEdit_rate->setText("3.41%");break;case 5:ui->lineEdit_rate->setText("2.76%");break;
            case 6:ui->lineEdit_rate->setText("2.28%");break;case 7:ui->lineEdit_rate->setText("3.25%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate->setText("3.57%");break;case 9:ui->lineEdit_rate->setText("2.76%");break;
            case 10:ui->lineEdit_rate->setText("2.28%");break;case 11:ui->lineEdit_rate->setText("3.25%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate->setText("3.57%");break;case 13:ui->lineEdit_rate->setText("2.98%");break;
            case 14:ui->lineEdit_rate->setText("2.45%");break;case 15:ui->lineEdit_rate->setText("3.5%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate->setText("4.13%");break;case 17:ui->lineEdit_rate->setText("3.19%");break;
            case 18:ui->lineEdit_rate->setText("2.63%");break;case 19:ui->lineEdit_rate->setText("3.75%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate->setText("4.4%");break;case 21:ui->lineEdit_rate->setText("3.4%");break;
            case 22:ui->lineEdit_rate->setText("2.8%");break;case 23:ui->lineEdit_rate->setText("4%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate->setText("4.25%");break;case 25:ui->lineEdit_rate->setText("4.25%");break;
            case 26:ui->lineEdit_rate->setText("4.25%");break;case 27:ui->lineEdit_rate->setText("4.25%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate->setText("4.5%");break;case 29:ui->lineEdit_rate->setText("4.5%");break;
            case 30:ui->lineEdit_rate->setText("4.5%");break;case 31:ui->lineEdit_rate->setText("4.5%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate->setText("4.7%");break;case 33:ui->lineEdit_rate->setText("4.7%");break;
            case 34:ui->lineEdit_rate->setText("4.7%");break;case 35:ui->lineEdit_rate->setText("4.7%");break;
            }
        }
        else
        {
            switch (ui->comboBox_rate->currentIndex())
            {

            //2020.4.20
            case 0:ui->lineEdit_rate->setText("2.75%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate->setText("2.75%");break;
            //2019.9.20
        case 2:ui->lineEdit_rate->setText("2.75%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate->setText("3.03%");break;
            case 4:ui->lineEdit_rate->setText("2.89%");break;case 5:ui->lineEdit_rate->setText("2.34%");break;
            case 6:ui->lineEdit_rate->setText("1.93%");break;case 7:ui->lineEdit_rate->setText("2.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate->setText("3.03%");break;case 9:ui->lineEdit_rate->setText("2.34%");break;
            case 10:ui->lineEdit_rate->setText("1.93%");break;case 11:ui->lineEdit_rate->setText("2.75%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate->setText("3.3%");break;case 13:ui->lineEdit_rate->setText("2.55%");break;
            case 14:ui->lineEdit_rate->setText("2.1%");break;case 15:ui->lineEdit_rate->setText("3%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate->setText("3.57%");break;case 17:ui->lineEdit_rate->setText("2.76%");break;
            case 18:ui->lineEdit_rate->setText("2.28%");break;case 19:ui->lineEdit_rate->setText("3.25%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate->setText("3.85%");break;case 21:ui->lineEdit_rate->setText("2.97%");break;
            case 22:ui->lineEdit_rate->setText("2.45%");break;case 23:ui->lineEdit_rate->setText("3.5%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate->setText("3.75%");break;case 25:ui->lineEdit_rate->setText("3.75%");break;
            case 26:ui->lineEdit_rate->setText("3.75%");break;case 27:ui->lineEdit_rate->setText("3.75%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate->setText("4%");break;case 29:ui->lineEdit_rate->setText("4%");break;
            case 30:ui->lineEdit_rate->setText("4%");break;case 31:ui->lineEdit_rate->setText("4%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate->setText("4.2%");break;case 33:ui->lineEdit_rate->setText("4.2%");break;
            case 34:ui->lineEdit_rate->setText("4.2%");break;case 35:ui->lineEdit_rate->setText("4.2%");break;

            }
        }
    }

    return Deadline;
}
void Widget::on_pushButton_cance_clicked()
{

    int ret = QMessageBox::question(this,"Question","是否需要清除数据？");
    if(ret == QMessageBox::Yes)
    {
        //关闭窗口;
        ui->lineEdit_value->clear();
        ui->lineEdit_area->clear();
        ui->comboBox_percent->setCurrentIndex(0);
        ui->comboBox_deadline->setCurrentText(0);
        ui->comboBox_rate->setCurrentText(0);
        ui->radioButton_benxi->clicked();
        if(ui->comboBox_type->currentIndex() == 0)
            ui->lineEdit_rate->setText("4.65%");
        else
        {
            ui->lineEdit_rate->setText("3.25%");
        }
    }
    else if(ret == QMessageBox::No)
    {
        //保持不动
        //忽略事件，事件继续给父组件传递
    }

}

/******************************************-----End_Calater_1-----***************************************************************/


/******************************************-----Result.NO.1-----*****************************************************************/
void Widget::on_comboBox_Retype_1_currentIndexChanged(int index)
{
    if(index == 0)
    {
        if(ui->comboBox_Redeadline_1->currentIndex() <20)
        {
            switch (ui->comboBox_Rerate_1->currentIndex())
            {
            //2020.4.20
            case 0:ui->lineEdit_Rerate_1->setText("4.65%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_1->setText("4.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_1->setText("4.85%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_1->setText("5.39%");break;
            case 4:ui->lineEdit_Rerate_1->setText("5.15%");break;case 5:ui->lineEdit_Rerate_1->setText("4.17%");break;
            case 6:ui->lineEdit_Rerate_1->setText("3.43%");break;case 7:ui->lineEdit_Rerate_1->setText("4.9%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_1->setText("5.67%");break;case 9:ui->lineEdit_Rerate_1->setText("4.38%");break;
            case 10:ui->lineEdit_Rerate_1->setText("3.61%");break;case 11:ui->lineEdit_Rerate_1->setText("5.15%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_1->setText("5.94%");break;case 13:ui->lineEdit_Rerate_1->setText("4.59%");break;
            case 14:ui->lineEdit_Rerate_1->setText("3.78%");break;case 15:ui->lineEdit_Rerate_1->setText("5.4%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_1->setText("6.22%");break;case 17:ui->lineEdit_Rerate_1->setText("4.8%");break;
            case 18:ui->lineEdit_Rerate_1->setText("3.96%");break;case 19:ui->lineEdit_Rerate_1->setText("5.65%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_1->setText("6.49%");break;case 21:ui->lineEdit_Rerate_1->setText("5.01%");break;
            case 22:ui->lineEdit_Rerate_1->setText("4.13%");break;case 23:ui->lineEdit_Rerate_1->setText("5.9%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_1->setText("6.7%");break;case 25:ui->lineEdit_Rerate_1->setText("5.2%");break;
            case 26:ui->lineEdit_Rerate_1->setText("4.3%");break;case 27:ui->lineEdit_Rerate_1->setText("6.15%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_1->setText("7.21%");break;case 29:ui->lineEdit_Rerate_1->setText("5.57%");break;
            case 30:ui->lineEdit_Rerate_1->setText("4.59%");break;case 31:ui->lineEdit_Rerate_1->setText("6.55%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_1->setText("7.48%");break;case 33:ui->lineEdit_Rerate_1->setText("5.78%");break;
            case 34:ui->lineEdit_Rerate_1->setText("4.76%");break;case 35:ui->lineEdit_Rerate_1->setText("6.8%");break;
            }
        }
        else
        {
            switch (ui->comboBox_Rerate_1->currentIndex())
            {

            //2020.4.20
            case 0:ui->lineEdit_Rerate_1->setText("3.85%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_1->setText("4.05%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_1->setText("4.2%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_1->setText("5.23%");break;
            case 4:ui->lineEdit_Rerate_1->setText("4.99%");break;case 5:ui->lineEdit_Rerate_1->setText("4.04%");break;
            case 6:ui->lineEdit_Rerate_1->setText("3.33%");break;case 7:ui->lineEdit_Rerate_1->setText("4.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_1->setText("5.5%");break;case 9:ui->lineEdit_Rerate_1->setText("4.25%");break;
            case 10:ui->lineEdit_Rerate_1->setText("3.5%");break;case 11:ui->lineEdit_Rerate_1->setText("5%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_1->setText("5.78%");break;case 13:ui->lineEdit_Rerate_1->setText("4.46%");break;
            case 14:ui->lineEdit_Rerate_1->setText("3.68%");break;case 15:ui->lineEdit_Rerate_1->setText("5.25%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_1->setText("6.05%");break;case 17:ui->lineEdit_Rerate_1->setText("4.68%");break;
            case 18:ui->lineEdit_Rerate_1->setText("3.85%");break;case 19:ui->lineEdit_Rerate_1->setText("5.5%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_1->setText("6.33%");break;case 21:ui->lineEdit_Rerate_1->setText("4.89%");break;
            case 22:ui->lineEdit_Rerate_1->setText("4.03%");break;case 23:ui->lineEdit_Rerate_1->setText("5.75%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_1->setText("6.6%");break;case 25:ui->lineEdit_Rerate_1->setText("5.1%");break;
            case 26:ui->lineEdit_Rerate_1->setText("4.2%");break;case 27:ui->lineEdit_Rerate_1->setText("6%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_1->setText("6.77%");break;case 29:ui->lineEdit_Rerate_1->setText("5.23%");break;
            case 30:ui->lineEdit_Rerate_1->setText("4.31%");break;case 31:ui->lineEdit_Rerate_1->setText("6.15%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_1->setText("7.04%");break;case 33:ui->lineEdit_Rerate_1->setText("5.44%");break;
            case 34:ui->lineEdit_Rerate_1->setText("4.48%");break;case 35:ui->lineEdit_Rerate_1->setText("6.4%");break;
            }
        }

    }
/**************************************切换为 组合型贷款*******************************************************************/
    else if(index == 2)
    {
        ui->stackedWidget->setCurrentWidget(ui->Calater_3);
        ui->comboBox_type_3->setCurrentIndex(0);

    }

/**************************************公积金贷款 值改变*******************************************************************/

    else
    {
        if(ui->comboBox_Redeadline_1->currentIndex() <20)
        {
            switch (ui->comboBox_Rerate_1->currentIndex())
            {
            //2020.4.20
            case 0:ui->lineEdit_Rerate_1->setText("3.25%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_1->setText("3.25%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_1->setText("3.25%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_1->setText("3.57%");break;
            case 4:ui->lineEdit_Rerate_1->setText("3.41%");break;case 5:ui->lineEdit_Rerate_1->setText("2.76%");break;
            case 6:ui->lineEdit_Rerate_1->setText("2.28%");break;case 7:ui->lineEdit_Rerate_1->setText("3.25%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_1->setText("3.57%");break;case 9:ui->lineEdit_Rerate_1->setText("2.76%");break;
            case 10:ui->lineEdit_Rerate_1->setText("2.28%");break;case 11:ui->lineEdit_Rerate_1->setText("3.25%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_1->setText("3.57%");break;case 13:ui->lineEdit_Rerate_1->setText("2.98%");break;
            case 14:ui->lineEdit_Rerate_1->setText("2.45%");break;case 15:ui->lineEdit_Rerate_1->setText("3.5%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_1->setText("4.13%");break;case 17:ui->lineEdit_Rerate_1->setText("3.19%");break;
            case 18:ui->lineEdit_Rerate_1->setText("2.63%");break;case 19:ui->lineEdit_Rerate_1->setText("3.75%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_1->setText("4.4%");break;case 21:ui->lineEdit_Rerate_1->setText("3.4%");break;
            case 22:ui->lineEdit_Rerate_1->setText("2.8%");break;case 23:ui->lineEdit_Rerate_1->setText("4%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_1->setText("4.25%");break;case 25:ui->lineEdit_Rerate_1->setText("4.25%");break;
            case 26:ui->lineEdit_Rerate_1->setText("4.25%");break;case 27:ui->lineEdit_Rerate_1->setText("4.25%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_1->setText("4.5%");break;case 29:ui->lineEdit_Rerate_1->setText("4.5%");break;
            case 30:ui->lineEdit_Rerate_1->setText("4.5%");break;case 31:ui->lineEdit_Rerate_1->setText("4.5%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_1->setText("4.7%");break;case 33:ui->lineEdit_Rerate_1->setText("4.7%");break;
            case 34:ui->lineEdit_Rerate_1->setText("4.7%");break;case 35:ui->lineEdit_Rerate_1->setText("4.7%");break;

            }
        }
        else
        {
            switch (ui->comboBox_Rerate_1->currentIndex())
            {

            //2020.4.20
            case 0:ui->lineEdit_Rerate_1->setText("2.75%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_1->setText("2.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_1->setText("2.75%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_1->setText("3.03%");break;
            case 4:ui->lineEdit_Rerate_1->setText("2.89%");break;case 5:ui->lineEdit_Rerate_1->setText("2.34%");break;
            case 6:ui->lineEdit_Rerate_1->setText("1.93%");break;case 7:ui->lineEdit_Rerate_1->setText("2.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_1->setText("3.03%");break;case 9:ui->lineEdit_Rerate_1->setText("2.34%");break;
            case 10:ui->lineEdit_Rerate_1->setText("1.93%");break;case 11:ui->lineEdit_Rerate_1->setText("2.75%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_1->setText("3.3%");break;case 13:ui->lineEdit_Rerate_1->setText("2.55%");break;
            case 14:ui->lineEdit_Rerate_1->setText("2.1%");break;case 15:ui->lineEdit_Rerate_1->setText("3%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_1->setText("3.57%");break;case 17:ui->lineEdit_Rerate_1->setText("2.76%");break;
            case 18:ui->lineEdit_Rerate_1->setText("2.28%");break;case 19:ui->lineEdit_Rerate_1->setText("3.25%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_1->setText("3.85%");break;case 21:ui->lineEdit_Rerate_1->setText("2.97%");break;
            case 22:ui->lineEdit_Rerate_1->setText("2.45%");break;case 23:ui->lineEdit_Rerate_1->setText("3.5%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_1->setText("3.75%");break;case 25:ui->lineEdit_Rerate_1->setText("3.75%");break;
            case 26:ui->lineEdit_Rerate_1->setText("3.75%");break;case 27:ui->lineEdit_Rerate_1->setText("3.75%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_1->setText("4%");break;case 29:ui->lineEdit_Rerate_1->setText("4%");break;
            case 30:ui->lineEdit_Rerate_1->setText("4%");break;case 31:ui->lineEdit_Rerate_1->setText("4%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_1->setText("4.2%");break;case 33:ui->lineEdit_Rerate_1->setText("4.2%");break;
            case 34:ui->lineEdit_Rerate_1->setText("4.2%");break;case 35:ui->lineEdit_Rerate_1->setText("4.2%");break;
            }
        }
    }
}
void Widget::on_comboBox_RecalaterWay_1_currentIndexChanged(int index)
{
    if(index == 1)
    {
        ui->stackedWidget->setCurrentWidget(ui->Calater_2);
        ui->comboBox_calaterWay_2->setCurrentIndex(0);
        switch (ui->comboBox_Retype_1->currentIndex())
        {
             case 0:ui->comboBox_type_2->setCurrentIndex(0);break;
             case 1:ui->comboBox_type_2->setCurrentIndex(1);break;
             default:
                 QMessageBox *eorr = new QMessageBox(this);
                 eorr->warning(this,"错误","你的选择有问题！",QMessageBox::Ok);
             break;
        }
    }
}
short Widget::on_comboBox_Redeadline_1_currentIndexChanged(int index)
{
        //按揭年数
        short Deadline = 0;
        switch (index)
        {
        case 0:Deadline = 300;break;case 1:Deadline = 288;break;
        case 2:Deadline = 276;break;case 3:Deadline = 264;break;
        case 4:Deadline = 252;break;case 5:Deadline = 240;break;
        case 6:Deadline = 228;break;case 7:Deadline = 216;break;
        case 8:Deadline = 204;break;case 9:Deadline = 192;break;
        case 10:Deadline = 180;break;case 11:Deadline = 168;break;
        case 12:Deadline = 156;break;case 13:Deadline = 144;break;
        case 14:Deadline = 132;break;case 15:Deadline = 120;break;
        case 16:Deadline = 108;break;case 17:Deadline = 96;break;
        case 18:Deadline = 84;break;case 19:Deadline = 72;break;
        case 20:Deadline = 60;break;case 21:Deadline = 48;break;
        case 22:Deadline = 36;break;case 23:Deadline = 24;break;
        case 24:Deadline = 12;break;
        }

        //商业贷款
        if(ui->comboBox_Retype_1->currentIndex() == 0)
        {
            if(index <20)//五年以上
            {
                switch (ui->comboBox_Rerate_1->currentIndex())
                {
                //2020.4.20
                case 0:ui->lineEdit_Rerate_1->setText("4.65%");break;
                //2020.3.20
                case 1:ui->lineEdit_Rerate_1->setText("4.75%");break;
                //2019.9.20
                case 2:ui->lineEdit_Rerate_1->setText("4.85%");break;
                //2015.10.24
                case 3:ui->lineEdit_Rerate_1->setText("5.39%");break;
                case 4:ui->lineEdit_Rerate_1->setText("5.15%");break;case 5:ui->lineEdit_Rerate_1->setText("4.17%");break;
                case 6:ui->lineEdit_Rerate_1->setText("3.43%");break;case 7:ui->lineEdit_Rerate_1->setText("4.9%");break;
                //2015.8.26
                case 8:ui->lineEdit_Rerate_1->setText("5.67%");break;case 9:ui->lineEdit_Rerate_1->setText("4.38%");break;
                case 10:ui->lineEdit_Rerate_1->setText("3.61%");break;case 11:ui->lineEdit_Rerate_1->setText("5.15%");break;
                //2015.6.28
                case 12:ui->lineEdit_Rerate_1->setText("5.94%");break;case 13:ui->lineEdit_Rerate_1->setText("4.59%");break;
                case 14:ui->lineEdit_Rerate_1->setText("3.78%");break;case 15:ui->lineEdit_Rerate_1->setText("5.4%");break;
                //2015.5.11
                case 16:ui->lineEdit_Rerate_1->setText("6.22%");break;case 17:ui->lineEdit_Rerate_1->setText("4.8%");break;
                case 18:ui->lineEdit_Rerate_1->setText("3.96%");break;case 19:ui->lineEdit_Rerate_1->setText("5.65%");break;
                //2015.3.1
                case 20:ui->lineEdit_Rerate_1->setText("6.49%");break;case 21:ui->lineEdit_Rerate_1->setText("5.01%");break;
                case 22:ui->lineEdit_Rerate_1->setText("4.13%");break;case 23:ui->lineEdit_Rerate_1->setText("5.9%");break;
                //2014.11.22
                case 24:ui->lineEdit_Rerate_1->setText("6.7%");break;case 25:ui->lineEdit_Rerate_1->setText("5.2%");break;
                case 26:ui->lineEdit_Rerate_1->setText("4.3%");break;case 27:ui->lineEdit_Rerate_1->setText("6.15%");break;
                //2012.7.6
                case 28:ui->lineEdit_Rerate_1->setText("7.21%");break;case 29:ui->lineEdit_Rerate_1->setText("5.57%");break;
                case 30:ui->lineEdit_Rerate_1->setText("4.59%");break;case 31:ui->lineEdit_Rerate_1->setText("6.55%");break;
                //2012.6.8
                case 32:ui->lineEdit_Rerate_1->setText("7.48%");break;case 33:ui->lineEdit_Rerate_1->setText("5.78%");break;
                case 34:ui->lineEdit_Rerate_1->setText("4.76%");break;case 35:ui->lineEdit_Rerate_1->setText("6.8%");break;
                }
            }
            else
            {
                switch (ui->comboBox_Rerate_1->currentIndex())
                {

                //2020.4.20
                case 0:ui->lineEdit_Rerate_1->setText("3.85%");break;
                //2020.3.20
                case 1:ui->lineEdit_Rerate_1->setText("4.05%");break;
                //2019.9.20
                case 2:ui->lineEdit_Rerate_1->setText("4.2%");break;
                //2015.10.24
                case 3:ui->lineEdit_Rerate_1->setText("5.23%");break;
                case 4:ui->lineEdit_Rerate_1->setText("4.99%");break;case 5:ui->lineEdit_Rerate_1->setText("4.04%");break;
                case 6:ui->lineEdit_Rerate_1->setText("3.33%");break;case 7:ui->lineEdit_Rerate_1->setText("4.75%");break;
                //2015.8.26
                case 8:ui->lineEdit_Rerate_1->setText("5.5%");break;case 9:ui->lineEdit_Rerate_1->setText("4.25%");break;
                case 10:ui->lineEdit_Rerate_1->setText("3.5%");break;case 11:ui->lineEdit_Rerate_1->setText("5%");break;
                //2015.6.28
                case 12:ui->lineEdit_Rerate_1->setText("5.78%");break;case 13:ui->lineEdit_Rerate_1->setText("4.46%");break;
                case 14:ui->lineEdit_Rerate_1->setText("3.68%");break;case 15:ui->lineEdit_Rerate_1->setText("5.25%");break;
                //2015.5.11
                case 16:ui->lineEdit_Rerate_1->setText("6.05%");break;case 17:ui->lineEdit_Rerate_1->setText("4.68%");break;
                case 18:ui->lineEdit_Rerate_1->setText("3.85%");break;case 19:ui->lineEdit_Rerate_1->setText("5.5%");break;
                //2015.3.1
                case 20:ui->lineEdit_Rerate_1->setText("6.33%");break;case 21:ui->lineEdit_Rerate_1->setText("4.89%");break;
                case 22:ui->lineEdit_Rerate_1->setText("4.03%");break;case 23:ui->lineEdit_Rerate_1->setText("5.75%");break;
                //2014.11.22
                case 24:ui->lineEdit_Rerate_1->setText("6.6%");break;case 25:ui->lineEdit_Rerate_1->setText("5.1%");break;
                case 26:ui->lineEdit_Rerate_1->setText("4.2%");break;case 27:ui->lineEdit_Rerate_1->setText("6%");break;
                //2012.7.6
                case 28:ui->lineEdit_Rerate_1->setText("6.77%");break;case 29:ui->lineEdit_Rerate_1->setText("5.23%");break;
                case 30:ui->lineEdit_Rerate_1->setText("4.31%");break;case 31:ui->lineEdit_Rerate_1->setText("6.15%");break;
                //2012.6.8
                case 32:ui->lineEdit_Rerate_1->setText("7.04%");break;case 33:ui->lineEdit_Rerate_1->setText("5.44%");break;
                case 34:ui->lineEdit_Rerate_1->setText("4.48%");break;case 35:ui->lineEdit_Rerate_1->setText("6.4%");break;
                }
            }
        }
        else //公积金贷款
        {
            if(index <20)
            {
                switch (ui->comboBox_Rerate_1->currentIndex())
                {
                //2020.4.20
                case 0:ui->lineEdit_Rerate_1->setText("3.25%");break;
                //2020.3.20
                case 1:ui->lineEdit_Rerate_1->setText("3.25%");break;
                //2019.9.20
                case 2:ui->lineEdit_Rerate_1->setText("3.25%");break;
                //2015.10.24
                case 3:ui->lineEdit_Rerate_1->setText("3.57%");break;
                case 4:ui->lineEdit_Rerate_1->setText("3.41%");break;case 5:ui->lineEdit_Rerate_1->setText("2.76%");break;
                case 6:ui->lineEdit_Rerate_1->setText("2.28%");break;case 7:ui->lineEdit_Rerate_1->setText("3.25%");break;
                //2015.8.26
                case 8:ui->lineEdit_Rerate_1->setText("3.57%");break;case 9:ui->lineEdit_Rerate_1->setText("2.76%");break;
                case 10:ui->lineEdit_Rerate_1->setText("2.28%");break;case 11:ui->lineEdit_Rerate_1->setText("3.25%");break;
                //2015.6.28
                case 12:ui->lineEdit_Rerate_1->setText("3.57%");break;case 13:ui->lineEdit_Rerate_1->setText("2.98%");break;
                case 14:ui->lineEdit_Rerate_1->setText("2.45%");break;case 15:ui->lineEdit_Rerate_1->setText("3.5%");break;
                //2015.5.11
                case 16:ui->lineEdit_Rerate_1->setText("4.13%");break;case 17:ui->lineEdit_Rerate_1->setText("3.19%");break;
                case 18:ui->lineEdit_Rerate_1->setText("2.63%");break;case 19:ui->lineEdit_Rerate_1->setText("3.75%");break;
                //2015.3.1
                case 20:ui->lineEdit_Rerate_1->setText("4.4%");break;case 21:ui->lineEdit_Rerate_1->setText("3.4%");break;
                case 22:ui->lineEdit_Rerate_1->setText("2.8%");break;case 23:ui->lineEdit_Rerate_1->setText("4%");break;
                //2014.11.22
                case 24:ui->lineEdit_Rerate_1->setText("4.25%");break;case 25:ui->lineEdit_Rerate_1->setText("4.25%");break;
                case 26:ui->lineEdit_Rerate_1->setText("4.25%");break;case 27:ui->lineEdit_Rerate_1->setText("4.25%");break;
                //2012.7.6
                case 28:ui->lineEdit_Rerate_1->setText("4.5%");break;case 29:ui->lineEdit_Rerate_1->setText("4.5%");break;
                case 30:ui->lineEdit_Rerate_1->setText("4.5%");break;case 31:ui->lineEdit_Rerate_1->setText("4.5%");break;
                //2012.6.8
                case 32:ui->lineEdit_Rerate_1->setText("4.7%");break;case 33:ui->lineEdit_Rerate_1->setText("4.7%");break;
                case 34:ui->lineEdit_Rerate_1->setText("4.7%");break;case 35:ui->lineEdit_Rerate_1->setText("4.7%");break;

                }
            }
            else
            {
                switch (ui->comboBox_Rerate_1->currentIndex())
                {

                //2020.4.20
                case 0:ui->lineEdit_Rerate_1->setText("2.75%");break;
                //2020.3.20
                case 1:ui->lineEdit_Rerate_1->setText("2.75%");break;
                //2019.9.20
                case 2:ui->lineEdit_Rerate_1->setText("2.75%");break;
                //2015.10.24
                case 3:ui->lineEdit_Rerate_1->setText("3.03%");break;
                case 4:ui->lineEdit_Rerate_1->setText("2.89%");break;case 5:ui->lineEdit_Rerate_1->setText("2.34%");break;
                case 6:ui->lineEdit_Rerate_1->setText("1.93%");break;case 7:ui->lineEdit_Rerate_1->setText("2.75%");break;
                //2015.8.26
                case 8:ui->lineEdit_Rerate_1->setText("3.03%");break;case 9:ui->lineEdit_Rerate_1->setText("2.34%");break;
                case 10:ui->lineEdit_Rerate_1->setText("1.93%");break;case 11:ui->lineEdit_Rerate_1->setText("2.75%");break;
                //2015.6.28
                case 12:ui->lineEdit_Rerate_1->setText("3.3%");break;case 13:ui->lineEdit_Rerate_1->setText("2.55%");break;
                case 14:ui->lineEdit_Rerate_1->setText("2.1%");break;case 15:ui->lineEdit_Rerate_1->setText("3%");break;
                //2015.5.11
                case 16:ui->lineEdit_Rerate_1->setText("3.57%");break;case 17:ui->lineEdit_Rerate_1->setText("2.76%");break;
                case 18:ui->lineEdit_Rerate_1->setText("2.28%");break;case 19:ui->lineEdit_Rerate_1->setText("3.25%");break;
                //2015.3.1
                case 20:ui->lineEdit_Rerate_1->setText("3.85%");break;case 21:ui->lineEdit_Rerate_1->setText("2.97%");break;
                case 22:ui->lineEdit_Rerate_1->setText("2.45%");break;case 23:ui->lineEdit_Rerate_1->setText("3.5%");break;
                //2014.11.22
                case 24:ui->lineEdit_Rerate_1->setText("3.75%");break;case 25:ui->lineEdit_Rerate_1->setText("3.75%");break;
                case 26:ui->lineEdit_Rerate_1->setText("3.75%");break;case 27:ui->lineEdit_Rerate_1->setText("3.75%");break;
                //2012.7.6
                case 28:ui->lineEdit_Rerate_1->setText("4%");break;case 29:ui->lineEdit_Rerate_1->setText("4%");break;
                case 30:ui->lineEdit_Rerate_1->setText("4%");break;case 31:ui->lineEdit_Rerate_1->setText("4%");break;
                //2012.6.8
                case 32:ui->lineEdit_Rerate_1->setText("4.2%");break;case 33:ui->lineEdit_Rerate_1->setText("4.2%");break;
                case 34:ui->lineEdit_Rerate_1->setText("4.2%");break;case 35:ui->lineEdit_Rerate_1->setText("4.2%");break;
                }
            }
        }

        return Deadline;
}
void Widget::on_comboBox_Rerate_1_currentIndexChanged(int index)
{
    if(ui->comboBox_Retype_1->currentIndex() == 0)
    {
        if(ui->comboBox_Redeadline_1->currentIndex() <20)//五年以上
        {
            switch (index)
            {
            //2020.4.20
            case 0:ui->lineEdit_Rerate_1->setText("4.65%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_1->setText("4.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_1->setText("4.85%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_1->setText("5.39%");break;
            case 4:ui->lineEdit_Rerate_1->setText("5.15%");break;case 5:ui->lineEdit_Rerate_1->setText("4.17%");break;
            case 6:ui->lineEdit_Rerate_1->setText("3.43%");break;case 7:ui->lineEdit_Rerate_1->setText("4.9%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_1->setText("5.67%");break;case 9:ui->lineEdit_Rerate_1->setText("4.38%");break;
            case 10:ui->lineEdit_Rerate_1->setText("3.61%");break;case 11:ui->lineEdit_Rerate_1->setText("5.15%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_1->setText("5.94%");break;case 13:ui->lineEdit_Rerate_1->setText("4.59%");break;
            case 14:ui->lineEdit_Rerate_1->setText("3.78%");break;case 15:ui->lineEdit_Rerate_1->setText("5.4%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_1->setText("6.22%");break;case 17:ui->lineEdit_Rerate_1->setText("4.8%");break;
            case 18:ui->lineEdit_Rerate_1->setText("3.96%");break;case 19:ui->lineEdit_Rerate_1->setText("5.65%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_1->setText("6.49%");break;case 21:ui->lineEdit_Rerate_1->setText("5.01%");break;
            case 22:ui->lineEdit_Rerate_1->setText("4.13%");break;case 23:ui->lineEdit_Rerate_1->setText("5.9%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_1->setText("6.7%");break;case 25:ui->lineEdit_Rerate_1->setText("5.2%");break;
            case 26:ui->lineEdit_Rerate_1->setText("4.3%");break;case 27:ui->lineEdit_Rerate_1->setText("6.15%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_1->setText("7.21%");break;case 29:ui->lineEdit_Rerate_1->setText("5.57%");break;
            case 30:ui->lineEdit_Rerate_1->setText("4.59%");break;case 31:ui->lineEdit_Rerate_1->setText("6.55%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_1->setText("7.48%");break;case 33:ui->lineEdit_Rerate_1->setText("5.78%");break;
            case 34:ui->lineEdit_Rerate_1->setText("4.76%");break;case 35:ui->lineEdit_Rerate_1->setText("6.8%");break;
            }
        }
        else
        {
            switch (index)
            {

            //2020.4.20
            case 0:ui->lineEdit_Rerate_1->setText("3.85%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_1->setText("4.05%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_1->setText("4.2%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_1->setText("5.23%");break;
            case 4:ui->lineEdit_Rerate_1->setText("4.99%");break;case 5:ui->lineEdit_Rerate_1->setText("4.04%");break;
            case 6:ui->lineEdit_Rerate_1->setText("3.33%");break;case 7:ui->lineEdit_Rerate_1->setText("4.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_1->setText("5.5%");break;case 9:ui->lineEdit_Rerate_1->setText("4.25%");break;
            case 10:ui->lineEdit_Rerate_1->setText("3.5%");break;case 11:ui->lineEdit_Rerate_1->setText("5%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_1->setText("5.78%");break;case 13:ui->lineEdit_Rerate_1->setText("4.46%");break;
            case 14:ui->lineEdit_Rerate_1->setText("3.68%");break;case 15:ui->lineEdit_Rerate_1->setText("5.25%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_1->setText("6.05%");break;case 17:ui->lineEdit_Rerate_1->setText("4.68%");break;
            case 18:ui->lineEdit_Rerate_1->setText("3.85%");break;case 19:ui->lineEdit_Rerate_1->setText("5.5%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_1->setText("6.33%");break;case 21:ui->lineEdit_Rerate_1->setText("4.89%");break;
            case 22:ui->lineEdit_Rerate_1->setText("4.03%");break;case 23:ui->lineEdit_Rerate_1->setText("5.75%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_1->setText("6.6%");break;case 25:ui->lineEdit_Rerate_1->setText("5.1%");break;
            case 26:ui->lineEdit_Rerate_1->setText("4.2%");break;case 27:ui->lineEdit_Rerate_1->setText("6%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_1->setText("6.77%");break;case 29:ui->lineEdit_Rerate_1->setText("5.23%");break;
            case 30:ui->lineEdit_Rerate_1->setText("4.31%");break;case 31:ui->lineEdit_Rerate_1->setText("6.15%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_1->setText("7.04%");break;case 33:ui->lineEdit_Rerate_1->setText("5.44%");break;
            case 34:ui->lineEdit_Rerate_1->setText("4.48%");break;case 35:ui->lineEdit_Rerate_1->setText("6.4%");break;
            }
        }
    }
    else //公积金贷款
    {
        if(ui->comboBox_Redeadline_1->currentIndex() <20)
        {
            switch (index)
            {
            //2020.4.20
            case 0:ui->lineEdit_Rerate_1->setText("3.25%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_1->setText("3.25%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_1->setText("3.25%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_1->setText("3.57%");break;
            case 4:ui->lineEdit_Rerate_1->setText("3.41%");break;case 5:ui->lineEdit_Rerate_1->setText("2.76%");break;
            case 6:ui->lineEdit_Rerate_1->setText("2.28%");break;case 7:ui->lineEdit_Rerate_1->setText("3.25%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_1->setText("3.57%");break;case 9:ui->lineEdit_Rerate_1->setText("2.76%");break;
            case 10:ui->lineEdit_Rerate_1->setText("2.28%");break;case 11:ui->lineEdit_Rerate_1->setText("3.25%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_1->setText("3.57%");break;case 13:ui->lineEdit_Rerate_1->setText("2.98%");break;
            case 14:ui->lineEdit_Rerate_1->setText("2.45%");break;case 15:ui->lineEdit_Rerate_1->setText("3.5%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_1->setText("4.13%");break;case 17:ui->lineEdit_Rerate_1->setText("3.19%");break;
            case 18:ui->lineEdit_Rerate_1->setText("2.63%");break;case 19:ui->lineEdit_Rerate_1->setText("3.75%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_1->setText("4.4%");break;case 21:ui->lineEdit_Rerate_1->setText("3.4%");break;
            case 22:ui->lineEdit_Rerate_1->setText("2.8%");break;case 23:ui->lineEdit_Rerate_1->setText("4%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_1->setText("4.25%");break;case 25:ui->lineEdit_Rerate_1->setText("4.25%");break;
            case 26:ui->lineEdit_Rerate_1->setText("4.25%");break;case 27:ui->lineEdit_Rerate_1->setText("4.25%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_1->setText("4.5%");break;case 29:ui->lineEdit_Rerate_1->setText("4.5%");break;
            case 30:ui->lineEdit_Rerate_1->setText("4.5%");break;case 31:ui->lineEdit_Rerate_1->setText("4.5%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_1->setText("4.7%");break;case 33:ui->lineEdit_Rerate_1->setText("4.7%");break;
            case 34:ui->lineEdit_Rerate_1->setText("4.7%");break;case 35:ui->lineEdit_Rerate_1->setText("4.7%");break;

            }
        }
        else
        {
            switch (index)
            {

            //2020.4.20
            case 0:ui->lineEdit_Rerate_1->setText("2.75%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_1->setText("2.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_1->setText("2.75%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_1->setText("3.03%");break;
            case 4:ui->lineEdit_Rerate_1->setText("2.89%");break;case 5:ui->lineEdit_Rerate_1->setText("2.34%");break;
            case 6:ui->lineEdit_Rerate_1->setText("1.93%");break;case 7:ui->lineEdit_Rerate_1->setText("2.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_1->setText("3.03%");break;case 9:ui->lineEdit_Rerate_1->setText("2.34%");break;
            case 10:ui->lineEdit_Rerate_1->setText("1.93%");break;case 11:ui->lineEdit_Rerate_1->setText("2.75%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_1->setText("3.3%");break;case 13:ui->lineEdit_Rerate_1->setText("2.55%");break;
            case 14:ui->lineEdit_Rerate_1->setText("2.1%");break;case 15:ui->lineEdit_Rerate_1->setText("3%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_1->setText("3.57%");break;case 17:ui->lineEdit_Rerate_1->setText("2.76%");break;
            case 18:ui->lineEdit_Rerate_1->setText("2.28%");break;case 19:ui->lineEdit_Rerate_1->setText("3.25%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_1->setText("3.85%");break;case 21:ui->lineEdit_Rerate_1->setText("2.97%");break;
            case 22:ui->lineEdit_Rerate_1->setText("2.45%");break;case 23:ui->lineEdit_Rerate_1->setText("3.5%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_1->setText("3.75%");break;case 25:ui->lineEdit_Rerate_1->setText("3.75%");break;
            case 26:ui->lineEdit_Rerate_1->setText("3.75%");break;case 27:ui->lineEdit_Rerate_1->setText("3.75%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_1->setText("4%");break;case 29:ui->lineEdit_Rerate_1->setText("4%");break;
            case 30:ui->lineEdit_Rerate_1->setText("4%");break;case 31:ui->lineEdit_Rerate_1->setText("4%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_1->setText("4.2%");break;case 33:ui->lineEdit_Rerate_1->setText("4.2%");break;
            case 34:ui->lineEdit_Rerate_1->setText("4.2%");break;case 35:ui->lineEdit_Rerate_1->setText("4.2%");break;
            }
        }
    }

}
void Widget::on_pushButton_Restart_1_clicked()
{
    // 第一个页面处理
    QString checkValue = ui->lineEdit_Revalue_1->text();
    QString checkArea = ui->lineEdit_Rearea_1->text();
    QString checkRate = ui->lineEdit_Rerate_1->text();
    bool Justise = 0;
    double rate = 0;
    //判断是否为空
    if(!checkValue.isEmpty()&&!checkArea.isEmpty())
    {
        int checkPoint = 0;
        bool flag1 = 0, flag2 = 0,flag3 = 0;
        //判断输入单价
        for(int i= 0 ;i<checkValue.length();i++)
        {
            if((checkValue[i]<='9'&&checkValue[i]>='0')||checkValue[i]=='.')
            {
                if(checkValue[i]=='.')
                    checkPoint++;
                if(checkPoint>1){flag1=1;break;}
            }
            else{flag1 = 1;break;}
        }

        //判断输入面积
        checkPoint = 0;
        for(int i= 0 ;i<checkArea.length();i++)
        {
            if((checkArea[i]<='9'&&checkArea[i]>='0')||checkArea[i]=='.')
            {
                if(checkArea[i]=='.')
                    checkPoint++;
                if(checkPoint>1){flag2=1;break;}
            }
            else{flag2 = 1;break;}
        }

        //判断输入利率
        checkPoint = 0;
        int n = 0;
        for(int i= 0 ;i<checkRate.length()-1;i++)
        {
            if((checkRate[i]<='9'&&checkRate[i]>='0')||checkRate[i]=='.')
            {
                QChar te = checkRate.at(i);
                if(checkRate[i]=='.')
                {
                    checkPoint++;
                }
                if(checkPoint>=2){flag3=1;break;}
                if(checkRate[i]!='.')
                {
                    rate = rate*10+(te.toLatin1()-'0');
                    n++;
                }
            }
            else{flag3 = 1;break;}
        }
        rate /= pow(10,n+1);
        if(rate == 0)
            flag3 = 1;

        if(flag1 == 1 && flag2 == 1 && flag3 == 1)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入的房屋单价、面积、利率有误",QMessageBox::Ok);
        }
        else if(flag1 == 1 && flag2 == 0 && flag3 == 0)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋单价有误",QMessageBox::Ok);
        }
        else if(flag1 == 0 && flag2 == 1 && flag3 == 0)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋面积有误",QMessageBox::Ok);
        }
        else if(flag1 == 0 && flag2 == 0 && flag3 == 1)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋利率不合法，请重新输入",QMessageBox::Ok);
        }
        else if(flag1 == 1 && flag2 == 0 && flag3 == 1)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋单价、利率有误",QMessageBox::Ok);
        }
        else if(flag1 == 0 && flag2 == 1 && flag3 == 1)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋面积、利率有误",QMessageBox::Ok);
        }
        else if(flag1 == 1 && flag2 == 1 && flag3 == 0)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋单价、面积有误",QMessageBox::Ok);
        }
        else
        {
            Justise = 1;
        }
    }
    //如果有输入有空
    else
    {
        if(checkValue.isEmpty()&&checkArea.isEmpty()&&checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋单价、面积、利率",QMessageBox::Ok);
        }
        else if(checkValue.isEmpty()&&!checkArea.isEmpty()&&!checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋单价",QMessageBox::Ok);
        }
        else if(!checkValue.isEmpty()&&checkArea.isEmpty()&&!checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋面积",QMessageBox::Ok);
        }
        else if(!checkValue.isEmpty()&&!checkArea.isEmpty()&&checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋利率",QMessageBox::Ok);
        }
        else if(!checkValue.isEmpty()&&checkArea.isEmpty()&&checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋面积、利率",QMessageBox::Ok);
        }
        else if(checkValue.isEmpty()&&!checkArea.isEmpty()&&checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋单价、利率",QMessageBox::Ok);
        }
        else if(checkValue.isEmpty()&&checkArea.isEmpty()&&!checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋面积、单价",QMessageBox::Ok);
        }
    }

    if(Justise ==1 )
    {
        double Percentage = 0;
        switch (ui->comboBox_Repercent_1->currentIndex())
        {
        case 0:Percentage = 0.9;break;case 1:Percentage = 0.85;break;
        case 2:Percentage = 0.8;break;case 3:Percentage = 0.75;break;
        case 4:Percentage = 0.7;break;case 5:Percentage = 0.65;break;
        case 6:Percentage = 0.6;break;case 7:Percentage = 0.55;break;
        case 8:Percentage = 0.5;break;case 9:Percentage = 0.45;break;
        case 10:Percentage = 0.4;break;case 11:Percentage = 0.35;break;
        case 12:Percentage = 0.3;break;case 13:Percentage = 0.25;break;
        case 14:Percentage = 0.2;break;
        }
        //房屋单价值
        double value = ui->lineEdit_Revalue_1->text().toDouble();
        ui->lineEdit_Revalue_1->setText(ui->lineEdit_Revalue_1->text());
        //房屋面积值
        double area = ui->lineEdit_Rearea_1->text().toDouble();

        ui->lineEdit_Rearea_1->setText(ui->lineEdit_Rearea_1->text());
        short Deadline =on_comboBox_Redeadline_1_currentIndexChanged(ui->comboBox_Redeadline_1->currentIndex());

//        还款日期下拉框
        ui->comboBox_Redeadline_1->setCurrentIndex(ui->comboBox_Redeadline_1->currentIndex());

//        利率下拉框
        ui->comboBox_Rerate_1->setCurrentIndex(ui->comboBox_Rerate_1->currentIndex());
        ui->lineEdit_Rerate_1->setText(QString("%1%").arg(rate*100));
//        按揭成数下拉框
        ui->comboBox_Repercent_1->setCurrentIndex(ui->comboBox_Repercent_1->currentIndex());

//        贷款类型下拉框
        ui->comboBox_Retype_1->setCurrentIndex(ui->comboBox_Retype_1->currentIndex());

        commercial_loan A(value,area,Percentage,Deadline,rate);

        //等额本息
        if(ui->radioButton_Rebenxi_1->isChecked())
        {

            ui->radioButton_Rebenxi_1->clicked();
            ui->label_Remonthpay_change->setText("每月月供:");
            ui->widget_Redecline_1->hide();
            //首付
            A.Average_Capital_Plus_Interest();
            //首付
            ui->label_ReFirst_1->setText(QString::number(A.Getfirstpayment(), 'f', 1));
            //月供
            ui->label_Remonthpay_1->setText(QString::number(A.Getmonthpayment(), 'f', 1));
            //贷款总额
            ui->label_ReLoan_1->setText(QString::number(A.Getloan(), 'f', 2));
            //支付利息
            ui->label_Reinterest_1->setText(QString::number(A.Getinterest(), 'f', 1));
            //还款总额
            ui->label_Retotalpay_1->setText(QString::number(A.Getendpayment(), 'f', 1));
            //还款月数
            ui->label_Redate_1->setText(QString::number(A.Getdeadline(), 'f', 1));
        }
        //等额本金
        else if(ui->radioButton_Rebenjin_1->isChecked())
        {
            ui->radioButton_Rebenjin_1->clicked();
            ui->widget_Redecline_1->show();
            ui->label_Remonthpay_change->setText("首月月供:");
            A.Average_capital();
            //首付
            ui->label_ReFirst_1->setText(QString::number(A.Getfirstpayment(), 'f', 1));
            //月供
            ui->label_Remonthpay_1->setText(QString::number(A.Getmonthpayment(), 'f', 1));
            //每月递减
            ui->label_Redecline_1->setText(QString::number(A.Getmonth_cut(), 'f', 1));
            //贷款总额
            ui->label_ReLoan_1->setText(QString::number(A.Getloan(), 'f', 1));
            //支付利息
            ui->label_Reinterest_1->setText(QString::number(A.Getinterest(), 'f', 1));
            //还款总额
            ui->label_Retotalpay_1->setText(QString::number(A.Getendpayment(), 'f', 1));
            //还款月数
            ui->label_Redate_1->setText(QString::number(A.Getdeadline(), 'f', 1));

        }
        ui->stackedWidget->setCurrentWidget(ui->Calater_1_Result);
    }
}
void Widget::on_pushButton_Recance_1_clicked()
{
    int ret = QMessageBox::question(this,"Question","是否需要清除数据？");
    if(ret == QMessageBox::Yes)
    {
        //关闭窗口;
        ui->lineEdit_Revalue_1->clear();
        ui->lineEdit_Rearea_1->clear();
        ui->comboBox_Repercent_1->setCurrentIndex(0);
        ui->comboBox_Redeadline_1->setCurrentText(0);
        ui->comboBox_Rerate_1->setCurrentText(0);
        ui->radioButton_Rebenxi_1->clicked();
        ui->lineEdit_value->clear();
        ui->lineEdit_area->clear();
        ui->comboBox_percent->setCurrentIndex(0);
        ui->comboBox_deadline->setCurrentText(0);
        ui->comboBox_rate->setCurrentText(0);
        ui->radioButton_benxi->clicked();
        ui->stackedWidget->setCurrentWidget(ui->Calater_1);

        if(ui->comboBox_Retype_1->currentIndex() == 0)
            ui->lineEdit_Rerate_1->setText("4.65%");
        else
        {
            ui->lineEdit_Rerate_1->setText("3.25%");
        }

        if(ui->comboBox_type->currentIndex() == 0)
            ui->lineEdit_rate->setText("4.65%");
        else
        {
            ui->lineEdit_rate->setText("3.25%");
        }


    }
    else if(ret == QMessageBox::No)
    {
        //保持不动
        //忽略事件，事件继续给父组件传递
    }

}
/******************************************-----End_Result.NO.1-----*************************************************************/


/******************************************-----Begin_Calater_2 -----************************************************************/
void Widget::on_comboBox_type_2_currentIndexChanged(int index)
{
    if(index == 0)
    {
        if(ui->comboBox_deadline_2->currentIndex() <20)
        {
            switch (ui->comboBox_rate_2->currentIndex())
            {
            //2020.4.20
            case 0:ui->lineEdit_rate_2->setText("4.65%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate_2->setText("4.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate_2->setText("4.85%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate_2->setText("5.39%");break;
            case 4:ui->lineEdit_rate_2->setText("5.15%");break;case 5:ui->lineEdit_rate_2->setText("4.17%");break;
            case 6:ui->lineEdit_rate_2->setText("3.43%");break;case 7:ui->lineEdit_rate_2->setText("4.9%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate_2->setText("5.67%");break;case 9:ui->lineEdit_rate_2->setText("4.38%");break;
            case 10:ui->lineEdit_rate_2->setText("3.61%");break;case 11:ui->lineEdit_rate_2->setText("5.15%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate_2->setText("5.94%");break;case 13:ui->lineEdit_rate_2->setText("4.59%");break;
            case 14:ui->lineEdit_rate_2->setText("3.78%");break;case 15:ui->lineEdit_rate_2->setText("5.4%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate_2->setText("6.22%");break;case 17:ui->lineEdit_rate_2->setText("4.8%");break;
            case 18:ui->lineEdit_rate_2->setText("3.96%");break;case 19:ui->lineEdit_rate_2->setText("5.65%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate_2->setText("6.49%");break;case 21:ui->lineEdit_rate_2->setText("5.01%");break;
            case 22:ui->lineEdit_rate_2->setText("4.13%");break;case 23:ui->lineEdit_rate_2->setText("5.9%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate_2->setText("6.7%");break;case 25:ui->lineEdit_rate_2->setText("5.2%");break;
            case 26:ui->lineEdit_rate_2->setText("4.3%");break;case 27:ui->lineEdit_rate_2->setText("6.15%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate_2->setText("7.21%");break;case 29:ui->lineEdit_rate_2->setText("5.57%");break;
            case 30:ui->lineEdit_rate_2->setText("4.59%");break;case 31:ui->lineEdit_rate_2->setText("6.55%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate_2->setText("7.48%");break;case 33:ui->lineEdit_rate_2->setText("5.78%");break;
            case 34:ui->lineEdit_rate_2->setText("4.76%");break;case 35:ui->lineEdit_rate_2->setText("6.8%");break;
            }
        }
        else
        {
            switch (ui->comboBox_rate_2->currentIndex())
            {

            //2020.4.20
            case 0:ui->lineEdit_rate_2->setText("3.85%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate_2->setText("4.05%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate_2->setText("4.2%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate_2->setText("5.23%");break;
            case 4:ui->lineEdit_rate_2->setText("4.99%");break;case 5:ui->lineEdit_rate_2->setText("4.04%");break;
            case 6:ui->lineEdit_rate_2->setText("3.33%");break;case 7:ui->lineEdit_rate_2->setText("4.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate_2->setText("5.5%");break;case 9:ui->lineEdit_rate_2->setText("4.25%");break;
            case 10:ui->lineEdit_rate_2->setText("3.5%");break;case 11:ui->lineEdit_rate_2->setText("5%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate_2->setText("5.78%");break;case 13:ui->lineEdit_rate_2->setText("4.46%");break;
            case 14:ui->lineEdit_rate_2->setText("3.68%");break;case 15:ui->lineEdit_rate_2->setText("5.25%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate_2->setText("6.05%");break;case 17:ui->lineEdit_rate_2->setText("4.68%");break;
            case 18:ui->lineEdit_rate_2->setText("3.85%");break;case 19:ui->lineEdit_rate_2->setText("5.5%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate_2->setText("6.33%");break;case 21:ui->lineEdit_rate_2->setText("4.89%");break;
            case 22:ui->lineEdit_rate_2->setText("4.03%");break;case 23:ui->lineEdit_rate_2->setText("5.75%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate_2->setText("6.6%");break;case 25:ui->lineEdit_rate_2->setText("5.1%");break;
            case 26:ui->lineEdit_rate_2->setText("4.2%");break;case 27:ui->lineEdit_rate_2->setText("6%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate_2->setText("6.77%");break;case 29:ui->lineEdit_rate_2->setText("5.23%");break;
            case 30:ui->lineEdit_rate_2->setText("4.31%");break;case 31:ui->lineEdit_rate_2->setText("6.15%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate_2->setText("7.04%");break;case 33:ui->lineEdit_rate_2->setText("5.44%");break;
            case 34:ui->lineEdit_rate_2->setText("4.48%");break;case 35:ui->lineEdit_rate_2->setText("6.4%");break;
            }
        }

    }
/**************************************切换为 组合型贷款*******************************************************************/
    else if(index == 2)
    {
        ui->stackedWidget->setCurrentWidget(ui->Calater_3);
        ui->comboBox_type_3->setCurrentIndex(0);
    }


/**************************************公积金贷款 值改变*******************************************************************/

    else
    {
        if(ui->comboBox_deadline_2->currentIndex() <20)
        {
            switch (ui->comboBox_rate_2->currentIndex())
            {
            //2020.4.20
            case 0:ui->lineEdit_rate_2->setText("3.25%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate_2->setText("3.25%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate_2->setText("3.25%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate_2->setText("3.57%");break;
            case 4:ui->lineEdit_rate_2->setText("3.41%");break;case 5:ui->lineEdit_rate_2->setText("2.76%");break;
            case 6:ui->lineEdit_rate_2->setText("2.28%");break;case 7:ui->lineEdit_rate_2->setText("3.25%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate_2->setText("3.57%");break;case 9:ui->lineEdit_rate_2->setText("2.76%");break;
            case 10:ui->lineEdit_rate_2->setText("2.28%");break;case 11:ui->lineEdit_rate_2->setText("3.25%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate_2->setText("3.57%");break;case 13:ui->lineEdit_rate_2->setText("2.98%");break;
            case 14:ui->lineEdit_rate_2->setText("2.45%");break;case 15:ui->lineEdit_rate_2->setText("3.5%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate_2->setText("4.13%");break;case 17:ui->lineEdit_rate_2->setText("3.19%");break;
            case 18:ui->lineEdit_rate_2->setText("2.63%");break;case 19:ui->lineEdit_rate_2->setText("3.75%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate_2->setText("4.4%");break;case 21:ui->lineEdit_rate_2->setText("3.4%");break;
            case 22:ui->lineEdit_rate_2->setText("2.8%");break;case 23:ui->lineEdit_rate_2->setText("4%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate_2->setText("4.25%");break;case 25:ui->lineEdit_rate_2->setText("4.25%");break;
            case 26:ui->lineEdit_rate_2->setText("4.25%");break;case 27:ui->lineEdit_rate_2->setText("4.25%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate_2->setText("4.5%");break;case 29:ui->lineEdit_rate_2->setText("4.5%");break;
            case 30:ui->lineEdit_rate_2->setText("4.5%");break;case 31:ui->lineEdit_rate_2->setText("4.5%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate_2->setText("4.7%");break;case 33:ui->lineEdit_rate_2->setText("4.7%");break;
            case 34:ui->lineEdit_rate_2->setText("4.7%");break;case 35:ui->lineEdit_rate_2->setText("4.7%");break;

            }
        }
        else
        {
            switch (ui->comboBox_rate_2->currentIndex())
            {

            //2020.4.20
            case 0:ui->lineEdit_rate_2->setText("2.75%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate_2->setText("2.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate_2->setText("2.75%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate_2->setText("3.03%");break;
            case 4:ui->lineEdit_rate_2->setText("2.89%");break;case 5:ui->lineEdit_rate_2->setText("2.34%");break;
            case 6:ui->lineEdit_rate_2->setText("1.93%");break;case 7:ui->lineEdit_rate_2->setText("2.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate_2->setText("3.03%");break;case 9:ui->lineEdit_rate_2->setText("2.34%");break;
            case 10:ui->lineEdit_rate_2->setText("1.93%");break;case 11:ui->lineEdit_rate_2->setText("2.75%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate_2->setText("3.3%");break;case 13:ui->lineEdit_rate_2->setText("2.55%");break;
            case 14:ui->lineEdit_rate_2->setText("2.1%");break;case 15:ui->lineEdit_rate_2->setText("3%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate_2->setText("3.57%");break;case 17:ui->lineEdit_rate_2->setText("2.76%");break;
            case 18:ui->lineEdit_rate_2->setText("2.28%");break;case 19:ui->lineEdit_rate_2->setText("3.25%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate_2->setText("3.85%");break;case 21:ui->lineEdit_rate_2->setText("2.97%");break;
            case 22:ui->lineEdit_rate_2->setText("2.45%");break;case 23:ui->lineEdit_rate_2->setText("3.5%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate_2->setText("3.75%");break;case 25:ui->lineEdit_rate_2->setText("3.75%");break;
            case 26:ui->lineEdit_rate_2->setText("3.75%");break;case 27:ui->lineEdit_rate_2->setText("3.75%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate_2->setText("4%");break;case 29:ui->lineEdit_rate_2->setText("4%");break;
            case 30:ui->lineEdit_rate_2->setText("4%");break;case 31:ui->lineEdit_rate_2->setText("4%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate_2->setText("4.2%");break;case 33:ui->lineEdit_rate_2->setText("4.2%");break;
            case 34:ui->lineEdit_rate_2->setText("4.2%");break;case 35:ui->lineEdit_rate_2->setText("4.2%");break;
            }
        }
    }
}
void Widget::on_comboBox_calaterWay_2_currentIndexChanged(int index)
{

    if(index == 1)
    {
        ui->stackedWidget->setCurrentWidget(ui->Calater_1);
        ui->comboBox_calaterWay->setCurrentIndex(0);
        switch (ui->comboBox_type_2->currentIndex())
        {
             case 0:ui->comboBox_type->setCurrentIndex(0);break;
             case 1:ui->comboBox_type->setCurrentIndex(1);break;
             default:
                 QMessageBox *eorr = new QMessageBox(this);
                 eorr->warning(this,"错误","你的选择有问题！",QMessageBox::Ok);
             break;
        }
    }

}
short Widget::on_comboBox_deadline_2_currentIndexChanged(int index)
{
    //按揭年数
    short Deadline = 0;
    switch (index)
    {
    case 0:Deadline = 300;break;case 1:Deadline = 288;break;
    case 2:Deadline = 276;break;case 3:Deadline = 264;break;
    case 4:Deadline = 252;break;case 5:Deadline = 240;break;
    case 6:Deadline = 228;break;case 7:Deadline = 216;break;
    case 8:Deadline = 204;break;case 9:Deadline = 192;break;
    case 10:Deadline = 180;break;case 11:Deadline = 168;break;
    case 12:Deadline = 156;break;case 13:Deadline = 144;break;
    case 14:Deadline = 132;break;case 15:Deadline = 120;break;
    case 16:Deadline = 108;break;case 17:Deadline = 96;break;
    case 18:Deadline = 84;break;case 19:Deadline = 72;break;
    case 20:Deadline = 60;break;case 21:Deadline = 48;break;
    case 22:Deadline = 36;break;case 23:Deadline = 24;break;
    case 24:Deadline = 12;break;
    }

    //商业贷款
    if(ui->comboBox_type_2->currentIndex()  == 0)
    {
        if(index <20)
        {
            switch (ui->comboBox_rate_2->currentIndex())
            {
            //2020.4.20
            case 0:ui->lineEdit_rate_2->setText("4.65%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate_2->setText("4.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate_2->setText("4.85%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate_2->setText("5.39%");break;
            case 4:ui->lineEdit_rate_2->setText("5.15%");break;case 5:ui->lineEdit_rate_2->setText("4.17%");break;
            case 6:ui->lineEdit_rate_2->setText("3.43%");break;case 7:ui->lineEdit_rate_2->setText("4.9%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate_2->setText("5.67%");break;case 9:ui->lineEdit_rate_2->setText("4.38%");break;
            case 10:ui->lineEdit_rate_2->setText("3.61%");break;case 11:ui->lineEdit_rate_2->setText("5.15%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate_2->setText("5.94%");break;case 13:ui->lineEdit_rate_2->setText("4.59%");break;
            case 14:ui->lineEdit_rate_2->setText("3.78%");break;case 15:ui->lineEdit_rate_2->setText("5.4%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate_2->setText("6.22%");break;case 17:ui->lineEdit_rate_2->setText("4.8%");break;
            case 18:ui->lineEdit_rate_2->setText("3.96%");break;case 19:ui->lineEdit_rate_2->setText("5.65%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate_2->setText("6.49%");break;case 21:ui->lineEdit_rate_2->setText("5.01%");break;
            case 22:ui->lineEdit_rate_2->setText("4.13%");break;case 23:ui->lineEdit_rate_2->setText("5.9%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate_2->setText("6.7%");break;case 25:ui->lineEdit_rate_2->setText("5.2%");break;
            case 26:ui->lineEdit_rate_2->setText("4.3%");break;case 27:ui->lineEdit_rate_2->setText("6.15%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate_2->setText("7.21%");break;case 29:ui->lineEdit_rate_2->setText("5.57%");break;
            case 30:ui->lineEdit_rate_2->setText("4.59%");break;case 31:ui->lineEdit_rate_2->setText("6.55%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate_2->setText("7.48%");break;case 33:ui->lineEdit_rate_2->setText("5.78%");break;
            case 34:ui->lineEdit_rate_2->setText("4.76%");break;case 35:ui->lineEdit_rate_2->setText("6.8%");break;
            }
        }
        else
        {
            switch (ui->comboBox_rate_2->currentIndex())
            {

            //2020.4.20
            case 0:ui->lineEdit_rate_2->setText("3.85%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate_2->setText("4.05%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate_2->setText("4.2%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate_2->setText("5.23%");break;
            case 4:ui->lineEdit_rate_2->setText("4.99%");break;case 5:ui->lineEdit_rate_2->setText("4.04%");break;
            case 6:ui->lineEdit_rate_2->setText("3.33%");break;case 7:ui->lineEdit_rate_2->setText("4.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate_2->setText("5.5%");break;case 9:ui->lineEdit_rate_2->setText("4.25%");break;
            case 10:ui->lineEdit_rate_2->setText("3.5%");break;case 11:ui->lineEdit_rate_2->setText("5%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate_2->setText("5.78%");break;case 13:ui->lineEdit_rate_2->setText("4.46%");break;
            case 14:ui->lineEdit_rate_2->setText("3.68%");break;case 15:ui->lineEdit_rate_2->setText("5.25%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate_2->setText("6.05%");break;case 17:ui->lineEdit_rate_2->setText("4.68%");break;
            case 18:ui->lineEdit_rate_2->setText("3.85%");break;case 19:ui->lineEdit_rate_2->setText("5.5%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate_2->setText("6.33%");break;case 21:ui->lineEdit_rate_2->setText("4.89%");break;
            case 22:ui->lineEdit_rate_2->setText("4.03%");break;case 23:ui->lineEdit_rate_2->setText("5.75%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate_2->setText("6.6%");break;case 25:ui->lineEdit_rate_2->setText("5.1%");break;
            case 26:ui->lineEdit_rate_2->setText("4.2%");break;case 27:ui->lineEdit_rate_2->setText("6%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate_2->setText("6.77%");break;case 29:ui->lineEdit_rate_2->setText("5.23%");break;
            case 30:ui->lineEdit_rate_2->setText("4.31%");break;case 31:ui->lineEdit_rate_2->setText("6.15%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate_2->setText("7.04%");break;case 33:ui->lineEdit_rate_2->setText("5.44%");break;
            case 34:ui->lineEdit_rate_2->setText("4.48%");break;case 35:ui->lineEdit_rate_2->setText("6.4%");break;
            }
        }

    }
/**************************************切换为 组合型贷款*******************************************************************/
    else if(ui->comboBox_type_2->currentIndex() == 2) ui->stackedWidget->setCurrentWidget(ui->Calater_3);

/**************************************公积金贷款 值改变*******************************************************************/

    else
    {
        if(index <20)
        {
            switch (ui->comboBox_rate_2->currentIndex())
            {
            //2020.4.20
            case 0:ui->lineEdit_rate_2->setText("3.25%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate_2->setText("3.25%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate_2->setText("3.25%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate_2->setText("3.57%");break;
            case 4:ui->lineEdit_rate_2->setText("3.41%");break;case 5:ui->lineEdit_rate_2->setText("2.76%");break;
            case 6:ui->lineEdit_rate_2->setText("2.28%");break;case 7:ui->lineEdit_rate_2->setText("3.25%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate_2->setText("3.57%");break;case 9:ui->lineEdit_rate_2->setText("2.76%");break;
            case 10:ui->lineEdit_rate_2->setText("2.28%");break;case 11:ui->lineEdit_rate_2->setText("3.25%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate_2->setText("3.57%");break;case 13:ui->lineEdit_rate_2->setText("2.98%");break;
            case 14:ui->lineEdit_rate_2->setText("2.45%");break;case 15:ui->lineEdit_rate_2->setText("3.5%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate_2->setText("4.13%");break;case 17:ui->lineEdit_rate_2->setText("3.19%");break;
            case 18:ui->lineEdit_rate_2->setText("2.63%");break;case 19:ui->lineEdit_rate_2->setText("3.75%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate_2->setText("4.4%");break;case 21:ui->lineEdit_rate_2->setText("3.4%");break;
            case 22:ui->lineEdit_rate_2->setText("2.8%");break;case 23:ui->lineEdit_rate_2->setText("4%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate_2->setText("4.25%");break;case 25:ui->lineEdit_rate_2->setText("4.25%");break;
            case 26:ui->lineEdit_rate_2->setText("4.25%");break;case 27:ui->lineEdit_rate_2->setText("4.25%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate_2->setText("4.5%");break;case 29:ui->lineEdit_rate_2->setText("4.5%");break;
            case 30:ui->lineEdit_rate_2->setText("4.5%");break;case 31:ui->lineEdit_rate_2->setText("4.5%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate_2->setText("4.7%");break;case 33:ui->lineEdit_rate_2->setText("4.7%");break;
            case 34:ui->lineEdit_rate_2->setText("4.7%");break;case 35:ui->lineEdit_rate_2->setText("4.7%");break;

            }
        }
        else
        {
            switch (ui->comboBox_rate_2->currentIndex())
            {

            //2020.4.20
            case 0:ui->lineEdit_rate_2->setText("2.75%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate_2->setText("2.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate_2->setText("2.75%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate_2->setText("3.03%");break;
            case 4:ui->lineEdit_rate_2->setText("2.89%");break;case 5:ui->lineEdit_rate_2->setText("2.34%");break;
            case 6:ui->lineEdit_rate_2->setText("1.93%");break;case 7:ui->lineEdit_rate_2->setText("2.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate_2->setText("3.03%");break;case 9:ui->lineEdit_rate_2->setText("2.34%");break;
            case 10:ui->lineEdit_rate_2->setText("1.93%");break;case 11:ui->lineEdit_rate_2->setText("2.75%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate_2->setText("3.3%");break;case 13:ui->lineEdit_rate_2->setText("2.55%");break;
            case 14:ui->lineEdit_rate_2->setText("2.1%");break;case 15:ui->lineEdit_rate_2->setText("3%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate_2->setText("3.57%");break;case 17:ui->lineEdit_rate_2->setText("2.76%");break;
            case 18:ui->lineEdit_rate_2->setText("2.28%");break;case 19:ui->lineEdit_rate_2->setText("3.25%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate_2->setText("3.85%");break;case 21:ui->lineEdit_rate_2->setText("2.97%");break;
            case 22:ui->lineEdit_rate_2->setText("2.45%");break;case 23:ui->lineEdit_rate_2->setText("3.5%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate_2->setText("3.75%");break;case 25:ui->lineEdit_rate_2->setText("3.75%");break;
            case 26:ui->lineEdit_rate_2->setText("3.75%");break;case 27:ui->lineEdit_rate_2->setText("3.75%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate_2->setText("4%");break;case 29:ui->lineEdit_rate_2->setText("4%");break;
            case 30:ui->lineEdit_rate_2->setText("4%");break;case 31:ui->lineEdit_rate_2->setText("4%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate_2->setText("4.2%");break;case 33:ui->lineEdit_rate_2->setText("4.2%");break;
            case 34:ui->lineEdit_rate_2->setText("4.2%");break;case 35:ui->lineEdit_rate_2->setText("4.2%");break;
            }
        }
    }

    return Deadline;

}
void Widget::on_comboBox_rate_2_currentIndexChanged(int index)
{
    if(ui->comboBox_type_2->currentIndex()  == 0)
    {
        if(ui->comboBox_deadline_2->currentIndex() <20)
        {
            switch (index)
            {
            //2020.4.20
            case 0:ui->lineEdit_rate_2->setText("4.65%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate_2->setText("4.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate_2->setText("4.85%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate_2->setText("5.39%");break;
            case 4:ui->lineEdit_rate_2->setText("5.15%");break;case 5:ui->lineEdit_rate_2->setText("4.17%");break;
            case 6:ui->lineEdit_rate_2->setText("3.43%");break;case 7:ui->lineEdit_rate_2->setText("4.9%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate_2->setText("5.67%");break;case 9:ui->lineEdit_rate_2->setText("4.38%");break;
            case 10:ui->lineEdit_rate_2->setText("3.61%");break;case 11:ui->lineEdit_rate_2->setText("5.15%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate_2->setText("5.94%");break;case 13:ui->lineEdit_rate_2->setText("4.59%");break;
            case 14:ui->lineEdit_rate_2->setText("3.78%");break;case 15:ui->lineEdit_rate_2->setText("5.4%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate_2->setText("6.22%");break;case 17:ui->lineEdit_rate_2->setText("4.8%");break;
            case 18:ui->lineEdit_rate_2->setText("3.96%");break;case 19:ui->lineEdit_rate_2->setText("5.65%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate_2->setText("6.49%");break;case 21:ui->lineEdit_rate_2->setText("5.01%");break;
            case 22:ui->lineEdit_rate_2->setText("4.13%");break;case 23:ui->lineEdit_rate_2->setText("5.9%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate_2->setText("6.7%");break;case 25:ui->lineEdit_rate_2->setText("5.2%");break;
            case 26:ui->lineEdit_rate_2->setText("4.3%");break;case 27:ui->lineEdit_rate_2->setText("6.15%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate_2->setText("7.21%");break;case 29:ui->lineEdit_rate_2->setText("5.57%");break;
            case 30:ui->lineEdit_rate_2->setText("4.59%");break;case 31:ui->lineEdit_rate_2->setText("6.55%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate_2->setText("7.48%");break;case 33:ui->lineEdit_rate_2->setText("5.78%");break;
            case 34:ui->lineEdit_rate_2->setText("4.76%");break;case 35:ui->lineEdit_rate_2->setText("6.8%");break;
            }
        }
        else
        {
            switch (index)
            {

            //2020.4.20
            case 0:ui->lineEdit_rate_2->setText("3.85%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate_2->setText("4.05%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate_2->setText("4.2%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate_2->setText("5.23%");break;
            case 4:ui->lineEdit_rate_2->setText("4.99%");break;case 5:ui->lineEdit_rate_2->setText("4.04%");break;
            case 6:ui->lineEdit_rate_2->setText("3.33%");break;case 7:ui->lineEdit_rate_2->setText("4.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate_2->setText("5.5%");break;case 9:ui->lineEdit_rate_2->setText("4.25%");break;
            case 10:ui->lineEdit_rate_2->setText("3.5%");break;case 11:ui->lineEdit_rate_2->setText("5%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate_2->setText("5.78%");break;case 13:ui->lineEdit_rate_2->setText("4.46%");break;
            case 14:ui->lineEdit_rate_2->setText("3.68%");break;case 15:ui->lineEdit_rate_2->setText("5.25%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate_2->setText("6.05%");break;case 17:ui->lineEdit_rate_2->setText("4.68%");break;
            case 18:ui->lineEdit_rate_2->setText("3.85%");break;case 19:ui->lineEdit_rate_2->setText("5.5%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate_2->setText("6.33%");break;case 21:ui->lineEdit_rate_2->setText("4.89%");break;
            case 22:ui->lineEdit_rate_2->setText("4.03%");break;case 23:ui->lineEdit_rate_2->setText("5.75%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate_2->setText("6.6%");break;case 25:ui->lineEdit_rate_2->setText("5.1%");break;
            case 26:ui->lineEdit_rate_2->setText("4.2%");break;case 27:ui->lineEdit_rate_2->setText("6%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate_2->setText("6.77%");break;case 29:ui->lineEdit_rate_2->setText("5.23%");break;
            case 30:ui->lineEdit_rate_2->setText("4.31%");break;case 31:ui->lineEdit_rate_2->setText("6.15%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate_2->setText("7.04%");break;case 33:ui->lineEdit_rate_2->setText("5.44%");break;
            case 34:ui->lineEdit_rate_2->setText("4.48%");break;case 35:ui->lineEdit_rate_2->setText("6.4%");break;
            }
        }

    }
/**************************************切换为 组合型贷款*******************************************************************/
    else if(ui->comboBox_type_2->currentIndex() == 2) ui->stackedWidget->setCurrentWidget(ui->Calater_3);

/**************************************公积金贷款 值改变*******************************************************************/

    else
    {
        if(ui->comboBox_deadline_2->currentIndex() <20)
        {
            switch (index)
            {
            //2020.4.20
            case 0:ui->lineEdit_rate_2->setText("3.25%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate_2->setText("3.25%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate_2->setText("3.25%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate_2->setText("3.57%");break;
            case 4:ui->lineEdit_rate_2->setText("3.41%");break;case 5:ui->lineEdit_rate_2->setText("2.76%");break;
            case 6:ui->lineEdit_rate_2->setText("2.28%");break;case 7:ui->lineEdit_rate_2->setText("3.25%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate_2->setText("3.57%");break;case 9:ui->lineEdit_rate_2->setText("2.76%");break;
            case 10:ui->lineEdit_rate_2->setText("2.28%");break;case 11:ui->lineEdit_rate_2->setText("3.25%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate_2->setText("3.57%");break;case 13:ui->lineEdit_rate_2->setText("2.98%");break;
            case 14:ui->lineEdit_rate_2->setText("2.45%");break;case 15:ui->lineEdit_rate_2->setText("3.5%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate_2->setText("4.13%");break;case 17:ui->lineEdit_rate_2->setText("3.19%");break;
            case 18:ui->lineEdit_rate_2->setText("2.63%");break;case 19:ui->lineEdit_rate_2->setText("3.75%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate_2->setText("4.4%");break;case 21:ui->lineEdit_rate_2->setText("3.4%");break;
            case 22:ui->lineEdit_rate_2->setText("2.8%");break;case 23:ui->lineEdit_rate_2->setText("4%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate_2->setText("4.25%");break;case 25:ui->lineEdit_rate_2->setText("4.25%");break;
            case 26:ui->lineEdit_rate_2->setText("4.25%");break;case 27:ui->lineEdit_rate_2->setText("4.25%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate_2->setText("4.5%");break;case 29:ui->lineEdit_rate_2->setText("4.5%");break;
            case 30:ui->lineEdit_rate_2->setText("4.5%");break;case 31:ui->lineEdit_rate_2->setText("4.5%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate_2->setText("4.7%");break;case 33:ui->lineEdit_rate_2->setText("4.7%");break;
            case 34:ui->lineEdit_rate_2->setText("4.7%");break;case 35:ui->lineEdit_rate_2->setText("4.7%");break;

            }
        }
        else
        {
            switch (index)
            {

            //2020.4.20
            case 0:ui->lineEdit_rate_2->setText("2.75%");break;
            //2020.3.20
            case 1:ui->lineEdit_rate_2->setText("2.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_rate_2->setText("2.75%");break;
            //2015.10.24
            case 3:ui->lineEdit_rate_2->setText("3.03%");break;
            case 4:ui->lineEdit_rate_2->setText("2.89%");break;case 5:ui->lineEdit_rate_2->setText("2.34%");break;
            case 6:ui->lineEdit_rate_2->setText("1.93%");break;case 7:ui->lineEdit_rate_2->setText("2.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_rate_2->setText("3.03%");break;case 9:ui->lineEdit_rate_2->setText("2.34%");break;
            case 10:ui->lineEdit_rate_2->setText("1.93%");break;case 11:ui->lineEdit_rate_2->setText("2.75%");break;
            //2015.6.28
            case 12:ui->lineEdit_rate_2->setText("3.3%");break;case 13:ui->lineEdit_rate_2->setText("2.55%");break;
            case 14:ui->lineEdit_rate_2->setText("2.1%");break;case 15:ui->lineEdit_rate_2->setText("3%");break;
            //2015.5.11
            case 16:ui->lineEdit_rate_2->setText("3.57%");break;case 17:ui->lineEdit_rate_2->setText("2.76%");break;
            case 18:ui->lineEdit_rate_2->setText("2.28%");break;case 19:ui->lineEdit_rate_2->setText("3.25%");break;
            //2015.3.1
            case 20:ui->lineEdit_rate_2->setText("3.85%");break;case 21:ui->lineEdit_rate_2->setText("2.97%");break;
            case 22:ui->lineEdit_rate_2->setText("2.45%");break;case 23:ui->lineEdit_rate_2->setText("3.5%");break;
            //2014.11.22
            case 24:ui->lineEdit_rate_2->setText("3.75%");break;case 25:ui->lineEdit_rate_2->setText("3.75%");break;
            case 26:ui->lineEdit_rate_2->setText("3.75%");break;case 27:ui->lineEdit_rate_2->setText("3.75%");break;
            //2012.7.6
            case 28:ui->lineEdit_rate_2->setText("4%");break;case 29:ui->lineEdit_rate_2->setText("4%");break;
            case 30:ui->lineEdit_rate_2->setText("4%");break;case 31:ui->lineEdit_rate_2->setText("4%");break;
            //2012.6.8
            case 32:ui->lineEdit_rate_2->setText("4.2%");break;case 33:ui->lineEdit_rate_2->setText("4.2%");break;
            case 34:ui->lineEdit_rate_2->setText("4.2%");break;case 35:ui->lineEdit_rate_2->setText("4.2%");break;
            }
        }
    }

}
void Widget::on_pushButton_start_2_clicked()
{
    // 第一个页面处理
    QString checkloan= ui->lineEdit_loan->text();
    QString checkRate = ui->lineEdit_rate_2->text();
    bool Justise = 0;
    double rate = 0;
    //判断是否为空
    if(!checkloan.isEmpty()&&!checkRate.isEmpty())
    {
        int checkPoint = 0;
        bool flag1 = 0,flag3 = 0;
        //判断输入单价
        for(int i= 0 ;i<checkloan.length();i++)
        {
            if((checkloan[i]<='9'&&checkloan[i]>='0')||checkloan[i]=='.')
            {
                if(checkloan[i]=='.')
                    checkPoint++;
                if(checkPoint>1){flag1=1;break;}
            }
            else{flag1 = 1;break;}
        }
        //判断输入利率
        checkPoint = 0;
        int n = 0;
        for(int i= 0 ;i<checkRate.length()-1;i++)
        {
            if((checkRate[i]<='9'&&checkRate[i]>='0')||checkRate[i]=='.')
            {
                QChar te = checkRate.at(i);
                if(checkRate[i]=='.')
                {
                    checkPoint++;
                }
                if(checkPoint>=2){flag3=1;break;}
                if(checkRate[i]!='.')
                {
                    rate = rate*10+(te.toLatin1()-'0');
                    n++;
                }
            }
            else{flag3 = 1;break;}
        }
        rate /= pow(10,n+1);
        if(rate == 0)
            flag3 = 1;

        if(flag1 == 1 && flag3 == 1)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入的房屋贷款总额、利率有误",QMessageBox::Ok);
        }
        else if(flag1 == 1 && flag3 == 0)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋贷款总额有误",QMessageBox::Ok);
        }
        else if(flag1 == 0 && flag3 == 1)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋利率不合法，请重新输入",QMessageBox::Ok);
        }
        else
        {
            Justise = 1;
        }
    }
    //如果有输入有空
    else
    {
        if(checkloan.isEmpty()&&checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋贷款总额、利率",QMessageBox::Ok);
        }
        else if(checkloan.isEmpty()&&!checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入贷款总额",QMessageBox::Ok);
        }
        else if(!checkloan.isEmpty()&&checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋利率",QMessageBox::Ok);
        }
    }

    if(Justise ==1 )
    {
        //房屋贷款总额
        double loan = ui->lineEdit_loan->text().toDouble();
        loan *= 10000;
        ui->lineEdit_Retotalloan->setText(ui->lineEdit_loan->text());

        short Deadline =on_comboBox_deadline_2_currentIndexChanged(ui->comboBox_deadline_2->currentIndex());

//        还款日期下拉框
        ui->comboBox_Redeadline_2->setCurrentIndex(ui->comboBox_deadline_2->currentIndex());

//        利率下拉框
        ui->comboBox_Rerate_2->setCurrentIndex(ui->comboBox_rate_2->currentIndex());
        ui->lineEdit_Rerate_2->setText(QString("%1%").arg(rate*100));

//        贷款类型下拉框
        ui->comboBox_Retype_2->setCurrentIndex(ui->comboBox_type_2->currentIndex());


        house_loan A(loan,rate,Deadline);

        //等额本息
        if(ui->radioButton_benxi_2->isChecked())
        {

            ui->radioButton_Rebenxi_2->clicked();
            ui->label_Remonthpay_change_2->setText("每月月供:");
            ui->widget_Redecline_2->hide();

            A.Average_Capital_Plus_Interest();
            //月供
            ui->label_Remonthpay_2->setText(QString::number(A.Getmonthpayment(), 'f', 1));
            //贷款总额
            ui->label_ReLoan_2->setText(QString::number(A.Getloan(), 'f', 1));
            //支付利息
            ui->label_Reinterest_2->setText(QString::number(A.Getinterest(), 'f', 1));
            //还款总额
            ui->label_Retotalpay_2->setText(QString::number(A.Getendpayment(), 'f', 1));
            //还款月数
            ui->label_Redate_2->setText(QString::number(A.Getdeadline(), 'f', 1));
        }
        //等额本金
        else if(ui->radioButton_benjin_2->isChecked())
        {
            ui->radioButton_Rebenjin_2->clicked();
            ui->widget_Redecline_2->show();
            ui->label_Remonthpay_change_2->setText("首月月供:");
            A.Average_capital();
            qDebug()<<A.Getmonthpayment();
            //月供
            ui->label_Remonthpay_2->setText(QString::number(A.Getmonthpayment(), 'f', 1));
            //每月递减
            ui->label_Redecline_2->setText(QString::number(A.Getmonth_cut(), 'f', 1));
            //贷款总额
            ui->label_ReLoan_2->setText(QString::number(A.Getloan(), 'f', 1));
            //支付利息
            ui->label_Reinterest_2->setText(QString::number(A.Getinterest(), 'f', 1));
            //还款总额
            ui->label_Retotalpay_2->setText(QString::number(A.Getendpayment(), 'f', 1));
            //还款月数
            ui->label_Redate_2->setText(QString::number(A.Getdeadline(), 'f', 1));
        }
        ui->stackedWidget->setCurrentWidget(ui->Calater_2_Result);
    }
}
void Widget::on_pushButton_cance_2_clicked()
{

        int ret = QMessageBox::question(this,"Question","是否需要清除数据？");
        if(ret == QMessageBox::Yes)
        {
            //关闭窗口;
            ui->lineEdit_loan->clear();
            ui->lineEdit_rate_2->setText("4.65%");
            ui->comboBox_deadline_2->setCurrentText(0);
            ui->comboBox_rate_2->setCurrentText(0);
            ui->radioButton_benxi_2->clicked();
            if(ui->comboBox_type_2->currentIndex() == 0)
                ui->lineEdit_rate_2->setText("4.65%");
            else
            {
                ui->lineEdit_rate_2->setText("3.25%");
            }

        }
        else if(ret == QMessageBox::No)
        {
            //保持不动
            //忽略事件，事件继续给父组件传递
        }

}
/******************************************-----End_Calater_2 -----*************************************************************/


/******************************************-----Begin_Result_NO.2 贷款总额-----**************************************************/
void Widget::on_comboBox_Retype_2_currentIndexChanged(int index)
{
    if(index == 0)
    {
        if(ui->comboBox_Redeadline_2->currentIndex() <20)
        {
            switch (ui->comboBox_Rerate_2->currentIndex())
            {
            //2020.4.20
            case 0:ui->lineEdit_Rerate_2->setText("4.65%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_2->setText("4.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_2->setText("4.85%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_2->setText("5.39%");break;
            case 4:ui->lineEdit_Rerate_2->setText("5.15%");break;case 5:ui->lineEdit_Rerate_2->setText("4.17%");break;
            case 6:ui->lineEdit_Rerate_2->setText("3.43%");break;case 7:ui->lineEdit_Rerate_2->setText("4.9%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_2->setText("5.67%");break;case 9:ui->lineEdit_Rerate_2->setText("4.38%");break;
            case 10:ui->lineEdit_Rerate_2->setText("3.61%");break;case 11:ui->lineEdit_Rerate_2->setText("5.15%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_2->setText("5.94%");break;case 13:ui->lineEdit_Rerate_2->setText("4.59%");break;
            case 14:ui->lineEdit_Rerate_2->setText("3.78%");break;case 15:ui->lineEdit_Rerate_2->setText("5.4%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_2->setText("6.22%");break;case 17:ui->lineEdit_Rerate_2->setText("4.8%");break;
            case 18:ui->lineEdit_Rerate_2->setText("3.96%");break;case 19:ui->lineEdit_Rerate_2->setText("5.65%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_2->setText("6.49%");break;case 21:ui->lineEdit_Rerate_2->setText("5.01%");break;
            case 22:ui->lineEdit_Rerate_2->setText("4.13%");break;case 23:ui->lineEdit_Rerate_2->setText("5.9%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_2->setText("6.7%");break;case 25:ui->lineEdit_Rerate_2->setText("5.2%");break;
            case 26:ui->lineEdit_Rerate_2->setText("4.3%");break;case 27:ui->lineEdit_Rerate_2->setText("6.15%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_2->setText("7.21%");break;case 29:ui->lineEdit_Rerate_2->setText("5.57%");break;
            case 30:ui->lineEdit_Rerate_2->setText("4.59%");break;case 31:ui->lineEdit_Rerate_2->setText("6.55%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_2->setText("7.48%");break;case 33:ui->lineEdit_Rerate_2->setText("5.78%");break;
            case 34:ui->lineEdit_Rerate_2->setText("4.76%");break;case 35:ui->lineEdit_Rerate_2->setText("6.8%");break;
            }
        }
        else
        {
            switch (ui->comboBox_Rerate_2->currentIndex())
            {

            //2020.4.20
            case 0:ui->lineEdit_Rerate_2->setText("3.85%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_2->setText("4.05%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_2->setText("4.2%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_2->setText("5.23%");break;
            case 4:ui->lineEdit_Rerate_2->setText("4.99%");break;case 5:ui->lineEdit_Rerate_2->setText("4.04%");break;
            case 6:ui->lineEdit_Rerate_2->setText("3.33%");break;case 7:ui->lineEdit_Rerate_2->setText("4.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_2->setText("5.5%");break;case 9:ui->lineEdit_Rerate_2->setText("4.25%");break;
            case 10:ui->lineEdit_Rerate_2->setText("3.5%");break;case 11:ui->lineEdit_Rerate_2->setText("5%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_2->setText("5.78%");break;case 13:ui->lineEdit_Rerate_2->setText("4.46%");break;
            case 14:ui->lineEdit_Rerate_2->setText("3.68%");break;case 15:ui->lineEdit_Rerate_2->setText("5.25%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_2->setText("6.05%");break;case 17:ui->lineEdit_Rerate_2->setText("4.68%");break;
            case 18:ui->lineEdit_Rerate_2->setText("3.85%");break;case 19:ui->lineEdit_Rerate_2->setText("5.5%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_2->setText("6.33%");break;case 21:ui->lineEdit_Rerate_2->setText("4.89%");break;
            case 22:ui->lineEdit_Rerate_2->setText("4.03%");break;case 23:ui->lineEdit_Rerate_2->setText("5.75%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_2->setText("6.6%");break;case 25:ui->lineEdit_Rerate_2->setText("5.1%");break;
            case 26:ui->lineEdit_Rerate_2->setText("4.2%");break;case 27:ui->lineEdit_Rerate_2->setText("6%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_2->setText("6.77%");break;case 29:ui->lineEdit_Rerate_2->setText("5.23%");break;
            case 30:ui->lineEdit_Rerate_2->setText("4.31%");break;case 31:ui->lineEdit_Rerate_2->setText("6.15%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_2->setText("7.04%");break;case 33:ui->lineEdit_Rerate_2->setText("5.44%");break;
            case 34:ui->lineEdit_Rerate_2->setText("4.48%");break;case 35:ui->lineEdit_Rerate_2->setText("6.4%");break;
            }
        }

    }
/**************************************切换为 组合型贷款*******************************************************************/
    else if(index == 2)
    {
        ui->stackedWidget->setCurrentWidget(ui->Calater_3);
        ui->comboBox_type_3->setCurrentIndex(0);
    }


/**************************************公积金贷款 值改变*******************************************************************/

    else
    {
        if(ui->comboBox_Redeadline_2->currentIndex() <20)
        {
            switch (ui->comboBox_Rerate_2->currentIndex())
            {
            //2020.4.20
            case 0:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_2->setText("3.57%");break;
            case 4:ui->lineEdit_Rerate_2->setText("3.41%");break;case 5:ui->lineEdit_Rerate_2->setText("2.76%");break;
            case 6:ui->lineEdit_Rerate_2->setText("2.28%");break;case 7:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_2->setText("3.57%");break;case 9:ui->lineEdit_Rerate_2->setText("2.76%");break;
            case 10:ui->lineEdit_Rerate_2->setText("2.28%");break;case 11:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_2->setText("3.57%");break;case 13:ui->lineEdit_Rerate_2->setText("2.98%");break;
            case 14:ui->lineEdit_Rerate_2->setText("2.45%");break;case 15:ui->lineEdit_Rerate_2->setText("3.5%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_2->setText("4.13%");break;case 17:ui->lineEdit_Rerate_2->setText("3.19%");break;
            case 18:ui->lineEdit_Rerate_2->setText("2.63%");break;case 19:ui->lineEdit_Rerate_2->setText("3.75%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_2->setText("4.4%");break;case 21:ui->lineEdit_Rerate_2->setText("3.4%");break;
            case 22:ui->lineEdit_Rerate_2->setText("2.8%");break;case 23:ui->lineEdit_Rerate_2->setText("4%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_2->setText("4.25%");break;case 25:ui->lineEdit_Rerate_2->setText("4.25%");break;
            case 26:ui->lineEdit_Rerate_2->setText("4.25%");break;case 27:ui->lineEdit_Rerate_2->setText("4.25%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_2->setText("4.5%");break;case 29:ui->lineEdit_Rerate_2->setText("4.5%");break;
            case 30:ui->lineEdit_Rerate_2->setText("4.5%");break;case 31:ui->lineEdit_Rerate_2->setText("4.5%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_2->setText("4.7%");break;case 33:ui->lineEdit_Rerate_2->setText("4.7%");break;
            case 34:ui->lineEdit_Rerate_2->setText("4.7%");break;case 35:ui->lineEdit_Rerate_2->setText("4.7%");break;

            }
        }
        else
        {
            switch (ui->comboBox_Rerate_2->currentIndex())
            {

            //2020.4.20
            case 0:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_2->setText("3.03%");break;
            case 4:ui->lineEdit_Rerate_2->setText("2.89%");break;case 5:ui->lineEdit_Rerate_2->setText("2.34%");break;
            case 6:ui->lineEdit_Rerate_2->setText("1.93%");break;case 7:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_2->setText("3.03%");break;case 9:ui->lineEdit_Rerate_2->setText("2.34%");break;
            case 10:ui->lineEdit_Rerate_2->setText("1.93%");break;case 11:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_2->setText("3.3%");break;case 13:ui->lineEdit_Rerate_2->setText("2.55%");break;
            case 14:ui->lineEdit_Rerate_2->setText("2.1%");break;case 15:ui->lineEdit_Rerate_2->setText("3%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_2->setText("3.57%");break;case 17:ui->lineEdit_Rerate_2->setText("2.76%");break;
            case 18:ui->lineEdit_Rerate_2->setText("2.28%");break;case 19:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_2->setText("3.85%");break;case 21:ui->lineEdit_Rerate_2->setText("2.97%");break;
            case 22:ui->lineEdit_Rerate_2->setText("2.45%");break;case 23:ui->lineEdit_Rerate_2->setText("3.5%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_2->setText("3.75%");break;case 25:ui->lineEdit_Rerate_2->setText("3.75%");break;
            case 26:ui->lineEdit_Rerate_2->setText("3.75%");break;case 27:ui->lineEdit_Rerate_2->setText("3.75%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_2->setText("4%");break;case 29:ui->lineEdit_Rerate_2->setText("4%");break;
            case 30:ui->lineEdit_Rerate_2->setText("4%");break;case 31:ui->lineEdit_Rerate_2->setText("4%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_2->setText("4.2%");break;case 33:ui->lineEdit_Rerate_2->setText("4.2%");break;
            case 34:ui->lineEdit_Rerate_2->setText("4.2%");break;case 35:ui->lineEdit_Rerate_2->setText("4.2%");break;
            }
        }
    }
}
void Widget::on_comboBox_RecalaterWay_2_currentIndexChanged(int index)
{
    if(index == 1)
    {
        ui->stackedWidget->setCurrentWidget(ui->Calater_1);
        ui->comboBox_calaterWay->setCurrentIndex(0);
        switch (ui->comboBox_Retype_2->currentIndex())
        {
             case 0:ui->comboBox_type->setCurrentIndex(0);break;
             case 1:ui->comboBox_type->setCurrentIndex(1);break;
             default:
                 QMessageBox *eorr = new QMessageBox(this);
                 eorr->warning(this,"错误","你的选择有问题！",QMessageBox::Ok);
             break;
        }
    }
}
short Widget::on_comboBox_Redeadline_2_currentIndexChanged(int index)
{
    short Deadline = 0;
    switch (index)
    {
    case 0:Deadline = 300;break;case 1:Deadline = 288;break;
    case 2:Deadline = 276;break;case 3:Deadline = 264;break;
    case 4:Deadline = 252;break;case 5:Deadline = 240;break;
    case 6:Deadline = 228;break;case 7:Deadline = 216;break;
    case 8:Deadline = 204;break;case 9:Deadline = 192;break;
    case 10:Deadline = 180;break;case 11:Deadline = 168;break;
    case 12:Deadline = 156;break;case 13:Deadline = 144;break;
    case 14:Deadline = 132;break;case 15:Deadline = 120;break;
    case 16:Deadline = 108;break;case 17:Deadline = 96;break;
    case 18:Deadline = 84;break;case 19:Deadline = 72;break;
    case 20:Deadline = 60;break;case 21:Deadline = 48;break;
    case 22:Deadline = 36;break;case 23:Deadline = 24;break;
    case 24:Deadline = 12;break;
    }

    if(ui->comboBox_Retype_2->currentIndex() == 0)
    {
        if(index <20)
        {
            switch (ui->comboBox_Rerate_2->currentIndex())
            {
            //2020.4.20
            case 0:ui->lineEdit_Rerate_2->setText("4.65%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_2->setText("4.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_2->setText("4.85%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_2->setText("5.39%");break;
            case 4:ui->lineEdit_Rerate_2->setText("5.15%");break;case 5:ui->lineEdit_Rerate_2->setText("4.17%");break;
            case 6:ui->lineEdit_Rerate_2->setText("3.43%");break;case 7:ui->lineEdit_Rerate_2->setText("4.9%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_2->setText("5.67%");break;case 9:ui->lineEdit_Rerate_2->setText("4.38%");break;
            case 10:ui->lineEdit_Rerate_2->setText("3.61%");break;case 11:ui->lineEdit_Rerate_2->setText("5.15%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_2->setText("5.94%");break;case 13:ui->lineEdit_Rerate_2->setText("4.59%");break;
            case 14:ui->lineEdit_Rerate_2->setText("3.78%");break;case 15:ui->lineEdit_Rerate_2->setText("5.4%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_2->setText("6.22%");break;case 17:ui->lineEdit_Rerate_2->setText("4.8%");break;
            case 18:ui->lineEdit_Rerate_2->setText("3.96%");break;case 19:ui->lineEdit_Rerate_2->setText("5.65%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_2->setText("6.49%");break;case 21:ui->lineEdit_Rerate_2->setText("5.01%");break;
            case 22:ui->lineEdit_Rerate_2->setText("4.13%");break;case 23:ui->lineEdit_Rerate_2->setText("5.9%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_2->setText("6.7%");break;case 25:ui->lineEdit_Rerate_2->setText("5.2%");break;
            case 26:ui->lineEdit_Rerate_2->setText("4.3%");break;case 27:ui->lineEdit_Rerate_2->setText("6.15%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_2->setText("7.21%");break;case 29:ui->lineEdit_Rerate_2->setText("5.57%");break;
            case 30:ui->lineEdit_Rerate_2->setText("4.59%");break;case 31:ui->lineEdit_Rerate_2->setText("6.55%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_2->setText("7.48%");break;case 33:ui->lineEdit_Rerate_2->setText("5.78%");break;
            case 34:ui->lineEdit_Rerate_2->setText("4.76%");break;case 35:ui->lineEdit_Rerate_2->setText("6.8%");break;
            }
        }
        else
        {
            switch (ui->comboBox_Rerate_2->currentIndex())
            {

            //2020.4.20
            case 0:ui->lineEdit_Rerate_2->setText("3.85%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_2->setText("4.05%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_2->setText("4.2%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_2->setText("5.23%");break;
            case 4:ui->lineEdit_Rerate_2->setText("4.99%");break;case 5:ui->lineEdit_Rerate_2->setText("4.04%");break;
            case 6:ui->lineEdit_Rerate_2->setText("3.33%");break;case 7:ui->lineEdit_Rerate_2->setText("4.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_2->setText("5.5%");break;case 9:ui->lineEdit_Rerate_2->setText("4.25%");break;
            case 10:ui->lineEdit_Rerate_2->setText("3.5%");break;case 11:ui->lineEdit_Rerate_2->setText("5%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_2->setText("5.78%");break;case 13:ui->lineEdit_Rerate_2->setText("4.46%");break;
            case 14:ui->lineEdit_Rerate_2->setText("3.68%");break;case 15:ui->lineEdit_Rerate_2->setText("5.25%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_2->setText("6.05%");break;case 17:ui->lineEdit_Rerate_2->setText("4.68%");break;
            case 18:ui->lineEdit_Rerate_2->setText("3.85%");break;case 19:ui->lineEdit_Rerate_2->setText("5.5%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_2->setText("6.33%");break;case 21:ui->lineEdit_Rerate_2->setText("4.89%");break;
            case 22:ui->lineEdit_Rerate_2->setText("4.03%");break;case 23:ui->lineEdit_Rerate_2->setText("5.75%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_2->setText("6.6%");break;case 25:ui->lineEdit_Rerate_2->setText("5.1%");break;
            case 26:ui->lineEdit_Rerate_2->setText("4.2%");break;case 27:ui->lineEdit_Rerate_2->setText("6%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_2->setText("6.77%");break;case 29:ui->lineEdit_Rerate_2->setText("5.23%");break;
            case 30:ui->lineEdit_Rerate_2->setText("4.31%");break;case 31:ui->lineEdit_Rerate_2->setText("6.15%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_2->setText("7.04%");break;case 33:ui->lineEdit_Rerate_2->setText("5.44%");break;
            case 34:ui->lineEdit_Rerate_2->setText("4.48%");break;case 35:ui->lineEdit_Rerate_2->setText("6.4%");break;
            }
        }

    }
/**************************************切换为 组合型贷款*******************************************************************/
    else if(ui->comboBox_Retype_2->currentIndex()  == 2) ui->stackedWidget->setCurrentWidget(ui->Calater_3);

/**************************************公积金贷款 值改变*******************************************************************/

    else
    {
        if(index <20)
        {
            switch (ui->comboBox_Rerate_2->currentIndex())
            {
            //2020.4.20
            case 0:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_2->setText("3.57%");break;
            case 4:ui->lineEdit_Rerate_2->setText("3.41%");break;case 5:ui->lineEdit_Rerate_2->setText("2.76%");break;
            case 6:ui->lineEdit_Rerate_2->setText("2.28%");break;case 7:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_2->setText("3.57%");break;case 9:ui->lineEdit_Rerate_2->setText("2.76%");break;
            case 10:ui->lineEdit_Rerate_2->setText("2.28%");break;case 11:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_2->setText("3.57%");break;case 13:ui->lineEdit_Rerate_2->setText("2.98%");break;
            case 14:ui->lineEdit_Rerate_2->setText("2.45%");break;case 15:ui->lineEdit_Rerate_2->setText("3.5%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_2->setText("4.13%");break;case 17:ui->lineEdit_Rerate_2->setText("3.19%");break;
            case 18:ui->lineEdit_Rerate_2->setText("2.63%");break;case 19:ui->lineEdit_Rerate_2->setText("3.75%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_2->setText("4.4%");break;case 21:ui->lineEdit_Rerate_2->setText("3.4%");break;
            case 22:ui->lineEdit_Rerate_2->setText("2.8%");break;case 23:ui->lineEdit_Rerate_2->setText("4%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_2->setText("4.25%");break;case 25:ui->lineEdit_Rerate_2->setText("4.25%");break;
            case 26:ui->lineEdit_Rerate_2->setText("4.25%");break;case 27:ui->lineEdit_Rerate_2->setText("4.25%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_2->setText("4.5%");break;case 29:ui->lineEdit_Rerate_2->setText("4.5%");break;
            case 30:ui->lineEdit_Rerate_2->setText("4.5%");break;case 31:ui->lineEdit_Rerate_2->setText("4.5%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_2->setText("4.7%");break;case 33:ui->lineEdit_Rerate_2->setText("4.7%");break;
            case 34:ui->lineEdit_Rerate_2->setText("4.7%");break;case 35:ui->lineEdit_Rerate_2->setText("4.7%");break;

            }
        }
        else
        {
            switch (ui->comboBox_Rerate_2->currentIndex())
            {

            //2020.4.20
            case 0:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_2->setText("3.03%");break;
            case 4:ui->lineEdit_Rerate_2->setText("2.89%");break;case 5:ui->lineEdit_Rerate_2->setText("2.34%");break;
            case 6:ui->lineEdit_Rerate_2->setText("1.93%");break;case 7:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_2->setText("3.03%");break;case 9:ui->lineEdit_Rerate_2->setText("2.34%");break;
            case 10:ui->lineEdit_Rerate_2->setText("1.93%");break;case 11:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_2->setText("3.3%");break;case 13:ui->lineEdit_Rerate_2->setText("2.55%");break;
            case 14:ui->lineEdit_Rerate_2->setText("2.1%");break;case 15:ui->lineEdit_Rerate_2->setText("3%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_2->setText("3.57%");break;case 17:ui->lineEdit_Rerate_2->setText("2.76%");break;
            case 18:ui->lineEdit_Rerate_2->setText("2.28%");break;case 19:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_2->setText("3.85%");break;case 21:ui->lineEdit_Rerate_2->setText("2.97%");break;
            case 22:ui->lineEdit_Rerate_2->setText("2.45%");break;case 23:ui->lineEdit_Rerate_2->setText("3.5%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_2->setText("3.75%");break;case 25:ui->lineEdit_Rerate_2->setText("3.75%");break;
            case 26:ui->lineEdit_Rerate_2->setText("3.75%");break;case 27:ui->lineEdit_Rerate_2->setText("3.75%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_2->setText("4%");break;case 29:ui->lineEdit_Rerate_2->setText("4%");break;
            case 30:ui->lineEdit_Rerate_2->setText("4%");break;case 31:ui->lineEdit_Rerate_2->setText("4%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_2->setText("4.2%");break;case 33:ui->lineEdit_Rerate_2->setText("4.2%");break;
            case 34:ui->lineEdit_Rerate_2->setText("4.2%");break;case 35:ui->lineEdit_Rerate_2->setText("4.2%");break;
            }
        }
    }
    return Deadline;
}
void Widget::on_comboBox_Rerate_2_currentIndexChanged(int index)
{
    if(ui->comboBox_Retype_2->currentIndex() == 0)
    {
        if(ui->comboBox_Redeadline_2->currentIndex() <20)
        {
            switch (index)
            {
            //2020.4.20
            case 0:ui->lineEdit_Rerate_2->setText("4.65%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_2->setText("4.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_2->setText("4.85%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_2->setText("5.39%");break;
            case 4:ui->lineEdit_Rerate_2->setText("5.15%");break;case 5:ui->lineEdit_Rerate_2->setText("4.17%");break;
            case 6:ui->lineEdit_Rerate_2->setText("3.43%");break;case 7:ui->lineEdit_Rerate_2->setText("4.9%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_2->setText("5.67%");break;case 9:ui->lineEdit_Rerate_2->setText("4.38%");break;
            case 10:ui->lineEdit_Rerate_2->setText("3.61%");break;case 11:ui->lineEdit_Rerate_2->setText("5.15%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_2->setText("5.94%");break;case 13:ui->lineEdit_Rerate_2->setText("4.59%");break;
            case 14:ui->lineEdit_Rerate_2->setText("3.78%");break;case 15:ui->lineEdit_Rerate_2->setText("5.4%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_2->setText("6.22%");break;case 17:ui->lineEdit_Rerate_2->setText("4.8%");break;
            case 18:ui->lineEdit_Rerate_2->setText("3.96%");break;case 19:ui->lineEdit_Rerate_2->setText("5.65%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_2->setText("6.49%");break;case 21:ui->lineEdit_Rerate_2->setText("5.01%");break;
            case 22:ui->lineEdit_Rerate_2->setText("4.13%");break;case 23:ui->lineEdit_Rerate_2->setText("5.9%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_2->setText("6.7%");break;case 25:ui->lineEdit_Rerate_2->setText("5.2%");break;
            case 26:ui->lineEdit_Rerate_2->setText("4.3%");break;case 27:ui->lineEdit_Rerate_2->setText("6.15%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_2->setText("7.21%");break;case 29:ui->lineEdit_Rerate_2->setText("5.57%");break;
            case 30:ui->lineEdit_Rerate_2->setText("4.59%");break;case 31:ui->lineEdit_Rerate_2->setText("6.55%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_2->setText("7.48%");break;case 33:ui->lineEdit_Rerate_2->setText("5.78%");break;
            case 34:ui->lineEdit_Rerate_2->setText("4.76%");break;case 35:ui->lineEdit_Rerate_2->setText("6.8%");break;
            }
        }
        else
        {
            switch (index)
            {

            //2020.4.20
            case 0:ui->lineEdit_Rerate_2->setText("3.85%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_2->setText("4.05%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_2->setText("4.2%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_2->setText("5.23%");break;
            case 4:ui->lineEdit_Rerate_2->setText("4.99%");break;case 5:ui->lineEdit_Rerate_2->setText("4.04%");break;
            case 6:ui->lineEdit_Rerate_2->setText("3.33%");break;case 7:ui->lineEdit_Rerate_2->setText("4.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_2->setText("5.5%");break;case 9:ui->lineEdit_Rerate_2->setText("4.25%");break;
            case 10:ui->lineEdit_Rerate_2->setText("3.5%");break;case 11:ui->lineEdit_Rerate_2->setText("5%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_2->setText("5.78%");break;case 13:ui->lineEdit_Rerate_2->setText("4.46%");break;
            case 14:ui->lineEdit_Rerate_2->setText("3.68%");break;case 15:ui->lineEdit_Rerate_2->setText("5.25%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_2->setText("6.05%");break;case 17:ui->lineEdit_Rerate_2->setText("4.68%");break;
            case 18:ui->lineEdit_Rerate_2->setText("3.85%");break;case 19:ui->lineEdit_Rerate_2->setText("5.5%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_2->setText("6.33%");break;case 21:ui->lineEdit_Rerate_2->setText("4.89%");break;
            case 22:ui->lineEdit_Rerate_2->setText("4.03%");break;case 23:ui->lineEdit_Rerate_2->setText("5.75%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_2->setText("6.6%");break;case 25:ui->lineEdit_Rerate_2->setText("5.1%");break;
            case 26:ui->lineEdit_Rerate_2->setText("4.2%");break;case 27:ui->lineEdit_Rerate_2->setText("6%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_2->setText("6.77%");break;case 29:ui->lineEdit_Rerate_2->setText("5.23%");break;
            case 30:ui->lineEdit_Rerate_2->setText("4.31%");break;case 31:ui->lineEdit_Rerate_2->setText("6.15%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_2->setText("7.04%");break;case 33:ui->lineEdit_Rerate_2->setText("5.44%");break;
            case 34:ui->lineEdit_Rerate_2->setText("4.48%");break;case 35:ui->lineEdit_Rerate_2->setText("6.4%");break;
            }
        }

    }
/**************************************切换为 组合型贷款*******************************************************************/
    else if(ui->comboBox_Retype_2->currentIndex()  == 2) ui->stackedWidget->setCurrentWidget(ui->Calater_3);

/**************************************公积金贷款 值改变*******************************************************************/

    else
    {
        if(ui->comboBox_Redeadline_2->currentIndex() <20)
        {
            switch (index)
            {
            //2020.4.20
            case 0:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_2->setText("3.57%");break;
            case 4:ui->lineEdit_Rerate_2->setText("3.41%");break;case 5:ui->lineEdit_Rerate_2->setText("2.76%");break;
            case 6:ui->lineEdit_Rerate_2->setText("2.28%");break;case 7:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_2->setText("3.57%");break;case 9:ui->lineEdit_Rerate_2->setText("2.76%");break;
            case 10:ui->lineEdit_Rerate_2->setText("2.28%");break;case 11:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_2->setText("3.57%");break;case 13:ui->lineEdit_Rerate_2->setText("2.98%");break;
            case 14:ui->lineEdit_Rerate_2->setText("2.45%");break;case 15:ui->lineEdit_Rerate_2->setText("3.5%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_2->setText("4.13%");break;case 17:ui->lineEdit_Rerate_2->setText("3.19%");break;
            case 18:ui->lineEdit_Rerate_2->setText("2.63%");break;case 19:ui->lineEdit_Rerate_2->setText("3.75%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_2->setText("4.4%");break;case 21:ui->lineEdit_Rerate_2->setText("3.4%");break;
            case 22:ui->lineEdit_Rerate_2->setText("2.8%");break;case 23:ui->lineEdit_Rerate_2->setText("4%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_2->setText("4.25%");break;case 25:ui->lineEdit_Rerate_2->setText("4.25%");break;
            case 26:ui->lineEdit_Rerate_2->setText("4.25%");break;case 27:ui->lineEdit_Rerate_2->setText("4.25%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_2->setText("4.5%");break;case 29:ui->lineEdit_Rerate_2->setText("4.5%");break;
            case 30:ui->lineEdit_Rerate_2->setText("4.5%");break;case 31:ui->lineEdit_Rerate_2->setText("4.5%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_2->setText("4.7%");break;case 33:ui->lineEdit_Rerate_2->setText("4.7%");break;
            case 34:ui->lineEdit_Rerate_2->setText("4.7%");break;case 35:ui->lineEdit_Rerate_2->setText("4.7%");break;

            }
        }
        else
        {
            switch (index)
            {

            //2020.4.20
            case 0:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2020.3.20
            case 1:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2019.9.20
            case 2:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2015.10.24
            case 3:ui->lineEdit_Rerate_2->setText("3.03%");break;
            case 4:ui->lineEdit_Rerate_2->setText("2.89%");break;case 5:ui->lineEdit_Rerate_2->setText("2.34%");break;
            case 6:ui->lineEdit_Rerate_2->setText("1.93%");break;case 7:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2015.8.26
            case 8:ui->lineEdit_Rerate_2->setText("3.03%");break;case 9:ui->lineEdit_Rerate_2->setText("2.34%");break;
            case 10:ui->lineEdit_Rerate_2->setText("1.93%");break;case 11:ui->lineEdit_Rerate_2->setText("2.75%");break;
            //2015.6.28
            case 12:ui->lineEdit_Rerate_2->setText("3.3%");break;case 13:ui->lineEdit_Rerate_2->setText("2.55%");break;
            case 14:ui->lineEdit_Rerate_2->setText("2.1%");break;case 15:ui->lineEdit_Rerate_2->setText("3%");break;
            //2015.5.11
            case 16:ui->lineEdit_Rerate_2->setText("3.57%");break;case 17:ui->lineEdit_Rerate_2->setText("2.76%");break;
            case 18:ui->lineEdit_Rerate_2->setText("2.28%");break;case 19:ui->lineEdit_Rerate_2->setText("3.25%");break;
            //2015.3.1
            case 20:ui->lineEdit_Rerate_2->setText("3.85%");break;case 21:ui->lineEdit_Rerate_2->setText("2.97%");break;
            case 22:ui->lineEdit_Rerate_2->setText("2.45%");break;case 23:ui->lineEdit_Rerate_2->setText("3.5%");break;
            //2014.11.22
            case 24:ui->lineEdit_Rerate_2->setText("3.75%");break;case 25:ui->lineEdit_Rerate_2->setText("3.75%");break;
            case 26:ui->lineEdit_Rerate_2->setText("3.75%");break;case 27:ui->lineEdit_Rerate_2->setText("3.75%");break;
            //2012.7.6
            case 28:ui->lineEdit_Rerate_2->setText("4%");break;case 29:ui->lineEdit_Rerate_2->setText("4%");break;
            case 30:ui->lineEdit_Rerate_2->setText("4%");break;case 31:ui->lineEdit_Rerate_2->setText("4%");break;
            //2012.6.8
            case 32:ui->lineEdit_Rerate_2->setText("4.2%");break;case 33:ui->lineEdit_Rerate_2->setText("4.2%");break;
            case 34:ui->lineEdit_Rerate_2->setText("4.2%");break;case 35:ui->lineEdit_Rerate_2->setText("4.2%");break;
            }
        }
    }
}
void Widget::on_pushButton_start_4_clicked()
{
    // 第一个页面处理
    QString checkloan= ui->lineEdit_Retotalloan->text();
    QString checkRate = ui->lineEdit_Rerate_2->text();
    bool Justise = 0;
    double rate = 0;
    //判断是否为空
    if(!checkloan.isEmpty()&&!checkRate.isEmpty())
    {
        int checkPoint = 0;
        bool flag1 = 0,flag3 = 0;
        //判断输入单价
        for(int i= 0 ;i<checkloan.length();i++)
        {
            if((checkloan[i]<='9'&&checkloan[i]>='0')||checkloan[i]=='.')
            {
                if(checkloan[i]=='.')
                    checkPoint++;
                if(checkPoint>1){flag1=1;break;}
            }
            else{flag1 = 1;break;}
        }
        //判断输入利率
        checkPoint = 0;
        int n = 0;
        for(int i= 0 ;i<checkRate.length()-1;i++)
        {
            if((checkRate[i]<='9'&&checkRate[i]>='0')||checkRate[i]=='.')
            {
                QChar te = checkRate.at(i);
                if(checkRate[i]=='.')
                {
                    checkPoint++;
                }
                if(checkPoint>=2){flag3=1;break;}
                if(checkRate[i]!='.')
                {
                    rate = rate*10+(te.toLatin1()-'0');
                    n++;
                }
            }
            else{flag3 = 1;break;}
        }
        rate /= pow(10,n+1);
        if(rate == 0)
            flag3 = 1;

        if(flag1 == 1 && flag3 == 1)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入的房屋贷款总额、利率有误",QMessageBox::Ok);
        }
        else if(flag1 == 1 && flag3 == 0)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋贷款总额有误",QMessageBox::Ok);
        }
        else if(flag1 == 0 && flag3 == 1)
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您的输入房屋利率不合法，请重新输入",QMessageBox::Ok);
        }
        else
        {
            Justise = 1;
        }
    }
    //如果有输入有空
    else
    {
        if(checkloan.isEmpty()&&checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋贷款总额、利率",QMessageBox::Ok);
        }
        else if(checkloan.isEmpty()&&!checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入贷款总额",QMessageBox::Ok);
        }
        else if(!checkloan.isEmpty()&&checkRate.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入房屋利率",QMessageBox::Ok);
        }
    }

    if(Justise ==1 )
    {
        //房屋贷款总额
        double loan = ui->lineEdit_Retotalloan->text().toDouble();
        loan *= 10000;
        ui->lineEdit_Retotalloan->setText(ui->lineEdit_Retotalloan->text());

        short Deadline =on_comboBox_deadline_2_currentIndexChanged(ui->comboBox_Redeadline_2->currentIndex());

//        还款日期下拉框
        ui->comboBox_Redeadline_2->setCurrentIndex(ui->comboBox_Redeadline_2->currentIndex());

//        利率下拉框
        ui->comboBox_Rerate_2->setCurrentIndex(ui->comboBox_Rerate_2->currentIndex());
        ui->lineEdit_Rerate_2->setText(QString("%1%").arg(rate*100));

//        贷款类型下拉框
        ui->comboBox_Retype_2->setCurrentIndex(ui->comboBox_Retype_2->currentIndex());


        house_loan A(loan,rate,Deadline);

        //等额本息
        if(ui->radioButton_Rebenxi_2->isChecked())
        {

            ui->radioButton_Rebenxi_2->clicked();
            ui->label_Remonthpay_change_2->setText("每月月供:");
            ui->widget_Redecline_2->hide();

            A.Average_Capital_Plus_Interest();
            //月供
            ui->label_Remonthpay_2->setText(QString::number(A.Getmonthpayment(), 'f', 1));
            //贷款总额
            ui->label_ReLoan_2->setText(QString::number(A.Getloan(), 'f', 1));
            //支付利息
            ui->label_Reinterest_2->setText(QString::number(A.Getinterest(), 'f', 1));
            //还款总额
            ui->label_Retotalpay_2->setText(QString::number(A.Getendpayment(), 'f', 1));
            //还款月数
            ui->label_Redate_2->setText(QString::number(A.Getdeadline(), 'f', 1));
        }
        //等额本金
        else if(ui->radioButton_Rebenjin_2->isChecked())
        {
            ui->radioButton_Rebenjin_2->clicked();
            ui->widget_Redecline_2->show();
            ui->label_Remonthpay_change_2->setText("首月月供:");
            A.Average_capital();
            qDebug()<<A.Getmonthpayment();
            //月供
            ui->label_Remonthpay_2->setText(QString::number(A.Getmonthpayment(), 'f', 1));
            //每月递减
            ui->label_Redecline_2->setText(QString::number(A.Getmonth_cut(), 'f', 1));
            //贷款总额
            ui->label_ReLoan_2->setText(QString::number(A.Getloan(), 'f', 1));
            //支付利息
            ui->label_Reinterest_2->setText(QString::number(A.Getinterest(), 'f', 1));
            //还款总额
            ui->label_Retotalpay_2->setText(QString::number(A.Getendpayment(), 'f', 1));
            //还款月数
            ui->label_Redate_2->setText(QString::number(A.Getdeadline(), 'f', 1));
        }
        ui->stackedWidget->setCurrentWidget(ui->Calater_2_Result);
    }

}
void Widget::on_pushButton_cance_4_clicked()
{
    int ret = QMessageBox::question(this,"Question","是否需要清除数据？");
    if(ret == QMessageBox::Yes)
    {
        ui->stackedWidget->setCurrentWidget(ui->Calater_2);
        ui->lineEdit_Retotalloan->clear();
        ui->lineEdit_rate_2->setText("4.65%");
        ui->comboBox_deadline_2->setCurrentText(0);
        ui->comboBox_rate_2->setCurrentText(0);
        ui->radioButton_benxi_2->clicked();
        ui->lineEdit_loan->clear();
        ui->lineEdit_rate_2->setText("4.65%");
        ui->comboBox_deadline_2->setCurrentText(0);
        ui->comboBox_rate_2->setCurrentText(0);
        ui->radioButton_benxi_2->clicked();

        if(ui->comboBox_Retype_2->currentIndex() == 0)
            ui->lineEdit_Rerate_2->setText("4.65%");
        else
        {
            ui->lineEdit_Rerate_2->setText("3.25%");
        }

        if(ui->comboBox_type_2->currentIndex() == 0)
            ui->lineEdit_rate_2->setText("4.65%");
        else
        {
            ui->lineEdit_rate_2->setText("3.25%");
        }

    }
    else if(ret == QMessageBox::No)
    {
        //保持不动
        //忽略事件，事件继续给父组件传递
    }

}
/******************************************-----End_Result_NO.2 贷款总额-----****************************************************/


/******************************************-----Begin_Calater_3 组合型贷款-----**************************************************/
void Widget::on_comboBox_type_3_currentIndexChanged(int index)
{
    if(index == 1|| index == 2)
    {
        ui->stackedWidget->setCurrentWidget(ui->Calater_1);
        if(index == 1)
        {
            ui->comboBox_type->setCurrentIndex(0);
            ui->comboBox_calaterWay->setCurrentIndex(0);
        }
        else {
                ui->comboBox_type->setCurrentIndex(1);
                ui->comboBox_calaterWay->setCurrentIndex(0);
        }
    }
}
void Widget::on_comboBox_shangRate_currentIndexChanged(int index)
{
    if(ui->comboBox_Redeadline_3->currentIndex() <20)
    {
        switch (index)
        {
        //2020.4.20
        case 0:ui->lineEdit_shangRate->setText("4.65%");break;
        //2020.3.20
        case 1:ui->lineEdit_shangRate->setText("4.75%");break;
        //2019.9.20
        case 2:ui->lineEdit_shangRate->setText("4.85%");break;
        //2015.10.24
        case 3:ui->lineEdit_shangRate->setText("5.39%");break;
        case 4:ui->lineEdit_shangRate->setText("5.15%");break;case 5:ui->lineEdit_shangRate->setText("4.17%");break;
        case 6:ui->lineEdit_shangRate->setText("3.43%");break;case 7:ui->lineEdit_shangRate->setText("4.9%");break;
        //2015.8.26
        case 8:ui->lineEdit_shangRate->setText("5.67%");break;case 9:ui->lineEdit_shangRate->setText("4.38%");break;
        case 10:ui->lineEdit_shangRate->setText("3.61%");break;case 11:ui->lineEdit_shangRate->setText("5.15%");break;
        //2015.6.28
        case 12:ui->lineEdit_shangRate->setText("5.94%");break;case 13:ui->lineEdit_shangRate->setText("4.59%");break;
        case 14:ui->lineEdit_shangRate->setText("3.78%");break;case 15:ui->lineEdit_shangRate->setText("5.4%");break;
        //2015.5.11
        case 16:ui->lineEdit_shangRate->setText("6.22%");break;case 17:ui->lineEdit_shangRate->setText("4.8%");break;
        case 18:ui->lineEdit_shangRate->setText("3.96%");break;case 19:ui->lineEdit_shangRate->setText("5.65%");break;
        //2015.3.1
        case 20:ui->lineEdit_shangRate->setText("6.49%");break;case 21:ui->lineEdit_shangRate->setText("5.01%");break;
        case 22:ui->lineEdit_shangRate->setText("4.13%");break;case 23:ui->lineEdit_shangRate->setText("5.9%");break;
        //2014.11.22
        case 24:ui->lineEdit_shangRate->setText("6.7%");break;case 25:ui->lineEdit_shangRate->setText("5.2%");break;
        case 26:ui->lineEdit_shangRate->setText("4.3%");break;case 27:ui->lineEdit_shangRate->setText("6.15%");break;
        //2012.7.6
        case 28:ui->lineEdit_shangRate->setText("7.21%");break;case 29:ui->lineEdit_shangRate->setText("5.57%");break;
        case 30:ui->lineEdit_shangRate->setText("4.59%");break;case 31:ui->lineEdit_shangRate->setText("6.55%");break;
        //2012.6.8
        case 32:ui->lineEdit_shangRate->setText("7.48%");break;case 33:ui->lineEdit_shangRate->setText("5.78%");break;
        case 34:ui->lineEdit_shangRate->setText("4.76%");break;case 35:ui->lineEdit_shangRate->setText("6.8%");break;
        }
    }
    else
    {
        switch (index)
        {

        //2020.4.20
        case 0:ui->lineEdit_shangRate->setText("3.85%");break;
        //2020.3.20
        case 1:ui->lineEdit_shangRate->setText("4.05%");break;
        //2019.9.20
        case 2:ui->lineEdit_shangRate->setText("4.2%");break;
        //2015.10.24
        case 3:ui->lineEdit_shangRate->setText("5.23%");break;
        case 4:ui->lineEdit_shangRate->setText("4.99%");break;case 5:ui->lineEdit_shangRate->setText("4.04%");break;
        case 6:ui->lineEdit_shangRate->setText("3.33%");break;case 7:ui->lineEdit_shangRate->setText("4.75%");break;
        //2015.8.26
        case 8:ui->lineEdit_shangRate->setText("5.5%");break;case 9:ui->lineEdit_shangRate->setText("4.25%");break;
        case 10:ui->lineEdit_shangRate->setText("3.5%");break;case 11:ui->lineEdit_shangRate->setText("5%");break;
        //2015.6.28
        case 12:ui->lineEdit_shangRate->setText("5.78%");break;case 13:ui->lineEdit_shangRate->setText("4.46%");break;
        case 14:ui->lineEdit_shangRate->setText("3.68%");break;case 15:ui->lineEdit_shangRate->setText("5.25%");break;
        //2015.5.11
        case 16:ui->lineEdit_shangRate->setText("6.05%");break;case 17:ui->lineEdit_shangRate->setText("4.68%");break;
        case 18:ui->lineEdit_shangRate->setText("3.85%");break;case 19:ui->lineEdit_shangRate->setText("5.5%");break;
        //2015.3.1
        case 20:ui->lineEdit_shangRate->setText("6.33%");break;case 21:ui->lineEdit_shangRate->setText("4.89%");break;
        case 22:ui->lineEdit_shangRate->setText("4.03%");break;case 23:ui->lineEdit_shangRate->setText("5.75%");break;
        //2014.11.22
        case 24:ui->lineEdit_shangRate->setText("6.6%");break;case 25:ui->lineEdit_shangRate->setText("5.1%");break;
        case 26:ui->lineEdit_shangRate->setText("4.2%");break;case 27:ui->lineEdit_shangRate->setText("6%");break;
        //2012.7.6
        case 28:ui->lineEdit_shangRate->setText("6.77%");break;case 29:ui->lineEdit_shangRate->setText("5.23%");break;
        case 30:ui->lineEdit_shangRate->setText("4.31%");break;case 31:ui->lineEdit_shangRate->setText("6.15%");break;
        //2012.6.8
        case 32:ui->lineEdit_shangRate->setText("7.04%");break;case 33:ui->lineEdit_shangRate->setText("5.44%");break;
        case 34:ui->lineEdit_shangRate->setText("4.48%");break;case 35:ui->lineEdit_shangRate->setText("6.4%");break;
        }
    }


}
void Widget::on_comboBox_gongRate_currentIndexChanged(int index)
{
    if(ui->comboBox_Redeadline_3->currentIndex() <20)
    {
        switch (index)
        {
        //2020.4.20
        case 0:ui->lineEdit_gongRate->setText("3.25%");break;
        //2020.3.20
        case 1:ui->lineEdit_gongRate->setText("3.25%");break;
        //2019.9.20
        case 2:ui->lineEdit_gongRate->setText("3.25%");break;
        //2015.10.24
        case 3:ui->lineEdit_gongRate->setText("3.57%");break;
        case 4:ui->lineEdit_gongRate->setText("3.41%");break;case 5:ui->lineEdit_gongRate->setText("2.76%");break;
        case 6:ui->lineEdit_gongRate->setText("2.28%");break;case 7:ui->lineEdit_gongRate->setText("3.25%");break;
        //2015.8.26
        case 8:ui->lineEdit_gongRate->setText("3.57%");break;case 9:ui->lineEdit_gongRate->setText("2.76%");break;
        case 10:ui->lineEdit_gongRate->setText("2.28%");break;case 11:ui->lineEdit_gongRate->setText("3.25%");break;
        //2015.6.28
        case 12:ui->lineEdit_gongRate->setText("3.57%");break;case 13:ui->lineEdit_gongRate->setText("2.98%");break;
        case 14:ui->lineEdit_gongRate->setText("2.45%");break;case 15:ui->lineEdit_gongRate->setText("3.5%");break;
        //2015.5.11
        case 16:ui->lineEdit_gongRate->setText("4.13%");break;case 17:ui->lineEdit_gongRate->setText("3.19%");break;
        case 18:ui->lineEdit_gongRate->setText("2.63%");break;case 19:ui->lineEdit_gongRate->setText("3.75%");break;
        //2015.3.1
        case 20:ui->lineEdit_gongRate->setText("4.4%");break;case 21:ui->lineEdit_gongRate->setText("3.4%");break;
        case 22:ui->lineEdit_gongRate->setText("2.8%");break;case 23:ui->lineEdit_gongRate->setText("4%");break;
        //2014.11.22
        case 24:ui->lineEdit_gongRate->setText("4.25%");break;case 25:ui->lineEdit_gongRate->setText("4.25%");break;
        case 26:ui->lineEdit_gongRate->setText("4.25%");break;case 27:ui->lineEdit_gongRate->setText("4.25%");break;
        //2012.7.6
        case 28:ui->lineEdit_gongRate->setText("4.5%");break;case 29:ui->lineEdit_gongRate->setText("4.5%");break;
        case 30:ui->lineEdit_gongRate->setText("4.5%");break;case 31:ui->lineEdit_gongRate->setText("4.5%");break;
        //2012.6.8
        case 32:ui->lineEdit_gongRate->setText("4.7%");break;case 33:ui->lineEdit_gongRate->setText("4.7%");break;
        case 34:ui->lineEdit_gongRate->setText("4.7%");break;case 35:ui->lineEdit_gongRate->setText("4.7%");break;

        }
    }
    else
    {
        switch (index)
        {

        //2020.4.20
        case 0:ui->lineEdit_gongRate->setText("2.75%");break;
        //2020.3.20
        case 1:ui->lineEdit_gongRate->setText("2.75%");break;
        //2019.9.20
        case 2:ui->lineEdit_gongRate->setText("2.75%");break;
        //2015.10.24
        case 3:ui->lineEdit_gongRate->setText("3.03%");break;
        case 4:ui->lineEdit_gongRate->setText("2.89%");break;case 5:ui->lineEdit_gongRate->setText("2.34%");break;
        case 6:ui->lineEdit_gongRate->setText("1.93%");break;case 7:ui->lineEdit_gongRate->setText("2.75%");break;
        //2015.8.26
        case 8:ui->lineEdit_gongRate->setText("3.03%");break;case 9:ui->lineEdit_gongRate->setText("2.34%");break;
        case 10:ui->lineEdit_gongRate->setText("1.93%");break;case 11:ui->lineEdit_gongRate->setText("2.75%");break;
        //2015.6.28
        case 12:ui->lineEdit_gongRate->setText("3.3%");break;case 13:ui->lineEdit_gongRate->setText("2.55%");break;
        case 14:ui->lineEdit_gongRate->setText("2.1%");break;case 15:ui->lineEdit_gongRate->setText("3%");break;
        //2015.5.11
        case 16:ui->lineEdit_gongRate->setText("3.57%");break;case 17:ui->lineEdit_gongRate->setText("2.76%");break;
        case 18:ui->lineEdit_gongRate->setText("2.28%");break;case 19:ui->lineEdit_gongRate->setText("3.25%");break;
        //2015.3.1
        case 20:ui->lineEdit_gongRate->setText("3.85%");break;case 21:ui->lineEdit_gongRate->setText("2.97%");break;
        case 22:ui->lineEdit_gongRate->setText("2.45%");break;case 23:ui->lineEdit_gongRate->setText("3.5%");break;
        //2014.11.22
        case 24:ui->lineEdit_gongRate->setText("3.75%");break;case 25:ui->lineEdit_gongRate->setText("3.75%");break;
        case 26:ui->lineEdit_gongRate->setText("3.75%");break;case 27:ui->lineEdit_gongRate->setText("3.75%");break;
        //2012.7.6
        case 28:ui->lineEdit_gongRate->setText("4%");break;case 29:ui->lineEdit_gongRate->setText("4%");break;
        case 30:ui->lineEdit_gongRate->setText("4%");break;case 31:ui->lineEdit_gongRate->setText("4%");break;
        //2012.6.8
        case 32:ui->lineEdit_gongRate->setText("4.2%");break;case 33:ui->lineEdit_gongRate->setText("4.2%");break;
        case 34:ui->lineEdit_gongRate->setText("4.2%");break;case 35:ui->lineEdit_gongRate->setText("4.2%");break;
        }
    }

}
short Widget::on_comboBox_Redeadline_3_currentIndexChanged(int index)
{
    short Deadline = 0;
    switch (index)
    {
    case 0:Deadline = 300;break;case 1:Deadline = 288;break;
    case 2:Deadline = 276;break;case 3:Deadline = 264;break;
    case 4:Deadline = 252;break;case 5:Deadline = 240;break;
    case 6:Deadline = 228;break;case 7:Deadline = 216;break;
    case 8:Deadline = 204;break;case 9:Deadline = 192;break;
    case 10:Deadline = 180;break;case 11:Deadline = 168;break;
    case 12:Deadline = 156;break;case 13:Deadline = 144;break;
    case 14:Deadline = 132;break;case 15:Deadline = 120;break;
    case 16:Deadline = 108;break;case 17:Deadline = 96;break;
    case 18:Deadline = 84;break;case 19:Deadline = 72;break;
    case 20:Deadline = 60;break;case 21:Deadline = 48;break;
    case 22:Deadline = 36;break;case 23:Deadline = 24;break;
    case 24:Deadline = 12;break;
    }

    if(index <20)
    {
        switch (ui->comboBox_gongRate->currentIndex())
        {
        //2020.4.20
        case 0:ui->lineEdit_gongRate->setText("3.25%");break;
        //2020.3.20
        case 1:ui->lineEdit_gongRate->setText("3.25%");break;
        //2019.9.20
        case 2:ui->lineEdit_gongRate->setText("3.25%");break;
        //2015.10.24
        case 3:ui->lineEdit_gongRate->setText("3.57%");break;
        case 4:ui->lineEdit_gongRate->setText("3.41%");break;case 5:ui->lineEdit_gongRate->setText("2.76%");break;
        case 6:ui->lineEdit_gongRate->setText("2.28%");break;case 7:ui->lineEdit_gongRate->setText("3.25%");break;
        //2015.8.26
        case 8:ui->lineEdit_gongRate->setText("3.57%");break;case 9:ui->lineEdit_gongRate->setText("2.76%");break;
        case 10:ui->lineEdit_gongRate->setText("2.28%");break;case 11:ui->lineEdit_gongRate->setText("3.25%");break;
        //2015.6.28
        case 12:ui->lineEdit_gongRate->setText("3.57%");break;case 13:ui->lineEdit_gongRate->setText("2.98%");break;
        case 14:ui->lineEdit_gongRate->setText("2.45%");break;case 15:ui->lineEdit_gongRate->setText("3.5%");break;
        //2015.5.11
        case 16:ui->lineEdit_gongRate->setText("4.13%");break;case 17:ui->lineEdit_gongRate->setText("3.19%");break;
        case 18:ui->lineEdit_gongRate->setText("2.63%");break;case 19:ui->lineEdit_gongRate->setText("3.75%");break;
        //2015.3.1
        case 20:ui->lineEdit_gongRate->setText("4.4%");break;case 21:ui->lineEdit_gongRate->setText("3.4%");break;
        case 22:ui->lineEdit_gongRate->setText("2.8%");break;case 23:ui->lineEdit_gongRate->setText("4%");break;
        //2014.11.22
        case 24:ui->lineEdit_gongRate->setText("4.25%");break;case 25:ui->lineEdit_gongRate->setText("4.25%");break;
        case 26:ui->lineEdit_gongRate->setText("4.25%");break;case 27:ui->lineEdit_gongRate->setText("4.25%");break;
        //2012.7.6
        case 28:ui->lineEdit_gongRate->setText("4.5%");break;case 29:ui->lineEdit_gongRate->setText("4.5%");break;
        case 30:ui->lineEdit_gongRate->setText("4.5%");break;case 31:ui->lineEdit_gongRate->setText("4.5%");break;
        //2012.6.8
        case 32:ui->lineEdit_gongRate->setText("4.7%");break;case 33:ui->lineEdit_gongRate->setText("4.7%");break;
        case 34:ui->lineEdit_gongRate->setText("4.7%");break;case 35:ui->lineEdit_gongRate->setText("4.7%");break;
        }
        switch (ui->comboBox_shangRate->currentIndex())
        {
        //2020.4.20
        case 0:ui->lineEdit_shangRate->setText("4.65%");break;
        //2020.3.20
        case 1:ui->lineEdit_shangRate->setText("4.75%");break;
        //2019.9.20
        case 2:ui->lineEdit_shangRate->setText("4.85%");break;
        //2015.10.24
        case 3:ui->lineEdit_shangRate->setText("5.39%");break;
        case 4:ui->lineEdit_shangRate->setText("5.15%");break;case 5:ui->lineEdit_shangRate->setText("4.17%");break;
        case 6:ui->lineEdit_shangRate->setText("3.43%");break;case 7:ui->lineEdit_shangRate->setText("4.9%");break;
        //2015.8.26
        case 8:ui->lineEdit_shangRate->setText("5.67%");break;case 9:ui->lineEdit_shangRate->setText("4.38%");break;
        case 10:ui->lineEdit_shangRate->setText("3.61%");break;case 11:ui->lineEdit_shangRate->setText("5.15%");break;
        //2015.6.28
        case 12:ui->lineEdit_shangRate->setText("5.94%");break;case 13:ui->lineEdit_shangRate->setText("4.59%");break;
        case 14:ui->lineEdit_shangRate->setText("3.78%");break;case 15:ui->lineEdit_shangRate->setText("5.4%");break;
        //2015.5.11
        case 16:ui->lineEdit_shangRate->setText("6.22%");break;case 17:ui->lineEdit_shangRate->setText("4.8%");break;
        case 18:ui->lineEdit_shangRate->setText("3.96%");break;case 19:ui->lineEdit_shangRate->setText("5.65%");break;
        //2015.3.1
        case 20:ui->lineEdit_shangRate->setText("6.49%");break;case 21:ui->lineEdit_shangRate->setText("5.01%");break;
        case 22:ui->lineEdit_shangRate->setText("4.13%");break;case 23:ui->lineEdit_shangRate->setText("5.9%");break;
        //2014.11.22
        case 24:ui->lineEdit_shangRate->setText("6.7%");break;case 25:ui->lineEdit_shangRate->setText("5.2%");break;
        case 26:ui->lineEdit_shangRate->setText("4.3%");break;case 27:ui->lineEdit_shangRate->setText("6.15%");break;
        //2012.7.6
        case 28:ui->lineEdit_shangRate->setText("7.21%");break;case 29:ui->lineEdit_shangRate->setText("5.57%");break;
        case 30:ui->lineEdit_shangRate->setText("4.59%");break;case 31:ui->lineEdit_shangRate->setText("6.55%");break;
        //2012.6.8
        case 32:ui->lineEdit_shangRate->setText("7.48%");break;case 33:ui->lineEdit_shangRate->setText("5.78%");break;
        case 34:ui->lineEdit_shangRate->setText("4.76%");break;case 35:ui->lineEdit_shangRate->setText("6.8%");break;
        }
    }
    else
    {
        switch (ui->comboBox_gongRate->currentIndex())
        {

        //2020.4.20
        case 0:ui->lineEdit_gongRate->setText("2.75%");break;
        //2020.3.20
        case 1:ui->lineEdit_gongRate->setText("2.75%");break;
        //2019.9.20
        case 2:ui->lineEdit_gongRate->setText("2.75%");break;
        //2015.10.24
        case 3:ui->lineEdit_gongRate->setText("3.03%");break;
        case 4:ui->lineEdit_gongRate->setText("2.89%");break;case 5:ui->lineEdit_gongRate->setText("2.34%");break;
        case 6:ui->lineEdit_gongRate->setText("1.93%");break;case 7:ui->lineEdit_gongRate->setText("2.75%");break;
        //2015.8.26
        case 8:ui->lineEdit_gongRate->setText("3.03%");break;case 9:ui->lineEdit_gongRate->setText("2.34%");break;
        case 10:ui->lineEdit_gongRate->setText("1.93%");break;case 11:ui->lineEdit_gongRate->setText("2.75%");break;
        //2015.6.28
        case 12:ui->lineEdit_gongRate->setText("3.3%");break;case 13:ui->lineEdit_gongRate->setText("2.55%");break;
        case 14:ui->lineEdit_gongRate->setText("2.1%");break;case 15:ui->lineEdit_gongRate->setText("3%");break;
        //2015.5.11
        case 16:ui->lineEdit_gongRate->setText("3.57%");break;case 17:ui->lineEdit_gongRate->setText("2.76%");break;
        case 18:ui->lineEdit_gongRate->setText("2.28%");break;case 19:ui->lineEdit_gongRate->setText("3.25%");break;
        //2015.3.1
        case 20:ui->lineEdit_gongRate->setText("3.85%");break;case 21:ui->lineEdit_gongRate->setText("2.97%");break;
        case 22:ui->lineEdit_gongRate->setText("2.45%");break;case 23:ui->lineEdit_gongRate->setText("3.5%");break;
        //2014.11.22
        case 24:ui->lineEdit_gongRate->setText("3.75%");break;case 25:ui->lineEdit_gongRate->setText("3.75%");break;
        case 26:ui->lineEdit_gongRate->setText("3.75%");break;case 27:ui->lineEdit_gongRate->setText("3.75%");break;
        //2012.7.6
        case 28:ui->lineEdit_gongRate->setText("4%");break;case 29:ui->lineEdit_gongRate->setText("4%");break;
        case 30:ui->lineEdit_gongRate->setText("4%");break;case 31:ui->lineEdit_gongRate->setText("4%");break;
        //2012.6.8
        case 32:ui->lineEdit_gongRate->setText("4.2%");break;case 33:ui->lineEdit_gongRate->setText("4.2%");break;
        case 34:ui->lineEdit_gongRate->setText("4.2%");break;case 35:ui->lineEdit_gongRate->setText("4.2%");break;
        }
        switch (ui->comboBox_shangRate->currentIndex())
        {

        //2020.4.20
        case 0:ui->lineEdit_shangRate->setText("3.85%");break;
        //2020.3.20
        case 1:ui->lineEdit_shangRate->setText("4.05%");break;
        //2019.9.20
        case 2:ui->lineEdit_shangRate->setText("4.2%");break;
        //2015.10.24
        case 3:ui->lineEdit_shangRate->setText("5.23%");break;
        case 4:ui->lineEdit_shangRate->setText("4.99%");break;case 5:ui->lineEdit_shangRate->setText("4.04%");break;
        case 6:ui->lineEdit_shangRate->setText("3.33%");break;case 7:ui->lineEdit_shangRate->setText("4.75%");break;
        //2015.8.26
        case 8:ui->lineEdit_shangRate->setText("5.5%");break;case 9:ui->lineEdit_shangRate->setText("4.25%");break;
        case 10:ui->lineEdit_shangRate->setText("3.5%");break;case 11:ui->lineEdit_shangRate->setText("5%");break;
        //2015.6.28
        case 12:ui->lineEdit_shangRate->setText("5.78%");break;case 13:ui->lineEdit_shangRate->setText("4.46%");break;
        case 14:ui->lineEdit_shangRate->setText("3.68%");break;case 15:ui->lineEdit_shangRate->setText("5.25%");break;
        //2015.5.11
        case 16:ui->lineEdit_shangRate->setText("6.05%");break;case 17:ui->lineEdit_shangRate->setText("4.68%");break;
        case 18:ui->lineEdit_shangRate->setText("3.85%");break;case 19:ui->lineEdit_shangRate->setText("5.5%");break;
        //2015.3.1
        case 20:ui->lineEdit_shangRate->setText("6.33%");break;case 21:ui->lineEdit_shangRate->setText("4.89%");break;
        case 22:ui->lineEdit_shangRate->setText("4.03%");break;case 23:ui->lineEdit_shangRate->setText("5.75%");break;
        //2014.11.22
        case 24:ui->lineEdit_shangRate->setText("6.6%");break;case 25:ui->lineEdit_shangRate->setText("5.1%");break;
        case 26:ui->lineEdit_shangRate->setText("4.2%");break;case 27:ui->lineEdit_shangRate->setText("6%");break;
        //2012.7.6
        case 28:ui->lineEdit_shangRate->setText("6.77%");break;case 29:ui->lineEdit_shangRate->setText("5.23%");break;
        case 30:ui->lineEdit_shangRate->setText("4.31%");break;case 31:ui->lineEdit_shangRate->setText("6.15%");break;
        //2012.6.8
        case 32:ui->lineEdit_shangRate->setText("7.04%");break;case 33:ui->lineEdit_shangRate->setText("5.44%");break;
        case 34:ui->lineEdit_shangRate->setText("4.48%");break;case 35:ui->lineEdit_shangRate->setText("6.4%");break;
        }
    }
    return Deadline;
}
void Widget::on_pushButton_start_3_clicked()
{
    QString checkShangLoan= ui->lineEdit_shangLoan->text();
    QString checkGongLoan = ui->lineEdit_gongLoan->text();
    QString checkShangRate= ui->lineEdit_shangRate->text();
    QString checkGongRate = ui->lineEdit_gongRate->text();
    bool Justise = 0;
    double rate1 = 0,rate2 = 0;
    //判断是否为空
    if(!checkShangLoan.isEmpty()&&!checkGongLoan.isEmpty()&&!checkShangRate.isEmpty()&&!checkGongRate.isEmpty())
    {
        int checkPoint = 0;
        bool flag1 = 0,flag2 = 0,flag3 = 0, flag4 = 0;
        //判断商贷额度输入
        for(int i= 0 ;i<checkShangLoan.length();i++)
        {
            if((checkShangLoan[i]<='9'&&checkShangLoan[i]>='0')||checkShangLoan[i]=='.')
            {
                if(checkShangLoan[i]=='.')
                    checkPoint++;
                if(checkPoint>1){flag1=1;break;}
            }
            else{flag1 = 1;break;}
        }
        //判断公积金额度输入
        checkPoint = 0;
        for(int i= 0 ;i<checkGongLoan.length();i++)
        {
            if((checkGongLoan[i]<='9'&&checkGongLoan[i]>='0')||checkGongLoan[i]=='.')
            {
                if(checkGongLoan[i]=='.')
                    checkPoint++;
                if(checkPoint>1){flag2=1;break;}
            }
            else{flag2 = 1;break;}
        }
        //判断输入商贷利率
        checkPoint = 0;
        int n = 0;
        for(int i= 0 ;i<checkShangRate.length()-1;i++)
        {
            if((checkShangRate[i]<='9'&&checkShangRate[i]>='0')||checkShangRate[i]=='.')
            {
                QChar te = checkShangRate.at(i);
                if(checkShangRate[i]=='.')
                {
                    checkPoint++;
                }
                if(checkPoint>=2){flag3=1;break;}
                if(checkShangRate[i]!='.')
                {
                    rate1 = rate1*10+(te.toLatin1()-'0');
                    n++;
                }
            }
            else{flag3 = 1;break;}
        }
        rate1 /= pow(10,n+1);
        if(rate1 == 0)
            flag3 = 1;

        //判断输入公基金贷款利率
        checkPoint = 0;
        int k = 0;
        for(int i= 0 ;i<checkGongRate.length()-1;i++)
        {
            if((checkGongRate[i]<='9'&&checkGongRate[i]>='0')||checkGongRate[i]=='.')
            {
                QChar te = checkGongRate.at(i);
                if(checkGongRate[i]=='.')
                {
                    checkPoint++;
                }
                if(checkPoint>=2){flag4=1;break;}
                if(checkGongRate[i]!='.')
                {
                    rate2 = rate2*10+(te.toLatin1()-'0');
                    k++;
                }
            }
            else{flag4 = 1;break;}
        }
        rate2 /= pow(10,k+1);
        if(rate2 == 0)
            flag4 = 1;

        if(flag1 == 1 || flag3 == 1 || flag2 == 1 || flag4 == 1)
        {
            if(flag1 == 1 && flag1 == flag2 && flag2 == flag3 && flag3 == flag4)
            {
                QMessageBox *eorr = new QMessageBox(this);
                eorr->warning(this,"错误","您的输入的房屋商业贷款额、公积金贷款额、及其对应的利率有误",QMessageBox::Ok);
            }
            else if(flag1 == 1 &&flag2 == 0 && flag3 == 0 && flag4 == 0)
            {
               QMessageBox *eorr = new QMessageBox(this);
               eorr->warning(this,"错误","您的输入房屋商业贷款额有误",QMessageBox::Ok);
            }
            else if(flag1 == 0 &&flag2 == 1 && flag3 == 0 && flag4 == 0)
            {
                 QMessageBox *eorr = new QMessageBox(this);
                 eorr->warning(this,"错误","您的输入房屋公积金贷款额有误",QMessageBox::Ok);
            }
            else if(flag1 == 0 &&flag2 == 0 && flag3 == 1 && flag4 == 0)
            {
                  QMessageBox *eorr = new QMessageBox(this);
                  eorr->warning(this,"错误","您的输入房屋商业贷款利率不合法，请重新输入",QMessageBox::Ok);
            }
            else if(flag1 == 0 &&flag2 == 0 && flag3 == 0 && flag4 == 1)
            {
                QMessageBox *eorr = new QMessageBox(this);
                eorr->warning(this,"错误","您的输入房屋公积金贷款利率不合法，请重新输入",QMessageBox::Ok);
            }
            else
            {
                QMessageBox *eorr = new QMessageBox(this);
                eorr->warning(this,"错误","您的输入有误，请重新输入",QMessageBox::Ok);
            }
        }
        else
        {
            Justise = 1;
        }
    }
    //如果有输入有空
    else
    {
        if(checkShangLoan.isEmpty()&&checkGongLoan.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入商业贷款额、公积金贷款额",QMessageBox::Ok);
        }
        else if(checkShangLoan.isEmpty()&&!checkGongLoan.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入商业贷款额",QMessageBox::Ok);
        }
        else if(!checkShangLoan.isEmpty()&&checkGongLoan.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入公积金贷款额",QMessageBox::Ok);
        }
        else
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您未位输入的选项，请仔细检查",QMessageBox::Ok);
        }
    }

    if(Justise ==1 )
    {
        //房屋贷款总额
        double loan1 = ui->lineEdit_shangLoan->text().toDouble();
        loan1 *= 10000;
        double loan2 = ui->lineEdit_gongLoan->text().toDouble();
        loan2 *= 10000;

        ui->lineEdit_ReshangLoan->setText(ui->lineEdit_shangLoan->text());
        ui->lineEdit_RegongLoan->setText(ui->lineEdit_gongLoan->text());
        short Deadline =on_comboBox_Redeadline_3_currentIndexChanged(ui->comboBox_Redeadline_3->currentIndex());

//        还款日期下拉框
        ui->comboBox_Redeadline_5->setCurrentIndex(ui->comboBox_Redeadline_3->currentIndex());

//        商贷利率下拉框
        ui->comboBox_ReshangRate->setCurrentIndex(ui->comboBox_shangRate->currentIndex());
        ui->lineEdit_ReshangRate->setText(QString("%1%").arg(rate1*100));

//        公积金贷款利率下拉框
        ui->comboBox_RegongRate->setCurrentIndex(ui->comboBox_gongRate->currentIndex());
        ui->lineEdit_RegongRate->setText(QString("%1%").arg(rate2*100));

//        贷款类型下拉框
        ui->comboBox_Retype_3->setCurrentIndex(ui->comboBox_type_3->currentIndex());


        house_loan A(loan1,rate1,Deadline);

        house_loan B(loan2,rate2,Deadline);

        //等额本息
        if(ui->radioButton_benxi_3->isChecked())
        {

            ui->radioButton_Rebenxi_3->clicked();
            ui->label_Remonthpay_change_4->setText("每月月供:");
            ui->widget_Redecline_4->hide();

            A.Average_Capital_Plus_Interest();
            B.Average_Capital_Plus_Interest();
            //月供
            ui->label_Remonthpay_4->setText(QString::number(A.Getmonthpayment()+B.Getmonthpayment(), 'f', 1));
            //贷款总额
            ui->label_ReLoan_4->setText(QString::number(A.Getloan()+B.Getloan(), 'f', 1));
            //支付利息
            ui->label_Reinterest_4->setText(QString::number(A.Getinterest()+B.Getinterest(), 'f', 1));
            //还款总额
            ui->label_Retotalpay_4->setText(QString::number(A.Getendpayment()+B.Getendpayment(), 'f', 1));
            //还款月数
            ui->label_Redate_4->setText(QString::number(A.Getdeadline()+B.Getdeadline(), 'f', 1));
        }
        //等额本金
        else if(ui->radioButton_benjin_3->isChecked())
        {
            ui->radioButton_Rebenjin_3->clicked();
            ui->widget_Redecline_4->show();
            ui->label_Remonthpay_change_4->setText("首月月供:");
            A.Average_capital();
            B.Average_capital();
            qDebug()<<A.Getmonthpayment();
            //月供
            ui->label_Remonthpay_4->setText(QString::number(A.Getmonthpayment()+B.Getmonthpayment(), 'f', 1));
            //每月递减
            ui->label_Redecline_4->setText(QString::number(A.Getmonth_cut()+B.Getmonth_cut(), 'f', 1));
            //贷款总额
            ui->label_ReLoan_4->setText(QString::number(A.Getloan()+B.Getloan(), 'f', 1));
            //支付利息
            ui->label_Reinterest_4->setText(QString::number(A.Getinterest()+B.Getinterest(), 'f', 1));
            //还款总额
            ui->label_Retotalpay_4->setText(QString::number(A.Getendpayment()+B.Getendpayment(), 'f', 1));
            //还款月数
            ui->label_Redate_4->setText(QString::number(A.Getdeadline()+B.Getdeadline(), 'f', 1));
        }
        ui->stackedWidget->setCurrentWidget(ui->Calater_3_Result);
    }

}
void Widget::on_pushButton_cance_3_clicked()
{
    int ret = QMessageBox::question(this,"Question","是否需要清除数据？");
    if(ret == QMessageBox::Yes)
    {
        ui->lineEdit_shangLoan->clear();
        ui->lineEdit_shangRate->setText("4.65%");
        ui->comboBox_shangRate->setCurrentText(0);
        ui->radioButton_benxi_3->clicked();
        ui->lineEdit_gongLoan->clear();
        ui->lineEdit_gongRate->setText("3.25%");
        ui->comboBox_gongRate->setCurrentText(0);
        ui->comboBox_Redeadline_3->setCurrentText(0);

    }
    else if(ret == QMessageBox::No)
    {
        //保持不动
        //忽略事件，事件继续给父组件传递
    }
}
/******************************************-----End_Calater_3 组合型贷款-----****************************************************/


/******************************************-----Begin_Result_NO.3 组合型贷款-----************************************************/
void Widget::on_comboBox_Retype_3_currentIndexChanged(int index)
{
    if(index == 1|| index == 2)
    {
        ui->stackedWidget->setCurrentWidget(ui->Calater_1);
        if(index == 1)
        {
            ui->comboBox_type->setCurrentIndex(0);
            ui->comboBox_calaterWay->setCurrentIndex(0);
        }
        else {
                ui->comboBox_type->setCurrentIndex(1);
                ui->comboBox_calaterWay->setCurrentIndex(0);
        }
    }
}
void Widget::on_comboBox_ReshangRate_currentIndexChanged(int index)
{
    if(ui->comboBox_Redeadline_5->currentIndex() <20)
    {
        switch (index)
        {
        //2020.4.20
        case 0:ui->lineEdit_ReshangRate->setText("4.65%");break;
        //2020.3.20
        case 1:ui->lineEdit_ReshangRate->setText("4.75%");break;
        //2019.9.20
        case 2:ui->lineEdit_ReshangRate->setText("4.85%");break;
        //2015.10.24
        case 3:ui->lineEdit_ReshangRate->setText("5.39%");break;
        case 4:ui->lineEdit_ReshangRate->setText("5.15%");break;case 5:ui->lineEdit_ReshangRate->setText("4.17%");break;
        case 6:ui->lineEdit_ReshangRate->setText("3.43%");break;case 7:ui->lineEdit_ReshangRate->setText("4.9%");break;
        //2015.8.26
        case 8:ui->lineEdit_ReshangRate->setText("5.67%");break;case 9:ui->lineEdit_ReshangRate->setText("4.38%");break;
        case 10:ui->lineEdit_ReshangRate->setText("3.61%");break;case 11:ui->lineEdit_ReshangRate->setText("5.15%");break;
        //2015.6.28
        case 12:ui->lineEdit_ReshangRate->setText("5.94%");break;case 13:ui->lineEdit_ReshangRate->setText("4.59%");break;
        case 14:ui->lineEdit_ReshangRate->setText("3.78%");break;case 15:ui->lineEdit_ReshangRate->setText("5.4%");break;
        //2015.5.11
        case 16:ui->lineEdit_ReshangRate->setText("6.22%");break;case 17:ui->lineEdit_ReshangRate->setText("4.8%");break;
        case 18:ui->lineEdit_ReshangRate->setText("3.96%");break;case 19:ui->lineEdit_ReshangRate->setText("5.65%");break;
        //2015.3.1
        case 20:ui->lineEdit_ReshangRate->setText("6.49%");break;case 21:ui->lineEdit_ReshangRate->setText("5.01%");break;
        case 22:ui->lineEdit_ReshangRate->setText("4.13%");break;case 23:ui->lineEdit_ReshangRate->setText("5.9%");break;
        //2014.11.22
        case 24:ui->lineEdit_ReshangRate->setText("6.7%");break;case 25:ui->lineEdit_ReshangRate->setText("5.2%");break;
        case 26:ui->lineEdit_ReshangRate->setText("4.3%");break;case 27:ui->lineEdit_ReshangRate->setText("6.15%");break;
        //2012.7.6
        case 28:ui->lineEdit_ReshangRate->setText("7.21%");break;case 29:ui->lineEdit_ReshangRate->setText("5.57%");break;
        case 30:ui->lineEdit_ReshangRate->setText("4.59%");break;case 31:ui->lineEdit_ReshangRate->setText("6.55%");break;
        //2012.6.8
        case 32:ui->lineEdit_ReshangRate->setText("7.48%");break;case 33:ui->lineEdit_ReshangRate->setText("5.78%");break;
        case 34:ui->lineEdit_ReshangRate->setText("4.76%");break;case 35:ui->lineEdit_ReshangRate->setText("6.8%");break;
        }
    }
    else
    {
        switch (index)
        {

        //2020.4.20
        case 0:ui->lineEdit_ReshangRate->setText("3.85%");break;
        //2020.3.20
        case 1:ui->lineEdit_ReshangRate->setText("4.05%");break;
        //2019.9.20
        case 2:ui->lineEdit_ReshangRate->setText("4.2%");break;
        //2015.10.24
        case 3:ui->lineEdit_ReshangRate->setText("5.23%");break;
        case 4:ui->lineEdit_ReshangRate->setText("4.99%");break;case 5:ui->lineEdit_ReshangRate->setText("4.04%");break;
        case 6:ui->lineEdit_ReshangRate->setText("3.33%");break;case 7:ui->lineEdit_ReshangRate->setText("4.75%");break;
        //2015.8.26
        case 8:ui->lineEdit_ReshangRate->setText("5.5%");break;case 9:ui->lineEdit_ReshangRate->setText("4.25%");break;
        case 10:ui->lineEdit_ReshangRate->setText("3.5%");break;case 11:ui->lineEdit_ReshangRate->setText("5%");break;
        //2015.6.28
        case 12:ui->lineEdit_ReshangRate->setText("5.78%");break;case 13:ui->lineEdit_ReshangRate->setText("4.46%");break;
        case 14:ui->lineEdit_ReshangRate->setText("3.68%");break;case 15:ui->lineEdit_ReshangRate->setText("5.25%");break;
        //2015.5.11
        case 16:ui->lineEdit_ReshangRate->setText("6.05%");break;case 17:ui->lineEdit_ReshangRate->setText("4.68%");break;
        case 18:ui->lineEdit_ReshangRate->setText("3.85%");break;case 19:ui->lineEdit_ReshangRate->setText("5.5%");break;
        //2015.3.1
        case 20:ui->lineEdit_ReshangRate->setText("6.33%");break;case 21:ui->lineEdit_ReshangRate->setText("4.89%");break;
        case 22:ui->lineEdit_ReshangRate->setText("4.03%");break;case 23:ui->lineEdit_ReshangRate->setText("5.75%");break;
        //2014.11.22
        case 24:ui->lineEdit_ReshangRate->setText("6.6%");break;case 25:ui->lineEdit_ReshangRate->setText("5.1%");break;
        case 26:ui->lineEdit_ReshangRate->setText("4.2%");break;case 27:ui->lineEdit_ReshangRate->setText("6%");break;
        //2012.7.6
        case 28:ui->lineEdit_ReshangRate->setText("6.77%");break;case 29:ui->lineEdit_ReshangRate->setText("5.23%");break;
        case 30:ui->lineEdit_ReshangRate->setText("4.31%");break;case 31:ui->lineEdit_ReshangRate->setText("6.15%");break;
        //2012.6.8
        case 32:ui->lineEdit_ReshangRate->setText("7.04%");break;case 33:ui->lineEdit_ReshangRate->setText("5.44%");break;
        case 34:ui->lineEdit_ReshangRate->setText("4.48%");break;case 35:ui->lineEdit_ReshangRate->setText("6.4%");break;
        }
    }
}
void Widget::on_comboBox_RegongRate_currentIndexChanged(int index)
{
    if(ui->comboBox_Redeadline_5->currentIndex() <20)
    {
        switch (index)
        {
        //2020.4.20
        case 0:ui->lineEdit_RegongRate->setText("3.25%");break;
        //2020.3.20
        case 1:ui->lineEdit_RegongRate->setText("3.25%");break;
        //2019.9.20
        case 2:ui->lineEdit_RegongRate->setText("3.25%");break;
        //2015.10.24
        case 3:ui->lineEdit_RegongRate->setText("3.57%");break;
        case 4:ui->lineEdit_RegongRate->setText("3.41%");break;case 5:ui->lineEdit_RegongRate->setText("2.76%");break;
        case 6:ui->lineEdit_RegongRate->setText("2.28%");break;case 7:ui->lineEdit_RegongRate->setText("3.25%");break;
        //2015.8.26
        case 8:ui->lineEdit_RegongRate->setText("3.57%");break;case 9:ui->lineEdit_RegongRate->setText("2.76%");break;
        case 10:ui->lineEdit_RegongRate->setText("2.28%");break;case 11:ui->lineEdit_RegongRate->setText("3.25%");break;
        //2015.6.28
        case 12:ui->lineEdit_RegongRate->setText("3.57%");break;case 13:ui->lineEdit_RegongRate->setText("2.98%");break;
        case 14:ui->lineEdit_RegongRate->setText("2.45%");break;case 15:ui->lineEdit_RegongRate->setText("3.5%");break;
        //2015.5.11
        case 16:ui->lineEdit_RegongRate->setText("4.13%");break;case 17:ui->lineEdit_RegongRate->setText("3.19%");break;
        case 18:ui->lineEdit_RegongRate->setText("2.63%");break;case 19:ui->lineEdit_RegongRate->setText("3.75%");break;
        //2015.3.1
        case 20:ui->lineEdit_RegongRate->setText("4.4%");break;case 21:ui->lineEdit_RegongRate->setText("3.4%");break;
        case 22:ui->lineEdit_RegongRate->setText("2.8%");break;case 23:ui->lineEdit_RegongRate->setText("4%");break;
        //2014.11.22
        case 24:ui->lineEdit_RegongRate->setText("4.25%");break;case 25:ui->lineEdit_RegongRate->setText("4.25%");break;
        case 26:ui->lineEdit_RegongRate->setText("4.25%");break;case 27:ui->lineEdit_RegongRate->setText("4.25%");break;
        //2012.7.6
        case 28:ui->lineEdit_RegongRate->setText("4.5%");break;case 29:ui->lineEdit_RegongRate->setText("4.5%");break;
        case 30:ui->lineEdit_RegongRate->setText("4.5%");break;case 31:ui->lineEdit_RegongRate->setText("4.5%");break;
        //2012.6.8
        case 32:ui->lineEdit_RegongRate->setText("4.7%");break;case 33:ui->lineEdit_RegongRate->setText("4.7%");break;
        case 34:ui->lineEdit_RegongRate->setText("4.7%");break;case 35:ui->lineEdit_RegongRate->setText("4.7%");break;

        }
    }
    else
    {
        switch (index)
        {

        //2020.4.20
        case 0:ui->lineEdit_RegongRate->setText("2.75%");break;
        //2020.3.20
        case 1:ui->lineEdit_RegongRate->setText("2.75%");break;
        //2019.9.20
        case 2:ui->lineEdit_RegongRate->setText("2.75%");break;
        //2015.10.24
        case 3:ui->lineEdit_RegongRate->setText("3.03%");break;
        case 4:ui->lineEdit_RegongRate->setText("2.89%");break;case 5:ui->lineEdit_RegongRate->setText("2.34%");break;
        case 6:ui->lineEdit_RegongRate->setText("1.93%");break;case 7:ui->lineEdit_RegongRate->setText("2.75%");break;
        //2015.8.26
        case 8:ui->lineEdit_RegongRate->setText("3.03%");break;case 9:ui->lineEdit_RegongRate->setText("2.34%");break;
        case 10:ui->lineEdit_RegongRate->setText("1.93%");break;case 11:ui->lineEdit_RegongRate->setText("2.75%");break;
        //2015.6.28
        case 12:ui->lineEdit_RegongRate->setText("3.3%");break;case 13:ui->lineEdit_RegongRate->setText("2.55%");break;
        case 14:ui->lineEdit_RegongRate->setText("2.1%");break;case 15:ui->lineEdit_RegongRate->setText("3%");break;
        //2015.5.11
        case 16:ui->lineEdit_RegongRate->setText("3.57%");break;case 17:ui->lineEdit_RegongRate->setText("2.76%");break;
        case 18:ui->lineEdit_RegongRate->setText("2.28%");break;case 19:ui->lineEdit_RegongRate->setText("3.25%");break;
        //2015.3.1
        case 20:ui->lineEdit_RegongRate->setText("3.85%");break;case 21:ui->lineEdit_RegongRate->setText("2.97%");break;
        case 22:ui->lineEdit_RegongRate->setText("2.45%");break;case 23:ui->lineEdit_RegongRate->setText("3.5%");break;
        //2014.11.22
        case 24:ui->lineEdit_RegongRate->setText("3.75%");break;case 25:ui->lineEdit_RegongRate->setText("3.75%");break;
        case 26:ui->lineEdit_RegongRate->setText("3.75%");break;case 27:ui->lineEdit_RegongRate->setText("3.75%");break;
        //2012.7.6
        case 28:ui->lineEdit_RegongRate->setText("4%");break;case 29:ui->lineEdit_RegongRate->setText("4%");break;
        case 30:ui->lineEdit_RegongRate->setText("4%");break;case 31:ui->lineEdit_RegongRate->setText("4%");break;
        //2012.6.8
        case 32:ui->lineEdit_RegongRate->setText("4.2%");break;case 33:ui->lineEdit_RegongRate->setText("4.2%");break;
        case 34:ui->lineEdit_RegongRate->setText("4.2%");break;case 35:ui->lineEdit_RegongRate->setText("4.2%");break;
        }
    }
}
short Widget::on_comboBox_Redeadline_5_currentIndexChanged(int index)
{
    short Deadline = 0;
    switch (index)
    {
    case 0:Deadline = 300;break;case 1:Deadline = 288;break;
    case 2:Deadline = 276;break;case 3:Deadline = 264;break;
    case 4:Deadline = 252;break;case 5:Deadline = 240;break;
    case 6:Deadline = 228;break;case 7:Deadline = 216;break;
    case 8:Deadline = 204;break;case 9:Deadline = 192;break;
    case 10:Deadline = 180;break;case 11:Deadline = 168;break;
    case 12:Deadline = 156;break;case 13:Deadline = 144;break;
    case 14:Deadline = 132;break;case 15:Deadline = 120;break;
    case 16:Deadline = 108;break;case 17:Deadline = 96;break;
    case 18:Deadline = 84;break;case 19:Deadline = 72;break;
    case 20:Deadline = 60;break;case 21:Deadline = 48;break;
    case 22:Deadline = 36;break;case 23:Deadline = 24;break;
    case 24:Deadline = 12;break;
    }

    if(index <20)
    {
        switch (ui->comboBox_RegongRate->currentIndex())
        {
        case 0:ui->lineEdit_RegongRate->setText("3.25%");break;
        //2020.3.20
        case 1:ui->lineEdit_RegongRate->setText("3.25%");break;
        //2019.9.20
        case 2:ui->lineEdit_RegongRate->setText("3.25%");break;
        //2015.10.24
        case 3:ui->lineEdit_RegongRate->setText("3.57%");break;
        case 4:ui->lineEdit_RegongRate->setText("3.41%");break;case 5:ui->lineEdit_RegongRate->setText("2.76%");break;
        case 6:ui->lineEdit_RegongRate->setText("2.28%");break;case 7:ui->lineEdit_RegongRate->setText("3.25%");break;
        //2015.8.26
        case 8:ui->lineEdit_RegongRate->setText("3.57%");break;case 9:ui->lineEdit_RegongRate->setText("2.76%");break;
        case 10:ui->lineEdit_RegongRate->setText("2.28%");break;case 11:ui->lineEdit_RegongRate->setText("3.25%");break;
        //2015.6.28
        case 12:ui->lineEdit_RegongRate->setText("3.57%");break;case 13:ui->lineEdit_RegongRate->setText("2.98%");break;
        case 14:ui->lineEdit_RegongRate->setText("2.45%");break;case 15:ui->lineEdit_RegongRate->setText("3.5%");break;
        //2015.5.11
        case 16:ui->lineEdit_RegongRate->setText("4.13%");break;case 17:ui->lineEdit_RegongRate->setText("3.19%");break;
        case 18:ui->lineEdit_RegongRate->setText("2.63%");break;case 19:ui->lineEdit_RegongRate->setText("3.75%");break;
        //2015.3.1
        case 20:ui->lineEdit_RegongRate->setText("4.4%");break;case 21:ui->lineEdit_RegongRate->setText("3.4%");break;
        case 22:ui->lineEdit_RegongRate->setText("2.8%");break;case 23:ui->lineEdit_RegongRate->setText("4%");break;
        //2014.11.22
        case 24:ui->lineEdit_RegongRate->setText("4.25%");break;case 25:ui->lineEdit_RegongRate->setText("4.25%");break;
        case 26:ui->lineEdit_RegongRate->setText("4.25%");break;case 27:ui->lineEdit_RegongRate->setText("4.25%");break;
        //2012.7.6
        case 28:ui->lineEdit_RegongRate->setText("4.5%");break;case 29:ui->lineEdit_RegongRate->setText("4.5%");break;
        case 30:ui->lineEdit_RegongRate->setText("4.5%");break;case 31:ui->lineEdit_RegongRate->setText("4.5%");break;
        //2012.6.8
        case 32:ui->lineEdit_RegongRate->setText("4.7%");break;case 33:ui->lineEdit_RegongRate->setText("4.7%");break;
        case 34:ui->lineEdit_RegongRate->setText("4.7%");break;case 35:ui->lineEdit_RegongRate->setText("4.7%");break;
        }
        switch (ui->comboBox_ReshangRate->currentIndex())
        {
        //2020.4.20
        case 0:ui->lineEdit_ReshangRate->setText("4.65%");break;
        //2020.3.20
        case 1:ui->lineEdit_ReshangRate->setText("4.75%");break;
        //2019.9.20
        case 2:ui->lineEdit_ReshangRate->setText("4.85%");break;
        //2015.10.24
        case 3:ui->lineEdit_ReshangRate->setText("5.39%");break;
        case 4:ui->lineEdit_ReshangRate->setText("5.15%");break;case 5:ui->lineEdit_ReshangRate->setText("4.17%");break;
        case 6:ui->lineEdit_ReshangRate->setText("3.43%");break;case 7:ui->lineEdit_ReshangRate->setText("4.9%");break;
        //2015.8.26
        case 8:ui->lineEdit_ReshangRate->setText("5.67%");break;case 9:ui->lineEdit_ReshangRate->setText("4.38%");break;
        case 10:ui->lineEdit_ReshangRate->setText("3.61%");break;case 11:ui->lineEdit_ReshangRate->setText("5.15%");break;
        //2015.6.28
        case 12:ui->lineEdit_ReshangRate->setText("5.94%");break;case 13:ui->lineEdit_ReshangRate->setText("4.59%");break;
        case 14:ui->lineEdit_ReshangRate->setText("3.78%");break;case 15:ui->lineEdit_ReshangRate->setText("5.4%");break;
        //2015.5.11
        case 16:ui->lineEdit_ReshangRate->setText("6.22%");break;case 17:ui->lineEdit_ReshangRate->setText("4.8%");break;
        case 18:ui->lineEdit_ReshangRate->setText("3.96%");break;case 19:ui->lineEdit_ReshangRate->setText("5.65%");break;
        //2015.3.1
        case 20:ui->lineEdit_ReshangRate->setText("6.49%");break;case 21:ui->lineEdit_ReshangRate->setText("5.01%");break;
        case 22:ui->lineEdit_ReshangRate->setText("4.13%");break;case 23:ui->lineEdit_ReshangRate->setText("5.9%");break;
        //2014.11.22
        case 24:ui->lineEdit_ReshangRate->setText("6.7%");break;case 25:ui->lineEdit_ReshangRate->setText("5.2%");break;
        case 26:ui->lineEdit_ReshangRate->setText("4.3%");break;case 27:ui->lineEdit_ReshangRate->setText("6.15%");break;
        //2012.7.6
        case 28:ui->lineEdit_ReshangRate->setText("7.21%");break;case 29:ui->lineEdit_ReshangRate->setText("5.57%");break;
        case 30:ui->lineEdit_ReshangRate->setText("4.59%");break;case 31:ui->lineEdit_ReshangRate->setText("6.55%");break;
        //2012.6.8
        case 32:ui->lineEdit_ReshangRate->setText("7.48%");break;case 33:ui->lineEdit_ReshangRate->setText("5.78%");break;
        case 34:ui->lineEdit_ReshangRate->setText("4.76%");break;case 35:ui->lineEdit_ReshangRate->setText("6.8%");break;
        }
    }
    else
    {
        switch (ui->comboBox_RegongRate->currentIndex())
        {

        //2020.4.20
        case 0:ui->lineEdit_RegongRate->setText("2.75%");break;
        //2020.3.20
        case 1:ui->lineEdit_RegongRate->setText("2.75%");break;
        //2019.9.20
        case 2:ui->lineEdit_RegongRate->setText("2.75%");break;
        //2015.10.24
        case 3:ui->lineEdit_RegongRate->setText("3.03%");break;
        case 4:ui->lineEdit_RegongRate->setText("2.89%");break;case 5:ui->lineEdit_RegongRate->setText("2.34%");break;
        case 6:ui->lineEdit_RegongRate->setText("1.93%");break;case 7:ui->lineEdit_RegongRate->setText("2.75%");break;
        //2015.8.26
        case 8:ui->lineEdit_RegongRate->setText("3.03%");break;case 9:ui->lineEdit_RegongRate->setText("2.34%");break;
        case 10:ui->lineEdit_RegongRate->setText("1.93%");break;case 11:ui->lineEdit_RegongRate->setText("2.75%");break;
        //2015.6.28
        case 12:ui->lineEdit_RegongRate->setText("3.3%");break;case 13:ui->lineEdit_RegongRate->setText("2.55%");break;
        case 14:ui->lineEdit_RegongRate->setText("2.1%");break;case 15:ui->lineEdit_RegongRate->setText("3%");break;
        //2015.5.11
        case 16:ui->lineEdit_RegongRate->setText("3.57%");break;case 17:ui->lineEdit_RegongRate->setText("2.76%");break;
        case 18:ui->lineEdit_RegongRate->setText("2.28%");break;case 19:ui->lineEdit_RegongRate->setText("3.25%");break;
        //2015.3.1
        case 20:ui->lineEdit_RegongRate->setText("3.85%");break;case 21:ui->lineEdit_RegongRate->setText("2.97%");break;
        case 22:ui->lineEdit_RegongRate->setText("2.45%");break;case 23:ui->lineEdit_RegongRate->setText("3.5%");break;
        //2014.11.22
        case 24:ui->lineEdit_RegongRate->setText("3.75%");break;case 25:ui->lineEdit_RegongRate->setText("3.75%");break;
        case 26:ui->lineEdit_RegongRate->setText("3.75%");break;case 27:ui->lineEdit_RegongRate->setText("3.75%");break;
        //2012.7.6
        case 28:ui->lineEdit_RegongRate->setText("4%");break;case 29:ui->lineEdit_RegongRate->setText("4%");break;
        case 30:ui->lineEdit_RegongRate->setText("4%");break;case 31:ui->lineEdit_RegongRate->setText("4%");break;
        //2012.6.8
        case 32:ui->lineEdit_RegongRate->setText("4.2%");break;case 33:ui->lineEdit_RegongRate->setText("4.2%");break;
        case 34:ui->lineEdit_RegongRate->setText("4.2%");break;case 35:ui->lineEdit_RegongRate->setText("4.2%");break;
        }
        switch (ui->comboBox_ReshangRate->currentIndex())
        {
        //2020.4.20
        case 0:ui->lineEdit_ReshangRate->setText("3.85%");break;
        //2020.3.20
        case 1:ui->lineEdit_ReshangRate->setText("4.05%");break;
        //2019.9.20
        case 2:ui->lineEdit_ReshangRate->setText("4.2%");break;
        //2015.10.24
        case 3:ui->lineEdit_ReshangRate->setText("5.23%");break;
        case 4:ui->lineEdit_ReshangRate->setText("4.99%");break;case 5:ui->lineEdit_ReshangRate->setText("4.04%");break;
        case 6:ui->lineEdit_ReshangRate->setText("3.33%");break;case 7:ui->lineEdit_ReshangRate->setText("4.75%");break;
        //2015.8.26
        case 8:ui->lineEdit_ReshangRate->setText("5.5%");break;case 9:ui->lineEdit_ReshangRate->setText("4.25%");break;
        case 10:ui->lineEdit_ReshangRate->setText("3.5%");break;case 11:ui->lineEdit_ReshangRate->setText("5%");break;
        //2015.6.28
        case 12:ui->lineEdit_ReshangRate->setText("5.78%");break;case 13:ui->lineEdit_ReshangRate->setText("4.46%");break;
        case 14:ui->lineEdit_ReshangRate->setText("3.68%");break;case 15:ui->lineEdit_ReshangRate->setText("5.25%");break;
        //2015.5.11
        case 16:ui->lineEdit_ReshangRate->setText("6.05%");break;case 17:ui->lineEdit_ReshangRate->setText("4.68%");break;
        case 18:ui->lineEdit_ReshangRate->setText("3.85%");break;case 19:ui->lineEdit_ReshangRate->setText("5.5%");break;
        //2015.3.1
        case 20:ui->lineEdit_ReshangRate->setText("6.33%");break;case 21:ui->lineEdit_ReshangRate->setText("4.89%");break;
        case 22:ui->lineEdit_ReshangRate->setText("4.03%");break;case 23:ui->lineEdit_ReshangRate->setText("5.75%");break;
        //2014.11.22
        case 24:ui->lineEdit_ReshangRate->setText("6.6%");break;case 25:ui->lineEdit_ReshangRate->setText("5.1%");break;
        case 26:ui->lineEdit_ReshangRate->setText("4.2%");break;case 27:ui->lineEdit_ReshangRate->setText("6%");break;
        //2012.7.6
        case 28:ui->lineEdit_ReshangRate->setText("6.77%");break;case 29:ui->lineEdit_ReshangRate->setText("5.23%");break;
        case 30:ui->lineEdit_ReshangRate->setText("4.31%");break;case 31:ui->lineEdit_ReshangRate->setText("6.15%");break;
        //2012.6.8
        case 32:ui->lineEdit_ReshangRate->setText("7.04%");break;case 33:ui->lineEdit_ReshangRate->setText("5.44%");break;
        case 34:ui->lineEdit_ReshangRate->setText("4.48%");break;case 35:ui->lineEdit_ReshangRate->setText("6.4%");break;
        }
    }
    return Deadline;
}
void Widget::on_pushButton_start_6_clicked()
{
    QString checkShangLoan= ui->lineEdit_ReshangLoan->text();
    QString checkGongLoan = ui->lineEdit_RegongLoan->text();
    QString checkShangRate= ui->lineEdit_ReshangRate->text();
    QString checkGongRate = ui->lineEdit_RegongRate->text();
    bool Justise = 0;
    double rate1 = 0,rate2 = 0;
    //判断是否为空
    if(!checkShangLoan.isEmpty()&&!checkGongLoan.isEmpty()&&!checkShangRate.isEmpty()&&!checkGongRate.isEmpty())
    {
        int checkPoint = 0;
        bool flag1 = 0,flag2 = 0,flag3 = 0, flag4 = 0;
        //判断商贷额度输入
        for(int i= 0 ;i<checkShangLoan.length();i++)
        {
            if((checkShangLoan[i]<='9'&&checkShangLoan[i]>='0')||checkShangLoan[i]=='.')
            {
                if(checkShangLoan[i]=='.')
                    checkPoint++;
                if(checkPoint>1){flag1=1;break;}
            }
            else{flag1 = 1;break;}
        }
        //判断公积金额度输入
        checkPoint = 0;
        for(int i= 0 ;i<checkGongLoan.length();i++)
        {
            if((checkGongLoan[i]<='9'&&checkGongLoan[i]>='0')||checkGongLoan[i]=='.')
            {
                if(checkGongLoan[i]=='.')
                    checkPoint++;
                if(checkPoint>1){flag2=1;break;}
            }
            else{flag2 = 1;break;}
        }
        //判断输入商贷利率
        checkPoint = 0;
        int n = 0;
        for(int i= 0 ;i<checkShangRate.length()-1;i++)
        {
            if((checkShangRate[i]<='9'&&checkShangRate[i]>='0')||checkShangRate[i]=='.')
            {
                QChar te = checkShangRate.at(i);
                if(checkShangRate[i]=='.')
                {
                    checkPoint++;
                }
                if(checkPoint>=2){flag3=1;break;}
                if(checkShangRate[i]!='.')
                {
                    rate1 = rate1*10+(te.toLatin1()-'0');
                    n++;
                }
            }
            else{flag3 = 1;break;}
        }
        rate1 /= pow(10,n+1);
        if(rate1 == 0)
            flag3 = 1;

        //判断输入公基金贷款利率
        checkPoint = 0;
        int k = 0;
        for(int i= 0 ;i<checkGongRate.length()-1;i++)
        {
            if((checkGongRate[i]<='9'&&checkGongRate[i]>='0')||checkGongRate[i]=='.')
            {
                QChar te = checkGongRate.at(i);
                if(checkGongRate[i]=='.')
                {
                    checkPoint++;
                }
                if(checkPoint>=2){flag4=1;break;}
                if(checkGongRate[i]!='.')
                {
                    rate2 = rate2*10+(te.toLatin1()-'0');
                    k++;
                }
            }
            else{flag4 = 1;break;}
        }
        rate2 /= pow(10,k+1);
        if(rate2 == 0)
            flag4 = 1;

        if(flag1 == 1 || flag3 == 1 || flag2 == 1 || flag4 == 1)
        {
            if(flag1 == 1 && flag1 == flag2 && flag2 == flag3 && flag3 == flag4)
            {
                QMessageBox *eorr = new QMessageBox(this);
                eorr->warning(this,"错误","您的输入的房屋商业贷款额、公积金贷款额、及其对应的利率有误",QMessageBox::Ok);
            }
            else if(flag1 == 1 &&flag2 == 0 && flag3 == 0 && flag4 == 0)
            {
               QMessageBox *eorr = new QMessageBox(this);
               eorr->warning(this,"错误","您的输入房屋商业贷款额有误",QMessageBox::Ok);
            }
            else if(flag1 == 0 &&flag2 == 1 && flag3 == 0 && flag4 == 0)
            {
                 QMessageBox *eorr = new QMessageBox(this);
                 eorr->warning(this,"错误","您的输入房屋公积金贷款额有误",QMessageBox::Ok);
            }
            else if(flag1 == 0 &&flag2 == 0 && flag3 == 1 && flag4 == 0)
            {
                  QMessageBox *eorr = new QMessageBox(this);
                  eorr->warning(this,"错误","您的输入房屋商业贷款利率不合法，请重新输入",QMessageBox::Ok);
            }
            else if(flag1 == 0 &&flag2 == 0 && flag3 == 0 && flag4 == 1)
            {
                QMessageBox *eorr = new QMessageBox(this);
                eorr->warning(this,"错误","您的输入房屋公积金贷款利率不合法，请重新输入",QMessageBox::Ok);
            }
            else
            {
                QMessageBox *eorr = new QMessageBox(this);
                eorr->warning(this,"错误","您的输入有误，请重新输入",QMessageBox::Ok);
            }
        }
        else
        {
            Justise = 1;
        }
    }
    //如果有输入有空
    else
    {
        if(checkShangLoan.isEmpty()&&checkGongLoan.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入商业贷款额、公积金贷款额",QMessageBox::Ok);
        }
        else if(checkShangLoan.isEmpty()&&!checkGongLoan.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入商业贷款额",QMessageBox::Ok);
        }
        else if(!checkShangLoan.isEmpty()&&checkGongLoan.isEmpty())
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","请输入公积金贷款额",QMessageBox::Ok);
        }
        else
        {
            QMessageBox *eorr = new QMessageBox(this);
            eorr->warning(this,"错误","您未位输入的选项，请仔细检查",QMessageBox::Ok);
        }
    }

    if(Justise ==1 )
    {
        //房屋贷款总额
        double loan1 = ui->lineEdit_ReshangLoan->text().toDouble();
        loan1 *= 10000;
        double loan2 = ui->lineEdit_RegongLoan->text().toDouble();
        loan2 *= 10000;

        ui->lineEdit_ReshangLoan->setText(ui->lineEdit_ReshangLoan->text());
        ui->lineEdit_RegongLoan->setText(ui->lineEdit_RegongLoan->text());
        short Deadline =on_comboBox_Redeadline_5_currentIndexChanged(ui->comboBox_Redeadline_5->currentIndex());

//        还款日期下拉框
        ui->comboBox_Redeadline_5->setCurrentIndex(ui->comboBox_Redeadline_5->currentIndex());

//        商贷利率下拉框
        ui->comboBox_ReshangRate->setCurrentIndex(ui->comboBox_ReshangRate->currentIndex());
        ui->lineEdit_ReshangRate->setText(QString("%1%").arg(rate1*100));

//        公积金贷款利率下拉框
        ui->comboBox_RegongRate->setCurrentIndex(ui->comboBox_RegongRate->currentIndex());
        ui->lineEdit_RegongRate->setText(QString("%1%").arg(rate2*100));

//        贷款类型下拉框
        ui->comboBox_Retype_3->setCurrentIndex(ui->comboBox_Retype_3->currentIndex());


        house_loan A(loan1,rate1,Deadline);

        house_loan B(loan2,rate2,Deadline);

        //等额本息
        if(ui->radioButton_Rebenxi_3->isChecked())
        {

            ui->radioButton_Rebenxi_3->clicked();
            ui->label_Remonthpay_change_4->setText("每月月供:");
            ui->widget_Redecline_4->hide();

            A.Average_Capital_Plus_Interest();
            B.Average_Capital_Plus_Interest();
            //月供
            ui->label_Remonthpay_4->setText(QString::number(A.Getmonthpayment()+B.Getmonthpayment(), 'f', 1));
            //贷款总额
            ui->label_ReLoan_4->setText(QString::number(A.Getloan()+B.Getloan(), 'f', 1));
            //支付利息
            ui->label_Reinterest_4->setText(QString::number(A.Getinterest()+B.Getinterest(), 'f', 1));
            //还款总额
            ui->label_Retotalpay_4->setText(QString::number(A.Getendpayment()+B.Getendpayment(), 'f', 1));
            //还款月数
            ui->label_Redate_4->setText(QString::number(A.Getdeadline()+B.Getdeadline(), 'f', 1));
        }
        //等额本金
        else if(ui->radioButton_Rebenjin_3->isChecked())
        {
            ui->radioButton_Rebenjin_3->clicked();
            ui->widget_Redecline_4->show();
            ui->label_Remonthpay_change_4->setText("首月月供:");
            A.Average_capital();
            B.Average_capital();
            qDebug()<<A.Getmonthpayment();
            //月供
            ui->label_Remonthpay_4->setText(QString::number(A.Getmonthpayment()+B.Getmonthpayment(), 'f', 1));
            //每月递减
            ui->label_Redecline_4->setText(QString::number(A.Getmonth_cut()+B.Getmonth_cut(), 'f', 1));
            //贷款总额
            ui->label_ReLoan_4->setText(QString::number(A.Getloan()+B.Getloan(), 'f', 1));
            //支付利息
            ui->label_Reinterest_4->setText(QString::number(A.Getinterest()+B.Getinterest(), 'f', 1));
            //还款总额
            ui->label_Retotalpay_4->setText(QString::number(A.Getendpayment()+B.Getendpayment(), 'f', 1));
            //还款月数
            ui->label_Redate_4->setText(QString::number(A.Getdeadline()+B.Getdeadline(), 'f', 1));
        }
        ui->stackedWidget->setCurrentWidget(ui->Calater_3_Result);
    }
}
void Widget::on_pushButton_cance_6_clicked()
{
    int ret = QMessageBox::question(this,"Question","是否需要清除数据？");
    if(ret == QMessageBox::Yes)
    {
        ui->stackedWidget->setCurrentWidget(ui->Calater_3);
        ui->lineEdit_shangLoan->clear();
        ui->lineEdit_shangRate->setText("4.65%");
        ui->comboBox_shangRate->setCurrentText(0);
        ui->radioButton_benxi_3->clicked();
        ui->lineEdit_gongLoan->clear();
        ui->lineEdit_gongRate->setText("4.65%");
        ui->comboBox_gongRate->setCurrentText(0);
        ui->comboBox_Redeadline_3->setCurrentText(0);

        ui->lineEdit_ReshangLoan->clear();
        ui->lineEdit_ReshangRate->setText("4.65%");
        ui->comboBox_ReshangRate->setCurrentText(0);
        ui->radioButton_Rebenxi_3->clicked();
        ui->lineEdit_RegongLoan->clear();
        ui->lineEdit_RegongRate->setText("4.65%");
        ui->comboBox_RegongRate->setCurrentText(0);
        ui->comboBox_Redeadline_5->setCurrentText(0);
    }
    else if(ret == QMessageBox::No)
    {
        //保持不动
        //忽略事件，事件继续给父组件传递
    }
}
/******************************************-----End_Result_NO.3 组合型贷款-----**************************************************/

