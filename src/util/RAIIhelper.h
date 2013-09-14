#ifndef UTIL_RAIIHELPER_H
#define UTIL_RAIIHELPER_H

#include <memory>


/**
 * RAII helper macro wrappers using std::unique_pointer
 *
 * example:
 *   CREATE_RAII(MyClass, destructorFunction) MyClassPtr
 *   Creates a typedef of MyClassPtr, which is a unique_ptr<MyClass>,
 *   that class destructorFunction upon deletion.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */

#define CREATE_RAII(OBJ, DEST)                           \
  struct OBJ ## Deleter {                                \
    void operator()( OBJ* ptr ) { if (ptr) DEST( ptr );} \
  };                                                     \
  typedef std::unique_ptr<OBJ, OBJ ## Deleter>


#endif
