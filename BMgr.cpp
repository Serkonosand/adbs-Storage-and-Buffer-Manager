#include "BMgr.h"

int hit = 0;
int miss = 0;
BCB::BCB()
{
    page_id = -1;
    frame_id = -1;
    count = 0;
    dirty = 0;
    latch = 0;
    next = NULL;
}

BMgr::BMgr()
{
    for (int i = 0; i < DEFBUFSIZE; i++)
    {
        ftop[i] = -1;
        ptof[i] = NULL;
        buffer[i] = (bFrame*)malloc(sizeof(bFrame));
    }
    storage.OpenFile("data.dbf");
}

int BMgr::newBCB(int page_id)
{
    int frame_id = -1;
    BCB* bcb = new BCB();
    if (lrulist.size() == DEFBUFSIZE)
    {
        frame_id = SelectVictim();
        RemoveLRUEle(frame_id);
    }
    bcb->frame_id = page_id;
    bool sign = false;
    for (int i = 0; i < DEFBUFSIZE; i++)
    {
        if(ftop[i] == -1)
        {
            ftop[i] = page_id;
            bcb->frame_id = i;
            frame_id = bcb->frame_id;
            sign = true;
            break;
        }
    }
    if (!sign)
    {
        printf("ERROR/BMgr.cpp-47: STH WRONG IN BUFFER MANAGER ! \n");
        return -1;
    }
    BCB * hd = ptof[Hash(bcb->page_id)];
    if (!hd)
        ptof[Hash(bcb->page_id)] = bcb;
    else
    {
        while (hd->next)
            hd = hd->next;
        hd->next = bcb;
    }
    bcb->count += 1;
    bcb->latch = 1;
    lrulist.push_front(frame_id);
    return frame_id;    
}

int BMgr::FixPage(int page_id, int prot)
{
    int fid = -1;
    BCB* bcb;
    if (page_id < MAXPAGES && page_id >= 0)
    {
        bcb = ptof[Hash(page_id)];
        while (bcb != NULL)
        {
            if (bcb->page_id == page_id)
            {
                hit += 1;
                fid = bcb->frame_id;
                RemoveLRUEle(fid);
                lrulist.push_front(fid);
                bcb->count += 1;
                bcb->latch = 1;
                break;
            }
            bcb = bcb->next;
        }
        if (bcb == NULL)
        {
            miss += 1;
            fid = newBCB(page_id);
            memcpy(buffer[fid], (storage.ReadPage(page_id).field),\
                     sizeof(*(buffer[fid])));
        }
        return fid;
    }
    else{
        printf("ERROR/BMgr.h-75:WRONG PAGE_ID\n");
        return -1;
    }
}

NewPage BMgr::FixNewPage()
{
    NewPage np;
    int i = 0;
    for (int i = 0; i < storage.GetNumPages(); i++)
    {
        if(storage.GetUse(i) == 0)
            break;
    }
    if (i == storage.GetNumPages())
    {
        storage.IncNumPages();
        storage.SetUse(i, 1);
    }
    np.page_id = i;
    np.frame_id = FixPage(i, 0);
    return np;
}

int BMgr::UnfixPage(int page_id)
{
    BCB* bcb = ptof[Hash(page_id)];
    while (bcb->next != NULL && bcb->page_id != page_id)
    {
        bcb = bcb->next;
    }
    if (bcb != NULL)
    {
        if (bcb->count == 0)
            bcb->latch = 0;
        else   
            bcb->count = bcb->count - 1;
    }
    return bcb->frame_id;
}

int BMgr::NumFreeFrames()
{
    int val = 0;
    for (int i = 0; i < DEFBUFSIZE; i++)
    {
        if (ftop[i] == -1)
        {
            val += 1;
        }
    }
    return val;
}

int BMgr::SelectVictim()
{
    int val;
    BCB* bcb = NULL;
    list<int>::iterator it = lrulist.end();
    while (1)
    {
        if (it == lrulist.begin())
            break;
        it-- ;
        bcb = ptof[Hash(ftop[*it])];
        while (bcb->frame_id != *it && bcb->next != NULL) 
        {
            bcb = bcb->next;
        }
        if (bcb->frame_id == *it && bcb->latch == 0)
        {
            break;
        }
    }
    if (bcb->frame_id != *it || bcb->latch != 0)
    {
        printf("ERROR/BMgr.cpp-175:ALL FRAME ARE USING!\n");
        return -1;
    }
    else
    {
        val = bcb->frame_id;
        if (bcb->dirty == 1)
        {
            storage.WritePage(bcb->page_id, *(buffer[val]));
        }
        RemoveBCB(bcb, bcb->page_id);
        return val;
    }
}

int BMgr::Hash(int page_id)
{
    return page_id % DEFBUFSIZE;
}

void BMgr::RemoveBCB(BCB * ptr, int page_id)
{
    BCB* head = ptof[Hash(page_id)];
    ftop[ptr->frame_id] = -1;
    if (head == ptr)
    {
        ptof[Hash(page_id)] = ptr->next;
        delete ptr;
    }
    else
    {
        while (head->next != ptr)
        {
            head = head->next;
        }
        head->next = ptr->next;
        delete ptr;
    }
}

void BMgr::RemoveLRUEle(int frid)
{
    lrulist.remove(frid);
}

void BMgr::SetDirty(int frame_id)
{
    BCB* bcb = ptof[Hash(ftop[frame_id])];
    while (bcb->next != NULL && bcb->frame_id != frame_id)
    {
        bcb = bcb->next;
    }
    bcb->dirty = 1;
}

void BMgr::UnsetDirty(int frame_id)
{
    BCB* bcb = ptof[Hash(ftop[frame_id])];
    while (bcb->next != NULL && bcb->frame_id != frame_id)
    {
        bcb = bcb->next;
    }
    bcb->dirty = 0;
}

void BMgr::WriteDirtys()
{
    list<int>::iterator it = lrulist.begin();
    while (it != lrulist.end())
    {
        BCB* bcb = ptof[Hash(ftop[*it])];
        while (bcb->next != NULL && bcb->frame_id != *it)
            bcb = bcb->next;
        if (bcb->dirty == 1)
        {
            storage.WritePage(bcb->page_id, *(buffer[bcb->frame_id]));
        }
        it++;
    }
    
}

void BMgr::PrintFrame(int frame_id)
{
    printf("frame_id: %d \t value: %s \n ", frame_id, (*(buffer[frame_id])->field));
}