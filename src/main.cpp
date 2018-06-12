#include <lib/lib.h>
#include "renoise.h"
//#include <pugixml/pugixml.hpp>
//#include <zip.h>


int main( int argc, char* argv[] ) {
	const char* inFile = argv[1];
	//const char* inFile = "TestSong.xrns";

	Log("Loading \"%s\"...", inFile);

	Renoise noise(inFile);

//	auto song = noise.doc.child("RenoiseSong");
//	Log("Version: %i", song.attribute("doc_version").as_int());
//
//	auto header = song.child("GlobalSongData");
//	Log("Title: %s", header.child_value("SongName"));
//	Log("BPM: %i", atoi(header.child_value("BeatsPerMin")));		// can't use .to_blah() methods on non-attributes

	Log("Name: %s", noise.GetName());
	Log("Artist: %s", noise.GetArtist());
	Log("BPM: %i", noise.GetBeatsPerMinute());

	noise.Save(argv[2]);

	//Log("%i", song.attribute("doc_version").as_int());

//	auto tracks = song.child("Tracks");
//
//	for ( auto itr = tracks.begin(); itr != tracks.end(); ++itr ) {
//		Log("%s", itr->name());
//	}

//
//	// https://libzip.org/documentation/
//	zip_t* zip = zip_open(inFile, ZIP_RDONLY, nullptr);
//
//	Log("%lu", zip_get_num_entries(zip, 0));
//
//	for ( auto idx = 0; idx < zip_get_num_entries(zip, 0); ++idx ) {
//		Log("* %s", zip_get_name(zip, idx, 0));
//	}
//
//	const char* innerFileName = "Song.xml";
//
//	zip_stat_t zfstat;
//	zip_stat(zip, innerFileName, 0, &zfstat);
//	zip_file_t* zfile = zip_fopen(zip, innerFileName, 0);
//
//	char* rawDoc = new char[zfstat.size+1];
//	zip_fread(zfile, rawDoc, zfstat.size);
//	rawDoc[zfstat.size] = 0;	// null terminate
//
//	zip_close(zip);
//
//
//	// https://pugixml.org/docs/manual.html#access.iterators
//	pugi::xml_document doc;
//	auto result = doc.load_string(rawDoc);
//	if ( !result )
//		return -1;
//
//	delete [] rawDoc;		// is it safe to delete here?
//
//	auto song = doc.child("RenoiseSong");
//
//	Log("%i", song.attribute("doc_version").as_int());
//
//	auto tracks = song.child("Tracks");
//
//	for ( auto itr = tracks.begin(); itr != tracks.end(); ++itr ) {
//		Log("%s", itr->name());
//	}
//
//	//delete [] rawDoc;

	return 0;
}
