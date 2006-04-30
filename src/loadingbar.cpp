/*
 * Stellarium
 * Copyright (C) 2005 Fabien Chereau
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "loadingbar.h"

LoadingBar::LoadingBar(Projector* _prj, float font_size, const string& font_name, const string&  splash_tex, 
	int screenw, int screenh, const wstring& extraTextString, float extraTextSize, 
	float extraTextPosx, float extraTextPosy) :
	prj(_prj), width(512), height(512), barwidth(400), barheight(10), extraText(extraTextString)
{
	splashx = (screenw - width)/2;
	splashy = (screenh - height)/2;
	barx = (screenw - barwidth)/2;
	bary = splashy + 34;
	barfont = new s_font(font_size, font_name);
	extraTextFont = new s_font(extraTextSize, font_name);
	assert(barfont);
	assert(extraTextFont);
	if (!splash_tex.empty()) splash = new s_texture(splash_tex, TEX_LOAD_TYPE_PNG_ALPHA);
	extraTextPos.set(extraTextPosx, extraTextPosy);
}
	
LoadingBar::~LoadingBar()
{
	delete barfont;
	delete extraTextFont;
	if (splash) delete splash;
	barfont = NULL;
}

void LoadingBar::Draw(float val)
{
	// percent complete bar only draws in 2d mode
	prj->set_orthographic_projection();
  
	// Draw the splash screen if available
	if (splash)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);
		glDisable(GL_CULL_FACE);
		glBindTexture(GL_TEXTURE_2D, splash->getID());

		glBegin(GL_QUADS);
			glTexCoord2i(0, 0);		// Bottom Left
			glVertex3f(splashx, splashy, 0.0f);
			glTexCoord2i(1, 0);		// Bottom Right
			glVertex3f(splashx + width, splashy, 0.0f);
			glTexCoord2i(1, 1);		// Top Right
			glVertex3f(splashx + width, splashy + height, 0.0f);
			glTexCoord2i(0, 1);		// Top Left
			glVertex3f(splashx, splashy + height, 0.0f);
		glEnd();
	}
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	// black out background of text for redraws (so can keep sky unaltered)
	glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i(1, 0);		// Bottom Right
		glVertex3f(barx + barwidth, bary-17, 0.0f);
		glTexCoord2i(0, 0);		// Bottom Left
		glVertex3f(barx, bary-17, 0.0f);
		glTexCoord2i(1, 1);		// Top Right
		glVertex3f(barx + barwidth, bary-5, 0.0f);
		glTexCoord2i(0, 1);		// Top Left
		glVertex3f(barx, bary-5, 0.0f);
	glEnd();
	glColor3f(0.8, 0.8, 1);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i(1, 0);		// Bottom Right
		glVertex3f(barx + barwidth, bary + barheight, 0.0f);
		glTexCoord2i(0, 0);		// Bottom Left
		glVertex3f(barx, bary + barheight, 0.0f);
		glTexCoord2i(1, 1);		// Top Right
		glVertex3f(barx + barwidth, bary, 0.0f);
		glTexCoord2i(0, 1);		// Top Left
		glVertex3f(barx, bary, 0.0f);
	glEnd();
	glColor3f(0.4f, 0.4f, 0.6f);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i(1, 0);		// Bottom Right
		glVertex3f(-1 + barx + barwidth * val, bary + barheight - 1, 0.0f);
		glTexCoord2i(0, 0);		// Bottom Left
		glVertex3f(1 + barx, bary + barheight - 1, 0.0f);
		glTexCoord2i(1, 1);		// Top Right
		glVertex3f(-1 + barx + barwidth * val, bary + 1, 0.0f);
		glTexCoord2i(0, 1);		// Top Left
		glVertex3f(1 + barx, bary + 1, 0.0f);
	glEnd();
	
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	barfont->print(barx, bary-5, message);
	extraTextFont->print(splashx + extraTextPos[0], splashy + extraTextPos[1], extraText);
	
	SDL_GL_SwapBuffers();	// And swap the buffers
	
	prj->reset_perspective_projection();
}
