#!/usr/bin/env python

fobj = open("currency_list_from_wikipedia.txt", "r")
outfile = open("currencies.json", "w")

for line in fobj:
    splitted = line.split("\t")
    
    code = splitted[0].strip()
    if code.find("["):
        code = code.split("[")[0].strip()
    
    name = splitted[3].strip()
    
    print(code + " " + name)

outfile.close()
fobj.close()
