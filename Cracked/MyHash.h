// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;

private:
	struct Node {
		Node(KeyType key, ValueType value) {
			m_key = key;
			m_value = value;
		}
		KeyType m_key;
		ValueType m_value;
		Node* m_next = nullptr;
	};


	void newHashTable();
	void deleteHash();
	void insert(Node* hashTable, Node* newNode);

	Node* m_hash = nullptr;
	double m_maxLoadFactor;
	int m_size;
	int m_numHash = 0;
};

//implementaitons of myhash member funcitons

//MyHash Constructor
template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor = 0.5) {
	m_size = 100;
	m_hash = new Node*[m_size];
	if (maxLoadFactor <= 0)
		m_maxLoadFactor = 0.5;
	else if (maxLoadFactor > 2.0)
		m_maxLoadFactor = 2.0;
	else
		m_maxLoadFactor = maxLoadFactor;
}

//MyHash destructor
template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash() {
	deleteHash();
}

//MyHash reset function
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset() {
	deleteHash();
	m_hash = new Node*[m_size];
}

//MyHash associate function
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) {
	ValueType* v = find(key);
	if (v == nullptr) {
		m_numHash++;
		if ((m_numHash) / m_size > m_maxLoadFactor)
			newHashTable();
		Node* p = new Node(key, value);
		insert(m_hash, p);
	}
	else {
		*v = value;
	}
}

//delete the hash table
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::deleteHash() {
	for (int i = 0; i < m_size; i++) {
		Node* p = m_hash[i];
		while (p != nullptr)
		{
			Node* k = p;
			p = p->next();
			delete k;
		}
	}
	delete m_hash;
}

//MyHash getNumItems
template<typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const {
	return m_numHash;
}

//MyHash getLoadFactor
template<typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const {
	return m_maxLoadFactor;
}

//MyHash find
template<typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const {
	int hashSpot = hash(key) % m_size;
	Node* p = m_hash[hashSpot];
	if (p->key == key)
		return *(p->value);
	else
		while (p != nullptr) {
			if (p->key == key)
				return *(p->value);
			p = p->next();
		}
	return nullptr;
}

//MyHash newHashTable
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::newHashTable() {
	m_size = m_size * 2;
	Node* newHash = new Node*[m_size];
	for (int i = 0; i < m_size/2; i++) {
		Node* p = m_hash[i];
		while (p != nullptr) {
			insert(newHash, p);
			p = p->next;
		}
	}
	deleteHash();
	m_hash = newHash;
}

//MyHash insert
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::insert(Node* hashTable, Node* newNode) {
	int hashSpot = hash(newNode->key) % m_size;
	if (hashTable[hashSpot] == nullptr)
		hashTable[hashSpot] = newNode;
	else {
		Node* c = hashTable[hashSpot];
		newNode->next = c;
		hashTable[hashSpot] = newNode;
	}
}