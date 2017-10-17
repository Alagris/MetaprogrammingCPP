//
//  metatuples.h
//  Sandbox
//
//  Created by Alagris on 14/10/2017.
//  Copyright © 2017 alagris. All rights reserved.
//

#ifndef metatuples_h
#define metatuples_h
#include "metasyntax.h"
#include "metabase.h"
namespace meta{namespace tuples{
    
    FUNCTION <typename H, typename T=NIL> DO Tuple {
        DATA H Head;
        DATA T Tail;
    };
    
    ////////////////////
    
    FUNCTION <typename T, CNST T ARR arr,int len> DO BuildTupleFromArray{
        ASSERT(len>0,"Length cannot be negative!");
        DATA Tuple<Object<T, &arr>,typename BuildTupleFromArray<T,arr+1,len-1> GET> OUT;
    };
    SPECIAL <typename T, CNST T ARR arr> DO BuildTupleFromArray<T,arr,1>{
        DATA Tuple<Object<T, &arr>> OUT;
    };
    SPECIAL <typename T, CNST T ARR arr> DO BuildTupleFromArray<T,arr,0>{
        DATA Tuple<NIL> OUT;
    };
    
    FUNCTION <typename T, T... arr> DO BuildTuple{
        LET T a[]={arr...};
        LET int len=sizeof(a)/sizeof(T);
        DATA BuildTupleFromArray<T, a,len> OUT;
    };
    
    ////////////////////
    
    FUNCTION <typename tuple, int index> DO GetAt{
        FORWARD GetAt<typename tuple::Tail,index-1> TO OUT;
    };
    SPECIAL <typename tuple> DO GetAt<tuple,0>{
        FORWARD tuple::Head OUT;
    };
    SPECIAL <int index> DO GetAt<NIL,index>{
        DATA NIL OUT;
    };
    
    ////////////////////
    FUNCTION <typename tuple,typename index1Head, int index2> DO SwapRestImpl{
        DATA Tuple<typename tuple::Head,SwapRestImpl<typename tuple::Tail,index1Head,index2-1>> OUT;
    };
    SPECIAL <typename tuple,typename index1Head> DO SwapRestImpl<tuple,index1Head,0>{
        DATA Tuple<index1Head,typename tuple::Tail> OUT;
    };
    FUNCTION <typename tuple, int index1, int index2> DO SwapImpl{
        DATA Tuple<typename tuple::Head,typename SwapImpl<typename tuple::Tail,index1-1,index2-1> GET > OUT;
    };
    SPECIAL <typename tuple, int index2> DO SwapImpl<tuple,0,index2>{
        DATA Tuple<
        typename GetAt<tuple, index2> GET::Head,
        typename SwapRestImpl<typename tuple::Tail,typename tuple::Head,index2-1> GET
        > OUT;
    };
    FUNCTION <typename tuple, int index1, int index2> DO Swap{
        LET int firstIndex= index1>index2?index2:index1;
        LET int secondIndex=index1>index2?index1:index2;
        FORWARD SwapImpl<tuple,firstIndex,secondIndex> TO OUT;
    };
}}

#endif /* metatuples_h */
