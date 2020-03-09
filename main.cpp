/**
 * TODO object Scanner
 * TODO object Parser
 * TODO object Token
*/
# include <stdio.h>
# include <string>
# include <vector>

using namespace std;

class Token {
public:
  Token( string str, int type ) {
    mName_ = str;
    mTtype_ = type;
  } // Token()

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

private:
  vector<Token> mTokens;
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