#ifndef _LALEMA_STRUCT_H_
#define _LALEMA_STRUCT_H_
typedef struct Point_ {
  int x, y;
} Point;

static bool arePointsEqual(struct Point_ p1, struct Point_ p2) {
    return p1.x == p2.x && p1.y == p2.y;
}
static struct Point_ newPoint(int x, int y) {
  Point p;
  p.x = x;
  p.y = y;
  return p;
}

#endif