#include "renoise.h"
#include <string.h>
#include <zip.h>

#include <stdio.h>

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


// MikeTracker Byte Format (FIRST BYTE)
// 0         : null (no change/placeholder)
// 1-97      : 10 octaves of notes from C-1 to B-9 (was 1-121)
// 98-121	 : ??? (23 values)
// 122-126   : ??? (5 values)
// 127       : NOTE OFF (0x7F)
// 0x80      : ??? (possibly 1 bit to say something)

// MikeTracker Byte Format (SECOND BYTE)
// 0x0-0xF   : 16 levels of volume
// 0x10-0x40 : 3 levels of Vibrato (or 4 if you count 0 as no vibrato)
// 0x50-0x80 : 4 levels of pitch-up
// 0x90-0xB0 : 4 levels of pitch-down
// 0xC0-0xF0 : ??? (4 values)

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

	// Parse the Sequence
	ParseSequence();

	// Parse all the patterns
	ParsePatterns();

	return true;
}


void Renoise::ParseSequence() {
	auto patternSequence = doc.child("RenoiseSong").child("PatternSequence");
	auto sequenceEntries = patternSequence.child("SequenceEntries");
	
	for ( auto sequenceItr = sequenceEntries.begin(); sequenceItr != sequenceEntries.end(); ++sequenceItr ) {
		auto entry = *sequenceItr; // <SequenceEntry>
		
		sequence.push_back(atoi(entry.child_value("Pattern")));
	}
}


void Renoise::ParsePatterns() {
	auto patternPool = doc.child("RenoiseSong").child("PatternPool");
	auto patterns = patternPool.child("Patterns");

	int currentPattern = 0;
	for ( auto patternItr = patterns.begin(); patternItr != patterns.end(); ++patternItr ) {
		auto pattern = *patternItr; //<Pattern>

		Log("Pattern %i", currentPattern);

		int numLines = atoi(pattern.child_value("NumberOfLines"));
		patternHeight.push_back(numLines);

		int numColumns = 1;						// Currently only supporting 1-line wide (ala PC Speaker)
		patternWidth.push_back(numColumns);

		int numParts = 1;
		patternFlags.push_back(0);

		patternData.push_back(new char[numLines * numColumns * numParts]);
		memset(patternData.back(), 0, numLines * numColumns * numParts);

		auto tracks = pattern.child("Tracks");
		for ( auto itrTrack = tracks.begin(); itrTrack != tracks.end(); ++itrTrack ) {
			Log("* %s", itrTrack->name());

			auto lines = itrTrack->child("Lines");
			for ( auto itrLines = lines.begin(); itrLines != lines.end(); ++itrLines ) {
				int index = itrLines->attribute("index").as_int();

				//Log("* %s: %i", itrLines->name(), index);

				// TODO: make sure this actually
				int note = 0;
				auto noteColumns = itrLines->child("NoteColumns");
				for ( auto itrNoteColumns = noteColumns.begin(); itrNoteColumns != noteColumns.end(); ++itrNoteColumns ) {
					const char* noteValue = itrNoteColumns->child_value("Note");
					int noteInstrument = atoi(itrNoteColumns->child_value("Instrument"));

					//Log("[%i] %s:%i", note, noteValue, noteInstrument);

					size_t patternIndex = (index * numColumns) + (note * numParts); // + part; // i.e. +0 for note, +1 for effect
					if ( strcmp("OFF", noteValue) == 0 ) {
						patternData.back()[patternIndex+0] = 127; // NOTE OFF
						//patternData[patternIndex+1] = 127;
					}
					else if ( (noteValue[0] >= 'A') && (noteValue[0] <= 'G') && ((noteValue[1] == '-') || (noteValue[1] == '#')) && (noteValue[2] >= '1') && (noteValue[2] <= '8') ) {
						int actualNote = 0;

						// Scan table for note
						for ( size_t idx = 0; idx < 12; ++idx ) {
							if ( strncmp(OctaveNotes[idx], noteValue, 2) ) {
								actualNote = idx+1;
								break;
							}
						}

						if ( !actualNote )
							ELog("ERROR! Unknown note \"%s\"", noteValue);

						actualNote += (noteValue[2] - 1 - '0') * 12;

						patternData.back()[patternIndex+0] = actualNote;
					}
					else {
						ELog("ERROR! Unknown note \"%s\"", noteValue);
					}

					note++;
				}

			}
		}
//		//Log("L: %s", pattern.child("NumberOfLines").value());

		currentPattern++;
	}
}

void Renoise::Save( const char* outFile ) {
	FILE* file = fopen(outFile, "w");
	if ( file ) {
		int total = 0;
		unsigned short blockSize = 0;
		
		// NB. fwrite: the return value is the count, so rather than setting the size, we set the count

		// File
		size_t fileHeaderSize = 2+2;
		blockSize = fileHeaderSize;
		unsigned short beats = (GetBeatsPerMinute() & 0xFFF) | (((GetLinesPerBeat() - 1) & 0xF)<<12);
		
		total += fwrite(&blockSize, 1, sizeof(unsigned short), file);
		total += fwrite(&beats, 1, sizeof(unsigned short), file);
		
		// Sequence
		size_t sequenceHeaderSize = 2;
		unsigned short sequenceSize = sequence.size() * sizeof(unsigned short);
		blockSize = sequenceHeaderSize + sequenceSize;

		total += fwrite(&blockSize, 1, sizeof(unsigned short), file);
		total += fwrite(&sequence[0], 1, sequenceSize, file);

		// Pattern(s)
		size_t patternHeaderSize = 2+2;
		for ( size_t idx = 0; idx < patternData.size(); ++idx ) {
			size_t patternSize = patternWidth[idx] * patternHeight[idx] * 1;
			unsigned short heightWidth = ((patternHeight[idx] - 1) & 0x3FF) | (((patternWidth[idx] - 1) & 0xF) << 10) | (0 << 14);
			blockSize = patternHeaderSize + patternSize;
			
			total += fwrite(&blockSize, 1, sizeof(unsigned short), file);
			total += fwrite(&heightWidth, 1, sizeof(unsigned short), file);

			total += fwrite(patternData[idx], 1, patternSize, file);
		}

		Log("%u bytes written", total);
		fclose(file);
	}
	else {
		exit(1);
	}
}
