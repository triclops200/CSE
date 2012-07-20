
#ifndef _CALL_H_
#define _CALL_H_
#include <iostream>
#include "helper.h"
#include "str2map.hpp"
using namespace std;

int main(){
cout << resolveKeys("TEST=1&TEST2=2")["TEST2"]<<"\n";
cout<<"Test";
cout<<"\n<html>\n<head>\n</head>\n<body>\n";
 for(int i = 0; i < 100; i++){
cout<<"\n<span> test ";
cout<<i;
cout<<"</span><br/>";
}cout<<"\n</body>\n</html>\n";
}
#endif
