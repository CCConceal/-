#include "CreditCardPayment.h"

bool CreditCardPayment::processPayment()
{
    std::cout << "=== Credit Card Payment(Just demo, No validation) ===\n";
    std::cout << "Please enter card holder name: ";
    std::getline(std::cin, cardHolderName);

    std::cout << "Please enter credit card number (16 digits): ";
    std::getline(std::cin, cardNumber);

    std::cout << "Please enter expiration date (MM/YY): ";
    std::getline(std::cin, expirationDate);

    std::cout << "Please enter CVV (3 digits): ";
    std::getline(std::cin, cvv);

    // For now we won't do real validation, just simulate success.

    std::cout << "Processing credit card payment...\n";

    std::cout << "Credit Card Payment successful.\n\n";

    return true;
}

std::string CreditCardPayment::getPaymentDescription() const
{
    // Just mask the card number except last 4 digits
    std::string masked = cardNumber;
    if (masked.size() > 4) {
        for (size_t i = 0; i < masked.size() - 4; ++i) {
            masked[i] = '*';
        }
    }
    return "Credit Card (" + masked + ")";
}
