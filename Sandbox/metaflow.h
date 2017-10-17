//
//  metaflow.h
//  Sandbox
//
//  Created by Alagris on 14/10/2017.
//  Copyright © 2017 alagris. All rights reserved.
//

#ifndef metaflow_h
#define metaflow_h
#include "metasyntax.h"
#include "metabase.h"
#include "metaequal.h"
/**Procedural-like flow control*/
namespace meta{namespace flow{
//    FUNCTION<IN first, IN... expressions> DO IfElse{
//        RETURN (first GET==true)||IfElse<expressions...> GET;
//    };
//    SPECIAL<IN first> DO IfElse<first>{
//        RETURN first GET==true;
//    };
//    
//    FUNCTION<bool first, bool... bools> DO IfElsePrimitive{
//        RETURN first||IfElsePrimitive<bools...> GET;
//    };
//    SPECIAL<bool first> DO IfElsePrimitive<first>{
//        RETURN first;
//    };
    
    FUNCTION<bool condition,IN ifTrue, IN ifFalse> DO IfElse;
    SPECIAL<IN ifTrue, IN ifFalse> DO IfElse<true,ifTrue,ifFalse>{
        DATA ifTrue OUT;
    };
    SPECIAL<IN ifTrue, IN ifFalse> DO IfElse<false,ifTrue,ifFalse>{
        DATA ifFalse OUT;
    };
    
    FUNCTION<bool condition,IN ifTrue> DO If;
    SPECIAL<IN ifTrue> DO If<true,ifTrue>{
        DATA ifTrue OUT;
    };
    SPECIAL<IN ifTrue> DO If<false,ifTrue>{
        DATA NIL OUT;
    };

    FUNCTION<IN condition,IN statements> DO Case{
        DATA condition cnd;
        DATA statements stmt;
    };
    namespace{
        FUNCTION<IN input,IN first> DO TestSwitch{
            RETURN meta::equal::Equal<input, IN first::cnd> GET;
        };
        FUNCTION<IN input,bool equal,IN previous,IN first,IN... cases> DO SwitchImpl;
        
        SPECIAL<IN input,IN previous,IN first,IN... cases> DO SwitchImpl<input,true,previous,first,cases...>{
            FORWARD previous::stmt TO OUT;
        };
        SPECIAL<IN input,IN previous,IN first,IN... cases> DO SwitchImpl<input,false,previous,first,cases...>{
            FORWARD SwitchImpl<input,TestSwitch<input,first>GET,cases...> TO OUT;
        };
    }
    FUNCTION<IN input, IN first,IN... cases> DO Switch{
        FORWARD SwitchImpl<input,TestSwitch<input,first>GET,first,cases...> TO OUT;
    };

    
}}


#endif /* metaflow_h */
