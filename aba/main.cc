#include <thread>
#include <atomic>
#include <iostream>

using namespace std;

template <typename T>
class Node {
  public:
    Node(T t) : data(t), next(nullptr) {}
    T data;
    Node<T> *next;  
};

template <typename T>
class Link {
    public:
        Link() : root{nullptr} {}
        void push(T t) {
            Node<T> *newRoot = new Node<T>(t);
            while (true) {
                newRoot->next = root->load();
                if (root->compare_exchange_strong(newRoot->next, newRoot)) { break; }
            }
        }
        T pop() {
            while (true) {
                Node<T> *ret = root->load();
                Node<T> *newRoot = ret->next;

                if (root->compare_exchange_strong(ret, newRoot)) { T t = ret->data; delete ret; return t; }
            }
        }

    private:
        atomic< Node<T> > *root;
};

int main() {
    using IntLink = Link<int>;

    IntLink link;
    link.push(10);
    link.push(20);
    link.push(30);
    cout << link.pop() << "|" << link.pop() << "|" << link.pop() << endl;

}