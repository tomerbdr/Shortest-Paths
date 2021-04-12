#pragma once
#include <iostream>
using namespace std;

namespace ShortestPaths 
{
	class PriorityQueueExceptions : exception
	{
	protected:
		string m_Msg;
	public:
		explicit PriorityQueueExceptions(const string i_Msg) : m_Msg(i_Msg) {}
		virtual const char* what() const { return m_Msg.c_str(); }
	};

	template <class T>
	struct Pair
	{
	public:
		int m_Key;
		T m_Data;
		Pair(const int i_Key,const T i_Data) : m_Key(i_Key) ,m_Data(i_Data) {}
		Pair(const Pair& i_Other) : Pair(i_Other.m_Key,i_Other.m_Data) {}
	};

	template <class T>
	class PriorityQueue
	{
	protected:
		Pair<T>** m_Array;
		unsigned int m_MaxSize;
		unsigned int m_CurrentSize;

	public:
		PriorityQueue() { m_Array = nullptr;  m_MaxSize = m_CurrentSize = 0; }
		PriorityQueue(Pair<T>* i_Array, const unsigned int i_Size) { Build(i_Array, i_Size); }
		~PriorityQueue() { delete[] m_Array; }
		virtual void Build(Pair<T>* i_Array, const unsigned int i_Size) = 0;
		virtual Pair<T>& DeleteMin() = 0;
		virtual void DecreaseKey(const unsigned int i_Place, const int i_NewKey) = 0;
		bool IsEmpty() { return m_CurrentSize == 0; }
	};

	template <class T>
	class ArrayPriorityQueue : PriorityQueue<T>
	{
	private:
		unsigned int m_MinIndex = 0;
		unsigned int findMinIndex()
		{
			if (this->m_CurrentSize == 0) /*Empty*/
				throw PriorityQueueExceptions("Quence is empty.");

			int minKey = this->m_Array[0]->m_Key;
			int minIndex = 0;

			for (int i = 0; i < this->m_CurrentSize; i++)
			{
				if ((*(this->m_Array[i])).m_Key < minKey)
				{
					minKey = (*(this->m_Array[i])).m_Key;
					minIndex = i;
				}
			}

			return minIndex;
		}
	public:
		virtual void Build(Pair<T>* i_Array, const unsigned int i_Size)
		{
			if (i_Size == 0)
				throw PriorityQueueExceptions("Invalid argument - Size cannot be 0");

			this->m_CurrentSize = 0;
			this->m_Array = new Pair<T>*[i_Size];
			for (int i = 0; i < i_Size; i++)
			{
				this->m_Array[i] = &(i_Array[i]);
				this->m_CurrentSize++;
			}
			this->m_MaxSize = i_Size;
			this->m_MinIndex = findMinIndex();

		}
		virtual Pair<T>& DeleteMin() 
		{
			if (this->IsEmpty())
			{
				throw PriorityQueueExceptions("Quence is empty."); // Empty
			}
			Pair<T>& pairToDelete = *this->m_Array[m_MinIndex];
			if (this->m_MinIndex != this->m_CurrentSize - 1) // The pair to delete is not locate in the last position of the array.
			{
				this->m_Array[this->m_MinIndex] = this->m_Array[this->m_CurrentSize - 1];
				this->m_Array[this->m_CurrentSize - 1] = nullptr;
			}
			else
			{
				this->m_Array[this->m_MinIndex] = nullptr;
			}

			this->m_CurrentSize--;
			this->m_MinIndex = findMinIndex(); // Finding new min
			return pairToDelete;
		}
		virtual void DecreaseKey(const unsigned int i_Place, const int i_NewKey) 
		{
			if (i_Place >= this->m_CurrentSize)
				throw PriorityQueueExceptions("Invalid argument - Place is out of range.");

			this->m_Array[i_Place]->m_Key = i_NewKey;

			if (i_Place == this->m_MinIndex)
				this->m_MinIndex = findMinIndex();
		}
	};

	template <class T>
	class HeapPriorityQueue : PriorityQueue<T>
	{
	private:
		static const unsigned int leftChild(int i_NodeIndex) { return i_NodeIndex * 2 + 1; }
		static const unsigned int rightChild(int i_NodeIndex) { return i_NodeIndex * 2 + 2; }
		static const int parent(int i_NodeIndex) { return (i_NodeIndex - 1) / 2 ; }
		void fixHeap(int i_NodeIndex)
		{
			unsigned int min,left,right;
			left = leftChild(i_NodeIndex);
			right = rightChild(i_NodeIndex);

			if ((left < this->m_CurrentSize) && (this->m_Array[left]->m_Key < this->m_Array[i_NodeIndex]->m_Key))
				min = left;
			else
				min = i_NodeIndex;
			if ((right < this->m_CurrentSize) && (this->m_Array[right]->m_Key < this->m_Array[min]->m_Key))
				min = right;

			if (min != i_NodeIndex)
			{
				Pair<T>* temp = this->m_Array[i_NodeIndex];
				this->m_Array[i_NodeIndex] = this->m_Array[min];
				this->m_Array[min] = temp;
				fixHeap(min);
			}

		}
	public:
		virtual void Build(Pair<T>* i_Array, const unsigned int i_Size)
		{
			if (i_Size == 0)
				throw PriorityQueueExceptions("Invalid argument - Size cannot be 0");

			this->m_CurrentSize = 0;
			this->m_Array = new Pair<T> * [i_Size];
			for (int i = 0; i < i_Size; i++)
			{
				this->m_Array[i] = &(i_Array[i]);
				this->m_CurrentSize++;
			}
			this->m_MaxSize = i_Size;

			for (int i = (i_Size / 2 - 1); i >= 0; i--)
				fixHeap(i);
		}
		virtual Pair<T>& DeleteMin()
		{
			if (this->IsEmpty())
			{
				throw PriorityQueueExceptions("Quence is empty."); // Empty
			}

			Pair<T>& minPair = *this->m_Array[0];
			this->m_Array[0] = this->m_Array[this->m_CurrentSize - 1];
			this->m_CurrentSize--;
			fixHeap(0);

			return minPair;
		}
		virtual void DecreaseKey(const unsigned int i_Place, const int i_NewKey)
		{
			if (i_Place >= this->m_CurrentSize)
				throw PriorityQueueExceptions("Invalid argument - Place out of range");

			if (this->m_Array[i_Place]->m_Key <= i_NewKey)
				throw PriorityQueueExceptions("New key isn't lower the original key");

			this->m_Array[i_Place]->m_Key = i_NewKey;
			
			int currentParentIndex = parent(i_Place);
			int decreaseKeyIndex = i_Place;
			while (currentParentIndex >= 0 && this->m_Array[currentParentIndex]->m_Key > this->m_Array[decreaseKeyIndex]->m_Key)
			{
				Pair<T>* temp = this->m_Array[decreaseKeyIndex];
				this->m_Array[decreaseKeyIndex] = this->m_Array[currentParentIndex];
				this->m_Array[currentParentIndex] = temp;
				decreaseKeyIndex = currentParentIndex;
				currentParentIndex = parent(decreaseKeyIndex);
			}
		}
	};
}