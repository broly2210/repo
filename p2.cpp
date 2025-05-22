#include <bits/stdc++.h>
using namespace std;

class OS
{
private:
    char Memory[400][4]; // Main memory with 400 words
    unsigned char R[4];  // General-purpose register
    unsigned char IR[4]; // Instruction register
    unsigned int IC;     // Instruction counter
    bool C;              // Toggle flag
    int SI;              // Interrupt
    char buffer[200];    // Buffer
    int PageTable[3] = {11, 23, 5}; // Page table entries
    const int PageSize = 10;        // Page size

public:
    void INIT();
    void PrintWelcome();
    void TranslateAddress(int VA);
    void READ();
    void WRITE();
};

void OS::INIT()
{
    // Initialize memory
    for (int i = 0; i < 400; i++)
        for (int j = 0; j < 4; j++)
            Memory[i][j] = ' ';

    // Initialize registers
    for (int i = 0; i < 4; i++)
    {
        IR[i] = ' ';
        R[i] = ' ';
    }

    C = false; // Clear toggle flag
    IC = 0;    // Reset instruction counter
    cout << "System initialized.\n";
}

void OS::PrintWelcome()
{
    // Load "WELCOME" into memory
    string welcome = "WELCOME";
    int loc = 0;
    for (char c : welcome)
    {
        Memory[loc / 4][loc % 4] = c;
        loc++;
    }
    Memory[loc / 4][loc % 4] = '\0'; // Null terminate
    cout << "Welcome message stored in memory.\n";
}

void OS::TranslateAddress(int VA)
{
    // Calculate page number and offset
    int pageNum = VA / PageSize;
    int offset = VA % PageSize;

    // Validate page number
    if (pageNum < 0 || pageNum >= 3)
    {
        cout << "Invalid virtual address! Page number out of bounds.\n";
        return;
    }

    // Translate to real address
    int realFrame = PageTable[pageNum];
    int realAddress = realFrame * PageSize + offset;

    cout << "Virtual Address " << VA << " maps to Real Address " << realAddress << ".\n";
}

void OS::READ()
{
    // Simulate reading "WELCOME" into memory
    string welcome = "WELCOME";
    int loc = 0;
    for (char c : welcome)
    {
        Memory[loc / 4][loc % 4] = c;
        loc++;
    }
    Memory[loc / 4][loc % 4] = '\0'; // Null terminate
}

void OS::WRITE()
{
    // Simulate writing memory content to output
    cout << "Outputting the welcome message:\n";
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (Memory[i][j] == '\0')
                break;
            cout << Memory[i][j];
        }
    }
    cout << endl;
}

int main()
{
    OS os;

    // Welcome message
    cout << "\t\tWelcome to Virtual Machine Simulator\n\n";

    // Initialize system
    cout << "Initializing the system...\n";
    os.INIT();

    // Store and display welcome message
    os.PrintWelcome();

    // Translate a virtual address
    int VA;
    cout << "Enter a virtual address to translate (e.g., 12): ";
    cin >> VA;
    os.TranslateAddress(VA);

    // Simulate output of welcome message
    os.WRITE();

    return 0;
}