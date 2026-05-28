#ifndef ITEMNODE_H
#define ITEMNODE_H

#include <iostream>
using namespace std;

// represents a single node in the binary search tree
// Each node stores one warehouse item's details
class ItemNode
{
public:
    string itemID;
    string itemName;
    string location;

    ItemNode *left;
    ItemNode *right;

    ItemNode(string id, string name, string loc)
    {
        itemID = id;
        itemName = name;
        location = loc;

        left = nullptr;
        right = nullptr;
    }
};

#endif