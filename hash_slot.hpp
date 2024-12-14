#include <cstddef>

/* Fill in the TODO sections in the following code. */

template <typename K, typename V>
class HashSlot
{
public:
    HashSlot(): _empty(true), _removed(false) {
    }
    
    K get_key() const {
        return _key;
    }

    V get_value() const {
        return _value;
    }

    void set_key_value(K key, V value) {
        // TODO
        _key = key;
        _value = value;
        _removed = false;
        _empty = false;
    }

    bool is_empty() const {
        //TODO
        return _empty;
    }

    void set_empty() {
        //TODO
        _empty = true;       
    }

    bool is_removed() const {
        //TODO
        return _removed;
    }

    void set_removed() {
        //TODO
        _empty = true;
        _removed = true;
    }
    
private:
    // key-value pair
    K _key;
    V _value;
    bool _empty;
    bool _removed;

    // disallow copy and assignment
    HashSlot(const HashSlot &);
    HashSlot & operator=(const HashSlot &);
};
