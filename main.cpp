/**
 * TODO object Scanner
 * TODO object Parser
 * TODO object Token
*/
# include <stdio.h>
# include <iostream>
# include <string>
# include <vector>

using namespace std;

enum Type {
  IDENTIFIER,  
  CONSTANT,    // e.g., 35, 35.67, 'a', "Hi, there", true, false
               //       .35, 35., 0023
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

class Token {
public:
  Token( string str, int type ) {
    mName_ = str;
    mTtype_ = type;
  } // Token()
  string get_name() {
    return mName_;
  } // get_name()
  int get_type() {
    return mTtype_;
  }

private:
  string mName_;
  int mTtype_;
};

class Scanner {
/**
 * * Do Lexical Analysis
 * TODO GetToken
 * TODO PeekToken
*/
public:
  Scanner() {
    mTokens_.clear();
    mCurrent_Column_ = 0;
    mCurrent_Line_ = 0;
    mLine_Buffer_ = "";
  } // Scanner()
  Token Peek_Token() {
    if ( mTokens_.size != 0 ) {
      Token temp = mTokens_.front();
      mTokens_.erase(mTokens_.begin());
      return temp;
    } // if
    else {
      /**
       * TODO when mTokens is enpty, than scan a new token list in
       */
    } // else
  }
  Token Get_Token() {
    if ( mTokens_.size != 0 ) {
      Token temp = mTokens_.front();
      mTokens_.erase(mTokens_.begin());
      return temp;
    } // if
    else {
      /**
       * TODO when mTokens is enpty, than scan a new token list in
       */
    } // else
  }

private:
  vector<Token> mTokens_;
  string mLine_Buffer_;
  int mCurrent_Column_, mCurrent_Line_;
  void Reset() {
    mTokens_.clear();
    mCurrent_Column_ = 0;
    mCurrent_Line_ = 0;
    mLine_Buffer_ = "";
  } // Reset()
  string Read_line_() {
    string str;
    scanf ( "%s", &str);
    return str;
  } // Read_line
  
};

class Parser {
/**
 * * Do Syntax Analysis 
*/
public:
  
private:
  Scanner mScanner;
};

int main() {
  int test_number = 0;
  bool quit = false;
  Parser parser;
  scanf( "%d", &test_number );
  printf( ">>Program starts...\n" );
  do {
    printf( ">" );
    // TODO call parser here than
  } while( !quit ); // TODO solve the problem of EOF
} // main()
