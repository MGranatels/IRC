# ------------------------------------------------------------------------------------ EDIT HERE ------------------------------------------------------------------------------------ #

NAME = ircserv


BOT_NAME = bot


PORT = 6667


PASSWORD = password


TRANSFER_DIR = files

# -------------------------------------------------------------------------------------- INFO --------------------------------------------------------------------------------------- #


#	Variables:
#
#	> NAME: - - - - - - - - the name of the executable file
#	> PORT: - - - - - - - - the port to run the server on
#	> PASSWORD: - - - - - - the password to use for the server


#	Flags:
#
#	> Wall: - - - - - - - - turns on most, but not all, compiler warnings
#	> Wextra: - - - - - - - enables some extra warning flags that are not enabled by -Wall
#	> Werror: - - - - - - - treats all warnings as errors
#	> std=c++98:  - - - - - sets the C++ standard to C++98
#	> g:  - - - - - - - - - produces debugging information
#	> fsanitize=address:  - turns on address sanitizer


#	Rules:
#
#	> make: - - - - - - - - compiles the program
#	> make all: - - - - - - compiles the program
#	> make clean: - - - - - removes all object files
#	> make config:  - - - - checks if Hexchat is installed and configures it to work with the project
#	> make fclean:  - - - - removes all object files and the executable
#	> make help:  - - - - - displays help message
#	> make install: - - - - tries to install Hexchat
#	> make re:  - - - - - - removes all object files and the executable, then compiles the program
#	> make run: - - - - - - compiles the program and runs the executable with the arguments [PORT] [PASSWORD] and Hexchat
#	> make uninstall: - - - tries to uninstall Hexchat
#	> make valgrind:  - - - recompiles the program without -fsanitize=address and runs valgrind on the executable with the arguments [PORT] [PASSWORD] and Hexchat


# --------------------------------------------------------------------------- DO NOT EDIT BELOW THIS LINE --------------------------------------------------------------------------- #

OK= [ \033[1;32mOK\033[0m ]
FAIL= [ \033[1;31mFAILED\033[0m ]
WARN= [ \033[1;33mWARNING\033[0m ]
WHITE= \033[1;37m
RESET= \033[0m

NAME = ircserv

SRCS = 	utils.cpp \
		Sockets.cpp \
		SocketConnection.cpp \
		Clients.cpp \
		Manager.cpp \
		ManagerCommands.cpp \
		ManagerActions.cpp \
		Channel.cpp \
		main.cpp


CC = @c++

OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g -Ofast -fsanitize=address -Iincludes/

SRCS_DIR = ./src/

OBJS_DIR = ./obj/

MAKEFLAGS += --no-print-directory

FILES_DIR = $(shell pwd)/$(TRANSFER_DIR)

HEXCHAT_DIR := $(shell flatpak run --user io.github.Hexchat --configdir 2> /dev/null | sed 's/hexchat /v=/g' || echo "")

HEXCHAT_CONF = $(HEXCHAT_DIR)/hexchat.conf

HEXCHAT_SERVLIST = $(HEXCHAT_DIR)/servlist.conf

HEXCHAT_EXECUTABLE := flatpak run --user io.github.Hexchat

HEXCHAT_VERSION := $(shell flatpak run --user io.github.Hexchat -v 2> /dev/null || echo "")

HEXCHAT_APPID := $(shell flatpak list --app --columns=application --user 2> /dev/null | grep Hexchat || echo "")

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@-mkdir -p $(OBJS_DIR); mkdir -p $(TRANSFER_DIR); chmod -R ugo+rwx $(TRANSFER_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@-if [ -f $@ ]; then echo "$(OK) Object file created successfully: $(WHITE)./$@$(RESET)"; else echo "$(FAIL) Object file creation failed: $(WHITE)$@$(RESET)"; fi

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@-if [ -f $(NAME) ]; then echo "$(OK) Executable file created successfully: $(WHITE)$(NAME)$(RESET)"; else echo "$(FAIL) Executable file creation failed: $(WHITE)$(NAME)$(RESET)"; fi

clean:
	@-if [ ! -d $(TRANSFER_DIR) ]; then echo -n ""; else rm -rf $(TRANSFER_DIR); if [ ! -d $(TRANSFER_DIR) ]; then echo "$(OK) Transfer directory deleted successfully: $(WHITE)$(TRANSFER_DIR)$(RESET)"; else echo "$(FAIL) Transfer directory deletion failed: $(WHITE)$(TRANSFER_DIR)$(RESET)"; fi fi
	@-if [ -d $(OBJS_DIR) ]; then rm -rf $(OBJS_DIR); if [ ! -d $(OBJS_DIR) ]; then echo "$(OK) Object directory deleted successfully: $(WHITE)$(OBJS_DIR)$(RESET)"; else echo "$(FAIL) Object directory deletion failed: $(WHITE)$(OBJS_DIR)$(RESET)"; fi fi
	@-if [ ! -f *.log > /dev/null 2> /dev/null  ]; then echo -n ""; else rm -f *.log > /dev/null 2> /dev/null; if [ ! -f *.log > /dev/null 2> /dev/null ]; then echo "$(OK) Log files deleted successfully: $(WHITE)./*.log$(RESET)"; else echo "$(FAIL) Log file deletion failed: $(WHITE)./*.log$(RESET)"; fi fi

config:
	@-if [ "$(HEXCHAT_APPID)" = "" ]; then $(MAKE) install; else flatpak override --user --filesystem=home $(HEXCHAT_APPID) > /dev/null 2> /dev/null; if [ ! -d "$(HEXCHAT_DIR)" ] || [ ! -f "$(HEXCHAT_CONF)" ] || [ ! -f  "$(HEXCHAT_SERVLIST)" ]; then echo "$(FAIL) $(WHITE)Hexchat not found$(RESET)\n$(WARN) This project was developed for use with Hexchat, and may not function correctly with other IRC clients."; else if [ "$(shell cat "$(HEXCHAT_CONF)" 2> /dev/null | grep -c "irc_notice_pos = 2")" = "0" ] || [ "$(shell cat "$(HEXCHAT_CONF)" 2> /dev/null | grep -c "gui_chanlist_minusers = 1")" = "0" ] || [ "$(shell cat "$(HEXCHAT_CONF)" 2> /dev/null | grep -c "dcc_dir = $(FILES_DIR)")" = "0" ]; then sed -i 's|.*dcc_dir = .*|dcc_dir = $(FILES_DIR)|' $(HEXCHAT_CONF); sed -i 's/.*irc_notice_pos = .*/irc_notice_pos = 2/' $(HEXCHAT_CONF); sed -i 's/.*gui_chanlist_minusers = .*/gui_chanlist_minusers = 1/' $(HEXCHAT_CONF); echo "$(OK) Hexchat configured successfully"; if [ "$(shell cat $(HEXCHAT_SERVLIST) 2> /dev/null | grep -c "ft_irc")" = "0" ]; then echo "" >> $(HEXCHAT_SERVLIST); echo "$(HEXCHAT_VERSION)" >> $(HEXCHAT_SERVLIST); echo "" >> $(HEXCHAT_SERVLIST); echo "N=ft_irc" >> $(HEXCHAT_SERVLIST); echo "P=$(PASSWORD)" >> $(HEXCHAT_SERVLIST); echo "L=7" >> $(HEXCHAT_SERVLIST); echo "E=UTF-8 (Unicode)" >> $(HEXCHAT_SERVLIST); echo "F=83" >> $(HEXCHAT_SERVLIST); echo "D=0" >> $(HEXCHAT_SERVLIST); echo "S=127.0.0.1/$(PORT)" >> $(HEXCHAT_SERVLIST); echo "$(OK) Server was successfully added to Hexchat"; fi fi fi fi

fclean: clean
	@-if [ ! -f $(NAME) ]; then echo -n ""; else rm $(NAME); if [ ! -f $(NAME) ]; then echo "$(OK) Executable file deleted successfully: $(WHITE)$(NAME)$(RESET)"; else echo "$(FAIL) Executable file deletion failed: $(WHITE)$(NAME)$(RESET)"; fi fi
	@-if [ ! -f $(BOT_NAME) ]; then echo -n ""; else rm $(BOT_NAME); if [ ! -f $(BOT_NAME) ]; then echo "$(OK) Executable file deleted successfully: $(WHITE)$(BOT_NAME)$(RESET)"; else echo "$(FAIL) Executable file deletion failed: $(WHITE)$(BOT_NAME)$(RESET)"; fi fi

help:
	@-echo	"> $(WHITE)make$(RESET): - - - - - - - - compiles the program"
	@-echo	"> $(WHITE)make all$(RESET): - - - - - - compiles the program"
	@-echo	"> $(WHITE)make clean$(RESET): - - - - - removes all object files"
	@-echo	"> $(WHITE)make config$(RESET):  - - - - checks for Hexchat and configures it if found"
	@-echo	"> $(WHITE)make fclean$(RESET):  - - - - removes all object files and the executable"
	@-echo	"> $(WHITE)make help$(RESET):  - - - - - displays help message"
	@-echo	"> $(WHITE)make install$(RESET): - - - - tries to install Hexchat"
	@-echo	"> $(WHITE)make re$(RESET):  - - - - - - removes all object files and the executable, then compiles the program"
	@-echo	"> $(WHITE)make run$(RESET): - - - - - - compiles the program and runs the executable with the arguments [PORT] [PASSWORD] and Hexchat"
	@-echo	"> $(WHITE)make uninstall$(RESET): - - - tries to uninstall Hexchat"
	@-echo	"> $(WHITE)make valgrind$(RESET):  - - - recompiles the program without -fsanitize=address and runs valgrind on the executable with the arguments [PORT] [PASSWORD] and Hexchat"

install:
	@-echo "$(WARN) Trying to install Hexchat. Please wait. $(WHITE)max: 1 min$(RESET)"; nohup flatpak remote-add --user --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo 2> /dev/null; nohup flatpak repair --user 2> /dev/null; nohup flatpak uninstall --unused --user -y 2> /dev/null; nohup flatpak update --appstream --user 2> /dev/null; nohup flatpak update --user 2> /dev/null; nohup flatpak install --user flathub io.github.Hexchat -y 2> /dev/null; sleep 0.01 && rm -f nohup.out; echo "$(WARN) You might want to start hexchat to generate the configuration files."; echo "$(WARN) After that, run $(WHITE)make config$(RESET)"

re: fclean all

run: all
	@-while true; do echo -n "Do you want to start an instance of Hexchat? [y/n] "; read yn; if [ "$$yn" = "y" ]; then nohup $(HEXCHAT_EXECUTABLE) 2> /dev/null > /dev/null & true; fi; if [ "$$yn" = "n" ]; then break; fi; done
	@-sleep 0.01 && rm -f nohup.out
	@-./$(NAME) $(PORT) $(PASSWORD)

uninstall:
	@-echo "$(WARN) Trying to uninstall Hexchat. Please wait. $(WHITE)max: 30 sec$(RESET)"; nohup flatpak repair --user 2> /dev/null; nohup flatpak uninstall --unused --user -y 2> /dev/null; nohup flatpak update --appstream --user 2> /dev/null; nohup flatpak update --user 2> /dev/null; nohup flatpak uninstall --user io.github.Hexchat -y 2> /dev/null; sleep 0.01 && rm -f nohup.out

valgrind:
	@-echo "$(WARN) Recompiling without $(WHITE)-fsanitize=address$(RESET) to run valgrind"
	@$(MAKE) re CFLAGS="$(filter-out -fsanitize=address,$(CFLAGS))" NAME=$(NAME) BOT_NAME=$(BOT_NAME)
	@-while true; do echo -n "Do you want to start an instance of Hexchat? [y/n] "; read yn; if [ "$$yn" = "y" ]; then nohup $(HEXCHAT_EXECUTABLE) 2> /dev/null > /dev/null & true; fi; if [ "$$yn" = "n" ]; then break; fi; done
	@-sleep 0.01 && rm -f nohup.out
	@-valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind_$(NAME).log ./$(NAME) $(PORT) $(PASSWORD)


.PHONY: all clean config fclean help install re run valgrind