#include <iostream>
#include <fstream>


using namespace std;

struct Node {
    int value;
    Node* next;
};

struct stackAnt {
    Node* head;     // a pointer to the topmost ant in hole
    int size;       // size of hole

    Node* initNode ();  // creates an unattached node for ant
    void create();      // creates the hole stack
    void push(int );    // pushes ants to hole
    int top();          // returns the value of topmost ant
    void pop();         // removes the topmost ant from hole
    bool isEmpty();     // returns true if the hole is empty
    };

struct queueAnt {
    Node* head;     // a pointer to the first ant in queue
    int size;       // size of queue

    Node* initNode ();  // creates an unattached node for ant
    void create();      // creates the hole queue
    void push(int );    // pushes ants to queue
    int top();          // returns the value of first ant
    void pop();         // removes the fisrt ant in the queue
    bool isEmpty();     // returns true if the queue is empty
    };

struct Ants {
    queueAnt ants;          // queue for ants
    queueAnt holeDepths;    // queue for holes
    stackAnt hole;          // stack for ants in the hole
    void ReadFile ( char *);    // reads the input file and calls appropriate functions
    void ShowContents ( bool ); // shows the ant sequences or hole depths
    void CrossRoad ();          // simulates crossing holes in the road
};

/*
 * Initializes a node to hold ant values
 *@return new node
 */

Node* stackAnt::initNode () {
    Node* newNode = new Node;
    newNode->next = NULL;
    newNode->value = 0;
    return newNode;
}

/*
 * Creates the stack for hole
 */
void stackAnt::create() {
    head = NULL;
    size = 0;
}

/*
 * Pushes ants to hole
 * @param value value of ant
 */
void stackAnt::push(int value) {
    Node* newNode = initNode();
    newNode->value = value;
    if(isEmpty()) {
        head = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    size++;
}

/*
 * Removes the topmost ant in hole
 */
void stackAnt::pop() {
    if (isEmpty()) {
        cout << "The hole is already empty!" << endl;
    } else {
        Node* traverse = head;
        head = traverse->next;
        delete traverse;
        size--;
    }
}

/*
 * Returns the value topmost ant in hole
 * @return the value topmost ant
 */
int stackAnt::top() {
    return head->value;
}

/*
 * Returns true if the hole is empty
 */
bool stackAnt::isEmpty() {
    return !head;
}

/*
 * Initializes a node to hold ant values
 *@return new node
 */
Node* queueAnt::initNode () {
    Node* newNode = new Node;
    newNode->next = NULL;
    newNode->value = 0;
    return newNode;
}

/*
 * Creates the queue
 */
void queueAnt::create() {
    head = NULL;
    size = 0;
}

/*
 * Pushes ants to queue
 * @param value value of ant
 */
void queueAnt::push(int value) {
    Node* newNode = initNode();
    newNode->value = value;
    if(isEmpty()) {
        head = newNode;
    } else {
        Node* traverser = head;
        while(traverser->next) {
            traverser = traverser->next;
        }
        traverser->next = newNode;
    }
    size++;
}

/*
 * Removes the first ant from queue
 */
void queueAnt::pop() {
    if (isEmpty()) {
        cout << "The deck is already empty!" << endl;
    } else {
        Node* traverse = head;
        head = traverse->next;
        delete traverse;
        size--;
    }
}

/*
 * Returns the value first ant in the queue
 * @return the value first ant in the queue
 */
int queueAnt::top() {
    return head->value;
}

/*
 * Returns true if the queue is empty
 */
bool queueAnt::isEmpty() {
    return !head;
}

/*
 * Reads the input file and pushes ant values to the appropriate queues
 * @param textFile text file containing inputs
 */
void Ants::ReadFile(char* textFile) {

    int holeDepth;      // depth of a hole
    ants.create();      // create the queueAnt ants
    holeDepths.create();    // create the queueAnt holeDepths
    int size;       // size of the queue (read from the first line)

    FILE* fileptr;   // a pointer to make file operations

    fileptr = fopen(textFile, "r");   // open the file in reading mode

    if (!fileptr) {
        cout << "File cannot be opened.";   // print if the file cannot be opened
        return;                            // exit program with error
    }
    // file operations starts here

    fscanf(fileptr, "%d ", &size); // reads the first line (size of ant queue)

    for (int i = 1; i <= size; i++ ) {
        ants.push(i);       // enqueues the ants sequentially
    }

    while (!feof(fileptr)) {
        fscanf(fileptr, "%d ", &holeDepth);
        holeDepths.push(holeDepth);     // reads the hole depths and enqueues
    }

    fclose(fileptr);        // close the text file
}

/*
 * Simulates the road by pushing ants to hole stack and popping them
 * to push to the end of the queue again.
 */
void Ants::CrossRoad () {
    hole.create();      // create the stackAnt hole

    while (!holeDepths.isEmpty()) {     // while holeDepths is not empty
        for (int i = 0; i < holeDepths.top(); i++) {
            hole.push(ants.top());      // takes the first ant and pushes to the hole
            ants.pop();         // removes ant from the queue
        }
        holeDepths.pop();       // removes hole from the queue

        while (!hole.isEmpty()) {       // while the hole is not empty
            ants.push(hole.top());      // takes the topmost ant in the hole and enqueues to the ants
            hole.pop();         // removes ant from the hole
        }
    }
}

/*
 * Shows the contents of queueAnt ants when true, or
 * queueAnt holeDepths when false.
 * @param show boolean to decide which queue to show contents of.
 */
void Ants::ShowContents (bool show) {

    if (show) {     // if true, get in
        Node *traverser = ants.head;        // a pointer to head of the queueAnt ants
        // print each ant
        while (traverser) {
            cout << traverser->value << " ";
            traverser = traverser->next;
        }
        } else {    // if false, get in
        Node *traverser = holeDepths.head;   // a pointer to head of the queueAnt holeDepths
        // print each ant
        while (traverser) {
            cout << traverser->value << " ";
            traverser = traverser->next;
        }
    }
    cout << endl;
}

int main (int argc , char ** argv ){
    Ants a;
    a.ReadFile(argv[1]); // store the number of ants and depths of holes
    cout << "The initial Ant sequence is: ";
    a.ShowContents(1); // list ant sequence ( initially : 1, 2, ... , N)
    cout << "The depth of holes are: ";
    a.ShowContents(0); // list depth of holes
    a.CrossRoad();
    cout << "The final Ant sequence is: ";
    a.ShowContents(1);
    return 0;
}