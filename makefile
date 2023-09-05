main: main.o interaction.o vmware.o utils.o hardware.o API.o timing.o analysis.o
	g++ main.o utils.o interaction.o vmware.o hardware.o API.o timing.o analysis.o -o main -liphlpapi -lole32 -loleaut32 -lWbemuuid

main.o: main.cpp
	g++ -c main.cpp

utils.o: utils.cpp
	g++ -c utils.cpp

interaction.o: human_interaction/interaction.cpp
	g++ -c human_interaction/interaction.cpp 

vmware.o: vmware_detection/vmware.cpp
	g++ -c vmware_detection/vmware.cpp

hardware.o: hardware_detection/hardware.cpp
	g++ -c hardware_detection/hardware.cpp

timing.o: timing/timing.cpp
	g++ -c timing/timing.cpp

API.o: API.cpp
	g++ -c API.cpp

analysis.o: analysis/analysis.cpp
	g++ -c analysis/analysis.cpp

run:
	main

clean: 
	del *.o
