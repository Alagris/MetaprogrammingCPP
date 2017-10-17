//
//  ex5.cpp
//  Sandbox
//
//  Created by Alagris on 15/10/2017.
//  Copyright © 2017 alagris. All rights reserved.
//

#include "ex5.hpp"
#include <iostream>
#include "meta.h"


namespace ex5 {
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    ////insertion sort of n POSITIVE numbers from array x
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    
    //////////////////////////////////////////////
    //functional
    //////////////////////////////////////////////
    
    
    FUNCTION<int j,typename x,int n,int max=j-1,bool jLessThanN=j<n> DO FindMax;
    FUNCTION<int j,typename x,int n,int max> DO FindMax<j,x,n,max,true>{
        LET int new_max=x GET[max]>x GET[j]?max:j;
        LET int new_j=j+1;
        RETURN FindMax<new_j,x,n,new_max, new_j<n > GET;
    };
    FUNCTION<int j,typename x,int n,int max> DO FindMax<j,x,n,max,false>{
        RETURN max;
    };
    FUNCTION<typename x,int n,int i=0,bool iLessThanNMinus1=i<n-1> DO For;
    FUNCTION<typename x,int n,int i> DO For<x,n,i,true>{
        LET int max=FindMax<i+1, x, n> GET;
        LET int new_i=i+1;
        FORWARD meta::arrays::SwapArray<int,n, i, max, x> GET new_x;
        FORWARD For<new_x,n,new_i> GET OUT;
    };
    FUNCTION<typename x,int n,int i> DO For<x,n,i,false>{
        DATA x OUT;
    };
    FUNCTION<CNST int... arr> DO Sort{
        DATA meta::Array<int,arr...> x;
        LET int n=x::len;
        FORWARD For<x,n> GET OUT;
    };
    
    void func(){
        typedef Sort<8,43,1,87,12,7> sorted;
        const int*const a=sorted GET GET;
        int len=sorted::n;
        
        for(int i=0;i<len;i++){
            std::cout<<a[i]<<" ";
        }
        std::cout<<"\n";
    }
    
    //////////////////////////////////////////////
    //Procedural
    //////////////////////////////////////////////
    void proc(){
        int x[]={5,9,1,5,0,2};
        int n=sizeof(x)/sizeof(int);
        for(int i=0;i<n;i++){
            int max=i;
            for(int j=i+1;j<n;j++){
                if(x[j]>x[max])max=j;
            }
            const int tmp=x[i];
            x[i]=x[max];
            x[max]=tmp;
            std::cout<<x[i]<<",";
        }
        std::cout<<"\n";
    }
    
    
    }
