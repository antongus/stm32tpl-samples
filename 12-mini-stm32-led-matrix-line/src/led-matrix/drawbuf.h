/**
*  @file drawbuf.h
*
*  Created on: 27.10.2012
*  Copyright (c) Anton Gusev aka AHTOXA
**/

#ifndef DRAWBUF_H_INCLUDED
#define DRAWBUF_H_INCLUDED

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <climits>
#include "drawpoint.h"

using Point = DrawPoint<uint32_t>;

template <uint32_t width, uint32_t height>
class DrawBuffer
{
public:
	static constexpr uint32_t Width() { return width; }
	static constexpr uint32_t Height() { return height; }

	void SetPixel(Point point, bool on = true);
	void ClearPixel(Point point) { SetPixel(point, false); }
	bool GetPixel(Point point);

	uint8_t GetVByte(Point point);
	uint8_t GetHByte(Point point);

	void SetWritePos(Point const& point);
	Point const& GetWritePos() { return writePos_; }

	void SetReadPos(Point const& point);
	Point const& GetReadPos() { return readPos_; }

	void PutChar(Point& point, char ch);
	void PutTwoChars(Point& point, char ch1, char ch2, int step);
	void PutString(Point& point, const char* s);
	void PutColumn(Point& point, uint8_t b);

	void ShiftRight();

	void Clear()
	{
		memset(buf_, 0, sizeof(buf_));
	}
	static constexpr uint32_t bitsPerItem = sizeof(uint32_t) * CHAR_BIT;          // how much bits can hold one buffer item
	static constexpr size_t bufWidth = (width + bitsPerItem - 1) / bitsPerItem; // width in uint32_t words
	static constexpr size_t bufHeight = height;
	uint32_t buf_[bufHeight][bufWidth];
private:
	Point writePos_ { 0, 0 };
	Point readPos_ { 0, 0 };

	/// get reference to word in buffer for point given
	uint32_t& Buf(Point const& point)
	{
		return buf_[point.row][point.col / bitsPerItem];
	}

	void ValidatePoint(Point& point);
};

template<uint32_t width, uint32_t height>
void DrawBuffer<width, height>::ValidatePoint(Point& point)
{
	point.col %= width;
	point.row %= height;
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::SetPixel(Point point, bool on)
{
	point += writePos_;
	if (point.row < height && point.col < width)
	{
		auto const mask = 1UL << (point.col % bitsPerItem);
		if (on)
			Buf(point) |= mask;
		else
			Buf(point) &= ~mask;
	}
}

template <uint32_t width, uint32_t height>
bool DrawBuffer<width, height>::GetPixel(Point point)
{
	point += readPos_;
	ValidatePoint(point);
	auto const mask = 1UL << (point.col % bitsPerItem);
	return Buf(point) & mask;
}

/**
 * Get byte of sequential dots, starting from given point.
 * dots are taken in vertical direction.
 * @param point
 * @return byte of points
 */
template <uint32_t width, uint32_t height>
uint8_t DrawBuffer<width, height>::GetVByte(Point point)
{
	point += readPos_;
	ValidatePoint(point);
	auto const mask = 1UL << (point.col % bitsPerItem);
	point.col /= bitsPerItem;
	uint32_t res = 0;
	for (auto i = 0; i < 8; ++i)
	{
		if (Buf(point) & mask) res |= 1u << i;
		++point.row;
	}
	return res;
}

/**
 * Get one byte of sequential dots, starting from given point.
 * dots are taken in horizontal direction.
 * @param point
 * @return byte of points
 */
template <uint32_t width, uint32_t height>
uint8_t DrawBuffer<width, height>::GetHByte(Point point)
{
	point += readPos_;
	ValidatePoint(point);

	unsigned ret {0};
	for (auto bit = 0u; bit < 8; ++bit)
	{
		if (GetPixel(point))
			ret |= 1u << bit;
		++point.col;
	}
	return ret;

	auto mask = 1ul << (point.col % bitsPerItem);
	point.col /= bitsPerItem;
	uint32_t res = 0;
	for (auto i = 0; i < 8; ++i)
	{
		if (Buf(point) & mask) res |= 1u << i;
		++point.col;
	}
	return res;
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::SetWritePos(Point const& point)
{
	writePos_ = point;
	ValidatePoint(writePos_);
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::SetReadPos(Point const& point)
{
	readPos_ = point;
	ValidatePoint(readPos_);
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::ShiftRight()
{
	++readPos_.col;;
	ValidatePoint(readPos_);
}

extern const char font7x5_ru[];

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::PutColumn(Point& point, uint8_t b)
{
	uint32_t mask = 1 << 7;
	for (auto row = 0u; row < 8; ++row)
	{
		SetPixel(Point(point.col, point.row + row), b & mask);
		mask >>= 1;
	}
}

static int charPos(char ch)
{
	const char russianA = 0xC0;
	if (ch >= russianA) ch -= (russianA - 0x80);
	ch -= 0x20;
	int pos = ch * 5;
	return pos;
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::PutChar(Point& point, char ch)
{
	int pos = charPos(ch);
	for (ch = 5; ch; ch--)
	{
		PutColumn(point, font7x5_ru[pos++]);
		++point.col;
	}
	PutColumn(point, 0);
	++point.col;
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::PutTwoChars(Point& point, char ch1, char ch2, int step)
{
	int pos1 = charPos(ch1);
	int pos2 = charPos(ch2);
	for (int i = 5; i; i--)
	{
		char b1 = font7x5_ru[pos1++];
		char b2 = font7x5_ru[pos2++];
		char b = (b2 >> step) | (b1 << (8-step));
		PutColumn(point, b);
		++point.col;
	}
	PutColumn(point, 0);
	++point.col;
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::PutString(Point& point, const char* s)
{
	while (*s)
		PutChar(point, *s++);
}

#endif // DRAWBUF_H_INCLUDED
