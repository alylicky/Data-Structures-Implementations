/**
 * @file schashtable.cpp
 * Implementation of the SCHashTable class.
 */

#include <iostream>
#include "schashtable.h"

template <class K, class V>
SCHashTable<K, V>::SCHashTable(size_t tsize)
{
    if (tsize <= 0)
        tsize = 17;
    size = findPrime(tsize);
    table = new std::list<std::pair<K, V>>[size];
    elems = 0;
}

template <class K, class V>
SCHashTable<K, V>::~SCHashTable()
{
    delete[] table;
}

template <class K, class V>
SCHashTable<K, V> const &SCHashTable<K, V>::
operator=(SCHashTable<K, V> const &rhs)
{
    if (this != &rhs)
    {
        delete[] table;
        table = new std::list<std::pair<K, V>>[rhs.size];
        for (size_t i = 0; i < rhs.size; i++)
            table[i] = rhs.table[i];
        size = rhs.size;
        elems = rhs.elems;
    }
    return *this;
}

template <class K, class V>
SCHashTable<K, V>::SCHashTable(SCHashTable<K, V> const &other)
{
    table = new std::list<std::pair<K, V>>[other.size];
    for (size_t i = 0; i < other.size; i++)
        table[i] = other.table[i];
    size = other.size;
    elems = other.elems;
}

template <class K, class V>
void SCHashTable<K, V>::insert(K const &key, V const &value)
{
    // std::cout << hashes::hash(key, size) << " " << size << std::endl;
    ++elems;
    if (shouldResize())
    {
        resizeTable();
    }
    size_t idx = hashes::hash(key, size);
    std::pair<K, V> p(key, value);
    // std::cout << idx << " " << size << std::endl;

    table[idx].push_front(p);
    return;
}

template <class K, class V>
void SCHashTable<K, V>::remove(K const &key)
{
    typename std::list<std::pair<K, V>>::iterator it;
    if (keyExists(key))
    {
        size_t idx = hashes::hash(key, size);
        typename std::list<std::pair<K, V>>::iterator it;

        for (it = table[idx].begin(); it != table[idx].end(); it++)
        {
            if (it->first == key)
            {
                table[idx].erase(it);
                return;
            }
        }
    }
}

template <class K, class V>
V SCHashTable<K, V>::find(K const &key) const
{
    if (keyExists(key))
    {
        size_t idx = hashes::hash(key, size);
        typename std::list<std::pair<K, V>>::iterator it;

        for (it = table[idx].begin(); it != table[idx].end(); it++)
        {
            if (it->first == key)
            {
                // std::cout << "found: " << key << " " << hashes::hash(key, size) << " " << it->second << std::endl;
                return it->second;
            }
        }
    }
    // std::cout << "not found: " << key << " " << hashes::hash(key, size) << " " << V() << std::endl;
    return V();
}

template <class K, class V>
V &SCHashTable<K, V>::operator[](K const &key)
{
    size_t idx = hashes::hash(key, size);
    typename std::list<std::pair<K, V>>::iterator it;
    for (it = table[idx].begin(); it != table[idx].end(); it++)
    {
        if (it->first == key)
            return it->second;
    }

    // was not found, insert a default-constructed version and return it
    ++elems;
    if (shouldResize())
        resizeTable();

    idx = hashes::hash(key, size);
    std::pair<K, V> p(key, V());
    table[idx].push_front(p);
    return table[idx].front().second;
}

template <class K, class V>
bool SCHashTable<K, V>::keyExists(K const &key) const
{
    size_t idx = hashes::hash(key, size);
    typename std::list<std::pair<K, V>>::iterator it;
    for (it = table[idx].begin(); it != table[idx].end(); it++)
    {
        if (it->first == key)
            return true;
    }
    return false;
}

template <class K, class V>
void SCHashTable<K, V>::clear()
{
    delete[] table;
    table = new std::list<std::pair<K, V>>[17];
    size = 17;
    elems = 0;
}

template <class K, class V>
void SCHashTable<K, V>::resizeTable()
{
    find("got");
    size_t old_size = size;
    size = findPrime(2 * size);
    std::list<std::pair<K, V>> *table2 = new std::list<std::pair<K, V>>[size];
    for (unsigned i = 0; i < old_size; i++)
    {
        typename std::list<std::pair<K, V>>::iterator it;
        for (it = table[i].begin(); it != table[i].end(); it++)
        {
            table2[i].push_back(*it);
            // std::cout << table2[i].back().second << " " << it->second << std::endl;
        }
    }
    table = new std::list<std::pair<K, V>>[size];
    for (unsigned i = 0; i < old_size; i++)
    {
        typename std::list<std::pair<K, V>>::iterator it;
        for (it = table2[i].begin(); it != table2[i].end(); it++)
        {
            insert(it->first, it->second);
        }
    }
    delete[] table2;
}