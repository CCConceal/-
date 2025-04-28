// DiscountManager.cpp
#include "DiscountManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

DiscountManager::DiscountManager() {
    // Initial load if necessary
}

void DiscountManager::loadDiscounts(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open discounts file: " << filename << "\n";
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string promoCode;
        double discountPercentage;
        std::getline(iss, promoCode, ',');
        iss >> discountPercentage;
        promoCodes.emplace_back(PromoCodeDiscount(promoCode, discountPercentage));
    }
    file.close();
    std::cout << "\n\n";
    std::cerr << "========== The above is [DiscountlManager] debugging information (the actual system will not show, here for the convenience of showing the error handling function of this project) ==========\n\n";
}

bool DiscountManager::applyPromoCode(const std::string& code, double& discountedPrice, double originalPrice) {
    for (auto& promo : promoCodes) {
        if (promo.isPromoCodeValid(code)) {
            discountedPrice = promo.calculateDiscountedPrice(originalPrice);
            return true;
        }
    }
    return false;
}

double DiscountManager::applyLoyaltyDiscount(int loyaltyLevel, double originalPrice) {
    LoyaltyDiscount loyaltyDiscount(loyaltyLevel);
    return loyaltyDiscount.calculateDiscountedPrice(originalPrice);
}
