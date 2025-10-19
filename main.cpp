// main.cpp (Tidak ada perubahan signifikan,
// karena semua perubahan dienkapsulasi di dalam kelas Store dan Utils)

#include <iostream>
#include <string>
#include <limits>
#include "Store.h"
#include "Bank.h"
#include "Buyer.h"
#include "Seller.h"
#include "Utils.h"

const std::string DATA_FILE = "data/simulation_data.txt";

// Deklarasi Fungsi Menu
void showMainMenu();
void showBuyerMenu(Store& store, Buyer* buyer);
void showSellerMenu(Store& store, Seller* seller);
void showStoreAdminMenu(Store& store);
void showBankAdminMenu(Bank& bank);

// Fungsi Utama
int main() {
    Bank bank;
    Store store(bank);

    store.loadData(DATA_FILE);

    Buyer* loggedInUser = nullptr;
    bool running = true;

    while (running) {
        if (loggedInUser == nullptr) {
            // Menu Utama (Login/Register)
            showMainMenu();
            int choice = Utils::getChoice(1, 6);

            std::string user, pass;
            switch (choice) {
                case 1: // Login
                    Utils::printHeader("Login");
                    std::cout << "Username: "; std::cin >> user;
                    std::cout << "Password: "; std::cin >> pass;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    
                    loggedInUser = store.login(user, pass);
                    if (loggedInUser == nullptr) {
                        std::cout << "\nLogin gagal. Username atau password salah.\n";
                    } else {
                        std::cout << "\nLogin berhasil!\n";
                    }
                    Utils::pause();
                    break;
                case 2: // Register Buyer
                    loggedInUser = store.registerBuyer();
                    std::cout << "\nRegistrasi berhasil, Anda otomatis login.\n";
                    Utils::pause();
                    break;
                case 3: // Register Seller
                    loggedInUser = store.registerSeller();
                    std::cout << "\nRegistrasi berhasil, Anda otomatis login.\n";
                    Utils::pause();
                    break;
                case 4: // Menu Admin Toko
                    showStoreAdminMenu(store);
                    break;
                case 5: // Menu Admin Bank
                    showBankAdminMenu(bank);
                    break;
                case 6: // Keluar
                    running = false;
                    break;
            }
        } else {
            // Menu Pengguna (Buyer/Seller) 
            if (store.isSeller(loggedInUser)) {
                Seller* seller = static_cast<Seller*>(loggedInUser);
                showSellerMenu(store, seller);
            } else {
                showBuyerMenu(store, loggedInUser);
            }
            loggedInUser = nullptr;
            std::cout << "Anda telah logout.\n";
            Utils::pause();
        }
    }

    // Simpan data persisten sebelum program berakhir
    store.saveData(DATA_FILE);
    std::cout << "Selamat tinggal!\n";
    
    return 0;
}

void showMainMenu() {
    Utils::printHeader("Selamat Datang di Toko Online");
    std::cout << "1. Login\n";
    std::cout << "2. Register sebagai Buyer\n";
    std::cout << "3. Register sebagai Seller\n";
    std::cout << "4. Menu Admin Toko\n";
    std::cout << "5. Menu Admin Bank\n";
    std::cout << "6. Keluar & Simpan\n";
}

void showBuyerMenu(Store& store, Buyer* buyer) {
    bool loggedIn = true;
    while(loggedIn) {
        Utils::printHeader("Menu Buyer: " + buyer->getName());
        std::cout << "1. Browse Items\n";
        std::cout << "2. Purchase Item\n";
        std::cout << "3. List My Orders\n";
        std::cout << "4. Cek Pengeluaran (Hari Ini)\n";
        std::cout << "5. Menu Bank\n";
        std::cout << "6. Logout\n";

        int choice = Utils::getChoice(1, 6);
        switch (choice) {
            case 1: store.browseItems(buyer); Utils::pause(); break;
            case 2: store.purchaseItem(buyer); Utils::pause(); break;
            case 3: store.listMyOrders(buyer); Utils::pause(); break;
            case 4: store.checkMySpending(buyer, 1); Utils::pause(); break;
            case 5: store.showBankMenu(buyer); break;
            case 6: loggedIn = false; break;
        }
    }
}

void showSellerMenu(Store& store, Seller* seller) {
    bool loggedIn = true;
    while(loggedIn) {
        Utils::printHeader("Menu Seller: " + seller->getName() + " (" + seller->getStoreName() + ")");

        std::cout << "--- (Fitur Buyer) ---\n";
        std::cout << "1. Browse Items\n";
        std::cout << "2. Purchase Item\n";
        std::cout << "3. List My Orders\n";
        std::cout << "4. Cek Pengeluaran (Hari Ini)\n";
        std::cout << "5. Menu Bank\n";
        std::cout << "--- (Fitur Seller) ---\n";
        std::cout << "6. Manage My Items\n";
        std::cout << "7. View My Orders (terjual)\n";
        std::cout << "8. Update Order Status\n";
        std::cout << "9. Discover Top K Popular Items\n";
        std::cout << "10. Discover Loyal Customers\n";
        std::cout << "11. Logout\n";

        int choice = Utils::getChoice(1, 11);
        switch (choice) {
            // Fitur Buyer
            case 1: store.browseItems(seller); Utils::pause(); break;
            case 2: store.purchaseItem(seller); Utils::pause(); break;
            case 3: store.listMyOrders(seller); Utils::pause(); break;
            case 4: store.checkMySpending(seller, 1); Utils::pause(); break;
            case 5: store.showBankMenu(seller); break;
            // Fitur Seller
            case 6: store.manageMyItems(seller); Utils::pause(); break;
            case 7: store.viewMyOrders(seller); Utils::pause(); break;
            case 8: store.updateMyOrderStatus(seller); Utils::pause(); break;
            case 9: 
                Utils::printHeader("Discover Top K Items");
                std::cout << "Masukkan 'k' (misal: 3): ";
                int k; std::cin >> k;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                store.discoverTopKItems(seller, k); 
                Utils::pause(); 
                break;
            case 10: store.discoverLoyalCustomers(seller); Utils::pause(); break;
            case 11: loggedIn = false; break;
        }
    }
}

void showStoreAdminMenu(Store& store) {
    bool running = true;
    while(running) {
        Utils::printHeader("Menu Admin Toko");
        std::cout << "1. List Transaksi (dibayar, belum selesai)\n";
        std::cout << "2. List Semua Transaksi Toko\n";
        std::cout << "3. List Most Frequent Items\n";
        std::cout << "4. List Most Active Buyers\n";
        std::cout << "5. List Most Active Sellers\n";
        std::cout << "6. Kembali\n";
        int choice = Utils::getChoice(1, 6);
        switch(choice) {
            case 1: store.listPaidNotCompleted(); Utils::pause(); break;
            case 2: store.listLatestKTransactions(999); Utils::pause(); break;
            case 3: 
                Utils::printHeader("List Most Frequent Items");
                std::cout << "Masukkan 'm' (misal: 5): ";
                int m; std::cin >> m;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                store.listMostFrequentItems(m); 
                Utils::pause(); 
                break;
            case 4: store.listMostActiveBuyers(); Utils::pause(); break;
            case 5: store.listMostActiveSellers(); Utils::pause(); break;
            case 6: running = false; break;
        }
    }
}

void showBankAdminMenu(Bank& bank) {
    bool running = true;
    while(running) {
        Utils::printHeader("Menu Admin Bank");
        std::cout << "1. List Semua Nasabah\n";
        std::cout << "2. List Akun Dormant\n";
        std::cout << "3. List Top N Users Today\n";
        std::cout << "4. List Transaksi Nasabah (Hari Ini)\n";
        std::cout << "5. Kembali\n";
        int choice = Utils::getChoice(1, 5);
        switch(choice) {
            case 1: bank.listAllCustomers(); Utils::pause(); break;
            case 2: bank.listDormantAccounts(); Utils::pause(); break;
            case 3: 
                Utils::printHeader("List Top N Users Today");
                std::cout << "Masukkan 'n' (misal: 3): ";
                int n; std::cin >> n;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                bank.listTopNUsersToday(n); 
                Utils::pause(); 
                break;
            case 4: bank.listTransactionsLastWeek(); Utils::pause(); break;
            case 5: running = false; break;
        }
    }
}