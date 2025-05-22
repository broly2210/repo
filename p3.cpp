#include <bits/stdc++.h>
using namespace std;

class OS
{
private:
    char Memory[200][4];  // Main memory with 200 words
    unsigned char R[4];   // General-purpose register
    unsigned char IR[4];  // Instruction register
    unsigned int IC;      // Instruction counter
    bool C;               // Toggle bit (condition flag)
    int SI;               // Service Interrupt
    char buffer[200];     // Buffer for file handling

public:
    void INIT();
    void LOAD();
    void EXECUTE();
    void MOS();
    void Start();
    int OppAdd();
    void READ();
    void WRITE();
    void HALT();
    void handleNR(); // New instruction: NR

    fstream inputfile;
    fstream outputfile;
};

void OS::INIT()
{
    for (int i = 0; i < 200; i++)
        for (int j = 0; j < 4; j++)
            Memory[i][j] = ' ';

    for (int i = 0; i < 4; i++)
    {
        IR[i] = ' ';
        R[i] = ' ';
    }
    C = false;
    IC = 0;
}

void OS::LOAD()
{
    cout << "Reading Data from Input File...\n";
    int memIndex = 0;

    while (!inputfile.eof())
    {
        inputfile.getline(buffer, 42);

        if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J')
        {
            INIT();
        }
        else if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A')
        {
            Start();
        }
        else if (buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
        {
            cout << "Execution Completed.\n";
        }
        else
        {
            int k = 0;
            for (int i = memIndex; i < memIndex + 10; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (buffer[k] != '\0')
                        Memory[i][j] = buffer[k++];
                }
            }
            memIndex += 10;
        }
    }
}

int OS::OppAdd()
{
    int addr = (IR[2] - '0') * 10 + (IR[3] - '0');
    return addr;
}

void OS::Start()
{
    IC = 0;
    EXECUTE();
}

void OS::EXECUTE()
{
    while (true)
    {
        // Load instruction into IR
        for (int i = 0; i < 4; i++)
            IR[i] = Memory[IC][i];
        IC++;

        int loc = OppAdd();

        if (IR[0] == 'G' && IR[1] == 'D') // GD
        {
            SI = 1;
            MOS();
        }
        else if (IR[0] == 'P' && IR[1] == 'D') // PD
        {
            SI = 2;
            MOS();
        }
        else if (IR[0] == 'H') // H
        {
            SI = 3;
            MOS();
            break;
        }
        else if (IR[0] == 'L' && IR[1] == 'R') // LR
        {
            for (int j = 0; j < 4; j++)
                R[j] = Memory[loc][j];
        }
        else if (IR[0] == 'S' && IR[1] == 'R') // SR
        {
            for (int j = 0; j < 4; j++)
                Memory[loc][j] = R[j];
        }
        else if (IR[0] == 'C' && IR[1] == 'R') // CR
        {
            int count = 0;
            for (int j = 0; j < 4; j++)
                if (Memory[loc][j] == R[j])
                    count++;

            C = (count == 4);
        }
        else if (IR[0] == 'N' && IR[1] == 'R') // NR
        {
            handleNR();
        }
        else if (IR[0] == 'B' && IR[1] == 'T') // BT
        {
            if (C)
            {
                IC = loc;
                C = false;
            }
        }
        else
        {
            cout << "Invalid Instruction Encountered: " << IR[0] << IR[1] << endl;
            break;
        }
    }
}

void OS::handleNR()
{
    // Set C = true if R and Memory[loc] are NOT equal
    int loc = OppAdd();
    int count = 0;
    for (int j = 0; j < 4; j++)
        if (Memory[loc][j] != R[j])
            count++;

    C = (count > 0); // Set true if there are differences
}

void OS::MOS()
{
    switch (SI)
    {
    case 1:
        READ();
        break;
    case 2:
        WRITE();
        break;
    case 3:
        HALT();
        break;
    default:
        cout << "Invalid Service Interrupt!" << endl;
    }
}

void OS::READ()
{
    inputfile.getline(buffer, 42);
    int loc = OppAdd();

    int k = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
            Memory[loc + i][j] = buffer[k++];
    }
}

void OS::WRITE()
{
    int loc = OppAdd();

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
            outputfile << Memory[loc + i][j];
    }
    outputfile << "\n";
}

void OS::HALT()
{
    outputfile << "Program Halted.\n";
}

int main()
{
    OS os;
    cout << "Virtual Machine Simulation with File Handling\n";
    os.inputfile.open("input.txt", ios::in);
    os.outputfile.open("output.txt", ios::out);

    if (!os.inputfile)
    {
        cout << "Input file not found.\n";
        return 0;
    }

    os.LOAD();

    os.inputfile.close();
    os.outputfile.close();

    return 0;
}