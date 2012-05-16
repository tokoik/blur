#ifndef OBJ_H
#define OBJ_H

/*
** 三角形分割された Alias OBJ 形式のデータ
*/
#include "Object.h"

class Obj : public Object {
public:
  Obj(void) {}
  Obj(const Obj &o) : Object(o) {}
  Obj(const char *name) { load(name); }
  virtual ~Obj(void) {}
  virtual bool load(const char *name, bool normalize = true);
};

#endif
