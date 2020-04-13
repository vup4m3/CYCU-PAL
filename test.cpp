# include <iostream>
# include <vector>
# include <string>
using namespace std;
class Test {
public:
  int a;
  Test() {
    a = 0;
  }
  int test() {
    return test_;
  }
  void edit(int num) {
    test_ = num;
  }
  void Print() {
    cout << a << endl;
  }
private:
  int test_;
};

class Btest {
public:
  Test *mTest;
  Btest( Test *test ) {
    mTest = test;
  }
  void Print() {
    mTest->Print();
  }

};


int main() {
  string num = "45";
  float temp = atof(num.c_str());
  cout << temp << endl;
}
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
