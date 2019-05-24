/**
*  @file cdcacm.h
*
*  USB CDC interface functions
*
*  Copyright © 2016 Anton B. Gusev aka AHTOXA
**/

#ifndef CDCACM_H_INCLUDED
#define CDCACM_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

// Functions from cdcacm.c that application should call to work with USB CDC

/// USB CDC initialization. Sh
void CdcAcmInit();
/// return 1 if USB connected
int  CdcAcmIsConfigured();
/// Send data through USB
int  CdcAcmSendData(char * buf, int size);
/// USB poll function. Should be called periodically. Possible from USB interrupt.
void CdcAcmPoll();

// functions that should be defined in application

/// This function is called when data received from USB CDC
void CdcAcmDataReceived(uint8_t * buf, size_t size);
/// This function is called when SOF received from USB
void CdcAcmSofReceived();
/// This function is called when reset received from USB
void CdcResetEvent();
/// This function is called when suspend received from USB
void CdcSuspendEvent();
/// This function is called when resume received from USB
void CdcResumeEvent();

/// This function is called to fill device serial number.
/// Function must fill buffer with textual representation
/// of serial number. Max 8 chars + zero termination symbol.
void FillSerialNumber(char* buf);

#ifdef __cplusplus
}
#endif

#endif // CDCACM_H_INCLUDED
