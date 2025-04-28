// PromoCodeDiscount.cpp
#include "PromoCodeDiscount.h"

PromoCodeDiscount::PromoCodeDiscount(const std::string& code, double percentage)
    : promoCode(code), discountPercentage(percentage) {}

double PromoCodeDiscount::calculateDiscountedPrice(double price) const {
    return price * (1 - discountPercentage / 100.0);
}

bool PromoCodeDiscount::isPromoCodeValid(const std::string& code) {
    return promoCode == code;
}
