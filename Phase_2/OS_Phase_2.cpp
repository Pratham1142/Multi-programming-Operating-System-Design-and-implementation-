#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <map>

using namespace std;

fstream fio;

void print_memory();
void get_in_buffer();
void ExecuteUserProgram();
void terminate(vector<int> em);
void print_memory();
char Main_Memory[300][4];
char IR[4];
int SI = 3;
int PI = 0;
int TI = 0;
int IC = 0;
char R[4];
bool C;
char buffer[40];
int flag = 1;
int PTR = -1;
int RA, TTC, LLC, TTL, TLL, EM, jobid;
map<int, string> errorMessage;

int PTrow = 0;

ifstream MyReadFile;

void Error_M_init()
{
    errorMessage[0] = "No error";
    errorMessage[1] = "Out of Data";
    errorMessage[2] = "Line Limit Exceeded";
    errorMessage[3] = "Time Limit Exceeded";
    errorMessage[4] = "Operation Code Error";
    errorMessage[5] = "Operand Error";
    errorMessage[6] = "Invalid Page Fault";
}

void displayPCB()
{
    fio << "Job id is: " << jobid << endl;
    fio << "TTC is: " << TTC << endl;
    fio << "LLC is: " << LLC << endl;
    fio << "TTL is: " << TTL << endl;
    fio << "TLL is: " << TLL << endl;
}

void displayProgramStatus()
{
    fio << "PCB Values - " << endl;
    displayPCB();
    fio << "IC is : " << IC << endl;
    fio << "C is : " << C << endl;
    fio << "SI is : " << SI << endl;
    fio << "TI is : " << TI << endl;
    fio << "PI is : " << PI << endl;
    fio << "IR is : " << endl;
    for (int i = 0; i < 4; i++)
    {
        fio << IR[i] << " ";
    }
    fio << endl;
    fio << "R is : ";
    for (int i = 0; i < 4; i++)
    {
        fio << R[i] << " ";
    }
    fio << endl;
}

bool checkpage(int address)
{
    if (Main_Memory[address][0] == '#' && Main_Memory[address][1] == '#' && Main_Memory[address][2] == '#' 
    && Main_Memory[address][4] == '#')
        return true;
    else
        return false;
}
int Address_Map(int VA)
{
    if ((VA < 0 || VA > 99) && (IR[2] != '#' && IR[3] != '#'))
    {
        PI = 2;
        return -1;
    }
    int PTE = PTR + VA / 10;
    RA = ((Main_Memory[PTE][2] - '0') * 10 + (Main_Memory[PTE][3] - '0')) * 10 + VA % 10;
    if (!(RA >= 0 && RA < 300) && (IR[2] != '#' && IR[3] != '#'))
    {
        PI = 3;
        return -1;
    }
    return RA;
}

string buffer_to_str(int i1, int i2)
{
    string j = "";
    for (int i = i1; i < i2; i++)
    {
        j += buffer[i];
    }
    return j;
}

int str_to_int(string number)
{
    int newNumber;
    for (int i = number.length() - 1; i >= 0; i--)
    {
        int power = number.length() - i - 1;
        newNumber += (std::pow(10.0, power) * (number[i] - '0'));
    }
    return newNumber;
}

void get_in_buffer()
{
    for (int i = 0; i < 40; i++)
    {
        buffer[i] = '#';
    }
    MyReadFile.getline(buffer, 40 + 1);
    for (int i = 0; i < 40; i++)
    {
        if (buffer[i] == '#')
        {
            buffer[i - 1] = '#';
            break;
        }
    }
}

void initialize_PCB()
{
    jobid = str_to_int(buffer_to_str(4, 8));
    TTL = str_to_int(buffer_to_str(8, 12));
    TLL = str_to_int(buffer_to_str(12, 16));
    TTC = 0;
    LLC = 0;
}

int Allocate()
{
    int a;

    while (true)
    {

        a = rand() % 30;
        // fio << "a" << a << endl;
        if (Main_Memory[a * 10][0] == '#' && Main_Memory[a * 10][1] == '#' && Main_Memory[a * 10][2] == '#'
         && Main_Memory[a * 10][3] == '#' && !(PTR == a * 10))
        {
            break;
        }
    }
    return a;
}

void Read()
{
    get_in_buffer();
    if (buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
    {
        terminate({1});
    }
    int loc = Address_Map((IR[2] - 48) * 10 + (IR[3] - '0'));
    for (int i = 0; i < 40; i++)
    {
        // fio<<"..."<<endl;
        Main_Memory[loc + i / 4][i % 4] = buffer[i];
    }
    // print_memory();
}

void Write()
{
    LLC++;
    if (LLC > TLL)
    {
        terminate({2});
    }
    int loc = Address_Map((IR[2] - 48) * 10 + (IR[3] - 48));

    int i = 0;
    while (Main_Memory[loc + i / 4][i % 4] != '#')
    {
        fio << Main_Memory[loc + i / 4][i % 4];
        i++;
    }
    // fio<<"Hello6"<<endl;
    // print_memory();
    fio << endl;
}

void terminate(vector<int> em)
{

    fio << endl
        << endl;

    TTC++;

    if (em[0] == 0)
    {
        fio << "Normal Termination" << endl;
    }
    else
    {   Error_M_init();
        fio << "Abnormal Termination" << endl;
        for (int i = 0; i < em.size(); i++)
        {   cout << em[i] << endl;
            fio << errorMessage[em[i]] << endl;
        }
    }

    displayProgramStatus();
    flag = 0;
}

void MOS()
{

    if (TI == 0 && SI == 1 && PI == 0)
    {
        Read();
    }
    else if (TI == 0 && SI == 2 && PI == 0)
    {
        Write();
    }
    else if (TI == 0 && SI == 3 && PI == 0)
    {
        terminate({0});
    }
    else if (TI == 2 && SI == 1 && PI == 0)
    {
        terminate({3});
    }
    else if (TI == 2 && SI == 2)
    {
        Write();
        terminate({3});
    }
    else if (TI == 2 && SI == 3 && PI == 0)
    {
        terminate({0});
    }

    else if (TI == 0 && PI == 1)
    {
        terminate({4});
    }
    else if (TI == 0 && PI == 2)
    {

        terminate({5});
    }
    else if (TI == 0 && PI == 3)
    {
        if ((IR[0] == 'G' && IR[1] == 'D') || (IR[0] == 'S' && IR[1] == 'R'))
        {
            // fio<<"Hello5 "<<PTrow<<endl;
            int x = Allocate();
            Main_Memory[PTR + PTrow][2] = char(x / 10 + 48);
            Main_Memory[PTR + PTrow][3] = char(x % 10 + 48);
            PTrow++;
            IC--;
            PI = 0;
            // print_memory();
            // exit(0);
            // ExecuteUserProgram();
        }
        else if((IR[0] != 'G' || IR[1] != 'D') || (IR[0] != 'S' || IR[1] != 'R'))
        {

            terminate({4});
        }
        else{
            terminate({6});
        }
        
    }
    else if (TI == 2 && PI == 1)
    {
        terminate({3, 4});
    }
    else if (TI == 2 && PI == 2)
    {
        terminate({3, 5});
    }
    else if (TI == 2 && PI == 3)
    {
        terminate({3});
    }

    TI = 0;
    PI = 0;
}

void ExecuteUserProgram()
{
    // fio << "Hello2" << endl;
    while (flag)
    {
        // fio << "Hello3" << endl;

        if (TTC == TTL)
        {
            TI = 2;
            break;
        }

        RA = Address_Map(IC);
        if (PI != 0)
        {
            fio << "HI " << PI << endl;
            break;
        }
        IR[0] = Main_Memory[RA][0];
        IR[1] = Main_Memory[RA][1];
        IR[2] = Main_Memory[RA][2];
        IR[3] = Main_Memory[RA][3];
        IC++;
        // fio << IR[0] << IR[1] << IR[2] << IR[3] << endl;
        RA = Address_Map(int((IR[2] - '0') * 10 + IR[3] - '0'));
        if (PI != 0)
        {
            // fio<<"HIII "<<PI<<endl;
            MOS();
            // fio<<"HIII "<<PI<<endl;
            // exit(0);

            continue;
        }
        // fio << IR[0] << IR[1] << IR[2] << IR[3] << endl;
        if (IR[0] == 'L' && IR[1] == 'R')
        {

            R[0] = Main_Memory[RA][0];
            R[1] = Main_Memory[RA][1];
            R[2] = Main_Memory[RA][2];
            R[3] = Main_Memory[RA][3];
        }
        else if (IR[0] == 'S' && IR[1] == 'R')
        {

            Main_Memory[RA][0] = R[0];
            Main_Memory[RA][1] = R[1];
            Main_Memory[RA][2] = R[2];
            Main_Memory[RA][3] = R[3];
            TTC++;
        }
        else if (IR[0] == 'C' && IR[1] == 'R')
        {

            int flag1 = 1;
            for (int i = 0; i < 4; i++)
            {
                if (Main_Memory[RA][i] != R[i])
                {
                    flag1 = 0;
                    break;
                }
            }
            if (flag1)
            {
                C = 1;
            }
            else
            {
                C = 0;
            }
        }
        else if (IR[0] == 'B' && IR[1] == 'T' && C == 1)
        {
            int loc = (IR[2] - 48) * 10 + (IR[3] - 48);
            IC = loc;
            IR[0] = Main_Memory[IC][0];
            IR[1] = Main_Memory[IC][1];
            IR[2] = Main_Memory[IC][2];
            IR[3] = Main_Memory[IC][3];
            continue;
        }
        else if (IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
            TTC++;
            MOS();
        }
        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            SI = 2;
            MOS();
        }
        else if (IR[0] == 'H')
        {
            SI = 3;
            MOS();
        }
        else
        {
            PI = 1;
            break;
        }
        TTC++;
    }

    if (PI != 0 || TI != 0)
    {
        MOS();
    }
}

void startexecution()
{
    IC = 0;
    // fio << "Hello1" << endl;
    ExecuteUserProgram();
}

void init()
{
    SI = 3;
    PI = 0;
    TI = 0;
    IC = 0;
    flag = 1;
    PTR = -1;
    PTrow = 0;
    for (int i = 0; i < 300; i++)
    {
        Main_Memory[i][0] = '#';
        Main_Memory[i][1] = '#';
        Main_Memory[i][2] = '#';
        Main_Memory[i][3] = '#';
    }
    for (int i = 0; i < 40; i++)
    {
        buffer[i] = '#';
    }
}

void load()
{
    int m = 0;
    srand(time(NULL));
    MyReadFile.open("input.txt");
    fio.open("output.txt", ios::trunc | ios::out | ios::in);
    while (MyReadFile)
    {
        get_in_buffer();
        // for (int i = 0; i < 40; i++)
        // {
        //     fio << buffer[i] << " ";
        // }
        // fio << endl;

        if (buffer[0] == '$')
        {
            if (buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J')
            {
                initialize_PCB();
                int k = Allocate();
                PTR = k * 10;
            }
            else if (buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A')
            {
                // fio << "yo" << endl;
                startexecution();
            }
            else if (buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
            {
                fio << endl;
                print_memory();
                flag = 1;
                init();
                continue;
            }
            else
            {
                fio << "Wrong Instruction" << endl;
                break;
            }
        }
        else
        {

            int x = Allocate();
            Main_Memory[PTR + PTrow][2] = char(x / 10 + 48);
            Main_Memory[PTR + PTrow][3] = char(x % 10 + 48);

            x = x * 10;
            for (int i = 0; i < 40; i++)
            {
                if (buffer[i] != 'H')
                {
                    Main_Memory[x][0] = buffer[i];
                    Main_Memory[x][1] = buffer[i + 1];
                    Main_Memory[x][2] = buffer[i + 2];
                    Main_Memory[x][3] = buffer[i + 3];
                    x++;
                    i += 3;
                }
                else if (buffer[i] == 'H')
                {
                    Main_Memory[x][0] = buffer[i];
                    Main_Memory[x][1] = '#';
                    Main_Memory[x][2] = '#';
                    Main_Memory[x][3] = '#';
                    x++;
                }
            }
            PTrow++;
        }
        for (int i = 0; i < 300; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (Main_Memory[i][j] == '\0')
                {
                    Main_Memory[i][j] = '#';
                }
            }
        }
    }
}

void print_memory()
{
    for (int i = 0; i < 300; i++)
    {
        fio << i << " ---> ";
        for (int j = 0; j < 4; j++)
        {
            fio << Main_Memory[i][j] << " ";
        }
        fio << endl;
    }
    fio << endl
        << endl
        << endl;
}

int main()
{
    init();
    load();
    return 0;
}