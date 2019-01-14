/* @Author
 *
 * Student Name: Atalay Küçükoğlu
 * Student ID: 150170074
 * Date: 27.11.2018
 *
 * @Brief
 *
 * A card game
 *
 * A card game that takes table deck count and player deck count
 * as inputs read from a text file and prints the number of cards
 * in bin after simulating the game
 *
 */

#include <iostream>     // to print to screen
#include <fstream>      // to make file operations

using namespace std;

struct Node {
    int value;       // card's value
    Node* next;      // a pointer to next node
};

struct Deck {
    Node* head;     // a pointer to the topmost card of deck
    int size;       // size of deck

    Node* initNode ();  // creates an unattached node
    void create();      // creates the deck
    void push(int );    // pushes card to deck
    int top();          // returns the value of topmost card
    void pop();         // removes the topmost card from deck
    bool isEmpty();     // returns true if the deck is empty
    void empty();       // deletes all the dynamically allocated data
};

struct cardGame {
    Deck player1;       // Player 1's deck
    Deck player2;       // Player 2's deck
    Deck tableDeck;     // table's deck
    Deck bin;           // cards that thrown in to the deck
    bool player1IsNext = true;    // Player 1's turn when true
    bool player1IsGiver;          // Player 1 gives cards when true
    bool isOver = false;          // the game ends when true

    void initGame();        // makes the game ready to play
    void readData(const char* textFile);    // reads the inputs from text file
    void checkCountInput(int count);    // checks if deck counts are valid
    void checkCardInput(int card);      // checks if card values are valid
    void determineGiver();      // determines which player gives cards next
    void throwToBin();          // throws cards to the bin
    void giveToNextPlayer();    // gives cards to next player
    void compareCards();        // compares cards
    void startGame();           // starts the game
    void endGame();             // ends the game by deleting all decks
};


/*
 * Initializes a node to hold card values
 *@return new node
 */
Node* Deck::initNode () {
    Node* newNode = new Node;
    newNode->next = nullptr;
    newNode->value = 0;
    return newNode;
}

/*
 * Creates the deck
 */
void Deck::create() {
    head = NULL;
    size = 0;
}

/*
 * Pushes cards to deck
 * @param value value of card
 */
void Deck::push(int value) {
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
 * Removes the topmost card from deck
 */
void Deck::pop() {
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
 * Returns the value topmost card of deck
 * @return the value topmost card
 */
int Deck::top() {
    return head->value;
}

/*
 * Returns true if the deck is empty
 */
bool Deck::isEmpty() {
    return !head;
}

/*
 * Deletes all the cards of deck
 * Deletes the dynamically allocated memory.
 */
void Deck::empty() {
    while (!isEmpty()) {
        pop();
    }
}

/*
 * Creates the decks to make the game ready to play
 */
void cardGame::initGame() {
    player1.create();
    player2.create();
    tableDeck.create();
    bin.create();
}

/*
 * Checks if the deck count input is valid
 */
void cardGame::checkCountInput(int count) {
    if (count < 0 || count > 1000) {
        cout << "Invalid input. Exiting game..." << endl;
        exit(1);
    }
}

/*
 * Checks if the card value input is valid
 */
void cardGame::checkCardInput(int card) {
    if (card == 0 || card < -1000000 || card > 1000000) {
        cout << "Invalid input. Exiting game..." << endl;
        exit(1);
    }
}

/*
 * Reads the inputs from the text file and fills the decks
 *
 * @param textFile text file containing inputs
 */
void cardGame::readData(const char* textFile) {

    FILE* fileptr;   // a pointer to make file operations

    fileptr = fopen(textFile, "r");   // open the file in reading mode

    if (!fileptr) {
        cout << "File cannot be opened.";   // print if the file cannot be opened
        exit(1);                            // exit program with error
    }
    // file operations starts here

    int tableDeckCount;      // number of cards in the table deck
    int playerDeckCount;     // number of cards in each player's deck
    int card;                // card to be pushed to deck

    fscanf(fileptr, "%d %d ", &tableDeckCount, &playerDeckCount);   // read the first line
    checkCountInput(tableDeckCount);        // check if table deck count is valid
    checkCountInput(playerDeckCount);       // check if player deck count is valid

    // fill the table deck
    for (int i = 0; i < tableDeckCount; i++) {
        fscanf(fileptr, "%d ", &card);  // read input
        checkCardInput(card);       // check if valid
        tableDeck.push(card);       // push card to the deck
    }

    // fill Player 1's deck
    for (int i = 0; i < playerDeckCount ; i++) {
        fscanf(fileptr, "%d ", &card);  // read input
        checkCardInput(card);       // check if valid
        player1.push(card);         // push card to the deck
    }

    for (int i = 0; i < playerDeckCount ; i++) {
        fscanf(fileptr, "%d ", &card);  // read input
        checkCardInput(card);       // check if valid
        player2.push(card);         // push card to the deck
    }
    fclose(fileptr);        // close the text file
}
/*
 * Determines which player gives card next
 */
void cardGame::determineGiver() {
    player1IsGiver = (tableDeck.top() < 0) == player1IsNext;
}

/*
 * Gives card to the next player
 */
void cardGame::giveToNextPlayer() {
    if (player1IsGiver) {       // if its Player 1's turn
        player2.push(player1.top());    // push the value
        player1.pop();      // delete the topmost card
    } else {    // if its Player 2's turn
        player1.push(player2.top());   // push the value
        player2.pop();      // delete the topmost card
    }
}

/*
 * Throws the cards to the bin
 */
void cardGame::throwToBin() {
    if (player1IsGiver) {   // if its Player 2's turn
        bin.push(player1.top());    // push the value
        player1.pop();  // delete card
    } else {    // if its Player 2's turn
        bin.push(player2.top());    // push the value
        player2.pop();      // delete card
    }
}

/*
 * Compares the topmost cards of each player and determines
 * what to do with the card
 */
void cardGame::compareCards () {
    determineGiver();   // determines which player gives cards
    if (player1IsGiver) {   // if Player 1 gives cards
        if (player1.top() > player2.top()) {
            giveToNextPlayer();     // give to next player
        } else {
            throwToBin();   // throw to the bin
        }
    } else {    // if Player 2 gives cards
        if (player2.top() > player1.top()) {
            giveToNextPlayer();     //give to next player
        } else {
            throwToBin();   // throw to the bin
        }
    }
}

/*
 * Starts the game and simulates until the game is over
 */
void cardGame::startGame () {
    while (!isOver) {   // while the game is not over
        for (int i = 0; i < abs(tableDeck.top()); i++) {
            if (player1.isEmpty() || player2.isEmpty()) { // if one the the players's deck is empty
                isOver = true;
                break;      // exit for loop
            }
            compareCards(); // do the card operations
        }
        tableDeck.pop();    // delete the topmost card of the table deck
        player1IsNext = !player1IsNext;     // switch the turn

        if (tableDeck.isEmpty())    // if table deck is empty
            isOver = true;      // ends the game
    }
    cout << bin.size << endl;   // print the number of cards in the bin at the end of the game
}

/*
 * Deletes the dynamically allocated memory and empties the decks
 */
void cardGame::endGame() {
    player1.empty();
    player2.empty();
    tableDeck.empty();
    bin.empty();
}

/*
 * @param argc argument count
 * @param argv argument vectors
 */
int main(int argc, char* argv[]) {
    // if there are no input text files, exit with error
    if (argc != 2) {
        return 1;
    }
    cardGame Game;      // declare Game type of struct cardGame
    Game.initGame();    // make the game ready
    Game.readData(argv[1]); // read inputs from textfile and fill the decks
    Game.startGame();       // simulate the game
    Game.endGame();         // delete the dynamically allocated memory

    return 0;
}









