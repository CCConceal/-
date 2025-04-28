#include "CashPayment.h"

bool CashPayment::processPayment()
{
    std::cout << "=== Cash on Delivery ===\n";
    std::cout << "Please enter a contact phone number (for confirmation): ";
    std::getline(std::cin, contactNumber);

    std::cout << "Cash on Delivery selected. Our staff will contact you via phone.\n\n";
    return true;
}

std::string CashPayment::getPaymentDescription() const
{
    std::string masked = contactNumber;
    if (masked.size() > 3) {
        for (size_t i = 0; i < masked.size() - 3; ++i) {
            masked[i] = '*';
        }
    }
    return "Cash on Delivery (contact: " + masked + ")";
}
