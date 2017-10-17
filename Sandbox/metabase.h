//
//  metabase.h
//  Sandbox
//
//  Created by Alagris on 14/10/2017.
//  Copyright © 2017 alagris. All rights reserved.
//

#ifndef metabase_h
#define metabase_h
#include "metasyntax.h"
namespace meta{
    struct NIL{
        DATA NIL value;
        DATA NIL Head;
        DATA NIL Tail;
    };
    FUNCTION <typename T, CNST T val> DO Object {
        LET T value=val;
    };
    FUNCTION <typename T, CNST T... vals> DO Array {
        LET T value[]={vals...};
        LET int len=sizeof...(vals);
    };
    FUNCTION <typename T, T... vals> CNST T Array<T,vals...>::value[];
    
    FUNCTION<IN T,IN U>CNST T convert(const U u){
        return u;
    }
}

#endif /* metabase_h */
