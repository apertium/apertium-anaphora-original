#ifndef _PARSEREF_
#define _PARSEREF_

#include <cstdio>
#include <string>
#include <cstdlib>
#include <vector>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <unordered_map>

using namespace std;

typedef vector< vector<wstring> > acceptable_tags;

struct markable_pattern
{
	vector<wstring> name;
	int head;
};

typedef vector< vector<markable_pattern> > acceptable_patterns;

class ParseRef
{
private:
	unordered_map<wstring, acceptable_tags> parameters; //parameter name mapped to the acceptable tag lists
	unordered_map<wstring, acceptable_tags> cats; //cat name mapped to acceptable tag lists

	unordered_map<wstring, acceptable_patterns> markables; //markable name mapped to acceptable pattern lists. Also each pattern has a head == 1

public:
	void parseDoc(char *docname);
	void parseParameters (xmlDocPtr doc, xmlNodePtr cur);
	void parseParameterItem (xmlDocPtr doc, xmlNodePtr cur);

	void parseCats (xmlDocPtr doc, xmlNodePtr cur);
	void parseCatItem (xmlDocPtr doc, xmlNodePtr cur); 

	void parseMarkables (xmlDocPtr doc, xmlNodePtr cur);
	void parsePatterns (xmlDocPtr doc, xmlNodePtr cur);
	void parsePatternItem (xmlDocPtr doc, xmlNodePtr cur); 
};

#endif