#include "DSMgr.h"
#include <iostream>
#include <cstdio>
using namespace std;
int w = 0;
int r = 0;
DSMgr::DSMgr()
{
    cout<<MAXPAGES<<"\t"<<FRAMESIZE<<"\t"<<DEFBUFSIZE<<endl;
    currFile = NULL;
    numPages = 0;
    for (int i = 0; i < MAXPAGES; i++)
    {
        pages[i] = 0;
    }
}

int DSMgr::OpenFile(string filename)
{
    currFile = fopen(filename.c_str(), "r+");
    if (currFile == NULL)
    {
        return -1;
    }
    return 0;
}

int DSMgr::CloseFile()
{
    fclose(currFile);
    currFile == NULL;
    return 0;
}
int DSMgr::Seek(int offset, int pos)
{
    if(fseek(currFile, (long)(pos + offset), SEEK_SET) == 0)
        return 0;
    else
        return -1;
}
bFrame DSMgr::ReadPage(int page_id)
{
    bFrame tmpPage;
    Seek(page_id * PAGESIZE, 0);
    if(fread(&tmpPage, PAGESIZE, 1, currFile))
    {
        r += 1;
        return tmpPage;    
    }
}
int DSMgr::WritePage(int frame_id, bFrame frm)
{
    int tmpbytes;
    Seek(frame_id * FRAMESIZE, 0);
    tmpbytes = (int)fwrite(&frm, FRAMESIZE, 1, currFile);
    if (tmpbytes)
    {
        w += 1;
        return tmpbytes;
    }
    else
        return -1;
}

FILE* DSMgr::GetFile()
{
    return currFile;
}

void DSMgr::IncNumPages()
{
    numPages += 1;
}
int DSMgr::GetNumPages()
{
    return numPages;
}

void DSMgr::SetUse(int index, int use_bit)
{
    pages[index] = use_bit;
}

int DSMgr::GetUse(int index)
{
    return pages[index];
}