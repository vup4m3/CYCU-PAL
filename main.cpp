# include <string>
# include <vector>
# include <iostream>

using namespace std;

enum Type {
  LETTER = 224, // a~b A~Z
  DIGIT,        // 0~9
  IDENTIFIER,  
  DOT,          // '.'
  LPAR,         // (
  RPAR,         // )
  PLUS,         // +
  MINUS,        // -
  STAR,         // *
  SLASH,        // /
  GREATER,      // >
  LESS,         // <
  EQ,           // =
  SEMI,         // ;
  COMMA,        // ,
  COLON,        // :
  GE,           // >=
  LE,           // <=
  NEQ,          // <>
  ASSIGN,       // :=
  INTVALUE,
  FLOATVALUE,
  QUIT,        // quit
  OTHER,
  BLANK,
  TAB,
  NEWLINE,
  NONE,
  PL_EOF
};

class Token {
private:
  string mName_;
  int mType_;

public:
  Token() {
    Reset();
  } // Token()

  Token( string str, int type ) {
    mName_ = str;
    mType_ = type;
  } // Token()

  void Reset() {
    mName_ = "";
    mType_ = NONE;
  } // Delete()

  string Name() {
    return mName_;
  } // Name()

  int Type() {
    return mType_;
  } // Type()

};

class Scanner {
/**
 * * Do Lexical Analysis
*/
public:
  Scanner() {
    Reset();
  } // Scanner()

  void Reset() {
    mNext_Token_ = Token();
    mCurrent_Column_ = 0;
    mCurrent_Line_ = 1;
    mLine_Input_.clear();
  } // Reset()

  Token Peek_Token() {  
    int type, state = 0;
    bool stop = false, interupt = false;
    string token_name = "";
    if ( mNext_Token_.Type() != NONE ) 
      return mNext_Token_;
    else {
      while ( !stop ) {
        if ( state == 0 ) {
          type = Input_Type_( Peek_Char_() );
          if ( type == DIGIT )
            state = INTVALUE;
          else if ( type == DOT )
            state = FLOATVALUE;
          else if ( type == LETTER )
            state = IDENTIFIER;
          else if ( Is_One_Char_Token_Type_( type ) ) {
            token_name = Get_Char_();
            mNext_Token_ = Token( token_name, type );
            return mNext_Token_;
          } // else if ()
          else if ( type == BLANK || type == TAB || type == NEWLINE  )
            Get_Char_();
          else if ( type != BLANK && type != TAB && type != NEWLINE ) 
            state = type;
        } // if ()
        else if ( !interupt ) {
          token_name += Get_Char_();
          type = Input_Type_( Peek_Char_() );
          if ( state == INTVALUE ) {
            if ( type == DOT ) 
              state = FLOATVALUE;
            else if ( type != DIGIT ) 
              interupt = true;
          } // if ()
          else if ( state == FLOATVALUE ) { 
            if ( type != DIGIT )
              interupt = true; 
          } // else if ()
          else if ( state == IDENTIFIER ) {
            if ( type != LETTER && type != DIGIT )
              interupt = true;
          } // else if ()
          else if ( state == SLASH ) {
            if ( type == SLASH ) { // Comment
              Reset();
              return Peek_Token();
            } // if
            else interupt = true; // /
          } // else if 
          else if ( state == GREATER ) {
            if ( type == EQ )
              state = GE; // >=
            else interupt = true; // >
          } // else if ()
          else if ( state == LESS ) {
            if ( type == EQ )
              state = LE; // <=
            else if ( type == GREATER )
              state = NEQ; // <>
            else interupt = true; // <
          } // else if ()
          else if ( state == COLON ) {
            if ( type == EQ )
              state = ASSIGN; // :=
            else {
              state = OTHER;
              interupt = true; // :
            } // else  
          } // else if ()
          else interupt = true;
        } // else if ()
        else { // interupt
          if ( state == OTHER || state == FLOATVALUE && token_name == "." ) {
            string error_msg = "Unrecognized token with first char : '";
            error_msg += Get_Char_();
            error_msg += "'"; 
            Reset();
            // throw error_msg;
            return Token( "", PL_EOF );
          } // if()
          else 
            mNext_Token_ = Token( token_name, state );
          stop = true;

        } // else 

      } // while 

      return mNext_Token_;

    } // else

    return Token();
  } // Peek_Token()
  
  Token Get_Token() {
    // TODO Peek_Token and delete it
    Token token =  Peek_Token();
    mNext_Token_.Reset();
    return Token();
  } // Get_Token()

private:
  Token mNext_Token_; // * Next Token save in Scanner
  vector<char> mLine_Input_;
  int mCurrent_Column_, mCurrent_Line_;

  char Peek_Char_() {
    char ch;
    if ( mLine_Input_.empty() ) {
      cin.get( ch );
      while ( ch != '\n' && !cin.eof() ) {
        mLine_Input_.push_back(ch);
        cin.get( ch );
      } // while
      if ( ch == '\n' )
        mLine_Input_.push_back(ch);
      else mLine_Input_.push_back('\0');
    } // if
    
      return mLine_Input_[0];
    
  } // Peek_Char()

  char Get_Char_() {
    char ch = Peek_Char_();
    mLine_Input_.erase(mLine_Input_.begin());
    if ( ch == '\n' ) {
      mCurrent_Column_ = 0;
      mCurrent_Line_++;
    } // if
    else mCurrent_Column_++;
    return ch;
  } // Get_Char()

  int Input_Type_( char ch ) {
    if ( ch == '+' ) return PLUS;
    else if ( ch == '-' ) return MINUS;
    else if ( ch == '.' ) return DOT;
    else if ( ch == '(' ) return LPAR;
    else if ( ch == ')' ) return RPAR;
    else if ( ch == '*' ) return STAR;
    else if ( ch == '/' ) return SLASH;
    else if ( ch == '>' ) return GREATER;
    else if ( ch == '<' ) return LESS;
    else if ( ch == '=' ) return EQ;
    else if ( ch == ';' ) return SEMI;
    else if ( ( ch >= 'a' && ch <= 'z' ) || ( ch >= 'A' && ch <= 'Z' ) ) return LETTER;
    else if ( ch >= '0' && ch <= '9' ) return DIGIT;
    else if ( ch == ':' ) return COLON;
    else if ( ch == ' ' ) return BLANK;
    else if ( ch == '\t' ) return TAB;
    else if ( ch == '\n' ) return NEWLINE;
    else if ( ch == '\0' ) return PL_EOF;
    else return OTHER;
  } // One_Char_Token()

  bool Is_One_Char_Token_Type_( int type ) {
    if ( type == LPAR || type == RPAR || type == PLUS || type == MINUS || type == STAR || type == SEMI )
      return true;
    else return false;
  } // Is_One_Char_Token_Type_()

};

class Parser {
/**
 * * Do Syntax Analysis 
 * ! Parser should be rewrite
*/
public:
  Parser() {
    mScnr_ = Scanner();
  } // Parser()

  vector<Token> Parse() {
    if ( Is_Command_() ) 
      return mTokens_;
    else {
      Error_();
      return mTokens_;
    } // else

  } // Parse()

  
private:
  Scanner mScnr_;
  // * Store Tokens after parse
  vector<Token> mTokens_;

  // * Syntactic Error (token recognized)
  void Error_() {
    string error = "Unexpected token : '";
    error += Get_Token_().Name();
    mScnr_.Reset();
    mTokens_.clear();
    // TODO throw
  } // Error()

  // * Peek a Token from Scanner
  int Peek_Token_Type_() {
    return mScnr_.Peek_Token().Type();
  } // Peek_Token_Type_()
  
  // * Get a Token from Scanner
  Token Get_Token_() {
    return mScnr_.Get_Token();
  } // Get_Token_()

  // * [SIGN] NUM | IDENT | '(' <Arith Exp> ')'
  // TODO IDENTIFER
  bool Is_Factor_() {
    if ( Is_Sign_() )
      ;
    if ( Is_Num_() )
      return true;
    else if ( Is_Id_() )
      ;
    else {
      if ( Peek_Token_Type_() == LPAR ) {
        mTokens_.push_back( Get_Token_() );
        if ( true ) { // ! Is_Arith_Exp()
          if ( Peek_Token_Type_() == RPAR ) {
            mTokens_.push_back( Get_Token_() );
            return true;
          } // if ()
          else {
            Error_();
            return false;
          }
        } // if ()
        else {
          Error_();
          return false;
        } // else 
      } // if 
    } // else 
  } // Is_Factor_()

  // * NUM
  bool Is_Num_() {
    if ( mScnr_.Peek_Token().Type() == INTVALUE || mScnr_.Peek_Token().Type() == FLOATVALUE ) {
      mTokens_.push_back( mScnr_.Get_Token() );
      return true;
    } // if()
    else return false;

  } // Is_Num_()
  
  // * IDENT
  bool Is_Id_() {
    if ( Peek_Token_Type_() == IDENTIFIER ) {
      mTokens_.push_back( Get_Token_() );
      return true;
    } // if 
    else return false;
  } // Is_Id_()

  // * - or +
  bool Is_Sign_() {
    if ( mScnr_.Peek_Token().Type() == PLUS || mScnr_.Peek_Token().Type() == MINUS ) {
      mTokens_.push_back( mScnr_.Get_Token() );
      return true;
    } // if()
    else return false;

  } // Is_Sign_()
};

int main() {
  int test_number = 0;
  char newline;
  Parser parser;
  bool quit = false;
  // cin >> test_number;
  cout << ">>Program starts..." << endl;
  do {
    cout << "> ";
    // TODO call parser here than
  } while( !quit ); // TODO solve the problem of EOF
} // main()
