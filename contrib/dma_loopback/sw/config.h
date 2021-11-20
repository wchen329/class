// Greg Stitt
// University of Florida

#ifndef __CONFIG_H__
#define __CONFIG_H__

//=============================================================
// Configuration settings

// Change "volatile int" to any data type you want to be stored in the input
// and output arrays. Note that removal of the volatile qualifier requires
// modification of main.cpp to use AFU::mallocNonvolatile(), which isn't 
// guaranteed to work.
typedef volatile int dma_data_t;

// Other examples:
//typedef volatile double dma_data_t;
//typedef volatile unsigned char dma_data_t;
// typedef int dma_data_t; // Must use AFU::mallocNonvolatile()
// You can also use struct and class types, but will have to change the 
// initialization and verification code in main.cpp.

// When simulating, there is a loop that does nothing but wait for the DMA 
// to finish. This constant "polling" is very inefficient and can slow down 
// the CPU. Defining this flag causes the processor to periodically sleep
// during this polling.
// NOTE: For execution on the FPGA, comment this out.
#define SLEEP_WHILE_WAITING

// The number of milliseconds to sleep when SLEEP_WHILE_WAITING is defined.
const unsigned SLEEP_MS = 10;


//=============================================================
// AFU MMIO Addresses

enum MmioAddr {
   MMIO_GO = 0x0050,
   MMIO_BASE_ADDR_S0 = 0x0052,
   MMIO_BASE_ADDR_S1 = 0x0054,
   MMIO_BASE_ADDR_S2 = 0x0056,
   MMIO_BASE_ADDR_S3 = 0x0058,
   MMIO_CV = 0x005A
};

#endif
