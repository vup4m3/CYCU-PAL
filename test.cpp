# include <iostream>
# include <string>
enum Type {
  IDENTIFIER,  
  CONSTANT,    // e.g., 35, 35.67, 'a', "Hi, there", true, false, .35, 35., 0023
  INT,         // int
  FLOAT,       // float
  CHAR,        // char
  BOOL,        // bool
  STRING,      // string
  VOID,        // void
  IF,          // if 
  ELSE,        // else
  WHILE,       // while
  DO,          // do
  RETURN,      // return
  LPAR,        // (
  RPAR,        // )
  LSQB,        // [
  RSQB,        // ]
  LBRACE,      // {
  RBRACE,      // }
  PLUS,        // +
  MINUS,       // -
  STAR,        // *
  SLASH,       // /
  PERCENT,     // %
  CIRCUMFLEX,  // ^
  GREATER,     // >
  LESS,        // <
  GE,          // >=
  LE,          // <=
  EQEQ,        // ==
  NEQ,         // !=
  AMPER,       // &
  VBAR,        // |
  EQ,          // =
  NOT,         // !
  AND,         // &&
  OR,          // ||
  PE,          // +=
  ME,          // -=
  TE,          // *=
  DE,          // /=
  RE,          // %=
  PP,          // ++
  MM,          // --
  RS,          // >>
  LS,          // <<
  SEMI,        // ;
  COMMA,       // ,
  QUE,         // ?
  COLON        // :
};
class Test {
public:
  Test() {
    test_ = 0;
  }
  int test() {
    return test_;
  }
  void edit(int num) {
    test_ = num;
  }
private:
  int test_;
};

using namespace std;

int main() {
  char ch = '\0';
  string str = "123";
  int a = 0;
  int b = 0;
  if ( ++a == 0 && ++b == 1 || ++b == 1 )
    cout << "true" << endl;
  
  cout << a << endl;
  cout << b << endl;
  
}
