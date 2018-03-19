#include <vector>

#include "amx/amx.h"
#include "plugincommon.h"

class Natives
{
public:

	static cell AMX_NATIVE_CALL Dialog_Open(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL Dialog_Show(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL Dialog_EnableFix(AMX *amx, cell *params);

	static void RegisterNatives(AMX *amx);

};