#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>


class DuplicateItemException : public std::runtime_error {
public:
    DuplicateItemException(const std::string& msg) : std::runtime_error(msg) {}
};


class ItemNotFoundException : public std::runtime_error {
public:
    ItemNotFoundException(const std::string& msg) : std::runtime_error(msg) {}
};


class StoredItem {
private:
    std::string id;
    std::string description;
    std::string location;


public:
    StoredItem(std::string id, std::string desc, std::string loc)
        : id(id), description(desc), location(loc) {}

    std::string getId() const { return id; }
    std::string getDescription() const { return description; }
    std::string getLocation() const { return location; }
};


class StorageManager {
private:
    std::unordered_map<std::string, std::shared_ptr<StoredItem>> itemById;
    std::map<std::string, std::shared_ptr<StoredItem>> itemByDescription;

public:
    void addItem(const std::shared_ptr<StoredItem>& item) {
        // TODO: Add item to both maps, throw if ID already exists
        const std::string& id = item->getId();
        const std::string& desc = item->getDescription();

        if (itemById.find(id) != itemById.end()) {
            throw DuplicateItemException("Item with ID '" + id + "' already exists.");
        }

        itemById[id] = item;
        itemByDescription[desc] = item;
    }

    std::shared_ptr<StoredItem> findById(const std::string& id) const {
        // TODO: Return item if found or throw ItemNotFoundException
        auto it = itemById.find(id);
        if (it == itemById.end()) {
            throw ItemNotFoundException("Item with ID '" + id + "' not found.");
        }
        return it->second;
    }

    void removeItem(const std::string& id) {
        // TODO: Remove from both maps, throw if not found
        auto it = itemById.find(id);
        if (it == itemById.end()) {
            throw ItemNotFoundException("Item with ID '" + id + "' not found.");
        }

        std::string desc = it->second->getDescription();
        itemByDescription.erase(desc);
        itemById.erase(it);
    }

    void listItemsByDescription() const {
        // TODO: Iterate over itemByDescription and print info
        for (const auto& pair : itemByDescription) {
            auto item = pair.second;
            std::cout << "ID: " << item->getId()
                      << ", Description: " << item->getDescription()
                      << ", Location: " << item->getLocation() << std::endl;
        }
    }
};


void testDuplicateAddition();
void testItemNotFound();


int main() {
    // TODO: Create StorageManager and test functionality
    StorageManager manager;

    // Create test items
    auto item1 = std::make_shared<StoredItem>("ITEM001", "LED Light", "Aisle 3, Shelf 1");
    auto item2 = std::make_shared<StoredItem>("ITEM002", "USB Cable", "Aisle 1, Shelf 2");
    auto item3 = std::make_shared<StoredItem>("ITEM003", "Power Strip", "Aisle 4, Shelf 3");

    try {
        manager.addItem(item1);
        manager.addItem(item2);
        manager.addItem(item3);
    } catch (const DuplicateItemException& e) {
        std::cout << "Error adding item: " << e.what() << std::endl;
    }

    // Test to find an item by ID
    try {
        auto found = manager.findById("ITEM002");
        std::cout << "\nFound item: " << found->getDescription()
                  << " at " << found->getLocation() << std::endl;
    } catch (const ItemNotFoundException& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // Test to remove a item that does not exist
    try {
        manager.removeItem("ITEM999");  // SHOULD trigger exception
    } catch (const ItemNotFoundException& e) {
        std::cout << "\nCaught exception while removing: " << e.what() << std::endl;
    }

    // List items sorted by description
    std::cout << "\nItems in storage (sorted by description):\n";
    manager.listItemsByDescription();

}