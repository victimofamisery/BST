#pragma once

#include <utility>
#include <cstddef>
/*!
    Имплементация бинарного дерева поиска
    Допускается дублирование ключей (аналог multimap)
*/
template <typename Key, typename Value>
class BinarySearchTree
{
    struct Node
    {
        Node(Key key, 
             Value value, 
             Node* parent = nullptr, 
             Node* left = nullptr, 
             Node* right = nullptr);
        Node* nextNode();
        Node* lastNode();
        Node* nextNode() const;
        Node* lastNode() const;
        std::pair<Key, Value> keyValuePair;
        Node* parent = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;
    };

public:
    //! Конструктор по умолчанию
    BinarySearchTree() = default;
    
    //! Копирование
    explicit BinarySearchTree(const BinarySearchTree& other);
    BinarySearchTree& operator=(const BinarySearchTree& other);
    //! Перемещение
    explicit BinarySearchTree(BinarySearchTree&& other) noexcept;
    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept;

    //! Деструктор
    ~BinarySearchTree();

    /*!
        Итератор бинарного дерева поиска

        Обходит дерево последовательно от меньшего ключа к большему 
    */
    class Iterator
    {
    public:
        explicit Iterator(Node* node);

        std::pair<Key, Value>& operator*();
        const std::pair<Key, Value>& operator*() const;

        std::pair<Key, Value>* operator->();
        const std::pair<Key, Value>* operator->() const;

        Iterator operator++();
        Iterator operator++(int);

        Iterator operator--();
        Iterator operator--(int);

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    private:
        // Node* _nextNode() const;
        // Node* _lastNode() const;
        Node* _node;
    };

    /*!
        Константный итератор
    */
    class ConstIterator
    {
    public:
        explicit ConstIterator(const Node* node);

        const std::pair<Key, Value>& operator*() const;

        const std::pair<Key, Value>* operator->() const;

        ConstIterator operator++();
        ConstIterator operator++(int);

        ConstIterator operator--();
        ConstIterator operator--(int);

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    private:
        // Node* _nextNode() const;
        // Node* _lastNode() const;
        const Node* _node;
    };

    // вставить элемент с ключем key и значением value
    void insert(const Key& key, const Value& value);

    // удалить все элементы с ключем key
    void erase(const Key& key);

    // найти первый элемент в дереве, равный ключу key
    ConstIterator find(const Key& key) const;
    Iterator find(const Key& key);

    // найти все элементы, у которых ключ равен key
    // первый итератор пары - первый элемент в дереве, равный key
    // второй итератор пары - первый элемент в дереве больший, чем key
    // [pair.first, pair.second) - полуинтервал, содержащий все элементы с ключем key
    std::pair<Iterator, Iterator> equalRange(const Key& key);
    std::pair<ConstIterator, ConstIterator> equalRange(const Key& key) const;

    // получить итератор на минимальное значение в дереве
    ConstIterator min(const Key& key) const;
    // получить итератор на максимальное значение в дереве
    ConstIterator max(const Key& key) const;

    Iterator begin();
    Iterator end();

    ConstIterator cbegin() const;
    ConstIterator cend() const;

    size_t size() const;
private:
    void _shiftNodes(Node* node1, Node* node2);
    Node* _findNode(const Key& key) const;
    void _recursiveDelete(Node* node);
    size_t _size = 0;
    Node* _root = nullptr; //!< корневой узел дерева
};


/*!
    Имплементация словаря
    Не допускается дублирование ключей (аналог std::map)
*/
template <typename Key, typename Value>
class Map
{
    BinarySearchTree<Key, Value> _tree;
public:
    using MapIterator = typename BinarySearchTree<Key, Value>::Iterator;
    using ConstMapIterator = typename BinarySearchTree<Key, Value>::ConstIterator;

    Map() = default;
    
    explicit Map(const Map& other);
    Map& operator=(const Map& other);

    explicit Map(Map&& other) noexcept;
    Map& operator=(Map&& other) noexcept;

    ~Map() = default;

    // вставить элемент с ключем key и значением value
    // если узел с ключем key уже представлен, то заменить его значение на value
    void insert(const Key& key, const Value& value);

    // удалить элемент с ключем key
    void erase(const Key& key);

    // найти элемент, равный ключу key
    ConstMapIterator find(const Key& key) const;
    MapIterator find(const Key& key);

    // доступ к элементу по ключу
    // если в момент обращения элемента не существует, создать его, 
    // ключ равен key, value равно дефолтному значению для типа Value
    const Value& operator[](const Key& key) const;
    Value& operator[](const Key& key);

    MapIterator begin();
    MapIterator end();

    ConstMapIterator cbegin() const;
    ConstMapIterator cend() const;

    size_t size() const;
};


/*!
    Имплементация множества
    Не допускается дублирование ключей (аналог std::set)
*/
template <typename Value>
class Set
{
    Map<Value, Value> _map;

public:
    using SetIterator = typename Map<Value, Value>::MapIterator;
    using ConstSetIterator = typename Map<Value, Value>::ConstMapIterator;

    Set() = default;

    explicit Set(const Set& other);
    Set& operator=(const Set& other);

    explicit Set(Set&& other) noexcept;
    Set& operator=(Set&& other) noexcept;

    ~Set() = default;

    void insert(const Value& value);

    void erase(const Value& value);

    ConstSetIterator find(const Value& value) const;
    SetIterator find(const Value& key);

    bool contains(const Value& value) const;
};

//BST

//Node
template <typename Key, typename Value>
BinarySearchTree<Key, Value>::Node::Node(Key key, Value value, Node* parent, Node* left, Node* right) 
:keyValuePair(key, value), parent(parent), left(left), right(right) {}

template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::_recursiveDelete(Node* node) {
    if (node->left) {
        _recursiveDelete(node->left);
    }
    if (node->right) {
        _recursiveDelete(node->right);
    }
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::Node* BinarySearchTree<Key, Value>::Node::nextNode() {
    auto node = this;
    if (!node) {
        return node;
    }
    if (node->right) {
        node = node->right;
        while (node->left) {
            node = node->left;
        }
    }
    else {
        while (node->parent && node->parent->right == node) {
            node = node->parent;
        }
        node = node->parent; 
    }
    return node;
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::Node* BinarySearchTree<Key, Value>::Node::lastNode() {
    auto node = this;
    if (!node) {
        return node;
    }
    if (node->left) {
        node = node->left;
        while (node->right) {
            node = node->right;
        }
    }
    else {
        while (node->parent && node->parent->left == node) {
            node = node->parent;
        }
        node = node->parent; 
    }
    return node;
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::Node* BinarySearchTree<Key, Value>::Node::nextNode() const {
    auto node = this;
    if (!node) {
        return node;
    }
    if (node->right) {
        node = node->right;
        while (node->left) {
            node = node->left;
        }
    }
    else {
        while (node->parent && node->parent->right == node) {
            node = node->parent;
        }
        node = node->parent; 
    }
    return node;
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::Node* BinarySearchTree<Key, Value>::Node::lastNode() const {
    auto node = this;
    if (!node) {
        return node;
    }
    if (node->left) {
        node = node->left;
        while (node->right) {
            node = node->right;
        }
    }
    else {
        while (node->parent && node->parent->left == node) {
            node = node->parent;
        }
        node = node->parent; 
    }
    return node;
}

//BigFive
template <typename Key, typename Value>
BinarySearchTree<Key, Value>::BinarySearchTree(const BinarySearchTree& other) {
    for (auto&& [key, value] : other) {
        insert(key, value);
    }
}

template <typename Key, typename Value>
BinarySearchTree<Key, Value>& BinarySearchTree<Key, Value>::operator=(const BinarySearchTree& other) {
    if (&other != this) {
        _recursiveDelete(_root);
        BinarySearchTree<Key, Value>* tmp = new BinarySearchTree<Key, Value>(other);
        this = tmp;
    }
    return *this;
}

template <typename Key, typename Value>
BinarySearchTree<Key, Value>::BinarySearchTree(BinarySearchTree&& other) noexcept {
    _root = std::move(other._root);
    _size = std::move(other._size);
}

template <typename Key, typename Value>
BinarySearchTree<Key, Value>& BinarySearchTree<Key, Value>::operator=(BinarySearchTree&& other) noexcept {
    if (&other != this) {
        _recursiveDelete(_root);
        this = std::move(other);
    }
    return this;
}

template <typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()  {
    _recursiveDelete(_root);
}


//Iterator
template <typename Key, typename Value>
BinarySearchTree<Key, Value>::Iterator::Iterator(Node* node) 
: _node(node) {}

template <typename Key, typename Value>
std::pair<Key, Value>& BinarySearchTree<Key, Value>::Iterator::operator*() {
    return _node->keyValuePair;
}

template <typename Key, typename Value>
const std::pair<Key, Value>& BinarySearchTree<Key, Value>::Iterator::operator*() const {
    return _node->keyValuePair;
}

template <typename Key, typename Value>
std::pair<Key, Value>* BinarySearchTree<Key, Value>::Iterator::operator->() {
    return &_node->keyValuePair;
}

template <typename Key, typename Value>
const std::pair<Key, Value>* BinarySearchTree<Key, Value>::Iterator::operator->() const {
    return &_node->keyValuePair;
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::Iterator BinarySearchTree<Key, Value>::Iterator::operator++() {
    _node = _node->nextNode();
    return *this;
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::Iterator BinarySearchTree<Key, Value>::Iterator::operator++(int) {
	Iterator bufIt = *this;
	++*this;
	return bufIt;
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::Iterator BinarySearchTree<Key, Value>::Iterator::operator--() {   
    _node = _node->lastNode();
    return *this;
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::Iterator BinarySearchTree<Key, Value>::Iterator::operator--(int) {
    Iterator bufIt = *this;
	--*this;
	return bufIt;
}

template <typename Key, typename Value>
bool BinarySearchTree<Key, Value>::Iterator::operator==(const Iterator& other) const {
    return _node == other._node;
}

template <typename Key, typename Value>
bool BinarySearchTree<Key, Value>::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

//ConstIterator
template <typename Key, typename Value>
BinarySearchTree<Key, Value>::ConstIterator::ConstIterator(const Node* node) 
: _node(node) {}

template <typename Key, typename Value>
const std::pair<Key, Value>& BinarySearchTree<Key, Value>::ConstIterator::operator*() const {
    return _node->keyValuePair;
}

template <typename Key, typename Value>
const std::pair<Key, Value>* BinarySearchTree<Key, Value>::ConstIterator::operator->() const {
    return &_node->keyValuePair;
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::ConstIterator BinarySearchTree<Key, Value>::ConstIterator::operator++() {
    _node = _node->nextNode();
    return *this;
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::ConstIterator BinarySearchTree<Key, Value>::ConstIterator::operator++(int) {
	ConstIterator bufIt = *this;
	++*this;
	return bufIt;
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::ConstIterator BinarySearchTree<Key, Value>::ConstIterator::operator--() {
    _node = _node->lastNode();
    return *this;
}



template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::ConstIterator BinarySearchTree<Key, Value>::ConstIterator::operator--(int) {
    typename ConstIterator::Iterator bufIt = *this;
	--*this;
	return bufIt;
}

template <typename Key, typename Value>
bool BinarySearchTree<Key, Value>::ConstIterator::operator==(const Iterator& other) const {
    return _node == other._node;
}

template <typename Key, typename Value>
bool BinarySearchTree<Key, Value>::ConstIterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

//Methods
template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::insert(const Key& key, const Value& value) {
    if (_size == 0) {
        _root = new Node(key, value);
    }
    else {
        Node* search = _root;
        while (true) {
            if (key >= search->keyValuePair.first) {
                if (!search->right) {
                    search->right = new Node(key, value, search);
                    break;
                }
                else {
                    search = search->right;
                }
            }
            if (key < search->keyValuePair.first) {
                if (!search->left) {
                    search->left = new Node(key, value, search);
                    break;
                }
                else {
                    search = search->left;
                }
            }
        }
    }
    _size++;
}

template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::erase(const Key& key) {
    Node* search = _findNode(key);
    if (!search->left) {
        _shiftNodes(search, search->right);
    }
    else if (!search->right) {
        _shiftNodes(search, search->left);
    }
    else {
        Node* replace = search->nextNode();
        if(replace->parent != search) {
            _shiftNodes(replace, replace->right);
            replace->right = search->right;
            replace->right->parent = replace;
        }
        _shiftNodes(search, replace);
        replace->left = search->left;
        replace->left->parent = replace;
    }
    delete search;
    _size--;
}

template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::_shiftNodes(Node* node1, Node* node2) {
    if (!node1->parent) {
        _root = node2;
    }
    else if (node1->parent->left == node1) {
        node1->parent->left = node2;
    }
    else {
        node1->parent->right = node2;
    }
    if (node2) {
        node2->parent = node1->parent;
    }
}


template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::ConstIterator BinarySearchTree<Key, Value>::find(const Key& key) const {
    return ConstIterator(_findNode(key));
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::Iterator BinarySearchTree<Key, Value>::find(const Key& key) {
    return Iterator(_findNode(key));
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::Node* BinarySearchTree<Key, Value>::_findNode(const Key& key) const {
    if (!_root) {
        return nullptr;
    }
    Node* search = _root;
    while (true) { 
        if (key == search->keyValuePair.first) {
            return search;
        }
        if (key < search->keyValuePair.first) {
            if (!search->left) {
                return nullptr;
            }
            search = search->left;
        }
        if (key > search->keyValuePair.first) {
            if (!search->right) {
                return nullptr;
            }
            search = search->right;
        }
    }
}

template <typename Key, typename Value>
std::pair<typename BinarySearchTree<Key, Value>::Iterator, typename BinarySearchTree<Key, Value>::Iterator> BinarySearchTree<Key, Value>::equalRange(const Key& key) {
    Iterator begin = find(key);
    Iterator end = begin;
    while ((*end).first == (*begin).first) {
        end++;
    }
    return std::pair<Iterator, Iterator> (begin, end);
}

template <typename Key, typename Value>
std::pair<typename BinarySearchTree<Key, Value>::ConstIterator, typename BinarySearchTree<Key, Value>::ConstIterator> BinarySearchTree<Key, Value>::equalRange(const Key& key) const {
    ConstIterator begin = find(key);
    ConstIterator end = begin;
    while ((*end).first == (*begin).first) {
        end++;
    }
    return std::pair<ConstIterator, ConstIterator> (begin, end);
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::ConstIterator BinarySearchTree<Key, Value>::min(const Key& key) const {
    auto range = equalRange(key);
    auto min = range.first;
    for (auto it = range.first; it < range.second; it++) {
        if ((*it).second < (*min).second) {
            min = it;
        }
    }
    return min;
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::ConstIterator BinarySearchTree<Key, Value>::max(const Key& key) const {
    auto range = equalRange(key);
    auto max = range.first;
    for (auto it = range.first; it < range.second; it++) {
        if ((*it).second > (*max).second) {
            max = it;
        }
    }
    return max;
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::Iterator BinarySearchTree<Key, Value>::begin() {
    Node* search = _root;
    while (search->left) {
        search = search->left;
    }
    return Iterator(search);
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::Iterator BinarySearchTree<Key, Value>::end() {
    return Iterator(nullptr);
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::ConstIterator BinarySearchTree<Key, Value>::cbegin() const {
    Node* search = _root;
    while (search->left) {
        search = search->left;
    }
    return ConstIterator(search);
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::ConstIterator BinarySearchTree<Key, Value>::cend() const {
    return ConstIterator(nullptr);
}

template <typename Key, typename Value>
size_t BinarySearchTree<Key, Value>::size() const {
 return _size;
}

//MAP

//BigFive
template <typename Key, typename Value>
Map<Key, Value>::Map(const Map& other) {
    _tree = other._tree;
}

template <typename Key, typename Value>
Map<Key, Value>& Map<Key, Value>::operator=(const Map& other) {
    if (&other != this) {
        delete _tree;
        _tree = other._tree;
    }
    return this;
}

template <typename Key, typename Value>
Map<Key, Value>::Map(Map&& other) noexcept {
    _tree = std::move(other._tree);
}

template <typename Key, typename Value>
Map<Key, Value>& Map<Key, Value>::operator=(Map&& other) noexcept {
    if (&other != this) {
        delete _tree;
        this = std::move(other);
    }
    return this;
}

//Methods
template <typename Key, typename Value>
void Map<Key, Value>::insert(const Key& key, const Value& value) {   
    MapIterator it = find(key);
    if (*it) {
        (*it).second = value;
    }
    else {
        _tree.insert(key, value);
    }
}

template <typename Key, typename Value>
void Map<Key, Value>::erase(const Key& key) {
    _tree.erase(key);
}

template <typename Key, typename Value>
typename Map<Key, Value>::ConstMapIterator Map<Key, Value>::find(const Key& key) const { 
    return _tree.find(key);
}

template <typename Key, typename Value>
typename Map<Key, Value>::MapIterator Map<Key, Value>::find(const Key& key) {
    return _tree.find(key);
}

template <typename Key, typename Value>
const Value& Map<Key, Value>::operator[](const Key& key) const {
    return (*find(key)).second;
}

template <typename Key, typename Value>
Value& Map<Key, Value>::operator[](const Key& key) {
    try {
        return ((*find(key)).second);
    }
    catch (...) {
        insert(key, Value());
        return ((*find(key)).second);
    }
}

template <typename Key, typename Value>
typename Map<Key, Value>::MapIterator Map<Key, Value>::begin() {
    return _tree.begin();
}

template <typename Key, typename Value>
typename Map<Key, Value>::MapIterator Map<Key, Value>::end() {
    return _tree.end();
}

template <typename Key, typename Value>
typename Map<Key, Value>::ConstMapIterator Map<Key, Value>::cbegin() const {
    return _tree.cbegin();
}

template <typename Key, typename Value>
typename Map<Key, Value>::ConstMapIterator Map<Key, Value>::cend() const {
    return _tree.cend();
}

template <typename Key, typename Value>
size_t Map<Key, Value>::size() const {
    return _tree.size();
}

//SET

//BigFive
template <typename Value>
Set<Value>::Set(const Set& other) {
    _map = other._map;
}

template <typename Value>
Set<Value>& Set<Value>::operator=(const Set& other) {
    if (&other != this) {
        delete _map;
        _map = other._map;
    }
    return this;
}

template <typename Value>
Set<Value>::Set(Set&& other) noexcept {
    _map = std::move(other._map);
}

template <typename Value>
Set<Value>& Set<Value>::operator=(Set&& other) noexcept {
    if (&other != this) {
        delete _map;
        this = std::move(other);
    }
    return this;
}

//Methods
template <typename Value>
void Set<Value>::insert(const Value& value) {
    _map.insert(value, value);
}

template <typename Value>
void Set<Value>::erase(const Value& value) {
    _map.erase(value);
}

template <typename Value>
typename Set<Value>::ConstSetIterator Set<Value>::find(const Value& value) const {
    return _map.find(value);
}

template <typename Value>
typename Set<Value>::SetIterator Set<Value>::find(const Value& key) {
    return _map.find(key);
}

template <typename Value>
bool Set<Value>::contains(const Value& value) const {
    try {
        find(value);
        return true;
    }
    catch (...) {
        return false;
    }
}
