#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>

class Item {
private:
    friend class boost::serialization::access;
    int id;
    int sellerId;
    std::string name;
    int stock;
    double price;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        (void)version;
        ar & id;
        ar & sellerId;
        ar & name;
        ar & stock;
        ar & price;
    }

public:
    Item(int id = 0, int sellerId = 0, std::string name = "", int stock = 0, double price = 0.0);
    int getId() const;
    int getSellerId() const;
    std::string getName() const;
    int getStock() const;
    double getPrice() const;
    void setStock(int newStock);
    void setPrice(double newPrice);
    void print() const;
};
#endif