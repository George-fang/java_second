#include <iostream>
#include <cstdio>
#include <memory>
#include <algorithm>
#include <map>
#include <string.h>
#include <vector>
#include <string>

using namespace std;

class IHolder{
public:
    unsigned long long size{};
    string name;
    vector<string> members;
    vector<unsigned long long> offsets;
    void* obj{};
    virtual ~IHolder() = default;
    template<class N>
    void add_member(unsigned long long offset){
        this->members.emplace_back(typeid(N).name());
        this->offsets.push_back(offset);
    }
    void* get_obj() const {
        return (void*)this->obj;
    }
};


template< class T >
class Holder : public IHolder
{
private:
public:
    // 构造函数，需要给出一个obj
    explicit Holder(T* obj){
        this->obj = obj;
    }
};



class IOC {

public:

    map<string, IHolder> myHolder;
    template<class T>
    void add_class(){
        string name = typeid(T).name();
        if(name[0] == 'P'){ // 如果这是一个指针
            if (myHolder.find(name.substr(1, name.length() - 1)) == myHolder.end()){ // 如果没有注册实例化的对象，那么报错
                cout << "you should add_class first but not add_class_pointer first!" << endl;
                exit(0);
            } else {
                T* T_obj_address = (T*) (unsigned long long)myHolder[name.substr(1, name.length() - 1)].get_obj();
                myHolder[name] = Holder<T>(T_obj_address);
            }
        } else {
            T* new_block =  (T*)(unsigned long long)malloc(sizeof(T));
            memset(new_block, 0, sizeof(T));
            myHolder[name] = Holder<T>(new_block);
        }
    }

    // 添加成员的方法，只需要给类的对象添加成员即可
    template<class T, class N>
    void add_member(unsigned long long offset){

        if(myHolder.find(typeid(T).name()) == myHolder.end()){
            this->add_class<N>();
        }
        myHolder[typeid(T).name()].add_member<N>(offset);
    }

    template<class T>
    T* get_class(){
        if(myHolder.find(typeid(T).name()) == myHolder.end()){
            return nullptr;
        } else {
            return (T*) this->myHolder[typeid(T).name()].obj;
        }
    }

    void build_all(){
        int add_time = 0;
        for(auto iter: this->myHolder){
            if(iter.first[0] == 'P'){ // 如果是指针，那么就直接跳过，因为其地址已经指向我们的单例对象了
                continue;
            }
            // 现在处理的是对象的情况，我们需要将相应偏移量的子对象指向固定的对象
            int member_count = iter.second.members.size();
            // 遍历每一个孩子对象
            for(int cur_member_id = 0; cur_member_id < member_count; ++ cur_member_id){
                string cur_member_name = iter.second.members[cur_member_id];
                if(cur_member_name[0] != 'P') continue;

                unsigned long long offset = iter.second.offsets[cur_member_id];
                unsigned long long my_obj_pointer = (unsigned long long )this->myHolder[cur_member_name.substr(1, cur_member_name.length() - 1)].obj;
                cout << (unsigned long long *)((unsigned long long)iter.second.obj + offset) << endl;
                *(unsigned long long *)((unsigned long long)iter.second.obj + offset) = my_obj_pointer;
            }
        }
    }

};
