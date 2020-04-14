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
  TEMP,
  // Op
  ASSIGN,        // :=
  EQ,           // =
  NEQ,          // <>
  LESS,         // <
  GREATER,      // >
  LE,           // <=
  GE,           // >=
  PLUS,         // +
  MINUS,        // -
  STAR,         // *
  SLASH,        // /
  LPAR,         // (
  RPAR,         // )
  SEMI,         // ;
  // None mean
  COLON,        // :
  DOT,          // '.'
  COMMA,        // ,
  // Reserved Word
  QUIT        // quit
};

class Identifer {
public:
  string name;
  int type;
  float value;

  Identifer( string str ) {
    name = str;
    type = 0;
    value = 0;
  } // Identifer()

};

class IdTable {
private:
  // * Debug
  vector<Identifer> mId_Table_;
  void IndexError_( int index ) {
    if ( index >= mId_Table_.size() )
      throw string( "IdTable index Error !!" );
  } // IndexError_()

public:
  int Get_Index( string name ) {
    for ( int i = 0; i < mId_Table_.size(); i++ ) {
      if ( name == mId_Table_[i].name )
        return i;
    } // for

    mId_Table_.push_back( Identifer( name ) );
    return mId_Table_.size() - 1;
  } // Get_Index()

  float Value( int index ) {
    IndexError_( index );
    return mId_Table_[index].value;
  } // Value

  int Value_Type( int index ) {
    IndexError_( index );
    return mId_Table_[index].type;
  } // Type

  void Assign(int index, int type, float value ) {
    IndexError_( index );
    mId_Table_[index].type = type;
    mId_Table_[index].value = value;
  } // Assign

};

IdTable gIdTable;

class Token {
private:
  int mType_;
  string mName_;
  float mValue_; // if type is Identifer, here will store the index of it in Id Table

  float String_to_Float_( string str ) {
    return atof( str.c_str() );
  } // String_to_Float_()

public:
  Token() {
    Reset();
  } // Token()

  Token( string str, int type ) {
    mType_ = type;
    mName_ = str;
    if ( type == IDENTIFIER )
      mValue_ = gIdTable.Get_Index( str );
    else if ( type == INTVALUE || type == FLOATVALUE )
      mValue_ = String_to_Float_( str );
    else mValue_ = 0;
  } // Token()

  Token( int type, float index ) {
    mType_ = type;
    mName_ = "temp";
    mValue_ = index;
  } // Token()

  void Reset() {
    mName_ = "";
    mType_ = NONE;
    mValue_ = 0;

  } // Delete()

  int Token_Type() {
    return mType_;
  } // Type()

  int Value_Type() {
    if ( mType_ == INTVALUE || mType_ == FLOATVALUE )
      return mType_;
    else
      return gIdTable.Value_Type( mValue_ );
  } // Value_Type()

  string Name() {
    return mName_;
  } // Name()

  float Value() {
    if ( mType_ == FLOATVALUE || mType_ == INTVALUE )
      return mValue_;
    else if ( mType_ == IDENTIFIER ) {
      return gIdTable.Value( mValue_ );
    } // else if
    else
      throw string( "Error!! you should not ask for Value...." ); // * Only for debug
  } // Value()

  void Assign( int type, float value ) {
    gIdTable.Assign( mValue_, type, value );
  } // Assign

  bool Is_Quit() {
    return mName_ == "quit";
  } // Is_Quit()

};

class Scanner {
// * Do Lexical Analysis
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
    return type == LPAR || type == RPAR || type == PLUS || type == MINUS || type == STAR || type == SEMI;
  } // Is_One_Char_Token_Type_()

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
    if ( mNext_Token_.Token_Type() == NONE ) {
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
          if ( state == OTHER || ( state == FLOATVALUE && token_name == "." ) ) {
            string error_msg = "Unrecognized token with first char : '";
            error_msg += token_name;
            error_msg += "'";
            Reset();
            throw error_msg;
          } // if()
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

};

// * Intermediate code
class InterCode {
public:
  int operater;
  Token arg1, arg2, arg3;
  InterCode( int op, Token a1, Token a2, Token a3 ) {
    operater = op;
    arg1 = a1;
    arg2 = a2;
    arg3 = a3;
  } // InterCode()

};

vector<InterCode> gInter_codes;
int gTemp_Number;

class Compiler {
private:
  vector<Token> mOperater_stack_;
  vector<Token> mVariable_stack_;
  Compiler *mChild_;
  bool mSwitch_Control_;

  void Call_Child_( Token token) {
    if ( token.Token_Type() == RPAR ) {
      mSwitch_Control_ = false;
      mChild_->Push_Token_Back( Token( ";", SEMI ) );
      delete mChild_;
      mChild_ = NULL;
    } // if
    else mChild_->Push_Token_Back( token );
  } // Call_Child_()

  int Op_Stack_Pop_Back_() {
    Token tmp = mOperater_stack_.back();
    mOperater_stack_.pop_back();
    return tmp.Token_Type();
  } // Op_Stack_Pop_Back()

  Token Val_Stack_Pop_Back_() {
    Token tmp = mOperater_stack_.back();
    mVariable_stack_.pop_back();
    return tmp;
  } // Val_Stack_Pop_Back_()

  // * ( int op, Token arg1, Token arg2, Token arg3 )
  void Make_Inter_Code_( Token token ) {
    int op = Op_Stack_Pop_Back_();
    Token a2 = Val_Stack_Pop_Back_();
    Token a1 = Val_Stack_Pop_Back_();
    if ( op == ASSIGN ) 
      gInter_codes.push_back( InterCode( op, Token( "None", 0 ), a2, a1 ) );
    else {
      Token temp = Token( TEMP, gTemp_Number );
      gTemp_Number++;
      gInter_codes.push_back( InterCode( op, a1, a2, temp ) );
    } // else
    if ( token.Token_Type() == SEMI && mVariable_stack_.size() != 0 )
      Make_Inter_Code_( token );
  } // mMake_Intercode

public:
  Compiler() {
    gInter_codes.clear();
    mOperater_stack_.clear();
    mOperater_stack_.push_back( Token( "$", NONE ) );
    mVariable_stack_.clear();
    mChild_ = NULL;
  } // Compiler()

  ~Compiler() {
    if ( mChild_ != NULL ) {
      delete mChild_;
      mChild_ = NULL;
    } // if
  } // Compilar()

  void Reset() {
    gInter_codes.clear();
    mOperater_stack_.clear();
    mOperater_stack_.push_back( Token( "$", NONE ) );
    mVariable_stack_.clear();
    gTemp_Number = 0;
    if ( mChild_ != NULL ) {
      delete mChild_;
      mChild_ = NULL;
    } // if
    mSwitch_Control_ = false;
  } // Reset()

  void Push_Token_Back( Token token ) {
    if ( !mSwitch_Control_ ) {
      if ( token.Token_Type() != LPAR ) { // (
        if ( token.Token_Type() >= ASSIGN ) {
          if ( token.Token_Type() <= mOperater_stack_.back().Token_Type() ) {
            Make_Inter_Code_( token );
          } // if
          if ( token.Token_Type() != SEMI )
            mOperater_stack_.push_back( token );
        } // if
        else
          mVariable_stack_.push_back( token );

      } // if
      else { // * meet '('
        mSwitch_Control_ = true;
        mChild_ = new Compiler();
      } // else

    } // if
    else Call_Child_(token);

  } // Push_Token_Back()

};

// * Do Syntax Analysis
class Parser {
private:
  Scanner mScnr_;
  // * Store Tokens after parse
  vector<Token> mTokens_;
  Compiler mCompiler_;

  // * Syntactic Error (token recognized)
  void Error_() {
    string error = "Unexpected token : '";
    error += mScnr_.Get_Token().Name();
    error += "'";
    mScnr_.Reset();
    mCompiler_.Reset();
    throw error;
  } // Error()

  // * If the Token match parameter, add it at the end of mTokens.
  bool Push_If_Match_( int type ) {
    if ( mScnr_.Peek_Token().Token_Type() == type ) {
      mCompiler_.Push_Token_Back( mScnr_.Get_Token() );
      return true;
    } // if
    else return false;
  } // Push_If_Match()

  // * NUM
  bool Is_Num_() {
    // * Translate SIGN NUM to ( 0 SIGN NUM )
    // * Ex. 1 - -1 >> 1 - ( 0 - 1 )
    if ( mScnr_.Peek_Token().Token_Type() == PLUS ||  mScnr_.Peek_Token().Token_Type() == MINUS ) {
      mCompiler_.Push_Token_Back( Token( "(", LPAR ) );
      mCompiler_.Push_Token_Back( Token( "0", INTVALUE ) );
      if ( Push_If_Match_( PLUS ) || Push_If_Match_( MINUS ) ) {
        if ( Push_If_Match_( INTVALUE ) || Push_If_Match_( FLOATVALUE) )
          mCompiler_.Push_Token_Back( Token( ")", RPAR ) );
        else Error_();
      } // if

    } // if
    else return ( Push_If_Match_( INTVALUE ) || Push_If_Match_( FLOATVALUE) );
  } // Is_Num_()

  // * - or +
  bool Is_Sign_() {
    return Push_If_Match_( PLUS ) || Push_If_Match_( MINUS );
  } // Is_Sign()

  // * [SIGN] NUM | IDENT | '(' <Arith Exp> ')'
  bool Is_Factor_() {
    if ( Push_If_Match_( IDENTIFIER ) )
      return true;
    else if ( ( Is_Sign_() && Is_Num_() ) || Is_Num_() )
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
    if  (( Is_Sign_() && Is_Num_() ) || Is_Num_() )
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
    if ( mScnr_.Peek_Token().Is_Quit() ) {
      mCompiler_.Push_Token_Back( Token( "quit", QUIT ) );
      mScnr_.Reset();
      return true;
    } // if
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

public:
  Parser() {
    mScnr_ = Scanner();
  } // Parser()

  void Parse() {
    if ( !Is_Command_() )
      Error_();

  } // Parse()

};

int main() {
  int test_number = 0;
  Parser parser;
  bool quit = false;
  // cin >> test_number;
  cout << ">>Program starts..." << endl;
  do {
    cout << "> ";
    try {
      parser.Parse();

    } // try
    catch( string error_info ) {
      cout << error_info << endl;

    } // carch
  } while( !quit ); // TODO solve the problem of EOF
} // main()
