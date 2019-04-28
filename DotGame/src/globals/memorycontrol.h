#ifndef __MEMORY_CONTROL_H__
#define __MEMORY_CONTROL_H__

#define GAME_NEW(objClass, arguments) new objClass arguments
#define GAME_NEW_ARRAY(objClass, num) new objClass[num]
#define GAME_DELETE(obj) delete obj
#define GAME_DELETE_ARRAY(obj) delete[]obj

#endif


