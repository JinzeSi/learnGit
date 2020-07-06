#pragma once
#include "kvstore_api.h"
#include "skiplist.h"
#include "Indexes.h"

class KVStore : public KVStoreAPI {
	// You can add your implementation here
private:
    skiplist *sl;
    uint64_t num;
    list *li;
    indexde *index;
    int maxlevel;
    int levelOne;
   // string dirtmp;
    string rootfile;
    
       
public:
	KVStore(const std::string &dir);

	~KVStore();

	void put(uint64_t key, const std::string s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;
    void compaction(std::string dir);
    void Docompaction(std::string dir,int level);
    void readForready(std::string,int num);
    void readForreadyTwo(std::string,std::string,int ,int);

};
