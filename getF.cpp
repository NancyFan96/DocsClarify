#include<iostream>
#include<fstream>
#include<vector>
#include<map>

using namespace std;

int main()
{
	ifstream fin1("Data/TestTruth.txt");
	ifstream fin2("ans.txt");
	ofstream fout("FVALUE.txt");

	map<int, vector<int> >mapper1;
	map<int, vector<int> >mapper2;

	int id;
	int class_;
	int num;
	int total1 = 0;
	int total2 = 0;
	int common = 0;
	double precision;
	double recall;
	double f;


	while(fin1 >> id)
	{
		fin1 >> num;
		total1 += num;
		for (int i = 0; i < num; i++)
		{
			fin1 >> class_;
			mapper1[id].push_back(class_);
		}
	}

	while(fin2 >> id)
	{
		fin2 >> num;
		total2 += num;
		for (int i = 0; i < num; i++)
		{
			fin2 >> class_;
			vector<int>temp = mapper1[id];
			mapper2[id].push_back(class_);
			vector<int>::iterator iter = find(temp.begin(), temp.end(), class_); 
			if (iter != temp.end())
				common++;
		}
	}

     precision = common*1.0/total2;
     recall = common*1.0/total1;

    f = 2 * precision * recall /(precision + recall);

    fout << "total true class number is " << total1<< endl;
    fout << "total class number we predict is " << total2 << endl;
    fout << "correct class number we predict is " << common << endl; 
    fout << "precision is " << precision << endl;
    fout << "recall is " << recall << endl;
    fout << "f value is " << f << endl;


	return 0;
}