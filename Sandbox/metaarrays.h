//
//  metaarrays.h
//  Sandbox
//
//  Created by Alagris on 14/10/2017.
//  Copyright © 2017 alagris. All rights reserved.
//

#ifndef metaarrays_h
#define metaarrays_h
#include "metasyntax.h"
#include "metabase.h"

namespace meta{namespace arrays{
    FUNCTION <typename T,int len,CNST int i,typename source,CNST T... result> DO SwapEndImpl{
        FORWARD SwapEndImpl<T,len-1,i+1,source,result...,source GET[i]> TO OUT;
    };
    
    SPECIAL<typename T,int i,typename source,CNST T... result> DO SwapEndImpl<T,0,i,source,result...>{
        DATA Array<T, result...> OUT;
    };
    
    FUNCTION <typename T,int len,CNST int i,T index1Val,int index2,typename source,CNST T... result> DO SwapRestImpl{
        FORWARD SwapRestImpl<T,len-1,i+1,index1Val,index2-1,source,result...,source GET[i]> TO OUT;
    };
    
    SPECIAL<typename T,int len,CNST int i,T index1Val,typename source,CNST T... result>
    DO SwapRestImpl<T,len,i,index1Val,0,source,result...>{
        FORWARD SwapEndImpl<T,len-1,i+1,source,result...,index1Val> TO OUT;
    };
    
    FUNCTION <typename T,int len,CNST int i,int index1,int index2,typename source,CNST T... result> DO SwapImpl{
        FORWARD SwapImpl<T,len-1,i+1,index1-1,index2-1,source,result...,source GET[i]> TO OUT;
    };
    
    SPECIAL <typename T,int len,CNST int i,int index2,typename source,CNST T... result>
    DO SwapImpl<T,len,i,0,index2,source,result...>{
        FORWARD SwapRestImpl<T,len-1,i+1,source GET[i],index2-1,source,result...,source GET[i+index2]> TO OUT;
    };
    FUNCTION <typename T,int len,int index1,int index2,typename arr,bool equal=index1==index2> DO SwapArrayImpl;
    SPECIAL <typename T,int len,int index1,int index2,typename arr> DO SwapArrayImpl<T,len,index1,index2,arr,true>{
        DATA arr OUT;
    };
    SPECIAL <typename T,int len,int index1,int index2,typename arr> DO SwapArrayImpl<T,len,index1,index2,arr,false>{
        FORWARD SwapImpl<T,len,0,index1,index2,arr> TO OUT;
    };
    FUNCTION <typename T,int len,int index1,int index2,typename arr> DO SwapArray{
        LET int firstIndex= index1>index2?index2:index1;
        LET int secondIndex=index1>index2?index1:index2;
        ASSERT(secondIndex<len,"Index out of bounds!");
        ASSERT(firstIndex>-1,"Index out of bounds!");
        FORWARD SwapArrayImpl<T,len,firstIndex,secondIndex,arr> TO OUT;
    };
    FUNCTION <typename T,int index1,int index2,CNST T... arr> DO SwapVariadic{
        DATA Array<T, arr...> array;
        LET int len = array::len;
        FORWARD SwapArray<T,len, index1, index2,array> TO OUT;
    };
}}


#endif /* metaarrays_h */
