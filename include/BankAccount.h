#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <string>
#include <vector>
#include <iostream>
#include "BankTransaction.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include "boost/date_time/gregorian/greg_serialize.hpp"

class BankAccount {
private:
    friend class boost::serialization::access;
    int id;
    std::string ownerName;
    double balance;
    std::vector<BankTransaction> history;
    boost::gregorian::date lastTransactionDate;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        (void)version;
        ar & id;
        ar & ownerName;
        ar & balance;
        ar & history;
        ar & lastTransactionDate;
    }

public:
    BankAccount(int id = 0, std::string name = "", double initialBalance = 0.0);
    int getId() const;
    double getBalance() const;
    std::string getOwnerName() const;
    bool withdraw(double amount);
    void deposit(double amount);
    void listCashFlowToday() const;
    void listCashFlowMonth() const;
    bool isDormant() const;
    int getTransactionsToday() const;
};
#endif