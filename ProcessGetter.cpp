// Purpose: Getting the process's (currerntly running) name, id, and memory usage
// Author(s): Raghad Mohammed Suliman Mohammed
// ID(s): 20220704
// Section: 12
// Date: 8.December.2023

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <windows.h>
#include <psapi.h>
#include <tchar.h>
using namespace std;


class prosecc{
protected :
    string name;
    string PID;
    long long memoUsag;
public:
    prosecc();
    prosecc(string n, string pid, float mu);
    void setName(string n);
    void setID (string id);
    void setMemo(long long usag);
    void diplayProseccInfo();

};
class proseccList : public prosecc{
private :
    vector <prosecc> prccList;
public:
    proseccList() = default;
    void loadFromOS();
    void displayList();
};



prosecc :: prosecc(){
    name = "";
    PID = "";
    memoUsag = 0;
}


prosecc :: prosecc(string name, string PID, float memoUsag){
    this->name = name;
    this->PID = PID;
    this->memoUsag = memoUsag;
}


void prosecc ::setName(std::string n) {
    name = n;
}


void prosecc ::setID(std::string id) {
    PID = id;
}


void prosecc ::setMemo(long long usag) {
    memoUsag = usag;
}


void prosecc:: diplayProseccInfo(){
    int width = 35;
    cout << left << setw(width) << setfill(' ') << name;
    cout << left << setw(width) << setfill(' ') << PID;
    cout << left << setw(width) << setfill(' ')<< setw(8)<< memoUsag <<"KB";
}


// The function that will get the process from the OS and load to it to the prosecc list vector
void proseccList :: loadFromOS() {

    prosecc aProcess;             // a prosecc object that will get pushed to the prosecc list
    DWORD processIDs[1024];       // array to store the process Ids that the function will load
    DWORD nNeededByts;            // to store the number of byets returned by the function
    DWORD nProcess;               // to store the number of returned processes
    if (EnumProcesses(processIDs, sizeof(processIDs), &nNeededByts) != 0) {

        // calculating how many processes the function returned
        nProcess = nNeededByts / sizeof(DWORD);

        //Creating handesl to pass to the GetProcessMemoryInfo
        HANDLE processHndl;
        HMODULE moduleHndl = NULL;
        TCHAR processName[MAX_PATH] = TEXT("unknown");    // initialize a buffer that will hold the name of a process


        PROCESS_MEMORY_COUNTERS pmc;      // variable of struct pointer to memory counter to get the memory usage of the process

        for (int i = 0; i < nProcess; i++) {


            //Getting the process handel by using the PID with open process function
            processHndl = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIDs[i]);


            // Making sure processHndl is pointing to an accessible process
            if (processHndl != NULL) {

                // Getting the memory usage of the process
                GetProcessMemoryInfo(processHndl, &pmc, sizeof(pmc));

                // Getting the processName
                GetModuleBaseName(processHndl, moduleHndl, processName, sizeof(processName) / sizeof(TCHAR));

                // setting the process informationes using setters
                aProcess.setName(processName);
                aProcess.setID(to_string(processIDs[i]));
                aProcess.setMemo((pmc.WorkingSetSize / 1024));
                prccList.push_back(aProcess);
            }
            CloseHandle(processHndl);
        }
    }
}


// printing all the prosecces that has been loaded informations : name , id, memory usage
void proseccList::displayList(){
    int width = 35;
    cout << left << setw(width) << setfill(' ') << "Name";
    cout << left << setw(width) << setfill(' ') << "PID";
    cout << left << setw(width) << setfill(' ') << "Memory Usage" << '\n';
    cout << "===================================================================================" << '\n';
    for (prosecc p : prccList){
        p.diplayProseccInfo();
        cout << '\n';
    }
}

int main() {


    proseccList pList;
    pList.loadFromOS();    // loading the process to the pList object we created
    pList.displayList();   // printing out the process informations to the console


    return 0;
}


















