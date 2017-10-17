//
//  ex3.cpp
//  Sandbox
//
//  Created by Alagris on 15/10/2017.
//  Copyright © 2017 alagris. All rights reserved.
//

#include "ex3.hpp"
#include <iostream>
#include "meta.h"


namespace ex3 {
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    ////parsing number x from numerical system of base n to integer
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    
    //////////////////////////////////////////////
    //functional
    //////////////////////////////////////////////
    typedef const char ARR str;
    FUNCTION<str x,int n,int o,int power,int i> DO While{
        LET int new_o=o+(x[i]-(x[i]>='A'?'A':'0'))*power;
        LET int new_power=power*n;
        LET int new_i=i-1;
        RETURN While<x,n,new_o,new_power,new_i> GET;
    };
    FUNCTION<str x,int n,int o,int power> DO While<x,n,o,power,-1>{
        RETURN o;
    };
    FUNCTION<int n,char... str> DO Parse{
        LET char x[]={str...};
        LET int len=sizeof(x)/sizeof(char);
        RETURN While<x, n, 0, 1,len-1> GET;
    };
    
    void func(){
        //FUNCTION metaprogramming does not support c-style
        //strings so we are forced to split string literals into
        //arrays of character literals
        std::cout<<Parse<10,'1','2','8','2'> GET<<"\n";
    }
    
    //////////////////////////////////////////////
    //Procedural
    //////////////////////////////////////////////
    void proc(){
        const char * const x="1832";
        const int n=10;
        int i=int(strlen(x))-1;
        int power=1;
        int o =0;
        static_assert('A'>'0',"Your platform doesn't follow ASCII standard! LOL");
        while(i>=0){
            o+=(x[i]-(x[i]>='A'?'A':'0'))*power;
            power*=n;
            i--;
        }
        std::cout<<o<<"\n";
    }
}
