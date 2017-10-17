//
//  ex1.cpp
//  Sandbox
//
//  Created by Alagris on 15/10/2017.
//  Copyright © 2017 alagris. All rights reserved.
//
#include <cmath>
#include <iostream>
#include "ex1.hpp"
#include "meta.h"


namespace ex1 {
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    ////converting number x to numerical system of base n
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    
    //////////////////////////////////////////////
    //functional
    //////////////////////////////////////////////
    FUNCTION<int x,int n,char... o> DO Do;
    FUNCTION<int x,int n,char... o> DO If;
    
    FUNCTION<int x,int n,char... o> DO If{
        RETURN Do<x,n,o...> GET;
    };
    FUNCTION<int n,char... o> DO If<0,n,o...>{
        LET char value[] = { o...,'\0' };
    };
    FUNCTION<int n,char... o> CNST char If<0,n,o...> GET[];
    
    FUNCTION<int x,int n,char... o> DO Do{
        LET char new_char=(x%n>9?('A'-10):'0')+x%n;
        LET int new_x=x/n;
        RETURN If<new_x,n,new_char,o...> GET;
    };
    
    void func(){
        std::cout<<Do<1443,10> GET<<"\n";
    }
    
    //////////////////////////////////////////////
    //Procedural
    //////////////////////////////////////////////
    void proc(){
        int x=543548;
        int n=10;
        char o[2048];
        
        int i=log(double(x))/log(double(n))+1;
        o[i--]='\0';
        do{
            o[i--]=(x%n>9?('A'-10):'0')+x%n;
            x/=n;
            
        }while(x>0);
        
        std::cout <<o<<"\n";
    }
}
