

CC = C:\Qt\Tools\mingw810_32\bin\gcc.exe
SRC = main.c
ALIBSDIR = ./
LFLAGS = -l:pdcurses.a
CFLAGS = -Wall -O0 -g
SRC = colors.c \
 	keybord.c \
  	move.c \
	winsize.c \
	resize.c \
	newwin.c \
	ignoreresize.c \
	pdce9.c \
	savedump.c \
	anycurses.c \
	scrollcmd.c \
	checkoverflow.c \
	scrollsupp.c

EXE = $(SRC:.c=.exe)
EXE_CLEAN = $(foreach file,$(EXE),$(if $(wildcard $(file)),"$(file)",))

# Проверка существования файла
# https://www.unix.com/unix-for-dummies-questions-and-answers/42744-how-can-i-check-if-directory-exists-makefile.html
# https://iga1976.wordpress.com/2018/11/27/how-do-i-check-if-file-exists-in-makefile/

# if else fi
# https://unix.stackexchange.com/questions/603997/makefile-file-exists-check-not-consistent


# Вызов функции ко всем элементам списка
# https://stackoverflow.com/questions/15977796/if-conditions-in-a-makefile-inside-a-target
# https://ftp.gnu.org/old-gnu/Manuals/make-3.80/html_node/make_79.html#SEC83
# https://stackoverflow.com/questions/68119063/makefile-applying-a-function-to-a-list

#	$(foreach file,$(EXE),$(call clean_if_exist,$(file)))

#@echo $(if 'olor'='color',"is not exists","is exists")

#	ifeq (color,color)
#		echo "$(1) is not exists"
#	else
#		echo "$(1) is exists"
#	endif

#$(foreach file,$(EXE),$(call clean_if_exist,$(file)))

all: $(EXE)

f:
	$(call test, hi bro, 14)

%.exe : %.c
	$(CC) $^ -L$(ALIBSDIR) -o $@ $(LFLAGS)

try_clean_all:
ifneq ($(strip $(EXE_CLEAN)),)
	@del $(EXE_CLEAN)
endif

.PHONY: clean

clean: try_clean_all