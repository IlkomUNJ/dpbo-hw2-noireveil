#include "BankTransaction.h"
#include <iostream>

BankTransaction::BankTransaction(std::string d, std::string t, double a)
    : date(d), type(t), amount(a) {}

void BankTransaction::print() const {
    std::cout << "  - Tanggal: " << date << ", Tipe: " << type 
              << ", Jumlah: " << amount << std::endl;
}