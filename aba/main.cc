#include <string.h>

#include <thread>
#include <atomic>
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

atomic_flag ready = ATOMIC_FLAG_INIT;

template <typename T>
class Node {
  public:
    Node(T t) : data(t), next(nullptr) {}
    ~Node() = default;
    Node(const Node&) = default;
    Node& operator=(const Node&) = default;
    T data;
    Node<T> *next;  
};

template <typename T>
class Link {
    public:
        Link() : root{nullptr} {}
        void push(Node<T> *newRoot) {
            while (true) {
                newRoot->next = root.load();
                if (root.compare_exchange_strong(newRoot->next, newRoot)) { break; }
            }
        }

        Node<T> *pop() {
            while (true) {
                Node<T> *ret = root.load();
                Node<T> *newRoot = ret->next;

                if (root.compare_exchange_strong(ret, newRoot)) { return ret; }
            }
        }

        Node<T> *popDelay() {
            while (true) {
                Node<T> *ret = root.load();
                Node<T> *newRoot = ret->next;

                // Produce ABA condition
                while (ready.test_and_set()) { this_thread::yield(); }

                if (root.compare_exchange_strong(ret, newRoot)) { return ret; }
            }
        }

        T peek() {
            cout << "root " << root.load() << endl;;
            return root.load()->data;
        }
    private:
        atomic<Node<T>*> root;
};

using IntLink = Link<int>;
using IntNode = Node<int>;
int main() {

    IntLink link;
    link.push(new IntNode(10));
    link.push(new IntNode(20));
    link.push(new IntNode(30));


    ready.test_and_set();

    vector<thread> threads;

    threads.emplace_back([](IntLink &l) {
        IntNode *n1 = l.popDelay();
        cout << "T1 node1 " << n1 << endl;
    }, ref(link));

    threads.emplace_back([](IntLink &l) {
        IntNode *n1 = l.pop();
        IntNode *n2 = l.pop();
        cout << "T2 node1 " << n1 << endl;
        cout << "T2 node2 " << n2 << endl;
        
        l.push(move(n1));
        delete n2;
        memset(n2, 0, sizeof(IntNode));
        
        ready.clear();
    }, ref(link));

    for (auto& thread: threads) {
        thread.join();
    }

    // Wild Pointer
    cout << "Peek " << link.peek() << endl;;

}