#ifndef HLM_GAMESAPI_H_
#define HLM_GAMESAPI_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  char name[20];
  void* (*setupFunction)(void);
  void (*loopFunction)(void*);
  void (*freeFunction)(void*);
} HLM_game;

#ifdef __cplusplus
}
#endif

#endif

 
