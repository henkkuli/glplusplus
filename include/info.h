#define TOSTRING(x) #x
#define LINESTRING2(x) TOSTRING(x)
#define LINESTRING LINESTRING2(__LINE__)
#define FILESTRING2(x) TOSTRING(x)
#define FILESTRING FILESTRING2(__FILE__)

#define INFO(format, ...) \
	printf(FILESTRING "#" LINESTRING ": " format "\n", __VA_ARGS__);