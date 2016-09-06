#include <iostream>
#include <string>
#include <sstream>
using namespace std;
const string eof("EOF");
const string INTEGER("INTEGER"),LPAREN("("),RPAREN(")");
const string PLUS("PLUS"),MINUS("MINUS"),MUL("MUL"),DIV("DIV");
class Token
{
public:
    Token()=default;
    Token(string Type,int Value):value(Value),type(Type){}

    int value;
    string type;
};

class Lexer{
public:
    Lexer(string Text):text(Text){}
    void error(){cerr<<"Error parsing input";};

    Token get_next_token();
    void Advance();
    void skip_space();
    int integer();

private:
    string text;
    int pos=0;
    Token current_token;
    char current_char=text[pos];
};

class Interpreter{
private:
    Token current_token;
    Lexer lexer;
public:
    Interpreter(Lexer l):lexer(l),current_token(l.get_next_token()){}
    void error(){cerr<<"Invalid syntax";};

    void eat(string token_type){
        if (current_token.type==token_type)
            current_token = lexer.get_next_token();
        else
            error();
    };
    int factor();

    int term();

    int expr();
};

int Interpreter::term(){
    int ret = factor();

    while(current_token.type==MUL||current_token.type==DIV)
    {
        auto token = current_token;
        if(token.type==MUL)
        {
            eat(MUL);
            ret*=factor();
        }
        else if (token.type==DIV)
        {
            eat(DIV);
            ret/=factor();
        }
    }

    return ret;
}
int Interpreter::factor(){
    auto token = current_token;
    if(token.type==INTEGER)
    {
        eat(INTEGER);
        return token.value;
    }
    else if(token.type==LPAREN)
    {
        eat(LPAREN);
        int ret = expr();
        eat(RPAREN);
        return ret;
    }

}
void Lexer::Advance(){
    pos++;
    if (pos>text.size()-1)
        current_char = 0;
    else
        current_char = text[pos];
}
void Lexer::skip_space(){
    while(isspace(current_char)&&(current_char!=0))
    {
        Advance();
    }
}
int Lexer::integer(){
    string result="";
    while((current_char!=0)&&isdigit(current_char) )
    {
        result+=current_char;
        Advance();
    }
    stringstream ss;
    int ret;
    ss<<result;
    ss>>ret;
    return ret;
}
Token Lexer::get_next_token(){

    while(current_char!=0){

    if( isspace(current_char) )
      {
    //    cout<<"space1"<<endl;
        skip_space();
        continue;
      }

    if(isdigit(current_char))
        return Token(INTEGER,integer());
     if(current_char=='+')
     {
         Advance();
         return Token(PLUS,'+');
     }

     if(current_char=='-')
     {
         Advance();
         return Token(MINUS,'-');
     }
     if(current_char=='/')
     {
         Advance();
         return Token(DIV,'/');
     }

     if(current_char=='*')
     {
         Advance();
         return Token(MUL,'*');
     }
     if(current_char=='(')
     {
         Advance();
         return Token(LPAREN,'(');
     }

     if(current_char==')')
     {
         Advance();
         return Token(RPAREN,')');
     }
     error();
    }

    return Token(eof,0);
}
int Interpreter::expr(){

    int ret = term();
    while(current_token.type==PLUS||current_token.type==MINUS)
    {
        auto token = current_token;
        if(token.type==PLUS)
        {
            eat(PLUS);
            ret +=term();
        }
        else if(token.type==MINUS)
        {
            eat(MINUS);
            ret-=term();
        }
    }

    return ret;

}
int main()
{
    string line;
    while(1)
    {
        cout<<"calc> ";
        if(!getline(cin,line))
            continue;

        if(line.empty())
            continue;
        Lexer lexer(line);
        Interpreter interpreter(lexer);
        cout<<interpreter.expr()<<endl;
    }
    return 0;
}
