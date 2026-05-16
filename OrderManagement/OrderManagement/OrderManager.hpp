#ifndef ORDER_MANAGER_HPP
#define ORDER_MANAGER_HPP

#include <iostream>
#include <string>

// Structure representing an individual customer order node
struct OrderNode {
    int orderId;
    std::string itemName;
    OrderNode* next;

    // Constructor for easy node creation
    OrderNode(int id, const std::string& name) {
        orderId = id;
        itemName = name;
        next = nullptr;
    }
};

// Custom Queue Implementation using a Singly Linked List
class OrderManager {
private:
    OrderNode* front; // Points to the oldest order (for processing/removal)
    OrderNode* rear;  // Points to the newest order (for adding)

    // Auxiliary lists to track total system history
    OrderNode* historyFront;
    OrderNode* historyRear;

public:
    OrderManager();
    ~OrderManager();

    // Core Functional Requirements
    void enqueueOrder(int id, const std::string& name); // Accept and record order
    OrderNode* dequeueOrder();                          // Assign to robot / remove
    void displayPendingOrders() const;                  // Display active queue
    void displayCompletedHistory() const;               // Display history
    void displayCurrentOrder() const;                   // Display current item processing

    // Helper Methods
    bool isEmpty() const;
    void clearQueue(OrderNode*& headPtr, OrderNode*& tailPtr);
};

#endif // ORDER_MANAGER_HPP