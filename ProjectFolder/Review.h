// Review.h
#ifndef REVIEW_H
#define REVIEW_H

#include <string>

class Review {
protected:
    std::string reviewID;
    std::string bookingID;
    std::string username;
    std::string hotelName;
public:
    Review(const std::string& rid, const std::string& bid, const std::string& user, const std::string& hotel);
    virtual void displayReview() const = 0;
    virtual ~Review() {}
    virtual void saveToFile(std::ofstream& ofs) const = 0; // 新增方法


    // Getter methods
    std::string getReviewID() const;
    std::string getBookingID() const;
    std::string getUsername() const;
    std::string getHotelName() const;

    // Static function for generating ReviewID
    static std::string generateReviewID();

    // Static method to set reviewCounter
    static void setReviewCounter(int counter);
private:
    static int reviewCounter; 
};

#endif // REVIEW_H
