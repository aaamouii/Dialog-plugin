#include "main.hpp"

cell AMX_NATIVE_CALL Natives::Dialog_Open(AMX *amx, cell *params)
{
	char *dialog_name;
	amx_StrParam(amx, params[2], dialog_name);
	Dialog::Open(amx, params[1], dialog_name);
	return 1;
}

cell AMX_NATIVE_CALL Natives::Dialog_Show(AMX *amx, cell *params)
{
	char *dialog_name;
	amx_StrParam(amx, params[2], dialog_name);
	Dialog::Show(amx, params[1], dialog_name);
	return 1;
}

cell AMX_NATIVE_CALL Natives::Dialog_EnableFix(AMX *amx, cell *params)
{
	Dialog::EnableFix();
	return 1;
}

vector<AMX_NATIVE_INFO> natives{
	{"_Dialog_Open",Natives::Dialog_Open},
	{"Dialog_Show",Natives::Dialog_Show},
	{"Dialog_EnableFix",Natives::Dialog_EnableFix}
};

void Natives::RegisterNatives(AMX *amx)
{
	amx_Register(amx, natives.data(), natives.size());
}