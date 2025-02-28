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
#include "config.h"
#include "init.h"


// The config files are saved to the users HOME directory in Linux, but
// not in Windows.
#ifdef LINUX
#include <string>
#include <sys/stat.h>
#include <dirent.h>
using namespace std;


// Helper function which makes sure that .tomatoes directory
// exists in the user's home directory.
std::string get_tomatoes_dir() {
	// Get the HOME environment variable
	char *home = getenv("HOME");
	if(home != NULL) {
		string tmp = (string)home + "/.tomatoes/";
		// Check if the directory exists
		DIR *dp = NULL;
		dp = opendir(tmp.c_str());
		if(!dp) {
			// It doesn't exist, try to create it
			if(mkdir(tmp.c_str(), S_IRWXU))
				// Can't create it
				return "";
			return tmp;
		}
		else {
			// It does exist.
			closedir(dp);
			return tmp;
		}
	}

	// The HOME variable wasn't available
	return "";
}
#endif


// Helper function which returns suitable path for the
// config file. It first checks the user's home directory,
// and if that fails it uses the CONFIG_DIR defined in the
// makefile.
std::string get_config_location(bool write) {
#ifdef LINUX
	// Get the path to the config file
	string tmp = get_tomatoes_dir() + "config.cfg";

	// Check if the config file exists there
	if(!write) {
		FILE *ftest = fopen(tmp.c_str(), "rt");
		if(!ftest) {
			// It doesn't exist, try the default
			return (CONFIG_DIR "config.cfg");
		}
		fclose(ftest);
	}

	return tmp;
#endif

	// Return the CONFIG_DIR
	return (CONFIG_DIR "config.cfg");
}

void load_default_config(CONFIG *conf) {
	conf->vid_w = 800;
	conf->vid_h = 600;
	conf->vid_color_depth = 32;
	conf->fullscreen = 1;
	conf->sound = 1;
	conf->sound_freq = 44100;
	conf->sound_vol = 205;
	conf->music_vol = 135;
	conf->show_fps = 0;
	conf->key_left[0] = 276;
	conf->key_left[1] = 97;
	conf->key_right[0] = 275;
	conf->key_right[1] = 100;
	conf->key_up[0] = 273;
	conf->key_up[1] = 119;
	conf->key_down[0] = 274;
	conf->key_down[1] = 115;
	conf->key_shoot[0] = 32;
	conf->key_shoot[1] = 304;
	conf->key_special[0] = 307;
	conf->key_special[1] = 306;
	conf->perspective_mode = 0;
	conf->moving_style[0] = 2;
	conf->moving_style[1] = 2;
}



// Load config from file
void load_config(string file, CONFIG *conf) {
	printf("config = %s\n", file.c_str());

	FILE *f = fopen(file.c_str(), "rt");
	if(!f)
		error_msg("Unable to load config file: %s!", file.c_str());

	fscanf(f, "video_mode = %d x %d\n", &(conf->vid_w), &(conf->vid_h));
	fscanf(f, "video_mode_color_depth = %d\n", &(conf->vid_color_depth));
	fscanf(f, "video_mode_fullscreen = %d\n", &(conf->fullscreen));
	fscanf(f, "sound_enabled = %d\n", &(conf->sound));
	fscanf(f, "sound_freq = %d\n", &(conf->sound_freq));
	fscanf(f, "sound_volume = %d\n", &(conf->sound_vol));
	fscanf(f, "music_volume = %d\n", &(conf->music_vol));
	fscanf(f, "show_fps = %d\n", &(conf->show_fps));
	fscanf(f, "key_left = %d %d\n", &(conf->key_left[0]), &(conf->key_left[1]));
	fscanf(f, "key_right = %d %d\n", &(conf->key_right[0]), &(conf->key_right[1]));
	fscanf(f, "key_up = %d %d\n", &(conf->key_up[0]), &(conf->key_up[1]));
	fscanf(f, "key_down = %d %d\n", &(conf->key_down[0]), &(conf->key_down[1]));
	fscanf(f, "key_shoot = %d %d\n", &(conf->key_shoot[0]), &(conf->key_shoot[1]));
	fscanf(f, "key_special = %d %d\n", &(conf->key_special[0]), &(conf->key_special[1]));
	fscanf(f, "perspective = %d\n", &(conf->perspective_mode));
	fscanf(f, "moving_style = %d %d\n", &(conf->moving_style[0]), &(conf->moving_style[1]));
	fclose(f);

	printf("video_mode = %d x %d\n", (conf->vid_w), (conf->vid_h));
	printf("video_mode_color_depth = %d\n", (conf->vid_color_depth));
	printf("video_mode_fullscreen = %d\n", (conf->fullscreen));
	printf("sound_enabled = %d\n", (conf->sound));
	printf("sound_freq = %d\n", (conf->sound_freq));
	printf("sound_volume = %d\n", (conf->sound_vol));
	printf("music_volume = %d\n", (conf->music_vol));
	printf("show_fps = %d\n", (conf->show_fps));
	printf("key_left = %d %d\n", (conf->key_left[0]), (conf->key_left[1]));
	printf("key_right = %d %d\n", (conf->key_right[0]), (conf->key_right[1]));
	printf("key_up = %d %d\n", (conf->key_up[0]), (conf->key_up[1]));
	printf("key_down = %d %d\n", (conf->key_down[0]), (conf->key_down[1]));
	printf("key_shoot = %d %d\n", (conf->key_shoot[0]), (conf->key_shoot[1]));
	printf("key_special = %d %d\n", (conf->key_special[0]), (conf->key_special[1]));
	printf("perspective = %d\n", (conf->perspective_mode));
	printf("moving_style = %d %d\n", (conf->moving_style[0]), (conf->moving_style[1]));
}


// Save config to file
void save_config(string file, CONFIG *conf) {

	FILE *f = fopen(file.c_str(), "wt");
	if(!f)
		error_msg("Unable to save config file: %s!", file.c_str());

	fprintf(f, "video_mode = %d x %d\n", (conf->vid_w), (conf->vid_h));
	fprintf(f, "video_mode_color_depth = %d\n", (conf->vid_color_depth));
	fprintf(f, "video_mode_fullscreen = %d\n", (conf->fullscreen));
	fprintf(f, "sound_enabled = %d\n", (conf->sound));
	fprintf(f, "sound_freq = %d\n", (conf->sound_freq));
	fprintf(f, "sound_volume = %d\n", (conf->sound_vol));
	fprintf(f, "music_volume = %d\n", (conf->music_vol));
	fprintf(f, "show_fps = %d\n", (conf->show_fps));
	fprintf(f, "key_left = %d %d\n", (conf->key_left[0]), (conf->key_left[1]));
	fprintf(f, "key_right = %d %d\n", (conf->key_right[0]), (conf->key_right[1]));
	fprintf(f, "key_up = %d %d\n", (conf->key_up[0]), (conf->key_up[1]));
	fprintf(f, "key_down = %d %d\n", (conf->key_down[0]), (conf->key_down[1]));
	fprintf(f, "key_shoot = %d %d\n", (conf->key_shoot[0]), (conf->key_shoot[1]));
	fprintf(f, "key_special = %d %d\n", (conf->key_special[0]), (conf->key_special[1]));
	fprintf(f, "perspective = %d\n", (conf->perspective_mode));
	fprintf(f, "moving_style = %d %d\n", (conf->moving_style[0]), (conf->moving_style[1]));
	fclose(f);
}


