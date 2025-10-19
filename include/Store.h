#ifndef STORE_H
#define STORE_H

#include "Bank.h"
#include "Buyer.h"
#include "Seller.h"
#include "Item.h"
#include "Order.h"
#include <vector>
#include <map>
#include <memory> 

class Store {
private:
    Bank& bank;
    std::vector<std::unique_ptr<Buyer>> users;
    std::vector<Item> items;
    std::vector<Order> orders;

    int nextUserId;
    int nextItemId;
    int nextOrderId;

    Buyer* getUser(int id);
    Seller* getSeller(int id);
    Item* getItem(int id);
    Order* getOrder(int id);
    Seller* getSellerFromItemId(int itemId);

    Buyer* getUser(int id) const;
    Seller* getSeller(int id) const;
    Item* getItem(int id) const;
    Order* getOrder(int id) const;
    Seller* getSellerFromItemId(int itemId) const;

public:
    Store(Bank& b);

    const std::vector<std::unique_ptr<Buyer>>& getUsers() const { return users; }

    // Fungsi Pendaftaran & Login
    Buyer* registerBuyer();
    Seller* registerSeller();
    Buyer* login(const std::string& username, const std::string& password);
    bool isSeller(Buyer* buyer);

    // Fungsi Buyer
    void browseItems(Buyer* buyer);
    void purchaseItem(Buyer* buyer);
    void listMyOrders(Buyer* buyer);
    void checkMySpending(Buyer* buyer, int k) const;

    // Fungsi Seller
    void manageMyItems(Seller* seller);
    void viewMyOrders(Seller* seller) const;
    void updateMyOrderStatus(Seller* seller);
    void discoverTopKItems(Seller* seller, int k) const;
    void discoverLoyalCustomers(Seller* seller) const;

    // Fungsi Bank
    void showBankMenu(Buyer* buyer);

    // Fungsi Laporan Store
    void listLatestKTransactions(int k) const;
    void listPaidNotCompleted() const;
    void listMostFrequentItems(int m) const;
    void listMostActiveBuyers() const;
    void listMostActiveSellers() const;

    // Fungsi Serialisasi
    void saveData(const std::string& filename) const;
    void loadData(const std::string& filename);
};
#endif