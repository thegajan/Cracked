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
		KeyType m_key;
		ValueType m_value;
		Node* m_next = nullptr;
	};

	int bucketNumber(const KeyType& key) const;
	void newHashTable();
	void deleteHash(Node** hash, int size);

	Node** m_hash = nullptr;
	double m_maxLoadFactor;
	int m_size;
	int m_numHash = 0;
};

//implementaitons of myhash member funcitons

//MyHash Constructor O(B)
template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor) {
	m_size = 100;
	m_hash = new Node*[m_size]();
	if (maxLoadFactor <= 0)
		m_maxLoadFactor = 0.5;
	else if (maxLoadFactor > 2.0)
		m_maxLoadFactor = 2.0;
	else
		m_maxLoadFactor = maxLoadFactor;
}

//MyHash destructor O(B)
template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash() {
	deleteHash(m_hash, m_size);
}

//delete the hash table
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::deleteHash(Node** hash, int size) {
	for (int i = 0; i < size; i++) {
		Node* p = hash[i];
		while (p != nullptr)
		{
			Node* k = p;
			p = p->m_next;
			delete k;
		}
	}
	delete[] hash;
}

//MyHash reset function O(B)
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset() {
	deleteHash(m_hash, m_size);
	m_numHash = 0;
	m_size = 100;
	m_hash = new Node*[m_size]();
}

//MyHash associate function O(1) or O(B)
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) {
	ValueType* v = find(key);
	if (v == nullptr) {
		m_numHash++;
		Node* p = new Node{ key, value };
		int hashSpot = bucketNumber(key);
		if (m_hash[hashSpot] == nullptr)
			m_hash[hashSpot] = p;
		else {
			Node* c = m_hash[hashSpot];
			p->m_next = c;
			m_hash[hashSpot] = p;
		}
		if (1.0*m_numHash / m_size > m_maxLoadFactor)
			newHashTable();
	}
	else {
		*v = value;
	}
}

//MyHash newHashTable
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::newHashTable() {
	Node** temp = m_hash;
	int tempSize = m_size;

	m_size = m_size * 2;
	m_hash = new Node*[m_size]();
	m_numHash = 0;

	for (int i = 0; i < tempSize; i++) {
		Node* p = temp[i];
		while (p != nullptr) {
			associate(p->m_key, p->m_value);
			p = p->m_next;
		}
	}

	deleteHash(temp, tempSize);
}

//MyHash find O(1) or O(X)
template<typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const {
	int hashSpot = bucketNumber(key);
	Node* p = m_hash[hashSpot];
	while (p != nullptr) {
		if (p->m_key == key)
			return &(p->m_value);
		p = p->m_next;
	}
	return nullptr;
}

//MyHash getNumItems O(1)
template<typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const {
	return m_numHash;
}

//MyHash getLoadFactor O(1)
template<typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const {
	return (1.0*m_numHash) / m_size;
}

//MyHash bucket number 
template<typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::bucketNumber(const KeyType& key) const {
	unsigned int ::hash(const KeyType& k);
	return ::hash(key) % m_size;
}