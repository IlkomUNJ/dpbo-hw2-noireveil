#include "Bank.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

struct UserActivity {
    int userId;
    std::string name;
    int transactionCount;

    bool operator<(const UserActivity& other) const {
        return transactionCount > other.transactionCount;
    }
};

Bank::Bank() : nextAccountId(1) {}

BankAccount* Bank::createAccount(std::string ownerName, double initialDeposit) {
    accounts.emplace_back(nextAccountId, ownerName, initialDeposit);
    nextAccountId++;
    std::cout << "Akun Bank berhasil dibuat untuk " << ownerName 
              << " dengan ID: " << (nextAccountId - 1) << std::endl;
    return &accounts.back();
}

BankAccount* Bank::getAccount(int id) {
    for (auto& acc : accounts) {
        if (acc.getId() == id) {
            return &acc;
        }
    }
    return nullptr;
}

BankAccount* Bank::getAccountByOwner(std::string name) {
     for (auto& acc : accounts) {
        if (acc.getOwnerName() == name) {
            return &acc;
        }
    }
    return nullptr;
}

void Bank::listTransactionsLastWeek() const {
    Utils::printHeader("Laporan Transaksi Nasabah (Hari Ini)");
    bool found = false;
    for (const auto& acc : accounts) {
        if (acc.getTransactionsToday() > 0) {
            std::cout << "\n--- Transaksi untuk: " << acc.getOwnerName() << " ---\n";
            acc.listCashFlowToday();
            found = true;
        }
    }
    if (!found) {
        std::cout << "Tidak ada transaksi bank hari ini di seluruh nasabah.\n";
    }
}

void Bank::listAllCustomers() const {
    Utils::printHeader("Daftar Semua Nasabah Bank");
    if (accounts.empty()) {
        std::cout << "Tidak ada nasabah.\n";
    }
    for (const auto& acc : accounts) {
        std::cout << "- ID: " << acc.getId() << ", Nama: " << acc.getOwnerName() 
                  << ", Saldo: " << acc.getBalance() << std::endl;
    }
}

void Bank::listDormantAccounts() const {
    Utils::printHeader("Daftar Akun Dormant (Tidak Aktif Bulan Ini)");
    bool found = false;
    for (const auto& acc : accounts) {
        if (acc.isDormant()) {
            std::cout << "- ID: " << acc.getId() << ", Nama: " << acc.getOwnerName() << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "Tidak ada akun dormant.\n";
    }
}

void Bank::listTopNUsersToday(int n) const {
    Utils::printHeader("Laporan Nasabah Teraktif (Hari Ini)");
    
    if (accounts.empty()) {
        std::cout << "Belum ada nasabah.\n";
        return;
    }

    std::vector<UserActivity> activityList;
    for (const auto& acc : accounts) {
        int count = acc.getTransactionsToday();
        activityList.push_back({acc.getId(), acc.getOwnerName(), count});
    }

    std::sort(activityList.begin(), activityList.end());

    int rank = 1;
    for (const auto& activity : activityList) {
        if (rank > n) break;
        if (activity.transactionCount > 0) {
            std::cout << "  " << rank << ". " << activity.name 
                      << " (Total Transaksi Hari Ini: " << activity.transactionCount << ")\n";
            rank++;
        }
    }
    
    if (rank == 1) {
        std::cout << "Tidak ada nasabah yang bertransaksi hari ini.\n";
    }
}