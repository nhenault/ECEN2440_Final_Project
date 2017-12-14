/*
 * circbuf.c
 *
 *  Created on: Nov 19, 2017
 *      Author: Nathan
 */

// INCLUDE ALL HEADER FILES
#include "msp.h"
#include "port.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"
#include "circbuf.h"
#include "functions.h"

// INCLUDE EXTRA HEADERS FOR FUNCTIONS
#include <stdlib.h>  // For malloc and free

CB_Status CB_initialize_buffer(CircBuf_t** buf, uint32_t length){
    if(!buf || !length) return CB_INIT_ERROR;

    (*buf)->buffer = (uint8_t*)malloc(sizeof(uint8_t) * length);
    (*buf)->head = (*buf)->buffer;
    (*buf)->tail = (*buf)->buffer;
    (*buf)->num_items = 0;
    (*buf)->length = length;

    if((*buf)->buffer && (*buf)->head && (*buf)->tail){
        return CB_NO_ERROR;
    }
    else {
        return CB_INIT_ERROR;
    }
}

CB_Status CB_clear_buffer(CircBuf_t* buf){
    if(!buf) return CB_CLEAR_ERROR;

    int i;
    for(i=0;i < buf->length;i++){
        buf->buffer[i] = 0;
    }
    buf->head = buf->buffer;
    buf->tail = buf->buffer;
    buf->num_items = 0;

    if(buf->head == buf->buffer && buf->tail == buf->buffer){
        return CB_NO_ERROR;
    }
    else {
        return CB_CLEAR_ERROR;
    }
}

CB_Status CB_delete_buffer(CircBuf_t* buf){
    if(!buf) return CB_DELETE_ERROR;

    free(buf->buffer);
    if(buf->buffer || buf->head || buf->tail){
        return CB_DELETE_ERROR;
    }
    else {
        return CB_NO_ERROR;
    }
}

int8_t CB_is_buffer_full(CircBuf_t* buf){
    if(!buf) return ERROR;

    if(buf->num_items == buf->length){
        return 1;
    }
    else {
        return 0;
    }
}

int8_t CB_is_buffer_empty(CircBuf_t* buf){
    if(!buf) return ERROR;

    if(buf->num_items == 0){
        return 1;
    }
    else {
        return 0;
    }
}

CB_Status CB_add_item_to_buffer(CircBuf_t* buf, uint8_t item){
    if(!buf || !item) return CB_ADD_ERROR;

    uint8_t* max_address = buf->buffer + buf->length;
    if(!CB_is_buffer_full(buf)){
        *buf->head = item;
        buf->num_items++;
        if(buf->head == max_address){
            buf->head = buf->buffer;
        }
        else {
            buf->head++;
        }

        if(buf->head){
            return CB_NO_ERROR;
        }
        else {
            return CB_ADD_ERROR;
        }
    }

    else {
        return CB_ADD_ERROR;
    }
}

uint8_t CB_remove_item_from_buffer(CircBuf_t* buf){
    if(!buf) return CB_REMOVE_ERROR;

    uint8_t ret = 0;
    uint8_t* max_address = buf->buffer + buf->length;
    if(!CB_is_buffer_empty(buf)){
        ret = *buf->tail;
        buf->num_items--;
        *buf->tail = 0;
        if(buf->tail == max_address){
            buf->tail = buf->buffer;
        }
        else {
            buf->tail++;
        }

        if(buf->tail){
            return ret;
        }
        else {
            return CB_REMOVE_ERROR;
        }
    }

    else {
        ret = CB_REMOVE_ERROR;
        return ret;
    }
}

CB_Status CB_copy_buffer(CircBuf_t* buf, char c[]){
    if(!buf || CB_is_buffer_empty(buf)) return CB_COPY_ERROR;

    int i = 0;
    uint8_t* max_address = buf->buffer + buf->length;
    uint8_t* temp = (uint8_t*)buf->tail;
    while(buf->tail != buf->head){
        c[i] = (char)*buf->tail;
        i++;
        if(buf->tail == max_address){
            buf->tail = buf->buffer;
        }
        else {
            buf->tail++;
        }
    }
    buf->tail = temp;
    return CB_NO_ERROR;
}
