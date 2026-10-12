#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;
class Objectpool{
    int n;//initial size
    int active;
    vector<int *> pool;
    queue<int> free_slots;//index of free slots
    unordered_map<int *,int> index;
    vector<bool> occupied;
public:
    Objectpool(int n){
        this->n=n;
        this->active=0;
        pool.resize(n);
        occupied.resize(n);
        for(int i=0;i<n;i++){
            pool[i]=new int(0);
            free_slots.push(i);
            index[pool[i]]=i;
        }
    }
    int* aquire(){
        if(free_slots.empty()){
            pool.push_back(nullptr);
            free_slots.push(this->n);
            pool.back()=new int(0);
            index[pool.back()]=this->n;
            occupied.push_back(false);
            (this->n)++;
        }
        int * ptr=pool[free_slots.front()];
        occupied[free_slots.front()]=true;
        free_slots.pop();
        this->active++;
        return ptr;
    }
    void release(int *ptr){
        int id;
        if(index.find(ptr)!=index.end()) id=index[ptr];
        else{
            cout<<"Bruhh give valid ptr !!\n";
            return;
        }
        if(occupied[id]){
            free_slots.push(id);
            this->active--;
        }
        else{
            cout<<"Bruhh why releasing twice !!\n";
            return;
        }
        occupied[id]=false;
        
        return;
    }
    ~Objectpool(){
        for(int *i:pool)
            delete i;
    }
    Objectpool(const Objectpool&) = delete;//disable copy construction obj b=a
    Objectpool& operator=(const Objectpool&) = delete;//disable normal copy b=a
};

int main() {
    Objectpool pool(2);

    int* a = pool.aquire();
    int* b = pool.aquire();

    *a = 10;
    *b = 20;

    cout << *a << " " << *b << "\n";  // 10 20

    pool.release(a);

    int* c = pool.aquire();

    cout << (a == c) << "\n";  // 1: same slot reused
    cout << *c << "\n";       // 10: value is retained

    pool.release(b);
    pool.release(c);

    return 0;
}
/*
    Limitations:
    1. Raw pointers require manual memory management.
    2. Multiple containers must remain synchronized.
    3. Allocation failures can leave containers inconsistent.
    4. Copying is disabled to prevent double deletion.
    5. This implementation supports only int, not generic types.
*/