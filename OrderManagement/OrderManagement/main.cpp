#include <iostream>
#include <fstream>
#include <string>
#include "OrderManager.hpp"
using namespace std;

int main() {
    // 1. Initialize your custom module
    OrderManager manager;

    cout << "=== WAREHOUSE SYSTEM: INITIALIZING ORDER MODULE ===\n\n";

    // 2. Open the CSV file
    ifstream file("orders.csv");

    if (!file.is_open()) {
        cerr << "Error: Could not open orders.csv. Check the file path!" << endl;
        return 1;
    }

    string idStr;
    string itemName;
    cout << "--- LOADING ORDERS FROM CSV ---\n";

    // 3. Read the file directly using the comma as a stopping point
    // getline(file, idStr, ',') reads until the comma.
    // getline(file, itemName) reads the rest of the line until the end.
    while (getline(file, idStr, ',') && getline(file, itemName)) {
        try {
            int id = stoi(idStr); // Convert the text ID into an integer

            // Add the parsed data directly into your custom queue
            manager.enqueueOrder(id, itemName);
        }
        catch (const invalid_argument& e) {
            cerr << "Error: Could not convert Order ID to a number -> " << idStr << "\n";
        }
    }

    file.close();
    cout << "--- CSV LOADING COMPLETE ---\n\n";

    // 4. Test the Display Output (Verify First-In, First-Out sequence)
    manager.displayPendingOrders();
    cout << "\n";

    // 5. Simulate Robot Assignment (Processing the oldest orders first)
    cout << "=== SIMULATING ROBOT TASK ASSIGNMENTS ===\n";
    manager.displayCurrentOrder();

    OrderNode* processed1 = manager.dequeueOrder();
    if (processed1) cout << "Robot assigned to fetch: " << processed1->itemName << "\n";

    manager.displayCurrentOrder();

    OrderNode* processed2 = manager.dequeueOrder();
    if (processed2) cout << "Robot assigned to fetch: " << processed2->itemName << "\n\n";

    // 6. Verify System States After Processing
    manager.displayPendingOrders();
    cout << "\n";
    manager.displayCompletedHistory();

    return 0;
}