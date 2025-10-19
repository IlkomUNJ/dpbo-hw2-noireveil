#ifndef BANKTRANSACTION_H
#define BANKTRANSACTION_H

#include <string>
#include <iostream>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>

class BankTransaction {
public:
    friend class boost::serialization::access;
    std::string date;
    std::string type;
    double amount;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        (void)version;
        ar & date;
        ar & type;
        ar & amount;
    }

public:
    BankTransaction(std::string d = "", std::string t = "", double a = 0.0);
    void print() const;
};
#endif