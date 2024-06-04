all: runCumulant

runCumulant: ECorr.o Cumulant.cpp ReweightTool.cxx NpartLoader.cxx
	g++ -std=c++17 $^ -o $@ `root-config --libs --cflags`

duoCBWC: duoCBWC.cpp ReweightTool.cxx 
	# g++ -std=c++11 $^ -o $@ `root-config --libs --cflags`
	@echo "Currently under construction, use runCumulant instead"

ECorr.o: ECorr.cpp
	g++ -c $^ `root-config --libs --cflags` -std=c++17