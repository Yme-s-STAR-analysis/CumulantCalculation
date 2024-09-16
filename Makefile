all: runCumulant

runCumulant: ECorr.o Cumulant.cpp ReweightTool.cxx CentManager.cxx
	g++ $^ -o $@ `root-config --libs --cflags` -std=c++11

runCumulant4: ECorr4.o Cumulant4.cpp ReweightTool.cxx CentManager.cxx
	g++ $^ -o $@ `root-config --libs --cflags` -std=c++11

# duoCBWC: duoCBWC.cpp ReweightTool.cxx  NpartLoader.cxx
# 	g++ $^ -o $@ `root-config --libs --cflags` -std=c++11 

duoCBWC: 
	@echo "Currently disabled!"

ECorr.o: ECorr.cpp
	g++ -c $^ `root-config --libs --cflags` -std=c++11

ECorr4.o: ECorr4.cpp
	g++ -c $^ `root-config --libs --cflags` -std=c++11