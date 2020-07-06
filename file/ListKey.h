#ifndef ListKey_h
#define ListKey_h
#include <iostream>
#include <fstream>
using namespace std;
using namespace std::__fs::filesystem;
namespace fs=std::__fs::filesystem;
struct NodeKey{
       uint64_t num;
       uint64_t value;
       NodeKey *next;
   };

class listKey {
   
public:
    NodeKey *head;
    uint64_t num;
    uint64_t min;
    uint64_t max;
    int numberofKey;
    int order;
    int level;
    uint64_t *Key;
    uint64_t *value;
    string name;
public:
    listKey()
    {
        head=new NodeKey;
        head->next=NULL;
        num=0;
        numberofKey=0;
    }
    void push(NodeKey * tmp)
    {
        NodeKey *p=head;
        if(p->next==NULL)
        {
            p->next=tmp;
            num+=sizeof(tmp->num);
            num+=sizeof(tmp->value);
            max=min=tmp->num;
            return ;
        }
        NodeKey*q=p->next;
        while(q!=NULL&&q->num<tmp->num)
        {
            p=p->next;
            q=q->next;
        }
        if(q==NULL)
        {
            p->next=tmp;
        }
        else{
            p->next=tmp;
            tmp->next=q;
        }
        num+=sizeof(tmp->num);
        num+=sizeof(tmp->value);
        if(tmp->num>max)
            max=tmp->num;
        if(tmp->num<min)
            min=tmp->num;
        
    }
    uint64_t getnum()
    {
        return num;
    }
    bool isempty()
    {
        return head->next==NULL;
    }
    NodeKey * pop()
    {
        NodeKey* p=head->next;
        if(p==NULL)
        {
            return p;
        }
        NodeKey* q=p->next;
        head->next=q;
        p->next=NULL;
        return p;
    }
    void input()
    {
        NodeKey *p=head->next;
        while(p)
        {
           p=p->next;
        }
    }
    void inputnum()
    {
        cout<<num<<endl;
    }
    void deleteSelf()
    {
       
        NodeKey *p=head;
        while(p!=NULL)
        {
            NodeKey *q=p;
            p=p->next;
            delete q;
            
        }
        delete []Key;
        delete []value;
        
    }
    
    ~listKey()
    {
        //deleteSelf();
       //delete head;
    }
};
#endif

