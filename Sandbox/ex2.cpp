//
//  ex2.cpp
//  Sandbox
//
//  Created by Alagris on 15/10/2017.
//  Copyright © 2017 alagris. All rights reserved.
//

#include "ex2.hpp"
#include <iostream>
#include "meta.h"

namespace ex2 {
    
    
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    ////Finding smallest number o whose
    ////digits multiplied by each other
    ////give number n
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    
    //////////////////////////////////////////////
    //functional
    //////////////////////////////////////////////
    
    FUNCTION<int n,int ten=1,int i=9,int o=0> DO For;
    
    FUNCTION<int n,int ten,int i,int o,int divReminder> DO If{
        LET int new_o=o;
        LET int new_ten=ten;
        LET int new_n=n;
        LET int new_i=i-1;
        DATA For<new_n,new_ten,new_i,new_o> new_for;
        LET int result_n=new_for::result_n;
        LET int result_o=new_for::result_o;
    };
    FUNCTION<int n,int ten,int i,int o> DO If<n,ten,i,o,0>{
        LET int new_o=o+i*ten;
        LET int new_ten=ten*10;
        LET int new_n=n/i;
        LET int new_i=i;
        DATA For<new_n,new_ten,new_i,new_o> new_for;
        LET int result_n=new_for::result_n;
        LET int result_o=new_for::result_o;
    };
    FUNCTION<int n,int ten,int i,int o> DO For{
        DATA If<n,ten,i,o,n%i> m_if;
        LET int result_o=m_if::result_o;
        LET int result_n=m_if::result_n;
    };
    FUNCTION<int n,int ten,int o> DO For<n,ten,1,o>{
        LET int result_o=o;
        LET int result_n=n;
    };
    void func(){
        For<81> t;
        if(t.result_n!=1){
            std::cout <<"None\n";
        }else{
            std::cout<<t.result_o<<"\n";
        }
        
    }
    
    //////////////////////////////////////////////
    //Procedural
    //////////////////////////////////////////////
    void proc(){
        int n=81;
        int o=0;
        int ten=1;
        for(int i=9;i>1 && n!=1;){
            if(n%i==0){
                o+=i*ten;
                ten*=10;
                n/=i;
            }else{
                i--;
            }
        }
        if(n!=1){
            std::cout <<"None\n";
        }else{
            std::cout <<o<<"\n";
        }
    }
}
