#ifndef IPC_UTILS_H_
#define IPC_UTILS_H_
#include <stdio.h>
#include <c6x.h>
#include <ti/csl/csl.h>

/* Global print level */
#define DEBUGLEVEL  0

/* Number of slaves to use */
#define NUMSLAVES   4

/* IPC print level */
#define IPC_DEBUGLEVEL   DEBUGLEVEL

/* Enable asserts */
#define IPC_ASSERTS

/* Use CSL library or assembly function to get core number */
#define USECSLDNUM
//#define USEMULTIPROC

#ifdef USEMULTIPROC
#undef USECSLDNUM
#endif

/***********************************************************************
* Custom memory/cache configuration:                                   *
* For the purpose of IPC, we will place some flags in MSMCSRAM.        *
* Since caching of MSMCSRAM can not be disabled and since we           *
* would like to avoid writing-back/invalidating the cache each time    *
* we consult a flag, we create a virtual, non-cached SRAM with virtual *
* addresses lying outside of the physical address ranges of the        *
* DSP. We then map the virtual address space back to a chunk of the    *
* physical memory and disable caching for the virtual SRAM.            *
*                                                                      *
* Related Documents:                                                   *
* SPRS691E, physical address space (section 2.3)                       *
* SPRUGWOC, memory attribute registers (MAR) (section 4.4)             *
* SPRUGY8, cache control and functions                                 *
*                                                                      *
* Note: For this to work, a platform package must be created including *
* 1.) A virtual memory space of length MSMCSRAM_NC_SIZE and start      *
*     address MSMCSRAM_NC_START_ADDR. This space must fall within      *
*     one or several MARs (modify config_cache()).                     *
* 2.) A physical address space with length MSMCSRAM_NC_SIZE and start  *
*     address MSMCSRAM_START_ADDR.                                     *
* 3.) Place section .ipc_nocache_section in the virtual memory space.  *
************************************************************************/

/* These addresses must match the platform package definitions */
#define MSMCSRAM_START_ADDR     (0x0C000000) // start MSMCSRAM_NC_PHY
#define MSMCSRAM_NC_SIZE        (0x00020000) // length MSMCSRAM_NC_PHY matching length MSMCSRAM_NC_VIRT
#define MSMCSRAM_SIZE           (0x00400000) // total length MSMCSRAM
#define MSMCSRAM_NC_START_ADDR  (0xA0000000) // start MSMCSRAM_NC_VIRT

/* Memory Attribute Register for which we config the cache */
#define MAR_MSMCSRAM_START    12
#define MAR_MSMCSRAM_END      12
#define MAR_NC_START          160
#define MAR_NC_END            161
#define MAR_CACHE_DDR3_START  128
#define MAR_CACHE_DDR3_END    135

/* L2 cache mode */
#define IPC_L2MODE CACHE_0KCACHE

/* Must be called by master and slaves before any other function,
 * maps  MSMCSRAM_NC_VIRT to MSMCSRAM_NC_PHY.
 * Note: This function invalidates all caches after configuration. */
void config_cache(Uint32 selfId);


/***************************************************
* Inter-Processor Communication - GAME RULES       *
* Master:                                          *
* - On every ipc_master_set_req(_slave_i),         *
*    an ipc_master_wait_ack(_slave_i) MUST follow  *
*                                                  *
* Slaves:                                          *
* - On every ipc_slave_wait_req,                   *
*    an ipc_slave_set_ack MUST follow              *
****************************************************/

/*******************
* MASTER FUNCTIONS *
********************/
void ipc_master_init(void); // Must be called first
void ipc_master_wait_for_slaves_after_init(void); // Can be called after ipc_master_init and ipc_slave_init
void ipc_master_set_req(uint8_t req_val);
void ipc_master_set_req_slave_i(uint8_t req_val, Uint32 slaveId);
void ipc_master_wait_ack(void); // calls ipc_master_reset_ack() on return
void ipc_master_wait_ack_slave_i(Uint32 slaveId); // calls ipc_master_reset_ack_slave_i(slaveId) on return
void ipc_master_reset_ack(void);
void ipc_master_reset_ack_slave_i(Uint32 slaveId);
void ipc_master_reset_req(void);


/******************
* SLAVE FUNCTIONS *
*******************/
void ipc_slave_init(Uint32 volatile selfId); // Must be called first
void ipc_slave_init2(void); // selfId extracted automatically
void ipc_slave_wait_req(void); // calls ipc_slave_reset_req() on return
void ipc_slave_reset_req(void);
void ipc_slave_set_ack(uint8_t ack_val); // increases ncalls
int ipc_slave_get_num_requests(void); // no protection from int overflow
void ipc_slave_reset_num_requests(void); // sets ncalls to 0
int ipc_slave_get_selfId(void); // get the selfId used by IPC


#endif /* IPC_UTILS_H_ */
