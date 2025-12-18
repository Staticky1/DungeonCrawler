#pragma once


#include "Math.h"


#define _VECTOR_DEFAULT_TOLERANCE 0.001f
#define _VECTOR_NORMALISED_TOLERANCE 0.01f

//
// A templated 2d vector class 
//
template <typename T> class TVector2D
{

public:

	T X;
	T Y;

	//Default Contstructor, No initialization
	__forceinline TVector2D() {};

	//Constructor with an X and Y Value
	TVector2D(T InX, T InY);

	//Constructor with one value
	TVector2D(T In);

	__forceinline ~TVector2D() {};

	// Adds one TVector to another of the same type, returns the resulting vector
	TVector2D<T> operator+(const TVector2D<T>& InV) const;

	//Adds a single value to each of the components of the vector, returns result
	template<typename T>
	inline TVector2D<T> operator+(const T& InBias) const
	{
		return TVector2D<T>(X + InBias, Y + InBias);
	}

	//Subtaces the components of two vectors and returns the result
	TVector2D<T> operator-(const TVector2D<T>& InV) const;

	//Subtracts a single value from each component of a vector
	template<typename T>
	inline TVector2D<T> operator-(const T& InBias) const
	{
		return TVector2D<T>(X - InBias, Y - InBias);
	}

	//Gets the result of scaling the vector, multiplys each component by a value.
	template<typename T>
	inline TVector2D<T> operator*(const T& InScale) const
	{
		return TVector2D<T>(X * InScale, Y * InScale);
	}

	//gets result of dividing each vector component by a value
	template<typename T>
	inline TVector2D<T> operator/(const T& InScale) const
	{
		const T RScale = T(1) / InScale;
		return TVector2D<T>(X * RScale, Y * RScale);
	}

	//Multiplys this vector by another vector, returns the result
	TVector2D<T> operator*(const TVector2D<T>& InV) const;

	//Divides this vector by another
	TVector2D<T> operator/(const TVector2D<T>& InV) const;

	//Calculates the dot product between this vector and another
	T operator|(const TVector2D<T>& InV) const;

	// Comparison operations

	//check if a vector is equal to another
	bool operator==(const TVector2D<T>& InV) const;

	//check if a vector is inequal with another
	bool operator!=(const TVector2D<T>& InV) const;

	//check if a vector is equal to this within a tolerance limit
	bool Equals(const TVector2D<T>& InV, T Tolerance = _VECTOR_DEFAULT_TOLERANCE) const;

	//Adds another vector to this vector
	TVector2D<T> operator+=(const TVector2D<T>& InV);

	//Subtracts another vector from this vector
	TVector2D<T> operator-=(const TVector2D<T>& InV);

	//Scales this vector by a value
	__forceinline TVector2D<T> operator*=(const T& Scale)
	{
		
		X *= Scale; Y *= Scale;
		return *this;
	}

	//Divides this vector by a value
	__forceinline TVector2D<T> operator/=(const T& Scale)
	{
		const T RV = (T)1 / Scale;
		X *= RV; Y *= RV;
		return *this;
	}

	//Multiplys this vector by another vector, mulitplys each component together
	TVector2D<T> operator*=(const TVector2D<T>& InV);

	//Divides this vector by another vector, divides each component together
	TVector2D<T> operator/=(const TVector2D<T>& InV);


	//Functions

public:

	//Sets the values of the vector directly
	void Set(T InX, T InY);

	//Returns the max value of the vector components
	T GetMax() const;

	//Returns the minimum values of this vectors components
	T GetMin() const;

	//Gets a copy of the vector with absolute values
	TVector2D<T> GetAbs() const;

	//Get the magnitude of this vector
	T Magnitude() const;

	//Gets the squared magitude of this vector
	T MagnitudeSquared() const;

	//Check if a vector if all the componets of this vector are 0
	bool IsZero() const;

	//Check if a vectors components are all zero, within a tolerance
	bool IsNearlyZero(T Tolerance = _VECTOR_DEFAULT_TOLERANCE) const;

	//Check if a vector is normalised
	bool IsNormalised() const;

	//Normalise this vector if it is larger than the tolerance, otherwise don't change it
	bool Normalise(T Tolerance = _VECTOR_DEFAULT_TOLERANCE);

	//Returns a normalised verson of this vector
	TVector2D<T> GetNormalised(T Tolerance = _VECTOR_DEFAULT_TOLERANCE);

	//Distance between two points
	static __forceinline T Distance(const TVector2D<T>& Vector1, const TVector2D<T>& Vector2);

	static __forceinline T DistanceSquared(const TVector2D<T>& Vector1, const TVector2D<T>& Vector2);

	//returns a value between two vectors
	TVector2D<T> Lerp(TVector2D<T>& V, float Alpha);
};

template<typename T>
inline TVector2D<T>::TVector2D(T InX, T InY)
{
	X = InX;
	Y = InY;
}

template<typename T>
inline TVector2D<T>::TVector2D(T In)
{
	X = In; Y = In;
}

template<typename T>
inline TVector2D<T> TVector2D<T>::operator+(const TVector2D<T>& InV) const
{
	return TVector2D<T>(X + InV.X, Y + InV.Y);
}

template<typename T>
inline TVector2D<T> TVector2D<T>::operator-(const TVector2D<T>& InV) const
{
	return TVector2D<T>(X - InV.X, Y - InV.Y);
}

template<typename T>
inline TVector2D<T> TVector2D<T>::operator*(const TVector2D<T>& InV) const
{
	return TVector2D<T>(X * InV.X, Y * InV.Y);
}

template<typename T>
inline TVector2D<T> TVector2D<T>::operator/(const TVector2D<T>& InV) const
{
	return TVector2D<T>(X / InV.X, Y / InV.Y);
}

template<typename T>
inline T TVector2D<T>::operator|(const TVector2D<T>& InV) const
{
	return X * InV.X + Y * InV.Y;
}

template<typename T>
inline bool TVector2D<T>::operator==(const TVector2D<T>& InV) const
{
	return X == InV.X && Y == InV.Y;
}

template<typename T>
inline bool TVector2D<T>::operator!=(const TVector2D<T>& InV) const
{
	return X!=InV.X || Y!=InV.Y;
}

template<typename T>
inline bool TVector2D<T>::Equals(const TVector2D<T>& InV, T Tolerance) const
{
	return std::abs(X-InV.X) <= Tolerance && std::abs(Y-InV.Y) <= Tolerance;
}

template<typename T>
inline TVector2D<T> TVector2D<T>::operator+=(const TVector2D<T>& InV)
{
	X += InV.X; Y += InV.Y;
	return *this;
}

template<typename T>
inline TVector2D<T> TVector2D<T>::operator-=(const TVector2D<T>& InV)
{
	X -= InV.X; Y -= InV.Y;
	return *this;
}

template<typename T>
inline TVector2D<T> TVector2D<T>::operator*=(const TVector2D<T>& InV)
{
	X *= InV.X; Y *= InV.Y;
	return *this;
}

template<typename T>
inline TVector2D<T> TVector2D<T>::operator/=(const TVector2D<T>& InV)
{
	X /= InV.X; Y /= InV.Y;
	return *this;
}

template<typename T>
inline void TVector2D<T>::Set(T InX, T InY)
{
	X = InX; Y = InY;
}

template<typename T>
inline T TVector2D<T>::GetMax() const
{
	return FMath::Max(X,Y);
}

template<typename T>
inline T TVector2D<T>::GetMin() const
{
	return FMath::Min(X,Y);
}

template<typename T>
inline TVector2D<T> TVector2D<T>::GetAbs() const
{
	return TVector2D<T>(FMath::Abs(X),FMath::Abs(Y));
}

template<typename T>
inline T TVector2D<T>::Magnitude() const
{
	return FMath::Sqrt(X*X + Y*Y);
}

template<typename T>
inline T TVector2D<T>::MagnitudeSquared() const
{
	return X*X + Y*Y;
}

template<typename T>
inline bool TVector2D<T>::IsZero() const
{
	return X == 0.f && Y == 0.f;
}

template<typename T>
inline bool TVector2D<T>::IsNearlyZero(T Tolerance) const
{
	return FMath::Abs(X) <= Tolerance && FMath::Abs(Y) <= Tolerance;
}

template<typename T>
inline bool TVector2D<T>::IsNormalised() const
{
	return FMath::Abs(1.f - MagnitudeSquared()) < _VECTOR_NORMALISED_TOLERANCE;
}

template<typename T>
inline bool TVector2D<T>::Normalise(T Tolerance)
{
	const T SquareMag = X * X + Y * Y;
	if (SquareMag > Tolerance)
	{
		const T Scale = FMath::Sqrt(SquareMag);
		X /= Scale; Y /= Scale;
		return true;
	}
	
	return false;
}

template<typename T>
inline TVector2D<T> TVector2D<T>::GetNormalised(T Tolerance)
{
	const T SquareMag = X * X + Y * Y;
	if (SquareMag == 1.f)
	{
		return *this;
	}
	else if (SquareMag < Tolerance)
	{
		return TVector2D<T>();
	}

	const T Scale = (T)FMath::InvSqrt(SquareMag);
	return TVector2D<T>(X *= Scale, Y *= Scale);
	
}

template<typename T>
inline T TVector2D<T>::Distance(const TVector2D<T>& Vector1, const TVector2D<T>& Vector2)
{
	return FMath::Sqrt(TVector2D<T>::DistanceSquared(Vector1,Vector2));
}

template<typename T>
inline T TVector2D<T>::DistanceSquared(const TVector2D<T>& Vector1, const TVector2D<T>& Vector2)
{
	return FMath::Square(Vector2.X - Vector1.X) + FMath::Square(Vector2.Y - Vector1.Y);
}

template<typename T>
inline TVector2D<T> TVector2D<T>::Lerp(TVector2D<T>& V, float Alpha)
{
	return TVector2D<T>(X,Y) * (1.f - Alpha) + V * Alpha;
}








	
