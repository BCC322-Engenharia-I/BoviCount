build:
	g++ src/*.cpp -o bin/app

run:
	bin/app

clean:
	rm -f bin/*