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

#define STACK_SIZE 1048576


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
         
         struct thread* next;
         };
 static struct {
    struct thread* head;
    struct thread* curr;
    jmp_buf ctx;  //main program context
    } state;
    
struct thread* new_thread = NULL;

int scheduler_create(scheduler_fnc_t fnc, void *arg) {   //1
    
    //initialize thread object
    new_thread = (struct thread*)malloc(sizeof(struct thread*));
    new_thread->status = STATUS_INIT;
    new_thread->next= NULL;
    new_thread->stack->memory = (void*)malloc(STACK_SIZE);
    num_threads++;
   
    if (state.head == NULL) {
        state.head = new_thread;
        state.curr = state.head;
    } else {
    struct thread* current = head;
        while (current->next!= NULL) {
            current = current->next;
        }
        lcurrent->next = new_thread;
    }
}

void scheduler_yield(void) { //1 
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

void destroy() {
    if (state.curr!= NULL) {
        struct thread* prev_thread = NULL;
        struct thread* current = state.head;
        while (current != state.curr) {
            prev_thread = current;
            current = current->next;
        }

        if (prev_thread != NULL) {
            prev_thread->next = current->next;
        } else {
            head = current->next;
        }
        
        FREE(current->stack->memory);
        FREE(current->stack->memory_);
        FREE(current);

    }
}

struct thread* thread_candidate(void) {
  
  if(state.head == NULL)
      return NULL;
      
  struct thread* current = state.curr;
  while(current!=NULL){
      if(current->status == STATUS_INIT || current->status == STATUS_SLEEPING) {
          return current;
      }
      if(current->next == NULL)
         current = head;
       else
          current = current->next;
          
       if(current == state.curr && current-> status == STATUS_TERMINATED)
         return NULL;
  }
  
  return NULL;
  }

void schedule(void) {
 
  state.curr->status = STATUS_SLEEPING;
  struct thread* candidate = thread_candidate();
  
  if(candidate == NULL)
     return NULL;
     
   if(candidate->status == STATUS_SLEEPING){
        state.curr = candidate;
        candidate-> STATUS_RUNNING;
        longjmp(candidate->ctx, 1);
   }
  
   else {
        
   }
}

