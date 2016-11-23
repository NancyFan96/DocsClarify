//
//  count.cpp
//  classifyDocs
//
//  Created by Nancy Fan on 16/11/13.
//  Copyright © 2016年 Nancy Fan. All rights reserved.
//

#include "count.hpp"
map <string, int> word2WID;
map <string, long double> NiceWords;
map <string, int> BestWords;
map <int, string>BestWordsRe;
bool HasCountWord[WORD_SIZE][TYPE_SIZE];
int  word2category[DOC_SIZE][TYPE_SIZE];
long double wordRank[WORD_SIZE][TYPE_SIZE];
long double probability[WORD_SIZE][TYPE_SIZE*2];
long double base_prob[BEST][TYPE_SIZE*2];
long double category_prob[TYPE_SIZE];
int TotalWord = 0;
int TotalDoc = 0;

//#define DEBUG

int countSelect()
{
    int docID, docCategoryNum;
    int IsINCategory[30];
    FILE * fin1 = fopen("../Data/train_filered_data.txt","r");
    FILE * fin2 = fopen("../Data/train_result.txt", "r");
    FILE * fout1 = fopen("../Data/select_meta.txt", "w");
    FILE * fout2 = fopen("../Data/select_result.txt", "w");
    int countDocsInCategory[30];
    char *word;
    char buf[SIZE];
    char sep[]=" \n\f\v\t";
    
    cout << "Start counting..."<< endl;
    
    memset(word2category, 0, sizeof(int)*DOC_SIZE*TYPE_SIZE);
    memset(countDocsInCategory, 0, sizeof(int)*TYPE_SIZE);
    
    while(fscanf(fin1, "%d", &docID)!= EOF){
        fscanf(fin2, "%d%d", &docID, &docCategoryNum);
        
        //cout << "Doc" << docID << endl;
        
        
        for(int i = 0; i < docCategoryNum; i++){
            fscanf(fin2,"%d",&IsINCategory[i]);
            countDocsInCategory[IsINCategory[i]]++;
            //cout << IsINCategory[i]<< endl;
        }
        memset(HasCountWord,0,sizeof(bool)*WORD_SIZE*TYPE_SIZE);
        fgets(buf,SIZE-1,fin1);
        for(word=strtok(buf,sep); word!=NULL; word=strtok(NULL,sep)){
            //printf("%s\n",word);
            string ww="";
            ww +=word;
            //cout << "word.."<< TotalDoc<< endl;
            //wordsBank[TotalDoc].push_back(ww);
            //cout << "word.."<< endl;

            if(word2WID.count(ww) == 0){
                word2WID[ww] = TotalWord;
                TotalWord++;
            }
            for(int i = 0; i < docCategoryNum; i++){
                int CategoryID = IsINCategory[i];
                int WordID = word2WID[ww];
                
                //cout << WordID << " " << CategoryID << endl;
                if(HasCountWord[WordID][0]==false){
                    word2category[WordID][0]++;
                    HasCountWord[WordID][0]=true;
                }
                if(HasCountWord[WordID][CategoryID] == false){
                    word2category[WordID][CategoryID]++;
                    HasCountWord[WordID][CategoryID] = true;
                }
                
                //cout << ww  << word2WID[ww]<<endl;
                //cout <<word2category[word2WID[ww]][IsINCategory[i]]<<endl;
            }
            
        }
        //cout << "word.."<< endl;

        TotalDoc++;
        //if(TotalDoc == 2) break;
    }
    fclose(fin1);
    fclose(fin2);
    
#ifdef DEBUG
    cout << "TotalDoc: " << TotalDoc << endl;
    
    for(int i = 0; i < TYPES; i++){
        cout << countDocsInCategory[i]<< endl;
    }
#endif
    
    fprintf(fout1, "WORDIF(WORD):\tNumbersOfDocsHaveThisWords\t...NumbersAppearInEachTypes\n");
    fprintf(fout2, "WORDIF(WORD):\tScoresGotInEachTypes\n");
    
    map<string, int>::iterator it = word2WID.begin();
    for(it = word2WID.begin(); it != word2WID.end(); it++){
        string cur_word = it->first;
        int cur_wordID = it->second;
        int M1,M2, A, B, C, D;
        long double BestScore = 0;
        
        fprintf(fout1, "%d(%s):\t", it->second, it->first.c_str());
       	fprintf(fout2, "%d(%s):\t", it->second, it->first.c_str());
        for(int i = 0; i <= TYPES; i++){
            fprintf(fout1, "%d\t", word2category[cur_wordID][i]);
            
            if(i == 0){
                M2 = word2category[cur_wordID][0];
                continue;
            }
            M1 = countDocsInCategory[i];
            A = word2category[cur_wordID][i];
            B = M2-A;
            C = M1-A;
            D = TotalDoc - M1 - B;
            long double tmpMul = (1.0*A*D-B*C);
            long double X = tmpMul*tmpMul/(1.0*M2*M1*(TotalDoc-M1)*(TotalDoc-M2))*TotalDoc;
            
            probability[cur_wordID][2*i-1] = 1.0*ENLARG*A/M1;
            probability[cur_wordID][2*i] = 1.0*ENLARG*B/(TotalDoc-M1);
            if(probability[cur_wordID][2*i-1]<0||probability[cur_wordID][2*i]<0)
            {
                printf("A=%d, B = %d, C = %d, D = %d, M1 = %d, M2 = %d, TotalDoc = %d\n", A, B, C, D, M1, M2, TotalDoc);
                cout << cur_word << endl;
                cout << probability[cur_wordID][2*i-1] << probability[cur_wordID][2*i-1]<< endl;
                return -1;
            }
            category_prob[i] = 1.0*ENLARG*M1/TotalDoc;
            
            BestScore = MAX(X, BestScore);
            //if(A < 0 || B < 0 || C < 0 || D < 0)
            //printf("A=%d, B = %d, C = %d, D = %d, M1 = %d, M2 = %d, TotalDoc = %d\n", A, B, C, D, M1, M2, TotalDoc);
            //if(X<0)
            //printf("%lf, %lf\n", tmpMul, X);
            fprintf(fout2, "%Lf\t", X);
        }
        //printf("BestScore for word \"%s\": %lf\n", it->first.c_str(), BestScore);
        if(NiceWords.count(it->first)==0){
            NiceWords[it->first]=BestScore;
        }
        else{
            NiceWords[it->first]=MAX(BestScore, NiceWords[it->first]);
        }
        cur_wordID++;
        fprintf(fout1, "\n");
        fprintf(fout2, "\n");
        
    }
    fclose(fout1);
    fclose(fout2);

    cout << "Chose..." << endl;
   
    FILE * fp = fopen("../Data/nicewords.txt","w");
    vector<PAIR> word_score_vec(NiceWords.begin(), NiceWords.end());
    sort(word_score_vec.begin(), word_score_vec.end(), CmpByValue());
    for (int i = 0; i != word_score_vec.size(); i++) {
        //cout << word_score_vec[i].first << "\t" << word_score_vec[i].second << endl;
        fprintf(fp, "%s(%d)\t%Lf\n",word_score_vec[i].first.c_str(), word2WID[word_score_vec[i].first], word_score_vec[i].second);
    }
    fclose(fp);
    
    cout << "Chose..." << endl;
    
    fp = fopen("../Data/basic_table.txt", "w");
    fprintf(fp,"Word(WORDID): TYPEID type: yes PROB_YES(*ENLARG), no PROB_NO(*ENLARG)\t...\n");
    for (int i = 0; i < BEST; i++) {
        string word = word_score_vec[i].first;
        int wordid = word2WID[word];
        int newid = i;
        long double p_yes, p_no;
        
        BestWords[word]=newid;
        BestWordsRe[newid] = word;
        fprintf(fp, "%s(%d):\n",word.c_str(), wordid);
        for(int j = 1; j <= TYPES; j++){
            p_yes = probability[wordid][2*j-1];
            p_no = probability[wordid][2*j];
            fprintf(fp," %d type: yes %Lf, no %Lf\n", j, p_yes, p_no);
            //cout << newid << endl;
            base_prob[newid][2*j-1] = (p_yes);
            base_prob[newid][2*j] = (p_no);
            //base_prob[newid][2*j-1] = log(p_yes);
            //base_prob[newid][2*j] = log(p_no);
            //printf("%Lf, %Lf\n", p_yes, p_no);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
    
    
    return 0;
}
