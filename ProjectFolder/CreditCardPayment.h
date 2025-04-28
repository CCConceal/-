#ifndef CREDITCARDPAYMENT_H
#define CREDITCARDPAYMENT_H

#include "Payment.h"
#include <string>
#include <iostream>

class CreditCardPayment : public Payment {
private:
    std::string cardNumber;
    std::string cardHolderName;
    std::string expirationDate;
    std::string cvv;
public:
    // You can add a constructor if you want to initialize something
    CreditCardPayment() = default;

    // This method will prompt the user for card info, then "process" it
    bool processPayment() override;

    // Return something like "Credit Card (****1234)"
    std::string getPaymentDescription() const override;
};

#endif // CREDITCARDPAYMENT_H
