#include <stdio.h>
#include <pugixml/pugixml.hpp>

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



	return 0;
}
