#include "pattern_ref.h"
#include "parse_ref.h"

#include<string>
#include<vector>
#include<deque>
#include<iostream>

using namespace std;

void print_markable(acceptable_patterns inp)
{
	for(acceptable_patterns::iterator i = inp.begin(); i != inp.end(); i++)
	{
		cerr <<"Pattern:\n";

		for(vector<markable_pattern>::iterator j = (*i).begin(); j != (*i).end(); j++)
		{
			wcerr << (*j).name;
			cerr << "\n";
		}
	}
}

int contains(vector<wstring> tags, wstring tag)
{
	if(std::find(tags.begin(), tags.end(), tag) != tags.end())
		return 1;
	else
		return 0;
}

int contains_any(vector<wstring> tags, vector<wstring> candidates)
{
	for(vector<wstring>::iterator it=candidates.begin();it!=candidates.end();++it)
	{
		if(std::find(tags.begin(), tags.end(), *it) != tags.end())
			return 1; //if any of the tags in candidates matches the tags list
	}

	return 0; //if no matches
}

int check_acceptable_tags(vector<wstring> input_tags, acceptable_tags check_tags) //all tags in any tag list in check_tags must exist in input_tags
{
	for (acceptable_tags::iterator i = check_tags.begin(); i != check_tags.end(); ++i)
	{
		
		int flag_contains_all = 1;

		vector<wstring> temp_tags = *i;

		for(std::vector<wstring>::iterator j = temp_tags.begin(); j != temp_tags.end(); ++j)
		{

			if(*j == L"*") //ignore * in the tags list
				continue;

			if(!contains(input_tags, *j)) //if the required tag is NOT in the input LU tags
			{
				flag_contains_all = 0;
				break;
			}
			/*
			else
			{
				cerr << "FoundTag:";
				wcerr << *j;
				cerr <<"\n";
			}
			*/
		}

		if(flag_contains_all == 1) //if any tag list fully matched
			return 1; //else continue to next tag list
	}

	return 0; //if it didn't return 1 then no tag list was fully matched
}

//check_acceptable_patterns(vector)


deque< vector<unique_LU> > add_properties(deque< vector<unique_LU> > context, ParseRef ref_file)
{
	unordered_map<wstring, acceptable_patterns> ref_markables = ref_file.get_markables();
	unordered_map<wstring, acceptable_tags> ref_cats = ref_file.get_cats();

	for (unordered_map<wstring, acceptable_patterns>::iterator it = ref_markables.begin(); it != ref_markables.end(); it++ ) //go through markables defined in xml file
	{
		//for each markable
		wstring markable_name = it->first;
		acceptable_patterns patterns_list = it->second;

		//print_markable(patterns_list);

		for(acceptable_patterns::iterator i = patterns_list.begin(); i!=patterns_list.end(); ++i) //go through patterns in the markable
		{
			//for each pattern
			vector<markable_pattern> current_pattern = *i;
			int len_pattern = current_pattern.size();

			for(deque< vector<unique_LU> >::iterator m = context.begin(); m!=context.end(); ++m) //go through sentences in the queue of context
			{
				if(len_pattern > (*m).size()) //if pattern is longer then sentence length then skip
					continue;

				for (vector<unique_LU>::iterator n = (*m).begin(); n+len_pattern-1 !=(*m).end(); ++n) //go through LUs in sentence to look for the pattern with a sliding window of size = pattern length
				{
					int match_flag = 0;

					for(int x = 0; x < len_pattern; ++x)
					{
						//this is the window -- check if pattern matches

						acceptable_tags pattern_item_tags = ref_cats[current_pattern[x].name]; //get pattern item tags from def-cats

						if(check_acceptable_tags((*(n+x)).pos_tags, pattern_item_tags)) //comparing current LU tags to pattern tags
						{
							match_flag = 1;

						}
						else
						{
							match_flag = 0;
							break;
						}

						//wcerr << (*(n+x)).wordform;
					}

					if(match_flag == 1) //if the entire pattern matched
					{
						//Add Property to the LUs
						/*
						cerr << "\n";
						wcerr << markable_name;
						cerr << " Pattern Matched at: ";
						wcerr << (*n).wordform;
						cerr << "\n";
						*/

						for(int x = 0; x < len_pattern; ++x)
						{
							((*(n+x)).properties).push_back(markable_name); //add markable name to properties

							if(current_pattern[x].head == 1)
							{
								((*(n+x)).properties).push_back(L"head"); // add "head" to properties
							}
						}

					}
				}
			}
		}
	}

	return context;
}

