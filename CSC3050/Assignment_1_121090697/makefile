objects = LabelTable.o phase1.o phase2.o tester.o

cc = g++

test : $(objects)
		$(cc) -o tester $(objects)

tester.o : tester.cpp LabelTable.h
		$(cc) -c tester.cpp

LabelTable.o : LabelTable.cpp LabelTable.h
		$(cc) -c LabelTable.cpp

phase1.o : phase1.cpp LabelTable.h
		$(cc) -c phase1.cpp

phase2.o : phase2.cpp LabelTable.h
		$(cc) -c phase2.cpp

clean :
		rm tester $(objects)