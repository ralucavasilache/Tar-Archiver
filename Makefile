build: archiver

star_dust: archiver.c
	gcc archiver.c -o archiver -Wall

clean:
	rm -f archiver
