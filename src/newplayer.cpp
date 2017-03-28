/* --------------------------------------------------------------------
EXTREME TUXRACER

Copyright (C) 1999-2001 Jasmin F. Patry (Tuxracer)
Copyright (C) 2010 Extreme Tuxracer Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
---------------------------------------------------------------------*/

#ifdef HAVE_CONFIG_H
#include <etr_config.h>
#endif

#include "newplayer.h"
#include "particles.h"
#include "audio.h"
#include "gui.h"
#include "ogl.h"
#include "textures.h"
#include "font.h"
#include "game_ctrl.h"
#include "translation.h"
#include "regist.h"
#include "winsys.h"
#include "spx.h"

CNewPlayer NewPlayer;

static TUpDown* avatar;
static TWidget* textbuttons[2];
static TTextField* textfield;

void QuitAndAddPlayer() {
	if (textfield->Text().getSize() > 0)
		Players.AddPlayer(textfield->Text(), Players.GetDirectAvatarName(avatar->GetValue()));
	State::manager.RequestEnterState(Regist);
}

void CNewPlayer::Keyb(sf::Keyboard::Key key, bool release, int x, int y) {
	if (release) return;

	KeyGUI(key, release);
	switch (key) {
		case sf::Keyboard::Escape:
			State::manager.RequestEnterState(Regist);
			break;
		case sf::Keyboard::Return:
			if (textbuttons[0]->focussed()) State::manager.RequestEnterState(Regist);
			else QuitAndAddPlayer();
			break;
		default:
			break;
	}
}

void CNewPlayer::TextEntered(char text) {
	TextEnterGUI(text);
}

void CNewPlayer::Mouse(int button, int state, int x, int y) {
	if (state == 1) {
		TWidget* clicked = ClickGUI(x, y);

		if (clicked == textbuttons[0])
			State::manager.RequestEnterState(Regist);
		else if (clicked == textbuttons[1])
			QuitAndAddPlayer();
	}
}

void CNewPlayer::Motion(int x, int y) {
	MouseMoveGUI(x, y);

	if (param.ui_snow) push_ui_snow(cursor_pos);
}

static int prevleft, prevtop, prevwidth;

void CNewPlayer::Enter() {
	Winsys.ShowCursor(!param.ice_cursor);
	Music.Play(param.menu_music, true);

	int framewidth = 400 * Winsys.scale;
	int frameheight = 50 * Winsys.scale;
	int frametop = AutoYPosN(38);
	TArea area = AutoAreaN(30, 80, framewidth);
	int prevoffs = 80;
	prevleft = area.left + prevoffs;
	prevtop = AutoYPosN(52);
	prevwidth = 75 * Winsys.scale;

	ResetGUI();

	avatar = AddUpDown(area.left + prevwidth + prevoffs + 8, prevtop, 0, (int)Players.numAvatars() - 1, 0, prevwidth - 34);
	int siz = FT.AutoSizeN(5);
	textbuttons[0] = AddTextButton(Trans.Text(8), area.left+50, AutoYPosN(70), siz);
	float len = FT.GetTextWidth(Trans.Text(15));
	textbuttons[1] = AddTextButton(Trans.Text(15), area.right-len-50, AutoYPosN(70), siz);

	textfield = AddTextField(emptyString, area.left, frametop, framewidth, frameheight);
}

void CNewPlayer::Loop(float time_step) {
	sf::Color col;

	ScopedRenderMode rm(GUI);
	Winsys.clear();

	if (param.ui_snow) {
		update_ui_snow(time_step);
		draw_ui_snow();
	}

	textfield->UpdateCursor(time_step);

	DrawGUIBackground(Winsys.scale);

	FT.SetColor(colWhite);
	FT.AutoSizeN(4);
	FT.DrawString(CENTER, AutoYPosN(30), Trans.Text(66));

	if (avatar->focussed()) col = colDYell;
	else col = colWhite;
	Players.GetAvatarTexture(avatar->GetValue())->DrawFrame(
	    prevleft, prevtop, prevwidth, prevwidth, 2, col);

	DrawGUI();
	Winsys.SwapBuffers();
}
