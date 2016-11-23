//
//  count.hpp
//  classifyDocs
//
//  Created by Nancy Fan on 16/11/13.
//  Copyright © 2016年 Nancy Fan. All rights reserved.
//

#ifndef count_hpp
#define count_hpp

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <math.h>

using namespace std;

typedef pair<string, long double> PAIR;

#define SIZE 2000
#define TYPES 22
#define DOC_SIZE 26000
#define TYPE_SIZE 30
#define WORD_SIZE 24000
#define BEST 25
#define ENLARG 100000
#define MAX(x,y) ((x>y)? (x) : (y))


struct CmpByValue {
    bool operator()(const PAIR& lhs, const PAIR& rhs) {
        return lhs.second > rhs.second;
    }
};

int countSelect();


#endif /* count_hpp */
