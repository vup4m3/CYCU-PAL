# include <stdio.h>
# include <iostream>
# include <string>
# include <vector>

using namespace std;

enum Type {
  IDENTIFIER,  
  LPAR,        // (
  RPAR,        // )
  PLUS,        // +
  MINUS,       // -
  STAR,        // *
  SLASH,       // /
  GREATER,     // >
  LESS,        // <
  GE,          // >=
  LE,          // <=
  NEQ,         // <>
  EQ,          // =
  SEMI,        // ;
  COMMA,       // ,
  ASSIGN,      // :=
  INT_VALUE,
  FLOAT_VALUE,
  QUIT         // quit
};

class Token {
public:
  Token( string str, int type ) {
    mName_ = str;
    mTtype_ = type;
    mEmpty_ = false;
  } // Token()

  Token() {
    mName_ = "";
    mTtype_ = 0;
    mEmpty_ = true; 
  } // Token()

  void Delete() {
    mName_ = "";
    mTtype_ = 0;
    mEmpty_ = true;
  }

  string Name() {
    return mName_;
  } // Name()

  int Type() {
    return mTtype_;
  } // Type()

  bool Empty() {
    return mEmpty_;
  } // Empty()

private:
  string mName_;
  int mTtype_;
  bool mEmpty_;
};

class Scanner {
/**
 * * Do Lexical Analysis
 * TODO GetToken
 * TODO PeekToken
 * TODO Solve the column problem
*/
public:
  Scanner() {
    Reset();
  } // Scanner()

  void Reset() {
    mCurrent_Column_ = 0;
    mCurrent_Line_ = 0;
    mLine_Buffer_ = "";
  } // Reset()

  Token Peek_Token() { // TODO 
    if ( !mNext_Token_.Empty() ) 
      return mNext_Token_;
    else {
      /**
       * TODO when mTokens is enpty, than scan a new token
       */
    } // else
  } // Peek_Token()
  
  Token Get_Token() {
    // TODO Peek_Token and delete it
  } // Get_Token()

private:
  Token mNext_Token_; // * Next Token save in Scanner
  string mLine_Buffer_; // * Buffer of new line from keyboard input
  int mCurrent_Column_, mCurrent_Line_;

  bool Read_line_() {
    /**
     * * Read a line from keyboard input
     * * Save it in mLine_Buffer
    */
    string str;
    char ch;
    if ( scanf( "%c", & ch) != EOF ) {
      do {
        mLine_Buffer_ += ch;
      } while ( scanf( "%c", & ch) != EOF && ch != '\n' );
      if (ch = '\n') {
        mCurrent_Line_++;
        mCurrent_Column_ = 0;
        mLine_Buffer_ += ch;
      }
      return true;
    } // if ()
    else return false;
  } // Read_line

  char Peek_Char() {
    /**
     * * Peek a char from mLine_Buffer
    */
    if ( !mLine_Buffer_.empty() )
      return mLine_Buffer_[mCurrent_Column_ - 1 ];
    else {
      if ( Read_line_() ) {
        return Peek_Char();
      }
      else return '\0';
    } // else
  } // Peek_Char()

  char Get_Char() {
    char ch;
    ch = Peek_Char();
    if ( ch != '\0' )
      mLine_Buffer_.erase(0);
    return ch;
  } // Get_Char()
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
    printf( "> " );
    // TODO call parser here than
  } while( !quit ); // TODO solve the problem of EOF
} // main()
