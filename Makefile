EXE = gtknotebook_dnd_demo0

all: ${EXE}

gtknotebook_dnd_demo0: gtknotebook_dnd_demo0.c
	${CC} -o $@ gtknotebook_dnd_demo0.c `pkg-config --libs --cflags gtk+-2.0`

clean:
	${RM} ${EXE} *.o
