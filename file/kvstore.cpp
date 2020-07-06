#include "kvstore.h"
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <string>
using namespace std::__fs::filesystem;
using namespace std;
namespace fs=std::__fs::filesystem;

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir)
{
    rootfile=dir;
    sl=new skiplist;
    li=new list;
    index=new indexde;
    path root(dir);
    if(!exists(root))
    {
        fs::create_directory(dir);
    }
    string dirtmp=dir;
    num=0;
    maxlevel=0;
    levelOne=0;
    readForready(dir,1);
    path str(dir+"/1");
    if(!exists(str))
    {
        fs::create_directories(dir+"/1");
    }
    path strtemp(dir+"/0");
        if(!exists(strtemp))
        {
            fs::create_directories(dir+"/0");
           
        }
  
}
void KVStore::readForready(string dir,int num)
{
    if(num==1)
    {
        path str(dir+"/1");
        if(!exists(str))
        {
            return;
        }
        string dirtemp=dir+"/1";
        string name="a.dat\0";
        path dirA(dirtemp+"/"+name);
        if(exists(dirA))
        {
            levelOne=1;
            readForreadyTwo(dirtemp,name,1,0);
        }
        name="b.dat\0";
        path dirB(dirtemp+"/"+name);
        if(exists(dirB))
        {
            levelOne=2;
            readForreadyTwo(dirtemp,name,1,1);
        }
    }
    else{
        string dirtmp="";
        if(num<10)
        {
            char a='0'+num;
            dirtmp=dir+'/'+a;
        }
        else
        {
            char a='0'+num/10;
            char b='0'+num%10;
            dirtmp=dir+'/'+a+b;
        }
        path str(dirtmp);
        if(!exists(str))
        {
            return;
        }
        maxlevel=num;
        for(int i=0;i<3*num-1;i++)
        {
            string name="";
            char a='a'+i/26;
            char b='a'+i%26;
            name=name+a+b+".dat";
            readForreadyTwo(dirtmp,name,num,i);
        }
    }
    readForready(dir, num+1);
}
void KVStore::readForreadyTwo(string dir,string name,int level,int order)
{
    string dirtemp=dir+'/'+name;
    ifstream inFile(dirtemp,ios::in|ios::binary);
    if(!inFile)
    return;
    
    listKey * tmp=new listKey;
    tmp->level=level;
    tmp->order=order;
    tmp->name=name;
    NodeKey *p=tmp->head;
    uint64_t l = inFile.tellg();
    inFile.seekg(0, ios::end);
    uint64_t m = inFile.tellg();
    uint64_t fileLength=m-l;
    inFile.seekg(0,ios::beg);
    uint64_t numtemp;
    inFile.read((char*)&numtemp,sizeof(numtemp));
    for(uint64_t i=numtemp;i<fileLength;i+=16)
    {
        uint64_t key;
        uint64_t value;
        inFile.seekg(i,ios::beg);
        inFile.read((char*)&key,sizeof(key));
        inFile.read((char*)&value,sizeof(value));
        NodeKey *q=new NodeKey;
        q->next=NULL;
        q->num=key;
        q->value=value;
        p->next=q;
        p=p->next;
        tmp->numberofKey++;
        if(i==numtemp)
            tmp->min=key;
        tmp->max=key;
    }
    p=tmp->head->next;
    tmp->value=new uint64_t[tmp->numberofKey];
     tmp->Key=new uint64_t[tmp->numberofKey];
    int sign=0;
    while(p)
    {
        tmp->value[sign]=p->value;
        tmp->Key[sign]=p->num;
        sign++;
        p=p->next;
    }
    tmp->num=numtemp;
    index->increase(tmp);
    inFile.close();
}
KVStore::~KVStore()
{
    
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string s)
{

    string sign=sl->search(sl, key);
    string str="";
    if(sign!="")
    {
        num-=8;
        num-=sign.length();
        num-=16;
    }
    if(s=="")
        str="##NULL##\0";
    else str=s;
    
    sl->insert(this->sl,key,str);
    num+=sizeof(key);
    num+=str.length();
    num+=16;
   if(num>=1024*1024*2)
    {
        if(levelOne<2)
        {
            li =new list;
            sl->sentToList(sl, li);
            char a='1';
            char b='a'+levelOne;
            string dir=rootfile+"/";
            string dirdraft=dir+"0/draft.dat";
            dir=dir+a+'/'+b+".dat";
            string route="";
            string dat=".dat";
            route =b+dat;
            li->sentToFile(dir,dirdraft,1,levelOne,index,route);
            num=0;
            sl->clear(sl);
            levelOne++;
            if(maxlevel==0)
                maxlevel++;
        
        }
        else{
            levelOne=0;
            string dir=rootfile+"/";
            compaction(dir);
            
        }
        
    }
   
    
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
    std::string res=sl->search(sl,key);
    
    if(res=="##delete##\0"||res=="##NULL##")
    {
        return "";
    }
    if(res=="")
    {
        res=index->search(key,rootfile);
    }
    if(res=="##delete##"||res=="##NULL##")
        return "";
    
	return res;
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
    string res=sl->search(sl,key);
    if(res=="##delete##\0")
    {
        return false;
    }
    if(res=="")
    {
        res=index->search(key,rootfile);
        if(res==""||res=="##delete##\0")
           return false;
    }
    /*std::string res=get(key);
   
    if(res!=""&&res!="##delete##\0")
    {
        put(key, "##delete##\0");
       
        return true;
    }*/
    
    put(key,"##delete##\0");
   
    return true;
}
void KVStore::compaction(std::string dir)
{
    li=new list;
    string dirone=dir+"1/b.dat";
    string dirtwo=dir+"1/a.dat";
    
    sl->sentToList(sl, li);
    sl->clear(sl);
    num=0;
    li->readfile(dirone,index,1,2);
    li->readfile(dirtwo,index,1,1);
    li->replaceMax();
    index->del(1, 1);
    index->del(1, 2);
    
    Docompaction(dir,2);
    remove(dirone);
    remove(dirtwo);
}
void KVStore::Docompaction(std::string dir,int level)
{
    string dirtemp="";
    
        if(level>9)
        {
            char a='0'+(level/10);
            char b='0'+(level%10);
            dirtemp=dir+a+b;
            
        }
        else{char a='0'+level;
             dirtemp=dir+a;
        }
    if(level>maxlevel)
    {
        maxlevel++;
        fs::create_directories(dirtemp);
    }
    uint64_t Maxtmp=li->getMax();
    uint64_t Mintmp=li->getMin();
    
    for(int i=0;i<3*level-1;i++)
    {
       uint64_t mintemp= index->getMin(level,i+1);
       uint64_t maxtemp=index->getMax(level, i+1);
        if(mintemp==0&&maxtemp==0)
        {
            continue;
        }
       else if((mintemp>=Mintmp&&mintemp<=Maxtmp)||(maxtemp>=Mintmp&&maxtemp<=Maxtmp)||((mintemp<=Mintmp)&&(maxtemp>=Maxtmp)))
       {
           
           string name=index->getName(level,i+1);
           char c='/';
           string dirtmp=dirtemp+c+name;
           li->readfile(dirtmp, index, level, i+1);
           index->del(level, i+1);
         
       }
        
    }
    for(int i=0;i<3*level-1;i++)
    {
       uint64_t mintemp= index->getMin(level,i+1);
       uint64_t maxtemp=index->getMax(level, i+1);
        if(mintemp==0&&maxtemp==0)
        {
            char b='a'+i%26;
            char d='a'+i/26;
            char c='/';
            string behind=".dat";
            string dirtmp=dirtemp+c+d+b+behind;
            string route="";
            route=route+d+b+behind;
            string dirdraft=dir+"0/draft.dat";
            li->CompactionSentToFile(dirtmp,dirdraft,level,i+1,route,index);
        }
       else if((mintemp>=Mintmp&&mintemp<=Maxtmp)||(maxtemp>=Mintmp&&maxtemp<=Maxtmp))
       {
           string name=index->getName(level,i+1);
           char c='/';
           string dirtmp=dirtemp+c+name;
           string dirdraft=dir+"0/draft.dat";
           li->CompactionSentToFile(dirtmp,dirdraft,level,i+1,name,index);
       }
        if(li->isempty())
        {
            for(int j=i+1;j<3*level-1;j++)
            {
                uint64_t delmintemp= index->getMin(level,j+1);
                uint64_t delmaxtemp=index->getMax(level, j+1);
                if(delmaxtemp==0&&delmintemp==0)
                {
                char b='a'+j%26;
                char d='a'+j/26;
                char c='/';
                string behind=".dat";
                string dirtmp=dirtemp+c+d+b+behind;
                path delName(dirtmp);
                remove(delName);
                }
            }
            return;
        }
    }
    li->replaceMax();
   
    Docompaction(dir,level+1);
    
    
    
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
    num=0;
    maxlevel=0;
    levelOne=0;
    index->reset();
    index=new indexde;
    sl->clear(sl);
    sl=new skiplist;
    li->clear();
    li=new list;
    remove_all(rootfile);
    fs::create_directory(rootfile);
    string temp=rootfile+"/0";
    fs::create_directory(temp);
    temp=rootfile+"/1";
    fs::create_directory(temp);
    
    
    
}
