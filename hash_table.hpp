#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <memory>

#define INITIAL_TABLE_SIZE 64

#include "hash_slot.hpp"
#include "hash_funcs.hpp"

template <typename K, typename V>
class HashTable {
public:
    HashTable(HashFunc *hash_func);
    ~HashTable();
    int get(const K &key, V &value);
    int put(const K &key, const V &value);
    int remove(const K &key);
    size_t get_table_size();
    size_t get_size();
    double get_load_factor();

protected:
    size_t table_size;
    
private:
    HashFunc *hash_func;
    size_t size;
    HashSlot<K, V> *table;


    // Should be overriden by the derived class
    virtual unsigned long get_next_pos(unsigned long pos,
                                       unsigned long step) = 0;
    unsigned long get_pos(const K key);
    void enlarge_table();
};

template <typename K, typename V>
HashTable<K, V>::HashTable(HashFunc *hash_func): table(), hash_func(hash_func),
                                                 size(0), table_size(INITIAL_TABLE_SIZE) {
    table = new HashSlot<K, V>[table_size];
}

template <typename K, typename V>
HashTable<K, V>::~HashTable() {
    delete[] table;
}

template <typename K, typename V>
void HashTable<K, V>::enlarge_table() {
    // TODO
    // std::cout << "Debug1\n";
    if(get_load_factor() > 0.5){
        // std::cout << "Debug1-1\n";
        // std::cout << table_size << '\n';
        table_size = table_size * 2;
        HashSlot<K,V> *new_table = new HashSlot<K, V>[table_size];
        for(int i = 0; i < table_size/2; i++){
            if(!table[i].is_empty()){
                unsigned long pos = get_pos(table[i].get_key());
                unsigned long initial = pos;
                int probe = 1;
                while(!new_table[pos].is_empty() || new_table[pos].is_removed()){
                    pos = get_next_pos(initial, probe);
                    probe++;
                }
                new_table[pos].set_key_value(table[i].get_key(),table[i].get_value());
            }
            if(table[i].is_removed()){
                unsigned long pos2 = get_pos(table[i].get_key());
                unsigned long initial2 = pos2;
                int probe = 1;
                while(!new_table[pos2].is_empty() || new_table[pos2].is_removed()){
                    pos2 = get_next_pos(initial2, probe);
                    probe++;
                }
                new_table[pos2].set_removed();
            }
        }
        delete[] table;
        table = new_table;
    }
}

template <typename K, typename V>
unsigned long HashTable<K, V>::get_pos(const K key) {
    // TODO
    unsigned long pos = hash_func->hash(key) % table_size;
    return pos;
}

template <typename K, typename V>
int HashTable<K, V>::get(const K &key, V &value) {
    // TODO
    // std::cout << "Debug2\n";
    int probe = 1;
    unsigned long pos = get_pos(key);
    unsigned long initial = pos;
    // if(table[pos].is_removed()){
    //     std::cout<< "Debug 2-1\n";
    // }
    while(!table[pos].is_empty() || table[pos].is_removed()){
        if(!table[pos].is_removed() && table[pos].get_key() == key){
            value = table[pos].get_value();
            // std::cout << "Getting key: " << key << "value: " << value << " at position " << pos << std::endl;
            return probe;
        }
        pos = get_next_pos(initial, probe);
        probe++;
    }

    return -1;
}

template <typename K, typename V>
int HashTable<K, V>::put(const K &key, const V &value) {
    // TODO
    // std::cout << "Debug3\n";
    int probe = 1;
    unsigned long pos = get_pos(key);
    unsigned long initial = pos;
    V value2;
    if(get(key,value2) != -1){
        return -1;
    }
    while(!table[pos].is_empty()){
        // std::cout << "Debug3-1\n";
        pos = get_next_pos(initial,probe);
        probe++;
    }
    table[pos].set_key_value(key,value);
    size++;
    enlarge_table();
    // std::cout << "Removing key: " << key << "value: " << value << " at position " << pos << std::endl;
    return probe;
}

template <typename K, typename V>
int HashTable<K, V>::remove(const K &key) {
    // TODO
    // std::cout << "Debug4\n";
    int probe = 1;
    unsigned long pos = get_pos(key);
    unsigned long initial = pos;
    V value;
    if(get(key, value) != -1){
        probe = get(key,value); 
        pos = get_next_pos(initial, probe-1);
        table[pos].set_removed();
        size--;
        return probe;
    }
    return -1;
}

template <typename K, typename V>
size_t HashTable<K, V>::get_table_size() {
    return table_size;
}

template <typename K, typename V>
size_t HashTable<K, V>::get_size() {
    return size;
}

template <typename K, typename V>
double HashTable<K, V>::get_load_factor() {
    return (double)size/table_size;
}


template <typename K, typename V>
class LinearProbeHashTable: public HashTable<K, V> {
public:
    LinearProbeHashTable(HashFunc *hash_func): HashTable<K, V>(hash_func) {
    }
    
private:
    virtual unsigned long get_next_pos(unsigned long pos, unsigned long step) {
        // TODO
        unsigned long p_pos = (pos + step) % this->table_size;
        return p_pos;
    }
};

template <typename K, typename V>
class QuadProbeHashTable: public HashTable<K, V> {
public:
    QuadProbeHashTable(HashFunc *hash_func): HashTable<K, V>(hash_func) {
    }
private:
    virtual unsigned long get_next_pos(unsigned long pos, unsigned long step) {
        // TODO
        unsigned long p_pos = (pos + (step + step * step)/2) % this->table_size;
        return p_pos;
    }
};

 