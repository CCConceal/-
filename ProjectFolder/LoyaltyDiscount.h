// LoyaltyDiscount.h
#ifndef LOYALTYDISCOUNT_H
#define LOYALTYDISCOUNT_H

#include "Discount.h"

class LoyaltyDiscount : public Discount {
private:
    int loyaltyLevel;
public:
    LoyaltyDiscount(int level);

    double calculateDiscountedPrice(double price) const override;
};

#endif // LOYALTYDISCOUNT_H
