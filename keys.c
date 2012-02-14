/*
This is the Nearly Free Mouse server.
Copyright (C) 2012 nearlyfreesolutions@gmail.com

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
Boston, MA  02110-1301, USA.
*/
#include "types.h"
#include <windows.h>

#define String const char *
//#include "keys.java"

String _escape = "Escape";
String _altltab = "Alt_L+Tab";
String _altlf4 = "Alt_L+F4";

typedef struct {
    const char **_key;
    uint32 _vkey;
    uint8 _alt;
} Key;

const Key _keys[] = {
    { &_escape, VK_ESCAPE },
/*
    { &_altltab, VK_TAB, 1 },
    { &_alttab, VK_TAB, 1 },
    { &_altlf4, VK_F4, 1 },
    { &_esc, VK_ESCAPE },
	{ &_super, VK_LWIN },
	{ &_del, VK_DELETE },
    { &_altf4, VK_F4, 1 },
	{ &_space_b, VK_SPACE },
    { &_dollar, VK_DOLLAR },
	{ &_percent },
	{ &_ampersand },
	{ &_parenleft },
	{ &_parenright },
	{ &_exclam },
	{ &_space, VK_SPACE },
	{ &_backspace, VK_BACK },
	{ &_return, VK_RETURN },
	{ &_tab, VK_TAB },
	{ &_at },
	{ &_numbersign },
	{ &_period },
	{ &_plus },
	{ &_minus },
	{ &_backslash },
	{ &_slash },
	{ &_semicolon },
	{ &_asterisk },
	{ &_apostrophe },
	{ &_comma },
	{ &_up },
	{ &_down },
	{ &_left },
	{ &_right },
	{ &_question },
	{ &_quotedbl },
	{ &_colon },
	{ &_grave },
	{ &_tilde },
	{ &_pipe },
	{ &_braceleft },
	{ &_braceright },
	{ &_bracketleft },
	{ &_bracketright },
	{ &_equal },
	{ &_underscore },
	{ &_division },
	{ &_multiply },
	{ &_caret },
	{ &_leftanglebracket },
	{ &_righttanglebracket },
	{ &_eurosign },
	{ &_poundsign },
*/
};

const uint32 _n_keys = sizeof(_keys)/sizeof(*_keys);


