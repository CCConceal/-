// StarReview.h
#ifndef STARREVIEW_H
#define STARREVIEW_H

#include "Review.h"

class StarReview : public Review {
private:
    int rating; // 1 to 5
public:
    StarReview(const std::string& rid, const std::string& bid, const std::string& user, const std::string& hotel, int rate);

    void displayReview() const override;
    void saveToFile(std::ofstream& ofs) const override;

    // Getter method
    int getRating() const;
};

#endif // STARREVIEW_H
