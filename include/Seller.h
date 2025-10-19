#ifndef SELLER_H
#define SELLER_H

#include "Buyer.h"
#include <vector>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/export.hpp>

class Seller : public Buyer {
private:
    friend class boost::serialization::access;
    std::string storeName;
    std::vector<int> itemIds;
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        (void)version;
        ar & boost::serialization::base_object<Buyer>(*this);
        ar & storeName;
        ar & itemIds;
    }

public:
    Seller(int id = 0, std::string name = "", std::string user = "", 
           std::string pass = "", int bId = 0, std::string storeName = "");
    virtual ~Seller() {}
    
    std::string getStoreName() const;
    const std::vector<int>& getItemIds() const;
    void addItem(int itemId); 
    void removeItem(int itemId);
};

BOOST_CLASS_EXPORT_KEY(Seller)

#endif