#include "Store.h"
#include "Utils.h"
#include "Seller.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <map>
#include <vector>       
#include <algorithm>
#include <string>
#include <memory> 
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/export.hpp>

#include "Buyer.h"
BOOST_CLASS_EXPORT_IMPLEMENT(Buyer)
BOOST_CLASS_EXPORT_IMPLEMENT(Seller)

Store::Store(Bank& b) : bank(b), nextUserId(1), nextItemId(1), nextOrderId(1) {}

// fungsi helper (non-const)
Buyer* Store::getUser(int id) {
    for (const auto& userPtr : users) {
        if (userPtr->getId() == id) return userPtr.get();
    }
    return nullptr;
}
Seller* Store::getSeller(int id) {
    return dynamic_cast<Seller*>(getUser(id));
}
Item* Store::getItem(int id) {
    for (auto& item : items) {
        if (item.getId() == id) return &item;
    }
    return nullptr;
}
Order* Store::getOrder(int id) {
    for (auto& order : orders) {
        if (order.getId() == id) return &order;
    }
    return nullptr;
}
Seller* Store::getSellerFromItemId(int itemId) {
    Item* item = getItem(itemId);
    return item ? getSeller(item->getSellerId()) : nullptr;
}

// fungsi helper (const)
Buyer* Store::getUser(int id) const {
    for (const auto& userPtr : users) {
        if (userPtr->getId() == id) return userPtr.get();
    }
    return nullptr;
}
Seller* Store::getSeller(int id) const {
    return dynamic_cast<Seller*>(getUser(id));
}
Item* Store::getItem(int id) const {
    for (const auto& item : items) {
        if (item.getId() == id) return const_cast<Item*>(&item);
    }
    return nullptr;
}
Order* Store::getOrder(int id) const {
     for (const auto& order : orders) {
        if (order.getId() == id) return const_cast<Order*>(&order);
    }
    return nullptr;
}
Seller* Store::getSellerFromItemId(int itemId) const {
    Item* item = getItem(itemId);
    return item ? getSeller(item->getSellerId()) : nullptr;
}

// login dan register
Buyer* Store::registerBuyer() {
    Utils::printHeader("Register Buyer Baru");
    std::string name, user, pass;
    double deposit;
    
    std::cout << "Nama Lengkap: "; std::getline(std::cin, name);
    std::cout << "Username: "; std::cin >> user;
    std::cout << "Password: "; std::cin >> pass;
    std::cout << "Deposit Awal Bank: "; std::cin >> deposit;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

    BankAccount* acc = bank.createAccount(name, deposit);
    auto newBuyer = std::make_unique<Buyer>(nextUserId, name, user, pass, acc->getId());
    Buyer* rawPtr = newBuyer.get();
    users.push_back(std::move(newBuyer));
    nextUserId++;
    std::cout << "\nBuyer " << name << " (ID: " << rawPtr->getId() << ") berhasil dibuat.\n";
    return rawPtr;
}

Seller* Store::registerSeller() {
    Utils::printHeader("Register Seller Baru");
    std::string name, user, pass, storeName;
    double deposit;

    std::cout << "Nama Lengkap: "; std::getline(std::cin, name);
    std::cout << "Nama Toko: "; std::getline(std::cin, storeName);
    std::cout << "Username: "; std::cin >> user;
    std::cout << "Password: "; std::cin >> pass;
    std::cout << "Deposit Awal Bank: "; std::cin >> deposit;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

    BankAccount* acc = bank.createAccount(name, deposit);
    auto newSeller = std::make_unique<Seller>(nextUserId, name, user, pass, acc->getId(), storeName);
    Seller* rawPtr = newSeller.get();
    users.push_back(std::move(newSeller));
    nextUserId++;
    std::cout << "\nSeller " << name << " (ID: " << rawPtr->getId() << ") berhasil dibuat.\n";
    return rawPtr;
}

Buyer* Store::login(const std::string& username, const std::string& password) {
    for (const auto& userPtr : users) {
        if (userPtr->getUsername() == username && userPtr->checkPassword(password)) {
            return userPtr.get();
        }
    }
    return nullptr;
}

bool Store::isSeller(Buyer* buyer) {
    return dynamic_cast<Seller*>(buyer) != nullptr;
}

// fungsi buyer
void Store::browseItems(Buyer* buyer) {
    (void)buyer; 
    Utils::printHeader("Daftar Semua Item di Toko");
    if (items.empty()) {
        std::cout << "Belum ada item yang dijual.\n";
        return;
    }
    for (const auto& item : items) {
        item.print();
    }
}

void Store::purchaseItem(Buyer* buyer) {
    browseItems(buyer); 
    std::cout << "\n--- Form Pembelian ---\n";
    
    if (items.empty()) {
        std::cout << "Toko belum memiliki item untuk dijual.\n";
        return;
    }
    
    std::cout << "Masukkan ID Item yang ingin dibeli (1-" << (nextItemId > 1 ? nextItemId - 1 : 1) << "): ";
    int itemId = Utils::getChoice(1, (nextItemId > 1 ? nextItemId - 1 : 1) );
    
    Item* item = getItem(itemId);
    if (!item) {
        std::cout << "Item tidak ditemukan.\n";
        return;
    }

    std::cout << "Item: " << item->getName() << ", Harga: " << item->getPrice() << ", Stok: " << item->getStock() << std::endl;
    if (item->getStock() == 0) {
        std::cout << "Stok item ini habis.\n";
        return;
    }

    std::cout << "Masukkan jumlah yang ingin dibeli: ";
    int qty = Utils::getChoice(1, item->getStock());
    
    double total = item->getPrice() * qty;
    std::cout << "Total Belanja: " << total << std::endl;
    std::cout << "Konfirmasi pembelian (y/n)? ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "Pembelian dibatalkan.\n";
        return;
    }

    BankAccount* buyerAcc = bank.getAccount(buyer->getBankAccountId());
    if (buyerAcc->withdraw(total)) {
        Seller* seller = getSellerFromItemId(itemId);
        BankAccount* sellerAcc = bank.getAccount(seller->getBankAccountId());
        sellerAcc->deposit(total);
        item->setStock(item->getStock() - qty);
        std::map<int, int> orderItems;
        orderItems[itemId] = qty;
        orders.emplace_back(nextOrderId, buyer->getId(), seller->getId(), Utils::getCurrentDate(), orderItems, total);
        orders.back().setStatus(Order::PAID);
        nextOrderId++;
        std::cout << "Pembelian berhasil! Order ID: " << (nextOrderId - 1) << std::endl;
    } else {
        std::cout << "Pembayaran Gagal. Saldo tidak cukup.\n";
    }
}

void Store::listMyOrders(Buyer* buyer) {
    Utils::printHeader("Daftar Pesanan Saya (" + buyer->getName() + ")");
    bool found = false;
    for (const auto& order : orders) {
        if (order.getBuyerId() == buyer->getId()) {
            Seller* seller = getSeller(order.getSellerId());
            order.printSummary(buyer->getName(), seller ? seller->getStoreName() : "N/A");
            found = true;
        }
    }
    if (!found) {
        std::cout << "Anda belum memiliki pesanan.\n";
    }
}

void Store::checkMySpending(Buyer* buyer, int k) const {
    (void)k; 
    Utils::printHeader("Cek Pengeluaran Hari Ini");
    double total = 0;
    std::string today = Utils::getCurrentDate();
    for (const auto& order : orders) {
        if (order.getBuyerId() == buyer->getId() && order.getDate() == today) {
            total += order.getTotal();
        }
    }
    std::cout << "Total pengeluaran Anda hari ini: " << total << std::endl;
}

// fungsi seller
void Store::manageMyItems(Seller* seller) {
    Utils::printHeader("Pengelolaan Item (" + seller->getStoreName() + ")");
    std::cout << "Item Anda Saat Ini:\n";
    if (seller->getItemIds().empty()) {
        std::cout << "Anda belum memiliki item.\n";
    }
    for (int id : seller->getItemIds()) {
        getItem(id)->print();
    }

    std::cout << "\n1. Tambah Item Baru\n";
    std::cout << "2. Update Item (Stok/Harga)\n";
    std::cout << "3. Kembali\n";
    int choice = Utils::getChoice(1, 3);

    if (choice == 1) {
        std::string name;
        int stock;
        double price;

        std::cout << "Nama item baru: "; std::getline(std::cin, name);
        std::cout << "Stok awal: "; std::cin >> stock;
        std::cout << "Harga: "; std::cin >> price;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        items.emplace_back(nextItemId, seller->getId(), name, stock, price);
        seller->addItem(nextItemId);
        std::cout << "Item baru (ID: " << nextItemId << ") berhasil ditambahkan.\n";
        nextItemId++;
    } else if (choice == 2) {
        std::cout << "Masukkan ID item yang ingin di-update: ";
        int id; std::cin >> id;
        Item* item = getItem(id);
        if (item && item->getSellerId() == seller->getId()) {
            std::cout << "Stok baru (sekarang: " << item->getStock() << "): ";
            int stock; std::cin >> stock;
            std::cout << "Harga baru (sekarang: " << item->getPrice() << "): ";
            double price; std::cin >> price;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            item->setStock(stock);
            item->setPrice(price);
            std::cout << "Item " << item->getName() << " berhasil di-update.\n";
        } else {
            std::cout << "Item tidak ditemukan atau bukan milik Anda.\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void Store::viewMyOrders(Seller* seller) const {
    Utils::printHeader("Daftar Pesanan Masuk (" + seller->getStoreName() + ")");
    bool found = false;
    for (const auto& order : orders) {
        if (order.getSellerId() == seller->getId()) {
            Buyer* buyer = getUser(order.getBuyerId()); // Menggunakan versi const
            order.printSummary(buyer ? buyer->getName() : "N/A", seller->getStoreName());
            found = true;
        }
    }
    if (!found) {
        std::cout << "Toko Anda belum memiliki pesanan.\n";
    }
}

void Store::updateMyOrderStatus(Seller* seller) {
    Utils::printHeader("Update Status Pesanan");
    viewMyOrders(seller); 
    
    if (orders.empty()) {
        std::cout << "Tidak ada order untuk di-update.\n";
        return;
    }
    
    std::cout << "\n--- Update Status Pesanan ---\n";
    std::cout << "Masukkan ID Order yang ingin di-update (1-" << (nextOrderId > 1 ? nextOrderId - 1 : 1) << "): ";
    int id = Utils::getChoice(1, (nextOrderId > 1 ? nextOrderId - 1 : 1));
    
    Order* order = getOrder(id);
    if (order && order->getSellerId() == seller->getId()) {
        std::cout << "Status Saat Ini: " << order->getStatusString() << std::endl;
        std::cout << "Pilih Status Baru:\n1. DIBAYAR\n2. SELESAI\n3. DIBATALKAN\n";
        int s = Utils::getChoice(1, 3);
        order->setStatus(static_cast<Order::Status>(s - 1));
        std::cout << "Status Order " << id << " berhasil diubah menjadi " << order->getStatusString() << std::endl;
    } else {
        std::cout << "Order tidak ditemukan atau bukan milik Anda.\n";
    }
}

// Helper struct untuk sorting
struct ItemActivity {
    int itemId;
    std::string name;
    int count; 
    bool operator<(const ItemActivity& other) const { return count > other.count; }
};

void Store::discoverTopKItems(Seller* seller, int k) const {
    Utils::printHeader("Laporan Item Terpopuler (" + seller->getStoreName() + ")");
    
    std::map<int, int> itemCounts;
    bool hasOrders = false;
    for (const auto& order : orders) {
        if (order.getSellerId() == seller->getId()) {
            hasOrders = true;
            for (const auto& pair : order.getItems()) {
                itemCounts[pair.first] += pair.second; 
            }
        }
    }

    if (!hasOrders) {
        std::cout << "Toko Anda belum memiliki penjualan untuk dianalisis.\n";
        return;
    }

    std::vector<ItemActivity> activityList;
    for (const auto& pair : itemCounts) {
        Item* item = getItem(pair.first);
        std::string itemName = item ? item->getName() : ("ID " + std::to_string(pair.first));
        activityList.push_back({pair.first, itemName, pair.second});
    }

    std::sort(activityList.begin(), activityList.end());

    int rank = 1;
    for (const auto& activity : activityList) {
        if (rank > k) break;
        std::cout << "  " << rank << ". " << activity.name 
                  << " (Terjual: " << activity.count << " unit)\n";
        rank++;
    }
}

// Helper struct untuk sorting
struct UserActivity {
    int userId;
    std::string name;
    int transactionCount;
    bool operator<(const UserActivity& other) const { return transactionCount > other.transactionCount; }
};

void Store::discoverLoyalCustomers(Seller* seller) const {
    Utils::printHeader("Laporan Pelanggan Setia (" + seller->getStoreName() + ")");

    std::map<int, int> buyerCounts;
    bool hasOrders = false;
    for (const auto& order : orders) {
        if (order.getSellerId() == seller->getId()) {
            buyerCounts[order.getBuyerId()]++;
            hasOrders = true;
        }
    }
    
    if (!hasOrders) {
        std::cout << "Toko Anda belum memiliki penjualan untuk dianalisis.\n";
        return;
    }

    std::vector<UserActivity> activityList;
    for (const auto& pair : buyerCounts) {
        Buyer* buyer = getUser(pair.first);
        std::string buyerName = buyer ? buyer->getName() : ("ID " + std::to_string(pair.first));
        activityList.push_back({pair.first, buyerName, pair.second});
    }

    std::sort(activityList.begin(), activityList.end());

    int rank = 1;
    for (const auto& activity : activityList) {
        std::cout << "  " << rank << ". " << activity.name 
                  << " (Total Transaksi: " << activity.transactionCount << ")\n";
        rank++;
    }
}

// fungsi bank
void Store::showBankMenu(Buyer* buyer) {
    BankAccount* acc = bank.getAccount(buyer->getBankAccountId());
    if (!acc) {
        std::cout << "Error: Akun bank tidak tertaut.\n";
        return;
    }
    
    bool running = true;
    while(running) {
        Utils::printHeader("Menu Bank (" + acc->getOwnerName() + ")");
        std::cout << "Saldo Anda: " << acc->getBalance() << std::endl;
        std::cout << "\n1. Deposit (Topup)\n";
        std::cout << "2. Withdraw (Tarik Tunai)\n";
        std::cout << "3. Cek Arus Kas Hari Ini\n";
        std::cout << "4. Cek Semua Riwayat Arus Kas\n";
        std::cout << "5. Kembali\n";
        int choice = Utils::getChoice(1, 5);
        
        double amount;
        switch (choice) {
            case 1:
                std::cout << "Jumlah deposit: "; std::cin >> amount;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                acc->deposit(amount);
                Utils::pause();
                break;
            case 2:
                std::cout << "Jumlah withdraw: "; std::cin >> amount;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                acc->withdraw(amount);
                Utils::pause();
                break;
            case 3:
                Utils::printHeader("Arus Kas Hari Ini");
                acc->listCashFlowToday();
                Utils::pause(); 
                break;
            case 4:
                Utils::printHeader("Semua Riwayat Arus Kas");
                acc->listCashFlowMonth();
                Utils::pause(); 
                break;
            case 5:
                running = false; 
                break;
        }
    }
}

// fungsi laporan
void Store::listLatestKTransactions(int k) const {
    (void)k; 
    Utils::printHeader("Semua Transaksi Toko");
    if (orders.empty()) {
        std::cout << "Belum ada transaksi.\n";
        return;
    }
    for (const auto& order : orders) {
        Buyer* buyer = getUser(order.getBuyerId());
        Seller* seller = getSeller(order.getSellerId());
        order.printSummary(buyer ? buyer->getName() : "N/A", 
                           seller ? seller->getStoreName() : "N/A");
    }
}

void Store::listPaidNotCompleted() const {
    Utils::printHeader("Transaksi Dibayar, Belum Selesai");
    bool found = false;
    for (const auto& order : orders) {
        if (order.getStatus() == Order::PAID) {
            Buyer* buyer = getUser(order.getBuyerId());
            Seller* seller = getSeller(order.getSellerId());
            order.printSummary(buyer ? buyer->getName() : "N/A", 
                               seller ? seller->getStoreName() : "N/A");
            found = true;
        }
    }
    if (!found) {
        std::cout << "Tidak ada transaksi yang menunggu penyelesaian.\n";
    }
}

void Store::listMostFrequentItems(int m) const {
    Utils::printHeader("Laporan Item Terpopuler (Semua Toko)");
    
    std::map<int, int> itemCounts;
    if (orders.empty()) {
        std::cout << "Belum ada transaksi untuk dianalisis.\n";
        return;
    }

    for (const auto& order : orders) {
        for (const auto& pair : order.getItems()) {
            itemCounts[pair.first] += pair.second; 
        }
    }

    std::vector<ItemActivity> activityList;
    for (const auto& pair : itemCounts) {
        Item* item = getItem(pair.first); // const
        std::string itemName = item ? item->getName() : ("ID " + std::to_string(pair.first));
        activityList.push_back({pair.first, itemName, pair.second});
    }

    std::sort(activityList.begin(), activityList.end());

    int rank = 1;
    for (const auto& activity : activityList) {
        if (rank > m) break;
        std::cout << "  " << rank << ". " << activity.name 
                  << " (Terjual: " << activity.count << " unit)\n";
        rank++;
    }
}

void Store::listMostActiveBuyers() const {
    Utils::printHeader("Laporan Pembeli Paling Aktif");

    if (orders.empty()) {
        std::cout << "Belum ada transaksi untuk dianalisis.\n";
        return;
    }

    std::map<int, int> buyerCounts;
    for (const auto& order : orders) {
        buyerCounts[order.getBuyerId()]++;
    }

    std::vector<UserActivity> activityList;
    for (const auto& pair : buyerCounts) {
        Buyer* buyer = getUser(pair.first); // const
        std::string buyerName = buyer ? buyer->getName() : ("ID " + std::to_string(pair.first));
        activityList.push_back({pair.first, buyerName, pair.second});
    }

    std::sort(activityList.begin(), activityList.end());

    int rank = 1;
    for (const auto& activity : activityList) {
        std::cout << "  " << rank << ". " << activity.name 
                  << " (Total Transaksi: " << activity.transactionCount << ")\n";
        rank++;
    }
}

void Store::listMostActiveSellers() const {
    Utils::printHeader("Laporan Penjual Paling Aktif");

    if (orders.empty()) {
        std::cout << "Belum ada transaksi untuk dianalisis.\n";
        return;
    }

    std::map<int, int> sellerCounts;
    for (const auto& order : orders) {
        sellerCounts[order.getSellerId()]++;
    }

    std::vector<UserActivity> activityList;
    for (const auto& pair : sellerCounts) {
        Seller* seller = getSeller(pair.first);
        std::string sellerName = seller ? seller->getStoreName() : ("ID " + std::to_string(pair.first));
        activityList.push_back({pair.first, sellerName, pair.second});
    }

    std::sort(activityList.begin(), activityList.end());

    int rank = 1;
    for (const auto& activity : activityList) {
        std::cout << "  " << rank << ". " << activity.name 
                  << " (Total Transaksi: " << activity.transactionCount << ")\n";
        rank++;
    }
}

// fungsi serialisasi (dengan Boost.Serialization)
void Store::saveData(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Tidak bisa membuka file untuk menyimpan: " << filename << std::endl;
        return;
    }

    try {
        boost::archive::text_oarchive out_archive(outFile);
        out_archive << bank;
        out_archive << users;
        out_archive << items;
        out_archive << orders;
        out_archive << nextUserId;
        out_archive << nextItemId;
        out_archive << nextOrderId;
    } catch (const std::exception& e) {
        std::cerr << "Error saat menyimpan data: " << e.what() << std::endl;
    }
    
    outFile.close();
    std::cout << "\nData berhasil disimpan ke " << filename << std::endl;
}

void Store::loadData(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Info: File data '" << filename << "' tidak ditemukan. Memulai dengan data baru." << std::endl;
        return;
    }

    try {
        boost::archive::text_iarchive in_archive(inFile);
        in_archive >> bank;
        in_archive >> users;
        in_archive >> items;
        in_archive >> orders;
        in_archive >> nextUserId;
        in_archive >> nextItemId;
        in_archive >> nextOrderId;
    } catch (const std::exception& e) {
        std::cerr << "Error saat memuat data: " << e.what() << ". Memulai dengan data baru." << std::endl;
        users.clear();
        items.clear();
        orders.clear();
        nextUserId = 1;
        nextItemId = 1;
        nextOrderId = 1;
    }

    inFile.close();
    std::cout << "Data berhasil dimuat dari " << filename << std::endl;
}