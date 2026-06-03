//Name: Ivy Ling Ke Lin
//TP No: TP076996

#include "RobotNavigation.hpp"

RobotNavigationModule::RobotNavigationModule()
{
    top = NULL;
}

RobotNavigationModule::~RobotNavigationModule()
{
    MoveNode* temp;

    while (top != NULL)
    {
        temp = top;
        top = top->next;
        delete temp;
    }
}

bool RobotNavigationModule::isEmpty()
{
    return top == NULL;
}

void RobotNavigationModule::recordMove(string move)
{
    MoveNode* newNode = new MoveNode;

    newNode->move = move;
    newNode->next = top;

    top = newNode;

    cout << "\n[SYSTEM] Movement Recorded : "
         << move << endl;
}

string RobotNavigationModule::popMove()
{
    if (isEmpty())
    {
        return "";
    }

    MoveNode* temp = top;

    string move = temp->move;

    top = top->next;

    delete temp;

    return move;
}

void RobotNavigationModule::displayNavigationLog()
{
    if (isEmpty())
    {
        cout << "\n[SYSTEM] No Navigation Record Found.\n";
        return;
    }

    cout << "\n====================================";
    cout << "\n        NAVIGATION LOG";
    cout << "\n====================================\n";

    MoveNode* current = top;

    int step = 1;

    while (current != NULL)
    {
        cout << "Step "
             << step
             << " : "
             << current->move
             << endl;

        current = current->next;
        step++;
    }
}

void RobotNavigationModule::returnToStart()
{
    if (isEmpty())
    {
        cout << "\n[SYSTEM] No Route Available.\n";
        return;
    }

    cout << "\n====================================";
    cout << "\n      ROBOT RETURNING";
    cout << "\n====================================\n";

    while (!isEmpty())
    {
        cout << popMove() << endl;
    }

    cout << "\n[SYSTEM] Robot Returned Successfully.\n";
}

void RobotNavigationModule::obstacleDetected()
{
    cout << "\n[WARNING] Obstacle Detected.\n";

    if (isEmpty())
    {
        cout << "[SYSTEM] No Recorded Path Available.\n";
        return;
    }

    cout << "[SYSTEM] Backtracking Current Route...\n";

    MoveNode* current = top;

    while (current != NULL)
    {
        cout << current->move << endl;
        current = current->next;
    }

    cout << "[SYSTEM] Alternative Route Required.\n";
}