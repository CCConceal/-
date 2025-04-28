
// Discount.h
#ifndef DISCOUNT_H
#define DISCOUNT_H

class Discount {
public:
    virtual double calculateDiscountedPrice(double price) const = 0;
    virtual ~Discount() {}
};

#endif // DISCOUNT_H
