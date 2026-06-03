//Name: Eu Xiao Xuan
//TP No: TP077171

#include "BST.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

BST::BST()
{
    root = nullptr;
} // constructor: When start a object then constructor will run automatically

void BST::insert(string id, string name, string location)
{
    // Create new node
    ItemNode *newNode = new ItemNode(id, name, location); // create a new item

    if (root == nullptr)
    {                   // If tree is empty
        root = newNode; // If empty then will be root automatically
        return;
    }

    ItemNode *current = root; // current start from root(BST search/insert will always starts with root)
    while (true)
    { // keep finding position till it inserted sucessfully
        // If ID duplicated
        if (id == current->itemID)
        {
            cout << "Error: Item ID already exists!" << endl;
            delete newNode;
            return;
        }
        // Go left
        if (id < current->itemID)
        { // when the current is higher than id then move to left)
            if (current->left == nullptr)
            {                            // Is there got empty position in left?
                current->left = newNode; // If empty than place it in left
                return;
            }
            current = current->left; // continue Go left if there is no place in left
        }
        else
        {
            if (current->right == nullptr)
            {
                current->right = newNode;
                return;
            }
            current = current->right; // move to Right
        }
    }
}
void BST ::display() // function that user use
{
    if (root == nullptr) // empty tree checking
    {
        cout << "No items in the system." << endl;
        return;
    }
    inorder(root); // start with root (inorder)
}
void BST::inorder(ItemNode *current)
{
    // stop when reach empty node
    if (current == nullptr)
    {
        return;
    }
    inorder(current->left); // go left

    // display current node
    cout << "Item ID: " << current->itemID << endl;
    cout << "Item Name: " << current->itemName << endl;
    cout << "Location: " << current->location << endl;
    cout << endl;

    inorder(current->right); // go right
}
ItemNode *BST::search(string id)
{
    ItemNode *current = root;  // Search start from root
    while (current != nullptr) // when current still have node then continue searching
    {
        // Item Found
        if (current->itemID == id) // comparing current node id
        {
            cout << "Item Found!" << endl;
            cout << "Item ID: " << current->itemID << endl;
            cout << "Item Name: " << current->itemName << endl;
            cout << "Location: " << current->location << endl;
            return current; // can use current to get location info (BST.h in public class mentioned)
        }

        // Go left
        else if (id < current->itemID) // if target is smaller than current then go left
        {
            current = current->left;
        }
        // Go right
        else
        { // otherwise, right
            current = current->right;
        }
    }
    // Item not found
    cout << "Item not found!" << endl; // when the tree doesnt have that item, current = nullptr
    return nullptr;                    // give us a empty answer
}
void BST::update(string id)
{
    ItemNode *current = search(id); // Search for the item first
    if (current == nullptr)         // if item not found, search() already prints error
    {
        return;
    }
    int choice;
    cout << "What would you like to update? " << endl;
    cout << "1. Item Name " << endl;
    cout << "2. Location " << endl;
    cout << "3. Both " << endl;
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore(); // ignore the \n from choice, then wait for next answer

    if (choice == 1)
    {
        string newName;
        cout << "Enter new Item Name: ";
        getline(cin, newName); // getline reads the full line including spaces  // cin>> would stop at first space, causing incomplete name
        current->itemName = newName;
    }
    else if (choice == 2)
    {
        string newLocation;
        cout << "Enter new Location: ";
        getline(cin, newLocation);
        current->location = newLocation;
    }
    else if (choice == 3)
    {
        string newName, newLocation;
        cout << "Enter new Item Name: ";
        getline(cin, newName);
        cout << "Enter new Location: ";
        getline(cin, newLocation);
        current->itemName = newName;
        current->location = newLocation;
    }
    else
    {
        cout << "Invalid choice!" << endl;
        return;
    }

    // Show updated result
    cout << "\nItem updated successfully !" << endl;
    cout << "ItemID: " << current->itemID << endl;
    cout << "Item Name: " << current->itemName << endl;
    cout << "Location: " << current->location << endl;
}
void BST::deleteItem(string id)
{
    // Check if tree is empty
    if (root == nullptr)
    {
        cout << "System is empty, nothing to delete!" << endl;
        return;
    }
    // Check if item exists before deleting
    ItemNode *check = search(id);
    if (check == nullptr)
    {
        return; // search() already print "Item not found!"
    }
    // Proceed with deletion
    root = deleteNode(root, id);
    cout << "Item " << id << "deleted successfully!" << endl;
}
ItemNode *BST::deleteNode(ItemNode *current, string id)
{
    // If Empty Node
    if (current == nullptr)
    {
        return nullptr;
    }
    // Step 1: Find the node to delete
    if (id < current->itemID)
    {
        current->left = deleteNode(current->left, id); // go left
    }
    else if (id > current->itemID)
    {
        current->right = deleteNode(current->right, id); // go right
    }
    else
    {
        // Node found! Now need to handle 3 cases:
        // Case 1: 0 child (leaf Node) - just delete
        if (current->left == nullptr && current->right == nullptr)
        {
            delete current;
            return nullptr;
        }

        // Case 2:1 chile -replace with child
        else if (current->left == nullptr)
        {
            // Only has right child
            ItemNode *temp = current->right;
            delete current;
            return temp;
        }
        else if (current->right == nullptr)
        {
            // Only has left child
            ItemNode *temp = current->left;
            delete current;
            return temp;
        }
        // Case 3:  2 children - find inorder successor
        else
        {
            // Find inorder successor (go right once, then all the way left)
            ItemNode *successor = current->right;
            while (successor->left != nullptr)
            {
                successor = successor->left; // keep going left
            }

            // Copy successor's data into current node
            current->itemID = successor->itemID;
            current->itemName = successor->itemName;
            current->location = successor->location;

            // delete the successor from right subtree
            current->right = deleteNode(current->right, successor->itemID);
        }
    }
    return current;
}
void BST::loadInventoryFromCSV() {
    ifstream file("inventory.csv");
    if (!file.is_open()) {
        cerr << "[ERROR] Could not open inventory.csv. Inventory index is empty!\n";
        return;
    }

    string line;
    getline(file, line); // Skip headers

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line.back() == '\r') {
            line.pop_back();
        }

        size_t firstComma = line.find(',');
        if (firstComma == string::npos) continue;

        size_t secondComma = line.find(',', firstComma + 1);
        if (secondComma == string::npos) continue;

        string id = line.substr(0, firstComma);
        string name = line.substr(firstComma + 1, secondComma - firstComma - 1);
        string location = line.substr(secondComma + 1);

        if (!id.empty() && !name.empty() && !location.empty()) {
            // FIX: Passing 'id' as the first parameter sets it as the primary sorting key
            insert(id, name, location);
        }
    }
    file.close();
}

// 2. NEW FUNCTION: Public wrapper to initiate an item name search
ItemNode* BST::searchByName(string targetName) {
    return searchByNameHelper(root, targetName);
}

// 3. NEW FUNCTION: Recursive function that traverses tree nodes to find an item name match
ItemNode* BST::searchByNameHelper(ItemNode* current, string targetName) {
    if (current == nullptr) {
        return nullptr;
    }

    // Check if the current node matches the requested item name
    if (current->itemName == targetName) {
        return current;
    }

    // Search the left subtree
    ItemNode* leftResult = searchByNameHelper(current->left, targetName);
    if (leftResult != nullptr) {
        return leftResult;
    }

    // Search the right subtree
    return searchByNameHelper(current->right, targetName);
}
