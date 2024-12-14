#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <memory>


template <typename T>
class TreeNode
{
    public:
        T element;
        std::unique_ptr<TreeNode<T>> left;
        std::unique_ptr<TreeNode<T>> right;

        TreeNode<T>(const T& e)
            :element{e}, left{nullptr}, right{nullptr} {}

        ~TreeNode() {}

};


template <typename T>
class BST
{
    public:
        std::unique_ptr<TreeNode<T>> root = nullptr;

        ~BST() {}

        bool insert(const T& key);
        bool search(const T& key);
        bool remove(const T& key);

    private:
        bool insert(std::unique_ptr<TreeNode<T>>& t, const T& key);
        bool search(std::unique_ptr<TreeNode<T>>& t, const T& key);
        bool remove(std::unique_ptr<TreeNode<T>>& t, const T& key);

        T find_rightmost_key(std::unique_ptr<TreeNode<T>>& t);
};

template <typename T>
bool BST<T>::insert(const T& key) {
    return insert(root, key);
}

template <typename T>
bool BST<T>::search(const T& key) {
    return search(root, key);
}

template <typename T>
bool BST<T>::remove(const T& key) {
    return remove(root, key);
}

template <typename T>
bool BST<T>::insert(std::unique_ptr<TreeNode<T>>& t, const T& key) {

    // TODO
    // if insertion fails (i.e. if the key already exists in tree), return false
    // otherwise, return true
    if(t == nullptr){
        t = std::make_unique<TreeNode<T>>(key);
        return true;
    }
    else if(t->element == key){
        return false;
    }
    else{
        if(key > t -> element){
            return insert(t -> right, key);
        }
        else{
            return insert(t -> left, key);
        }
    }
}

template <typename T>
bool BST<T>::search(std::unique_ptr<TreeNode<T>>& t, const T& key) {

    // TODO
    // if key exists in tree, return true
    // otherwise, return false
    if(t == nullptr){
        return false;
    }
    else if(t -> element == key){
        return true;
    }
    else{
        if(t -> element > key){
            return search(t -> left, key);
            
        }
        else{
            return search(t -> right, key);
            
        }
    }
}

template <typename T>
T BST<T>::find_rightmost_key(std::unique_ptr<TreeNode<T>>& t) {
    // TODO
    if(t -> right == nullptr){
        return t->element;
    }
    else{
        return find_rightmost_key(t -> right);
    }
}

template <typename T>
bool BST<T>::remove(std::unique_ptr<TreeNode<T>>& t, const T& key) {

    // TODO
    // if key does not exist in tree, return false
    // otherwise, return true

    if(t == nullptr){
        return false;
    }
    else{
        if(key > t -> element){
            return remove(t -> right, key);
        }
        else if(key < t->element){
            return remove(t -> left, key);
        }
        else{
            // TODO when key == t -> element
            if(t->right == nullptr && t->left == nullptr){
                t.reset();
            }
            else if(t->right != nullptr && t->left == nullptr){
                t = std::move(t->right);
            }
            else if(t->left != nullptr && t->right == nullptr){
                t = std::move(t->left);
            }
            else{
                t->element = find_rightmost_key(t->left);
                return remove(t-> left, t->element);
            }
        }
        return true;
    }

}
