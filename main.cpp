#include "BMgr.h"
#include <iostream>
#include <cstdlib>

using namespace std;

extern int hit;
extern int miss;
extern int r;
extern int w;

int main()
{
    FILE* f = fopen("data.dbf", "w");
    fclose(f);
    BMgr BufManager, Init;
    for (int i = 0; i < DEFBUFSIZE; i++)
    {
        NewPage np;
        np = BufManager.FixNewPage();
        BufManager.UnfixPage(np.page_id);
    }
    Init.storage.CloseFile();
    cout<<"hit:"<<hit<<"  miss:"<<miss<<endl;
    hit = 0;
    miss = 0;
    r = 0;
    w = 0;
    
}