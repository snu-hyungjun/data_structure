#ifndef __FHEAP_H_
#define __FHEAP_H_

#include <iostream>
#include <initializer_list>
#include <optional>
#include <vector>
#include <cmath>
#include <memory>
#include <queue>

template <typename T>
class PriorityQueue {
    public:
        virtual void insert(const T& item) = 0;
        virtual std::optional<T> extract_min() = 0;
        virtual bool is_empty() const = 0;
};

template <typename T>
class FibonacciNode {
    public:
        // constructors
        FibonacciNode()
            :key(std::nullopt), degree(0), child(nullptr), right(nullptr) {}
        FibonacciNode(const T& item)
            :key(item), degree(0), child(nullptr), right(nullptr) {}

        // destructor
        ~FibonacciNode() = default;

        T key;
        size_t degree;

        std::shared_ptr<FibonacciNode<T>> right;
        std::shared_ptr<FibonacciNode<T>> child;
        // NOTE: If you set left/parent pointer to shared_ptr, circular reference may cause!
        // So, left/parent pointer should be set to weak_ptr.
        std::weak_ptr<FibonacciNode<T>> left;
        std::weak_ptr<FibonacciNode<T>> parent;
};

template <typename T>
class FibonacciHeap : public PriorityQueue<T> {
    public:
        // Default Constructor
        FibonacciHeap()
            : min_node(nullptr), size_(0) {}

        // Constructor with Value
        FibonacciHeap(const T& item)
            : min_node(nullptr), size_(0) { insert(item); }

        // Disable copy constructor.
        FibonacciHeap(const FibonacciHeap<T> &);

        // Constructor with initializer_list
        // ex) FibonacciHeap<int> fheap = {1, 2, 3, 4};
        FibonacciHeap(std::initializer_list<T> list): min_node(nullptr), size_(0) {
            for(const T& item : list) {
                insert(item);
            }
        }

        // Destructor
        ~FibonacciHeap();

        // Insert item into the heap.
        void insert(const T& item) override;

        // Return raw pointer of the min node.
        // If the heap is empty, return nullptr.
        FibonacciNode<T>* get_min_node() { return min_node.get(); }

        // Return minimum value of the min node.
        // If the heap is empty, return std::nullopt.
        std::optional<T> get_min() const;

        // 1. Return minimum value of the min node
        // 2. Remove the node which contains minimum value from the heap.
        // If the heap is empty, return std::nullopt;
        std::optional<T> extract_min() override;

        // Return true if the heap is empty, false if not.
        bool is_empty() const override { return !size_; }

        // Return the number of nodes the heap contains.
        size_t size() const { return size_; }


    private:
        // Points to the node which has minimum value.
        std::shared_ptr<FibonacciNode<T>> min_node;

        // Value that represents how many nodes the heap contains.
        size_t size_;

        void insert(std::shared_ptr<FibonacciNode<T>>& node);

        // After extract, clean up the heap.
        void consolidate();

        // Combine two nodes.
        void merge(std::shared_ptr<FibonacciNode<T>>& x, std::shared_ptr<FibonacciNode<T>>& y);
};


template <typename T>
FibonacciHeap<T>::~FibonacciHeap() {
    //TODO
    if(min_node){
        std::shared_ptr<FibonacciNode<T>> current = min_node;
        do{
            std::shared_ptr<FibonacciNode<T>> next = current->right;
            if(current->child){
                current->child->parent.reset();
            }
            current->parent.reset();
            current->left.reset();
            current->right.reset();
            current->child.reset();
            current = next;
        } while(current && current != min_node);
    }
    min_node.reset();
    size_ = 0;
}

template <typename T>
std::optional<T> FibonacciHeap<T>::get_min() const {
    //TODO
    if(min_node){
        return min_node->key;
    }
    return std::nullopt;
}

template <typename T>
void FibonacciHeap<T>::insert(const T& item) {
    // TODO
    std::shared_ptr<FibonacciNode<T>> node = std::make_shared<FibonacciNode<T>>(item);
    insert(node);
}

template <typename T>
void FibonacciHeap<T>::insert(std::shared_ptr<FibonacciNode<T>>& node) {
    if (!min_node) {
        min_node = node;
        node->left = node;
        node->right = node;
    }
    else {
        node->right = min_node->right;       
        node->left = min_node;               
        min_node->right->left = node;        
        min_node->right = node;               

        if (node->key < min_node->key) {
            min_node = node;
        }
    }
    size_++;
}

template <typename T>
std::optional<T> FibonacciHeap<T>::extract_min() {
    // TODO
    // std::cout<<"Debug1\n";
    if(!min_node){
        return std::nullopt;
    }
    else{
        // std::cout<<"Debug2\n";
        T return_key = min_node -> key;
        if(!(min_node -> child)){
            if(min_node -> right == min_node){
                min_node.reset();
                size_--;
                return return_key;
            }
            else{
                min_node -> left.lock() -> right = min_node -> right;
                min_node -> right -> left = min_node -> left.lock(); 
                min_node = min_node -> left.lock();
            }
        }
        else{
            // std::cout<<"Debug3\n";
            // std::cout << "min_node : " << min_node -> key << std::endl;
            // std::cout << "min_node child : " << min_node -> child -> key << std::endl;
            auto node0 = min_node -> child;
            auto loop_end3 = node0;
            while(node0 -> right != loop_end3){
                node0 -> parent.reset();
                node0 = node0->right;
                // std::cout << "up child key : " << node0 -> key << std::endl;
            }
            node0 -> parent.reset();
            if(min_node -> right == min_node){
                min_node = min_node -> child;
            }
            else{
                min_node -> child-> left.lock() -> right = min_node -> right;
                min_node -> right -> left = min_node -> child -> left.lock();
                min_node -> left.lock() -> right = min_node -> child;
                min_node -> child -> left = min_node -> left.lock();
                min_node = min_node -> child;
            }
            // min_node -> right.reset();
        }
        // std::cout<<"Debug4\n";
        // T min_key = min_node -> key;
        // auto node1 = min_node;
        // std::shared_ptr<FibonacciNode<T>> start = min_node;
        // node1 = min_node->right;
        // while(node1 != start){
        //     if(min_key > node1->key){
        //         min_key = node1->key;
        //         min_node = node1;
        //         // std::cout << "min_node key : " << min_node->key << "\n";
        //     }
        //     node1 = node1->right;
        // }

        consolidate();
        // std::cout << "min key : " << min_node -> key << "\n";     // 여기서 출력이 밑에 2개가 다르게 발생
        size_--;
        // std::cout<<"Debug_end\n";
        return return_key;
    }
    
}

template <typename T>
void FibonacciHeap<T>::consolidate(){
    //TODO
    if(!min_node){
        return;
    }
    int D = static_cast<int>(std::floor(std::log2(static_cast<double>(size_))) + 1);
    std::vector<std::shared_ptr<FibonacciNode<T>>> A(D+1, nullptr);
    std::vector<std::shared_ptr<FibonacciNode<T>>> roots;
    std::shared_ptr<FibonacciNode<T>> current = min_node;
    do{
        roots.push_back(current);
        current = current->right;
    } while(current != min_node);
    for(auto& w : roots){
        std::shared_ptr<FibonacciNode<T>> x = w;
        int d = static_cast<int>(x->degree);
        while(d < A.size() && A[d] != nullptr){
            std::shared_ptr<FibonacciNode<T>> y = A[d];
            if(x->key > y->key){
                std::swap(x, y);
            }
            merge(x, y);
            A[d] = nullptr;
            d++;
        }
        if(d >= A.size()){
            A.resize(d+1, nullptr);
        }
        A[d] = x;
    }
    min_node = nullptr;
    for(auto& node : A){
        if(node){
            if(!min_node){
                min_node = node;
                node->left = node;
                node->right = node;
            }
            else{
                node->left = min_node->left;
                node->right = min_node;
                min_node->left.lock()->right = node;
                min_node->left = node;
                if(node->key < min_node->key){
                    min_node = node;
                }
            }
        }
    }
}

template <typename T>
void FibonacciHeap<T>::merge(std::shared_ptr<FibonacciNode<T>>& x, std::shared_ptr<FibonacciNode<T>>& y) {
    //TODO
    if (x->key > y->key) {
        std::shared_ptr<FibonacciNode<T>> y2 = y;

        x->left.lock()->right = x->right;
        x->right->left = x->left.lock();

        x->parent = y;
        if (y->child) {
            x->right = y->child;
            y->child->left.lock()->right = x;
            x->left = y->child->left.lock();
            y->child->left = x;
        } else {
            x->left = x;
            x->right = x;
        }
        y->child = x;
        y->degree = y->degree + 1;
        // std::cout << "y key : " << y->key << "\n";
        // std::cout << "y child key: " << y->child->key << "\n";
    } else {
        y->left.lock()->right = y->right;
        y->right->left = y->left.lock();
        y->parent = x;

        if (x->child) {
            y->right = x->child;
            y->left = x->child->left.lock();
            x->child->left.lock()->right = y;
            x->child->left = y;
        } else {
            y->left = y;
            y->right = y;
        }
        x->child = y;
        x->degree = x->degree + 1;
        // std::cout << "x key : " << x->key << "\n";
        // std::cout << "x child key: " << x->child->key << "\n";
    }
}


#endif // __FHEAP_H_
