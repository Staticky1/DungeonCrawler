#pragma once

#include "TArray.h"
#include <cstdlib>

//Generic Math functions

struct FMath
{
	// Returns higher value two values
	template<class T>
	static constexpr __forceinline T Max(const T A, const T B)
	{
		return (B < A) ? A : B;
	}

	// Returns lower value two values
	template<class T>
	static constexpr __forceinline T Min(const T A, const T B)
	{
		return (A < B) ? A : B;
	}

	//Returns the Max value from an array
	// @param	Array containing values
	// @param	Optional pointer for returning the value of the max element
	// @return	The max value found in the array, or default if the array is empty
	template<class T>
	static __forceinline T Max(const TArray<T>& Array, int* MaxIndex = NULL)
	{
		if (Array.Num() == 0)
		{
			if (MaxIndex)
			{
				*MaxIndex = -1;
			}
			return T();
		}

		T CurrentMax = Array[0];
		int CurrentMaxIndex = 0;
		for (int Index = 0; Index < Array.Num(); Index++)
		{	
			const T Value = Array[Index];
			if (Value > CurrentMax)
			{
				CurrentMax = Value;
				CurrentMaxIndex = Index;
			}
		}

		if (MaxIndex)
		{
			*MaxIndex = CurrentMaxIndex;
		}
		return CurrentMax;
	}

	//Returns the Minimum value from an array
	// @param	Array containing values
	// @param	Optional pointer for returning the value of the min element
	// @return	The max value found in the array, or default if the array is empty
	template<class T>
	static __forceinline T Min(const TArray<T>& Array, int* MinIndex = NULL)
	{
		if (Array.Num() == 0)
		{
			if (MinIndex)
			{
				*MinIndex = -1;
			}
			return T();
		}

		T CurrentMin = Array[0];
		int CurrentMinIndex = 0;
		for (int Index = 0; Index < Array.Num(); Index++)
		{
			const T Value = Array[Index];
			if (Value < CurrentMin)
			{
				CurrentMin = Value;
				CurrentMinIndex = Index;
			}
		}

		if (MinIndex)
		{
			*MinIndex = CurrentMinIndex;
		}
		return CurrentMin;
	}

	//get an absolute value
	template< class T >
	static constexpr __forceinline T Abs(const T A)
	{
		return (A < (T)0) ? -A : A;
	}

	//Returns the square root of a value
	static __forceinline float Sqrt(float Value) { return sqrtf(Value); }
	static __forceinline double Sqrt(double Value) { return sqrt(Value); }
	static __forceinline int Sqrt(int Value) { return (int)sqrt(Value); }

	//Returns the inverse square root
	static __forceinline float InvSqrt(float F) { return 1.0f / sqrtf(F); }
	static __forceinline double InvSqrt(double F) { return 1.0 / sqrt(F); }
	static __forceinline int InvSqrt(int F) { return int(1.0 / sqrt(F)); }

	//Multiplys a value by itself
	template <class T>
	static constexpr __forceinline T Square(const T A)
	{
		return A * A;
	}
};