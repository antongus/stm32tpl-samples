/**
*  @file Trace.h
*
*  Журнал отладочных сообщений в ОЗУ.
*
*  Copyright 2023 Anton B. Gusev
**/

#pragma once

#include "micro_format.hpp"
#include "scmRTOS.h"
#include "textstream.h"

extern TextStream& debug;

/// флаги трассировки
struct Trace
{
	static constexpr auto Config        {1u << 0};   //!< события чтения/записи конфигурации
	static constexpr auto Uplink        {1u << 1};   //!< события в канале связи с ПК
	static constexpr auto MbTransport   {1u << 2};   //!< события в каналах связи MODBUS
	static constexpr auto MbProto       {1u << 3};   //!< события в протоколе обмена с MODBUS-датчиками
	static constexpr auto Sdio          {1u << 4};   //!< события в обмене с SD-картой
	static constexpr auto DataQueue     {1u << 5};   //!< события в очереди данных

	static constexpr auto flags { 0
		| 0 * Config
		| 0 * Uplink
		| 0 * MbTransport
		| 0 * MbProto
		| 1 * Sdio
		| 1 * DataQueue
	};
};


namespace tracer
{

/**
 * Пустой трассировщик (для релизных сборок).
 */
struct NoTrace
{
	static inline void clear() {}
	static inline const char* buffer()
	{
		static char buf[] {""};
		return buf;
	};

	template<typename ... Args>
	static inline void print(const char* , const Args&... ) {}
};

/**
 * Небольшой журнал в ОЗУ
 */
template <size_t bufSize>
class MemoryTrace
{
public:
	static inline const char* buffer() { return m_buffer; }
	static inline void clear()
	{
		m_len = 0;
		m_buffer[0] = '\0';
	}

	template <typename ... Args>
	static inline void print(const char* format, const Args& ... args);

private:
	static inline size_t m_len {0};
	static inline char m_buffer[bufSize];

	template <typename ... Args>
	friend void trace(const char* format, const Args& ... args);
};

/**
 * Трассировщик в debug порт
 */
struct DebugPortTrace
{
	static inline void clear() {}
	static inline const char* buffer()
	{
		static char buf[] {""};
		return buf;
	};

	template<typename ... Args>
	static inline void print(const char* format, const Args&... args)
	{
		mf::format([](void* , char character){
			::debug.PutChar(character);
			return true;
		}, nullptr, format, args...);
	}
};


/// выбор типа трассировщика
//using TraceType = MemoryTrace<2048>;
using TraceType = DebugPortTrace;
//using TraceType = NoTrace;

extern TraceType debugTrace;

template<size_t bufSize>
template<typename ... Args>
void MemoryTrace<bufSize>::print(const char* format, const Args&... args)
{
	mf::format([](void* , char character){
		if (m_len < bufSize - 1)
		{
			m_buffer[m_len++] = character;
			m_buffer[m_len] = 0;
		}
		return true;
	}, nullptr, format, args...);
}

template <typename ... Args>
inline void trace(const char* format, const Args& ... args)
{
	TraceType::print(format, args...);
}

inline char const* result() { return TraceType::buffer(); }
inline void clear() { TraceType::clear(); }

}
