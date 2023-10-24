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
         scheduler_fnc_t fnc;
         void *arg;
         struct thread* next;
         };
 static struct {
    struct thread* head;
    struct thread* curr;
    jmp_buf ctx;  /*main program context*/
    } state;
    
struct thread* new_thread = NULL;
  struct thread*  current = NULL;

int scheduler_create(scheduler_fnc_t fnc, void *arg) {   /*1*/
    
    /*initialize thread object*/
    new_thread = (struct thread*)malloc(sizeof(struct thread*));
    new_thread->status = STATUS_INIT;
    new_thread->next= NULL;
    new_thread->fnc = fnc;
    new_thread->arg = arg;
    new_thread->stack.memory = (void*)malloc(STACK_SIZE);
   
    if (state.head == NULL) {
        state.head = new_thread;
        state.curr = state.head;
    } else {
    struct thread* current = state.head;
        while (current->next!= NULL) {
            current = current->next;
        }
      fnc(arg);
}
  return 0;
}

void scheduler_yield(void) {  
        
        setjmp(state.curr->ctx);
        state.curr->status = STATUS_SLEEPING;
        if (!state.ctx) {
            longjmp(state.ctx, 1);
        }
    }

void destroy() {
        while (state.head!= NULL) {
        struct thread* curr= state.head->next;
        FREE(curr->stack.memory);
        FREE(curr->stack.memory_);
        FREE(curr);
        }
        
        FREE(state.curr->stack.memory);
        FREE(state.curr->stack.memory_);
        FREE(state.curr);
        
        FREE(state.head->stack.memory);
        FREE(state.head->stack.memory_);
        FREE(state.head);
    }

struct thread* thread_candidate(void) {
  
  if(state.head == NULL)
      return NULL;
      
  current = state.curr;
  while(current!=NULL){
      if(current->status == STATUS_INIT || current->status == STATUS_SLEEPING) {
          return current;
      }
      if(current->next == NULL)
         current = state.head;
       else
          current = current->next;
          
       if(current == state.curr && current-> status == STATUS_TERMINATED)
         return NULL;
  }
  
  return NULL;
  }

void schedule(void) {

  struct thread* candidate = thread_candidate();
     
   if(candidate != NULL && candidate->status == STATUS_SLEEPING){
        state.curr = candidate;
        candidate->status = STATUS_RUNNING;
        longjmp(candidate->ctx, 1);
   }
  
   else {
   exit(0);
        
   }
}

void scheduler_execute(void) {

  setjmp(state.ctx);
  schedule();
  destroy();
 }


