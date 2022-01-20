#ifndef LAB_BMgr

#include "DSMgr.h"
#include <cstdio>
#include <list>
using namespace std;
struct NewPage
{
    int page_id;
    int frame_id;
};
struct BCB 
{ 
 
    BCB(); 
    int page_id; 
    int frame_id; 
    int latch; 
    int count; 
    int dirty; 
    BCB * next; 
 
};
class BMgr 
{ 
    public: 
        BMgr(); 
      // Interface functions 
        int FixPage(int page_id, int prot); 
        NewPage FixNewPage(); 
        int UnfixPage(int page_id); 
        int NumFreeFrames(); 
    
      // Internal Functions 
        int SelectVictim(); 
        int Hash(int page_id); 
        void RemoveBCB(BCB * ptr, int page_id); 
        void RemoveLRUEle(int frid); 
        void SetDirty(int frame_id); 
        void UnsetDirty(int frame_id); 
        void WriteDirtys(); 
        void PrintFrame(int frame_id); 

        DSMgr storage;
    private: 
      // Hash Table 
        int ftop[DEFBUFSIZE]; 
        BCB* ptof[DEFBUFSIZE]; 
        bFrame *buffer[DEFBUFSIZE];
        list<int> lrulist;

        //func
        int newBCB(int page_id);
};

#endif