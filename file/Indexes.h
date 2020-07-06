//
//  Header.h
//  file
//
//  Created by 斯金泽 on 2020/4/1.
//  Copyright © 2020 斯金泽. All rights reserved.
//
#ifndef Indexed_h
#define Indexed_h
#include "ListKey.h"
struct NodeIndexed{
    listKey *body;
    NodeIndexed *next;
    NodeIndexed *nextLevel;
};
class indexde{
private:
    NodeIndexed *head;
public:
    indexde(){
        head=new NodeIndexed;
        head->body=NULL;
        head->next=NULL;
        head->nextLevel=NULL;
    }
    ~indexde()
    {
       // reset();
       // delete head;
    }
    string getName(int level,int order)
    {
        NodeIndexed *tmp=head;
                for(int i=1;i<level;i++)
                {
                    tmp=tmp->nextLevel;
                    if(tmp==NULL)
                    {
                        return 0;
                    }
                }
                for(int i=1;i<order;i++)
                {
                    tmp=tmp->next;
                    if(tmp==NULL)
                    {
                        return 0;
                    }
                }
                return tmp->body->name;
    }
    void increase(listKey *tmp){
        if(tmp->order==0&&tmp->level==1)
        {
            listKey *del=head->body;
            head->body=tmp;
            if(del!=NULL)
               del->deleteSelf();
            return ;
        }
        NodeIndexed *p=head;
        for(int i=1;i<tmp->level;i++)
        {
            if(p->nextLevel==NULL)
            {
                NodeIndexed* p1=new NodeIndexed;
                p1->body=NULL;
                p1->next=NULL;
                p1->nextLevel=NULL;
                p->nextLevel=p1;
            }
            p=p->nextLevel;
        }
        for(int i=0;i<tmp->order;i++)
        {
            if(p->next==NULL)
            {
                NodeIndexed* p1=new NodeIndexed;
                p1->body=NULL;
                p1->next=NULL;
                p1->nextLevel=NULL;
                p->next=p1;
            }
            p=p->next;
        }
        if(p->body==NULL)
        {
            p->body=tmp;
        }
        else{
            listKey *del=p->body;
            p->body=tmp;
            if(del!=NULL)
               del->deleteSelf();
        }
        
        return;
    }
    void input(){
        NodeIndexed *p=head->nextLevel;
        while(p)
        {
            NodeIndexed *p1=p;
            while(p1)
            {
                cout<<p1->body->max<<" "<<p1->body->min<<p1->body->name<<endl;
                p1=p1->next;
            }
            p=p->nextLevel;
        }
    
    
    }
    uint64_t getMin(int level,int order)
    {
        NodeIndexed *tmp=head;
        for(int i=1;i<level;i++)
        {
            tmp=tmp->nextLevel;
            if(tmp==NULL)
            {
                return 0;
            }
        }
        for(int i=1;i<order;i++)
        {
            tmp=tmp->next;
            if(tmp==NULL)
            {
                return 0;
            }
        }
        if(tmp->body==NULL)
                   return 0;
        return tmp->body->min;
    }
    uint64_t getMax(int level,int order)
    {
        NodeIndexed *tmp=head;
        for(int i=1;i<level;i++)
        {
            tmp=tmp->nextLevel;
            if(tmp==NULL)
            {
                return 0;
            }
        }
        for(int i=1;i<order;i++)
        {
            tmp=tmp->next;
            if(tmp==NULL)
            {
                return 0;
            }
        }
        if(tmp->body==NULL)
            return 0;
        return tmp->body->max;
    }
    listKey* get(int level,int ordr)
    {
        NodeIndexed *tmp=head;
        for(int i=1;i<level;i++)
        {
            tmp=tmp->nextLevel;
            if(tmp==NULL)
            {
                return NULL;
            }
        }
        for(int i=1;i<ordr;i++)
        {
            tmp=tmp->next;
            if(tmp==NULL)
            {
                return NULL;
            }
        }
        return tmp->body;
    }
    void del(int level,int ordr)
    {
        NodeIndexed *tmp=head;
        for(int i=1;i<level;i++)
        {
            tmp=tmp->nextLevel;
            if(tmp==NULL)
            {
                return ;
            }
        }
        for(int i=1;i<ordr;i++)
        {
            tmp=tmp->next;
            if(tmp==NULL)
            {
                return ;
            }
        }
        tmp->body->deleteSelf();
        tmp->body=NULL;
    }
    string search(uint64_t key,string dirtmp)
    {
        NodeIndexed* tmp =this->head;
        
            
        if(tmp->next!=NULL&&tmp->next->body!=NULL)
        {
            string temp=searchKey(key,1,2,dirtmp);
            if(temp!="")
                return temp;
        }
        if(tmp->body!=NULL)
        {
            string temp=searchKey(key,1,1,dirtmp);
            if(temp!="")
                return temp;
        }
        
        int numlevel=2;
        tmp=tmp->nextLevel;
        while(tmp)
        {
            int num=1;
            NodeIndexed * tmp2=tmp;
            while(tmp2)
            {
                
                if(tmp2->body!=NULL&&tmp2->body->max>=key&&tmp2->body->min<=key)
                {
                    string temp=searchKey(key,numlevel,num,dirtmp);
                    if(temp!="")
                    {
                        return temp;
                    }
                    
                    break;
                }
                num++;
                tmp2=tmp2->next;
            }
            numlevel++;
            tmp=tmp->nextLevel;
        }
        return "";
    }
    string searchKey(uint64_t key ,int level,int order,string dirtemp)
    {
        NodeIndexed *tmp=head;
        for(int i=1;i<level;i++)
            tmp=tmp->nextLevel;
        for(int i=1;i<order;i++)
        {
            tmp=tmp->next;
        }
        int num=tmp->body->numberofKey;
        
        int low=0,high=num-1,mid;
            while(low<=high)
            {
                mid=(low+high)/2;
                if(tmp->body->Key[mid]==key)
                {
                    uint64_t value=tmp->body->value[mid];
                    uint64_t valueTwo=0;
                    if(mid==num-1)
                    {
                        valueTwo=tmp->body->num;
                    }
                    else valueTwo=tmp->body->value[mid+1];
                    if(level>9)
                    {
                    
                        char a='0'+(level/10);
                        char b='0'+(level%10);
                        dirtemp=dirtemp+'/'+a+b;
                        
                    }
                    else{
                        char a='0'+level;
                        dirtemp=dirtemp+'/'+a;
                    }
                    dirtemp=dirtemp+'/'+tmp->body->name;
                    uint64_t numtmp=0;
                    if(mid==num-1)
                        numtmp=valueTwo-value;
                    else
                        numtmp=valueTwo-value-8;
                    path str(dirtemp);
                    ifstream inFile(str,ios::in|ios::binary);
                    inFile.seekg(value,ios::beg);
                    char * val=new char[numtmp+1];
                    inFile.read(val,numtmp);
                    val[numtmp]='\0';
                    string result=val;
                    return result;
                    
                }
                if(tmp->body->Key[mid]>key)
                        high=mid-1;
                else
                        low=mid+1;
            }
            return "";
    }
    void reset()
    {
       NodeIndexed* p=head;
        NodeIndexed*q=p;
        while(p)
        {
            q=p;
            while(q)
            {
                NodeIndexed *tmp=q;
                if(q->body!=NULL)
                    q->body->deleteSelf();
                q=q->next;
                delete tmp;
            }
            p=p->nextLevel;
        }
        //head=new NodeIndexed;
        head=new NodeIndexed;
        head->body=NULL;
        head->next=NULL;
        head->nextLevel=NULL;
    }
};
#endif
