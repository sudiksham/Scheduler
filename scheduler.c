/**
 * Tony Givargis
 * Copyright (C), 2023
 * University of California, Irvine
 *
 * CS 238P - Operating Systems
 * scheduler.c
 */

#undef _FORTIFY_SOURCE

#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include "system.h"
#include "scheduler.h"
#define MAX_THREADS 50


/**
 * Needs:
 *   setjmp()
 *   longjmp()
 */

/* research the above Needed API and design accordingly */


struct thread{
     jmp_buf ctx;
     enum {
         STATUS_INIT,
         STATUS_RUNNING,
         STATUS_SLEEPING,
         STATUS_TERMINATED
         }status;
     struct {
         void *memory_;
         void *memory;
         }stack;
         scheduler_fnc_t function;
         void *arg;
         struct thread* next;
         };

struct thread* new_thread = NULL;
struct thread* head = NULL;
struct thread* current_thread = NULL;
struct thread* prev_thread = NULL;
struct thread* last = NULL;
int num_threads = 0;




int scheduler_create(scheduler_fnc_t fnc, void *arg) {
      if (num_threads >= MAX_THREADS) {
        fprintf(stderr, "Maximum number of threads reached\n");
        return -1;
    }
    
    new_thread = (struct thread*)malloc(sizeof(struct thread*));
    new_thread->status = STATUS_INIT;
    new_thread->next= NULL;
    new_thread->function = fnc;
    new_thread->arg = arg;
    num_threads++;

    if (head == NULL) {
        head = new_thread;
        current_thread = head;
    } else {
        last = head;
        while (last->next!= NULL) {
            last = last->next;
        }/**
 * Tony Givargis
 * Copyright (C), 2023
 * University of California, Irvine
 *
 * CS 238P - Operating Systems
 * scheduler.c
 */

#undef _FORTIFY_SOURCE

#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include "system.h"
#include "scheduler.h"
#define MAX_THREADS 50


/**
 * Needs:
 *   setjmp()
 *   longjmp()
 */

/* research the above Needed API and design accordingly */


struct thread{
     jmp_buf ctx;
     enum {
         STATUS_INIT,
         STATUS_RUNNING,
         STATUS_SLEEPING,
         STATUS_TERMINATED
         }status;
     struct {
         void *memory_;
         void *memory;
         }stack;
         scheduler_fnc_t function;
         void *arg;
         struct thread* next;
         }thread;

struct thread* new_thread = NULL;
struct thread* head = NULL;
struct thread* current_thread = NULL;
struct thread* prev_thread = NULL;
struct thread* last = NULL;
int num_threads = 0;




int scheduler_create(scheduler_fnc_t fnc, void *arg) {
      if (num_threads >= MAX_THREADS) {
        fprintf(stderr, "Maximum number of threads reached\n");
        return -1;
    }
    
    new_thread = (struct thread*)malloc(sizeof(struct thread*));
    new_thread->status = STATUS_INIT;
    new_thread->next= NULL;
    new_thread->function = fnc;
    new_thread->arg = arg;
    num_threads++;

    if (head == NULL) {
        head = new_thread;
        current_thread = head;
    } else {
        last = head;
        while (last->next!= NULL) {
            last = last->next;
        }
        last->next = new_thread;
    }

    if (setjmp(new_thread->ctx) == 0) {
        fnc(arg);
        new_thread->status = STATUS_TERMINATED;
        scheduler_yield();
    }
    
   return 0;
}

void scheduler_yield(void) {
      if (current_thread != NULL) {
        current_thread->status = STATUS_SLEEPING;
        prev_thread = current_thread;
        current_thread = current_thread->next;

        while (current_thread == NULL || current_thread->status == STATUS_TERMINATED) {
            if (current_thread == NULL) {
                current_thread = head;
            } else {
                current_thread = current_thread->next;
            }
        }

        current_thread->status = STATUS_RUNNING;

        if (setjmp(prev_thread->ctx) == 0) {
            longjmp(current_thread->ctx, 1);
        }
    }
}

void scheduler_execute(void) {
     if (current_thread != NULL) {
        if (setjmp(current_thread->ctx) == 0) {
            longjmp(current_thread->ctx, 1);
        }
        schedule();
        destroy();
        return 0;
    }
    return -1;
}

void destroy() {
    if (current_thread != NULL) {
        thread* prev_thread = NULL;
        thread* current = head;
        while (current != current_thread) {
            prev_thread = current;
            current = current->next;
        }

        if (prev_thread != NULL) {
            prev_thread->next = current->next;
        } else {
            head = current->next;
        }

        free(current);
        num_threads--;

        if (current_thread == current) {
            current_thread = NULL;
        }
    }
}

void schedule(void) {
}

struct thread* thread_candidate(void) {

}
     
        last->next = new_thread;
    }

    if (setjmp(new_thread->ctx) == 0) {
        fnc(arg);
        new_thread->status = STATUS_TERMINATED;
        scheduler_yield();
    }
    
   return 0;
}

void scheduler_yield(void) {
      if (current_thread != NULL) {
        current_thread->status = STATUS_SLEEPING;
        prev_thread = current_thread;
        current_thread = current_thread->next;

        while (current_thread == NULL || current_thread->status == STATUS_TERMINATED) {
            if (current_thread == NULL) {
                current_thread = head;
            } else {
                current_thread = current_thread->next;
            }
        }

        current_thread->status = STATUS_RUNNING;

        if (setjmp(prev_thread->ctx) == 0) {
            longjmp(current_thread->ctx, 1);
        }
    }
}

void scheduler_execute(void) {


}
     
