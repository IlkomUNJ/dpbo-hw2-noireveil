#ifndef BANK_H
#define BANK_H

#include "BankAccount.h"
#include <vector>
#include <map>
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

class Bank {
private:
    friend class boost::serialization::access;
    std::vector<BankAccount> accounts;
    int nextAccountId;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        (void)version;
        ar & accounts;
        ar & nextAccountId;
    }

public:
    Bank();
    BankAccount* createAccount(std::string ownerName, double initialDeposit);
    BankAccount* getAccount(int id);
    BankAccount* getAccountByOwner(std::string name);

    void listTransactionsLastWeek() const;
    void listAllCustomers() const;
    void listDormantAccounts() const;
    void listTopNUsersToday(int n) const;
};
#endif