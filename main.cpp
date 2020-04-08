# include <string>
# include <vector>
# include <iostream>

using namespace std;

enum {
  LETTER = 224, // a~b A~Z
  DIGIT,        // 0~9
  BLANK,
  TAB,
  NEWLINE,
  NONE,
  OTHER,
  PL_EOF,
  // Value
  INTVALUE,
  FLOATVALUE,
  // Type
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
  ASSIGN,        // :=
  // Reserved Word
  QUIT        // quit
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
    if ( mNext_Token_.Type() == NONE ) {
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
          else 
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
            error_msg += token_name;
            error_msg += "'"; 
            Reset();
            throw error_msg;
          } // if()
          else if ( state == IDENTIFIER )
            mNext_Token_ = Token( token_name, Reserved_Word_( token_name ) );
          else 
            mNext_Token_ = Token( token_name, state );
          stop = true;

        } // else 

      } // while()

    } // if() 

    return mNext_Token_;
  } // Peek_Token()
  
  Token Get_Token() {
    Token token =  Peek_Token();
    mNext_Token_.Reset();
    return token;
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

  int Reserved_Word_( string str ) {
    /*if ( str == "quit" )
      return QUIT;
      else
    */
    return IDENTIFIER;
  } // Reserved_Word_()

};

class Parser {
/**
 * * Do Syntax Analysis 
*/
public:
  Parser() {
    mScnr_ = Scanner();
  } // Parser()

  vector<Token> Parse() {
    if ( Is_Command_() ) 
      return mTokens_;
    else 
      Error_();

  } // Parse()

private:
  Scanner mScnr_;
  // * Store Tokens after parse
  vector<Token> mTokens_;

  // * If the Token match parameter, add it at the end of mTokens.
  bool Push_If_Match_( int num ) {
    if ( mScnr_.Peek_Token().Type() == num ) {
      mTokens_.push_back( mScnr_.Get_Token() );
      return true;
    } // if 
    else return false;
  } // Push_If_Match()

  // * Syntactic Error (token recognized)
  void Error_() {
    string error = "Unexpected token : '";
    error += mScnr_.Get_Token().Name();
    error += "'";
    mScnr_.Reset();
    mTokens_.clear();
    throw error;
  } // Error()

  // * NUM
  bool Is_Num_() {
    if ( Push_If_Match_( INTVALUE ) || Push_If_Match_( FLOATVALUE) )
      return true;
    else return false;

  } // Is_Num_()
  
  // * - or +
  bool Is_Sign_() {
    if ( Push_If_Match_( PLUS ) || Push_If_Match_( MINUS ) )
      return true;
    else return false;
  } // Is_Sign()

  // * [SIGN] NUM | IDENT | '(' <Arith Exp> ')'
  bool Is_Factor_() {
    if ( Push_If_Match_( IDENTIFIER ) )
      return true;
    else if ( Is_Sign_() && Is_Num_() || Is_Num_() )
      return true;
    else {
      if ( Push_If_Match_( LPAR ) ) {
        if ( Is_Arith_Exp_() ) { 
          if ( Push_If_Match_( RPAR ) )
            return true;
          else 
            Error_(); // * With out RPAR
        } // if ()
        else {
          Error_(); // * With out LPAR
          return false;
        } // else 
      } // if 
    } // else 
  } // Is_Factor_()

  // * <Term> ::= <Factor> { '*' <Factor> | '/' <Factor> }
  bool Is_Term_() {
    if ( Is_Factor_() ) {
      while ( Push_If_Match_( STAR ) || Push_If_Match_( SLASH ) ) {
        if ( !Is_Factor_() ) 
          Error_();
      } // while()

      return true;
    } // if ()
    else return false;
  } // Is_Term_()

  // * <ArithExp> ::= <Term> { '+' <Term> | '-' <Term> }
  bool Is_Arith_Exp_() {
    if ( Is_Term_() ) {
      while ( Push_If_Match_( PLUS ) || Push_If_Match_( MINUS ) ) {
        if ( !Is_Term_() ) 
          Error_();
        else return true;
      } // while()
    } // if ()
    else return false;
  } // Is_Arith_Exp()

  // * Not_Id_StartFactor ::= [ SIGN ] NUM | '(' <ArithExp> ')' 
  bool Is_Not_ID_StartFactor_() {
    if ( Is_Sign_() && Is_Num_() || Is_Num_() )
      return true;
    else {
      if ( Push_If_Match_( LPAR ) ) { // (
        if ( Is_Arith_Exp_() ) { 
          if ( Push_If_Match_( RPAR ) ) // )
            return true;
          else 
            Error_(); // * With out RPAR
        } // if ()
        else 
          Error_(); // * With out LPAR
      } // if
      else return false;
    } // else
  } // Not_Id_StartFactor_()

  // * <Not_ID_StartTerm> ::= <Not_ID_StartFactor> { '*' <Factor> | '/' <Factor> }
  bool Is_Not_ID_StartTerm_() {
    if ( Is_Not_ID_StartFactor_() ) {
      while ( Push_If_Match_( STAR ) || Push_If_Match_( SLASH ) ) {
        if ( !Is_Factor_() ) 
          Error_();
      } // while
      
      return true;
    } // if
    else return false;
  } // Is_Not_ID_StartTerm_()

  // * <Not_ID_StartArithExpOrBexp> ::= <Not_ID_StartTerm> { '+' <Term> || '-' <Term> }
  bool Is_Not_ID_StartArithExpOrBexp_() {
    if ( Is_Not_ID_StartTerm_() ) {
      while ( Push_If_Match_( PLUS ) || Push_If_Match_( MINUS ) ) {
        if ( !Is_Term_() ) 
          Error_();
      } // while
      
      return true;
    } // if
    else return false;
  } // Is_Not_ID_StartArithExpOrBexp_()

  // * '=' || '<>' || '>' || '<' || '>=' || '<='
  bool Is_BooleanOperater_() {
    return Push_If_Match_( EQ ) || Push_If_Match_( NEQ ) || Push_If_Match_( GREATER ) 
        || Push_If_Match_( LESS ) || Push_If_Match_( GE ) || Push_If_Match_( LE );
  } // Is_BooleanOperater_()

  // * <IDlessArithExp> ::= { '+' <Term> | '-' <Term> | '*' <Factor> | '/' <Factor> } [<BooleanOperater> <ArithExp>]
  bool Is_IDlessArithExp_() {
    while ( Push_If_Match_( PLUS ) || Push_If_Match_( MINUS ) ) {
      if ( !Is_Term_() ) 
        Error_();
    } // while
    while ( Push_If_Match_( STAR ) || Push_If_Match_( SLASH ) ) {
      if ( !Is_Factor_() ) 
        Error_();
    } // while
    if ( Is_BooleanOperater_() ) {
      if ( !Is_Arith_Exp_() ) 
        Error_();
      
    } // if
    return true;
  } // Is_IDlessArithExp_()

  // * <Command> ::= IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp> ) ';' | <NOT_IDStartArithExpOrBexp> ';' | QUIT
  bool Is_Command_() {
    if ( Push_If_Match_( IDENTIFIER ) ) { // IDENT
      if ( Push_If_Match_( ASSIGN ) ) { // :=
        if ( Is_Arith_Exp_() ) {
          if ( Push_If_Match_( SEMI ) )
            return true;
        } //if
        Error_();
      } // if
      else if ( Is_IDlessArithExp_() && Push_If_Match_( SEMI ) ) 
        return true;
      else if ( Push_If_Match_( SEMI ) )
        return true;
      else Error_();
    } // if ()
    else if ( Is_Not_ID_StartArithExpOrBexp_() ) {
      if ( Push_If_Match_( SEMI ) )
        return true;
      else 
        Error_();
    } // if
    else if ( Push_If_Match_( QUIT ) )
      return true;
    else
      Error_();
  } // Is_Command_()

};

int main() {
  int test_number = 0;
  char newline;
  Parser parser;
  vector<Token> tokens;
  bool quit = false;
  // cin >> test_number;
  cout << ">>Program starts..." << endl;
  do {
    cout << "> ";
    try {
      tokens = parser.Parse();
    } // try
    catch( string error_info ) {
      cout << error_info << endl;
    } // carch
  } while( !quit ); // TODO solve the problem of EOF
} // main()
