#include "BankAccount.h"
#include "Utils.h"
#include <iostream>

BankAccount::BankAccount(int id, std::string name, double initialBalance)
    : id(id), ownerName(name), balance(initialBalance), 
      lastTransactionDate(Utils::getCurrentBoostDate()) {}

int BankAccount::getId() const { return id; }
double BankAccount::getBalance() const { return balance; }
std::string BankAccount::getOwnerName() const { return ownerName; }

bool BankAccount::withdraw(double amount) {
    if (amount <= 0) {
        std::cout << "Jumlah penarikan tidak valid.\n";
        return false;
    }
    if (balance < amount) {
        std::cout << "Saldo tidak mencukupi.\n";
        return false;
    }
    balance -= amount;
    lastTransactionDate = Utils::getCurrentBoostDate();
    history.emplace_back(Utils::getCurrentDate(), "DEBIT", amount); 
    std::cout << "Penarikan " << amount << " berhasil. Saldo baru: " << balance << std::endl;
    return true;
}

void BankAccount::deposit(double amount) {
    if (amount <= 0) {
        std::cout << "Jumlah deposit tidak valid.\n";
        return;
    }
    balance += amount;
    lastTransactionDate = Utils::getCurrentBoostDate();
    history.emplace_back(Utils::getCurrentDate(), "CREDIT", amount);
    std::cout << "Deposit " << amount << " berhasil. Saldo baru: " << balance << std::endl;
}

void BankAccount::listCashFlowToday() const {
    std::string todayStr = Utils::getCurrentDate();
    std::cout << "--- Arus Kas Hari Ini (" << todayStr << ") ---\n";
    bool found = false;
    for (const auto& trans : history) {
        if (trans.date == todayStr) {
            trans.print();
            found = true;
        }
    }
    if (!found) {
        std::cout << "Tidak ada transaksi hari ini.\n";
    }
}

void BankAccount::listCashFlowMonth() const {
    std::cout << "--- Semua Riwayat Arus Kas ---\n";
    if (history.empty()) {
        std::cout << "Tidak ada riwayat transaksi.\n";
        return;
    }
    for (const auto& trans : history) {
        trans.print();
    }
}

bool BankAccount::isDormant() const {
    boost::gregorian::date today = Utils::getCurrentBoostDate();
    return lastTransactionDate.year() != today.year() || 
           lastTransactionDate.month() != today.month();
}

int BankAccount::getTransactionsToday() const {
    int count = 0;
    std::string todayStr = Utils::getCurrentDate();
    for (const auto& trans : history) {
        if (trans.date == todayStr) {
            count++;
        }
    }
    return count;
}