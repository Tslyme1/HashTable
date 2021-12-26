#include "pch.h"
#include "HashTable.h"

#include <stdexcept>

HashTable::HashTable(size_t buf_size): _buffer_size(buf_size), _values_count(0) {
    _data = new TableLine[_buffer_size];
    for (size_t i = 0; i < _buffer_size; ++i) {
        _data[i] = TableLine();
    }
}

HashTable::~HashTable() {
    delete[] _data;
}

HashTable::HashTable(const HashTable& b) {
    _buffer_size  = b._buffer_size;
    _values_count = b._values_count;
    _data = new TableLine[_buffer_size];

    for (size_t i = 0; i < _buffer_size; ++i) {
        _data[i] = TableLine();
        for (const auto& it : b._data[i]) {
            _data[i].push_back(it);
        }
    }
}

HashTable::HashTable(HashTable&& other) {
    _buffer_size  = other._buffer_size;
    _values_count = other._values_count;
    _data = other._data;
    
    other._data = nullptr;
    other._buffer_size  = 0;
    other._values_count = 0;
}

Value& HashTable::operator[](const Key& key) {
    try {
        return this->at(key);
    }
    catch (const std::invalid_argument&) {
        Value* value = new Value(0, 0);
        this->insert(key, *value);
        
        return *value;
    }
}

const Value& HashTable::operator[](const Key& key) const {
    return this->at(key);
}

HashTable& HashTable::operator=(const HashTable& b) {
    HashTable hashTable(b);
    swap(hashTable);

    return *this;
}

HashTable& HashTable::operator=(HashTable&& b) {
    HashTable hashTable(b);
    
    swap(hashTable);
    
    b._data = nullptr;
    b._buffer_size = 0;

    return *this;
}


bool operator==(const HashTable& a, const HashTable& b) {
    if (a._buffer_size != b._buffer_size 
        && a._values_count != b._values_count) { return false; }

    for (size_t i = 0; i < a._buffer_size; ++i) {
        TableLine a_line = a._data[i];
        TableLine b_line = b._data[i];
        
        if (a_line.size() == b_line.size()) {
            for (auto& a_it : a_line) {
                for (auto& b_it : b_line) {
                    Value a_val = a_it.second;
                    Value b_val = b_it.second;

                    Key a_key = a_it.first;
                    Key b_key = b_it.first;

                    if (a_val != b_val || a_key.compare(b_key)) {
                        return false;
                    }
                }
            }
        } else {
            return false;
        }
    }
    return true;
}

bool operator!=(const HashTable& a, const HashTable& b) {
    return !(a == b);
}

bool HashTable::insert(const Key& key, const Value& value) {
    if (contains(key)) { return false; }
    
    double load = _values_count / (double)_buffer_size;
    if (load > _MAX_LOAD) { redistribute(); }

    _data[hash(key, _buffer_size)].push_back(std::pair<Key, Value>{key, Value(value)});
    
    _values_count++;
    
    return true;
}

void HashTable::swap(HashTable& other) {
    std::swap(_buffer_size, other._buffer_size);
    std::swap(_values_count, other._values_count);
    std::swap(_data, other._data);
}

bool HashTable::contains(const Key& key) const {
    try {
        this->at(key);
        return true;
    }
    catch (const std::invalid_argument&) {
        return false;
    }
}

Value& HashTable::at(const Key& key){
    TableLine line = _data[hash(key, _buffer_size)];
    
    for (auto& it : line) {
        if (!key.compare(it.first)) {
            Value* v = new Value(it.second);
            return *v;
        }
    }
    
    throw std::invalid_argument("No such key");
}

const Value& HashTable::at(const Key& key) const {
    TableLine line = _data[hash(key, _buffer_size)];
    
    for (auto& it : line) {
        if (!key.compare(it.first)) {

            return it.second;
        }
    }

    throw std::invalid_argument("No such key");
}

bool HashTable::erase(const Key& key) {
    try {
        Value& value = this->at(key);

        TableLine& line = _data[hash(key,_buffer_size)];
        
        line.remove_if([&](std::pair<Key, Value> p) { return !key.compare(p.first); });

        _values_count--;
        
        return true;
    }
    catch (const std::invalid_argument&) {
        return false;
    }
}

void HashTable::clear() {
    for (size_t i = 0; i < _buffer_size; ++i) {
        _data[i].clear();
    }
    _values_count = 0;
}

bool HashTable::empty() const {
    return (_values_count == 0);
}

size_t HashTable::size() const {
    return _values_count;
}

void HashTable::redistribute() {
    size_t new_buffer_size = _buffer_size * 3 / 2;
    TableLine *new_data = new TableLine[new_buffer_size];
    
    for (size_t i = 0; i < new_buffer_size; ++i) {
        new_data[i] = TableLine();
    }

    for (size_t i = 0; i < _buffer_size; ++i) {
        TableLine line = _data[i];
        for (auto it : line) {
            Key key = it.first;

            new_data[hash(key, new_buffer_size)].push_back(it);
        }
    }

    delete[] _data;

    _data = new_data;
    _buffer_size = new_buffer_size;

}

size_t HashTable::hash(const Key& key, size_t size) const {
    size_t len = key.length();
    size_t h = 0;
    for (size_t i = 0; i < len; ++i) {
        size_t symbol = key[i];
        h += (symbol % 3) * (size_t)pow(3, i);
    }
    return h % size;
}
