#pragma once
#include <lib/lib.h>
#include <pugixml/pugixml.hpp>
#include <vector>

class Renoise {
public:
	pugi::xml_document doc;


	std::vector<int> patternWidth;
	std::vector<int> patternHeight;
	std::vector<char*> patternData;


	Renoise();
	Renoise( const char* inFile );

	bool Load( const char* inFile );
	void ParsePatterns();

	inline int GetBeatsPerMinute() {
		return atoi(doc.child("RenoiseSong").child("GlobalSongData").child_value("BeatsPerMin"));
	}
	inline int GetSignatureNumerator() {
		return atoi(doc.child("RenoiseSong").child("GlobalSongData").child_value("SignatureNumerator"));
	}
	inline int GetSignatureDenominator() {
		return atoi(doc.child("RenoiseSong").child("GlobalSongData").child_value("SignatureDenominator"));
	}

	inline const char* GetName() {
		return doc.child("RenoiseSong").child("GlobalSongData").child_value("SongName");
	}
	inline const char* GetArtist() {
		return doc.child("RenoiseSong").child("GlobalSongData").child_value("Artist");
	}


};
