// DiscountManager.h
#ifndef DISCOUNTMANAGER_H
#define DISCOUNTMANAGER_H

#include "Discount.h"
#include "PromoCodeDiscount.h"
#include "LoyaltyDiscount.h"
#include <vector>
#include <string>

class DiscountManager {
private:
    std::vector<PromoCodeDiscount> promoCodes;
public:
    DiscountManager();

    void loadDiscounts(const std::string& filename);
    bool applyPromoCode(const std::string& code, double& discountedPrice, double originalPrice);
    double applyLoyaltyDiscount(int loyaltyLevel, double originalPrice);
};

#endif // DISCOUNTMANAGER_H

