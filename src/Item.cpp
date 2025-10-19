#include "Item.h"
#include <iostream>

Item::Item(int id, int sellerId, std::string name, int stock, double price)
    : id(id), sellerId(sellerId), name(name), stock(stock), price(price) {}

int Item::getId() const { return id; }
int Item::getSellerId() const { return sellerId; }
std::string Item::getName() const { return name; }
int Item::getStock() const { return stock; }
double Item::getPrice() const { return price; }

void Item::setStock(int newStock) { stock = newStock; }
void Item::setPrice(double newPrice) { price = newPrice; }
void Item::print() const {
    std::cout << "---------------------------------\n"
              << "ID Item   : " << id << "\n"
              << "Nama      : " << name << "\n"
              << "Harga     : " << price << "\n"
              << "Stok      : " << stock << "\n"
              << "ID Penjual: " << sellerId << "\n"
              << "---------------------------------\n";
}