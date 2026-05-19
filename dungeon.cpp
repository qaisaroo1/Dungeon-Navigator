#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;


struct Room {
    string name;
    string description;
    Room* north = nullptr; Room* south = nullptr;
    Room* east = nullptr;  Room* west = nullptr;
    bool visited = false; 
    Room* parent = nullptr; 
    Room(string n, string d) : name(n), description(d) {}
};

Room* currentRoom = nullptr;
Room* treasureRoomPtr = nullptr;
stack<Room*> historyStack; 


void simulateCombatDebug(string monsterName) {
    cout << "\n======================================" << endl;
    cout << "   COMBAT INITIATED: vs " << monsterName << endl;
    cout << "======================================" << endl;
    queue<string> combatQueue; 

   
    cout << "[DEBUG: QUEUE] Enqueuing combat events...\n";
    combatQueue.push("Player draws Rusty Sword.");
    combatQueue.push("Monster growls fiercely!");
    combatQueue.push("Player attacks for 5 damage!");
    combatQueue.push("Monster defends.");
    combatQueue.push("Player uses Health Potion.");

    cout << "[DEBUG: QUEUE] Processing events (FIFO)...\n\n";
    int turn = 1;
    while(!combatQueue.empty()) {
        cout << "Turn " << turn++ << ": " << combatQueue.front() << endl;
        combatQueue.pop();
    }
    cout << "\n*** Victory! Monster defeated. ***\n" << endl;
}


struct ItemNode {
    string name;
    ItemNode* next = nullptr;
};

class Inventory {
private:
    ItemNode* head = nullptr;
public:
    void createDefaultItems() {
        addItem("Rusty Sword");
        addItem("Health Potion");
    }
    void addItem(string itemName) {
        ItemNode* newNode = new ItemNode{itemName, head};
        head = newNode;
    }
    void showItems() {
        cout << "\n--- Inventory (Linked List) ---" << endl;
        if (head == nullptr) cout << "Empty." << endl;
        else {
            ItemNode* temp = head;
            while (temp != nullptr) { cout << "- " << temp->name << endl; temp = temp->next; }
        }
        cout << "-------------------------------\n" << endl;
    }
};
Inventory playerInventory;


struct MonsterNode {
    string name; string weakness;
    MonsterNode *left = nullptr, *right = nullptr;
    MonsterNode(string n, string w) : name(n), weakness(w) {}
};

class Bestiary {
private:
    MonsterNode* root = nullptr;
    MonsterNode* insert(MonsterNode* node, string name, string weakness) {
        if (!node) return new MonsterNode(name, weakness);
        if (name < node->name) node->left = insert(node->left, name, weakness);
        else if (name > node->name) node->right = insert(node->right, name, weakness);
        return node;
    }
    MonsterNode* search(MonsterNode* node, string name) {
        if (!node || node->name == name) return node;
        if (name < node->name) return search(node->left, name);
        return search(node->right, name);
    }
public:
    void loadMonsters() {
        root = insert(root, "Goblin", "Fire");
        root = insert(root, "Troll", "Acid");
        root = insert(root, "Dragon", "Ice Spears");
        cout << "[DEBUG: BST] Monsters loaded alphabetically into tree.\n";
    }
    void checkInfoInteractive() {
        string inputName;
        cout << "\n> Enter monster name to search in BST (e.g., Goblin, Troll): ";
        cin >> inputName;
        MonsterNode* res = search(root, inputName);
        if (res) cout << "\n[BESTIARY ENTRY Found via O(log n) search]\nName: " << res->name << "\nWeakness: " << res->weakness << "\n\n";
        else cout << "\n[!] No entry found for '" << inputName << "'.\n\n";
    }
};
Bestiary monsterBook;


vector<Room*> allRooms;
void setupWorld() {
    Room* start = new Room("Dungeon Entrance", "A dark, damp cave entrance.");
    Room* forest = new Room("Forest Clearing", "Sunlight filters through trees. Dead end.");
    Room* hallway = new Room("Dark Hallway", "A narrow stone corridor connecting rooms.");
    Room* torchRoom = new Room("Torch Room", "A brightly lit room. Good for reading.");
    Room* treasure = new Room("Treasure Chamber", "Piles of GOLD! You found it!");

    allRooms = {start, forest, hallway, torchRoom, treasure};

  
    start->north = forest;  forest->south = start;
    start->east = torchRoom; torchRoom->west = start;
    start->south = hallway; hallway->north = start;
    hallway->east = treasure; treasure->west = hallway;

    currentRoom = start;
    treasureRoomPtr = treasure;
}

void movePlayer(string direction) {
    Room* nextRoom = nullptr;
    if (direction == "n") nextRoom = currentRoom->north;
    else if (direction == "s") nextRoom = currentRoom->south;
    else if (direction == "e") nextRoom = currentRoom->east;
    else if (direction == "w") nextRoom = currentRoom->west;

    if (nextRoom) {
        cout << "[DEBUG: STACK] Pushing '" << currentRoom->name << "' to history.\n";
        historyStack.push(currentRoom); 
        currentRoom = nextRoom;
        cout << "\n> Moving " << direction << "...\n";
    } else {
        cout << "\n> You cannot go that way. (No connection defined in Graph)\n";
    }
}

void goBack() {
    if (!historyStack.empty()) {
        cout << "[DEBUG: STACK] Popping top room to go back.\n";
        currentRoom = historyStack.top(); 
        historyStack.pop();               
        cout << "\n> Retracing steps...\n";
    } else {
        cout << "\n> Already at start point.\n";
    }
}


void findShortestPath() {
    if (currentRoom == treasureRoomPtr) { cout << "\nYou have arrived!\n"; return; }
    for (Room* r : allRooms) { r->visited = false; r->parent = nullptr; }
    queue<Room*> q; q.push(currentRoom); currentRoom->visited = true;
    bool found = false;
    while (!q.empty()) {
        Room* curr = q.front(); q.pop();
        if (curr == treasureRoomPtr) { found = true; break; }
        Room* neighbors[4] = {curr->north, curr->south, curr->east, curr->west};
        for (Room* n : neighbors) {
            if (n && !n->visited) { n->visited = true; n->parent = curr; q.push(n); }
        }
    }
    if (found) {
        cout << "\n[BFS ALGORITHM HINT] Shortest Path Calculated:\nStart";
        stack<Room*> pathStack; Room* c = treasureRoomPtr;
        while(c != currentRoom && c) { pathStack.push(c); c = c->parent; }
        while(!pathStack.empty()){ cout << " -> " << pathStack.top()->name; pathStack.pop(); }
        cout << endl;
    }
}


int main() {
    setupWorld();
    monsterBook.loadMonsters();
    playerInventory.createDefaultItems();
    cout << "=== DUNGEON NAVIGATOR ENGINE ONLINE ===\n";
    int choice = 0;
    while (true) {
        cout << "\n===============================\n[" << currentRoom->name << "]\n" << currentRoom->description << "\n===============================\n";
        cout << "1. Move North  | 2. Move South | 3. Move East | 4. Move West\n";
        cout << "5. Undo Move (Stack)\n6. Check Inventory (Linked List)\n";
        cout << "7. Check Bestiary (BST Search)\n8. Combat Demo (Queue)\n";
        cout << "9. Get Hint (BFS Algo)\n10. Quit\n> Selection [1-10]: ";
        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); continue; }

        switch (choice) {
            case 1: movePlayer("n"); break; case 2: movePlayer("s"); break;
            case 3: movePlayer("e"); break; case 4: movePlayer("w"); break;
            case 5: goBack(); break;
            case 6: playerInventory.showItems(); break;
            case 7: monsterBook.checkInfoInteractive(); break;
            case 8: simulateCombatDebug("Test Dummy"); break;
            case 9: findShortestPath(); break;
            case 10: return 0;
            default: cout << "Invalid selection.\n";
        }
    }
}