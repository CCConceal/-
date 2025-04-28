#ifndef CASHPAYMENT_H
#define CASHPAYMENT_H

#include "Payment.h"
#include <string>
#include <iostream>

class CashPayment : public Payment {
private:
    std::string contactNumber; // for confirmation or something
public:
    CashPayment() = default;

    bool processPayment() override;
    std::string getPaymentDescription() const override;
};

#endif // CASHPAYMENT_H
