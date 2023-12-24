objects = LabelTable.o main.o instructions.o dump.o

cc = g++

main : $(objects)
		$(cc) -o main $(objects)

main.o : main.cpp instructions.h LabelTable.h
		$(cc) -c main.cpp

instructions.o : instructions.cpp instructions.h LabelTable.h
		$(cc) -c instructions.cpp

dump.o : dump.cpp dump.h
		$(cc) -c dump.cpp

LabelTable.o : LabelTable.cpp LabelTable.h
		$(cc) -c LabelTable.cpp

clean :
		rm main $(objects)