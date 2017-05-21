OBJS = main.o common.o sydan.o sydan_filled.o hiutale.o snow.o sydan_parts.o \
	tausta.o
# sydan_parts.o hiutale.o \
#	sydan_border.o

main: $(OBJS) Makefile
	gcc -o main $(OBJS) -lglut -lGL -lGLU -lm

common.o: common.cc common.hh Makefile
	gcc -c common.cc

main.o: main.cc Makefile
	gcc -c main.cc

sydan.o: sydan.cc sydan.hh Makefile
	gcc -c sydan.cc

sydan_filled.o: sydan_filled.cc sydan_filled.hh Makefile
	gcc -c sydan_filled.cc

sydan_parts.o: sydan_parts.cc sydan_parts.hh Makefile
	gcc -c sydan_parts.cc

hiutale.o: hiutale.cc hiutale.hh Makefile
	gcc -c hiutale.cc

tausta.o: tausta.cc tausta.hh Makefile
	gcc -c tausta.cc

snow.o: snow.cc snow.hh Makefile
	gcc -c snow.cc

clean:
	rm -f *.o main *\# *~
