#ifndef BST_H
#define BST_H

#include "ItemNode.h"

// BST class to manage warehouse items
// Items are organized by itemID as the BST key
class BST
{
private:
    ItemNode *root;                                     // pointer to the root node of the tree
    void inorder(ItemNode *current);                    // helper function, public environment doesnt need this
    ItemNode *deleteNode(ItemNode *current, string id); // helper for delete (helper function)

public:
    BST(); // Constructor - initializes empty tree
    void insert(string id, string name, string location);
    void display();
    ItemNode *search(string id); // allows other modules in the system to get item's location immediately
    void update(string id);
    void deleteItem(string id);
};

#endif