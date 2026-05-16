#include "RobotAssignment.hpp"

void printTestMenu() {
    cout << "\n";
    for (int i = 0; i < 50; i++) cout << '=';
    cout << "\n  ROBOT ASSIGNMENT - INTERACTIVE TEST\n";
    for (int i = 0; i < 50; i++) cout << '=';
    cout << "\n  1.  Enqueue a robot\n";
    cout << "  2.  Init default robots\n";
    cout << "  3.  Assign a task\n";
    cout << "  4.  Complete a task\n";
    cout << "  5.  Set robot to maintenance\n";
    cout << "  6.  Restore a robot\n";
    cout << "  7.  Dequeue front robot\n";
    cout << "  8.  Display all robots\n";
    cout << "  9.  Display assignment log\n";
    cout << "  10. Display status summary\n";
    cout << "  0.  Exit\n";
    for (int i = 0; i < 50; i++) cout << '-';
    cout << "\n  Enter choice: ";
}

int main() {
    RobotAssignmentModule m;
    int choice;

    do {
        printTestMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[ERROR] Invalid input.\n";
            continue;
        }
        cin.ignore(1000, '\n');

        int    id;
        string name, task;

        switch (choice) {

        case 1:
            cout << "  Enter Robot ID  : ";
            if (!(cin >> id)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "[ERROR] Invalid ID.\n";
                break;
            }
            cin.ignore(1000, '\n');
            cout << "  Enter Robot Name: ";
            getline(cin, name);
            m.enqueueRobot(id, name);
            break;

        case 2:
            cout << "  How many robots to init: ";
            if (!(cin >> id)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "[ERROR] Invalid number.\n";
                break;
            }
            cin.ignore(1000, '\n');
            m.initDefaultRobots(id);
            break;

        case 3:
            cout << "  Enter task description: ";
            getline(cin, task);
            m.assignTask(task);
            break;

        case 4:
            cout << "  Enter Robot ID that completed task: ";
            if (!(cin >> id)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "[ERROR] Invalid ID.\n";
                break;
            }
            cin.ignore(1000, '\n');
            m.completeTask(id);
            break;

        case 5:
            cout << "  Enter Robot ID to set maintenance: ";
            if (!(cin >> id)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "[ERROR] Invalid ID.\n";
                break;
            }
            cin.ignore(1000, '\n');
            m.setMaintenance(id);
            break;

        case 6:
            cout << "  Enter Robot ID to restore: ";
            if (!(cin >> id)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "[ERROR] Invalid ID.\n";
                break;
            }
            cin.ignore(1000, '\n');
            m.restoreRobot(id);
            break;

        case 7:
            m.dequeueRobot();
            break;

        case 8:
            m.displayAllRobots();
            break;

        case 9:
            m.displayAssignmentLog();
            break;

        case 10:
            m.displayStatusSummary();
            break;

        case 0:
            cout << "\n[EXIT] Test session ended.\n\n";
            break;

        default:
            cout << "[ERROR] Choose 0-10.\n";
            break;
        }

    } while (choice != 0);

    return 0;
}