#ifndef LAB_DSMgr
#define LAB_DSMgr


#include <string>


#define MAXPAGES 50000
#define FRAMESIZE 4096
#define DEFBUFSIZE 1024
#define PAGESIZE 4096

struct bFrame
{
    char field[FRAMESIZE];
};


class DSMgr 
{ 
public: 
    DSMgr(); 
    int OpenFile(std::string filename); 
    int CloseFile(); 
    bFrame ReadPage(int page_id); 
    int WritePage(int frame_id, bFrame frm); 
    int Seek(int offset, int pos); 
    FILE * GetFile(); 
    void IncNumPages(); 
    int GetNumPages(); 
    void SetUse(int index, int use_bit); 
    int GetUse(int index); 
private: 
    FILE *currFile; 
    int numPages; 
//    int basePage;
    int pages[MAXPAGES]; 
};  
#endif