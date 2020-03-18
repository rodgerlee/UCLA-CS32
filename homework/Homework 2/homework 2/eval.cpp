//
//  eval.cpp
//  homework 2
//
//  Created by Rodger Lee on 2/1/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

int infixtopostfix(string infix, const Map& values, string& postfix, int& returntype);


int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    //returntype takes in special cases such as return 1 and return 2
    int returntype = 0;
    infixtopostfix(infix, values, postfix, returntype);
    
    //bypass evaluation, and return without changing result: at this point, the infix has properly converted  expression to postfix. so this satisfies returntype 1 and 2.
    if (returntype > 0)
        return returntype;
    
    
    int temp = 0;
    //postfix expression is created: time to evaluate postfix expression.
    stack<int> evalstack;
    int v1;
    int v2;
    for (int i = 0; i < postfix.length(); i++)
    {
        if (isalpha(postfix[i]))
        {
            ValueType val;
            values.get(postfix[i], val);
            evalstack.push(val);
        }
            
        else if (postfix[i] == '*' || postfix[i] == '/' || postfix[i] == '+' || postfix[i] == '-')
        {
            //pop two values if the next postfix char is an operator.
            
            v2 = evalstack.top();
            evalstack.pop();
            
            //takes care of functions that have invalid postfix expressions. (ie. a+), each operator must have two operands
            if (evalstack.empty())
                return 1;
            
            v1 = evalstack.top();
            evalstack.pop();
            
            if (postfix[i] == '*')
                temp = v1*v2;
            if (postfix[i] == '/')
            {
                //returns 3 for special case of 0 in denominator
                if (v2 == 0)
                    return 3;
                temp = v1/v2;
            }
                
            if (postfix[i] == '+')
                temp = v1+v2;
            if (postfix[i] == '-')
                temp = v1-v2;
            
            evalstack.push(temp);
        }
    }
    if (evalstack.size() == 1)
        result = evalstack.top();
    else
        result = temp;
    return 0;
    
}


int infixtopostfix(string infix, const Map& values, string& postfix, int& returntype)
{
    stack<char> operatorstack;
    stack<char> postfixstack;
    string temp = "";
    //convert infix to postfix
    
    for (int i = 0; i < infix.length(); i++)
    {
        //check if character is a lower case alphabet
        
        if (islower(infix[i]))
        {
            //if there contains two alpha characters in a row, the expression is false
            if (!postfixstack.empty() && islower(postfixstack.top()) && operatorstack.empty())
                return 1;
            postfixstack.push(infix[i]);
            temp += infix[i];
            
            //change return type to 2 if the letter does not match with one in the map, however, still find postfix expression
            if (!values.contains(infix[i]))
                returntype = 2;
            
        }
        //check for opening parentheses, operatorstack is empty means there is no operator between the previous term and this term, which makes the expression invalid (ie. 3(5+6) )
        else if (infix[i] == '(')
        {
            if (operatorstack.empty())
                return 1;
            operatorstack.push(infix[i]);
        }
        
        //check for operator, operator must not be the first term of the expression, nor the last
        else if (((infix[i] == '*' || infix[i] == '/' || infix[i] == '+' || infix[i] == '-')) && (!postfixstack.empty()))
        {
            if (operatorstack.empty())
                operatorstack.push(infix[i]);
            else
            {
                //adds multiply and divide to the stack after popping and appending all operators that have equal or greater precedence, such as other * and / signs
                if (infix[i] == '*' || infix[i] == '/')
                {
                    while (operatorstack.top() != '(' && !operatorstack.empty())
                    {
                        if (operatorstack.top() == '*' || operatorstack.top() == '/')
                        {
                            postfixstack.push(operatorstack.top());
                            temp += operatorstack.top();
                            operatorstack.pop();
                        }
                        //does not pop and pushes * or / to the stack. this happens in the case of an + or - sign at the top of the stack
                        else
                        {
                            operatorstack.push(infix[i]);
                            break;
                        }
                    }
                }
                
                // in the case that the operator is addition or subtraction , all operators are popped until parantheses is reached
                else
                {
                    while ((operatorstack.top() != '(') && (!operatorstack.empty()))
                    {
                        postfixstack.push(operatorstack.top());
                        temp += operatorstack.top();
                        operatorstack.pop();
                    }
                    operatorstack.push(infix[i]);
                }
            }
        }
        
        //skip blanks
        else if (isblank(infix[i]))
            continue;
        
        //wwe must check all of the previous operators and pop until we reach an opening bracket. the brackets must contain operators in between, otherwise the expression is false.
        else if (infix[i] == ')')
        {
            if (operatorstack.top() == '(')
                return 1;
            while ((operatorstack.top() != '(') && (!operatorstack.empty()))
            {
                postfixstack.push(operatorstack.top());
                temp += operatorstack.top();
                operatorstack.pop();
            }
            if (operatorstack.top() == '(')
                operatorstack.pop();
        }
        
        //if character is none of the above, the expression is false and we return 1
        else
            return 1;
    }
    
    //if we loop through the entire infix string and it is empty then the expression does not change result and returns 1
    if (postfixstack.empty())
    {
        returntype = 1;
        return returntype;
    }
    
    //all infix tokens are gone, pop all operators and append onto postfix
    while (!operatorstack.empty())
    {
        if (operatorstack.top() == '(')
            return 1;
        postfixstack.push(operatorstack.top());
        temp += operatorstack.top();
        operatorstack.pop();
    }
    
    postfix = temp;
    return 0;
    
}
  // Evaluates an integer arithmetic expression
  //   If infix is a syntactically valid infix integer expression whose
  //   only operands are single lower case letters (whether or not they
  //   appear in the values map), then postfix is set to the postfix
  //   form of the expression; otherwise postfix may or may not be
  //   changed, result is unchanged, and the function returns 1.  If
  //   infix is syntactically valid but contains at least one lower
  //   case letter operand that does not appear in the values map, then
  //   result is unchanged and the function returns 2.  If infix is
  //   syntactically valid and all its lower case operand letters
  //   appear in the values map, then if evaluating the expression
  //   (using for each letter in the expression the value in the map
  //   that corresponds to it) attempts to divide by zero, then result
  //   is unchanged and the function returns 3; otherwise, result is
  //   set to the value of the expression and the function returns 0.

