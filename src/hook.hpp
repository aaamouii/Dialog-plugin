#include <memory>

#include "urmem.hpp"

using namespace std;

#ifdef _WIN32
	#define THISCALL __thiscall
#else
	#define THISCALL
#endif

class Hook
{
public:

	using hook = shared_ptr<urmem::hook>;

	static bool Init();
	static void UnInit();
	static int THISCALL HOOK_OnDialogResponse(void *_this, int playerid, int dialogid, int response, int listitem, char *inputtext);

};