headers = "
#ifndef _CALL_H_
#define _CALL_H_
#include <iostream>
#include \"helper.h\"
using namespace std;
"
source = ""
source +=headers+"\n"
source += "int call(Map Request){"
fname =  ARGV[0]
contents = File.open(fname,"rb").read
sections = contents.split("<%");
msections = []
sections.each do |o|
	o.split("%>").each do |s|
		msections << s;
	end
end
i=1
if contents.strip[0..1] == "<%"
	i=-1
end
firsttype = i;
cppsections = []
htmlsections = []
msections.each do |s|
	if i == -1
		cppsections << s
	else
		htmlsections << s
	end
	i*=-1
end
i = firsttype;
v = 1
c = 0
msections.each do
	begin
		if i == -1
			val = ""
			if cppsections[c][0]=='='
				val = "cout<<"+cppsections[c][1..-1]+";\n"
			else
				val = cppsections[c]
			end
			source += val
		else 
			source += "cout<<\"" +htmlsections[c].gsub("\"","\\\"").gsub("\n","\\n")+"\";\n";
		end
		if v == 2
			v = 0
			c+=1
		end
		v+=1
	rescue
	end
	i *= -1
end	

source += "}
#endif
"
puts source
