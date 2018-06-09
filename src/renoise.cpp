#include "renoise.h"
#include <zip.h>

const char* OctaveNotes[] = {
	"C-",
	"C#",
	"D-",
	"D#",
	"E-",
	"F-",
	"F#",
	"G-",
	"G#",
	"A-",
	"A#",
	"B-",
};


Renoise::Renoise() {
}


Renoise::Renoise( const char* inFile ) {
	Load(inFile);
}


bool Renoise::Load( const char* inFile ) {
	// https://libzip.org/documentation/
	zip_t* zip = zip_open(inFile, ZIP_RDONLY, nullptr);

	if ( !zip )
		return false;

//	Log("%lu", zip_get_num_entries(zip, 0));
//
//	for ( auto idx = 0; idx < zip_get_num_entries(zip, 0); ++idx ) {
//		Log("* %s", zip_get_name(zip, idx, 0));
//	}

	const char* innerFileName = "Song.xml";

	zip_stat_t zfstat;
	zip_stat(zip, innerFileName, 0, &zfstat);
	zip_file_t* zfile = zip_fopen(zip, innerFileName, 0);

	if ( !zfile )
		return false;

	char* rawDoc = new char[zfstat.size+1];
	zip_fread(zfile, rawDoc, zfstat.size);
	rawDoc[zfstat.size] = 0;	// null terminate

	zip_close(zip);

	// https://pugixml.org/docs/manual.html#access.iterators
	auto result = doc.load_string(rawDoc);
	if ( !result )
		return false;

	delete [] rawDoc;		// is it safe to delete here?

	// Parse all the patterns
	ParsePatterns();

	return true;
}


void Renoise::ParsePatterns() {
	auto patternPool = doc.child("RenoiseSong").child("PatternPool");
	auto patterns = patternPool.child("Patterns");

	int currentPattern = 0;
	for ( auto patternItr = patterns.begin(); patternItr != patterns.end(); ++patternItr ) {
		auto pattern = *patternItr; //<Pattern>

		Log("Pattern %i", currentPattern);

		patternWidth.push_back(1);		// Currently only supporting 1-line wide (ala PC Speaker)

		int numLines = atoi(pattern.child_value("NumberOfLines"));
		patternHeight.push_back(numLines);

		auto tracks = pattern.child("Tracks");

		for ( auto itrTrack = tracks.begin(); itrTrack != tracks.end(); ++itrTrack ) {
			Log("* %s", itrTrack->name());
		}
//		//Log("L: %s", pattern.child("NumberOfLines").value());

		currentPattern++;
	}


}
