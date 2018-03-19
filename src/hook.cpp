#include "main.hpp"

typedef void(*logprintf_t)(char* format, ...);
extern logprintf_t logprintf;

#ifdef _WIN32

const char *pattern =
"\x83\xEC\x08\x53\x8B\x5C\x24\x20" \
"\x55\x8B\x6C\x24\x20\x56\x33\xF6" \
"\x57\x8B\xF9\x89\x74\x24\x10\xEB" \
"\x07";

const char *mask = "xxxxxxxxxxxxxxxxxxxxxxxxx";

#else

const char *pattern =
"\x55\x89\xE5\x57\x56\x53\x83\xEC" \
"\x2C\x8B\x75\x08\xC7\x45\xE4\x00" \
"\x00\x00\x00\x8B\x7D\x14\x89\xF3" \
"\xEB\x14";

const char *mask = "xxxxxxxxxxxxxxxxxxxxxxxxxx";

#endif

Hook::hook _on_dialog_response_hook;

bool Hook::Init()
{
	urmem::sig_scanner scanner;

	if (scanner.init(urmem::get_func_addr(logprintf))) {
		urmem::address_t addr;

		if (scanner.find(pattern, mask, addr)) {
			_on_dialog_response_hook = make_shared<urmem::hook>(addr, urmem::get_func_addr(&Hook::HOOK_OnDialogResponse));

			return true;
		}
	}
	return false;
}

void Hook::UnInit()
{
	_on_dialog_response_hook.reset();
}

int THISCALL Hook::HOOK_OnDialogResponse(void *_this, int playerid, int dialogid, int response, int listitem, char *inputtext)
{
	if (dialogid == 32700) {
		Dialog::OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
	}
	return 1;
}
