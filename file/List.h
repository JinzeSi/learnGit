#ifndef List_h
#define List_h
#include <iostream>
#include <fstream>
#include "ListKey.h"
#include "Indexes.h"
using namespace std;
using namespace std::__fs::filesystem;
namespace fs=std::__fs::filesystem;
struct Node{
       uint64_t num;
       string value;
       Node *next;
   };

class list {
   
private:
    Node *head;
    uint64_t num;
    uint64_t min;
    uint64_t max;
public:
    list()
    {
        head=new Node;
        head->next=NULL;
        num=0;
    }
    void clear()
    {
        Node *p=head;
        while(p)
        {
            Node *q=p;
            p=p->next;
            delete q;
        }
        
    }
    void push(Node * tmp)
    {
        Node*p=head;
        if(p->next==NULL)
        {
            p->next=tmp;
            num+=sizeof(tmp->num);
            num+=tmp->value.length();
            max=min=tmp->num;
            return ;
        }
        Node*q=p->next;
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
        num+=tmp->value.length();
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
    Node * pop()
    {
        Node* p=head->next;
        if(p==NULL)
        {
            return p;
        }
        Node* q=p->next;
        head->next=q;
        p->next=NULL;
        return p;
    }
    void input()
    {
        Node *p=head->next;
        while(p)
        {
            p=p->next;
        }
    }
    void inputnum()
    {
        cout<<num<<endl;
    }
    void sentToFile(string dir,string dirdraft,int level,int order,indexde *index,string route)
    {
        cout<<dir<<endl;
        int numKey=0;
        path str(dirdraft);
        ofstream outFile(str, ios::out | ios::binary);
        outFile.seekp(0,ios::beg);
        uint64_t numtmp=this->num+sizeof(this->num);
        outFile.write((char*)&numtmp,sizeof(numtmp));
        uint64_t num=sizeof(this->num);
        
        listKey* lk=new listKey;
       
        while(!this->isempty())
        {
            numKey++;
            Node *tmp=this->pop();
            outFile.write((char*)&tmp->num, sizeof(tmp->num));
            num+=sizeof(tmp->num);
            NodeKey *tmpKey=new NodeKey;
            tmpKey->num=tmp->num;
            tmpKey->value=num;
            tmpKey->next=NULL;
            lk->push(tmpKey);
            const char* s=tmp->value.c_str();
            outFile.write(s,strlen(s));
            num+=tmp->value.length();
            delete tmp;
        }
        lk->Key=new uint64_t[numKey];
        lk->value=new uint64_t[numKey];
        lk->numberofKey=numKey;
        int numKey2=0;
        if(!lk->isempty())
        {
            NodeKey *tmp=lk->head->next;
            while(tmp)
            {
                lk->Key[numKey2]=tmp->num;
                lk->value[numKey2]=tmp->value;
                outFile.write((char*)&tmp->num, sizeof(tmp->num));
                outFile.write((char*)&tmp->value, sizeof(tmp->value));
                tmp=tmp->next;
                numKey2++;
            }
            
        }
        
        lk->level=level;
        lk->order=order;
        lk->name=route;
        lk->num=numtmp;
        outFile.close();
        index->increase(lk);
        path nameFrom(dirdraft);
        path nameTo(dir);
        rename(nameFrom,nameTo);
        
        
    }
    void readfile(string dir,indexde *index,int level,int order)//归并排序
    {
        path str(dir);
        Node* qtmp=head;
        Node* qtmp2=head->next;
        ifstream inFile(str,ios::in|ios::binary);
        inFile.seekg(0,ios::beg);
        uint64_t num;
        inFile.read((char*)&num,sizeof(num));
        listKey *tmp=index->get(level, order);
        
        NodeKey *p=tmp->head->next;
        while(p->next!=NULL)
        {
            uint64_t numOne=p->value;
            uint64_t numTwo=p->next->value;
            uint64_t numtmp=numTwo-numOne-8;
            char * value=new char[numtmp+1];
            inFile.seekg(numOne,ios::beg);
            inFile.read(value,numtmp);
            value[numtmp]='\0';
            string val=value;
            Node *q=new Node;
            q->num=p->num;
            q->value=val;
            q->next=NULL;
           
            
               while(qtmp2&&qtmp2->num<q->num)
               {
                   qtmp=qtmp->next;
                   qtmp2=qtmp2->next;
               }
           
                if(qtmp2)
                {
                    if(qtmp2->num==q->num)
                    {
                        delete q;
                    }
                    else{
                        qtmp->next=q;
                    q->next=qtmp2;
                
                    if(qtmp==head)
                        min=q->num;
                        qtmp=q;
                    }
                }
                else{
                    qtmp->next=q;
                    max=q->num;
                    qtmp=q;
                }
            p=p->next;
        }
        uint64_t numOne=p->value;
        uint64_t numtmp=num-numOne;
        inFile.seekg(numOne,ios::beg);
        char * value=new char[numtmp+1];
        inFile.read(value,numtmp);
        value[numtmp]='\0';
        string val=value;
        Node *q=new Node;
        q->num=p->num;
        q->value=val;
        q->next=NULL;
        while(qtmp2&&qtmp2->num<q->num)
        {
            qtmp=qtmp->next;
            qtmp2=qtmp2->next;
        }
         if(qtmp2)
         {
             if(qtmp2->num==q->num)
            {
                p=p->next;
                delete q;
            }
             else{
             qtmp->next=q;
             q->next=qtmp2;
             if(qtmp==head)
                 min=q->num;
             }
         }
         else{
             qtmp->next=q;
             max=q->num;
         }
        replaceMax();
        inFile.close();
        return;
    }
    void CompactionSentToFile(string dir,string dirdraft,int level,int order,string route,indexde* index)
    {
        list *tmp=new list;
        tmp->head->next=head->next;
        
        tmp->min=head->next->num;
        int num=0;
        int numtwo=0;
        Node*p=head->next;
        while(true)
        {
            num+=sizeof(p->num);
            num+=p->value.length();
            numtwo+=sizeof(p->num);
            numtwo+=p->value.length();
            num+=16;
            if(num>=1024*1024*2)
                break;
            if(p->next==NULL)
            {
                break;
            }
            p=p->next;
        }
        head->next=p->next;
        if(!isempty())
        {
            min=head->next->num;
        }
        tmp->num=numtwo;
        tmp->max=p->num;
        p->next=NULL;
        tmp->replaceMax();
        tmp->sentToFile(dir,dirdraft, level,order-1,index, route);
        
    }
    uint64_t getMax()
    {
        return max;
    }
    uint64_t getMin()
    {
        return min;
    }
    void replaceMax()
    {
        min=head->next->num;
        Node *p=head->next;
        while(p->next)
            p=p->next;
        max=p->num;
    }
    ~list()
    {
       // clear();
       // delete head;
    }
};

#endif
