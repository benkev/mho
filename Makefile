current_time := $(shell date "+%Y.%m.%d-%H.%M.%S")
bison_tf := bison_trace_$(current_time).txt
flex_tf := flex_trace_$(current_time).txt

ru:	read_units.y read_units.l read_units.c read_units_funcs.c read_units.h 
	bison -dt read_units.y
	flex -o read_units.lex.c read_units.l
	g++ -g read_units.c read_units.tab.c read_units.lex.c read_units_funcs.c \
		MHO_Unit.cc -lm -o ru

clean:
	rm -f read_units.tab.h read_units.tab.c read_units.lex.h read_units.lex.c ru
