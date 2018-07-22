#include <lib/lib.h>
#include "renoise.h"


int main( int argc, char* argv[] ) {
	const char* inFile = argv[1];
	
	if ( argc < 3 ) {
		Log("Usage: %s infile.xrns outfile.bin\n", argv[0]);
		return -1;
	}

	Log("Loading \"%s\"...", inFile);

	Renoise noise(inFile);
	
	Log("Name: %s", noise.GetName());
	Log("Artist: %s", noise.GetArtist());
	Log("BPM: %i", noise.GetBeatsPerMinute());

	noise.Save(argv[2]);

	return 0;
}
