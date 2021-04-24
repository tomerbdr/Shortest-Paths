#pragma once
#include <iostream>
#define NOT_IN_QUEUE -1 // MUSE BE A NEGATIVE NUMBER 
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

	// Pair of <Key,Data> 
	template <class T>
	struct Pair
	{
	private:
		// This struct use exactly as float - with improve of define Infinty value.
		// Initially an instance of the class initiliazed to Infinity until get a value.
		struct Value
		{
			float m_Value;
			bool isInfinity;
			Value() { isInfinity = true; m_Value = 0; }
			Value(float i_Num) { isInfinity = false; m_Value = i_Num; }
			void operator=(float i_Num) { isInfinity = false; m_Value = i_Num; }
			bool operator >(float i_Num) { return (isInfinity || m_Value > i_Num); }
			bool operator <(float i_Num) { return (!isInfinity && m_Value < i_Num); }
			bool operator ==(float i_Num) { return (!isInfinity && m_Value == i_Num); }
			bool operator >=(float i_Num) { return (this->operator>(i_Num) || this->operator==(i_Num)); }
			bool operator <=(float i_Num) { return (this->operator<(i_Num) || this->operator==(i_Num)); }
			bool operator >(Value& i_Other) 
			{
				bool res;
				if (i_Other.isInfinity)
					res = false;
				else if (this->isInfinity)
					res = true;
				else
					res = this->m_Value > i_Other.m_Value;
				
				return res;
			}
			bool operator <(Value& i_Other)
			{
				bool res;
				if (this->isInfinity)
					res = false;
				else if (i_Other.isInfinity)
					res = true;
				else
					res = this->m_Value < i_Other.m_Value;

				return res;
			}
			operator float() { return m_Value; }
		};

	public:
		Value m_Key;
		T m_Data;
		Pair() { m_Data = 0; }
		Pair(const float i_Key, const T i_Data) : m_Key(i_Key), m_Data(i_Data) {}
		Pair(const Pair& i_Other) : Pair(i_Other.m_Key, i_Other.m_Data) {}
	};

	/**** An abstract class which use as a BASE class to different data structores of Min Prioty Queue****/
	template <class T>
	class PriorityQueue
	{
	protected:
		Pair<T>** m_Array;
		int* m_CurrIndex; // Conatin the current position of all array members
		int** m_PtrToCurrIndex; 
		unsigned int m_MaxSize;
		unsigned int m_CurrentSize;

		/* Get an array of pairs and initilize the queue */
		void init(Pair<T>* i_Array, const unsigned int i_Size)
		{
			this->m_MaxSize = i_Size;
			this->m_CurrentSize = 0;
			this->m_Array = new Pair<T> * [this->m_MaxSize];
			this->m_CurrIndex = new int[this->m_MaxSize];
			this->m_PtrToCurrIndex = new int* [this->m_MaxSize];

			for (int i = 0; i < i_Size; i++)
			{
				this->m_Array[i] = &(i_Array[i]);
				this->m_CurrIndex[i] = i; // Currently all the objects in the array are in the starting position
				this->m_PtrToCurrIndex[i] = &(this->m_CurrIndex[i]); // Point to the node of position
				this->m_CurrentSize++;
			}
		}

		/* Get the current index of the object that was in the i_InitialInd place at the init stage */
		unsigned int getCurrentIndex(const unsigned int i_InitialInd) { return m_CurrIndex[i_InitialInd]; }

		/* Swaps between objects in the array - Updating also currentIndex array */
		void swap(const unsigned int i_FirstInd, const unsigned int i_SecondInd)
		{
			// Swap
			Pair<T>* tempPair = m_Array[i_FirstInd];
			int* tempPtrToInd = m_PtrToCurrIndex[i_FirstInd];

			m_Array[i_FirstInd] = m_Array[i_SecondInd];
			m_PtrToCurrIndex[i_FirstInd] = m_PtrToCurrIndex[i_SecondInd];

			m_Array[i_SecondInd] = tempPair;
			m_PtrToCurrIndex[i_SecondInd] = tempPtrToInd;

			// Updating new index
			if (m_PtrToCurrIndex[i_FirstInd] != nullptr)
				*(m_PtrToCurrIndex[i_FirstInd]) = i_FirstInd;

			if (m_PtrToCurrIndex[i_SecondInd] != nullptr)
				*(m_PtrToCurrIndex[i_SecondInd]) = i_SecondInd;
		}

	public:
		PriorityQueue() { m_Array = nullptr; m_CurrIndex = nullptr; m_PtrToCurrIndex = nullptr;  m_MaxSize = m_CurrentSize = 0; }
		PriorityQueue(Pair<T>* i_Array, const unsigned int i_Size) { Build(i_Array, i_Size); }
		~PriorityQueue() { delete[] m_Array; delete[] m_CurrIndex; delete[] m_PtrToCurrIndex; }
		bool IsEmpty() { return m_CurrentSize == 0; }

		/***Pure Virtual methods***/
		virtual void Build(Pair<T>* i_Array, const unsigned int i_Size) = 0;
		virtual Pair<T>& DeleteMin() = 0;
		virtual void DecreaseKey(const unsigned int i_Place, const float i_NewKey) = 0;
	};

	template <class T>
	class ArrayPriorityQueue : public PriorityQueue<T>
	{
	private:
		unsigned int m_MinIndex = 0;

		// Returning the index of the min key in the queue
		unsigned int findMinIndex()
		{
			if (this->m_CurrentSize == 0) /*Empty*/
				throw PriorityQueueExceptions("Quence is empty.");

			int minIndex = 0;

			for (int i = 0; i < this->m_CurrentSize; i++)
			{
				if (this->m_Array[i]->m_Key < this->m_Array[minIndex]->m_Key)
				{
					minIndex = i;
				}
			}

			return minIndex;
		}

	public:
		virtual void Build(Pair<T>* i_Array, const unsigned int i_Size) override
		{
			if (i_Size == 0)
				throw PriorityQueueExceptions("Invalid argument - Size cannot be 0");

			this->init(i_Array, i_Size); // INIT arrays
			this->m_MinIndex = findMinIndex();
		}
		virtual Pair<T>& DeleteMin() override
		{
			if (this->IsEmpty())
			{
				throw PriorityQueueExceptions("Quence is empty."); // Empty
			}

			Pair<T>& pairToDelete = *this->m_Array[m_MinIndex];
			this->m_Array[this->m_MinIndex] = nullptr;
			*this->m_PtrToCurrIndex[this->m_MinIndex] = NOT_IN_QUEUE;
			this->m_PtrToCurrIndex[this->m_MinIndex] = nullptr; // position not rellevant anymore

			if (this->m_MinIndex != this->m_CurrentSize - 1) // The pair to delete is not locate in the last position of the array.
			{
				this->swap(this->m_MinIndex, this->m_CurrentSize - 1); // Swap between nodes to shrink the array
			}

			this->m_CurrentSize--;
			if (this->m_CurrentSize != 0)
				this->m_MinIndex = findMinIndex(); // Finding new min

			return pairToDelete;
		}
		virtual void DecreaseKey(const unsigned int i_Place, const float i_NewKey) override
		{
			if (i_Place >= this->m_MaxSize || this->m_CurrIndex[i_Place] == NOT_IN_QUEUE)
				throw PriorityQueueExceptions("Invalid argument - Object doesnt exist in the queue.");

			unsigned int currentIndex = this->getCurrentIndex(i_Place); // Get current index of the object that was initial in the i_Place index
			this->m_Array[currentIndex]->m_Key = i_NewKey;

			if (currentIndex != this->m_MinIndex)
				this->m_MinIndex = findMinIndex();
		}
	};

	template <class T>
	class HeapPriorityQueue : public PriorityQueue<T>
	{
	private:
		static const unsigned int leftChild(int i_NodeIndex) { return i_NodeIndex * 2 + 1; }
		static const unsigned int rightChild(int i_NodeIndex) { return i_NodeIndex * 2 + 2; }
		static const int parent(int i_NodeIndex) { return (i_NodeIndex - 1) / 2; }
		void fixHeap(int i_NodeIndex)
		{
			unsigned int min, left, right;
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
				this->swap(i_NodeIndex, min);
				fixHeap(min);
			}

		}
	public:
		virtual void Build(Pair<T>* i_Array, const unsigned int i_Size) override
		{
			if (i_Size == 0)
				throw PriorityQueueExceptions("Invalid argument - Size cannot be 0");

			this->init(i_Array, i_Size); // INIT arrays

			for (int i = (i_Size / 2 - 1); i >= 0; i--)
				fixHeap(i);
		}
		virtual Pair<T>& DeleteMin() override
		{
			if (this->IsEmpty())
			{
				throw PriorityQueueExceptions("Quence is empty."); // Empty
			}

			Pair<T>& minPair = *this->m_Array[0];
			*this->m_PtrToCurrIndex[0] = NOT_IN_QUEUE;
			this->m_Array[0] = nullptr;
			this->m_PtrToCurrIndex[0] = nullptr;
			this->swap(0, this->m_CurrentSize - 1); // Swap with the last object on the array
			this->m_CurrentSize--;
			fixHeap(0);

			return minPair;
		}
		virtual void DecreaseKey(const unsigned int i_Place, const float i_NewKey) override
		{
			if (i_Place >= this->m_MaxSize)
				throw PriorityQueueExceptions("Invalid argument - Place out of range");

			unsigned int currentIndex = this->getCurrentIndex(i_Place); // Get the current index of the object that was in the i_Place index intial

			if (currentIndex == NOT_IN_QUEUE)
				throw PriorityQueueExceptions("Invalid argument - Object had been deleted from queue allready.");

			if (this->m_Array[currentIndex]->m_Key <= i_NewKey)
				throw PriorityQueueExceptions("New key isn't lower the original key");

			this->m_Array[currentIndex]->m_Key = i_NewKey;

			int currentParentIndex = parent(currentIndex);
			int decreaseKeyIndex = currentIndex;
			while (currentParentIndex >= 0 && this->m_Array[currentParentIndex]->m_Key > this->m_Array[decreaseKeyIndex]->m_Key)
			{
				this->swap(currentParentIndex, decreaseKeyIndex);
				decreaseKeyIndex = currentParentIndex;
				currentParentIndex = parent(decreaseKeyIndex);
			}
		}
	};
}