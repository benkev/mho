ru:	read_units.y read_units.l read_units_funcs.c read_units.h \
	MHO_Unit.cc MHO_Unit.hh
	bison -dt read_units.y
	flex -o read_units.lex.c read_units.l
	g++ -g read_units.tab.c read_units.lex.c read_units_funcs.c \
		MHO_Unit.cc -lm -o units

clean:
	rm -f read_units.tab.h read_units.tab.c read_units.lex.h read_units.lex.c

purge:
	rm -f read_units.tab.h read_units.tab.c read_units.lex.h read_units.lex.c \
		units

