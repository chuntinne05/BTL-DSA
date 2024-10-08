/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include <iostream>
#include <sstream>
#include <type_traits>

#include "include/list/IList.h"
using namespace std;

template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head;
    Node *tail;
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);
    void (*deleteUserData)(DLinkedList<T> *);

public:
    DLinkedList(void (*deleteUserData)(DLinkedList<T> *) = 0,
                bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // Inherit from IList: END

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    Iterator begin() { return Iterator(this, true); }
    Iterator end() { return Iterator(this, false); }

    BWDIterator bbegin() { return BWDIterator(this, true); }
    BWDIterator bend() { return BWDIterator(this, false); }

protected:
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);

    //! FUNTION STATIC
public:
    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }

public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

public:
    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*() { return pNode->data; }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };

    class BWDIterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        BWDIterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->tail->prev;
                else
                    pNode = 0;
            }
            else
            {
                this->pNode = pList->head;
            }
            this->pList = pList;
        }

        BWDIterator &operator=(const BWDIterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }

        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pPrev = pNode->next; // Đối với BWDIterator, chuyển tới node kế tiếp khi xoá
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pPrev;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }

        bool operator!=(const BWDIterator &iterator)
        {
            return pNode != iterator.pNode;
        }

        BWDIterator &operator++()
        { // Tiến ngược
            pNode = pNode->prev;
            return *this;
        }

        BWDIterator operator++(int)
        {
            BWDIterator iterator = *this;
            ++*this;
            return iterator;
        }
        BWDIterator &operator--()
        {
            pNode = pNode->next;
            return *this;
        }

        BWDIterator operator--(int)
        {
            BWDIterator iterator = *this;
            --*this;
            return iterator;
        }
    };
};

template <class T>
using List = DLinkedList<T>;

//! ////////////////////////////////////////////////////////////////////
//! //////////////////////     METHOD DEFNITION      ///////////////////
//! ////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(
    void (*deleteUserData)(DLinkedList<T> *),
    bool (*itemEqual)(T &, T &))
{
    // TO DO
    this->deleteUserData = deleteUserData;
    this->itemEqual = itemEqual;
    this->count = 0;
    this->head = new Node();
    this->tail = new Node();
    this->head->next = this->tail;
    this->tail->prev = this->head;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
    // TODO
    copyFrom(list);
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    // TODO
    if (this != &list)
    {
        removeInternalData();
        copyFrom(list);
    }
    return *this;
}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    // TODO
    removeInternalData();
    delete head;
    delete tail;
}

template <class T>
void DLinkedList<T>::add(T e)
{
    // TODO
    Node *newNode = new Node(e, tail, tail->prev);
    tail->prev->next = newNode;
    tail->prev = newNode;
    count++;
}
template <class T>
void DLinkedList<T>::add(int index, T e)
{
    if (index < 0 || index > count)
    {
        throw std::out_of_range("Index is out of range!");
    }

    Node *newNode = new Node(e);

    if (index == 0)
    {
        newNode->next = head->next;
        newNode->prev = head;
        if (head->next != nullptr)
        {
            head->next->prev = newNode;
        }
        head->next = newNode;
        if (count == 0)
        {
            tail->prev = newNode;
        }
    }
    else if (index == this->count)
    {
        newNode->prev = tail->prev;
        newNode->next = tail;
        if (tail->prev != nullptr)
        {
            tail->prev->next = newNode;
        }
        tail->prev = newNode;
        if (count == 0)
        {
            head->next = newNode;
        }
    }

    else
    {
        Node *prevNode = getPreviousNodeOf(index);
        // cout << prevNode->data << "\n";
        newNode->next = prevNode->next;
        newNode->prev = prevNode;
        if (prevNode->next != nullptr)
        {
            prevNode->next->prev = newNode;
        }
        prevNode->next = newNode;

        if (newNode->next != nullptr)
        {
            newNode->next->prev = newNode;
        }
    }
    count++;
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
{
    /**
     * Returns the node preceding the specified index in the doubly linked list.
     * If the index is in the first half of the list, it traverses from the head; otherwise, it traverses from the tail.
     * Efficiently navigates to the node by choosing the shorter path based on the index's position.
     */
    // TODO
    if (index < 0 || index > count)
    {
        throw std::out_of_range("Index is out of range!");
    }
    Node *current = nullptr;
    if (index <= count / 2)
    {
        current = head->next;
        for (int i = 0; i < index - 1; ++i)
        {
            current = current->next;
        }
    }
    else
    {
        current = tail->prev;
        for (int i = count - 1; i >= index; i--)
        {
            current = current->prev;
        }
    }
    return current;
}

template <class T>
T DLinkedList<T>::removeAt(int index)
{
    // TODO
    Node *nodeRemoved = getPreviousNodeOf(index + 1);
    T data = nodeRemoved->data;

    nodeRemoved->prev->next = nodeRemoved->next;
    nodeRemoved->next->prev = nodeRemoved->prev;

    if (deleteUserData)
    {
        delete nodeRemoved;
    }
    count--;
    return data;
}

template <class T>
bool DLinkedList<T>::empty()
{
    return count == 0;
}

template <class T>
int DLinkedList<T>::size()
{
    return count;
}

template <class T>
void DLinkedList<T>::clear()
{
    removeInternalData();
    head->next = tail;
    tail->prev = head;
    count = 0;
}

template <class T>
T &DLinkedList<T>::get(int index)
{
    return getPreviousNodeOf(index + 1)->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    Node *current = head->next;
    for (int i = 0; i < count; ++i)
    {
        if (equals(current->data, item, itemEqual))
        {
            return i;
        }
        current = current->next;
    }
    return -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    int idx = indexOf(item);
    if (idx != -1)
    {
        removeAt(idx);
        if (removeItemData)
        {
            removeItemData(item);
        }
        return true;
    }
    return false;
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    return indexOf(item) != -1;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{
    /**
     * Converts the list into a string representation, where each element is formatted using a user-provided function.
     * If no custom function is provided, it directly uses the element's default string representation.
     * Example: If the list contains {1, 2, 3} and the provided function formats integers, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function that converts an item of type T to a string. If null, default to string conversion of T.
     * @return A string representation of the list with elements separated by commas and enclosed in square brackets.
     */
    // TODO
    stringstream ss;
    ss << "[";
    Node *current = head->next;
    for (int i = 0; i < count; ++i)
    {
        if (i > 0)
        {
            ss << ", ";
        }
        if (item2str)
        {
            ss << item2str(current->data);
        }
        else
        {
            ss << current->data;
        }
        current = current->next;
    }
    ss << "]";
    return ss.str();
}

template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
{
    /**
     * Copies the contents of another doubly linked list into this list.
     * Initializes the current list to an empty state and then duplicates all data and pointers from the source list.
     * Iterates through the source list and adds each element, preserving the order of the nodes.
     */
    // TODO
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;
    this->count = list.count;

    this->head = new Node();
    this->tail = new Node();
    this->head->next = this->tail;
    this->tail->prev = this->head;

    Node *srcNode = list.head->next;
    while (srcNode != list.tail)
    {
        add(srcNode->data);
        srcNode = srcNode->next;
    }
}

template <class T>
void DLinkedList<T>::removeInternalData()
{
    /**
     * Clears the internal data of the list by deleting all nodes and user-defined data.
     * If a custom deletion function is provided, it is used to free the user's data stored in the nodes.
     * Traverses and deletes each node between the head and tail to release memory.
     */
    // TODO
    Node *current = head->next;
    while (current != tail)
    {
        Node *nextNode = current->next;
        if (deleteUserData)
        {
            delete current;
        }

        current = nextNode;
    }
}

#endif /* DLINKEDLIST_H */

int main()
{
    DLinkedList<int> list;
    list.add(0);
    list.add(0, 1);
    list.add(1, 2);
    list.add(3, 3);
    list.add(2, 4);
    list.add(4, 5);
    list.add(6);
    list.add(6, 7);
    list.add(0, 8);
    list.add(1, 9);
    list.add(2, 10);
    cout << list.toString();
}

// [8, 9, 10, 1, 2, 4, 0, 5, 3, 7, 6]