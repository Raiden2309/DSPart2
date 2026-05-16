#ifndef ROBOT_ASSIGNMENT_HPP
#define ROBOT_ASSIGNMENT_HPP

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

struct Robot {
    int    id;
    string name;
    string status;
    string currentTask;
    int    taskCount;

    Robot() : id(0), name(""), status("Available"),
        currentTask("None"), taskCount(0) {
    }

    Robot(int i, const string& n)
        : id(i), name(n), status("Available"),
        currentTask("None"), taskCount(0) {
    }
};

struct QueueNode {
    Robot      data;
    QueueNode* next;

    QueueNode(const Robot& r) : data(r), next(nullptr) {}
};

struct LogNode {
    int      logID;
    int      robotID;
    string   robotName;
    string   taskDesc;
    LogNode* next;

    LogNode(int lid, int rid, const string& rname, const string& task)
        : logID(lid), robotID(rid), robotName(rname),
        taskDesc(task), next(nullptr) {
    }
};

class RobotAssignmentModule {

private:
    QueueNode* front;
    QueueNode* rear;
    int        count;
    int        nextRobot;

    LogNode* logHead;
    LogNode* logTail;
    int        logCount;

    void       appendLog(int robotID, const string& robotName, const string& task);
    void       clearLog();
    QueueNode* findRobotNode(int robotID) const;
    void       advanceRotation();
    void       printDivider(int width = 60, char ch = '-') const;
    string     intToStr(int n) const;

public:
    RobotAssignmentModule();
    ~RobotAssignmentModule();

    bool enqueueRobot(int id, const string& name);
    bool dequeueRobot();
    void initDefaultRobots(int n = 5);

    bool assignTask(const string& taskDesc);
    bool completeTask(int robotID);
    bool setMaintenance(int robotID);
    bool restoreRobot(int robotID);

    void displayAllRobots()     const;
    void displayAssignmentLog() const;
    void displayStatusSummary() const;

    bool isEmpty()        const { return count == 0; }
    int  getTotalRobots() const { return count; }
    int  getLogCount()    const { return logCount; }
};

#endif