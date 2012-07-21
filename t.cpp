
#include <iostream>
#include <string>
#include <map>
#include "str2map.hpp"
using namespace std;





int call(map<string,string> Request){
cout<<"Test";
cout<<"\n<html>\n<head>\n</head>\n<body>\n";
 for(int i = 0; i < 100; i++){
cout<<"\n<span> test ";
cout<<i;
cout<<"</span><br/>";
}cout<<"\n</body>\n</html>\n";
}

int main(int argc, char**argv){
	call(resolveKeys(argv[1]));	
}

