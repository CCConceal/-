// ReviewManager.cpp
#include "ReviewManager.h"
#include "TextReview.h"
#include "StarReview.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

ReviewManager::ReviewManager() {}

void ReviewManager::loadReviews(const std::string& filename) {
    std::cerr << "[ReviewManager::loadReviews] Starting to load reviews from: " << filename << "\n";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ReviewManager::loadReviews] Unable to open reviews file: " << filename << ". Reviews will not be loaded.\n";
        return; // Logging errors only, not throwing exceptions
    }

    std::string line;
    int lineNumber = 0;

    // Read and skip table header rows
    if (std::getline(file, line)) {
        lineNumber++;
        std::cerr << "[ReviewManager::loadReviews] Skipping header line: " << line << "\n";

        /*reference:utf8-bom-strip. (n.d.). GitHub Repository. Retrieved December 29, 2024, from https://github.com/zer4tul/utf8-bom-strip
        */
        if (line.size() >= 3 &&
            static_cast<unsigned char>(line[0]) == 0xEF &&
            static_cast<unsigned char>(line[1]) == 0xBB &&
            static_cast<unsigned char>(line[2]) == 0xBF) {
            std::cerr << "[ReviewManager::loadReviews] Detected and removing BOM.\n";
            line.erase(0, 3);
        }
    }

    while (std::getline(file, line)) {
        lineNumber++;
        std::cerr << "[ReviewManager::loadReviews] Processing line " << lineNumber << ": " << line << "\n";
        try {
            std::istringstream iss(line);
            std::string reviewID, bookingID, username, hotelName, reviewType, contentOrRating;


            std::getline(iss, reviewID, ',');
            std::getline(iss, bookingID, ',');
            std::getline(iss, username, ',');
            std::getline(iss, hotelName, ',');
            std::getline(iss, reviewType, ',');
            std::getline(iss, contentOrRating, ',');


            std::cerr << "[ReviewManager::loadReviews] Parsed fields - reviewID: " << reviewID
                << ", bookingID: " << bookingID
                << ", username: " << username
                << ", hotelName: " << hotelName
                << ", reviewType: " << reviewType
                << ", contentOrRating: " << contentOrRating << "\n";

            // Instantiate the corresponding derived class according to reviewType
            std::shared_ptr<Review> review;
            if (reviewType == "Star") {
                int rating = std::stoi(contentOrRating);
                review = std::make_shared<StarReview>(reviewID, bookingID, username, hotelName, rating);
            }
            else if (reviewType == "Text") {
                review = std::make_shared<TextReview>(reviewID, bookingID, username, hotelName, contentOrRating);
            }
            else {
                std::cerr << "[ReviewManager::loadReviews] Unknown review type '" << reviewType
                    << "' in line " << lineNumber << ". Skipping.\n";
                continue;
            }

            // Add to comment list
            reviews.emplace_back(review);
            std::cerr << "[ReviewManager::loadReviews] Added new review: " << reviewID << "\n";
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "[ReviewManager::loadReviews] Invalid argument in line " << lineNumber << ":\n"
                << "  " << line << "\n"
                << "  Reason: " << e.what() << "\n";
            continue;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "[ReviewManager::loadReviews] Out of range error in line " << lineNumber << ":\n"
                << "  " << line << "\n"
                << "  Reason: " << e.what() << "\n";
            continue;
        }
        catch (...) {
            std::cerr << "[ReviewManager::loadReviews] Unknown exception while parsing line " << lineNumber << ":\n"
                << "  " << line << "\n";
            continue;
        }
    }

    file.close();
    std::cerr << "[ReviewManager::loadReviews] Finished loading: " << filename << "\n";
    std::cerr << "========== The above is [ReviewManager] debugging information (the actual system will not show, here for the convenience of showing the error handling function of this project) ==========\n\n";
}

void ReviewManager::saveReview(const std::shared_ptr<Review>& review, const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "[ReviewManager::saveReview] Unable to open reviews file for appending: " << filename << "\n";
        return;
    }

    // Write different contents according to the Review type
    review->saveToFile(file);
    file.close();
}

void ReviewManager::displayReviewsByHotel(const std::string& hotelName) const {
    for (const auto& review : reviews) {
        if (review->getHotelName() == hotelName) {
            review->displayReview();
            std::cout << "-------------------------\n";
        }
    }
}
