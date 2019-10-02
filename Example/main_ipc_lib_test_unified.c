#include <stdio.h>
#include <c6x.h>
#include <assert.h>
#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_cache.h>
#include <ti/csl/csl_cacheAux.h>

#include "ipc_utils.h"

#define TIMINGLEVEL 1

void simpleDelay(void);
void init_timer(void);
void assert_int(int volatile lval, int volatile rval, const char* name);

/* Slave i data */
#pragma DATA_ALIGN(selfId, CACHE_L1D_LINESIZE)
#pragma DATA_SECTION(selfId, ".local_data")
Uint32 volatile selfId;

void main_func(void) {
    int i, t0, t1, round_i;
    const int NUM_ROUNDS = 10;
    selfId = CSL_chipReadReg(CSL_CHIP_DNUM);
    assert(selfId <= NUMSLAVES);

#if DEBUGLEVEL >= 2
    printf("selfId = %d\n", selfId);
#endif

    config_cache(selfId);

    /* Initialise variables and sync */
    if (selfId == 0) { // MASTER

        printf("Initializing\n");
        ipc_master_init();
        printf("Waiting for slaves\n");
        ipc_master_wait_for_slaves_after_init();

        /* Call slaves one-by-one */
        for (i = 1; i <= NUMSLAVES; i++) {
            printf("Calling slave %d\n", i);
            simpleDelay();
            ipc_master_set_req_slave_i(1, i); // 1
            simpleDelay();
            ipc_master_wait_ack_slave_i(i); // 1
        }

        /* Synchronise all slaves */
        ipc_master_set_req(1); // 2
        ipc_master_wait_ack(); // 2

    } else { // SLAVE
        assert(selfId >= 1);
        assert(selfId <= NUMSLAVES);

        /* Initialize IPC framework */
        ipc_slave_init2();
        assert_int(selfId, ipc_slave_get_selfId(), "selfId-ipcId");

        /* Being called one-by-one */
        ipc_slave_wait_req(); // 1
        ipc_slave_set_ack(1); // 1

        /* Sync call */
        ipc_slave_wait_req(); // 2
        ipc_slave_set_ack(1); // 2
    }

    /***************************************************************
     **************************** MASTER ***************************
     ***************************************************************/
    if (selfId == 0) { // MASTER
#if DEBUGLEVEL >= 2
        printf("selfId = %d <- master\n", selfId);
#endif
        /* Wait a bit */
        simpleDelay();
#if TIMINGLEVEL >= 1
        init_timer();
#endif

#pragma UNROLL(1)
        for (round_i = 0; round_i < NUM_ROUNDS; round_i++) {
#if TIMINGLEVEL >= 1
            t0 = _itoll(TSCH, TSCL);
#endif
#if DEBUGLEVEL >= 3
            printf("m: requesting from slaves at round = %d, NUMSLAVES = %d\n", round_i, NUMSLAVES);
#endif
            /***********************
             * REQUEST from slaves *
             ***********************/
            ipc_master_set_req(1); // 3

            /****************************
             * Wait for ACKNOWLEDGEMENT *
             ****************************/
            ipc_master_wait_ack(); // 3

#if DEBUGLEVEL >= 4
            printf("master: received acknowledgement at round = %d\n", round_i);
#endif

#if TIMINGLEVEL >= 1
            t1 = _itoll(TSCH, TSCL);
            printf("round_%d: %d cycles (NUMSLAVES=%d)\n", round_i, (t1-t0), NUMSLAVES);
#endif
        }
    /***************************************************************
     **************************** SLAVES ***************************
     ***************************************************************/
    } else { // SLAVE
        ipc_slave_reset_num_requests();
        assert(ipc_slave_get_num_requests() == 0);
#if DEBUGLEVEL >= 2
        printf("selfId = %d <- slave, numrequests=%d\n", selfId, ipc_slave_get_num_requests());
#endif
        do {
            /********************
             * Wait for REQUEST *
             ********************/
            ipc_slave_wait_req(); // 3

            /***********************
             * ACKNOWLEDGE request *
             ***********************/
            ipc_slave_set_ack(1); // 3

        } while (ipc_slave_get_num_requests() < NUM_ROUNDS);
#if DEBUGLEVEL >= 1
        printf("slave_%d: Terminating after %d rounds\n", selfId, ipc_slave_get_num_requests());
#endif
        simpleDelay();
    }

    if (selfId == 0) { // MASTER
        /* Call slaves one by one before terminating */
        for (i = 1; i <= NUMSLAVES; i++) {
            printf("Calling slave %d\n", i);
            simpleDelay();
            ipc_master_set_req_slave_i(1, i); // 4
            simpleDelay();
            ipc_master_wait_ack_slave_i(i); // 4
        }
#if DEBUGLEVEL >= 0
            printf("Terminating test\n");
#endif
    } else {
        /* Wait until being called */
        ipc_slave_wait_req(); // 4
#if DEBUGLEVEL >= 0
        printf("slave %d reached the end!\n", (int)selfId);
#endif
        ipc_slave_set_ack(1); // 4
    }
}

int main(void) {
    main_func();
    return 0;
}


/******************************************************************************
 * Other Utils
 *****************************************************************************/

void init_timer() {
    TSCL = 1;
}


void simpleDelay() {
    const int n = 100000;
    int i;
    float a = 2, b = 4, c;
    for (i = 0; i < n; i++)
        c = a + b;
    a = b - c;
}


void init_real_vec(float *in, const int len, const float value) {
    int i;
    for (i = 0; i < len; i++) {
       in[i] = value;
    }
}


void init_real_vec_v(float volatile *in, const int len, const float value) {
    int i;
    for (i = 0; i < len; i++) {
       in[i] = value;
    }
}



float calc_error(const float *in1, const float *in2, const int len) {
    int i;
    float error = 0;
    for (i = 0; i < len; i++) {
        error += (in1[i] - in2[i]) * (in1[i] - in2[i]);
    }
    return error;
}


void init_cplx_vec(float *in, const int len, const float value) {
    int i;
    for (i = 0; i < len; i++) {
        if (value >= 0) {
            in[2 * i] = value;
            in[2 * i + 1] = value;
        } else {
            in[2 * i] = (float)i;
            in[2 * i + 1] = (float)i;
        }
    }
}


void init_cplx_vec_volatile(volatile float *in, const int len, const float value) {
    int i;
    for (i = 0; i < len; i++) {
        if (value >= 0) {
            in[2 * i] = value;
            in[2 * i + 1] = value;
        } else {
            in[2 * i] = (float)i;
            in[2 * i + 1] = (float)i;
        }
    }
}


void print_vec_cplx(const float *vec, const int len, const char *name) {
    int i;
    printf("%s = [", name);
    for (i = 0; i < len; i++) {
        if (i == len - 1) {
            printf("%.8f+%.8fim", vec[2 * i], vec[2 * i + 1]);
        } else {
            printf("%.8f+%.8fim,", vec[2 * i], vec[2 * i + 1]);
        }
    }
    printf("]\n");
}


void print_cplx_matrix(const float *mat, const int nrows, const int ncols, const char *name) {
    int i, j, ind;
    printf("%s = [", name);
    for (i = 0; i < nrows; i++) {
        if (i > 0) {
            printf("     ");
        }
        for (j = 0; j < ncols; j++) {
            ind = i * ncols + j;
            if ((j == ncols - 1) && (i == nrows - 1)) {
                printf("%.3f+%.3fim", mat[2*ind], mat[2*ind+1]);
            } else {
                printf("%.3f+%.3fim,", mat[2*ind], mat[2*ind+1]);
            }
        }
        if (i == nrows -1) {
            printf("]\n");
        } else {
            printf("\n");
        }
    }
}


void init_mat_by_row_num(float *in, const int nrows, const int ncols) {
    int i, j;
    for (i = 0; i < nrows; i++) {
        for (j = 0; j < ncols; j++) {
            in[i * ncols + j] = (float)(i);
        }
    }
}


void init_mat_by_row_num_v(float volatile *in, const int nrows, const int ncols) {
    int i, j;
    for (i = 0; i < nrows; i++) {
        for (j = 0; j < ncols; j++) {
            in[i * ncols + j] = (float)(i);
        }
    }
}


void init_mat_by_row_num_v_2(float volatile *in, const int nrows, const int ncols, const float offset) {
    int i, j;
    for (i = 0; i < nrows; i++) {
        for (j = 0; j < ncols; j++) {
            in[i * ncols + j] = (float)(i) + offset;
        }
    }
}


void assert_float(float lval, float rval, const char* name) {
    if (lval != rval) {
        printf("Assertion %s failed. lval=%f, rval=%f\n",
               name, lval, rval);
        assert(lval == rval);
    }
}


void assert_int(int volatile lval, int volatile rval, const char* name) {
    if (lval != rval) {
        printf("Assertion %s failed. lval=%d, rval=%d\n",
               name, lval, rval);
        assert(lval == rval);
    }
}
