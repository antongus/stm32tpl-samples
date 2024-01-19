/**
*  @file SdCard.cpp
*
*  lo-level disk-io interface for fatfs
*
*  Copyright (c) 2024 by Anton Gusev
**/

#include "SdCard.h"
#include "hw.h"
#include "scmRTOS.h"

using namespace STM32::SD;

#if FF_FS_REENTRANT	/* Mutal exclusion */

static OS::TMutex ffMutexes[FF_VOLUMES + 1];

int ff_mutex_create (int ) { return 1; }
void ff_mutex_delete (int ) {}
int ff_mutex_take (int vol)	{ return ffMutexes[vol].try_lock(FF_FS_TIMEOUT); }
void ff_mutex_give (int vol) { ffMutexes[vol].unlock(); }

#endif

DSTATUS disk_initialize(BYTE drv)
{
	return disk_status(drv);
}

extern "C" DSTATUS disk_status(BYTE drv)
{
	switch(drv)
	{

	case CARD1 : // local card
		return (SdCard::getState() == CardState::Ready) ? 0 : STA_NOINIT;

	default:
		return STA_NOINIT;
	}
}

DRESULT disk_read(BYTE drv, BYTE* buff, LBA_t sector, UINT sectorCount)
{
	if (!sectorCount) return RES_PARERR;
	if (disk_status(drv) & STA_NOINIT) return RES_NOTRDY;

	switch(drv)
	{
	case CARD1 : // local card
		return (SdCard::readBuffer(buff, sector, sectorCount) == SdioError::None) ? RES_OK : RES_ERROR;

	default:
		return RES_ERROR;
	}
}

DRESULT disk_write (BYTE drv, const BYTE *buff, LBA_t sector, UINT sectorCount)
{
	if (!sectorCount) return RES_PARERR;
	DSTATUS stat = disk_status(0);
	if (stat & STA_NOINIT) return RES_NOTRDY;
	if (stat & STA_PROTECT) return RES_WRPRT;

	switch(drv)
	{
	case CARD1 : // local card
		return (SdCard::writeBuffer(buff, sector, sectorCount) == SdioError::None) ? RES_OK : RES_ERROR;

	default:
		return RES_ERROR;
	}
}

DRESULT disk_ioctl (BYTE drv, BYTE ctrl, void *buff)
{
	DRESULT res = RES_ERROR;

	if (disk_status(drv) & STA_NOINIT) return RES_NOTRDY;

	switch (ctrl) {
	case CTRL_SYNC :		// Make sure that no pending write process
		res = RES_OK;
		break;

	case GET_SECTOR_COUNT :	// Get number of sectors on the disk (DWORD)
		if (drv == CARD1)
		{
			*(DWORD*)buff = SdCard::getSectorCount();
			res = RES_OK;
		}
		break;

	case GET_SECTOR_SIZE :	// Get R/W sector size (WORD)
		if (drv == CARD1)
		{
			*(WORD*)buff = 512;
			res = RES_OK;
		}
		break;

	case GET_BLOCK_SIZE :	// Get erase block size in unit of sector (DWORD)
		if (drv == CARD1)
		{
			*(DWORD*)buff = SdCard::getBlockSize();
			res = RES_OK;
		}
		break;

	default:
		res = RES_PARERR;
		break;
	}

	return res;
}

extern "C" uint32_t get_fattime()
{
	struct tm tim;

	auto now = rtc.ReadTime();
	TimeUtil::localtime(now, &tim);

	uint32_t res =
			  ((uint32_t)(tim.tm_year - 80) << 25)
			| ((uint32_t)(tim.tm_mon + 1) << 21)
			| ((uint32_t)tim.tm_mday << 16)
			| ((uint16_t)tim.tm_hour << 11)
			| ((uint16_t)tim.tm_min << 5)
			| (tim.tm_sec >> 1);

	return res;
}

extern "C" void ff_puts(char * s)
{
	(void)s;
}


extern "C" OS_INTERRUPT void SDIO_IRQHandler()
{
	SdCard::sdioInterruptHandler();
}

extern "C" void DMA2_Stream3_IRQHandler()
{
	SdCard::dmaInterruptHandler();
}
