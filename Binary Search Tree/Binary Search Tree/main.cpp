// ============================
// Binary Search Tree
// Insert, Traversal, Delete
// ============================

#include <iostream>
using namespace std;

// ============================
// Tree Node Structure
// ============================
struct TreeNode
{
    int data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int v)
    {
        data = v;
        left = right = NULL;
    }
};

// ============================
// BST CLASS
// ============================
class BST
{
private:
    TreeNode* root = NULL;

    // INSERT
    TreeNode* insert(TreeNode* node, int key)
    {
        if (node == NULL)
            return new TreeNode(key);

        if (key < node->data)
            node->left = insert(node->left, key);
        else if (key > node->data)
            node->right = insert(node->right, key);

        return node;
    }

    // TRAVERSALS
    void inorder(TreeNode* node)
    {
        if (node == NULL) return;

        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }

    void preorder(TreeNode* node)
    {
        if (node == NULL) return;

        cout << node->data << " ";
        preorder(node->left);
        preorder(node->right);
    }

    void postorder(TreeNode* node)
    {
        if (node == NULL) return;

        postorder(node->left);
        postorder(node->right);
        cout << node->data << " ";
    }

    // TREE PROPERTIES
    int countNodes(TreeNode* node)
    {
        if (node == NULL) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    int sumNodes(TreeNode* node)
    {
        if (node == NULL) return 0;
        return node->data + sumNodes(node->left) + sumNodes(node->right);
    }

    int height(TreeNode* node)
    {
        if (node == NULL) return -1;
        return 1 + max(height(node->left), height(node->right));
    }

    int minValue(TreeNode* node)
    {
        while (node->left != NULL)
            node = node->left;
        return node->data;
    }

    int maxValue(TreeNode* node)
    {
        while (node->right != NULL)
            node = node->right;
        return node->data;
    }

public:

    void insert(int key)
    {
        root = insert(root, key);
    }

    void inorder()
    {
        inorder(root);
    }

    void preorder()
    {
        preorder(root);
    }

    void postorder()
    {
        postorder(root);
    }

    int countNodes()
    {
        return countNodes(root);
    }

    int sumNodes()
    {
        return sumNodes(root);
    }

    int height()
    {
        return height(root);
    }

    int minValue()
    {
        return minValue(root);
    }

    int maxValue()
    {
        return maxValue(root);
    }

    // ============================
    // DELETE NODE (ITERATIVE)
    // ============================
    void deleteNode(int key)
    {
        TreeNode* parent = NULL;
        TreeNode* node = root;

        // search node
        while (node != NULL && node->data != key)
        {
            parent = node;

            if (key < node->data)
                node = node->left;
            else
                node = node->right;
        }

        if (node == NULL)
        {
            cout << "Key not found!" << endl;
            return;
        }

        // CASE 1: leaf node
        if (node->left == NULL && node->right == NULL)
        {
            if (parent == NULL)
                root = NULL;
            else if (parent->left == node)
                parent->left = NULL;
            else
                parent->right = NULL;

            delete node;
        }

        // CASE 2: one child (right)
        else if (node->left == NULL)
        {
            if (parent == NULL)
                root = node->right;
            else if (parent->left == node)
                parent->left = node->right;
            else
                parent->right = node->right;

            delete node;
        }

        // CASE 3: one child (left)
        else if (node->right == NULL)
        {
            if (parent == NULL)
                root = node->left;
            else if (parent->left == node)
                parent->left = node->left;
            else
                parent->right = node->left;

            delete node;
        }

        // CASE 4: two children
        else
        {
            TreeNode* succParent = node;
            TreeNode* succ = node->right;

            while (succ->left != NULL)
            {
                succParent = succ;
                succ = succ->left;
            }

            node->data = succ->data;

            if (succParent->left == succ)
                succParent->left = succ->right;
            else
                succParent->right = succ->right;

            delete succ;
        }
    }
};

// ============================
// MAIN FUNCTION
// ============================
int main()
{
    BST tree;

    int values[] = { 11, 6, 8, 19, 4, 10, 5, 17, 43, 49, 31 };
    int n = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < n; i++)
        tree.insert(values[i]);

    cout << "Inorder: ";
    tree.inorder();

    cout << "\nPreorder: ";
    tree.preorder();

    cout << "\nPostorder: ";
    tree.postorder();

    cout << "\n\nHeight: " << tree.height();
    cout << "\nTotal Nodes: " << tree.countNodes();
    cout << "\nTotal Sum: " << tree.sumNodes();
    cout << "\nMin Value: " << tree.minValue();
    cout << "\nMax Value: " << tree.maxValue();

    cout << "\n\nDeleting 19...";
    tree.deleteNode(19);

    cout << "\nInorder after delete: ";
    tree.inorder();

    return 0;
}