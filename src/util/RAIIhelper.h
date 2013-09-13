#ifndef UTIL_SDLHELPER_H
#define UTIL_SDLHELPER_H

/**
 * RAII helper macro wrappers using std::unique_pointer
 *
 * example:
 *   CREATE_RAII(MyClass, destructorFunction) MyClassPtr
 *   Creates a typedef of MyClassPtr, which is a unique_ptr<MyClass>,
 *   that class destructorFunction upon deletion.
 *
 * @author SwarmingLogic
 */

#define CREATE_RAII(OBJ, DEST)                           \
  struct OBJ ## Deleter {                                \
    void operator()( OBJ* ptr ) { if (ptr) DEST( ptr );} \
  };                                                     \
  typedef std::unique_ptr<OBJ, OBJ ## Deleter>


#endif
