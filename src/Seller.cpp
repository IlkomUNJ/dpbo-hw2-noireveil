#include "Seller.h"
#include <fstream>
#include <algorithm> 
#include <vector>
#include <string>
#include <cstddef>

Seller::Seller(int id, std::string name, std::string user, std::string pass, int bId, std::string storeName)
    : Buyer(id, name, user, pass, bId), storeName(storeName) {}

std::string Seller::getStoreName() const { return storeName; }
const std::vector<int>& Seller::getItemIds() const { return itemIds; }

void Seller::addItem(int itemId) {
    itemIds.push_back(itemId);
}

void Seller::removeItem(int itemId) {
    itemIds.erase(std::remove(itemIds.begin(), itemIds.end(), itemId), itemIds.end());
}