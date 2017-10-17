//
//  metamath.h
//  Sandbox
//
//  Created by Alagris on 14/10/2017.
//  Copyright © 2017 alagris. All rights reserved.
//

#ifndef metamath_h
#define metamath_h
#include <limits>
#include "metasyntax.h"
#include "metabase.h"
#include "metaequal.h"
/**generic operations on numerals*/
namespace meta{namespace math{
    typedef unsigned int uint;
    FUNCTION<IN T> CNST T max(T a, T b){
        return a>b?a:b;
    }
    FUNCTION<IN T> CNST T min(T a, T b){
        return a<b?a:b;
    }
    FUNCTION<IN T> CNST T abs(T a){
        return a>0?a:-a;
    }
    FUNCTION<IN T> CNST T roundMin(T n,T min){
        return n<min?min:n;
    }
    FUNCTION<IN T> CNST T roundMax(T n,T max){
        return n>max?max:n;
    }
    namespace{
        FUNCTION<IN T> CNST T powImpl(T base, int power){
            return power==0?1:(powImpl<T>(base,power-1)*base);
        }
    }
    FUNCTION<IN T> CNST T pow(T base, int power){
        return power>0?powImpl<T>(base,power):(1/powImpl<T>(base,-power));
    }
}}
/**Operations on floating points*/
namespace meta{namespace math{
    /**returns -1 if no 'one' was found. 1 right after binary point has index 0*/
    CNST int findRightmostOneAfterPoint(const float f){
        return int(f)==f?-1:(findRightmostOneAfterPoint(f*2)+1);
    }
    CNST int findLeftmostOneBeforePoint(const float f){
        return int(f)==0?-1:(findLeftmostOneBeforePoint(f/2)+1);
    }
    CNST int32_t extractExponent(const float f){
        return -(findRightmostOneAfterPoint(f)+1);
    }
    CNST int64_t extractMantissa(const float f,int32_t exponent){
        return int64_t(f*pow<float>(2,-exponent));
    }
}}

/**Operations on integers*/
namespace meta{namespace math{
    typedef unsigned int uint;
    FUNCTION<IN T,T n > DO Abs{
        RETURN n>0?n:-1;
    };
    FUNCTION<CNST int64_t n,CNST int64_t min > DO RoundMin{
        RETURN n<min?min:n;
    };
    FUNCTION<int64_t integer,int base=2> DO IntegerLength{
        RETURN 1+IntegerLength<integer/base> GET;
    };
    SPECIAL<int base> DO IntegerLength<0,base>{
        RETURN 0;
    };
    FUNCTION<uint64_t integer> DO UIntegerByteLength{
        RETURN 1+UIntegerByteLength<(integer>>8)> GET;
    };
    SPECIAL<> DO UIntegerByteLength<0>{
        RETURN 0;
    };
    /**first bit has index 0. Returns -1 if no 'one' was found */
    FUNCTION<uint64_t integer> DO FindRightmostBinaryOne{
        LET int OUT =integer==0?-1:((integer%2==1) ? 0 : (1+FindRightmostBinaryOne<(integer>>1)> GET));
    };
    /**first bit has index 0. Returns -1 if no 'one' was found */
    FUNCTION<uint64_t integer> DO FindLeftmostBinaryOne{
        LET int OUT = integer==0? -1 : (FindLeftmostBinaryOne<(integer>>1)> GET+1);
    };
    FUNCTION<int64_t integer> DO IntegerByteLength{
        RETURN UIntegerByteLength<uint64_t(integer>0?integer:-integer)> GET;
    };
}}
        
/**Real number declaration and basics*/
namespace meta{namespace math{
    FUNCTION<int64_t m=0,int32_t e=0> DO Real{
        LET int64_t mantissa=m;
        LET int32_t exponent=e;
        LET int32_t maxMantissaLen=sizeof(int64_t)*8-1;
        LET int32_t maxExponentLen=sizeof(int32_t)*8-1;
    };
    
    DATA Real<std::numeric_limits<int64_t>::max(),std::numeric_limits<int32_t>::max()> Infinity;
    DATA Real<std::numeric_limits<int64_t>::min(),std::numeric_limits<int32_t>::max()> NegativeInfinity;
    DATA Real<std::numeric_limits<int64_t>::max()-1,std::numeric_limits<int32_t>::max()> AlmostInfinity;
    DATA Real<std::numeric_limits<int64_t>::min()+1,std::numeric_limits<int32_t>::max()> AlmostNegativeInfinity;
    DATA NIL NaN;
    DATA Real<0,0> Zero;
    
    FUNCTION<IN real> DO EvaluateReal{
//        LET uint64_t powerOf2 =real::exponent>=0?(1<<real::exponent):(1<<(-real::exponent));
//        RETURN real::exponent>=0?float(real::mantissa)*float(powerOf2):float(real::mantissa)/float(powerOf2);
        RETURN float(real::mantissa)*pow<float>(2,real::exponent);
    };
    
}}

/**Real number shifting operations*/
namespace meta{namespace math{
    FUNCTION<int64_t mantissa,int32_t exponent,bool negMantissa=mantissa<0> DO RoundToInfinty;
    SPECIAL<int64_t mantissa,int32_t exponent> DO RoundToInfinty<mantissa,exponent,false>{
        DATA Infinity OUT;
    };
    SPECIAL<int64_t mantissa,int32_t exponent> DO RoundToInfinty<mantissa,exponent,true>{
        DATA NegativeInfinity OUT;
    };
    
    FUNCTION<IN real,CNST int32_t newExponent> DO SetPrecisionWindowOfReal{
        DATA Real<int64_t(real::mantissa*pow<float>(2, real::exponent-newExponent)) , newExponent> OUT;
    };
    FUNCTION<IN real,CNST int32_t leftmostDigitPower> DO SetPrecisionWindowLeftBoundOfReal{
        LET int32_t newExponent =leftmostDigitPower-Infinity::maxMantissaLen;
        FORWARD SetPrecisionWindowOfReal<real,newExponent> TO OUT;
    };
    FUNCTION<IN real,CNST int32_t shift> DO RightSlidePrecisionWindowOfReal{
        //because we need to preserve sign bit we have to
        //divide/multiply rather than just use right/left shift
        DATA Real<int64_t(real::mantissa*pow<float>(2, shift)), real::exponent-shift> OUT;
    };
    FUNCTION<IN real,CNST int32_t shift> DO LeftSlidePrecisionWindowOfReal{
        //because we need to preserve sign bit we have to
        //divide/multiply rather than just use right/left shift
        DATA Real<int64_t(real::mantissa/pow<float>(2, shift)), real::exponent+shift> OUT;
    };
    FUNCTION<IN real,uint freeSpaceFromRight=0> DO MaximallyLeftSlidePrecisionWindowOfReal{
        LET int trailingZeros=RoundMin<FindRightmostBinaryOne<uint64_t(real::mantissa)> GET,0> GET;
        LET int maxZeroesToRemove=RoundMin<trailingZeros-freeSpaceFromRight,0> GET;
        LET int zeroesToRemove=Infinity::exponent-trailingZeros>=real::exponent?trailingZeros:(Infinity::exponent-real::exponent);
        FORWARD LeftSlidePrecisionWindowOfReal<real,zeroesToRemove> TO OUT;
    };
    FUNCTION<IN real,uint freeSpaceFromLeft=0> DO MaximallyRightSlidePrecisionWindowOfReal{
        LET int length=IntegerLength<real::mantissa> GET;
        ASSERT(Infinity::maxMantissaLen>=length,"Make sure maxMantissaLen is correct!\n");
        LET int remainingZeroes=RoundMin<Infinity::maxMantissaLen-freeSpaceFromLeft-length,0> GET;
        FORWARD RightSlidePrecisionWindowOfReal<real,remainingZeroes> TO OUT;
    };
    FUNCTION<IN real> DO FixRealNumber{
        FORWARD MaximallyLeftSlidePrecisionWindowOfReal<real> TO OUT;
    };
    FUNCTION<IN real,int32_t shift> DO LeftShiftReal{
        DATA Real<real::mantissa,real::exponent+shift> OUT;
    };
    FUNCTION<IN real,int64_t shift> DO RightShiftReal{
        DATA Real<real::mantissa,real::exponent-shift> OUT;
    };
    FUNCTION<IN real,CNST uint64_t shift> DO LeftShiftMantissaReal{
        //because we need to preserve sign bit we have to
        //divide/multiply rather than just use right/left shift
        DATA Real<(real::mantissa*pow<float>(2, shift)),real::exponent> OUT;
    };
    FUNCTION< IN  real,CNST uint64_t shift> DO RightShiftMantissaReal{
        //because we need to preserve sign bit we have to
        //divide/multiply rather than just use right/left shift
        DATA Real<(int64_t(real::mantissa/pow<double>(2, shift))),real::exponent> OUT;
    };
}}
        
/**Real number comparision operations*/
namespace meta{namespace math{
    //        /**Comparision like r1>=r2*/
    //        FUNCTION<IN r1,IN  r2> DO CompareRealsImpl{
    //            FORWARD FixRealNumber<r1> GET fr1;
    //            FORWARD FixRealNumber<r2> GET fr2;
    //            RETURN util::flow::IfElsePrimitive<
    //            (fr1::exponent>fr2::exponent),
    //            (fr1::exponent==fr2::exponent),
    //            (fr1::exponent<fr2::exponent)
    //            > GET;
    //        };
    //        FUNCTION<IN r1,IN  r2,bool bigger1=(r1::exponent>=r2::exponent)> DO CompareReals;
    //        FUNCTION<IN r1,IN  r2> DO CompareReals<r1,r2,false>{
    //            DATA r2 bigger;
    //            DATA r1 smaller;
    //        };
    //        FUNCTION<IN r1,IN  r2> DO CompareReals<r1,r2,true>{
    //            DATA r1 bigger;
    //            DATA r2 smaller;
    //        };
    FUNCTION<IN r1,IN  r2,bool bigger1=(r1::exponent>=r2::exponent)> DO CompareExponents;
    SPECIAL<IN r1,IN  r2> DO CompareExponents<r1,r2,false>{
        DATA r2 bigger;
        DATA r1 smaller;
        LET bool r1IsBigger=false;
    };
    SPECIAL<IN r1,IN  r2> DO CompareExponents<r1,r2,true>{
        DATA r1 bigger;
        DATA r2 smaller;
        LET bool r1IsBigger=true;
    };
    namespace {
        FUNCTION<IN r1,IN  r2> DO MakeExponentsEqual{
            FORWARD FixRealNumber<r1> TO fixed1;
            FORWARD FixRealNumber<r2> TO fixed2;
            DATA CompareExponents<fixed1, fixed2> compared;
            LET bool r1IsBigger = compared::r1IsBigger;
            LET int biggerLen =IntegerLength<compared::bigger::mantissa> GET;
            LET int smallerLen =IntegerLength<compared::smaller::mantissa> GET;
            LET int biggerLeftmostBit=biggerLen+compared::bigger::exponent;
            LET int smallerLeftmostBit=smallerLen+compared::smaller::exponent;
            LET int leftmostBitOfBoth=max<int>(biggerLeftmostBit,smallerLeftmostBit);
            /*sum of two numbers can maximally produce a number bigger by one digit.
             Therefore we add 1 for safety to prevent overflow.*/
            LET int precissionWindowLeftBoundary=leftmostBitOfBoth+1;
            FORWARD SetPrecisionWindowLeftBoundOfReal<IN compared::bigger,precissionWindowLeftBoundary> TO bigger;
            FORWARD SetPrecisionWindowLeftBoundOfReal<IN compared::smaller,precissionWindowLeftBoundary> TO smaller;
        };
    }
    
    FUNCTION<IN real> DO IsInfinity{
        RETURN false;
    };
    SPECIAL<> DO IsInfinity<Infinity>{
        RETURN true;
    };
    FUNCTION<IN real> DO IsNegativeInfinity{
        RETURN false;
    };
    SPECIAL<> DO IsNegativeInfinity<NegativeInfinity>{
        RETURN true;
    };
    FUNCTION<IN real> DO IsAnyInfinity{
        RETURN IsNegativeInfinity<real> GET||IsInfinity<real> GET;
    };
    FUNCTION<IN real> DO IsNaN{
        RETURN false;
    };
    SPECIAL<> DO IsNaN<NaN>{
        RETURN true;
    };
    FUNCTION<IN real> DO IsSpecial{
        RETURN IsAnyInfinity<real> GET||IsNaN<real> GET;
    };
    FUNCTION<IN real> DO IsWholeReal{
        RETURN real::exponent>=0 || real::mantissa==0;
    };
}}
        
/**Real number arithmetic operations*/
namespace meta{namespace math{
    namespace {
        FUNCTION<int64_t newMantissa,int32_t exponent1,int32_t exponent2,int32_t exponent3,bool overflow>
        DO GuardRealBoundsImpl;
        SPECIAL<int64_t newMantissa,int32_t exponent1,int32_t exponent2,int32_t exponent3>
        DO GuardRealBoundsImpl<newMantissa,exponent1,exponent2,exponent3,false>{
            DATA Real<newMantissa,exponent1+exponent2+exponent3> OUT;
        };
        SPECIAL<int64_t newMantissa,int32_t exponent1,int32_t exponent2,int32_t exponent3>
        DO GuardRealBoundsImpl<newMantissa,exponent1,exponent2,exponent3,true>{
            FORWARD RoundToInfinty<newMantissa, 0> TO OUT;
        };
        FUNCTION<int32_t positiveExponent,int32_t exponent2> DO TestRealBoundsImpl{
            RETURN (Infinity::exponent-positiveExponent)<=exponent2?true:false;
        };
        /**Returns true if multiplication results in overflow*/
        FUNCTION<int32_t exponent1,int32_t exponent2> DO TestRealBounds{
            RETURN exponent1<0?
            (exponent2<0?false:TestRealBoundsImpl<exponent1,exponent2> GET):
            TestRealBoundsImpl<exponent1,exponent2> GET;
        };
        FUNCTION<int64_t newMantissa,int32_t exponent1,int32_t exponent2,int32_t exponent3> DO GuardRealBounds{
            LET bool exponentOverflow12=TestRealBounds<exponent1,exponent2> GET;
            LET bool exponentOverflow123=exponentOverflow12||TestRealBounds<exponent3,exponent1+exponent2> GET;
            FORWARD GuardRealBoundsImpl<newMantissa,exponent1,exponent2,exponent3,exponentOverflow123> TO multiplied;
            FORWARD FixRealNumber<multiplied> TO OUT;
        };
    }
    
    FUNCTION<IN real> DO AbsReal{
        DATA Real<(real::mantissa>0?real::mantissa:-real::mantissa), real::exponent> OUT;
    };
    
    FUNCTION<IN real> DO FlipRealNumber{
        // (m*b^x)^(-1)=m^(-1)*b^(-x)
        //How to express m^(-1) solely in whole numbers:
        //  m^(-1)*b^(-x)=
        //  1/m*b^(-x)=
        //  b^l/b^l*b^(-x)/m=
        //  b^l/m*b^(-x)/b^l=
        //  b^l/m*b^(-x-l)
        //
        //where l is some constant that specifies prescission.
        //The bigger the better. In our case b=2 (binary system)
        //so maximal l=sizeof(mantissa type)*8-1 (-2 for signed type).
        LET int32_t l=sizeof(int64_t)*8-2;
        LET int64_t bToPowerOfL=pow<float>(2, l);
        FORWARD FixRealNumber<Real<bToPowerOfL/real::mantissa,-l-real::exponent>> TO OUT;
        
    };
    FUNCTION<IN real> DO NegateRealNumber{
        DATA Real<-real::mantissa,real::exponent> OUT;
    };
    
    FUNCTION<IN r1,IN  r2> DO MultiplyReals{
        LET int r1len=FindLeftmostBinaryOne<r1::mantissa> GET+1;
        LET int r2len=FindLeftmostBinaryOne<r2::mantissa> GET+1;
        LET int expectedNewLen=r1len+r2len;
        LET int overflowBits=expectedNewLen>Zero::maxMantissaLen?expectedNewLen-Zero::maxMantissaLen:0;
        LET int o1=overflowBits/2;
        LET int o2=(overflowBits+1)/2;
        FORWARD RightShiftMantissaReal<r1,o1> TO new_r1;
        FORWARD RightShiftMantissaReal<r2,o2> TO new_r2;
        FORWARD GuardRealBounds<new_r1::mantissa*new_r2::mantissa, new_r1::exponent, new_r2::exponent, overflowBits> TO OUT;
    };
    SPECIAL<IN r1> DO MultiplyReals<r1,NaN>{
        DATA NaN OUT;
    };
    SPECIAL<IN  r2> DO MultiplyReals<NaN,r2>{
        DATA NaN OUT;
    };
    SPECIAL<IN r1> DO MultiplyReals<r1,Infinity>{
        DATA Infinity OUT;
    };
    SPECIAL<IN  r2> DO MultiplyReals<Infinity,r2>{
        DATA Infinity OUT;
    };
    SPECIAL<> DO MultiplyReals<Zero,Infinity>{
        DATA NaN OUT;
    };
    SPECIAL<> DO MultiplyReals<Infinity,Zero>{
        DATA NaN OUT;
    };
    SPECIAL<IN r1> DO MultiplyReals<r1,NegativeInfinity>{
        DATA NegativeInfinity OUT;
    };
    SPECIAL<IN  r2> DO MultiplyReals<NegativeInfinity,r2>{
        DATA NegativeInfinity OUT;
    };
    SPECIAL<> DO MultiplyReals<Zero,NegativeInfinity>{
        DATA NaN OUT;
    };
    SPECIAL<> DO MultiplyReals<NegativeInfinity,Zero>{
        DATA NaN OUT;
    };
    
    FUNCTION<IN r1,IN  r2> DO DivideReals{
        FORWARD FlipRealNumber<r2> TO flippedR2;
        FORWARD MultiplyReals<r1, flippedR2> TO OUT;
    };
    SPECIAL<IN r1> DO DivideReals<r1,NaN>{
        DATA NaN OUT;
    };
    SPECIAL<IN  r2> DO DivideReals<NaN,r2>{
        DATA NaN OUT;
    };
    SPECIAL<IN r1> DO DivideReals<r1,Infinity>{
        DATA Zero OUT;
    };
    SPECIAL<IN  r2> DO DivideReals<Infinity,r2>{
        DATA Infinity OUT;
    };
    SPECIAL<IN r1> DO DivideReals<r1,NegativeInfinity>{
        DATA Zero OUT;
    };
    SPECIAL<IN  r2> DO DivideReals<NegativeInfinity,r2>{
        DATA NegativeInfinity OUT;
    };
    
    FUNCTION<IN r1,IN  r2> DO AddReals{
        DATA MakeExponentsEqual<r1, r2> equal;
        DATA Real<equal::smaller::mantissa+equal::bigger::mantissa ,equal::smaller::exponent> summed;
        FORWARD FixRealNumber<summed> TO OUT;
    };
    SPECIAL<IN r1> DO AddReals<r1,NaN>{
        DATA NaN OUT;
    };
    SPECIAL<IN  r2> DO AddReals<NaN,r2>{
        DATA NaN OUT;
    };
    SPECIAL<IN r1> DO AddReals<r1,Infinity>{
        DATA Infinity OUT;
    };
    SPECIAL<IN  r2> DO AddReals<Infinity,r2>{
        DATA Infinity OUT;
    };
    SPECIAL<IN r1> DO AddReals<r1,NegativeInfinity>{
        DATA NegativeInfinity OUT;
    };
    SPECIAL<IN  r2> DO AddReals<NegativeInfinity,r2>{
        DATA NegativeInfinity OUT;
    };
        
    FUNCTION<IN r1,IN  r2> DO SubtractReals{
        FORWARD AddReals<r1,IN NegateRealNumber<r2> GET> TO OUT;
    };
    SPECIAL<IN r1> DO SubtractReals<r1,NaN>{
        DATA NaN OUT;
    };
    SPECIAL<IN  r2> DO SubtractReals<NaN,r2>{
        DATA NaN OUT;
    };
    SPECIAL<IN r1> DO SubtractReals<r1,Infinity>{
        DATA Infinity OUT;
    };
    SPECIAL<IN  r2> DO SubtractReals<Infinity,r2>{
        DATA Infinity OUT;
    };
    SPECIAL<IN r1> DO SubtractReals<r1,NegativeInfinity>{
        DATA NegativeInfinity OUT;
    };
    SPECIAL<IN  r2> DO SubtractReals<NegativeInfinity,r2>{
        DATA NegativeInfinity OUT;
    };
}}
        
namespace meta{ namespace equal{
    FUNCTION<int64_t m1,int32_t e1,int64_t m2, int32_t e2> DO Equal<meta::math::Real<m1,e1>,meta::math::Real<m2,e2>>{
        FORWARD meta::math::FixRealNumber<IN meta::math::Real<m1,e1>> TO f1;
        FORWARD meta::math::FixRealNumber<IN meta::math::Real<m2,e2>> TO f2;
        RETURN f1::mantissa==f2::mantissa&&f1::exponent==f1::exponent;
    };
}}
        
        

#endif /* metamath_h */
