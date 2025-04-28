// LoyaltyDiscount.cpp
#include "LoyaltyDiscount.h"

LoyaltyDiscount::LoyaltyDiscount(int level)
    : loyaltyLevel(level) {}

double LoyaltyDiscount::calculateDiscountedPrice(double price) const {

    return price * (1 - (loyaltyLevel * 10));
}
