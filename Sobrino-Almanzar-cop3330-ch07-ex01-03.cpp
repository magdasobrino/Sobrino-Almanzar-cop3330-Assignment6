/*
 *  UCF COP3330 Fall 2021 Assignment 6 Solution
 *  Copyright 2021 Magdalena Sobrino-Almanzar
 */

//Exercises 7.1, 7.2, 7.3//

//exercise 1//
//Allow underscores in the calculator’s variable names.//
//The system will check all the constant if it isn't and the character doesn't match any of the other cases then it will simply return "Bad token".

//exercise 2//
/*/. Provide an assignment operator, =, so that you can change the value of a variable after you introduce it using let. 
Discuss why that can be useful and how it can be a source of problems./*/ 

//User will have the chance to re entering the value if made a mistake. 
//Only one variable to be assigned upon initialisation in need of more, a new variable with a new value has to be created//

//exercise 3//
/*/Provide named constants that you really can’t change the value of. 
Hint: You have to add a member to Variable that distinguishes between constants and variables and check for it in set_value(). 
If you want to let the user define constants (rather than just having pi and e defined as constants), you’ll have to add a notation to let the user express that, for example, const pi = 3.14;./*/


#include "std_lib_facilities.h"


//Add constants//

const char number = '9';    
const char quit   = 'q';    
const char print  = ';';    
const char name   = 'a';    // name token
const char let    = 'L';    // declaration token
const char con    = 'C';    // declaration token
const string declkey = "let";		
const string constkey = "const";	
const string prompt  = "> ";
const string result  = "= ";
const char square_root = '@';
const char exponent = '^';
const string sqrtkey = "sqrt";
const string expkey = "pow";


class Token {
public:
    char kind;
    double value;
    string name;

    Token(char ch) : kind{k}, value(0) {}
    Token(char ch, double val) : kind(ch), value(val) {}
    Token(char ch, string n) : kind(ch), name(n) {}
};

class Token_option {
public:
    Token get();                // get a Token
    void putback(Token t);      // put a token back
    void ignore(char c);        // discard characters up to and including a c
private:
    bool full { false };        // is there a Token in the buffer?
    Token buffer {'0'};         // here is where putback() stores a Token
};

Token_stream::Token_stream()
:full(false), buffer(0)    // no Token in buffer
{
}

void Token_option::ignore(char c)
    // c represents the kind of Token
{
    // first look in buffer
    if (full && c == buffer.kind) {
        full = false;
        return;
    }
    full = false;

    // now search for input
    char ch = 0;
    while (cin >> ch)
        if (ch == c) return;
}

void Token_option::putback(Token t)
{
    buffer = t;                 // copy t to buffer
    full = true;                // buffer is now full
};

Token Token_option::get()
{
    if (full) {             // do we already have a Token?
        full = false;       // remove Token from buffer
        return buffer;
    }

    char ch;
    cin.get(ch);            // look for any char including whitespace

    while (isspace(ch) && ch != '\n') cin.get(ch);

    switch (ch) {
        case '\n':
            return Token{print};
        case print:
        case quit:
        case '(':
        case ')':
        case '{':
        case '}':
        case '!':
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '=':
        case ',':
            return Token { ch };    // let each character represent itself
        case '.':
        case '0': 
        case '1': 
        case '2': 
        case '3': 
        case '4':
        case '5': 
        case '6': 
        case '7': 
        case '8': 
        case '9':
            {
                cin.putback(ch);    // put digit back into input stream
                double val;
                cin >> val;         // read floating-point number
                return Token { number, val };  
            }
        default:
            if (isalpha(ch)) {
                string s;
                s += ch;
                while (cin.get(ch) &&
                        ((isalpha(ch) || isdigit(ch) || ch == '_')))
                    s += ch;
                cin.putback(ch);
                if (s == declkey) return Token{let};    // declaration keyword
                else if (s == sqrtkey) return Token{square_root};
                else if (s == expkey) return Token{exponent};
                else return Token{name, s};
            }
            error("Bad token");
    }
};

Token_option ts;        // provides get() and putback() 


class Variable {
public:
    string name;
    double value;
    bool var;
    Variable(string n, double v, bool va = true)
        : name{n}, value{v}, var{valarray} { }
};

vector<Variable> var_table;

class Symbol_table {
    vector<Variable> var_table;
public:
    bool is_declared(string);
    double get_value(string);
    double set_value(string, double);
    double define_name(string, double, bool con = false);
};

bool Symbol_table::is_declared(string var)
    // is var already in var_table?
{
    for (const Variable& v : var_table)
        if (v.name == var) return true;
    return false;
}

double Symbol_table::get_value(string s)
    // return the value of the Variable named s
{
    for (const Variable& v : var_table)
        if (v.name == s) return v.value;
    error("get: undefined variable ", s);
}

double Symbol_table::set_value(string s, double d)
    // set the Variable named s to d
{
    for (Variable& v : var_table)
        if (v.name == s) {
            if (v.constant) error("Can't overwrite constant variable");
            v.value = d;
            return d;
        }
    error("set: undefined variable ", s);
}

double Symbol_table::define_name(string var, double val, bool con)
    // add {var,val,con} to var_table
{
    if (is_declared(var)) error(var, " declared twice");
    var_table.push_back(Variable{var,val,con});
    return val;
}


Symbol_table st;            // allows Variable storage and retrieval//

double get_value(string s)
    // return the value of the Variable names s
{
    for (int i = 0; i<var_table.size(); ++i)
        if (var_table[i].name == s) return var_table[i].value;
    error("get: undefined variable ", s);
}

void set_value(string s, double d)
    // set the Variable named s to d
{
    for (int i = 0; i<var_table.size(); ++i)
        if (var_table[i].name == s) {
			if (var_table[i].var==false) error(s," is a constant");
            var_table[i].value = d;
            return;
        }
    error("set: undefined variable ", s);
}

bool is_declared(string var)
    // is var already in var_table?
{
    for (int i = 0; i<var_table.size(); ++i)
        if (var_table[i].name == var) return true;
    return false;
}

double define_name(string s, double val, bool var=true)
    // add (s,val,var) to var_table
{
    if (is_declared(s)) error(s," declared twice");
    var_table.push_back(Variable(s,val,var));
    return val;
}

// additional calculator functions

double expression();        // forward declaration for primary to call

double calc_sqrt()
{
    char ch;
    if (cin.get(ch) && ch != '(') error("'(' expected");
    cin.putback(ch);
    double d = expression();
    if (d < 0) error("sqrt: negative val is imaginary");
    return sqrt(d);
}

double calc_pow()
{
    Token t = ts.get();
    if (t.kind != '(') error("'(' expected");
    double base = expression();
    t = ts.get();
    if (t.kind != ',') error("',' expected");
    int power = narrow_cast<int>(expression());
    t = ts.get();
    if (t.kind != ')') error("')' expected");
    return pow(base, power);
}

// input grammar functions

double primary()            // deal with numbers and parenthesis
    // ex 2 - added '{' case
{
    Token t = ts.get();
    switch (t.kind) {
        case '(':                   // handle '(' expression ')'
            {
                double d = expression();
                t = ts.get();
                if (t.kind != ')') error("')' expected");
                return d;
            }
        case '{':
            {
                double d = expression();
                t = ts.get();
                if (t.kind != '}') error("'}' expected");
                return d;
            }
        case number: 
            return t.value;         // return the number's value
        case name:
            return handle_variable(t);
        case '-':
            return -primary();
        case '+':
            return primary();
        case square_root:
            return calc_sqrt();
        case exponent:
            return calc_pow();
        default:
            error("primary expected");
    }
}



double term()              
{
    double left = primary();
    Token t = ts.get();             

    while (true) {
        switch (t.kind) {
            case '*':
                left *= primary();
                t = ts.get();
                break;
            case '/':
                {
                    double d = primary();
                    if (d == 0) error("divide by zero");
                    left /= d;
                    t = ts.get();
                    break;
                }
            case '%':
                {
                    double d = primary();
                    if (d == 0) error("%: divide by zero");
                    left = fmod(left, d);
                    t = ts.get();
                    break;
                }
            default:
                ts.putback(t);      
                return left;
        }
    }
}

double expression()         // deal with + and -
{
    double left = term();           // read and evaluate a term
    Token t = ts.get();             // get next token from Token_option

    while (true) {
        switch (t.kind) {
            case '+':
                left += term();     // evaluate term and add
                t = ts.get();
                break;
            case '-':
                left -= term();     // evaluate term and subtract
                t = ts.get();
                break;
            default:
                ts.putback(t);      
                return left;
        }
    }
}

double declaration(Token k) //k will be "let" or "con"(stant)//
    
{
    Token t = ts.get();
    if (t.kind != name) error("name expected in declaration");
    string var_name = t.name;

    Token t2 = ts.get();
    if (t2.kind != '=') error("= missing in declaration of ", var_name);

    double d = expression();
    st.define_name(var_name, d);
    return d;
}

double statement()
{
    Token t = ts.get();
    switch (t.kind) {
        case let:
            return declaration();
        default:
            ts.putback(t);
            return expression();
    }
}

void delete_operartion()
{
    ts.ignore(print);
}


void calculator()                   
{
    while (cin)
        try {
            cout << prompt;
            Token t = ts.get();
            while (t.kind == print) t = ts.get();   // discard extra 'prints'
            if (t.kind == help) print_help();
            else if (t.kind == quit) return;
            else {
                ts.putback(t);
                cout << result << statement() << '\n';
            }
        }
        catch (exception& e) {
            cerr << e.what() << '\n';       // write error message to user
            delete_operation();
        }
}


int main()
try {
    st.define_name("pi", 3.1415926535, true);  
         // constants
    st.define_name("e", 2.7182818284, true);      

    calculator();
    return 0;
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
