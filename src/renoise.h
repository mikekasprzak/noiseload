#pragma once
#include <lib/lib.h>
#include <pugixml/pugixml.hpp>
#include <vector>

class Renoise {
public:
	pugi::xml_document doc;
	
	std::vector<unsigned short> sequence;

	std::vector<unsigned short> patternHeight;
	std::vector<unsigned char> patternWidth;
	std::vector<unsigned char> patternFlags;
	std::vector<char*> patternData;


	Renoise();
	Renoise( const char* inFile );

	bool Load( const char* inFile );
	void ParseSequence();
	void ParsePatterns();

	void Save( const char* outFile );

	inline int GetBeatsPerMinute() {
		return atoi(doc.child("RenoiseSong").child("GlobalSongData").child_value("BeatsPerMin"));
	}
	inline int GetLinesPerBeat() {
		return atoi(doc.child("RenoiseSong").child("GlobalSongData").child_value("LinesPerBeat"));
	}
	inline int GetTicksPerLine() {
		return atoi(doc.child("RenoiseSong").child("GlobalSongData").child_value("TicksPerLine"));
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
