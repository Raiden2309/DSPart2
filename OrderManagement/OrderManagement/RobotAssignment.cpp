#include "RobotAssignment.hpp"

RobotAssignmentModule::RobotAssignmentModule()
    : front(nullptr), back(nullptr), count(0), nextRobot(nullptr),
    logHead(nullptr), logTail(nullptr), logCount(0) {
}


RobotAssignmentModule::~RobotAssignmentModule() {
    // Break the circular link so we can walk it like a normal list
    if (back != nullptr) back->next = nullptr;
    QueueNode* ptr = front;
    while (ptr != nullptr) {
        QueueNode* tmp = ptr;
        ptr = ptr->next;
        delete tmp; // Free each node individually
    }
    clearLog();
}

void RobotAssignmentModule::clearLog() {
    // Traverse the singly-linked log and delete every node
    LogNode* ptr = logHead;
    while (ptr != nullptr) {
        LogNode* tmp = ptr;
        ptr = ptr->next;
        delete tmp;
    }
    logHead = logTail = nullptr;
    logCount = 0;
}

// Searches the circular queue for a robot by ID.
// Returns a pointer to its node, or nullptr if not found.
QueueNode* RobotAssignmentModule::findRobotNode(int robotID) const {
    if (front == nullptr) return nullptr;
    QueueNode* ptr = front;
    // Use do-while so we check front before looping back to it
    do {
        if (ptr->data.id == robotID) return ptr;
        ptr = ptr->next;
    } while (ptr != front);
    return nullptr;
}

void RobotAssignmentModule::printDivider(int width, char ch) const {
    for (int i = 0; i < width; i++) cout << ch;
    cout << "\n";
}

string RobotAssignmentModule::intToStr(int n) const {
    if (n == 0) return "0";
    string result;
    while (n > 0) {
        result = static_cast<char>('0' + n % 10) + result;
        n /= 10;
    }
    return result;
}

void RobotAssignmentModule::appendLog(int rid, const string& rname, const string& task) {
    ++logCount;
    LogNode* entry = new LogNode(logCount, rid, rname, task);
    if (logHead == nullptr) {
        logHead = logTail = entry;
    }
    else {
        // Append to the tail of the singly-linked log list
        logTail->next = entry;
        logTail = entry;
    }
}

void RobotAssignmentModule::loadRobotsFromCSV() {
    ifstream file("robots.csv");
    if (!file.is_open()) {
        cout << "[FILE] " << "robots.csv" << " not found. Initialising default robots.\n";
        initDefaultRobots();
        return;
    }

    // Skip the header row, no values
    string header;
    getline(file, header);

    int loaded = 0;
    string idStr, name, status, currentTask, taskCountStr;

    while (file.good()) {
        getline(file, idStr, ',');
        if (idStr.empty()) break;

        getline(file, name, ',');
        getline(file, status, ',');
        getline(file, currentTask, ',');
        getline(file, taskCountStr);

        // Strip Windows-style carriage return if present
		// CSV in windows saves with \r\n, so the last field may have a trailing \r
        if (!taskCountStr.empty() && taskCountStr.back() == '\r')
            taskCountStr.pop_back();

        // Manual string-to-int
        // Easier to search for and lesser memory used
        int id = 0;
        for (char c : idStr)
            if (c >= '0' && c <= '9') id = id * 10 + (c - '0');

        int taskCount = 0;
        for (char c : taskCountStr)
            if (c >= '0' && c <= '9') taskCount = taskCount * 10 + (c - '0');

        if (id <= 0) continue;

        Robot r(id, name);
        r.status = status;
        r.currentTask = currentTask;
        r.taskCount = taskCount;

        QueueNode* newNode = new QueueNode(r);
        if (front == nullptr) {
            // First node will point to itself to form the circle
            newNode->next = newNode;
            front = back = newNode;
            nextRobot = newNode;
        }
        else {
            // Link new node at the back, pointing forward to front to maintain circular link
            newNode->next = front;
            back->next = newNode;
            back = newNode;
        }
        count++;
        loaded++;
    }

    file.close();

    if (loaded == 0) {
        cout << "[FILE] " << "robots.csv" << " was empty. Initialising default robots.\n";
        initDefaultRobots();
    }
}

void RobotAssignmentModule::initDefaultRobots(int n) {
    if (n <= 0) {
        cout << "[WARNING] Count must be positive.\n";
        return;
    }
    cout << "\n[INIT] Registering " << n << " default robot(s)...\n";
    for (int i = 1; i <= n; i++)
        enqueueRobot(i, "Robot-" + intToStr(i));
    cout << "[INIT] Done. " << count << " robot(s) ready.\n";
}

bool RobotAssignmentModule::enqueueRobot(int id, const string& name) {
    // Validate inputs before doing anything
    if (id <= 0) {
        cout << "[ERROR] Invalid Robot ID: " << id << ". ID must be a positive integer.\n";
        return false;
    }
    if (name.empty()) {
        cout << "[ERROR] Robot name cannot be empty.\n";
        return false;
    }
    if (findRobotNode(id) != nullptr) {
        cout << "[WARNING] Robot ID " << id << " already exists. Skipping.\n";
        return false;
    }

    QueueNode* newNode = new QueueNode(Robot(id, name));
    if (front == nullptr) {
        // Empty queue — node points to itself to start the circle
        newNode->next = newNode;
        front = back = newNode;
        nextRobot = newNode;     
    }
    else {
        // Attach new node at the back, maintaining the circular link
        newNode->next = front;
        back->next = newNode;
        back = newNode;
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
        // Only one robot — reset all pointers
        front = back = nextRobot = nullptr;
    }
    else {
        // If the rotation pointer was on front, advance it first
        if (nextRobot == front)
            nextRobot = front->next;

        // Move front forward and re-link back to maintain the circle
        front = front->next;
        back->next = front;
    }

    cout << "[DEQUEUED] " << target->data.name << " (ID: " << target->data.id << ")\n";
    delete target;
    count--;
    return true;
}

bool RobotAssignmentModule::assignTask(const string& taskDesc) {
    if (isEmpty()) {
        cout << "[ERROR] No robots in the queue.\n";
        return false;
    }
    if (taskDesc.empty()) {
        cout << "[ERROR] Task description is empty.\n";
        return false;
    }

    // Remember where rotation started so we know when we've done a full loop
    QueueNode* start = nextRobot;
    QueueNode* ptr = nextRobot;

    do {
        if (ptr->data.status == "Available") {

            // Assign task and update robot state
            ptr->data.status = "Busy";
            ptr->data.currentTask = taskDesc;
            ptr->data.taskCount++;
            appendLog(ptr->data.id, ptr->data.name, taskDesc);

            cout << "[ASSIGNED] \"" << taskDesc << "\"\n"
                << "           --> " << ptr->data.name
                << " (ID: " << ptr->data.id << ")\n";

			// Move to next robot for the next assignment, rotate fairly through the queue
            nextRobot = ptr->next; 
            return true;
        }

        cout << "[SKIP] " << ptr->data.name << " is " << ptr->data.status << ".\n";
        ptr = ptr->next;

    } while (ptr != start);  // Stop when looped back to where it started

    cout << "[ERROR] All " << count << " robot(s) are Busy or under Maintenance.\n";
    return false;
}

bool RobotAssignmentModule::completeTask(int robotID) {
    QueueNode* node = findRobotNode(robotID);
    if (node == nullptr) {
        cout << "[ERROR] Robot ID " << robotID << " not found.\n";
        return false;
    }

    // Can only complete a task if the robot is actually working on one
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

    // Guard against setting maintenance on an already offline robot
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

    // No need to restore a robot that is already available
    if (node->data.status != "Maintenance") {
        cout << "[WARNING] " << node->data.name << " cannot be restored (Status: " << node->data.status << "). Only Maintenance robots can be restored.\n";
        return false;
    }

    cout << "[RESTORED] " << node->data.name << " is back online.\n";
    node->data.status = "Available";
    node->data.currentTask = "None";
    return true;
}

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
    do {
        string marker = (ptr == nextRobot) ? "  <-- next" : "";
        cout << left
            << setw(6) << ptr->data.id
            << setw(14) << ptr->data.name
            << setw(15) << ptr->data.status
            << setw(8) << ptr->data.taskCount
            << ptr->data.currentTask << marker << "\n";
        ptr = ptr->next;
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
        if (ptr->data.status == "Available") available++;
        else if (ptr->data.status == "Busy")      busy++;
        else                                      maintenance++;
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