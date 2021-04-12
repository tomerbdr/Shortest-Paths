#pragma once
#include <iostream>
using namespace std;

namespace ShortestPaths {
	/**** Linked-list for use such as <list> in STL****/
	template <class T>
	class list
	{
	private:
		class ListNode
		{
		public:
			T m_Data;
			ListNode* m_Next;
			ListNode* m_Prev;
		};

		ListNode* m_Head;
		ListNode* m_Tail;
		int m_Size;

	public:
		class Iterator
		{
			friend class list;
			ListNode* m_Ptr;
		public:
			Iterator() {};
			Iterator(ListNode* i_Ptr) { m_Ptr = i_Ptr; }
			Iterator(const Iterator& other) : m_Ptr(other.m_Ptr) {}
			Iterator& operator++() {
				m_Ptr = m_Ptr->m_Next;
				return *this;
			}
			Iterator& operator++(int)
			{
				Iterator temp(this->m_Ptr);
				m_Ptr = m_Ptr->m_Next;
				return temp;
			}
			bool operator !=(const Iterator& other) const { return !(*this == other); }
			bool operator ==(const Iterator& other) const { return (this->m_Ptr == other.m_Ptr); }
			T& operator*() { return m_Ptr->m_Data; }
		};

		list() :m_Head(nullptr), m_Tail(nullptr), m_Size(0) {}
		~list() 
		{
			ListNode* currentNode = m_Head;
			ListNode* nodeToDel;

			while (currentNode != nullptr) 
			{
				nodeToDel = currentNode;
				currentNode = currentNode->m_Next;
				delete nodeToDel;
			}
		}
		void push_front(T i_Data)
		{
			ListNode* nodeToAdd = new (ListNode);
			nodeToAdd->m_Prev = nullptr;
			nodeToAdd->m_Data = i_Data;

			if (!m_Head) { // Empty List
				nodeToAdd->m_Next = nullptr;
				m_Head = m_Tail = nodeToAdd;
			}
			else { // Insert before current head
				nodeToAdd->m_Next = m_Head;
				m_Head->m_Prev = nodeToAdd;
				m_Head = nodeToAdd;
			}

			m_Size++;
		}
		void push_back(T i_data)
		{
			ListNode* nodeToAdd = new ListNode;
			nodeToAdd->m_Next = nullptr;
			nodeToAdd->m_Data = i_data;

			if (!m_Head) { // Empty list
				nodeToAdd->m_Prev = nullptr;
				m_Head = m_Tail = nodeToAdd;
			}
			else { // Insert to tail
				nodeToAdd->m_Prev = m_Tail;
				m_Tail->m_Next = nodeToAdd;
				m_Tail = nodeToAdd;
			}

			m_Size++;
		}
		void pop_front()
		{
			this->erase(this->front());
		}
		void pop_back()
		{
			this->erase(this->end());
		}
		void clear()
		{
			ListNode* temp = m_Head;
			ListNode* del;
			while (temp != nullptr)
			{
				del = temp;
				temp = temp->m_Next;
				delete del;
				m_Size--;
			}
			m_Head = m_Tail = nullptr;
		}
		T& front() { return m_Head->m_Data; }
		int size() { return m_Size; }
		bool empty() { return m_Size; }
		bool operator ==(list& other)
		{
			ListNode* first = this->m_Head;
			ListNode* second = other->m_Head;
			while (first && second)
			{
				if (first != second) {
					return false;
				}
				else {
					first = first->m_Next;
					second = second->m_Next;
				}
			}
			if (!(first) && !(second))
				return true;
			else
				return false;
		}
		list& operator =(list& other)
		{
			ListNode* temp = other->m_Head;
			this->clear();

			while (temp)
			{
				this->push_back(temp->m_Data);
				temp = temp->m_Next;
			}

		}
		Iterator begin() { return Iterator(m_Head); }
		Iterator end() { return Iterator(nullptr); }
		Iterator erase(const Iterator& i_iterToErase) {
			Iterator prevIter = i_iterToErase.m_Ptr->m_Prev;
			
			if (prevIter == nullptr) // Node to erase is the head of the list
			{
				m_Head = i_iterToErase.m_Ptr->m_Next;
				if (m_Head == nullptr) // After erase the node - the list will be empty
					m_Tail == nullptr;
				else
					m_Head->m_Prev = nullptr;
			}
			else // Node to erase is not the head
			{
				prevIter.m_Ptr->m_Next = i_iterToErase.m_Ptr->m_Next;
				if (i_iterToErase.m_Ptr->m_Next == nullptr) // Node to erase is the tail of the list
				{
					m_Tail = prevIter.m_Ptr;
				}
				else
				{
					i_iterToErase.m_Ptr->m_Next->m_Prev = prevIter.m_Ptr;
				}
			}

			delete i_iterToErase.m_Ptr;
			m_Size--;
			return prevIter;
		}
	};


}
