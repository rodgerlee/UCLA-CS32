// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.
#ifndef MYHASH_INCLUDED
#define MYHASH_INCLUDED

const int DEFAULTNUMBUCKETS = 8;

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    ~ExpandableHashMap();
    void reset(); //resets hashmap back to 8 buckets, deletes all items
    int size() const;   //return number of associations in hashmap
    
    // The associate method associates one item (key) with another (value).
    // If no association currently exists with that key, this method inserts
    // a new association into the hashmap with that key/value pair. If there is
    // already an association with that key in the hashmap, then the item
    // associated with that key is replaced by the second parameter (value).
    // Thus, the hashmap must contain no duplicate keys.
    void associate(const KeyType& key, const ValueType& value);

      // for a map that can't be modified, return a pointer to const ValueType
    
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be
    // used to examine that value, and if the hashmap is allowed to be modified, to
    // modify that value directly within the map (the second overload enables
    // this). Using a little C++ magic, we have implemented it in terms of the
    // first overload, which you must implement.
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
    struct Bucket
    {
        KeyType m_key;
        ValueType m_value;
        Bucket* m_next;
    };
    
    //hash table (dynamic array)
    Bucket** m_hashTable;
    
    //max load factor
    double m_maxLoadFactor;
    int m_numBuckets;
    int m_numItems;
    
    //private helper functions
    void initializeHashTable(Bucket**& hashTable, int numBuckets);
    void deleteHashTable(Bucket**& hashTable, int numBuckets);
    unsigned int getBucketNumber(const KeyType& key, int numBuckets) const;
    
    
    
    //REMEMBER INSERT DOES NOT HANDLE INCREMENTING M_NUMITEMS
    void insertNewBucket(const KeyType& key, const ValueType& value, Bucket** hashTable, int numBuckets);
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
    initializeHashTable(m_hashTable, DEFAULTNUMBUCKETS);
    m_maxLoadFactor = maximumLoadFactor;
    m_numBuckets = DEFAULTNUMBUCKETS;
    m_numItems = 0;
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
    deleteHashTable(m_hashTable, m_numBuckets);
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    deleteHashTable(m_hashTable, m_numBuckets);
    
    m_numItems = 0;
    m_numBuckets = DEFAULTNUMBUCKETS;
    
    initializeHashTable(m_hashTable, DEFAULTNUMBUCKETS);
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    return m_numItems;  //returns number of associations
}

// The associate method associates one item (key) with another (value).
// If no association currently exists with that key, this method inserts
// a new association into the hashmap with that key/value pair. If there is
// already an association with that key in the hashmap, then the item
// associated with that key is replaced by the second parameter (value).
// Thus, the hashmap must contain no duplicate keys.
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    ValueType* associatevalue = find(key);
    if (associatevalue == nullptr)
    {
        insertNewBucket(key, value, m_hashTable, m_numBuckets);
        m_numItems++;
    }
    else
    {
        *associatevalue = value;
    }
    
    double loadfactor = static_cast<double>(m_numItems / m_numBuckets); //get load factor
    if (loadfactor > m_maxLoadFactor)
    {
        Bucket** tempHashTable;
        initializeHashTable(tempHashTable, m_numBuckets*2);
        
        for (int i = 0; i < m_numBuckets; i++)
        {
            Bucket *currentBucket = m_hashTable[i];
            
            while (currentBucket != nullptr) //while there are items in the current bucket
            {
                KeyType currentkey = currentBucket->m_key;
                ValueType currentvalue = currentBucket->m_value;
                
                insertNewBucket(currentkey, currentvalue, tempHashTable, m_numBuckets*2);
                
                currentBucket = currentBucket->m_next;
            }
        }
        
         //replace current hashtable with new hashtable
        deleteHashTable(m_hashTable, m_numBuckets);
        m_numBuckets *= 2;
        
        initializeHashTable(m_hashTable, m_numBuckets);
        
        for (int j = 0; j < m_numBuckets; j++)
        {
            m_hashTable[j] = tempHashTable[j];
        }
        
        delete [] tempHashTable;
    }
    
}

// If no association exists with the given key, return nullptr; otherwise,
// return a pointer to the value associated with that key. This pointer can be
// used to examine that value, and if the hashmap is allowed to be modified, to
// modify that value directly within the map (the second overload enables
// this). Using a little C++ magic, we have implemented it in terms of the
// first overload, which you must implement.

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned int bucketnum = getBucketNumber(key, m_numBuckets);
    
    Bucket* head = m_hashTable[bucketnum];
    Bucket* p = head;
    
    while (p != nullptr)
    {
        if (p->m_key == key)
            return &p->m_value;
        
        p = p->m_next;
    }
    
    return nullptr;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::initializeHashTable(Bucket**& hashTable, int numBuckets)
{
    hashTable = new Bucket*[numBuckets];
    for (int i = 0; i < numBuckets; i++)
    {
        hashTable[i] = nullptr;
    }
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::deleteHashTable(Bucket**& hashTable, int numBuckets)
{
    //delete all the nodes in each linked list
    for (int i = 0; i < numBuckets; i++)
    {
        Bucket* p = hashTable[i];
        
        while (p != nullptr)
        {
            Bucket* n = p->m_next;
            delete p;
            p = n;
        }
    }
    
    //delete the table itself
    delete [] hashTable;
}

template<typename KeyType, typename ValueType>
unsigned int ExpandableHashMap<KeyType, ValueType>::getBucketNumber(const KeyType& key, int numBuckets) const
{
    unsigned int hasher(const KeyType& k);    //prototype

    unsigned int hashValue = hasher(key);

    unsigned int bucketNum = hashValue % numBuckets;
    
    return bucketNum;
}


template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::insertNewBucket(const KeyType& key, const ValueType& value, Bucket** hashTable, int numBuckets)
{
    Bucket* newBucket = new Bucket;
    newBucket->m_key = key;
    newBucket->m_value = value;
    newBucket->m_next = nullptr;
    
    unsigned int bucketnum = getBucketNumber(key, numBuckets);
    
    Bucket* head = hashTable[bucketnum];
    Bucket* p = head; //check head
    
    if (p == nullptr)
    {
        hashTable[bucketnum] = newBucket;
    }
    else
    {
        while (p->m_next != nullptr)
        {
            p = p->m_next;
        }
        
        p->m_next = newBucket;
    }
}

#endif // MYHASH_INCLUDED
