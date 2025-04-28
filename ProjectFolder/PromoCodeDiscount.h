// PromoCodeDiscount.h
#ifndef PROMOCODEDISCOUNT_H
#define PROMOCODEDISCOUNT_H

#include "Discount.h"
#include <string>

class PromoCodeDiscount : public Discount {
private:
    std::string promoCode;
    double discountPercentage;
public:
    PromoCodeDiscount(const std::string& code, double percentage);

    double calculateDiscountedPrice(double price) const override;
    bool isPromoCodeValid(const std::string& code);
};

#endif // PROMOCODEDISCOUNT_H
