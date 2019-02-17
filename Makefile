NAME=		TAAG

##
##		RESSOURCES
##

OBJ_PATH=	obj

CPPFLAGS=	-Iincludes -Iincludes/template -Iincludes/gui -I$(SDL_MAIN_INCLUDE_PATH) -I$(SDL_IMAGE_INCLUDE_PATH) \
			-I$(SDL_MIXER_INCLUDE_PATH) -I$(SDL_TTF_INCLUDE_PATH)

DIR = 			srcs/ \
				srcs/basics \
				srcs/gui \
				srcs/taag/basics \
				srcs/taag/menu_player_editor \
				srcs/taag/menu_monster_editor \
				srcs/taag/menu_spell_editor \
				srcs/taag/menu_map_editor \
				srcs/taag/game_engine \
				srcs/taag

SRCS_LIST = $(foreach dir, $(DIR), $(wildcard $(dir)/*.cpp))

SRC = 		$(foreach file, $(SRCS_LIST), $(notdir $(file)))

OBJ=		$(SRC:%.cpp=%.o)

OBJS =		$(addprefix $(OBJ_PATH)/, $(OBJ))
##
##		COMPILER FLAGS
##

CFLAGS=		-std=c++11 -Werror -Wextra -g -fsanitize=address

CC=			g++

vpath %.cpp ./srcs/:./srcs/basics/:./srcs/taag/:./srcs/taag/basics:./srcs/gui:./srcs/taag/menu_player_editor:./srcs/taag/menu_spell_editor:./srcs/taag/menu_monster_editor:./srcs/taag/menu_map_editor:./srcs/taag/game_engine

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(SDL_MAIN_LIBS_PATH):$(SDL_IMAGE_LIBS_PATH):$(SDL_MIXER_LIBS_PATH):$(SDL_TTF_LIBS_PATH)

LDFLAGS=	-L$(SDL_MAIN_LIBS_PATH) -L$(SDL_IMAGE_LIBS_PATH) \
			-L$(SDL_MIXER_LIBS_PATH) -L$(SDL_TTF_LIBS_PATH)

LIBS=		-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -framework GLUT -framework OpenGL

LDLIBS=		$(LDFLAGS) $(LIBS)

##
##		SDL FILES
##

CURL_PATH = 			curl -L

SDL_FOLDER =			$(HOME)/sdl/

SDL_MAIN_DOWNLOAD =		https://www.libsdl.org/release/SDL2-2.0.8.tar.gz
SDL_MAIN_VERSION =		SDL2-2.0.8.tar.gz
SDL_MAIN_FOLDER =		$(SDL_MAIN_VERSION:.tar.gz=)

SDL_IMAGE_DOWNLOAD =	https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.3.tar.gz
SDL_IMAGE_VERSION =		SDL2_image-2.0.3.tar.gz
SDL_IMAGE_FOLDER =		$(SDL_IMAGE_VERSION:.tar.gz=)

SDL_MIXER_DOWNLOAD =	https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.2.tar.gz
SDL_MIXER_VERSION =		SDL2_mixer-2.0.2.tar.gz
SDL_MIXER_FOLDER =		$(SDL_MIXER_VERSION:.tar.gz=)

SDL_TTF_DOWNLOAD =		https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.tar.gz
SDL_TTF_VERSION =		SDL2_ttf-2.0.14.tar.gz
SDL_TTF_FOLDER =		$(SDL_TTF_VERSION:.tar.gz=)

FREETYPE_DOWNLOAD =		https://download.savannah.gnu.org/releases/freetype/freetype-2.8.1.tar.gz
FREETYPE_VERSION =		freetype-2.8.1.tar.gz
FREETYPE_FOLDER =		$(FREETYPE_VERSION:.tar.gz=)

SDL_MAIN_HEADER =			SDL.h\
							SDL_assert.h\
							SDL_atomic.h\
							SDL_audio.h\
							SDL_bits.h\
							SDL_blendmode.h\
							SDL_clipboard.h\
							SDL_cpuinfo.h\
							SDL_egl.h\
							SDL_endian.h\
							SDL_error.h\
							SDL_events.h\
							SDL_filesystem.h\
							SDL_gamecontroller.h\
							SDL_gesture.h\
							SDL_haptic.h\
							SDL_hints.h\
							SDL_joystick.h\
							SDL_keyboard.h\
							SDL_keycode.h\
							SDL_loadso.h\
							SDL_log.h\
							SDL_main.h\
							SDL_messagebox.h\
							SDL_mouse.h\
							SDL_mutex.h\
							SDL_name.h\
							SDL_opengl.h\
							SDL_opengl_glext.h\
							SDL_opengles.h\
							SDL_opengles2_gl2ext.h\
							SDL_opengles2_gl2.h\
							SDL_opengles2_gl2platform.h\
							SDL_opengles2.h\
							SDL_opengles2_khrplatform.h\
							SDL_pixels.h\
							SDL_platform.h\
							SDL_power.h\
							SDL_quit.h\
							SDL_rect.h\
							SDL_render.h\
							SDL_rwops.h\
							SDL_scancode.h\
							SDL_shape.h\
							SDL_stdinc.h\
							SDL_surface.h\
							SDL_system.h\
							SDL_syswm.h\
							SDL_thread.h\
							SDL_timer.h\
							SDL_touch.h\
							SDL_types.h\
							SDL_version.h\
							SDL_video.h\
							SDL_vulkan.h\
							begin_code.h\
							close_code.h\

SDL_IMAGE_HEADER =			SDL_image.h

SDL_MIXER_HEADER =			SDL_mixer.h

SDL_TTF_HEADER =			SDL_ttf.h

SDL_MAIN_OBJ_LIBS =			libSDL2.a

SDL_IMAGE_OBJ_LIBS =		libSDL2_image.a

SDL_MIXER_OBJ_LIBS =		libSDL2_mixer.a

SDL_TTF_OBJ_LIBS =			libSDL2_ttf.a

SDL_MAIN_LIBS_PATH =		$(SDL_FOLDER)$(SDL_MAIN_FOLDER)/lib/
SDL_MAIN_INCLUDE_PATH =		$(SDL_FOLDER)$(SDL_MAIN_FOLDER)/include/SDL2/
SDL_IMAGE_LIBS_PATH =		$(SDL_FOLDER)$(SDL_IMAGE_FOLDER)/lib/
SDL_IMAGE_INCLUDE_PATH =	$(SDL_FOLDER)$(SDL_IMAGE_FOLDER)/include/SDL2/
SDL_MIXER_LIBS_PATH =		$(SDL_FOLDER)$(SDL_MIXER_FOLDER)/lib/
SDL_MIXER_INCLUDE_PATH =	$(SDL_FOLDER)$(SDL_MIXER_FOLDER)/include/SDL2/
SDL_TTF_LIBS_PATH =			$(SDL_FOLDER)$(SDL_TTF_FOLDER)/lib/
SDL_TTF_INCLUDE_PATH =		$(SDL_FOLDER)$(SDL_TTF_FOLDER)/include/SDL2/

SDL_MAIN_LIBS =			$(addprefix $(SDL_MAIN_LIBS_PATH), $(SDL_MAIN_OBJ_LIBS))
SDL_MAIN_INCLUDE =		$(addprefix $(SDL_MAIN_INCLUDE_PATH), $(SDL_MAIN_HEADER))
SDL_IMAGE_LIBS =		$(addprefix $(SDL_IMAGE_LIBS_PATH), $(SDL_IMAGE_OBJ_LIBS))
SDL_IMAGE_INCLUDE =		$(addprefix $(SDL_IMAGE_INCLUDE_PATH), $(SDL_IMAGE_HEADER))
SDL_MIXER_LIBS =		$(addprefix $(SDL_MIXER_LIBS_PATH), $(SDL_MIXER_OBJ_LIBS))
SDL_MIXER_INCLUDE =		$(addprefix $(SDL_MIXER_INCLUDE_PATH), $(SDL_MIXER_HEADER))
SDL_TTF_LIBS =			$(addprefix $(SDL_TTF_LIBS_PATH), $(SDL_TTF_OBJ_LIBS))
SDL_TTF_INCLUDE =		$(addprefix $(SDL_TTF_INCLUDE_PATH), $(SDL_TTF_HEADER))


##
##		Compile using the program's name to recompile that program.
##		Compile with test for special testing purposes.
##

all:
	@$(MAKE) $(MFLAGS) sdl
	@$(MAKE) $(MFLAGS) $(NAME)

sdl:
				@if [ ! -d "$(SDL_FOLDER)" ] ; then mkdir $(SDL_FOLDER) ; fi
				@echo "Verifing sdl_main ...\c"
				@make sdl_main
				@echo " DONE"
				@echo "Verifing sdl_image ...\c"
				@make sdl_image
				@echo " DONE"
				@echo "Verifing sdl_mixer ...\c"
				@make sdl_mixer
				@echo " DONE"
				@echo "Verifing sdl_tff ...\c"
				@make sdl_ttf
				@echo " DONE"

sdl_main:
	@if [ ! -d "$(SDL_FOLDER)$(SDL_MAIN_FOLDER)" ] ; then ($(CURL_PATH) $(SDL_MAIN_DOWNLOAD) > $(SDL_FOLDER)$(SDL_MAIN_VERSION) ;\
															tar -xzf $(SDL_FOLDER)$(SDL_MAIN_VERSION) -C $(SDL_FOLDER)); fi
	@if [ ! -d "$(SDL_FOLDER)$(SDL_MAIN_FOLDER)/build" ] ; then mkdir $(SDL_FOLDER)$(SDL_MAIN_FOLDER)/build ; fi
	@$(foreach file, $(SDL_MAIN_LIBS), test -e $(file) || (echo "\n\033[31mMissing file ($(file)) in $@\033[0m" ; \
	cd $(SDL_FOLDER)$(SDL_MAIN_FOLDER)/build ; ../configure --prefix=$(SDL_FOLDER)$(SDL_MAIN_FOLDER) ; \
	make $(MFLAGS) -C $(SDL_FOLDER)$(SDL_MAIN_FOLDER)/build ; \
	make $(MFLAGS) -C $(SDL_FOLDER)$(SDL_MAIN_FOLDER)/build install );)
	@$(foreach file, $(SDL_MAIN_INCLUDE), test -e $(file) || (echo "\n\033[31mMissing file ($(file)) in $@\033[0m" ; \
	cd $(SDL_FOLDER)$(SDL_MAIN_FOLDER)/build ; ../configure --prefix=$(SDL_FOLDER)$(SDL_MAIN_FOLDER) ; \
	make $(MFLAGS) -C $(SDL_FOLDER)$(SDL_MAIN_FOLDER)/build ; \
	make $(MFLAGS) -C $(SDL_FOLDER)$(SDL_MAIN_FOLDER)/build install );)

sdl_image:
	@if [ ! -d "$(SDL_FOLDER)$(SDL_IMAGE_FOLDER)" ] ; then ($(CURL_PATH) $(SDL_IMAGE_DOWNLOAD) > $(SDL_FOLDER)$(SDL_IMAGE_VERSION) ;\
															tar -xzf $(SDL_FOLDER)$(SDL_IMAGE_VERSION) -C $(SDL_FOLDER)) ; fi
	@if [ ! -d "$(SDL_FOLDER)$(SDL_IMAGE_FOLDER)/build" ] ; then mkdir $(SDL_FOLDER)$(SDL_IMAGE_FOLDER)/build ; fi
	@$(foreach file, $(SDL_IMAGE_LIBS), test -e $(file) || (echo "		\n\033[31mMissing file ($(file)) in $@\033[0m" ; \
	cd $(SDL_FOLDER)$(SDL_IMAGE_FOLDER)/build ; ../configure --prefix=$(SDL_FOLDER)$(SDL_IMAGE_FOLDER) --with-sdl-prefix=$(SDL_FOLDER)$(SDL_MAIN_FOLDER) ; \
	make $(MFLAGS) -C $(SDL_FOLDER)$(SDL_IMAGE_FOLDER)/build ; \
	make $(MFLAGS) -C $(SDL_FOLDER)$(SDL_IMAGE_FOLDER)/build install);)
	@$(foreach file, $(SDL_IMAGE_INCLUDE), test -e $(file) || (echo "		\n\033[31mMissing file ($(file)) in $@\033[0m" ; \
	cd $(SDL_FOLDER)$(SDL_IMAGE_FOLDER)/build ; ../configure --prefix=$(SDL_FOLDER)$(SDL_IMAGE_FOLDER) --with-sdl-prefix=$(SDL_FOLDER)$(SDL_MAIN_FOLDER) ; \
	make $(MFLAGS) -C $(SDL_FOLDER)$(SDL_IMAGE_FOLDER)/build ; \
	make $(MFLAGS) -C $(SDL_FOLDER)$(SDL_IMAGE_FOLDER)/build install);)

sdl_mixer:
	@if [ ! -d "$(SDL_FOLDER)$(SDL_MIXER_FOLDER)" ] ; then ($(CURL_PATH) $(SDL_MIXER_DOWNLOAD) > $(SDL_FOLDER)$(SDL_MIXER_VERSION) ;\
															tar -xzf $(SDL_FOLDER)$(SDL_MIXER_VERSION) -C $(SDL_FOLDER) ;) fi
	@if [ ! -d "$(SDL_FOLDER)$(SDL_MIXER_FOLDER)/build" ] ; then mkdir $(SDL_FOLDER)$(SDL_MIXER_FOLDER)/build ; fi
	@$(foreach file, $(SDL_MIXER_LIBS), test -e $(file) || (echo "\n\033[31mMissing file ($(file)) in $@\033[0m" ; \
	cd $(SDL_FOLDER)$(SDL_MIXER_FOLDER)/build ; ../configure --prefix=$(SDL_FOLDER)$(SDL_MIXER_FOLDER) --with-sdl-prefix=$(SDL_FOLDER)$(SDL_MAIN_FOLDER) ; \
	make $(MFLAGS) -C $(SDL_FOLDER)$(SDL_MIXER_FOLDER)/build ; \
	make $(MFLAGS) -C $(SDL_FOLDER)$(SDL_MIXER_FOLDER)/build install);)
	@$(foreach file, $(SDL_MIXER_INCLUDE), test -e $(file) || (echo "\n\033[31mMissing file ($(file)) in $@\033[0m" ; \
	cd $(SDL_FOLDER)$(SDL_MIXER_FOLDER)/build ; ../configure --prefix=$(SDL_FOLDER)$(SDL_MIXER_FOLDER) --with-sdl-prefix=$(SDL_FOLDER)$(SDL_MAIN_FOLDER) ; \
	make $(MFLAGS) -C $(SDL_FOLDER)$(SDL_MIXER_FOLDER)/build ; \
	make $(MFLAGS) -C $(SDL_FOLDER)$(SDL_MIXER_FOLDER)/build install);)

sub_sdl_ttf:
	cd $(SDL_FOLDER)$(FREETYPE_FOLDER)/build ; ../configure --prefix=$$PWD
	make $(MFLAGS) -C $(SDL_FOLDER)$(FREETYPE_FOLDER)/build
	make $(MFLAGS) -C $(SDL_FOLDER)$(FREETYPE_FOLDER)/build install
	cd $(SDL_FOLDER)$(SDL_TTF_FOLDER) ; ./configure --prefix=$$PWD --with-sdl-prefix=$(SDL_FOLDER)$(SDL_MAIN_FOLDER) --with-freetype-exec-prefix=$(SDL_FOLDER)$(FREETYPE_FOLDER)/build
	make $(MFLAGS) -C $(SDL_FOLDER)$(SDL_TTF_FOLDER)
	make $(MFLAGS) -C $(SDL_FOLDER)$(SDL_TTF_FOLDER) install

sdl_ttf:
	@if [ ! -d "$(SDL_FOLDER)$(SDL_TTF_FOLDER)" ] ; then ($(CURL_PATH) $(SDL_TTF_DOWNLOAD) > $(SDL_FOLDER)$(SDL_TTF_VERSION) ;\
															tar -xzf $(SDL_FOLDER)$(SDL_TTF_VERSION) -C $(SDL_FOLDER)) ; fi
	@if [ ! -d "$(SDL_FOLDER)$(SDL_TTF_FOLDER)/build" ] ; then mkdir $(SDL_FOLDER)$(SDL_TTF_FOLDER)/build ; fi
	@if [ ! -f "$(SDL_FOLDER)$(FREETYPE_VERSION)" ] ; then $(CURL_PATH) $(FREETYPE_DOWNLOAD) > $(SDL_FOLDER)$(FREETYPE_VERSION) ; fi
	@if [ ! -d "$(SDL_FOLDER)$(FREETYPE_FOLDER)" ] ; then tar -xzf $(SDL_FOLDER)$(FREETYPE_VERSION) -C $(SDL_FOLDER) ; fi
	@if [ ! -d "$(SDL_FOLDER)$(FREETYPE_FOLDER)/build" ] ; then mkdir $(SDL_FOLDER)$(FREETYPE_FOLDER)/build ; fi
	@$(foreach file, $(SDL_TTF_LIBS), test -e $(file) || (echo "\n\033[31mMissing file ($(file)) in $@\033[0m" ; make sub_sdl_ttf );)
	@$(foreach file, $(SDL_TTF_INCLUDE), test -e $(file) || (echo "\n\033[31mMissing file ($(file)) in $@\033[0m" ; make sub_sdl_ttf );)

##
##		Use fclean to clean this program.
##

$(NAME):		$(OBJS) main.cpp
				@echo "Compiling $(NAME) ...\c"
				@$(CC) $(CFLAGS) $(CPPFLAGS) -o $(NAME) main.cpp $(OBJS) $(LDLIBS)
				@echo " DONE"

$(OBJ_PATH)/%.o : %.cpp
				@mkdir $(OBJ_PATH) 2> /dev/null || true
				@echo "Compiling $< ...\c"
				@$(CC) $(CFLAGS) -o $@ -c $< $(CPPFLAGS)
				@echo " DONE"

clean:
				@rm -rf $(OBJ_PATH)
				@echo "clean"

fclean:			clean
				@rm -f $(NAME)
				@echo "fclean"

re:				fclean all

.PHONY:			all clean fclean re
