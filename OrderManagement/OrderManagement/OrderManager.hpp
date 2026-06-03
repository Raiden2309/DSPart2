#ifndef ORDER_MANAGER_HPP
#define ORDER_MANAGER_HPP

#include <iostream>
#include <string>

struct OrderNode {
    int orderId;
    std::string itemName;
    OrderNode* next;

    OrderNode(int id, const std::string& name) : orderId(id), itemName(name), next(nullptr) {}
};

class OrderManager {
private:
    OrderNode* front;
    OrderNode* rear;
    OrderNode* historyFront;
    OrderNode* historyRear;
    void clearQueue(OrderNode*& headPtr, OrderNode*& tailPtr);

public:
    OrderManager();
    ~OrderManager();

    bool isEmpty() const;
    void enqueueOrder(int id, const std::string& name);
    OrderNode* dequeueOrder();
    void displayCurrentOrder() const;
    void displayPendingOrders() const;
    void displayCompletedHistory() const;

    // Added for automatic background loading
    void loadOrdersFromCSV();
};

#endif