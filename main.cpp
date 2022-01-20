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
    FILE *f = fopen("data.dbf", "w");
    fclose(f);
    BMgr BufManager, Init;
    for (int i = 0; i < MAXPAGES; i++)
    {
        cout<<"breakpoint: "<<i<<endl;
        NewPage np;
        np = Init.FixNewPage();
        Init.UnfixPage(np.page_id);
    }
    Init.storage.CloseFile();
    cout << "hit:" << hit << "  miss:" << miss << endl;
    cout<<"begin test"<<endl;
    hit = 0;
    miss = 0;
    r = 0;
    w = 0;
    FILE *file2 = fopen("data-5w-50w-zipf.txt", "r");
    //    FILE* file2 = fopen("t2.txt", "r");
    int operation;
    int page;
    int frame_id;
    int i = 0;
    while (fscanf(file2, "%d,%d", &operation, &page) != EOF)
    {
        i++;
        page -= 1;
        frame_id = BufManager.FixPage(page, 0);
        if (operation == 1)
        {
            //if write, set frame to dirty
            BufManager.SetDirty(frame_id);
        }
        BufManager.UnfixPage(page);
    }
    BufManager.WriteDirtys();
    BufManager.storage.CloseFile();
    cout << "total tests:" << i << "\nhit: " << hit << " miss:" << miss << " read:" << r << " write:" << w << endl;
    return 0;
}