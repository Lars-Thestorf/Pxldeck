#ifndef _LALEMA_GAMESAPI_H_
#define _LALEMA_GAMESAPI_H_

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
} LaLeMaGame;

#ifdef __cplusplus
}
#endif

#endif

