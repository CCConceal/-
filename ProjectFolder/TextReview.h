// TextReview.h
#ifndef TEXTREVIEW_H
#define TEXTREVIEW_H

#include "Review.h"

class TextReview : public Review {
private:
    std::string content;
public:
    TextReview(const std::string& rid, const std::string& bid, const std::string& user, const std::string& hotel, const std::string& cont);

    void displayReview() const override;
    void saveToFile(std::ofstream& ofs) const override;
    // Getter method
    std::string getContent() const;
};

#endif // TEXTREVIEW_H
