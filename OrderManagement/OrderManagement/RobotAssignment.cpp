#include "RobotAssignment.hpp"

RobotAssignmentModule::RobotAssignmentModule()
    : front(nullptr), rear(nullptr), count(0), nextRobot(0),
    logHead(nullptr), logTail(nullptr), logCount(0) {
}

RobotAssignmentModule::~RobotAssignmentModule() {
    if (rear != nullptr) {
        rear->next = nullptr;
        QueueNode* ptr = front;
        while (ptr != nullptr) {
            QueueNode* tmp = ptr;
            ptr = ptr->next;
            delete tmp;
        }
    }
    clearLog();
}

// Helpers

void RobotAssignmentModule::clearLog() {
    LogNode* ptr = logHead;
    while (ptr != nullptr) {
        LogNode* tmp = ptr;
        ptr = ptr->next;
        delete tmp;
    }
    logHead = nullptr;
    logTail = nullptr;
    logCount = 0;
}

QueueNode* RobotAssignmentModule::findRobotNode(int robotID) const {
    if (front == nullptr) return nullptr;
    QueueNode* ptr = front;
    do {
        if (ptr->data.id == robotID) return ptr;
        ptr = ptr->next;
    } while (ptr != front);
    return nullptr;
}

void RobotAssignmentModule::appendLog(int rid, const string& rname, const string& task) {
    LogNode* entry = new LogNode(++logCount, rid, rname, task);
    if (logHead == nullptr) {
        logHead = logTail = entry;
    }
    else {
        logTail->next = entry;
        logTail = entry;
    }
}

void RobotAssignmentModule::advanceRotation() {
    if (count == 0) return;
    nextRobot = (nextRobot + 1) % count;
}

void RobotAssignmentModule::printDivider(int width, char ch) const {
    for (int i = 0; i < width; i++) cout << ch;
    cout << "\n";
}

string RobotAssignmentModule::intToStr(int n) const {
    if (n == 0) return "0";
    string result = "";
    while (n > 0) {
        result = static_cast<char>('0' + n % 10) + result;
        n /= 10;
    }
    return result;
}

// Setup

void RobotAssignmentModule::initDefaultRobots(int n) {
    if (n <= 0) {
        cout << "[WARNING] Count must be positive.\n";
        return;
    }
    cout << "\n[INIT] Registering " << n << " robot(s)...\n";
    for (int i = 1; i <= n; i++)
        enqueueRobot(i, "Robot-" + intToStr(i));
    cout << "[INIT] Done. " << count << " robot(s) ready.\n";
}

bool RobotAssignmentModule::enqueueRobot(int id, const string& name) {
    if (findRobotNode(id) != nullptr) {
        cout << "[WARNING] Robot ID " << id << " already exists. Skipping.\n";
        return false;
    }

    QueueNode* newNode = new QueueNode(Robot(id, name));

    if (front == nullptr) {
        newNode->next = newNode;
        front = newNode;
        rear = newNode;
    }
    else {
        newNode->next = front;
        rear->next = newNode;
        rear = newNode;
    }

    count++;
    cout << "[ENQUEUED] " << name << " (ID: " << id << ")  |  Queue size: " << count << "\n";
    return true;
}

bool RobotAssignmentModule::dequeueRobot() {
    if (isEmpty()) {
        cout << "[ERROR] Queue is empty.\n";
        return false;
    }

    QueueNode* target = front;

    if (count == 1) {
        front = nullptr;
        rear = nullptr;
    }
    else {
        front = front->next;
        rear->next = front;

        // If nextRobot pointed at or past front, nudge it back
        if (nextRobot >= count - 1) nextRobot = 0;
    }

    cout << "[DEQUEUED] " << target->data.name << " (ID: " << target->data.id << ")\n";
    delete target;
    count--;
    return true;
}

// Core Operations

bool RobotAssignmentModule::assignTask(const string& taskDesc) {
    if (isEmpty()) {
        cout << "[ERROR] No robots in the queue.\n";
        return false;
    }
    if (taskDesc.empty()) {
        cout << "[ERROR] Task description is empty.\n";
        return false;
    }

    // Walk the queue starting from nextRobot
    QueueNode* ptr = front;
    for (int i = 0; i < nextRobot; i++) ptr = ptr->next;

    QueueNode* start = ptr;

    do {
        if (ptr->data.status == "Available") {
            ptr->data.status = "Busy";
            ptr->data.currentTask = taskDesc;
            ptr->data.taskCount++;
            appendLog(ptr->data.id, ptr->data.name, taskDesc);

            cout << "[ASSIGNED] \"" << taskDesc << "\"\n"
                << "           --> " << ptr->data.name
                << " (ID: " << ptr->data.id << ")\n";

            advanceRotation();
            return true;
        }

        cout << "[SKIP] " << ptr->data.name << " is " << ptr->data.status << ".\n";
        ptr = ptr->next;
        advanceRotation();

    } while (ptr != start);

    cout << "[ERROR] All " << count << " robot(s) are Busy or under Maintenance.\n";
    return false;
}

bool RobotAssignmentModule::completeTask(int robotID) {
    QueueNode* node = findRobotNode(robotID);
    if (node == nullptr) {
        cout << "[ERROR] Robot ID " << robotID << " not found.\n";
        return false;
    }
    if (node->data.status != "Busy") {
        cout << "[WARNING] " << node->data.name << " is not Busy (Status: " << node->data.status << ").\n";
        return false;
    }
    cout << "[COMPLETED] " << node->data.name << " finished: \"" << node->data.currentTask << "\"\n";
    node->data.status = "Available";
    node->data.currentTask = "None";
    return true;
}

bool RobotAssignmentModule::setMaintenance(int robotID) {
    QueueNode* node = findRobotNode(robotID);
    if (node == nullptr) {
        cout << "[ERROR] Robot ID " << robotID << " not found.\n";
        return false;
    }
    if (node->data.status == "Maintenance") {
        cout << "[WARNING] " << node->data.name << " is already under Maintenance.\n";
        return false;
    }
    cout << "[MAINTENANCE] " << node->data.name << " taken offline.\n";
    node->data.status = "Maintenance";
    node->data.currentTask = "None";
    return true;
}

bool RobotAssignmentModule::restoreRobot(int robotID) {
    QueueNode* node = findRobotNode(robotID);
    if (node == nullptr) {
        cout << "[ERROR] Robot ID " << robotID << " not found.\n";
        return false;
    }
    if (node->data.status == "Available") {
        cout << "[WARNING] " << node->data.name << " is already Available.\n";
        return false;
    }
    cout << "[RESTORED] " << node->data.name << " is back online.\n";
    node->data.status = "Available";
    node->data.currentTask = "None";
    return true;
}

// Display

void RobotAssignmentModule::displayAllRobots() const {
    cout << "\n";
    printDivider(72, '=');
    cout << "  ROBOT ASSIGNMENT TABLE  (" << count << " robot(s) in queue)\n";
    printDivider(72, '=');

    if (isEmpty()) {
        cout << "  (no robots registered)\n";
        printDivider(72, '=');
        return;
    }

    cout << left
        << setw(6) << "ID"
        << setw(14) << "Name"
        << setw(15) << "Status"
        << setw(8) << "Tasks"
        << "Current Task\n";
    printDivider(72, '-');

    QueueNode* ptr = front;
    int idx = 0;
    do {
        string marker = (idx == nextRobot) ? "  <-- next" : "";
        cout << left
            << setw(6) << ptr->data.id
            << setw(14) << ptr->data.name
            << setw(15) << ptr->data.status
            << setw(8) << ptr->data.taskCount
            << ptr->data.currentTask << marker << "\n";
        ptr = ptr->next;
        idx++;
    } while (ptr != front);

    printDivider(72, '=');
    cout << "  \"<-- next\" = next robot in the rotation\n\n";
}

void RobotAssignmentModule::displayAssignmentLog() const {
    cout << "\n";
    printDivider(68, '=');
    cout << "  ASSIGNMENT LOG  (" << logCount << " record(s))\n";
    printDivider(68, '=');

    if (logHead == nullptr) {
        cout << "  (no assignments recorded yet)\n";
        printDivider(68, '=');
        return;
    }

    cout << left
        << setw(6) << "Log#"
        << setw(8) << "RobotID"
        << setw(14) << "Robot Name"
        << "Task\n";
    printDivider(68, '-');

    LogNode* ptr = logHead;
    while (ptr != nullptr) {
        cout << left
            << setw(6) << ptr->logID
            << setw(8) << ptr->robotID
            << setw(14) << ptr->robotName
            << ptr->taskDesc << "\n";
        ptr = ptr->next;
    }
    printDivider(68, '=');
    cout << "\n";
}

void RobotAssignmentModule::displayStatusSummary() const {
    cout << "\n";
    printDivider(36, '=');
    cout << "  ROBOT STATUS SUMMARY\n";
    printDivider(36, '-');

    if (isEmpty()) {
        cout << "  No robots registered.\n";
        printDivider(36, '=');
        return;
    }

    int available = 0, busy = 0, maintenance = 0;
    QueueNode* ptr = front;
    do {
        if (ptr->data.status == "Available")   available++;
        else if (ptr->data.status == "Busy")        busy++;
        else                                        maintenance++;
        ptr = ptr->next;
    } while (ptr != front);

    cout << left
        << setw(18) << "Total Robots" << ": " << count << "\n"
        << setw(18) << "Available" << ": " << available << "\n"
        << setw(18) << "Busy" << ": " << busy << "\n"
        << setw(18) << "Maintenance" << ": " << maintenance << "\n";
    printDivider(36, '=');
    cout << "\n";
}