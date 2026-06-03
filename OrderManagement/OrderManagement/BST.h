//Name: Eu Xiao Xuan
//TP No: TP077171

#ifndef BST_H
#define BST_H

#include "ItemNode.h"

class BST
{
private:
    ItemNode* root;
    void inorder(ItemNode* current);
    ItemNode* deleteNode(ItemNode* current, string id);

    // Private helper for matching string descriptions sequentially
    ItemNode* searchByNameHelper(ItemNode* current, string targetName);

public:
    BST();
    void insert(string id, string name, string location);
    void display();
    ItemNode* search(string id); // This searches by ID key (e.g., "INV01")

    // Public function to search by Name string (e.g., "Desk Lamp")
    ItemNode* searchByName(string targetName);

    void update(string id);
    void deleteItem(string id);
    void loadInventoryFromCSV();
};

#endif