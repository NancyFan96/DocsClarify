//
//  main.cpp
//  classifyDocs
//
//  Created by Nancy Fan on 16/11/13.
//  Copyright © 2016年 Nancy Fan. All rights reserved.
//

#include <iostream>
#include "count.hpp"



extern map <string, int> BestWords;
extern map <int, string>BestWordsRe;
extern long double base_prob[BEST][TYPE_SIZE*2];
extern long double category_prob[TYPE_SIZE];
extern int TotalDoc;

vector<string> wordsBank;


int main() {
    if(countSelect()==0){
        cout << "TRAINGING SUCCESS...\n"<< endl;
    }
    else{
        cout << "TRAINGING ERRO!\n"<< endl;
    }
    
    
    for(int i = 0; i < BEST; i++){
        for(int j = 1; j <= 2* TYPES; j++){
            cout << base_prob[i][j]<<"\t";
        }
        cout << endl << endl;
    }
    
    FILE * fp = fopen("../Data/ans.txt", "w");
    FILE * fin1 = fopen("../Data/test_filered_data.txt","r");
    int docID = 0;

    cout << "Start classify..." << endl;
    while(fscanf(fin1, "%d", &docID)!= EOF){
        vector<string> eigenwords;
        int eigen_count[BEST];
        memset(eigen_count, 0, sizeof(int)*BEST);
        char *word;
        char buf[SIZE];
        char sep[]=" \n\f\v\t";

        //cout << "Processing " << docID << endl;
        
        fgets(buf,SIZE-1,fin1);
        for(word=strtok(buf,sep); word!=NULL; word=strtok(NULL,sep)){
            //printf("%s\n",word);
            string ww="";
            ww +=word;
            wordsBank.push_back(ww);
        }
        //cout << docID << endl;

        
        fprintf(fp, "%d\t", docID);
        for(int j = 1; j <=TYPES; j++){
            long double yes = (category_prob[j]);
            long double no = 1.0*ENLARG - (category_prob[j]);
            map <string, int> :: iterator it = BestWords.begin();
            while(it != BestWords.end()){
                string teststr = it->first;
                int teststrid = it->second;
                //cout << teststr << teststrid << endl;
                vector <string> :: iterator findit = find(wordsBank.begin(), wordsBank.end(), teststr);
                if(findit != wordsBank.end()){
                    eigenwords.push_back(teststr);
                    eigen_count[teststrid]++;
                    if(eigen_count[teststrid]==1){
                        yes += (base_prob[teststrid][2*j-1]);
                        no += (base_prob[teststrid][2*j]);
                    }
                }
                it++;
             }
            if(yes>no)
                fprintf(fp, "TYPE %d\t", j);
        }
        fprintf(fp, "\n");
        
    }
    fclose(fin1);
    fclose(fp);

    return 0;
}
