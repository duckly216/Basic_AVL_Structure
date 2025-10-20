//
// Created by jorge on 6/2/2025.
//

#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <algorithm>
#include <ostream>

using namespace std;
struct Student
{
    string _name; string GatorID;
    Student(string name, string gatorID): _name(name), GatorID(gatorID) {}
    bool operator<(const Student &other) const
    {
        return GatorID < other.GatorID;
    }
    bool operator>(const Student &other) const
    {
        return GatorID > other.GatorID;
    }
    bool operator==(const Student &other) const
    {
        return _name == other._name && GatorID == other.GatorID;
    }

};
class TreeNode
{

    public:
    Student _student;
    TreeNode* left; TreeNode* right;
    // TreeNode* parent; // IDK if necessary
    int height; // Or Balance Factor
    string getName() const { return _student._name; }
    string getGatorID() const { return _student.GatorID; }
    TreeNode(const Student& student) : _student(student),
    left(nullptr), right(nullptr), height(1) {}

};
/*
* UFIDs are strictly eight digits long and must be unique.
* Names must include only alphabets from [a-z, A-Z, and spaces]
* Any invalid or misspelled commands must be ignored with an "unsuccessful" message
* followed by executing the next command.
 */
class AVL {
    TreeNode* root = nullptr;
    stack<string> output;
    // Internal Helper Methods //
    TreeNode* inter_insert(TreeNode* node, const Student& newStu);
    TreeNode* inter_remove(TreeNode* node, string ID, bool& verifier);
    TreeNode* inter_search(TreeNode* node, string target);


    // Balance & Height Methods
    TreeNode* balance(TreeNode* node);
    int calculateBalanceFactor(TreeNode* node)
    {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    // Height - 1
    // O(1) Time Complexity due to using built in height attribute that's already updated with each insertion/deletion
    int getHeight(TreeNode* node)
    {
        return node ? node->height : 0;
    }

    // Traversals
    void getInorder(TreeNode* node, vector<Student>& inorder);
    void getPreorder(TreeNode* node, vector<Student>& preorder);
    void getPostorder(TreeNode* node, vector<Student>& postorder);
    void getLevels(TreeNode* node, int& count);

    // Rotations : Time Complexsity - O(1)
    TreeNode* rotateLeft(TreeNode* node);
    TreeNode* rotateRight(TreeNode* node);
    TreeNode* rotateLR(TreeNode* node);
    TreeNode* rotateRL(TreeNode* node);

public:
    // Public Usage //
    void printOutput()
    {
        stack<string> outputCopy = output;
        vector<string> toPrint;
        while (!outputCopy.empty())
        {
            toPrint.push_back(outputCopy.top());
            outputCopy.pop();
        }
        reverse(toPrint.begin(), toPrint.end());
        for (auto& element : toPrint)
            cout << element;
    }
    string getRecentOutput(){return output.top();};
    TreeNode* getRoot(){return root;}
    void printLevelCount();
    /*
     * Takes in a desired target, can be ID or name
     * Will verify validity of inserted name and ID
     */
    bool search(const string& target);
    bool verifyID(string ID);
    /* Insert will deal with validating names and IDs */
    bool insert(const string& name, const string& ID);
    // V
    bool remove(const string& ID);
    /* Remove the Nth GatorID from the inorder traversal of the tree (N = 0 for the first item, etc).
     * If removal is successful, print “successful.”
     * Optional: Balance the tree automatically if necessary. We will test your code only on cases where the tree will be balanced before and after the deletion. So you can implement a BST deletion and still get full credit.
     * If the Nth GatorID does not exist within the tree, print “unsuccessful.”
     */
    bool removeInorder(int N);
    void clear();

    /* Used for Tests, and can be used for printing
     */
    vector<Student> inorder();
    vector<Student> preorder();
    vector<Student> postorder();
    void print_order(vector<Student>& nodes);
    bool checkIfBalanced(TreeNode* node);
};




