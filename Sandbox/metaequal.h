//
//  metaequal.h
//  Sandbox
//
//  Created by Alagris on 15/10/2017.
//  Copyright © 2017 alagris. All rights reserved.
//

#ifndef metaequal_h
#define metaequal_h
#include "metabase.h"
namespace meta{namespace equal{
    
    FUNCTION<IN t1,IN t2> DO Equal{
        RETURN false;
    };
    
    SPECIAL<typename T,T t1,T t2> DO Equal<Object<T, t1>,Object<T, t2>>{
        RETURN t1==t2;
    };
    SPECIAL<typename T> DO Equal<T,T>{
        RETURN true;
    };
    SPECIAL<typename T,T*t1,T*t2> DO Equal<Array<T, t1>,Array<T, t2>>{
        RETURN t1==t2;
    };
}}

#endif /* metaequal_h */
