#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
using namespace std;


ifstream fin("input.txt");
ofstream fout("output.txt");

char M[300][4];
char buffer[40];
char IR[4];
char R[5];
int IC;
int C;
int SI;
int PI;
int TI;
int PTR;
bool breakFlag;

struct PCB{
    int job_id;
    int TTL;
    int TLL;
    int TTC;
    int LLC;

    void setPCB(int id, int ttl, int tll){
        job_id = id;
        TTL = ttl;
        TLL = tll;
        TTC = 0;
        LLC = 0;
    }
};

PCB pcb;

string error[7] = {"No Error", "Out of Data", "Line Limit Exceeded", "Time Limit Exceeded",
    "Operation Code Error", "Operand Error", "Invalid Page Fault"};

void init();
void read(int RA);
void write(int RA);
int addressMap(int VA);
void execute_user_program();
void start_execution();
int allocate();
void load();

void init(){
    memset(M, '\0', 1200);
    memset(IR, '\0', 4);
    memset(R, '\0', 5);
    C = 0;
    SI = 0;
    PI = 0;
    TI = 0;
    breakFlag = false;
}
void Terminate(int EM, int EM2=-1){
    fout << endl << endl;
    if(EM == 0){
        fout << " terminated normally. " << error[EM]<<endl;
    }
    else{
        fout <<" terminated abnormally due to " << error[EM] << (EM2 != -1 ? (". " + error[EM2]) : "") << endl;
        fout << "IC="<<IC<<", IR="<<IR<<", C="<<C<<", R="<<R<<", TTL="<<pcb.TTL<<", TTC="<<pcb.TTC<<", TLL="<<pcb.TLL<<", LLC="<<pcb.LLC;
    }
}
void load(){
    int m;                                  
    int currPTR;                            
    char temp[5];                           
    memset(buffer, '\0', 40);

    while(!fin.eof()){
        fin.getline(buffer,41);
        memset(temp, '\0', 5);
        memcpy(temp,buffer,4);

        if(!strcmp(temp,"$AMJ")){
            init();

            int jobId, TTL, TLL;
            memcpy(temp, buffer+4, 4);
            jobId = atoi(temp);
            memcpy(temp, buffer+8, 4);
            TTL = atoi(temp);
            memcpy(temp, buffer+12, 4);
            TLL = atoi(temp);
            pcb.setPCB(jobId, TTL, TLL);

            PTR = allocate()*10;
            memset(M[PTR], '*', 40);
            currPTR = PTR;
        }
        else if(!strcmp(temp,"$DTA")){
            start_execution();
        }
        else if(!strcmp(temp,"$END")){
            continue;
        }
        else{
            if(breakFlag) continue;

            do{
                m = allocate();
            }while(M[m*10][0]!='\0');

            itoa(m, M[currPTR], 10);
            currPTR++;

            strcpy(M[m*10],buffer);

            cout << "PTR = " << PTR << endl;
            for(int i=0; i<300; i++){
                cout<<"M["<<i<<"] :";
                for(int j=0 ; j<4; j++){
                    cout<<M[i][j];
                }
                cout<<endl;
            }
            cout<<endl;
        }
    }
}
int main(){
    load();
    fin.close();
    fout.close();
    return -1;
}