/*************************************************************************

                         "I Have No Tomatoes"
                  Copyright (c) 2004, Mika Halttunen

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute
 it freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must
    not claim that you wrote the original software. If you use this
    software in a product, an acknowledgment in the product documentation
    would be appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must
    not be misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.


 Mika Halttunen <lsoft@mbnet.fi>

*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>


#include "config.h"
#include "mpak.h"
#include "mymath.h"


// Keep global pointers to the SDL window and the SDL GL context
SDL_Window *sdl_window;
SDL_GLContext sdl_context;


// Keep a global pointer to the config
CONFIG config;


// Keep a global pointer to the data file
MPAK_FILE pakfile;



// Display an error message and quit
void error_msg(const char *msg, ...) {

	char *buf = new char[4096];

	// Format the msg to buf
	va_list ap;
	va_start(ap, msg);
	vsprintf(buf, msg, ap);
	va_end(ap);

	// Output to stderr
	fprintf(stderr, "Error appeared:\n - %s\n", buf);

	SDL_Quit();

	// Display the message using MessageBoxes on Windows
#ifdef WIN32
	MessageBox(HWND_DESKTOP, buf, "Error appeared", MB_OK|MB_ICONERROR);
#endif

	delete [] buf;
	exit(-1);
}


// Initialize SDL and OpenGL
void init_sdl_and_gl() {

	// Load the config
	load_config(get_config_location(), &config);

	// Initialize SDL with video and timer support
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) < 0)
		error_msg("Unable to init SDL: %s", SDL_GetError());

	// Hide the mouse cursor
	SDL_ShowCursor(0);

	// Open the pakfile, with globally define OVERRIDE_DIR being the override directory
	pakfile.init();
	if(!pakfile.open_mpk(MPAK_READ, MPK_DIR "tomatoes.mpk", OVERRIDE_DIR))
		error_msg("Unable to open 'tomatoes.mpk'.\nThe file either doesn't exist or is corrupted.");

	// Load an icon
	SDL_Surface *icon = NULL;
	FILE *fin = pakfile.open_file("icon.bmp");
	if(!fin)
		error_msg("Unable to load the icon!\n");
	icon = SDL_LoadBMP_RW(SDL_RWFromFP(fin, SDL_TRUE), 1);
	if(!icon)
		error_msg("Unable to load the icon!\n");
	SDL_SetColorKey(icon, SDL_TRUE, SDL_MapRGB(icon->format, 255, 0, 255));
	SDL_SetWindowIcon(sdl_window, icon);
	SDL_FreeSurface(icon);

	// Initialize math tables
	init_math();

	// Set the video mode
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	sdl_window = SDL_CreateWindow("I Have No Tomatoes", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, config.vid_w, config.vid_h,
		(config.fullscreen) ? (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL) : (SDL_WINDOW_OPENGL));
	if(sdl_window == NULL)
		error_msg("Unable to set the OpenGL video mode %d x %d!\n%s", config.vid_w, config.vid_h, SDL_GetError());

	sdl_context = SDL_GL_CreateContext(sdl_window);
	if (sdl_context == NULL)
		error_msg("Failed to create OpenGL context!\n%s", SDL_GetError());

	//~ screen = SDL_SetVideoMode(config.vid_w, config.vid_h, config.vid_color_depth, (config.fullscreen) ? (SDL_OPENGL|SDL_FULLSCREEN) : (SDL_OPENGL));

	// Set OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	glClearColor(0, 0, 0, 0);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DITHER);

	int win_h = 0, win_w = 0;
	SDL_GetWindowSize(sdl_window, &win_w, &win_h);
	glViewport(0, 0, win_w, win_h);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	// Alpha test
	glAlphaFunc(GL_GREATER, 0.1f);
}


