#include "Buyer.h"
#include <fstream>

Buyer::Buyer(int id, std::string name, std::string user, std::string pass, int bId)
    : id(id), name(name), username(user), password(pass), bankAccountId(bId) {}

int Buyer::getId() const { return id; }
std::string Buyer::getName() const { return name; }
std::string Buyer::getUsername() const { return username; }
int Buyer::getBankAccountId() const { return bankAccountId; }

bool Buyer::checkPassword(const std::string& pass) const {
    return password == pass;
}