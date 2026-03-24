#this is the build script 
#the compiler need to be 32-bit
all:
	#Fact That I Create This In Linux
	i686-w64-mingw32-gcc $(wildcard ./*.c) proxy.def \
	-shared -s -Os -Wl,--gc-sections \
	-o ./vorbisFile.dll 
