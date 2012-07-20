/*
 * str2map.cpp
 *
 *  Created on: Jul 20, 2012
 *      Author: joshua
 */
#include <iostream>
#include <map>
#include <string>
using namespace std;

map<string,string> resolveKeys(char* KeyString) {
	int P = 0;
	map<string,string> KeyMap;
	string Key = "";
	string Value = "";
	bool Mode = 1; // 0 = key, 1 = value
	while(KeyString[P]) {
		switch(KeyString[P]) {
		case '&':
			KeyMap[Key] = Value;
			Key = "";
			Value = "";
			Mode = 1;
			break;
		case '=':
			Mode = 0;
			break;
		default:
			if(Mode)
				Key += KeyString[P];
			else
				Value += KeyString[P];
		break;
		}
		P++;
	}
	KeyMap[Key] = Value;
	return KeyMap;
}
