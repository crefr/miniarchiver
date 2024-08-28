FILENAME = a.exe
OBJDIR = Obj/
SRCDIR = sources/
HEADDIR = headers/

CC = g++
CFLAGS = -c -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline							\
		 -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default				\
		 -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy 				\
		 -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers 	\
		 -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing 		\
		 -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE				\
		 -I./$(HEADDIR)

$(FILENAME): $(OBJDIR)main.o $(OBJDIR)archiver.o
	$(CC) $^ -o $@

$(OBJDIR)main.o: $(SRCDIR)main.cpp $(HEADDIR)archiver.h
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)archiver.o: $(SRCDIR)archiver.cpp $(HEADDIR)archiver.h
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm $(OBJDIR)*

