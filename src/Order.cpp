#include "Order.h"
#include <iostream>

Order::Order(int id, int bId, int sId, std::string date, std::map<int, int> items, double total)
    : id(id), buyerId(bId), sellerId(sId), date(date), items(items), 
      totalAmount(total), status(PAID) {}

int Order::getId() const { return id; }
int Order::getBuyerId() const { return buyerId; }
int Order::getSellerId() const { return sellerId; }
std::string Order::getDate() const { return date; }
Order::Status Order::getStatus() const { return status; }
double Order::getTotal() const { return totalAmount; }
const std::map<int, int>& Order::getItems() const { return items; }

void Order::setStatus(Status newStatus) { status = newStatus; }
std::string Order::getStatusString() const {
    switch (status) {
        case PAID: return "DIBAYAR";
        case COMPLETED: return "SELESAI";
        case CANCELLED: return "DIBATALKAN";
        default: return "UNKNOWN";
    }
}

void Order::printSummary(const std::string& buyerName, const std::string& sellerName) const {
    std::cout << "=================================\n"
              << "Order ID  : " << id << "\n"
              << "Tanggal   : " << date << "\n"
              << "Pembeli   : " << buyerName << " (ID: " << buyerId << ")\n"
              << "Penjual   : " << sellerName << " (ID: " << sellerId << ")\n"
              << "Status    : " << getStatusString() << "\n"
              << "Total     : " << totalAmount << "\n"
              << "--- Item Dibeli ---\n";
    for (const auto& pair : items) {
        std::cout << "  - Item ID: " << pair.first << ", Jumlah: " << pair.second << "\n";
    }
    std::cout << "=================================\n";
}