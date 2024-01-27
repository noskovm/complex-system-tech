build:
	mkdir build && cd build && cmake .. && cmake --build .

run:
	cd ./build && ./complex_system_tech

clean:
	rm -r ./build