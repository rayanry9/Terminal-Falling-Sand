main:
	if [ ! -d "build" ]; then mkdir build/; fi 
	g++ main.cpp -o build/FallingSand -lncursesw	

clean:
	rm -rf build/
