#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>

/**
 * Abstract base class for payment methods.
 */
class Payment {
public:
    virtual ~Payment() = default;

    // This method will be used to process the payment
    // and also prompt user for relevant details (like card number, phone, etc.)
    virtual bool processPayment() = 0;

    // Return a short description of the payment method,
    // e.g. "Credit Card (****1234)" or "E-wallet (+86...)"
    virtual std::string getPaymentDescription() const = 0;
};

#endif // PAYMENT_H
