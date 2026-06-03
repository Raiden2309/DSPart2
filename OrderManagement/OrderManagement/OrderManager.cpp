//Name: Edwin Chin Chun Wui//TP No.: TP107065
#include "OrderManager.hpp"
#include <fstream> // Required for ifstream file streaming

using namespace std;

// Constructor initializes all tracking pointers to null
OrderManager::OrderManager() {
    front = nullptr;
    rear = nullptr;
    historyFront = nullptr;
    historyRear = nullptr;
}

// Destructor cleans up dynamic memory to prevent memory leaks
OrderManager::~OrderManager() {
    clearQueue(front, rear);
    clearQueue(historyFront, historyRear);
}

// Helper to free memory of a specific linked list
void OrderManager::clearQueue(OrderNode*& headPtr, OrderNode*& tailPtr) {
    OrderNode* current = headPtr;
    while (current != nullptr) {
        OrderNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    headPtr = nullptr;
    tailPtr = nullptr;
}

// Check if active queue is empty
bool OrderManager::isEmpty() const {
    return front == nullptr;
}

// Silent helper for background parsing to guarantee terminal window privacy
void OrderManager::loadOrdersFromCSV() {
    ifstream file("orders.csv");
    if (!file.is_open()) {
        cerr << "[ERROR] Could not open orders.csv during startup. Check the file path!\n";
        return;
    }

    string idStr, name;
    while (getline(file, idStr, ',') && getline(file, name)) {
        try {
            int id = stoi(idStr);

            // Replicated enqueue logic inline to keep it completely private/silent on screen
            OrderNode* newNode = new OrderNode(id, name);
            if (isEmpty()) {
                front = newNode;
                rear = newNode;
            }
            else {
                rear->next = newNode;
                rear = newNode;
            }
        }
        catch (const invalid_argument& e) {
            // Skips corrupt rows silently to preserve menu appearance
        }
    }
    file.close();
}

// Requirement: Accept and record new customer orders into the system
void OrderManager::enqueueOrder(int id, const string& name) {
    OrderNode* newNode = new OrderNode(id, name);

    if (isEmpty()) {
        front = newNode;
        rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
    cout << "Order #" << id << " (" << name << ") added successfully.\n";
}

// Requirement: Process orders sequentially according to arrival & remove them
OrderNode* OrderManager::dequeueOrder() {
    if (isEmpty()) {
        cout << "System Alert: No pending orders to assign!\n";
        return nullptr;
    }

    OrderNode* orderedNode = front;

    front = front->next;
    if (front == nullptr) {
        rear = nullptr;
    }

    orderedNode->next = nullptr;

    if (historyFront == nullptr) {
        historyFront = orderedNode;
        historyRear = orderedNode;
    }
    else {
        historyRear->next = orderedNode;
        historyRear = orderedNode;
    }

    return orderedNode;
}

// Requirement: Display current order being processed
void OrderManager::displayCurrentOrder() const {
    if (isEmpty()) {
        cout << "Current Status: Idle. No orders are currently being processed.\n";
    }
    else {
        cout << "Current Order Processing: ID: " << front->orderId
            << " | Item: " << front->itemName << "\n";
    }
}

// Requirement: Display current pending orders
void OrderManager::displayPendingOrders() const {
    if (isEmpty()) {
        cout << "Pending Orders List is currently empty.\n";
        return;
    }

    cout << "--- PENDING ORDERS LIST ---\n";
    OrderNode* current = front;
    while (current != nullptr) {
        cout << "-> [Order ID: " << current->orderId << " | Item: " << current->itemName << "]\n";
        current = current->next;
    }
    cout << "---------------------------\n";
}

// Requirement: Display completed order history
void OrderManager::displayCompletedHistory() const {
    if (historyFront == nullptr) {
        cout << "Completed History is empty. No tasks completed yet.\n";
        return;
    }

    cout << "--- COMPLETED ORDER HISTORY ---\n";
    OrderNode* current = historyFront;
    while (current != nullptr) {
        cout << "[Archived ID: " << current->orderId << " | Item: " << current->itemName << "]\n";
        current = current->next;
    }
    cout << "-------------------------------\n";
}