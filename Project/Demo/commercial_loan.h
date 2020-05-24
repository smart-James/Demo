#ifndef COMMERCIAL_LOAN_H
#define COMMERCIAL_LOAN_H

#include "house_loan.h"


class commercial_loan : virtual public house_loan
{
    public:
        commercial_loan(double value,double area,double percentage,short deadline,double rate);


        double Getvalue() { return m_value; }
        void Setvalue(double val) { m_value = val; }
        double Getarea() { return m_area; }
        void Setarea(double val) { m_area = val; }
        double Getpercentage() { return m_percentage; }
        void Setpercentage(double val) { m_percentage = val; }
        double Getfirstpayment() { return m_firstpayment; }
        void Setfirstpayment(double val) { m_firstpayment = val; }

    protected:

    private:
        //单价
        double m_value;
        //面积
        double m_area;
        //贷款成数
        double m_percentage;
        //首付
        double m_firstpayment;
};
commercial_loan::commercial_loan(double value,double area,double percentage,short deadline,double rate):house_loan(value*area*percentage,rate,deadline)
{
    this->m_value = value;
    this->m_area = area;
    this->m_percentage = percentage;
    this->m_firstpayment = value*area - value*area*percentage;
}

#endif // COMMERCIAL_LOAN_H
