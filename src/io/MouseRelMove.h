#ifndef IO_MOUSERELMOVE_H
#define IO_MOUSERELMOVE_H

#include <math/Point.h>
#include <math/Pointf.h>
#include <util/Log.h>


/**
 * MouseRelMove class for constant capture mouse movement.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class MouseRelMove
{
public:
  MouseRelMove();
  virtual ~MouseRelMove();

  void move(Point point);
  Point reset();

  Point get() const;
  int getX() const;
  int getY() const;

private:
  Log log_;
  Point accum_;

  // NonCopyable
  MouseRelMove(const MouseRelMove& c);
  MouseRelMove& operator=(const MouseRelMove& c);
};

#endif
