#include <stdio.h>
#include <pugixml/pugixml.hpp>
#include <zip.h>

#define Log(str, ...) printf(str "\n", ## __VA_ARGS__)
#define ELog(str, ...) fprintf(stderr, str "\n", ## __VA_ARGS__)

int main( int argc, char* argv[] ) {
	Log("moon");
	ELog("mom");

	pugi::xml_document doc;
	auto result = doc.load_file("Song.xml");
	if ( !result )
		return -1;

	auto song = doc.child("RenoiseSong");

	Log("%i", song.attribute("doc_version").as_int());

	auto tracks = song.child("Tracks");

	for ( auto itr = tracks.begin(); itr != tracks.end(); ++itr ) {
		Log("%s", itr->name());
	}


	zip_t* zip = zip_open("TestSong.xrns", ZIP_RDONLY, nullptr);

	Log("%lu", zip_get_num_entries(zip, 0));

	for ( auto idx = 0; idx < zip_get_num_entries(zip, 0); ++idx ) {
		Log("* %s", zip_get_name(zip, idx, 0));
	}


	zip_close(zip);



	return 0;
}
