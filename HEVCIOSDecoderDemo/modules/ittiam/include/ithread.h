/******************************************************************************
*
* Copyright (C) 2012 Ittiam Systems Pvt Ltd, Bangalore
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************/
/**
*******************************************************************************
* @file
*  ithread.h
*
* @brief
*  This file contains all the necessary structure and  enumeration
* definitions needed for the Application  Program Interface(API) of the
* Thread Abstraction Layer
*
* @author
*  Harish
*
* @remarks
*  None
*
*******************************************************************************
*/
#ifndef __ITHREAD_H__
#define __ITHREAD_H__

#ifdef _MSC_VER
	#define THREADAPI __declspec(dllexport)
#else
	#define THREADAPI
#endif

UWORD32 THREADAPI ithread_get_handle_size(void);

UWORD32 THREADAPI ithread_get_mutex_lock_size(void);

WORD32  THREADAPI ithread_create(void *thread_handle, void *attribute, void *strt, void *argument);

void    THREADAPI ithread_exit(void *val_ptr);

WORD32  THREADAPI ithread_join(void *thread_id, void **val_ptr);

WORD32  THREADAPI ithread_get_mutex_struct_size(void);

WORD32  THREADAPI ithread_mutex_init(void *mutex);

WORD32  THREADAPI ithread_mutex_destroy(void *mutex);

WORD32  THREADAPI ithread_mutex_lock(void *mutex);

WORD32  THREADAPI ithread_mutex_unlock(void *mutex);

void    THREADAPI ithread_yield(void);

void    THREADAPI ithread_sleep(UWORD32 u4_time);

void    THREADAPI ithread_msleep(UWORD32 u4_time_ms);

void    THREADAPI ithread_usleep(UWORD32 u4_time_us);

UWORD32 THREADAPI ithread_get_sem_struct_size(void);

WORD32  THREADAPI ithread_sem_init(void *sem, WORD32 pshared, UWORD32 value);

WORD32  THREADAPI ithread_sem_post(void *sem);

WORD32  THREADAPI ithread_sem_wait(void *sem);

WORD32  THREADAPI ithread_sem_destroy(void *sem);

WORD32 THREADAPI ithread_set_affinity(WORD32 core_id);
#endif /* __ITHREAD_H__ */
