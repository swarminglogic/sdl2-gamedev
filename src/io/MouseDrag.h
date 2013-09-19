#ifndef IO_MOUSEDRAG_H
#define IO_MOUSEDRAG_H

#include <math/Point.h>
#include <math/Pointf.h>
#include <util/Log.h>


/**
 * MouseDrag class for handling:
 *    mouse-down -> drag -> mouse-up
 *
 * Simple two-state machine, IDLE, DRAGGING.
 *   startDragging should only be called when dragging (warning logged if not)
 *
 * Only has keeps track of movement since last down.
 * Use your own accumulator.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class MouseDrag
{
public:
  MouseDrag();
  virtual ~MouseDrag();


  //////////////////////
  // STATE MACHINE
  //////////////////////

  // TODO swarminglogic, 2013-09-19: Consider throwing exception instead of
  // assertions. This is because it depends on user input, perhaps mouse moves
  // out of screen, and does not register <up>.

  /**
   * Resets internals, registers starting point.
   *
   * Transitions:
   * [DRAGGING] -> [DRAGGING]  ->  ASSERT FAIL
   * [IDLE]     -> [DRAGGING]
   *
   * @param point Location where down event occured.
   */
  void down(Point point);

  /**
   * If [DRAGGING], registers current point.
   *
   * Transitions:
   * [DRAGGING] -> [DRAGGING] ->  return true
   * [IDLE]     -> [ - ]      ->  return false
   *
   * @param point Location where move event ocurred.
   * @return True if move was performed.
   */
  bool move(Point point);

  /**
   * Transitions:
   * [DRAGGING] -> [IDLE]
   * [IDLE]     -> [IDLE]      -> ASSERT FAIL
   *
   * @param point (optional) Use if final update is important.
   */
  Point up(Point point);
  void up();

  /**
   * Clears internals
   * Transitions:
   * [.] -> [IDLE]
   */
  void reset();


  ////////////
  // GETTERS
  ////////////
  /**
   * @return drag delta since last [mouse down] event.
   */
  Point get() const;
  int getX() const;
  int getY() const;

private:
  inline int deltaX() const;
  inline int deltaY() const;

  Log logger_;
  Point initial_;
  Point current_;
  bool isDragging_;

  // NonCopyable
  MouseDrag(const MouseDrag& c);
  MouseDrag& operator=(const MouseDrag& c);
};

#endif
