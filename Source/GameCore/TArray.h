#pragma once

#include <algorithm>
#include <iostream>

//	TArray class
//	Holds a refrence to an array of a type, can add, remove and query the array dynamiclly

template <typename T> class TArray
{

private:


public:

	TArray();
	TArray(T* InArray, int ArraySize);

	TArray(std::initializer_list<T> List);

	~TArray();


	T* ArrayPointer = nullptr;
	int ArrayLength = 0;

	//gets a reference to the item in the array at a supplied index
	T GetElement(int Index) const;

	//adds an element to the array
	void AddElement(const T& Element);

	//adds an elemt to the arry only if the element is not already in the array, returns true if the item was added
	bool AddUnique(T& Element);

	//adds multiple elements to an array
	void Append(T* Elements, int Count);
	void Append(TArray<T>* Elements);

	//Remove a specific item from the array, returns true if an item is removed, has the option to remove all of a matching element from an array, rather than just the first instance
	bool RemoveElement(const T& ElementToRemove, bool bOnlyRemoveFirst = true);

	//Removes the item at an index, returns true in an item is removed
	bool RemoveIndex(int IndexToRemove);
	
	//Returns true in an index is within an array
	bool IsValidIndex(int IndexToCheck) const;

	//Checks if an elemnt is part on the array
	bool Contains(const T& Element) const;

	//Returns the index of an element in the array, returns -1 if the element is not found
	int Find(const T& Element) const;

	//Returns the lenght of the array
	int Num() const;

	//Clears the array, setting it's length to zero and removing any content
	void Clear();

	//Swap two elements in the array
	//void Swap(T ElementA, T ElementB);

	//Swap the values of two indexs of the array
	void Swap(int IndexA, int IndexB);
	
	 // Array bracket operator. Returns reference to element at given index.
	 //	returns Reference to indexed element.
 
	__forceinline T operator[](int Index)
	{
		return GetElement(Index);
	}

	//Returns a random element of the array
	T RandomElement() const;

};

//Default constructor, creates a TArray class holding an empty templated array
template<typename T>
inline TArray<T>::TArray()
{
	ArrayPointer = new T[0];
}

template<typename T>
inline TArray<T>::TArray(T* InArray, int ArraySize)
{
	ArrayPointer = new T[0];
	Append(InArray, ArraySize);
}

template<typename T>
inline TArray<T>::TArray(std::initializer_list<T> List)
{
	ArrayPointer = new T[0];
	for (auto Element : List)
	{
		AddElement(Element);
	}
	
}

//Destructor that delets the held array from memory
template<typename T>
inline TArray<T>::~TArray()
{
	if (ArrayPointer)
	{
		delete [] ArrayPointer;
	}
}

template<typename T>
inline T TArray<T>::GetElement(int Index) const
{
	try
	{
		if (IsValidIndex(Index))
		{
			return ArrayPointer[Index];
		}
		else
		{
			throw 100;
		}
	}
	catch (...)
	{
		std::cout << "ERROR: Invalid Array Index Accessed!";
		return ArrayPointer[Index]; //Could return junk data
	}
}

template<typename T>
inline void TArray<T>::AddElement(const T& Element )
{

	//Create a copy of the array with a size one larger
	T* TempArray = new T[ArrayLength + 1];
	std::copy(ArrayPointer, ArrayPointer + ArrayLength, TempArray);

	//Delete the old array and replace it with the new array
	delete[] ArrayPointer;
	ArrayPointer = TempArray;

	//Set the last index of the array to the added element
	ArrayPointer[Num()] = Element;

	ArrayLength++;
	
}

template<typename T>
inline bool TArray<T>::AddUnique(T& Element)
{
	if (Contains(Element))
	{
		//Element is already in the array
		return false;
	}
	else
	{
		AddElement(Element);
		return true;
	}
}

template<typename T>
inline void TArray<T>::Append(T* Elements, int Count)
{

	//Create a copy of the array with a new size
	T* TempArray = new T[Num() + Count];
	std::copy(ArrayPointer, ArrayPointer + Num(), TempArray);

	int ArrayStartingLenght = Num();

	//Delete the old array and replace it with the new array
	delete[] ArrayPointer;
	ArrayPointer = TempArray;

	//add the elements from the array input
	for (int ElementIndex = ArrayStartingLenght; ElementIndex < ArrayStartingLenght + Count; ElementIndex++)
	{
		ArrayPointer[ElementIndex] = Elements[ElementIndex - ArrayStartingLenght];
		ArrayLength++;
	};

}

//Overload of append to take a TArray pointer
template<typename T>
inline void TArray<T>::Append(TArray<T>* Elements)
{
	Append(Elements->GetArrayPointer(),Elements->Num());
}

template<typename T>
inline bool TArray<T>::RemoveElement(const T& ElementToRemove, bool bOnlyRemoveFirst)
{
	int ElementIndex = Find(ElementToRemove);

	if (IsValidIndex(ElementIndex))
	{
		RemoveIndex(ElementIndex);
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
inline bool TArray<T>::RemoveIndex(int IndexToRemove)
{
	if (IsValidIndex(IndexToRemove))
	{
		T LastItem = ArrayPointer[Num() - 1];

		//replace the index of the item to remove with the last item in the array
		ArrayPointer[IndexToRemove] = LastItem;

		T* TempArr = new T [Num() - 1];
		std::copy(ArrayPointer, ArrayPointer + Num() - 1, TempArr);

		delete[] ArrayPointer;
		ArrayPointer = TempArr;

		ArrayLength--;

		return true;
	}
	else
	{
		return false;
	}

}

template<typename T>
inline bool TArray<T>::IsValidIndex(int IndexToCheck) const
{
	if (IndexToCheck > -1 && IndexToCheck < Num())
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
inline bool TArray<T>::Contains(const T& Element) const
{
	if (Find(Element) != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
inline int TArray<T>::Find(const T& Element) const
{
	for (int ElementIndex = 0; ElementIndex < Num(); ElementIndex++)
	{
		if (ArrayPointer[ElementIndex] == Element)
		{
			return ElementIndex;
		}
	}

	return -1;
}

template<typename T>
inline int TArray<T>::Num() const
{
	return ArrayLength;
}

template<typename T>
inline void TArray<T>::Clear()
{
	ArrayLength = 0;
	delete[] ArrayPointer;
	ArrayPointer = new T[0];
}
/*
template<typename T>
inline void TArray<T>::Swap(T ElementA, T ElementB)
{
	int IndexA = Find(ElementA);
	int IndexB = Find(ElementB);

	if (IndexA != -1 && IndexB != -1)
	{
		ArrayPointer[IndexA] = ElementB;
		ArrayPointer[IndexB] = ElementA;
	}
}
*/
template<typename T>
inline void TArray<T>::Swap(int IndexA, int IndexB)
{
	T ElementA = ArrayPointer[IndexA];
	T ElementB = ArrayPointer[IndexB];

	ArrayPointer[IndexA] = ElementB;
	ArrayPointer[IndexB] = ElementA;

}

template<typename T>
inline T TArray<T>::RandomElement() const
{
	int RandomIndex = std::rand() % Num();
	return GetElement(RandomIndex);
}
