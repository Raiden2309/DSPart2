#include <iostream>
#include <fstream>
#include <string>
#include "OrderManager.hpp"
#include "RobotAssignment.hpp"
#include "BST.h" 
#include "RobotNavigation.hpp" 

using namespace std;

void printMainMenu() {
    cout << "\n==================================================\n";
    cout << "   WAREHOUSE NAVIGATION SYSTEM - MAIN MENU   \n";
    cout << "==================================================\n";
    cout << "  1. Enter Order Management Module\n";
    cout << "  2. Enter Robot Assignment Module\n";
    cout << "  3. Enter Warehouse Inventory Module (BST)\n";
    cout << "  4. Enter Active Robot Navigation Tracking (Stack)\n";
    cout << "  0. Exit System\n";
    cout << "--------------------------------------------------\n";
    cout << "  Enter choice: ";
}

void printOrderMenu() {
    cout << "\n  --- ORDER MANAGEMENT SUB-MENU ---\n";
    cout << "  1. [Automated] Load Orders from CSV\n";
    cout << "  2. Display Pending Orders\n";
    cout << "  3. Process Next Order (Fulfill, Dispatch & Route)\n";
    cout << "  4. Display Completed Order History\n";
    cout << "  0. Return to Master Control\n";
    cout << "--------------------------------------------------\n";
    cout << "  Enter choice: ";
}

void printRobotMenu() {
    cout << "\n  --- ROBOT ASSIGNMENT SUB-MENU ---\n";
    cout << "  1.  Enqueue a new robot\n";
    cout << "  2.  Init default robots\n";
    cout << "  3.  Assign a manual task to a robot\n";
    cout << "  4.  Complete a robot's task (Safely Return Base)\n";
    cout << "  5.  Set robot to maintenance\n";
    cout << "  6.  Restore a robot from maintenance\n";
    cout << "  7.  Dequeue front robot\n";
    cout << "  8.  Display all robots\n";
    cout << "  9.  Display assignment log\n";
    cout << "  10. Display status summary\n";
    cout << "  0.  Return to Master Control\n";
    cout << "--------------------------------------------------\n";
    cout << "  Enter choice: ";
}

void printInventoryMenu() {
    cout << "\n  --- WAREHOUSE INVENTORY SUB-MENU (BST) ---\n";
    cout << "  1. Insert New Item\n";
    cout << "  2. Search Item by ID\n";
    cout << "  3. Update Item Information\n";
    cout << "  4. Delete Item from System\n";
    cout << "  5. Display All Sorted Items (Inorder)\n";
    cout << "  0. Return to Master Control\n";
    cout << "--------------------------------------------------\n";
    cout << "  Enter choice: ";
}

void printNavigationMenu() {
    cout << "\n  --- ROBOT NAVIGATION SUB-MENU (STACK) ---\n";
    cout << "  1. Record a Live Manual Movement Step\n";
    cout << "  2. Undo/Pop Last Movement\n";
    cout << "  3. Display Current Dispatched Route Navigation Log\n";
    cout << "  4. Trigger Obstacle Avoidance Routing\n";
    cout << "  5. Safely Return Dispatched Robot to Base\n";
    cout << "  0. Return to Master Control\n";
    cout << "--------------------------------------------------\n";
    cout << "  Enter choice: ";
}   

int main() {
    OrderManager orderManager;
    RobotAssignmentModule robotManager;
    BST inventoryManager;
    RobotNavigationModule navigationManager;

    // ----------------------------------------------------------------
    // SILENT BACKGROUND DATA INITIALIZATION LAYER
    // ----------------------------------------------------------------
    robotManager.loadRobotsFromCSV();
    orderManager.loadOrdersFromCSV();
    inventoryManager.loadInventoryFromCSV();

    int mainChoice;

    do {
        printMainMenu();

        if (!(cin >> mainChoice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[ERROR] Invalid input. Please enter a number.\n";
            continue;
        }
        cin.ignore(1000, '\n');

        switch (mainChoice) {
        case 1: {
            int orderChoice;
            do {
                printOrderMenu();
                if (!(cin >> orderChoice)) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "[ERROR] Invalid input.\n";
                    continue;
                }
                cin.ignore(1000, '\n');

                switch (orderChoice) {
                case 1:
                    cout << "[INFO] Orders have already been loaded automatically on system startup.\n";
                    break;
                case 2:
                    orderManager.displayPendingOrders();
                    break;
                case 3: {
                    orderManager.displayCurrentOrder();
                    OrderNode* processedOrder = orderManager.dequeueOrder();

                    if (processedOrder) {
                        cout << "[SYSTEM] Order for '" << processedOrder->itemName << "' dequeued.\n";

                        // Use our new function to find the name within the ID-sorted tree structure
                        cout << "[LINK] Querying BST Inventory via Item Name description...\n";
                        ItemNode* itemDetails = inventoryManager.searchByName(processedOrder->itemName);

                        string locationStr = "Location Unknown";
                        string internalIdStr = "N/A";

                        if (itemDetails != nullptr) {
                            locationStr = itemDetails->location;
                            internalIdStr = itemDetails->itemID; // Retrieve the formal corporate ID from the node
                        }

                        // The assignment log can now show both the item ID and location info
                        string automatedTask = "Fetch Order #" + to_string(processedOrder->orderId) +
                            " [" + processedOrder->itemName + " (ID: " + internalIdStr + ")] from " + locationStr;

                        cout << "[LINK] Dispatching task payload to fleet manager...\n";
                        bool assignedSuccess = robotManager.assignTask(automatedTask);

                        if (assignedSuccess) {
                            cout << "[LINK] Initializing telemetry data inside Navigation Stack...\n";
                            while (!navigationManager.isEmpty()) { navigationManager.popMove(); }

                            navigationManager.recordMove("Departed Base Station");
                            navigationManager.recordMove("Navigated into " + locationStr);
                            navigationManager.recordMove("Arrived at target picker bay location");

                            cout << "\n[SUCCESS] Pipeline Complete: Task populated using inventory item code validation rules.\n";
                        }
                    }
                    break;
                }
                case 4:
                    orderManager.displayCompletedHistory();
                    break;
                case 0:
                    cout << "[SYSTEM] Returning to Main Menu...\n";
                    break;
                default:
                    cout << "[ERROR] Invalid choice.\n";
                    break;
                }
            } while (orderChoice != 0);
            break;
        }

        case 2: {
            int robotChoice;
            do {
                printRobotMenu();
                if (!(cin >> robotChoice)) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "[ERROR] Invalid input.\n";
                    continue;
                }
                cin.ignore(1000, '\n');

                int id;
                string name, task;

                switch (robotChoice) {
                case 1:
                    cout << "  Enter Robot ID  : ";
                    if (!(cin >> id)) { cin.clear(); cin.ignore(1000, '\n'); cout << "[ERROR] Invalid ID.\n"; break; }
                    cin.ignore(1000, '\n');
                    cout << "  Enter Robot Name: ";
                    getline(cin, name);
                    robotManager.enqueueRobot(id, name);
                    break;
                case 2:
                    cout << "  How many robots to init: ";
                    if (!(cin >> id)) { cin.clear(); cin.ignore(1000, '\n'); cout << "[ERROR] Invalid number.\n"; break; }
                    cin.ignore(1000, '\n');
                    robotManager.initDefaultRobots(id);
                    break;
                case 3:
                    cout << "  Enter manual task description: ";
                    getline(cin, task);
                    robotManager.assignTask(task);
                    break;
                case 4:
                    cout << "  Enter Robot ID that completed task: ";
                    if (!(cin >> id)) { cin.clear(); cin.ignore(1000, '\n'); cout << "[ERROR] Invalid ID.\n"; break; }
                    cin.ignore(1000, '\n');
                    if (robotManager.completeTask(id)) {
                        // SYSTEM-WIDE RESPONSE: When task finishes, automatically execute backtracking return
                        cout << "[LINK] Routing active tracking telemetry back to base station...\n";
                        navigationManager.returnToStart();
                    }
                    break;
                case 5:
                    cout << "  Enter Robot ID to set maintenance: ";
                    if (!(cin >> id)) { cin.clear(); cin.ignore(1000, '\n'); cout << "[ERROR] Invalid ID.\n"; break; }
                    cin.ignore(1000, '\n');
                    robotManager.setMaintenance(id);
                    break;
                case 6:
                    cout << "  Enter Robot ID to restore: ";
                    if (!(cin >> id)) { cin.clear(); cin.ignore(1000, '\n'); cout << "[ERROR] Invalid ID.\n"; break; }
                    cin.ignore(1000, '\n');
                    robotManager.restoreRobot(id);
                    break;
                case 7:
                    robotManager.dequeueRobot();
                    break;
                case 8:
                    robotManager.displayAllRobots();
                    break;
                case 9:
                    robotManager.displayAssignmentLog();
                    break;
                case 10:
                    robotManager.displayStatusSummary();
                    break;
                case 0:
                    cout << "[SYSTEM] Returning to Main Menu...\n";
                    break;
                default:
                    cout << "[ERROR] Invalid choice.\n";
                    break;
                }
            } while (robotChoice != 0);
            break;
        }

        case 3: {
            int inventoryChoice;
            do {
                printInventoryMenu();
                if (!(cin >> inventoryChoice)) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "[ERROR] Invalid input.\n";
                    continue;
                }
                cin.ignore(1000, '\n');

                string itemId, itemName, location;

                switch (inventoryChoice) {
                case 1:
                    cout << "  Enter Item ID (Key used in matching Order name): ";
                    getline(cin, itemId);
                    cout << "  Enter Full Item Name Description: ";
                    getline(cin, itemName);
                    cout << "  Enter Warehouse Location: ";
                    getline(cin, location);
                    inventoryManager.insert(itemId, itemName, location);
                    break;
                case 2:
                    cout << "  Enter Item ID to search: ";
                    getline(cin, itemId);
                    inventoryManager.search(itemId);
                    break;
                case 3:
                    cout << "  Enter Item ID to update: ";
                    getline(cin, itemId);
                    inventoryManager.update(itemId);
                    break;
                case 4:
                    cout << "  Enter Item ID to delete: ";
                    getline(cin, itemId);
                    inventoryManager.deleteItem(itemId);
                    break;
                case 5:
                    cout << "\n--- Current Warehouse Inventory ---\n";
                    inventoryManager.display();
                    break;
                case 0:
                    cout << "[SYSTEM] Returning to Main Menu...\n";
                    break;
                default:
                    cout << "[ERROR] Invalid choice.\n";
                    break;
                }
            } while (inventoryChoice != 0);
            break;
        }

        case 4: {
            int navChoice;
            do {
                printNavigationMenu();
                if (!(cin >> navChoice)) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "[ERROR] Invalid input.\n";
                    continue;
                }
                cin.ignore(1000, '\n');

                string moveDirection;
                switch (navChoice) {
                case 1:
                    cout << "  Enter direction coordinate string (e.g., Forward, Left): ";
                    getline(cin, moveDirection);
                    navigationManager.recordMove(moveDirection);
                    break;
                case 2: {
                    string removedMove = navigationManager.popMove();
                    if (!removedMove.empty()) {
                        cout << "\n[SYSTEM] Reverted path step: " << removedMove << "\n";
                    }
                    else {
                        cout << "\n[SYSTEM] Stack empty.\n";
                    }
                    break;
                }
                case 3:
                    navigationManager.displayNavigationLog();
                    break;
                case 4:
                    navigationManager.obstacleDetected();
                    break;
                case 5:
                    navigationManager.returnToStart();
                    break;
                case 0:
                    cout << "[SYSTEM] Returning to Main Menu...\n";
                    break;
                default:
                    cout << "[ERROR] Invalid choice.\n";
                    break;
                }
            } while (navChoice != 0);
            break;
        }

        case 0:
            cout << "\n[SYSTEM SHUTDOWN] Terminating Warehouse Master Control...\n\n";
            break;

        default:
            cout << "[ERROR] Invalid choice. Please select 0, 1, 2, 3, or 4.\n";
            break;
        }

    } while (mainChoice != 0);

    return 0;
}