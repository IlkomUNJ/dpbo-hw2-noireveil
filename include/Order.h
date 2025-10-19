#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>

class Order {
public:
    enum Status { PAID, COMPLETED, CANCELLED };

private:
    friend class boost::serialization::access;
    int id;
    int buyerId;
    int sellerId;
    std::string date;
    std::map<int, int> items;
    double totalAmount;
    Status status;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        (void)version;
        ar & id;
        ar & buyerId;
        ar & sellerId;
        ar & date;
        ar & items;
        ar & totalAmount;
        ar & status;
    }

public:
    Order(int id = 0, int bId = 0, int sId = 0, std::string date = "", 
          std::map<int, int> items = {}, double total = 0.0);
    int getId() const;
    int getBuyerId() const;
    int getSellerId() const;
    std::string getDate() const;
    Status getStatus() const;
    double getTotal() const;
    const std::map<int, int>& getItems() const;
    void setStatus(Status newStatus);
    void printSummary(const std::string& buyerName, const std::string& sellerName) const;
    std::string getStatusString() const;
};
#endif