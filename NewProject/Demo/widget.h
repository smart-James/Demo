#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void closeEvent(QCloseEvent *ev)override;
    void keyPressEvent(QKeyEvent *)override;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result)override;

private slots:


    void on_pushButton_start_clicked();

    void on_comboBox_type_currentIndexChanged(int index);

    void on_comboBox_calaterWay_currentIndexChanged(int index);

    void on_comboBox_rate_currentIndexChanged(int index);

    short on_comboBox_deadline_currentIndexChanged(int index);

    void on_comboBox_Retype_1_currentIndexChanged(int index);

    void on_comboBox_RecalaterWay_1_currentIndexChanged(int index);


    short on_comboBox_Redeadline_1_currentIndexChanged(int index);

    void on_comboBox_Rerate_1_currentIndexChanged(int index);

    void on_pushButton_Restart_1_clicked();

    void on_comboBox_type_2_currentIndexChanged(int index);

    void on_comboBox_calaterWay_2_currentIndexChanged(int index);

    short on_comboBox_deadline_2_currentIndexChanged(int index);

    void on_comboBox_rate_2_currentIndexChanged(int index);

    void on_pushButton_start_2_clicked();

    void on_comboBox_Retype_2_currentIndexChanged(int index);

    void on_comboBox_RecalaterWay_2_currentIndexChanged(int index);

    short on_comboBox_Redeadline_2_currentIndexChanged(int index);

    void on_comboBox_Rerate_2_currentIndexChanged(int index);

    void on_pushButton_start_4_clicked();

    void on_comboBox_type_3_currentIndexChanged(int index);

    void on_comboBox_shangRate_currentIndexChanged(int index);

    void on_comboBox_gongRate_currentIndexChanged(int index);

    short on_comboBox_Redeadline_3_currentIndexChanged(int index);

    void on_pushButton_start_3_clicked();

    void on_comboBox_Retype_3_currentIndexChanged(int index);

    void on_comboBox_ReshangRate_currentIndexChanged(int index);

    void on_comboBox_RegongRate_currentIndexChanged(int index);

    short on_comboBox_Redeadline_5_currentIndexChanged(int index);

    void on_pushButton_start_6_clicked();

    void on_pushButton_cance_clicked();

    void on_pushButton_Recance_1_clicked();

    void on_pushButton_cance_2_clicked();

    void on_pushButton_cance_4_clicked();

    void on_pushButton_cance_3_clicked();

    void on_pushButton_cance_6_clicked();

    void Clear();

    double Interest(int typeIn,int dateIn,int rateIn);

private:
    Ui::Widget *ui;
    int m_nBorderWidth; //m_nBorder表示鼠标位于边框缩放范围的宽度
};
#endif // WIDGET_H
