#ifndef _DEQUE_H
#define _DEQUE_H

#include <string>
#include <iostream>
#include <type_traits>
#include <optional>
#include <iostream>
#include <memory>
#include <cassert>

/* NOTE: Deque, ArrayDeque, ListDeque Declaration modification is not allowed.
 * Fill in the TODO sections in the following code. */
template <typename T>
class Deque {
public:
    virtual ~Deque() = default;

    /* NOTE: We won't implement push functions that take rvalue references. */
    virtual void push_front(const T&) = 0;
    virtual void push_back(const T&) = 0;

    /* NOTE: Unlike STL implementations which have separate `front` and
       pop_front` functions, we have one unified method for removing an elem. */
    virtual std::optional<T> remove_front() = 0;
    virtual std::optional<T> remove_back() = 0;

    virtual bool empty() = 0;
    virtual size_t size() = 0;

    virtual T& operator[](size_t) = 0;
};

template <typename T>
class ArrayDeque : public Deque<T> {
public:
    ArrayDeque();
    ~ArrayDeque() = default;

    void push_front(const T&) override;
    void push_back(const T&) override;

    std::optional<T> remove_front() override;
    std::optional<T> remove_back() override;

    bool empty() override;
    size_t size() override;
    size_t capacity();

    T& operator[](size_t) override;

private:
    std::unique_ptr<T[]> arr;
    size_t front;
    size_t back;
    size_t size_;
    size_t capacity_;

    void resize();
};

template <typename T>
ArrayDeque<T>::ArrayDeque() :
    front{63 /* You can change this */},
    back{0 /* You can change this */},
    size_{0}, capacity_{64} {
    arr = std::make_unique<T[]>(capacity_);
}




template <typename T>
void ArrayDeque<T>::push_front(const T& item) {
    // TODO
    // front는 숫자한칸 앞 back에는 숫자가 있는 마지막 <<< front에 숫자가 없다 < 이걸 프로그래밍 가정으로
    if(size() == capacity()){
        resize();
    }

    arr[front] = item;
    front = (front - 1) % capacity_;
    size_++;

}

template <typename T>
void ArrayDeque<T>::push_back(const T& item) {
    // TODO
    if(size() == capacity()){
        resize();
    }
    arr[back] = item;
    back = (back + 1) % capacity_;
    size_++;

}


template <typename T>
std::optional<T> ArrayDeque<T>::remove_front() {
    //TODO
    if (empty()) {
        return std::nullopt;
    } else {
        front = (front + 1) % capacity_;
        size_--;
        T value = arr[front];
        return value;
    }
}

template <typename T>
std::optional<T> ArrayDeque<T>::remove_back() {
    // TODO
    if (empty()) {
        return std::nullopt;
    } else {
        back = (back - 1) % capacity_;
        T value = arr[back];
        size_--;
        return value;
    }
}

template <typename T>
void ArrayDeque<T>::resize() {
    // TODO
    size_t new_capacity_ = 2 * capacity_;
    std::unique_ptr<T[]> new_arr = std::make_unique<T[]>(new_capacity_);

    for(size_t i = 0; i < capacity_; i++){
        new_arr[i] = arr[((front + i + 1) % capacity_)];
    }

    arr.reset(new_arr.release());
    front = new_capacity_ - 1;
    back = capacity_;
    capacity_ = new_capacity_;
}

template <typename T>
bool ArrayDeque<T>::empty() {
    // TODO
    if(size() == 0){
        return true;
    }
    return false;
}

template <typename T>
size_t ArrayDeque<T>::size() {
    // TODO
    return size_;
}

template <typename T>
size_t ArrayDeque<T>::capacity() {
    // TODO
    return capacity_;
}

template <typename T>
T& ArrayDeque<T>::operator[](size_t idx) {
    size_t x = (front + idx + 1) % capacity_;
    return arr[x];
}

template<typename T>
struct ListNode {
    std::optional<T> value;
    ListNode* prev;
    ListNode* next;

    ListNode() : value(std::nullopt), prev(this), next(this) {}
    ListNode(const T& t) : value(t), prev(this), next(this) {}

    ListNode(const ListNode&) = delete;
};

template<typename T>
class ListDeque : public Deque<T> {
public:
    ListDeque();
    ~ListDeque();

    void push_front(const T&) override;
    void push_back(const T&) override;

    std::optional<T> remove_front() override;
    std::optional<T> remove_back() override;

    bool empty() override;
    size_t size() override;

    T& operator[](size_t) override;

    size_t size_ = 0;
    ListNode<T>* sentinel = nullptr;
};

// Constructor
template<typename T>
ListDeque<T>::ListDeque() : sentinel(new ListNode<T>{}), size_(0) {}

// Destructor
template<typename T>
ListDeque<T>::~ListDeque() {
    // Delete all nodes except the sentinel
    ListNode<T>* current = sentinel->next;
    while (current != sentinel) {
        ListNode<T>* to_delete = current;
        current = current->next;
        delete to_delete;
    }
    delete sentinel;  // Delete the sentinel node
}


template<typename T>
void ListDeque<T>::push_front(const T& t) {
    ListNode<T>* new_node = new ListNode<T>(t);
    new_node->next = sentinel->next;
    new_node->prev = sentinel;
    sentinel->next->prev = new_node;
    sentinel->next = new_node;
    size_++;
}


template<typename T>
void ListDeque<T>::push_back(const T& t) {
    ListNode<T>* new_node = new ListNode<T>(t);
    new_node->prev = sentinel->prev;
    new_node->next = sentinel;
    sentinel->prev->next = new_node;
    sentinel->prev = new_node;
    size_++;
}


template<typename T>
std::optional<T> ListDeque<T>::remove_front() {
    if (empty()) {
        return std::nullopt;
    }
    ListNode<T>* to_delete = sentinel->next;
    T value = to_delete->value.value();
    sentinel->next = to_delete->next;
    to_delete->next->prev = sentinel;
    delete to_delete;
    size_--;
    return value;
}


template<typename T>
std::optional<T> ListDeque<T>::remove_back() {
    if (empty()) {
        return std::nullopt;
    }
    ListNode<T>* to_delete = sentinel->prev;
    T value = to_delete->value.value();
    sentinel->prev = to_delete->prev;
    to_delete->prev->next = sentinel;
    delete to_delete;
    size_--;
    return value;
}


template<typename T>
bool ListDeque<T>::empty() {
    return size_ == 0;
}

template<typename T>
size_t ListDeque<T>::size() {
    return size_;
}


template<typename T>
T& ListDeque<T>::operator[](size_t idx) {
    ListNode<T>* current = sentinel->next;
    for (size_t i = 0; i < idx; ++i) {
        current = current->next;
    }
    return current->value.value();
}


template<typename T>
std::ostream& operator<<(std::ostream& os, const ListNode<T>& n) {
    if (n.value)
        os << n.value.value();

    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const ListDeque<T>& l) {
    auto np = l.sentinel->next;
    while (np != l.sentinel) {
        os << *np << ' ';
        np = np->next;
    }

    return os;
}

#endif // _DEQUE_H
