# include <iostream>
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

int main() {
  Test a1;
  Test b1;
  b1 = a1;
  a1.edit(1);
  b1.edit(2);
  std::cout << a1.test() << std::endl;
  b1 = a1;
  std::cout << b1.test() << std::endl;

}
