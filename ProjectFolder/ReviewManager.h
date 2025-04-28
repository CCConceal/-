// ReviewManager.h
#ifndef REVIEWMANAGER_H
#define REVIEWMANAGER_H

#include "Review.h" 
#include <memory>
#include <vector>
#include <string>

class ReviewManager {
private:
    std::vector<std::shared_ptr<Review>> reviews;
public:
    ReviewManager();

    void loadReviews(const std::string& filename);
    void saveReview(const std::shared_ptr<Review>& review, const std::string& filename);
    void displayReviewsByHotel(const std::string& hotelName) const;
};

#endif // REVIEWMANAGER_H
