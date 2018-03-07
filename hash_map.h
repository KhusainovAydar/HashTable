//
// Created by MacBook on 03.02.18.
//

#ifndef HASHTABLE_HASH_MAP_H
#define HASHTABLE_HASH_MAP_H

#include <stdexcept>
#include <iostream>
#include <vector>
#include <list>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
private:
    std::vector<std::list<std::pair<const KeyType, ValueType>>> Map;
    Hash hasher;
    size_t sizeMap;
    const size_t firstSizeMap = 19;
    size_t sizeVectorMap;

public:
    HashMap<KeyType, ValueType, Hash>() {
        clear();
        sizeMap = 0;
        sizeVectorMap = firstSizeMap;
        Map.resize(sizeVectorMap);
    }

    HashMap<KeyType, ValueType, Hash> (Hash _hasher)
    : sizeMap(0)
    , sizeVectorMap(firstSizeMap)
    , hasher(_hasher)
    {
        clear();
        Map.resize(sizeVectorMap);
    }

    template<typename Iter>
    HashMap<KeyType, ValueType, Hash>(Iter begin, Iter end)
    : sizeMap(0)
    , sizeVectorMap(firstSizeMap)
    {
        clear();
        Map.resize(sizeVectorMap);
        auto first = begin, second = end;
        while (first != second) {
            insert(*first);
            ++first;
        }
    }

    HashMap<KeyType, ValueType, Hash>(std::initializer_list<std::pair<const KeyType, ValueType>> init)
    : sizeMap(0)
    , sizeVectorMap(firstSizeMap)
    {
        clear();
        Map.resize(sizeVectorMap);
        for (auto & t : init) {
            insert(t);
        }
    }

    size_t size() const {
        return sizeMap;
    }

    bool empty() const {
        return (sizeMap == 0);
    }

    Hash hash_function() const {
        return hasher;
    }

    void check() {
        if (sizeMap * 3 > sizeVectorMap * 2) {
            build();
        }
    }

    void insert(std::pair<KeyType, ValueType> value) {
        if (find(value.first) != end()) {
            return;
        }
        ++sizeMap;
        size_t id = hasher(value.first) % sizeVectorMap;
        Map[id].push_back(value);

        check();
        return;
    }

    void erase(KeyType key) {
        size_t id = hasher(key) % sizeVectorMap;
        auto delpair = Map[id].begin();
        while (delpair != Map[id].end()) {
            if (delpair->first == key) {
                Map[id].erase(delpair);
                --sizeMap;
                return;
            }
            ++delpair;
        }
        return;
    }

    ValueType& operator [] (const KeyType & key) {
        auto findVal = find(key);
        if (findVal == end()) {
            insert({key, ValueType()});
            findVal = find(key);
        }
        return findVal->second;
    }

    const ValueType& at(const KeyType & key) const {
        const_iterator findVal = find(key);
        if (findVal == end()) {
            throw std::out_of_range("lol");
        }
        return findVal->second;
    }

    void clear() {
        if (empty())
            return;
        for (auto &i : Map) {
            i.clear();
        }
        sizeMap = 0;
    }

    void build() {
        std::vector<std::pair<const KeyType, ValueType>> copyMap;
        for (auto & t : Map) {
            for (auto & i : t) {
                copyMap.push_back(i);
            }
        }
        clear();
        sizeVectorMap *= 2;
        Map.resize(sizeVectorMap);
        for (auto & t : copyMap) {
            insert(t);
        }
    }

    HashMap& operator = (HashMap other) {
        clear();
        for (auto &t : other) {
            insert(t);
        }
        return *this;
    }

    class iterator {
        using reference = typename std::pair<const KeyType, ValueType>&;
        using pointer = typename std::pair<const KeyType, ValueType>;
        using hashMapT = HashMap<KeyType, ValueType, Hash>*;
        using iteratorT = typename std::list<std::pair<const KeyType, ValueType>>::iterator;

      private:
        hashMapT hashMap;
        size_t id;
        iteratorT it;

      public:
        iterator() {}

        iterator(hashMapT _hashMap, size_t _id, iteratorT _it)
        : hashMap(_hashMap)
        , id(_id)
        , it(_it)
        {}

        iterator(hashMapT _hashMap)
        : hashMap(_hashMap) {
            id = hashMap->Map.size() - 1;
            it = hashMap->Map[id].end();
            for (size_t i = 0; i != hashMap->Map.size(); ++i) {
                if (!hashMap->Map[i].empty()) {
                    id = i;
                    it = hashMap->Map[i].begin();
                    break;
                }
            }
        }

        reference operator* () {
            return *it;
        }

        iteratorT operator -> () {
            return it;
        }

        iterator& operator++ () {
            ++it;
            if (it == hashMap->Map[id].end()) {
                ++id;
                while (id < hashMap->Map.size() && hashMap->Map[id].empty()) {
                    ++id;
                }
                if (id == hashMap->Map.size()) {
                    --id;
                    it = hashMap->Map[id].end();
                } else {
                    it = hashMap->Map[id].begin();
                }
            }
            return *this;
        }

        iterator operator++ (int) {
            auto temp = *this;
            ++*this;
            return temp;
        }

        iterator& operator-- () {
            if (it == hashMap->Map[id].begin()) {
                --id;
                while (id >= 0 && hashMap->Map[id].empty()) {
                    --id;
                }
                it = hashMap->Map[id].rbegin();
            } else {
                --it;
            }
            return *this;
        }

        iterator operator-- (int) {
            auto temp = *this;
            --*this;
            return temp;
        }

        bool operator == (iterator other) const {
            return id == other.id && it == other.it;
        }

        bool operator != (iterator other) const {
            return !(*this == other);
        }
    };

    class const_iterator {
        using reference = const std::pair<const KeyType, ValueType>&;

        using pointer = const std::pair<const KeyType, ValueType>*;

        using hashMapT = const HashMap<KeyType, ValueType, Hash>*;

        using iteratorT = typename std::list<std::pair<const KeyType, ValueType>>::const_iterator;

      private:
        hashMapT hashMap;
        size_t id;
        iteratorT it;

      public:
        const_iterator() {}

        const_iterator(hashMapT _hashMap, size_t _id, iteratorT _it)
        : hashMap(_hashMap)
        , id(_id)
        , it(_it)
        {}

        const_iterator(hashMapT _hashMap)
        : hashMap(_hashMap) {
            id = hashMap->Map.size() - 1;
            it = hashMap->Map[id].end();
            for (size_t i = 0; i != hashMap->Map.size(); ++i) {
                if (!hashMap->Map[i].empty()) {
                    id = i;
                    it = hashMap->Map[i].begin();
                    break;
                }
            }
        }

        reference operator* () {
            return *it;
        }

        iteratorT operator -> () {
            return it;
        }

        const_iterator& operator++ () {
            ++it;
            if (it == hashMap->Map[id].end()) {
                ++id;
                while (id < hashMap->Map.size() && hashMap->Map[id].empty()) {
                    ++id;
                }
                if (id == hashMap->Map.size()) {
                    --id;
                    it = hashMap->Map[id].end();
                } else {
                    it = hashMap->Map[id].begin();
                }
            }
            return *this;
        }

        const_iterator operator++ (int) {
            auto temp = *this;
            ++*this;
            return temp;
        }

        const_iterator& operator-- () {
            if (it == hashMap->Map[id].begin()) {
                --id;
                while (id >= 0 && hashMap->Map[id].empty()) {
                    --id;
                }
                it = hashMap->Map[id].rbegin();
            } else {
                --it;
            }
            return *this;
        }

        const_iterator operator-- (int) {
            auto temp = *this;
            --*this;
            return temp;
        }

        bool operator == (const_iterator other) const {
            return id == other.id && it == other.it;
        }

        bool operator != (const_iterator other) const {
            return !(*this == other);
        }
    };

    iterator begin() {
        return iterator(this);
    }

    iterator end() {
        return iterator(this, sizeVectorMap - 1, Map.back().end());
    }

    const_iterator begin() const {
        return const_iterator(this);
    }

    const_iterator end() const {
        return const_iterator(this, sizeVectorMap - 1, Map.back().end());
    }

    iterator find(const KeyType& key) {
        size_t id = hasher(key) % sizeVectorMap;
        auto findpair = Map[id].begin();
        while (findpair != Map[id].end()) {
            if (findpair->first == key) {
                return iterator(this, id, findpair);
            }
            ++findpair;
        }
        return end();
    }

    const_iterator find(const KeyType& key) const {
        size_t id = hasher(key) % sizeVectorMap;
        if (Map[id].size()) {
            auto findpair = Map[id].begin();
            while (findpair != Map[id].end()) {
                if (findpair->first == key) {
                    return const_iterator(this, id, findpair);
                }
                ++findpair;
            }
        }
        return end();
    }
};


#endif //HASHTABLE_HASH_MAP_H