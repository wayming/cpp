#include <string>
#include <iostream>
#include <memory>

using namespace std;
class Resource {
    public:
        Resource(string name) : oName{name} { cout << "Resource " << oName << " created" << endl; }
        ~Resource() { cout << "Resource " << oName << " destroyed" << endl; }
        void Ref(shared_ptr<Resource> other) { oResourceRef = other; }
    private:
        string oName;
        // shared_ptr<Resource> oResourceRef;
        weak_ptr<Resource> oResourceRef;
};


int main()
{
    auto res1 = make_shared<Resource>(string("resource 1"));
    auto res2 = make_shared<Resource>(string("resource 2"));
    
    res1->Ref(res2);
    res2->Ref(res1);
}