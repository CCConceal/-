#ifndef EWALLETPAYMENT_H
#define EWALLETPAYMENT_H

#include "Payment.h"
#include <string>
#include <iostream>

class EWalletPayment : public Payment {
private:
    std::string phoneNumber;  // or e-wallet account
public:
    EWalletPayment() = default;

    bool processPayment() override;
    std::string getPaymentDescription() const override;
};

#endif // EWALLETPAYMENT_H

