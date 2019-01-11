#ifndef INCLUDE_H
# define INCLUDE_H


#ifdef __APPLE__
#	include <stdlib.h>
#elif defined _WIN32
#	include <io.h>
#endif

# include <stdio.h>
# include <iostream>
# include <fstream>
# include <fcntl.h>
# include <string>
# include <map>

# include <string.h>
# include <dirent.h>
# include <vector>
# include <math.h>
# include <iomanip>
# include <sstream>

# include <SDL.h>

#ifdef __APPLE__
#	include <OpenGL/gl3.h>
#	include <OpenGL/gl3ext.h>
#elif defined _WIN32
#	include <glew.h>
#	include <SDL_opengl.h>
#endif

# include <SDL_image.h>
# include <SDL_ttf.h>
# include <SDL_mixer.h>

#endif
