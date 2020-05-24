#ifndef HOUSE_LOAN_H
#define HOUSE_LOAN_H

#include <cmath>

/*
 *Fonction : 等额本息（Average_Capital_Plus_Interest）
 *Fonction : 等额本金（Average_capital）
 *
 *param: value：单价, area：总面积
 *param: Percentage: 贷款百分比
 *param: rate: 利率
 *param: deadline: 还款期数（月份）
 *param: loans: 贷款总数
*/
class house_loan
{
    public:
        house_loan()
        {
            this->m_deadline=-1;
            this->m_loan=-1;
            this->m_rate=-1;
            this->m_endpayment = -1;
            this->m_monthpayment = -1;
            this->m_month_cut=-1;
            this->m_interest=-1;
        }
        house_loan(double loan, double rate,short deadline)
        {
            this->m_loan=loan;
            this->m_rate=rate;
            this->m_deadline=deadline;
            this->m_endpayment = -1;
            this->m_monthpayment = -1;
            this->m_month_cut=-1;
            this->m_interest=-1;
        }

        double Getloan() { return m_loan; }
        void Setloan(double val) { m_loan = val; }
        double Getrate() { return m_rate; }
        void Setrate(double val) { m_rate = val; }
        short Getdeadline() { return m_deadline; }
        void Setdeadline(double val) { m_deadline = val; }

        double Getendpayment() { return m_endpayment; }
        double Getinterest() { return m_interest; }
        double Getmonthpayment() { return m_monthpayment; }
        double Getmonth_cut() { return m_month_cut; }
        //等额本息
        void Average_Capital_Plus_Interest();
        //等额本金
        void Average_capital();

    protected:

    private:
        double m_loan;//贷款总额
        double m_rate;//贷款利率
        double m_endpayment;//还款总额
        double m_interest;//利息
        double m_monthpayment;// 月供 或 首月月供
        double m_month_cut;//每月递减
        short m_deadline;//还款时间
};
void house_loan::Average_Capital_Plus_Interest()
{   //等额本息:即借款人每月按照固定金额还款
    //每月还款金额：【贷款本金×月利率×（1+月利率）^还款月数】÷【（1+月利率）^还款月数-1】
    this->m_monthpayment=(m_loan*(m_rate/12)*pow((1+m_rate/12),m_deadline))/(pow((1+m_rate/12),m_deadline)-1);
    this->m_endpayment = m_monthpayment * m_deadline;
    this->m_interest = m_endpayment - m_loan;
}
void house_loan::Average_capital()
{
    //等额本金
    //每月还款金额=（贷款本金/还款月数）+（本金-已归还本金累计额）×每月利率
    //还款总额=（还款月数+1）×贷款额×月利率/2+ 贷款额
    double capital = m_loan / m_deadline;//每月本金
    //每月递减
    m_month_cut = m_loan*(m_rate/12)- (m_loan - capital)*(m_rate/12);
    m_endpayment = (m_deadline+1)*m_loan*(m_rate/12/2)+m_loan;
    m_interest = m_endpayment - m_loan;
    m_monthpayment = capital+m_loan*(m_rate/12);
}
#endif // HOUSE_LOAN_H
