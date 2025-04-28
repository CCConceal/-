#include "EWalletPayment.h"

bool EWalletPayment::processPayment()
{
    std::cout << "=== E-wallet Payment ===\n";
    std::cout << "Please enter your E-wallet phone number: ";
    std::getline(std::cin, phoneNumber);

    std::cout << "Processing e-wallet payment...\n";
    // ...
    std::cout << "E-wallet Payment successful.\n\n";

    return true;
}

std::string EWalletPayment::getPaymentDescription() const
{
    // mask phone number
    std::string masked = phoneNumber;
    if (masked.size() > 3) {
        for (size_t i = 0; i < masked.size() - 3; ++i) {
            masked[i] = '*';
        }
    }
    return "E-wallet (" + masked + ")";
}
