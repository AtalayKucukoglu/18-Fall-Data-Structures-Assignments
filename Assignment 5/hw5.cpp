/*
 * @Author
 * Student Name: Atalay Küçükoğlu
 * Student ID: 150170074
 * Date: 24.12.2018
 *
 * @Brief
 *
 * This program takes a command line arguement text file and
 * reads input values and a target value from text.
 * Constructs a binary tree with the values given in the first line
 * of input file. With pre-order traversal, traverses by adding
 * values to another and print two paths that reaches target value by
 * summing up all values in the path.
 */


#include <iostream>
#include <fstream>

using namespace std;

/*
 * Node to hold values of tree
 */
struct Node {
    int value;      // value of the node
    Node* left;     // left child
    Node* right;    // right child
};

/*
 * Node to hold node pointers to the values
 * These nodes are used in stacks and queues.
 */
struct ListNode {
    Node* qnode;        // a pointer to the tree nodes
    ListNode* next;     // a pointer to the next node
};

struct queueNodes {
    ListNode* head;     // a pointer to the first node in queue
    int size;       // size of queue

    ListNode* initNode (Node* qnode);  // creates an unattached node for node
    void create();      // creates the hole queue
    void push(Node* qnode );    // pushes nodes to queue
    Node* front();          // returns the value of first node
    void pop();         // removes the first node in the queue
    bool isEmpty();     // returns true if the queue is empty
};

struct stackNodes {
    ListNode* head;     // a pointer to the first node in queue
    int size;       // size of queue

    ListNode* initNode (Node* qnode);  // creates an unattached node
    void create();      // creates the stack
    void push(Node* qnode );    // pushes nodes to queue
    Node* top();          // returns the value of first node
    void pop();         // removes the first node in the queue
    bool isEmpty();     // returns true if the queue is empty
    void printReverse();    // prints the path
    void empty();       // empties the stack
};

struct BinaryTree {
    Node* root;     // root of tree
    queueNodes qnodes;  // queue that is used to construct tree properly
    stackNodes path;    // a stack to find path
    int target;         // target value
    int sum;            // sum of the values in the path

    void create();  // creates a binary tree
    Node* initNode(int value);   //initializes a node with the value
    void readFile(char* textFile);      // reads the text files and calls construct
    void construct(int value);      // fills the tree
    void popAndDoublePush(Node* newNode);   // makes push and pop operations in the queue
    void findPath(Node*);       // finds the paths
    void updateSum();       // updates the sum of pasth values


};


/*
 * Initializes a node to hold node values
 *@return new node
 */

ListNode* queueNodes::initNode(Node* qnode) {
    ListNode* newNode = new ListNode;
    newNode->next = NULL;
    newNode->qnode = qnode;
    return newNode;
}

/*
 * Creates the queue
 */
void queueNodes::create() {
    head = NULL;
    size = 0;
}

/*
 * Pushes nodes to queue
 * @param value value of node
 */
void queueNodes::push(Node* qnode) {
    ListNode* newNode = initNode(qnode);
    if(isEmpty()) {
        head = newNode;
    } else {
        ListNode* traverser = head;
        while(traverser->next) {
            traverser = traverser->next;
        }
        traverser->next = newNode;
    }
    size++;
}

/*
 * Removes the first node from queue
 */
void queueNodes::pop() {
    if (isEmpty()) {
        cout << "The queue is already empty!" << endl;
    } else {
        ListNode* traverse = head;
        head = traverse->next;
        delete traverse;
        size--;
    }
}

/*
 * Returns the value first node in the queue
 * @return the value first node in the queue
 */
Node* queueNodes::front() {
    return head->qnode;
}

/*
 * Returns true if the queue is empty
 */
bool queueNodes::isEmpty() {
    return !head;
}

ListNode* stackNodes::initNode(Node* qnode) {
    ListNode* newNode = new ListNode;
    newNode->next = NULL;
    newNode->qnode = qnode;
    return newNode;
}

/*
 * Creates the queue
 */
void stackNodes::create() {
    head = NULL;
    size = 0;
}

/*
 * Pushes nodes to queue
 * @param value value of node
 */
void stackNodes::push(Node* qnode) {
    ListNode* newNode = initNode(qnode);
    if(isEmpty()) {
        head = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    size++;
}

/*
 * Removes the first node from queue
 */
void stackNodes::pop() {
    if (isEmpty()) {
        cout << "The path is already empty!" << endl;
    } else {
        ListNode* traverse = head;
        head = traverse->next;
        delete traverse;
        size--;
    }
}

/*
 * Returns the value first node in the queue
 * @return the value first node in the queue
 */
Node* stackNodes::top() {
    return head->qnode;
}

/*
 * Returns true if the queue is empty
 */
bool stackNodes::isEmpty() {
    return !head;
}

void stackNodes::printReverse() {
    if (isEmpty()) {
        cout << "No Path Found" << endl;
        return;
    }
    int array[100] = {0};
    int i = 0;
    int arrSize = 0;
    ListNode* traverse = head;
    while (traverse) {
        array[i] = traverse->qnode->value;
        arrSize++;
        i++;
        traverse = traverse->next;
    }
    cout << "Path Found:";
    for (i = size - 1; i >= 0; i--) {
        cout << " " << array[i];
    }
    cout << endl;

}
/*
 * Clears the stack
 */
void stackNodes::empty() {
    while (!isEmpty()) {
        pop();
    }
}

/*
 * Creates a new tree
 */
void BinaryTree::create() {
    root = NULL;
    qnodes.create();
    path.create();
    target = 0;
    sum = 0;
}

/*
 * Initializes a new node for tree
 */

Node* BinaryTree::initNode(int value) {
    Node* newNode = new Node;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->value = value;
    return newNode;
}

/*
 * Fills the queue to construct the tree properly
 */
void BinaryTree::popAndDoublePush(Node* newNode) {
    qnodes.pop();
    qnodes.push(newNode);
    qnodes.push(newNode);
}

/*
 * Constructs the binary tree
 */
void BinaryTree::construct(int value) {
    Node* newNode = initNode(value);   // create an unattached node
    // if root is null, make it root
    if (root == NULL) {
        root = newNode;
        qnodes.push(newNode);
        qnodes.push(newNode);
    } else {    // if root is not null
        if (qnodes.front()->left == NULL) {
            qnodes.front()->left = newNode;
            popAndDoublePush(newNode);
        } else {
            if (qnodes.front()->right == NULL) {
                qnodes.front()->right = newNode;
                popAndDoublePush(newNode);
            }
        }
    }
}

/*
 * Reads the input file and calls construct function
 * @param textFile text file containing inputs
 */
void BinaryTree::readFile(char* textFile) {

    FILE* fileptr;   // a pointer to make file operations
    fileptr = fopen(textFile, "r");   // open the file in reading mode

    if (!fileptr) {
        cout << "File cannot be opened.";   // print if the file cannot be opened
        return;                            // exit program with error
    }
    // file operations starts here
    int value;      // value read from text file
    int values[100];
    int i = 0;
    int size = 0;
    while (!feof(fileptr)) {
        fscanf(fileptr, "%d ", &value);     // read the input and target values
        values[i] = value;
        i++;
        size++;
    }

    // send values to construct binary tree
    for(int i = 0; i < size - 1; i++) {
        construct(values[i]);
    }
    target = values[size - 1]; // send target value

    fclose(fileptr);        // close the text file
}

/*
 * Updates the sum of the path values
 */
void BinaryTree::updateSum() {
    sum = 0;
    if (path.isEmpty()) {
        return;
    }
    ListNode* traverse = path.head;
    while (traverse) {
        sum += traverse->qnode->value;
        traverse = traverse->next;
    }
}
/*
 * Finds the path that leads to the target
 */
void BinaryTree::findPath(Node* traverse) {
    path.push(traverse);
    updateSum();
    if (sum == target) {
        return;
    }
    if (sum > target) {
        path.pop();
        updateSum();
        return;
    }
    if (sum < target) {
        if (traverse->left) {
            findPath(traverse->left);   // go to the left child
        }
        if (traverse->right) {
            findPath(traverse->right);  // go to the right child
        }
    }
    if (sum != target) {
        path.pop();
        updateSum();
    }
}

int main(int argc, char* argv[]) {
    BinaryTree inTreeGue;
    inTreeGue.create();     // creates the tree
    inTreeGue.readFile(argv[1]);    // reads the text file

    inTreeGue.path.push(inTreeGue.root);    // pushes root to the path

    // if target equals to the root value, print the path including only root
    if (inTreeGue.target == inTreeGue.root->value) {
        inTreeGue.path.printReverse();  // print for left subtree
        inTreeGue.path.printReverse();  // print for right subtree
        return 0;
    }
    inTreeGue.findPath(inTreeGue.root->left);   // send the left subtree to find the path
    if (inTreeGue.sum == inTreeGue.root->value) {
        inTreeGue.path.empty();     // if no path found, clear the path
    }
    inTreeGue.path.printReverse();      // print path for left subtree
    inTreeGue.path.empty();
    inTreeGue.sum = 0;
    inTreeGue.path.push(inTreeGue.root);    // pushes root to the path
    inTreeGue.findPath(inTreeGue.root->right);      // send the left subtree to find the path
    if (inTreeGue.sum == inTreeGue.root->value) {
        inTreeGue.path.empty();     // if no path found, clear the path
    }
    inTreeGue.path.printReverse();      // print path for right subtree
    inTreeGue.path.empty();     // clear the stack "path"
    return 0;
}

