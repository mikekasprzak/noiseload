#pragma once
#include <lib/lib.h>
#include <pugixml/pugixml.hpp>

class Renoise {
public:
	pugi::xml_document doc;


	Renoise();
	Renoise( const char* inFile );

	bool Load( const char* inFile );
};
