/* On Mac OS (aka OS X) the ucontext.h functions are deprecated and requires the
   following define.
*/
#define _XOPEN_SOURCE 700

/* On Mac OS when compiling with gcc (clang) the -Wno-deprecated-declarations
   flag must also be used to suppress compiler warnings.
*/

#include <signal.h>   /* SIGSTKSZ (default stack size), MINDIGSTKSZ (minimal
                         stack size) */
#include <stdio.h>    /* puts(), printf(), fprintf(), perror(), setvbuf(), _IOLBF,
                         stdout, stderr */
#include <stdlib.h>   /* exit(), EXIT_SUCCESS, EXIT_FAILURE, malloc(), free() */
#include <ucontext.h> /* ucontext_t, getcontext(), makecontext(),
                         setcontext(), swapcontext() */
#include <stdbool.h>  /* true, false */

#include "sthreads.h"

/* Stack size for each context. */
#define STACK_SIZE SIGSTKSZ

struct thread t;
struct thread n;
ucontext_t fun1_ctx, fun2_ctx;

int threadID = 1;
//threadID = 1;

/*******************************************************************************
                             Global data structures

                Add data structures to manage the threads here.
********************************************************************************/




/*******************************************************************************
                             Auxiliary functions

                      Add internal helper functions here.
********************************************************************************/




/*******************************************************************************
                    Implementation of the Simple Threads API
********************************************************************************/

int listchecknext(struct thread *cur){

if(cur->next->tid == -1){
  return -1;
  }else{return 1;}

}

void cur_threadsetup(struct thread *cur){
ucontext_t tempctx;

cur->tid = threadID;
cur->state = ready;
cur->ctx = tempctx;
threadID++;

//return *cur;

}

void next_threadsetup(struct thread *next){
ucontext_t tempctx2;

next->tid = -1;
next->state = ready;
next->ctx = tempctx2;

///return *next;

}

int  init(){




  //ucontext_t tempctx;
  ucontext_t nxtctx;
  ucontext_t foo_done_ctx;

  
  //t = malloc(sizeof(thread));
  t.tid = threadID;
  t.state = ready;
  //t.ctx = tempctx;
  n.tid = -1;
  t.next = &n;
  threadID++;
  //cur_threadsetup(&t);
  //next_threadsetup(&n);
  
  if(listchecknext(&t) == -1){
    printf("Next is empty");
    


  }else{
    printf("Next is NOT empty");

  }

  //init_context1(&foo_done_ctx, foo_done, "done", NULL);
  //init_context0(&fun1_ctx, funt1, &foo_done_ctx);
  //init_context0(&fun2_ctx, funt2, NULL);

  printf("\nTID: %d\n",t.tid);
  //setcontext(&fun1_ctx);
  
  return 1;
}

void funt1(){
    for (int i = 0; i < 15; i++) {
    printf("fun1 (%d)\n", i);

    /* Hand over control to the bar context */
    if (swapcontext(&fun1_ctx, &fun2_ctx) < 0) {
      perror("swapcontext");
      exit(EXIT_FAILURE);
    }


}
}
void funt2(){
    for (int i = 0; i < 15; i++) {

 printf("fun2 (%d)\n", i);

    /* Hand over control to the bar context */
    if (swapcontext(&fun2_ctx, &fun1_ctx) < 0) {
      perror("swapcontext");
      exit(EXIT_FAILURE);
    }
}
}

void foo_done(char *msg) {
  printf("foo  -  %s!\n", msg);
}
void init_context(ucontext_t *ctx, ucontext_t *next) {
  /* Allocate memory to be used as the stack for the context. */
  void *stack = malloc(STACK_SIZE);

  if (stack == NULL) {
    perror("Allocating stack");
    exit(EXIT_FAILURE);
  }

  if (getcontext(ctx) < 0) {
    perror("getcontext");
    exit(EXIT_FAILURE);
  }

  /* Before invoking makecontext(ctx), the caller must allocate a new stack for
     this context and assign its address to ctx->uc_stack, and define a successor
     context and assigns address to ctx->uc_link.
  */

  ctx->uc_link           = next;
  ctx->uc_stack.ss_sp    = stack;
  ctx->uc_stack.ss_size  = STACK_SIZE;
  ctx->uc_stack.ss_flags = 0;
}

void init_context0(ucontext_t *ctx, void (*func)(), ucontext_t *next) {
  init_context(ctx, next);
  makecontext(ctx, func, 0);
}

/* Initialize context ctx with a call to function func with one string argument.
 */
void init_context1(ucontext_t *ctx, void (*func)(), const char *str, ucontext_t *next) {
  init_context(ctx, next);
  makecontext(ctx, func, 1, str);
}


void link_context(ucontext_t *a, ucontext_t *b) {
  a->uc_link = b;
}

tid_t spawn(void (*start)()){
  return -1;
}

void yield(){
}

void  done(){
}

tid_t join() {
  return -1;
}


int main(){


for(int i = 0; i < 10; i++){
  init();
  printf("cur tid: %d state: %c next tid:%d\n",t.tid,t.state,t.next->tid);
}

  return 0;
}