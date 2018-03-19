/*
	MIT License

	Copyright (c) 2018 Kash Cherry

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
#if defined _dialog_included
	#endinput
#endif
#define _dialog_included

native _Dialog_Open(playerid, const dialog_name[]);
native Dialog_Show(playerid, const dialog_name[]);
native Dialog_EnableFix();

#define DialogCreate:%0(%1) \
		forward dcr_%0(%1); \
		public dcr_%0(%1)
		
#define DialogResponse:%0(%1) \
		forward dre_%0(%1); \
		public dre_%0(%1)
		
#if defined OnDialogReceived
	forward OnDialogReceived(dialog[], playerid, response, listitem, inputtext[]);
#endif
#if defined OnDialogPerformed
	forward OnDialogPerformed(dialog[], playerid, response, listitem, inputtext[], result);
#endif
		
stock Dialog_Open(playerid, const dialog_name[], style, caption[], info[], button1[], button2[])
{
	_Dialog_Open(playerid, dialog_name);
	return ShowPlayerDialog(playerid, 32700, style, caption, info, button1, button2);
}