//
//  ex4.cpp
//  Sandbox
//
//  Created by Alagris on 15/10/2017.
//  Copyright © 2017 alagris. All rights reserved.
//

#include "ex4.hpp"
#include <iostream>
#include "meta.h"

namespace ex4 {
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    ////reverse array x
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    
    //////////////////////////////////////////////
    //functional
    //////////////////////////////////////////////
    FUNCTION<typename x,int n,int i,bool iLessThanHalfN=i<n/2 > DO ReverseArray;
    
    FUNCTION<typename x,int n,int i> DO ReverseArray<x,n,i,true>{
        FORWARD meta::arrays::SwapArray<int, n, i, n-i-1, typename ReverseArray<x,n,i+1> GET> TO OUT;
    };
    
    FUNCTION<typename x,int n,int i> DO ReverseArray<x,n,i,false>{
        DATA x OUT;
    };
    
    FUNCTION<CNST int...x> DO ReverseVariadic{
        DATA meta::Array<int, x...> array;
        LET int len=array::len;
        FORWARD ReverseArray<array,len,0> TO OUT;
    };
    
    void func(){
        int len=ReverseVariadic<0,1,2,3,4,5,6,7,8,9,10,11,12>::len;
        const int *const a=ReverseVariadic<0,1,2,3,4,5,6,7,8,9,10,11,12> GET GET;
        
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
        for(int i=0;i<n/2;i++){
            int j=n-i-1;
            const int tmp=x[i];
            x[i]=x[j];
            x[j]=tmp;
            std::cout<<x[i]<<",";
        }
        std::cout<<"\n";
    }
}
