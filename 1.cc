#include <iostream>
#include <memory>
#include <atomic>

using namespace std;

int main()
{
// std::atomic<std::shared_ptr<long>> instance;

// std::cout << "instance.is_lock_free = " << instance.is_lock_free()
// << "\n";
cout << __cpp_lib_syncbuf << endl;
return 0;
}
