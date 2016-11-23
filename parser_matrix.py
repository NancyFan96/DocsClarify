import sys
import csv
if (len(sys.argv)!= 3):
	print("usage: python parser_matrix.py <inputFile> <outputFile>")
	exit()

fout=open(sys.argv[2],'w')

csvfile = file(sys.argv[1],'rb')
reader = csv.reader(csvfile)
cnt = 0
for line in reader:
	cnt=cnt+1
	category_num = 0
	category_index=[]
	for index in range(len(line)):
		if(line[index] == '1'):
			category_num=category_num+1
			#printf(str(category_num))
			category_index.append(index+1)
	#fout.write('%d',cnt)
	#fout.write('\t')
	#fout.write('%d',category_num)
	fout.write(str(cnt)+'\t'+str(category_num))
	for number in category_index:
		fout.write(' '+str(number))
		#fout.write('%d', number)
	fout.write('\n')