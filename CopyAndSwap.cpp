#include <iostream>
#include <string>
#include <initializer_list>
#include <algorithm> 

using namespace std;

struct Data {
    int i;
    long l;
    string s;
};

class DataManager {
public:
    explicit DataManager(const initializer_list<Data>& args) : size{args.size()}
    {
        p = new Data[size];
        auto idx = 0;
        for (const auto &arg : args) {
            p[idx] = Data{arg};
            idx++;
        }
    }
    
    ~DataManager() { if(p != nullptr) { delete[] p; p = nullptr; } }
    
    DataManager(const DataManager& src)
    {
        size = src.size;
        p = new Data[size];
        auto idx = 0;
        while (idx < size) {
            p[idx].i = src.p[idx].i;
            p[idx].l = src.p[idx].l;
            p[idx].s = src.p[idx].s;
            idx++;
        }
    }
    
    DataManager& operator=(const DataManager& src)
    {
        DataManager temp{src};
        swap(temp);
        return *this;    
    }
    
    void swap(DataManager& src) noexcept
    {
        std::swap(size, src.size);
        std::swap(p, src.p);
    }
    
    void print()
    {
        if (p != nullptr) {
            auto idx = 0;
            while (idx < size) {
                cout << p[idx].i << "|" << p[idx].l << "|" << p[idx].s;
                idx++;
            }
            cout << endl;
        } else {
            cout << "NULL" << endl;
        }
    }
private:
    Data* p{nullptr};
    unsigned long size;
};

int main() {
    DataManager d1{{10, 100001, "www"}, {20, 200001, "ttt"}};
    d1.print();
    DataManager d2{{30, 300001, "www"}, {40, 400001, "ttt"}, {50, 500001, "nnn"}};
    d2.print();
    
    d2 = d1;
    d1.print();
    d2.print();

    std::cout << "Hello world!" << std::endl;
    return 0;
}