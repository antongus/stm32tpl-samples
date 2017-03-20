/**
*  @file drawpoint.h
*
*  DrawPoint class template
*
*  Copyright © 2017 Anton B. Gusev aka AHTOXA
**/


#ifndef DRAWPOINT_H_INCLUDED
#define DRAWPOINT_H_INCLUDED


template<typename T>
struct DrawPoint
{
	T col;
	T row;

	constexpr DrawPoint(T const& c = T(), T const& r = T())
		: col(c)
		, row(r)
	{}

	constexpr DrawPoint(const DrawPoint<T>& other) = default;

	DrawPoint<T>& operator+=(const DrawPoint<T>& other)
	{
		col += other.col;
		row += col.row;
		return *this;
	}
	DrawPoint<T>& operator-=(const DrawPoint<T>& other)
	{
		col -= other.col;
		row -= col.row;
		return *this;
	}
};


template<typename T>
inline DrawPoint<T> operator+(const DrawPoint<T>& p1, const DrawPoint<T>& p2)
{
	DrawPoint<T> ret { p1 };
	ret += p2;
	return ret;
}

#endif // DRAWPOINT_H_INCLUDED
