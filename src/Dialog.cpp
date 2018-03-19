#include "main.hpp"

typedef void(*logprintf_t)(char* format, ...);
extern logprintf_t logprintf;

Dialog::AmxList _amx_list;
Dialog::Callbacks _on_dialog_received, _on_dialog_performed;

bool gFix_Enabled = false;

void Dialog::Init(AMX *amx)
{
	Dialog::AmxListItem _list_item{ amx };
	if (amx_FindPublic(amx, "OnDialogReceived", &_list_item._on_dialog_received.id) == AMX_ERR_NONE) {
		_list_item._on_dialog_received.exists = true;
	}
	if (amx_FindPublic(amx, "OnDialogPerformed", &_list_item._on_dialog_performed.id) == AMX_ERR_NONE) {
		_list_item._on_dialog_performed.exists = true;
	}
	if (isGamemode(amx)) {
		_amx_list.push_front(_list_item);
	} else {
		if (amx_FindPublic(amx, "OnDialogResponse", &_list_item._on_dialog_response.id) == AMX_ERR_NONE) {
			_list_item._on_dialog_response.exists = true;
		}
		_amx_list.push_back(_list_item);
	}
}

void Dialog::Show(AMX *amx, int playerid, const char *dialog_name)
{
	string func_name = "dcr_";
	func_name.insert(4, dialog_name);
	
	int idx;
	if (amx_FindPublic(amx, func_name.c_str(), &idx) == AMX_ERR_NONE) {
		amx_Push(amx, playerid);
		amx_Exec(amx, nullptr, idx);
		return;
	}

	logprintf("[Dialog] Dialog_Show: cannot find dialog \"%s\"", dialog_name);
}

void Dialog::Open(AMX *amx, int playerid, const char *dialog_name)
{
	string DialogFunction = "dre_";
	DialogFunction.insert(4, dialog_name);
	
	int idx;
	if (amx_FindPublic(amx, DialogFunction.c_str(), &idx) == AMX_ERR_NONE) {
		const auto amx_iter = find_if(_amx_list.begin(), _amx_list.end(), [amx](const Dialog::AmxListItem &item) {
			return item.amx == amx;
		});

		if (amx_iter != _amx_list.end()) {
			Dialog::DialogMap::const_iterator iter = amx_iter->_dialog_map.find(playerid);
			if (iter != amx_iter->_dialog_map.end()) amx_iter->_dialog_map.erase(iter);

			Dialog::DialogMapItem _new_dialog{ idx,dialog_name };
			amx_iter->_dialog_map.insert(pair<int, Dialog::DialogMapItem>(playerid, _new_dialog));
		}
		return;
	}

	logprintf("[Dialog] Dialog_Open: cannot find dialog \"%s\"", dialog_name);
}

void Dialog::EnableFix()
{
	gFix_Enabled = true;
}

void Dialog::OnDialogResponse(int playerid, int dialogid, int response, int listitem, char *inputtext)
{
	if (gFix_Enabled) {
		for (auto i = strlen(inputtext); --i != -1;) {
			if (inputtext[i] == '%') inputtext[i] = '#';
			else if (inputtext[i] == '{' && inputtext[i + 7] == '}') strdel(inputtext, i, i + 8);
		}
	}

	Dialog::DialogMap::const_iterator dialog_iter;
	cell addr_params, dialog_addr, retval = -1;
	
	for (const auto &iter : _amx_list) {
		if (iter._on_dialog_response.exists) {
			amx_PushString(iter.amx, &addr_params, nullptr, inputtext, NULL, NULL);
			amx_Push(iter.amx, listitem);
			amx_Push(iter.amx, response);
			amx_Push(iter.amx, dialogid);
			amx_Push(iter.amx, playerid);
			amx_PushString(iter.amx, &dialog_addr, nullptr, dialog_iter->second.dialog.c_str(), NULL, NULL);
			amx_Exec(iter.amx, &retval, iter._on_dialog_response.id);
			amx_Release(iter.amx, addr_params);
			amx_Release(iter.amx, dialog_addr);
		}

		if ((dialog_iter = iter._dialog_map.find(playerid)) != iter._dialog_map.end()) {
			if (iter._on_dialog_received.exists) {
				amx_PushString(iter.amx, &addr_params, nullptr, inputtext, NULL, NULL);
				amx_Push(iter.amx, listitem);
				amx_Push(iter.amx, response);
				amx_Push(iter.amx, playerid);
				amx_PushString(iter.amx, &dialog_addr, nullptr, dialog_iter->second.dialog.c_str(), NULL, NULL);
				amx_Exec(iter.amx, &retval, iter._on_dialog_received.id);
				amx_Release(iter.amx, addr_params);
				amx_Release(iter.amx, dialog_addr);

				if (!retval) return;
			}
			
			amx_PushString(iter.amx, &addr_params, nullptr, inputtext, NULL, NULL);
			amx_Push(iter.amx, listitem);
			amx_Push(iter.amx, response);
			amx_Push(iter.amx, playerid);
			amx_PushString(iter.amx, &dialog_addr, nullptr, dialog_iter->second.dialog.c_str(), NULL, NULL);
			amx_Exec(iter.amx, &retval, dialog_iter->second.funcidx);
			amx_Release(iter.amx, addr_params);
			amx_Release(iter.amx, dialog_addr);
		}

		if (iter._on_dialog_performed.exists) {
			amx_Push(iter.amx, retval);
			amx_PushString(iter.amx, &addr_params, nullptr, inputtext, NULL, NULL);
			amx_Push(iter.amx, listitem);
			amx_Push(iter.amx, response);
			amx_Push(iter.amx, playerid);
			amx_PushString(iter.amx, &dialog_addr, nullptr, dialog_iter->second.dialog.c_str(), NULL, NULL);
			amx_Exec(iter.amx, &retval, iter._on_dialog_performed.id);
			amx_Release(iter.amx, addr_params);
			amx_Release(iter.amx, dialog_addr);
		}
	}
}