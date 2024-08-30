all: runCumulant

runCumulant: ECorr.o Cumulant.cpp ReweightTool.cxx CentManager.cxx
	g++ $^ -o $@ `root-config --libs --cflags` -std=c++11

# duoCBWC: duoCBWC.cpp ReweightTool.cxx  NpartLoader.cxx
# 	g++ $^ -o $@ `root-config --libs --cflags` -std=c++11 

duoCBWC: 
	@echo "Currently disabled!"

ECorr.o: ECorr.cpp
	g++ -c $^ `root-config --libs --cflags` -std=c++11