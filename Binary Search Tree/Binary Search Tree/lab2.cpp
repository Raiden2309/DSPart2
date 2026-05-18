//#include <iostream>
//using namespace std;
//
//struct TreeNode {
//    char data;
//    TreeNode* left;
//    TreeNode* right;
//};
//
//class BinaryTree {
//
//public:
//
//    TreeNode* createNode(char value) {
//        TreeNode* newNode = new TreeNode;
//        newNode->data = value;
//        newNode->left = NULL;
//        newNode->right = NULL;
//        return newNode;
//    }
//
//    // Build tree using FORMULA (2*i+1, 2*i+2)
//    TreeNode* buildTree(char arr[]) {
//
//        TreeNode* nodes[6];
//
//        // Step 1: create nodes
//        for (int i = 0; i < 6; i++) {
//            nodes[i] = createNode(arr[i]);
//        }
//
//        // Step 2: link using formula
//        for (int i = 0; i < 6; i++) {
//
//            int leftIndex = 2 * i + 1;
//            int rightIndex = 2 * i + 2;
//
//            if (leftIndex < 6)
//                nodes[i]->left = nodes[leftIndex];
//
//            if (rightIndex < 6)
//                nodes[i]->right = nodes[rightIndex];
//        }
//
//        return nodes[0];
//    }
//
//    int countNodes(TreeNode* root) {
//        if (root == NULL) return 0;
//        return 1 + countNodes(root->left) + countNodes(root->right);
//    }
//
//    int height(TreeNode* root) {
//        if (root == NULL) return 0;
//
//        int l = height(root->left);
//        int r = height(root->right);
//
//        return 1 + max(l, r);
//    }
//
//    void preorder(TreeNode* root) {
//        if (root == NULL) return;
//        cout << root->data << " ";
//        preorder(root->left);
//        preorder(root->right);
//    }
//
//    void inorder(TreeNode* root) {
//        if (root == NULL) return;
//        inorder(root->left);
//        cout << root->data << " ";
//        inorder(root->right);
//    }
//
//    void postorder(TreeNode* root) {
//        if (root == NULL) return;
//        postorder(root->left);
//        postorder(root->right);
//        cout << root->data << " ";
//    }
//};
//
//int main() {
//
//    BinaryTree t;
//
//    char data[6] = { 'A','B','C','D','E','F' };
//
//    TreeNode* root = t.buildTree(data);
//
//    cout << "Preorder: ";
//    t.preorder(root);
//    cout << endl;
//
//    cout << "Inorder: ";
//    t.inorder(root);
//    cout << endl;
//
//    cout << "Postorder: ";
//    t.postorder(root);
//    cout << endl;
//
//    cout << "Total Nodes: " << t.countNodes(root) << endl;
//    cout << "Height: " << t.height(root) << endl;
//
//    return 0;
//}