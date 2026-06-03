#include <iostream>
#include <string>

using namespace std;

// ─────────────────────────────────────────
// PHASE 1: The Node struct
// ─────────────────────────────────────────

struct Node
{
    string key;
    string value;
    Node *prev;
    Node *next;

    // Constructor
    Node(string k, string v)
    {
        key = k;
        value = v;
        prev = nullptr;
        next = nullptr;
    }
};

/* LRU Cache Class Skeleton*/

class LRUCache
{
private:
    int capacity;
    int currentSize;
    Node *head;         // pointer to the front of the list (most recently used)
    Node *tail;         // pointer to the end of the list (least recently used)
    Node **lookupTable; // array of node pointers for fast key search

    /* PHASE 2 : PRivate Helper Functions*/

    // Helper 1 : inserting a node at the VERY front (MRU position)
    void insertAtHead(Node *node)
    {
        node->next = head;
        node->prev = nullptr;

        if (head != nullptr)
        {
            head->prev = node; // linking old head onto our new node
        }

        head = node; // move the head pointer to the new node

        // empty list

        if (tail == nullptr)
        {
            tail = head;
        }
    }

    // Helper 2: Break a node's links to remove it from the chain
    void removeNode(Node *node)
    {
        // if it is the head node, move node towards the right
        if (node == head)
        {
            head = head->next;
        }

        // if its the tail node, shift the tail to the left
        if (node == tail)
        {
            tail = tail->prev;
        }

        // bridging the gap between the neighbours (if they exist)

        if (node->next != nullptr)
        {
            node->next->prev = node->prev;
        }

        if (node->prev != nullptr)
        {
            node->prev->next = node->next;
        }

        // isolate the node completely

        node->next = nullptr;
        node->prev = nullptr;
    }
    // Find the index of a key in the lookup table (-1 if not found)

    int findIndex(string key)
    {
        for (int i = 0; i < capacity; i++)
        {
            if (lookupTable[i] != nullptr && lookupTable[i]->key == key)
            {
                return i; // found it at index i
            }
        }
        return -1; // not found
    }

    int findEmptySlot()
    {
        for (int i = 0; i < capacity; i++)
        {
            if (lookupTable[i] == nullptr)
            {
                return i;
            }
        }
        return -1; // should never happen if logic is correct
    }

public:
    // Constructor
    LRUCache(int cap)
    {
        capacity = cap;
        currentSize = 0;
        head = nullptr;
        tail = nullptr;

        // Allocate the lookup array with 'cap' slots
        // Each slot starts as nullptr (empty)

        lookupTable = new Node *[capacity];
        for (int i = 0; i < capacity; i++)
        {
            lookupTable[i] = nullptr;
        }
    }

    Node *getHead()
    {
        return head;
    }

    // ─────────────────────────────────────────
    // PHASE 3: Core get() and set() logic
    // ─────────────────────────────────────────

    string get(string key)
    {
        int idx = findIndex(key);

        // cache miss - key doesnt exist
        if (idx == -1)
        {
            return "MISS";
        }

        // cache hit - move this node to head (most recently used)
        Node *targetNode = lookupTable[idx];
        removeNode(targetNode);
        insertAtHead(targetNode);

        return targetNode->value;
    }

    void set(string key, string value)
    {
        int idx = findIndex(key);

        // case 1: key already exists : update the value and move to head

        if (idx != -1)
        {
            Node *existingNode = lookupTable[idx];
            existingNode->value = value;
            removeNode(existingNode);
            insertAtHead(existingNode);
            return;
        }

        // Case 2: New key — check if cache is full
        if (currentSize == capacity)
        {
            // Evict the tail (least recently used)
            cout << "Eviction Warning: Evicting LRU Key (" << tail->key << ")." << endl;

            // Find and clear the tail's slot in lookup table
            int tailIdx = findIndex(tail->key);
            lookupTable[tailIdx] = nullptr;

            // remove from list and free memory
            Node *lruNode = tail;
            removeNode(lruNode);
            delete lruNode;

            currentSize--;
        }

        // Create new node and add to lookup table + list
        Node *newNode = new Node(key, value);
        int emptySlot = findEmptySlot();
        lookupTable[emptySlot] = newNode;
        insertAtHead(newNode);
        currentSize++;
    }

    // Destructor
    ~LRUCache()
    {
        Node *current = head;
        while (current != nullptr)
        {
            Node *nextNode = current->next;
            delete current;
            current = nextNode;
        }

        // free the lookup array itself
        delete[] lookupTable;
        cout << "Cleaning up memory ... All allocated nodes deleted safely." << endl;
    }
};

// ─────────────────────────────────────────
// PHASE 4: Interactive CLI Loop
// ─────────────────────────────────────────

void printState(Node *head)
{
    if (head == nullptr)
    {
        cout << "Cache is empty " << endl;
        return;
    }

    cout << "[Head] ->";
    Node *current = head;
    while (current != nullptr)
    {
        cout << "(" << current->key << ": " << current->value << ")";
        if (current->next != nullptr)
        {
            cout << " <-> ";
        }
        current = current->next;
    }
    cout << " -> [Tail]" << endl;
}

int main()
{
    cout << "===========================================" << endl;
    cout << "  INITIALIZING CUSTOM LRU CACHE SIMULATOR " << endl;
    cout << "===========================================" << endl;

    int capacity;
    cout << "Enter maximum cache capacity: ";
    cin >> capacity;

    // Input validation — capacity must be at least 1
    while (capacity < 1)
    {
        cout << "Capacity must be at least 1. Try again: ";
        cin >> capacity;
    }

    LRUCache cache(capacity);
    cout << "Cache initialized with capacity " << capacity << "." << endl
         << endl;

    string command;

    while (true)
    {
        cout << "Enter Command (SET key val / GET key / EXIT): ";
        cin >> command;

        // Convert command to uppercase so "set", "Set", "SET" all work
        for (int i = 0; i < command.length(); i++)
        {
            command[i] = toupper(command[i]);
        }

        // ── EXIT ──────────────────────────────────
        if (command == "EXIT")
        {
            cout << "Exiting Program. Goodbye!" << endl;
            break;
        }

        // ── GET ───────────────────────────────────
        else if (command == "GET")
        {
            string key;
            cin >> key;

            string result = cache.get(key);

            if (result == "MISS")
            {
                cout << "Cache Miss! ERR: Key '" << key << "' is not present in cache." << endl;
            }
            else
            {
                cout << "Cache Hit! Value: " << result << endl;
                printState(cache.getHead());
            }
        }
        // ── SET ───────────────────────────────────

        else if (command == "SET")
        {
            string key, value;
            cin >> key >> value;

            cache.set(key, value);
            cout << "Success! ";
            printState(cache.getHead());
        }

        // ── INVALID ────────────────────────────────
        else
        {
            cout << "Invalid command. Use SET key val, GET key, or EXIT." << endl;
        }
        cout << endl;
    }
    return 0;
}