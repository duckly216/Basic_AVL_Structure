/*
* Created by jorge on 6/2/2025.
* Bibliography:
* 1. https://www.geeksforgeeks.org/deletion-in-binary-search-tree/
* 2. https://www.geeksforgeeks.org/deletion-in-an-avl-tree/
* 3. https://www.geeksforgeeks.org/level-order-tree-traversal/
*/

#include "AVL.h"

#include <queue>
// --- --- --- --- --- --- AVL Tree Methods --- --- --- --- --- --- //

// --- --- --- Internal Helper Methods --- --- --- //

TreeNode* AVL::inter_insert(TreeNode* node, const Student& newStu)
{
    // Inserts new node when it finds a space
    if (node == nullptr)
        return new TreeNode(newStu);
    if (newStu.GatorID < node->getGatorID())
        node->left = inter_insert(node->left, newStu);
    else if (newStu.GatorID > node->getGatorID())
        node->right = inter_insert(node->right, newStu);
    // Duplicate found, does not insert
    else
        return node;
    // Updates height "bottom -to- up"
    node->height = 1+ max(getHeight(node->left), getHeight(node->right));

    // Balances if needed
    return balance(node);
}
/* Implementation based on sources 1 and 2
 */
TreeNode* AVL::inter_remove(TreeNode* node, string ID, bool& verifier){

    // key Not Found
    if(node == nullptr)
        return nullptr;
    else if (ID < node->getGatorID() )
        node->left = inter_remove(node->left, ID, verifier);
    else if (ID > node->getGatorID())
        node->right = inter_remove(node->right, ID,  verifier);
    else if (ID == node->getGatorID()){ // ID is in the node
        verifier = true;
        // Root has no children or has one child
        if(node->left == nullptr || node->right == nullptr)
        {
            // If node left is not a nullptr, child is the left node, else it is right node
            TreeNode* child = node->left ? node->left : node->right;
            delete node;
            return child;

        }
        // Root has two children
        else
        {
            // Removing INORDER SUCCESSOR
            TreeNode* successor = node->right;
            while (successor->left != nullptr)
                successor = successor->left;
            node->_student = successor->_student;
            node->right = inter_remove(node->right, successor->getGatorID(), verifier);
        }
    }
    if (node == nullptr)
        return node;
    // Updates height "bottom -to- up"
    node->height = 1+ max(getHeight(node->left), getHeight(node->right));
    return balance(node);
}

// Looks for the node that matches the ID
TreeNode* AVL::inter_search(TreeNode* node, string ID)
{
    if (node == nullptr)
        return nullptr;
    else if (node->getGatorID() == ID)
        return node;
    else if (ID < node->getGatorID())
        return inter_search(node->left, ID);
    else if (ID > node->getGatorID())
        return inter_search(node->right, ID);
    // Would give error if this return was not included
    return nullptr;
}

// Balance Methods //

TreeNode* AVL::balance(TreeNode* node)
{
    int balanceFactor = calculateBalanceFactor(node);

    /* Note:
     * if balance factor is >= 1, node is left heavy
     * if balance factor is <= -1, node is right heavy
     */
    // Right Heavy
    if (balanceFactor < -1)
    {
        // Subtree is left heavy
        if (calculateBalanceFactor(node->right) >= 1)
            return  rotateRL(node);
        // Subtree is right heavy, for deletion, it includes equal case
        else
            return  rotateLeft(node);
    }
    // Left Heavy
    else if (balanceFactor > 1)
    {
        // Subtree is right heavy
        if (calculateBalanceFactor(node->left) <= -1)
            return  rotateLR(node);
        // Subtree is left heavy, for deletion, it includes equal case
        else
            return rotateRight(node);
    }
    return node;
}
// Traversals //
void AVL::getInorder(TreeNode* node, vector<Student>& inorder)
{
    if (node == nullptr)
        return;

    getInorder(node->left, inorder);
    inorder.push_back(node->_student);
    getInorder(node->right, inorder);
}

void AVL::getPreorder(TreeNode* node, vector<Student>& preorder)
{
    if (node == nullptr)
        return;

    preorder.push_back(node->_student);
    getPreorder(node->left, preorder);
    getPreorder(node->right, preorder);
}

void AVL::getPostorder(TreeNode* node, vector<Student>& postorder)
{
    if (node == nullptr)
        return;

    getPostorder(node->left, postorder);
    getPostorder(node->right, postorder);
    postorder.push_back(node->_student);
}

/* Spiral Approach
 * Implementation based on source 3
 */
void AVL::getLevels(TreeNode* node, int& count)
{
    if (node == nullptr)
        return;
    // Queue Implementation of Level Order
    queue<TreeNode*> q;
    q.push(node);
    while (!q.empty())
    {
        int len = q.size();
        for (int i = 0; i < len; i++)
        {
            // Temporary node to hold the first value of the level
            TreeNode* temp = q.front();
            q.pop();
            // Enqueue Left child, if it exists
            if (temp -> left != nullptr)
                q.push(temp -> left);
            // Enqueue Right child, if it exists
            if (temp -> right != nullptr)
                q.push(temp -> right);
        }
        count++;
    }

}

// Rotations : Time Complexity - O(1) //

TreeNode* AVL::rotateLeft(TreeNode* node)
{
    TreeNode* grandchild = node->right->left;
    TreeNode* newParent = node->right;
    node->right = grandchild;
    newParent->left = node;
    // Updates Heights of moved nodes
    node->height =  1 + max(getHeight(node->right), getHeight(node->left));
    newParent->height = 1 + max(getHeight(newParent->right), getHeight(newParent->left));
    return newParent;
}

TreeNode* AVL::rotateRight(TreeNode* node)
{
    TreeNode* grandchild = node->left->right;
    TreeNode* newParent = node->left;
    node->left = grandchild;
    newParent->right = node;
    // Updates Heights of moved nodes
    node->height =  1 + max(getHeight(node->right), getHeight(node->left));
    newParent->height = 1 + max(getHeight(newParent->right), getHeight(newParent->left));
    return newParent;
}

TreeNode* AVL::rotateLR(TreeNode* node)
{
    node->left = rotateLeft(node->left);
    node = rotateRight(node);
    return node;
}

TreeNode* AVL::rotateRL(TreeNode* node)
{
    node->right = rotateRight(node->right);
    node = rotateLeft(node);
    return node;
}

// --- --- --- Public Methods --- --- --- //

void AVL::printLevelCount()
{
    int count = 0;
    getLevels(root,count);
    output.push(to_string(count) + "\n") ;
}
// Able to tell whether the search was for a name or for ID, with each having its own output
bool AVL::search(const string& target)
{
    regex validIDRegex("^[0-9]{8}$");
    regex validNameRegex("^[A-Za-z\\s]+$");

    if (regex_match(target, validIDRegex))
    {
        TreeNode* node = inter_search(root, target);
        if (node != nullptr)
        {
            output.push(node->getName() + "\n");
            return true;
        }
    }
    else if (regex_match(target, validNameRegex))
    {
        vector<Student> studentNames = preorder();
        bool found = false;
        for (auto& student : studentNames)
            if (student._name == target)
            {
                output.push(student.GatorID + "\n") ;
                found = true;
            }

        if (found)
            return true;
    }

    output.push("unsuccessful\n") ;
    return false;
}

bool AVL::insert(const string& name, const string& ID)
{
    regex validIDRegex("^[0-9]{8}$");
    regex validNameRegex("^[A-Za-z\\s]+$");
    // Validates format of name and ID, while also checking if ID isnt duplicate
    if (regex_match(name, validNameRegex) && regex_match(ID, validIDRegex) && inter_search(root, ID) == nullptr)
    {
        const Student newStu(name, ID);
        root = inter_insert(root, newStu);
        output.push( "successful\n");
        return true;
    }
    output.push("unsuccessful\n") ;
    return false;

}

bool AVL::remove(const string& ID)
{
    regex validIDRegex("^[0-9]{8}$");
    bool wasRemoved = false;
    // If ID inserted is not valid, wont traverse the list
    if (regex_match(ID, validIDRegex))
    {
        root  = inter_remove(root, ID, wasRemoved);
        // If an element was removed, then it gets deleted
        if (wasRemoved)
        {
            output.push("successful\n") ;
            return wasRemoved;
        }
    }
    output.push("unsuccessful\n") ;
    return wasRemoved;
}

bool AVL::removeInorder(int N)
{
    bool wasRemoved = false;
    vector<Student> students = inorder();
    int nodeCount = students.size();
    // checks if there is a Nth node
    if (nodeCount > N || N < 0)
    {
        string target = students.at(N).GatorID;
        root  = inter_remove(root, target, wasRemoved);
        // If an element was removed, then it gets deleted
        if (wasRemoved)
        {
            output.push("successful\n") ;
            return wasRemoved;
        }
    }
    output.push("unsuccessful\n") ;
    return wasRemoved;
}

vector<Student> AVL::inorder()
{
    vector<Student> inorder;
    getInorder(root, inorder);
    return inorder;
}

vector<Student> AVL::preorder()
{
    vector<Student> preorder;
    getPreorder(root, preorder);
    return preorder;
}

vector<Student> AVL::postorder()
{
    vector<Student> postorder;
    getPostorder(root, postorder);
    return postorder;
}
//Prints the _order of the tree, which is stored in a vector
void AVL:: print_order(vector<Student>& nodes)
{
    int index = 0;
    string names = "";
    for (auto& student : nodes)
    {
        names += student._name;
        if (index != int(nodes.size()) - 1)
            names += ", " ;
        index++;
    }
    output.push(names+'\n');
}

/* Traverses Whole Tree to check if the tree is balanced */
bool AVL::checkIfBalanced(TreeNode* node )
{
    if (node == nullptr)
        return true;
    // Queue Implementation of Level Order, using O(n) space complexity
    queue<TreeNode*> q;
    q.push(node);
    while (!q.empty())
    {
        int len = q.size();
        for (int i = 0; i < len; i++)
        {
            // Temporary node to hold the first value of the level
            TreeNode* temp = q.front();
            int currNodeBalanceFactor = calculateBalanceFactor(temp);
            // If balance factor is not -1, 0, or 1, return false
            if (currNodeBalanceFactor > 1 || currNodeBalanceFactor < -1)
                return false;
            q.pop();
            // Enqueue Left child, if it exists
            if (temp -> left != nullptr)
                q.push(temp -> left);
            // Enqueue Right child, if it exists
            if (temp -> right != nullptr)
                q.push(temp -> right);
        }
    }
    return true;
}
