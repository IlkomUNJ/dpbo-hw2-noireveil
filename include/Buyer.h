#ifndef BUYER_H
#define BUYER_H

#include <string>
#include <iostream>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/export.hpp>

class Buyer {
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        (void)version;
        ar & id;
        ar & name;
        ar & username;
        ar & password;
        ar & bankAccountId;
    }

protected:
    int id;
    std::string name;
    std::string username;
    std::string password;
    int bankAccountId;

public:
    Buyer(int id = 0, std::string name = "", std::string user = "", std::string pass = "", int bId = 0);
    virtual ~Buyer() {}
    int getId() const;
    std::string getName() const;
    std::string getUsername() const;
    int getBankAccountId() const;
    bool checkPassword(const std::string& pass) const;
};

BOOST_CLASS_EXPORT_KEY(Buyer)

#endif