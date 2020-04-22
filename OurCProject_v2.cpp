# include <string>
# include <vector>
# include <iostream>
# include <iomanip>
# include <stdlib.h>

using namespace std;

enum Type {
  LETTER = 224, // a~b A~Z
  DIGIT = 225,        // 0~9
  BLANK = 226,
  TAB = 227,
  NEWLINE = 228,
  NONE = 229, // 229
  OTHER = 230,
  PL_EOF = 231,
  // Value Type
  INTVALUE = 232, // 232
  FLOATVALUE = 233, // 233
  BOOLVALUE = 234, // 234
  UNDEFINE = 235, // 235
  // Token Type
  IDENTIFIER = 236, // 236
  TEMP = 237, // 237
  // Op
  PESO = 238, // $ 238
  SEMI = 239,  // ; 239
  ASSIGN = 240,        // := 
  EQ = 241,           // =
  NEQ = 242,          // <>
  LESS = 243,         // <
  GREATER = 244,      // >
  LE = 245,           // <=
  GE = 246,           // >=
  PLUS = 247,         // + 
  MINUS = 248,        // - 
  STAR = 249,         // * 
  SLASH = 250,        // / 
  LPAR = 251,         // (
  RPAR = 252,         // )
  // None mean
  COLON = 253,        // :
  DOT = 254,          // '.'
  COMMA = 255,        // ,
  UNDERLINE = 256,
  // Reserved Word
  QUIT = 257        // quit 
};

class Variable {
private:
  int mVar_type_;
  float mVar_value_;

public:
  Variable() {
    mVar_type_ = UNDEFINE;
    mVar_value_ = 0;
  } // Variable()

  Variable( int type, float value ) {
    mVar_type_ = type;
    mVar_value_ = value;
  } // Variable()

  void Assign( int type, float value ) {
    mVar_type_ = type;
    mVar_value_ = value;
  } // Assign()

  int Var_Type() {
    return mVar_type_;
  } // Var_Type()

  float Var_Value() {
    return mVar_value_;
  } // Var_Value()
};

class Identifer {
private:
  string mName_;
  Variable mVar_;
public:
  Identifer( string str ) {
    mName_ = str;
    mVar_ = Variable();
    
  } // Identifer()

  void Assign( int id_type, float id_value ) {
    mVar_.Assign( id_type, id_value );
    
  } // Assign()

  string Name() {
    return mName_;
  } // Name()

  Variable Var() {
    return mVar_;
  } // Var()

};

vector<Identifer> gIdTable;
vector<Variable> gTempTable;

class Token {
private:
  int mToken_Type_;
  string mName_;
  float mValue_; // if type is Identifer, here will store the index of it in Id Table

  float String_to_Float_( string str ) {
    return atof( str.c_str() );
  } // String_to_Float_()

  // * Search ID's address in ID Table
  int Get_Id_Address( string str, int i ) {
    if ( i == gIdTable.size() ) {
      Identifer id = Identifer( str );
      gIdTable.push_back( id );
      return i;
    } // if
    else if ( gIdTable[i].Name() == str )
      return i;
    else return Get_Id_Address( str, i+1 );
  } // Get_Id_Address()

public:
  Token() {
    mToken_Type_ = NONE;
    mName_ = "None";
    mValue_ = 0;
  } // Token()

  Token( string str, int type ) {
    mValue_ = 0;
    mToken_Type_ = type;
    mName_ = str;
    if ( type == IDENTIFIER )
      mValue_ = Get_Id_Address( str, 0 );
    else if ( type == INTVALUE || type == FLOATVALUE )
      mValue_ = String_to_Float_( str );
    else mValue_ = 0;
  } // Token()

  Token( int type, float index ) {
    if ( type == TEMP ) {
      mToken_Type_ = TEMP;
      mValue_ = index;
      Variable var = Variable();
      gTempTable.push_back( var );
    } // if
    
  } // Token()

  void Reset() {
    mName_ = "";
    mToken_Type_ = NONE;
    mValue_ = 0;

  } // Reset()

  int Token_Type() {
    return mToken_Type_;
  } // Token_Type()

  int Variable_Type() {
    if ( mToken_Type_ == IDENTIFIER )
      return gIdTable[mValue_].Var().Var_Type();
    else if ( mToken_Type_ == TEMP )
      return gTempTable[mValue_].Var_Type();
    else
      return mToken_Type_;
  } // Variable_Type()()

  string Name() {
    return mName_;
  } // Name()

  float Value() {
    if ( mToken_Type_ == IDENTIFIER )
      return gIdTable[mValue_].Var().Var_Value();
    else if ( mToken_Type_ == TEMP ) 
      return gTempTable[mValue_].Var_Value();
    else return mValue_;
  } // Value()

  void Assign( int value_type, float value ) {

    if ( mToken_Type_ == IDENTIFIER )
      gIdTable[mValue_].Assign( value_type, value );
    else 
      gTempTable[mValue_].Assign( value_type, value );
  } // Assign()

  int Priority() {
    if ( EQ <= mToken_Type_ && mToken_Type_ <= GE )
      return EQ;
    else if ( mToken_Type_ == MINUS )
      return PLUS;
    else if ( mToken_Type_ == SLASH )
      return STAR;
    else if ( LPAR <= mToken_Type_ && mToken_Type_ <= RPAR )
      return LPAR;
    else
      return mToken_Type_;
  } // Priority()

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
        mLine_Input_.push_back( ch );
        cin.get( ch );
      } // while

      if ( ch == '\n' )
        mLine_Input_.push_back( ch );
      else mLine_Input_.push_back( '\0' );
    } // if

    return mLine_Input_[0];
  } // Peek_Char_()

  char Get_Char_() {
    char ch = Peek_Char_();
    mLine_Input_.erase( mLine_Input_.begin() );
    if ( ch == '\n' ) {
      mCurrent_Column_ = 0;
      mCurrent_Line_++;
    } // if
    else mCurrent_Column_++;
    return ch;

  } // Get_Char_()

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
    else if ( ch == '_' ) return UNDERLINE;
    else return OTHER;

  } // Input_Type_()

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
            if ( type != LETTER && type != DIGIT && type != UNDERLINE )
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

// * Intermediate code ( op, a1, a2, a3 )
class InterCode {
private:
  int mOperater_;
  Token mArg1_, mArg2_, mArg3_;

public:
  InterCode( int op, Token a1, Token a2, Token a3 ) {
    mOperater_ = op;
    mArg1_ = a1;
    mArg2_ = a2;
    mArg3_ = a3;
  } // InterCode()

  int Operater() {
    return mOperater_;
  } // Operater()

  Token Arg1() {
    return mArg1_;
  } // Arg1()

  Token Arg2() {
    return mArg2_;
  } // Arg2()

  Token Arg3() {
    return mArg3_;
  } // Arg3()

};

vector<InterCode> gInter_codes;
int gTemp_Number;

class Compiler {
private:
  vector<Token> mOperater_stack_;
  vector<Token> mVariable_stack_;
  Token mIdentifer_;
  Compiler *mChild_;

  int Op_Stack_Pop_Back_() {
    Token tmp = mOperater_stack_.back();
    mOperater_stack_.pop_back();
    return tmp.Token_Type();

  } // Op_Stack_Pop_Back_()

  Token Var_Stack_Pop_Back_() {
    if ( mVariable_stack_.empty() )
      return Token();
    else {
      Token tmp = mVariable_stack_.back();
      mVariable_stack_.pop_back();
      return tmp;
    } // else
  } // Var_Stack_Pop_Back_()

  // * ( int op, Token arg1, Token arg2, Token arg3 )
  void Make_Inter_Code_( int type ) {
    int op = Op_Stack_Pop_Back_();
    Token a2 = Var_Stack_Pop_Back_();
    Token a1 = Var_Stack_Pop_Back_();
    Token none = Token();
    if ( op == ASSIGN ) {
      InterCode ic = InterCode( op, none, a2, mIdentifer_ );
      gInter_codes.push_back( ic );
    } // if
    else if ( op == QUIT ) {
      InterCode ic = InterCode( op, none, none, none );
      gInter_codes.push_back( ic );
    } // else if
    else if ( op == NONE && type == SEMI ) {
      Token print = a2;
      if ( print.Token_Type() == NONE )
        print = mIdentifer_;
      InterCode inter_code = InterCode( NONE, none, none, print );
      gInter_codes.push_back( inter_code );
    } // else if
    else {
      Token temp = Token( TEMP, gTemp_Number );
      gTemp_Number++;
      InterCode inter_code = InterCode( op, a1, a2, temp );
      gInter_codes.push_back( inter_code );
      mVariable_stack_.push_back( temp );
    } // else

  } // Make_Inter_Code_()

public:
  Compiler() {
    Token none = Token( "None", NONE );
    mOperater_stack_.clear();
    mOperater_stack_.push_back( none );
    mVariable_stack_.clear();
    mIdentifer_ = Token();
    mChild_ = NULL;
  } // Compiler()

  ~Compiler() {
    if ( mChild_ != NULL ) {
      delete mChild_;
      mChild_ = NULL;
    } // if

  } // ~Compiler()

  void Reset() {
    mOperater_stack_.clear();
    Token none = Token( "None", NONE );
    mOperater_stack_.push_back( none );
    mVariable_stack_.clear();
    if ( mChild_ != NULL ) {
      delete mChild_;
      mChild_ = NULL;
    } // if

  } // Reset()

  void Push_Op_Back( Token token ) {
    int type = token.Token_Type();
    int token_priority = token.Priority();
    if ( mChild_ == NULL ) {
      if ( type != LPAR ) {
        if ( ( !mOperater_stack_.empty() && token_priority <= mOperater_stack_.back().Priority() ) ||
             ( type == SEMI && !mVariable_stack_.empty() ) || 
             ( type == PESO && mOperater_stack_.size() > 1 ) ) {
          Make_Inter_Code_( type );
          Push_Op_Back( token ); 
        } // if
        else if ( type != SEMI && type != PESO )
          mOperater_stack_.push_back( token );
      } // if 
      else mChild_ = new Compiler();
    } // if
    else {
      if ( token.Token_Type() == RPAR && mChild_->mChild_ == NULL ) {
        Token peso = Token( "$", PESO );
        mChild_->Push_Op_Back( peso );
        mVariable_stack_.push_back( mChild_->mVariable_stack_.front() );
        delete mChild_;
        mChild_ = NULL;
      } // if 
      else mChild_->Push_Op_Back( token );

    } // else

  } // Push_Op_Back()

  void Push_Var_Back( Token token ) {
    if ( mChild_ == NULL )
      mVariable_stack_.push_back( token );
    else 
      mChild_->Push_Var_Back( token );

  } // Push_Var_Back()

  void Id_Assign( Token token ) {
    mIdentifer_ = token;
  } // Id_Assign()

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
  } // Error_()

  void Undefine_Error_Check_( Token token ) {
    if ( token.Variable_Type() == UNDEFINE ) {
      string error = "Undefined identifier : '";
      error += token.Name();
      error += "'";
      throw error;
    } // if

  } // Undefine_Error_Check_()

  bool Push_Var_If_Match_( int type ) {
    if ( mScnr_.Peek_Token().Token_Type() == type ) {
      Token token = mScnr_.Get_Token();
      Undefine_Error_Check_( token );
      mCompiler_.Push_Var_Back( token );
      return true;
    } // if
    else return false;
  } // Push_Var_If_Match_()

  bool Push_Op_If_Match_( int type ) {
    if ( mScnr_.Peek_Token().Token_Type() == type ) {
      Token token = mScnr_.Get_Token();
      mCompiler_.Push_Op_Back( token );
      return true;
    } // if
    else return false;
  } // Push_Op_If_Match_()

  // * NUM
  bool Is_Num_() {
    // * Translate SIGN NUM to ( 0 SIGN NUM )
    // * Ex. 1 - -1 >> 1 - ( 0 - 1 )
    if ( mScnr_.Peek_Token().Token_Type() == PLUS ||  mScnr_.Peek_Token().Token_Type() == MINUS ) {
      Token lpar( "(", LPAR );
      Token value_zero( "0", INTVALUE );
      Token rpar( ")", RPAR );
      mCompiler_.Push_Op_Back( lpar );
      mCompiler_.Push_Var_Back( value_zero );
      if ( Push_Op_If_Match_( PLUS ) || Push_Op_If_Match_( MINUS ) ) {
        if ( Push_Var_If_Match_( INTVALUE ) || Push_Var_If_Match_( FLOATVALUE ) ) {
          mCompiler_.Push_Op_Back( rpar );
          return true;
        } // if
        else Error_();
      } // if

    } // if
    else return ( Push_Var_If_Match_( INTVALUE ) || Push_Var_If_Match_( FLOATVALUE ) );

    return false;
  } // Is_Num_()

  // * [SIGN] NUM | IDENT | '(' <Arith Exp> ')'
  bool Is_Factor_() {
    if ( Push_Var_If_Match_( IDENTIFIER ) )
      return true;
    else if ( Is_Num_() )
      return true;
    else {
      if ( Push_Op_If_Match_( LPAR ) ) {
        if ( Is_Arith_Exp_() ) {
          if ( Push_Op_If_Match_( RPAR ) )
            return true;
          else
            Error_(); // * With out RPAR
        } // if ()
        else {
          Error_(); // * With out LPAR
        } // else
      } // if
    } // else

    return false;
  } // Is_Factor_()

  // * <Term> ::= <Factor> { '*' <Factor> | '/' <Factor> }
  bool Is_Term_() {
    if ( Is_Factor_() ) {
      while ( Push_Op_If_Match_( STAR ) || Push_Op_If_Match_( SLASH ) ) {
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
      while ( Push_Op_If_Match_( PLUS ) || Push_Op_If_Match_( MINUS ) ) {
        if ( !Is_Term_() )
          Error_();
      } // while()

      return true;
    } // if ()
    else return false;
  } // Is_Arith_Exp_()

  // * Not_Id_StartFactor ::= [ SIGN ] NUM | '(' <ArithExp> ')'
  bool Is_Not_ID_StartFactor_() {
    if  ( Is_Num_() )
      return true;
    else {
      if ( Push_Op_If_Match_( LPAR ) ) { // (
        if ( Is_Arith_Exp_() ) {
          if ( Push_Op_If_Match_( RPAR ) ) // )
            return true;
          else
            Error_(); // * With out RPAR
        } // if ()
        else
          Error_(); // * With out LPAR
      } // if
      else return false;
    } // else

    return false;
  } // Is_Not_ID_StartFactor_()

  // * <Not_ID_StartTerm> ::= <Not_ID_StartFactor> { '*' <Factor> | '/' <Factor> }
  bool Is_Not_ID_StartTerm_() {
    if ( Is_Not_ID_StartFactor_() ) {
      while ( Push_Op_If_Match_( STAR ) || Push_Op_If_Match_( SLASH ) ) {
        if ( !Is_Factor_() )
          Error_();
      } // while

      return true;
    } // if
    else return false;
  } // Is_Not_ID_StartTerm_()

  bool Is_Not_ID_StartArithExpOrBexp_() {
    if ( Is_Not_ID_StartArithExp_() ) {
      if ( Is_BooleanOperater_() ) {
        if ( Is_Arith_Exp_() )
          return true;
        else Error_();
      } // if

      return true;
    } // if
    else return false;

  } // Is_Not_ID_StartArithExpOrBexp_()

  // * <Not_ID_StartArithExp> ::= <Not_ID_StartTerm> { '+' <Term> || '-' <Term> }
  bool Is_Not_ID_StartArithExp_() {
    if ( Is_Not_ID_StartTerm_() ) {
      while ( Push_Op_If_Match_( PLUS ) || Push_Op_If_Match_( MINUS ) ) {
        if ( !Is_Term_() )
          Error_();
      } // while

      return true;
    } // if
    else return false;
  } // Is_Not_ID_StartArithExp_()

  // * '=' || '<>' || '>' || '<' || '>=' || '<='
  bool Is_BooleanOperater_() {
    return Push_Op_If_Match_( EQ ) || Push_Op_If_Match_( NEQ ) || Push_Op_If_Match_( GREATER )
        || Push_Op_If_Match_( LESS ) || Push_Op_If_Match_( GE ) || Push_Op_If_Match_( LE );
  } // Is_BooleanOperater_()

  // * <IDlessArithExp> ::= { '+' <Term> | '-' <Term> 
  // * | '*' <Factor> | '/' <Factor> } [<BooleanOperater> <ArithExp>]
  bool Is_IDlessArithExp_( Token token ) {
    int next_type = mScnr_.Peek_Token().Token_Type();
    if ( ( EQ <= next_type && next_type <= STAR ) || next_type == SEMI  ) {
      Undefine_Error_Check_( token );
      mCompiler_.Push_Var_Back( token );
      while ( PLUS <= next_type && next_type <= SLASH ) {
        if ( ( Push_Op_If_Match_( PLUS ) || Push_Op_If_Match_( MINUS ) ) && !Is_Term_() )
          Error_();
        else if ( ( Push_Op_If_Match_( STAR ) || Push_Op_If_Match_( SLASH ) ) && !Is_Factor_() )
          Error_();

        next_type = mScnr_.Peek_Token().Token_Type();
      } // while
   
      if ( Is_BooleanOperater_() ) {
        if ( !Is_Arith_Exp_() )
          Error_();
   
      } // if
    } // if

    return true;
  } // Is_IDlessArithExp_()

  // * <Command> ::= IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp> ) 
  // * ';' | <NOT_IDStartArithExpOrBexp> ';' | QUIT
  bool Is_Command_() {
    if ( mScnr_.Peek_Token().Is_Quit() ) {
      Token quit( "quit", QUIT ); 
      Token semi( ";", SEMI );
      mCompiler_.Push_Op_Back( quit );
      mCompiler_.Push_Op_Back( semi );
      mScnr_.Reset();
      return true;
    } // if

    if ( mScnr_.Peek_Token().Token_Type() == IDENTIFIER ) { // IDENT
      Token id = mScnr_.Get_Token();
      if ( Push_Op_If_Match_( ASSIGN ) ) { // :=
        mCompiler_.Id_Assign( id );
        if ( Is_Arith_Exp_() ) {
          if ( Push_Op_If_Match_( SEMI ) )
            return true;
        } // if

        Error_();
      } // if
      else if ( Is_IDlessArithExp_( id ) && Push_Op_If_Match_( SEMI ) ) 
        return true;
      else Error_();
    } // if ()
    else if ( Is_Not_ID_StartArithExpOrBexp_() ) {
      if ( Push_Op_If_Match_( SEMI ) )
        return true;
      else
        Error_();
    } // if
    else
      Error_();
    
    return false;
  } // Is_Command_()

public:
  Parser() {
    mScnr_ = Scanner();
  } // Parser()

  void Reset() {
    mScnr_.Reset();
    mCompiler_.Reset();
  } // Reset()
  
  void Parse() {
    mCompiler_.Reset();
    if ( !Is_Command_() )
      Error_();

  } // Parse()

};

class Runner {
private:
  int mCommand_;
  bool mQuit_;

  Variable Eval_( InterCode inter_code ) {
    int op = inter_code.Operater();
    Token a1 = inter_code.Arg1();
    Token a2 = inter_code.Arg2();
    Token a3 = inter_code.Arg3();
    int type = NONE;
    float value = 0;
    if ( op == QUIT ) {
      mQuit_ = true;
    } // if
    else if ( op == ASSIGN ) {
      type = Get_Type_( a2 );
      value = a2.Value();
      a3.Assign( type, value );
    } // else if
    else if ( EQ <= op && op <= GE ) {
      type = BOOLVALUE;
      value = Bool_Value_( op, a1.Value(), a2.Value() );
      a3.Assign( type, value );
    } // else if 
    else if ( PLUS <= op && op <= SLASH ) {
      type = Get_Type_( a1, a2 );
      value = Arith_Value_( op, a1.Value(), a2.Value() );
      a3.Assign( type, value );
    } // else if
    else if ( op == NONE ) {
      type = Get_Type_( a3 );
      value = a3.Value();
    } // else if

    return Variable( type, value );
  } // Eval_()
  
  float Arith_Value_( int op, float v1, float v2 ) {
    if ( op == PLUS )
      return v1 + v2;
    else if ( op == MINUS )
      return v1 - v2;
    else if ( op == STAR )
      return v1 * v2;
    else {
      if ( v2 == 0 ) 
        throw string( "Error" );
      else return v1 / v2;
    } // else
  } // Arith_Value_()

  float Bool_Value_( int op, float v1, float v2 ) {
    if ( ( op == EQ && v1 == v2 ) || ( op == NEQ && v1 != v2 ) || 
         ( op == LESS && v1 < v2 ) || ( op == GREATER && v1 > v2 ) || 
         ( op == LE && v1 <= v2 ) || ( op == GE && v1 >= v2 ) )
      return 1;
    else return 0;
  } // Bool_Value_()

  int Get_Type_( Token token ) {
    if ( token.Variable_Type() == UNDEFINE ) {
      string error = "Run Undefined identifier : '";
      error += token.Name();
      error += "'";
      throw error;
    } // if
    else return token.Variable_Type();
  } // Get_Type_()

  int Get_Type_( Token t1, Token t2 ) {
    if ( Get_Type_( t1 ) == FLOATVALUE || Get_Type_( t2 ) == FLOATVALUE )
      return FLOATVALUE;
    else return INTVALUE;
  } // Get_Type_()

  void Print( Variable var ) {
    if ( var.Var_Type() == INTVALUE )
      cout << fixed <<  setprecision( 0 ) << var.Var_Value() << endl;
    else if ( var.Var_Type() == FLOATVALUE )
      cout << fixed <<  setprecision( 3 ) << var.Var_Value() << endl;
    else if ( var.Var_Type() == BOOLVALUE ) {
      if ( var.Var_Value() == 0 )
        cout << "false" << endl;
      else cout << "true" << endl;
    } // else if
  } // Print()

public:
  Runner() {
    Reset();
  } // Runner()

  void Reset() {
    mCommand_ = 0;
    gInter_codes.clear();
    mQuit_ = false;
  } // Reset()

  void Run() {
    Variable var;
    while ( mCommand_ < gInter_codes.size() && !mQuit_ ) {
      var = Eval_( gInter_codes[mCommand_] );
      mCommand_++;
    } // while

    Print( var );
  } // Run()

  bool Quit() {
    return mQuit_;
  } // Quit()

};

int main() {
  char ch[10];
  Parser parser;
  Runner runner;
  cin.getline( ch, 10 );
  cout << "Program starts..." << endl;
  do {
    cout << "> ";
    try {
      parser.Parse();
      runner.Run();
    } // try
    catch( string error_info ) {
      cout << error_info << endl;
      parser.Reset();
      runner.Reset();
    } // catch
  } while ( !runner.Quit() ); // TODO solve the problem of EOF
  cout << "Program exits..." << endl;
} // main()
