#include <stdio.h>
#include <pugixml/pugixml.hpp>
#include <zip.h>

#define Log(str, ...) printf(str "\n", ## __VA_ARGS__)
#define ELog(str, ...) fprintf(stderr, str "\n", ## __VA_ARGS__)

int main( int argc, char* argv[] ) {
	const char* inFile = "TestSong.xrns";

	Log("Loading Song file...");

	// https://pugixml.org/docs/manual.html#access.iterators
//	pugi::xml_document doc;
//	auto result = doc.load_file("Song.xml");
//	if ( !result )
//		return -1;
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


	// https://libzip.org/documentation/
	zip_t* zip = zip_open(inFile, ZIP_RDONLY, nullptr);

	Log("%lu", zip_get_num_entries(zip, 0));

	for ( auto idx = 0; idx < zip_get_num_entries(zip, 0); ++idx ) {
		Log("* %s", zip_get_name(zip, idx, 0));
	}

	const char* innerFileName = "Song.xml";

	zip_stat_t zfstat;
	zip_stat(zip, innerFileName, 0, &zfstat);
	zip_file_t* zfile = zip_fopen(zip, innerFileName, 0);

	char* rawDoc = new char[zfstat.size];
	zip_fread(zfile, rawDoc, zfstat.size);

	zip_close(zip);


	pugi::xml_document doc;
	auto result = doc.load_string(rawDoc);
	if ( !result )
		return -1;

	auto song = doc.child("RenoiseSong");

	Log("%i", song.attribute("doc_version").as_int());

	auto tracks = song.child("Tracks");

	for ( auto itr = tracks.begin(); itr != tracks.end(); ++itr ) {
		Log("%s", itr->name());
	}

	delete [] rawDoc;

	return 0;
}
