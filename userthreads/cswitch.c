#define _XOPEN_SOURCE 700

#include <signal.h>   
                         
#include <stdio.h>    
                        
#include <stdlib.h>   
#include <ucontext.h> 

                         
/* Stack size for each context. */
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


/* Number of iterations for functions foo() and bar(). */
#define N 15

/* Two contexts. */
ucontext_t fun1_ctx, fun2_ctx, fun3_ctx;

void funt1(){
    for (int i = 0; i < N; i++) {
    printf("fun1 (%d)\n", i);

    /* Hand over control to the bar context */
    if (swapcontext(&fun1_ctx, &fun2_ctx) < 0) {
      perror("swapcontext");
      exit(EXIT_FAILURE);
    }


}
}


void funt2(){
    for (int i = 0; i < N; i++) {

 printf("fun2 (%d)\n", i);

    /* Hand over control to the bar context */
    if (swapcontext(&fun2_ctx, &fun3_ctx) < 0) {
      perror("swapcontext");
      exit(EXIT_FAILURE);
    }
}
}


void funt3(){
    for (int i = 0; i < N; i++) {

 printf("fun3 (%d)\n", i);

    /* Hand over control to the bar context */
    if (swapcontext(&fun3_ctx, &fun1_ctx) < 0) {
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

/* Link context a to context b such that when context a returns context b is resumed.
 */
void link_context(ucontext_t *a, ucontext_t *b) {
  a->uc_link = b;
}

int main() {
  ucontext_t foo_done_ctx;

  /* Flush each printf() as it happens. */
  setvbuf(stdout, 0, _IOLBF, 0);

  /* Initialize contexts(). */
  init_context1(&foo_done_ctx, foo_done, "done", NULL);
  init_context0(&fun1_ctx, funt1, &foo_done_ctx);
  init_context0(&fun2_ctx, funt2, NULL);
  init_context0(&fun3_ctx, funt3, NULL);

  /* Transfers control to the foo context. */
  setcontext(&fun1_ctx);
  
  fprintf(stderr, "ERROR! A successful call to setcontext() does not return!\n");

    return 1;
}


 
