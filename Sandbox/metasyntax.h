//
//  metasyntax.h
//  Sandbox
//
//  Created by Alagris on 14/10/2017.
//  Copyright © 2017 alagris. All rights reserved.
//

#ifndef metasyntax_h
#define metasyntax_h
//This set of macros
//is used to create a brand new
//keyword system designed specially
//to make reading template-metaprogramming
//a lot easier and intuitive
//(as long as you understand these
//macros of course)
#define CNST constexpr const
#define LET CNST static
#define AUTO_LET LET auto&
#define RETURN AUTO_LET value =
#define ARR * const
#define DATA typedef
#define FORWARD DATA typename
#define FUNCTION template
#define DO struct
#define OUT value
#define GET ::value
#define ASSERT static_assert
#define IN typename
/**SPECIAL is semantically equal to FUNCTION but is used 
 for clarity to indicate that specified function is in 
 fact template specialization of another already 
 defined FUNCTION*/
#define SPECIAL FUNCTION
/*TO is semantically equal to GET but 
 conceptually TO should be used only after FORWARD.
 It looks more natural this way.*/
#define TO GET
#endif /* metasyntax_h */
