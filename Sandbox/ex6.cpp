//
//  ex6.cpp
//  Sandbox
//
//  Created by Alagris on 15/10/2017.
//  Copyright © 2017 alagris. All rights reserved.
//

#include "ex6.hpp"
#include <iostream>
#include <bitset>
#include "meta.h"

namespace  ex6{
    
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    ////matrix multiplication (o=m1*m2)
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    typedef float Mat4[16];
    //(0,0) x-------->      (0,3)
    //  y   0   1   2   3
    //  |   4   5   6   7
    //  |   8   9   10  11
    //  V   12  13  14  15
    //(3,0)                 (3,3)
    CNST int index(const int x,const int y){
        return  x+4*y;
    }
    CNST float get(const Mat4 m,const int x,const int y){
        return m[index(x,y)];
    }
    //////////////////////////////////////////////
    //functional
    //////////////////////////////////////////////
    FUNCTION<CNST Mat4 m1,CNST Mat4 m2,int x,int y,IN sum=meta::math::Zero,int i=0> DO Sum{
        LET float fiy=get(m1,i,y);
        LET float fxi=get(m2,x,i);
        LET int32_t eiy=meta::math::extractExponent(fiy);
        LET int32_t exi=meta::math::extractExponent(fxi);
        DATA meta::math::Real<meta::math::extractMantissa(fiy, eiy),eiy> m1iy;
        DATA meta::math::Real<meta::math::extractMantissa(fxi, exi),exi> m2xi;
        FORWARD meta::math::MultiplyReals<m1iy, m2xi> TO m1m2;
        FORWARD meta::math::AddReals<m1m2 , sum> TO new_sum;
        FORWARD Sum<m1,m2,x,y,new_sum,i+1> TO OUT;
    };
    FUNCTION<CNST Mat4 m1,CNST Mat4 m2,int x,int y,IN sum> DO Sum<m1,m2,x,y,sum,4>{
        DATA sum OUT;
    };
    FUNCTION<CNST Mat4 m1,CNST Mat4 m2,int x=0,int y=0> DO Multiply{
        FORWARD Sum<m1,m2,x,y> TO sum;
        DATA meta::tuples::Tuple<sum,IN Multiply<m1,m2,x+1,y> GET> OUT;
    };
    FUNCTION<CNST Mat4 m1,CNST Mat4 m2,int y> DO Multiply<m1,m2,4,y>{
        FORWARD Multiply<m1, m2,0,y+1> TO OUT;
    };
    FUNCTION<CNST Mat4 m1,CNST Mat4 m2,int x> DO Multiply<m1,m2,x,4>{
        DATA meta::NIL OUT;
    };
    
    FUNCTION<IN tuple> void print(int y=0,int x=0){
        if(y>=4)return;
        std::cout<<meta::math::EvaluateReal<IN tuple::Head > GET<<"\t";
        if(x+1<4){
            print<IN tuple::Tail>(y,x+1);
        }else{
            std::cout<<"\n";
            print<IN tuple::Tail>(y+1,0);
        }
    }
    FUNCTION<> void  print<meta::NIL>(int y,int x){
        return;
    }
    CNST Mat4 m1={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    CNST Mat4 m2={10,11,12,13,14,15,16,17,18,19,110,111,112,113,114,115};
    void func(){
        DATA Multiply<m1, m2> GET o;
        print<o>();
    }
    //////////////////////////////////////////////
    //Procedural
    //////////////////////////////////////////////
    
    
    void proc(){
        Mat4 o;
        //o=m1*m2
        for(int y=0;y<4;y++){
            for(int x=0;x<4;x++){
                float sum=0;
                for(int i=0;i<4;i++){
                    sum+=get(m1,i,y)*get(m2,x,i);
                }
                o[index(x,y)]=sum;
                std::cout<<sum<<"\t";
            }
            std::cout<<"\n";
        }
    }
}
