//
//  skiplist.h
//  redblacktree
//
//  Created by 斯金泽 on 2020/3/22.
//  Copyright © 2020 斯金泽. All rights reserved.
//

#ifndef skiplist_h
#define skiplist_h
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include "List.h"
using namespace std;
#define MAX_LEVEL 20 //最大层数

//节点
struct nodeStructure
{
    uint64_t key;
    std::string value;
    struct nodeStructure *forward[MAX_LEVEL];
};

//跳表
class skiplist
{
private:
    int level;
    nodeStructure *header;
    
public:
    //创建节点
    nodeStructure* createNode(int level,uint64_t key,std::string value)
    {
        nodeStructure *ns=new nodeStructure;
        ns->key=key;
        ns->value=value;
        for(int i=0;i<level;i++)
        ns->forward[i]=NULL;
        
        return ns;
    }
    
    //初始化跳表
    skiplist()
    {

        this->level=0;
        this->header=createNode(MAX_LEVEL-1,-1,"0");
        for(int i=0;i<MAX_LEVEL;i++)
        {
            this->header->forward[i]=NULL;
        }
    }
    ~skiplist()
    {
        //clear(this);
        //delete header;
    }
    
    //随机产生层数
    int randomLevel()
    {
        int k=1;
        while (rand()%2)
            k++;
        k=(k<MAX_LEVEL)?k:MAX_LEVEL;
        return k;
    }
    
    //插入节点
    void insert(skiplist *sl,uint64_t key,std::string value)
    {
        
        nodeStructure *update[MAX_LEVEL];
        nodeStructure *p, *q = NULL;
        p=sl->header;
        int k=sl->level;
        //从最高层往下查找需要插入的位置
        //填充update
        for(int i=k-1; i >= 0; i--){
            while((q=p->forward[i])&&(q->key<key))
            {
                p=q;
            }
            if (q&&q->key==key)
            {
                q->value=value;
            }
            update[i]=p;
        }
        //不能插入相同的key
        if(q&&q->key==key)
        {
            return ;
        }
        
        //产生一个随机层数K
        //新建一个待插入节点q
        //一层一层插入
        k=randomLevel();
        //更新跳表的level
        if(k>(sl->level))
        {
            for(int i=sl->level; i < k; i++){
                update[i] = sl->header;
            }
            sl->level=k;
        }
        
        q=createNode(k,key,value);
        q->value=value;
        //逐层更新节点的指针，和普通列表插入一样
        for(int i=0;i<k;i++)
        {
            q->forward[i]=update[i]->forward[i];
            update[i]->forward[i]=q;
        }
        return ;
    }
    
    //搜索指定key的value
    std::string search(skiplist *sl,uint64_t key)
    {
        nodeStructure *p,*q=NULL;
        p=sl->header;
        //从最高层开始搜
        int k=sl->level;
        for(int i=k-1; i >= 0; i--){
            while((q=p->forward[i])&&(q->key<=key))
            {
                if(q->key == key)
                {
                    return q->value;
                }
                p=q;
            }
        }
        return "";
    }
    
    //删除指定的key
    bool deleteSL(skiplist *sl,uint64_t key)
    {
        nodeStructure *update[MAX_LEVEL];
        nodeStructure *p,*q=NULL;
        p=sl->header;
        //从最高层开始搜
        int k=sl->level;
        for(int i=k-1; i >= 0; i--){
            while((q=p->forward[i])&&(q->key<key))
            {
                p=q;
            }
            update[i]=p;
        }
        if(q&&q->key==key)
        {
            //逐层删除，和普通列表删除一样
            for(int i=0; i<sl->level; i++){
                if(update[i]->forward[i]==q){
                    update[i]->forward[i]=q->forward[i];
                }
            }
            free(q);
            //如果删除的是最大层的节点，那么需要重新维护跳表的
            for(int i=sl->level - 1; i >= 0; i--){
                if(sl->header->forward[i]==NULL){
                    sl->level--;
                }
            }
            return true;
        }
        else
            return false;
    }
    void printSL(skiplist *sl)
    {
        //从最高层开始打印
        nodeStructure *p,*q=NULL;
        
        //从最高层开始搜
        int k=sl->level;
        for(int i=k-1; i >= 0; i--)
        {
            p=sl->header;
            q=p->forward[i];
            while(q)
            {
                cout<<p->value<<"  ";
                p=q;
                q=p->forward[i];
            }
            printf("\n");
        }
        printf("\n");
    }
    void sentToList(skiplist *sl ,list *li)
    {
        nodeStructure *p=NULL;
        p=sl->header->forward[0];
        
        while(p)
        {
            Node *tmp=new Node;
            tmp->num=p->key;
            tmp->value=p->value;
            tmp->next=NULL;
            li->push(tmp);
            p=p->forward[0];
        }
    }
    void clear(skiplist *sl)
    {
        nodeStructure *p,*q=NULL;
            p=sl->header;
            q=p->forward[0];
            while(q)
            {
                p=q;
                q=p->forward[0];
                delete p;
            }
        
        
        sl->level=0;
        sl->header=createNode(MAX_LEVEL-1,-1,"0");
        for(int i=0;i<MAX_LEVEL;i++)
        {
            sl->header->forward[i]=NULL;
        }
    }
};
#endif /* skiplist_h */
