#include <unordered_map>
#include <list>
#include <string.h>

#include "amx/amx.h"
#include "plugincommon.h"

#include "urmem.hpp"

using namespace std;

class Dialog
{
public:

	struct AmxListItem;
	struct DialogMapItem;

	using DialogMap = unordered_map<int, DialogMapItem>;
	using AmxList = list<AmxListItem>;
	
	struct Callbacks {
		bool exists;
		int id;
	};

	struct DialogMapItem {
		int funcidx;
		string dialog;
	};

	struct AmxListItem
	{
		AMX *amx;
		DialogMap _dialog_map;
		Callbacks _on_dialog_performed, _on_dialog_received, _on_dialog_response;
	};

	static void Init(AMX *amx);

	static void Open(AMX *amx, int playerid, const char *dialog_name);
	static void Show(AMX *amx, int playerid, const char *dialog_name);
	static void EnableFix();

	static void OnDialogResponse(int playerid, int dialogid, int response, int listitem, char *inputtext);

private:

	static bool strdel(char *cstr, int from, int to)
	{
		int length = strlen(cstr);
		int index = from;
		int offs = to - index;

		if (index >= length || offs <= 0) {
			return false;
		}
		if (index + offs > length) {
			offs = length - index;
		}

		index--;
		do {
			index++;
			cstr[index] = cstr[index + offs];
		} while (cstr[index] != 0);

		return true;
	}

	static bool isGamemode(AMX *amx)
	{
		int idx;
		if (amx_FindPublic(amx, "OnFilterScriptInit", &idx) == AMX_ERR_NONE) return false;
		return true;
	}
};