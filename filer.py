#inpuut:a text file indicating lots of short documents

import sys
import nltk
from nltk.corpus import stopwords

if(len(sys.argv)!= 3):
	print("usage: python filer.py <inputData> <outputData>")
	exit()
fin = open(sys.argv[1],'r')
fout = open(sys.argv[2],'w')

def removeStopWords(originSegs):
	stops = set(stopwords.words('english'))
	resultStr = ""
	for seg in originSegs:
		seg = seg.lower()
		if seg not in stops and seg.isalpha():
			resultStr = resultStr + seg + " "
	return resultStr


document = 0

for line in open(sys.argv[1]):
	#wordlist=[]
	document = document + 1
	begin=line.find('~') + 1
	line = line[begin:]
	tmp = line.split(' ')
	tmp2 = removeStopWords(tmp)

	fout.write(str(document) + ' ' + tmp2 + '\n')
