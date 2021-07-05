#include "scanner.h"
#include "symtab.h"
// globals
Token_stream ts;
Symbol_table st; 
bool bif=false;
bool belse=false;
int valif=-10000;
int valelse=-10000;

int statement()
{
    
	Token t = ts.get();  // get the next token from token stream

	switch (t.kind) {
	case INT:
		return declaration();
    case IF :
    {
        if(st.sym_tab.find("if")==st.sym_tab.end())
            st.declare("if", -1);

        else
        {
            st.set("if",-1);
             bif=false;
             belse=false;
             valif=-10000;
             valelse=-10000;
        }
    
        ts.putback(t);
        return expression();
    }
    case NOLINE:
         ts.putback(t);
         return expression();
    
    case EXIT :
        exit(0);
                 
	default:
		ts.putback(t);     // put t back into the token stream
		return expression();
	}
}

int declaration()
{
	Token t = ts.get();
	if (t.kind != ID) throw runtime_error("name expected in declaration");
	string name = t.name;
        st.declare(name, 0);
	return 0;
}

// + and -
int expression()
{

    int left = term();      // read and evaluate a Term
    int right;
   
    Token t = ts.get();
       // get the next token from token stream
    
    while (true) {
        switch (t.kind) {
        case '+':
            
            left += term();    // evaluate Term and add

            t = ts.get();
            break;
        case '-':

            left -= term();  // evaluate Term and subtract                     
            t = ts.get();
            break;
        case '>':
        {
            right = term();
            t = ts.get();

            if(left>right)
            left=1;
            else
            left=0;
            break;
        }
        case '<':
        {
            right = term();

            if(left<right)
            left=1;
            else
            left=0;
            
            t = ts.get();
            break;

        }
        case '&':
        {
            right = term();
            if(((left==0 && right==1) || (left==1 && right==0) || (left==0 && right==0)))
            left=0;
            else
            left=1;

            t = ts.get();
            break;
 
        }

        case '|':
        {
           
            right = term();
            if((left==0 && right==1) || (left==1 && right==0) || (left==1 && right==1))
            left=1;
            else
            left=0;

            t = ts.get();
            break;

        }    

        default:
        
        if(bif==true && belse==true && st.get("if").value==1 && valif!=-10000)
         left = valif;

        if(belse==true && bif==true && st.get("if").value==0 && valelse!=-10000)
        {
            left = valelse;
            belse=false;
            bif=false; 
        }
         
        ts.putback(t); // put t back into the token stream
        
        
        return left;   // finally: no more + or -: return the answer
        
        }
    }
}

// * and /
int term()
{
    
    int left = primary();
    Token t = ts.get();

    while (true) {
        switch (t.kind) {
        case '*':
            left *= primary();
            t = ts.get();
            break;
        case '/':
        {
            int d = primary();
            if (d == 0) throw runtime_error("divide by zero");
            left /= d;
            t = ts.get();
            break;
        }
        case '>':
        {
            
            int d = primary();

            if(left>d)
            left=1;
            else 
            left=0;

            t = ts.get();
            break;
        }
        case '<':
        {

            int d = primary();

            if(left<d)
            left=1;
            else 
            left=0;

            t = ts.get();
            break;
            
        }   
        case '&':
        {
            
            int d = term();
   
            if(((left==0 && d==1) || (left==1 && d==0) || (left==0 && d==0)))
            left=0;
            else
            left=1;
            
            t = ts.get();
            break;

        }

        case '|':
        {
            
            int d = term();
      
            if((left==0 && d==1) || (left==1 && d==0) || (left==1 && d==1))
            left=1;
            else
            left=0;

            t = ts.get();
            break;

        }

        default:
            
            ts.putback(t);
            return left;
        }
    }
}

int primary()
{
    Token t = ts.get();

    int result;
    switch (t.kind) {
    case '(':    // handle '(' expression ')'
    {
        int d = expression();
      
        t = ts.get();
        if (t.kind != ')') throw runtime_error("')' expected");
            return d;
    }
    
    case '!':
    {
        int d=expression();
        if(d==0)
        return 1;
        else 
        return 0;

    }
    
    case IF:
    {
       
        int d=expression();
        bif=true;
        if(d==1)
        {
            int ans=expression();
            valif=ans;
           
            st.set("if",1);
            return ans;
        }
        else 
        {
            int ans=expression();
          
            t = ts.get();
            if(t.kind!=ELSE) throw runtime_error("need else after if");
            
            ts.putback(t);
            st.set("if",0);
            return ans;
        }
        
    }

    case ELSE:
    {
       
        int d=expression();
        belse=true;
        valelse=d;
        return d;
    }

    case NOLINE:
    {
        int p=expression();
        return p;
    }


    case '-':
    return - primary();
    case '+':
	return primary();
    case NUM:
    return t.value; // return the number value
    
    case ID:
    {
	string n = t.name;
  
	Token next = ts.get();
    if (next.kind == '=') {	// name = expression

            int d = expression();
        st.set(n, d);
            return d; // return the assignment value
	}
	else
    {
        ts.putback(next);		// not an assignment
        return st.get(t.name).value;  // return the id value
	}

    }
    default:
        throw runtime_error("primary expected");
    }
}
