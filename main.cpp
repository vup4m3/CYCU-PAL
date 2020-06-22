// TODO EOF process 
# include <string>
# include <vector>
# include <iostream>
# include <iomanip>
# include <stdlib.h>
# include <sstream>

using namespace std;

enum Type {
  LETTER = 224, DIGIT, BLANK,
  TAB, NEWLINE, NONE,
  OTHER, PL_EOF, UNDERLINE,
  DOT, QUOT, QUOTQUOT,
  ARRAY,
  // Reserved word
  INT, FLOAT, CHAR, 
  BOOL, STRING, VOID,
  IF, ELSE, WHILE,
  DO, RETURN,
  // Identifier Type
  IDENTIFER,
  FUNCTION, UNDEFINE, CONSTANT,
  // Constant Type
  INT_CONS, FLOAT_CONS, STR_CONS,
  CHAR_CONS, TRUE_CONS, FALSE_CONS,
  LPAR,         // (
  RPAR,         // )
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
  // ,
  COMMA,       // ,
  QUE,         // ?
  COLON,       // :
  BACKSLASH
};

// * 1 : unrecognized token with first char
// * 2 : unexpected token 
// * 3 : undefined identifier 
class Error_Info {
private:
  int mLine_;
  int mType_;
  string mName_;
public:
  Error_Info( int line, int type, string name ) {
    mLine_ = line;
    mType_ = type;
    mName_ = name;
  } // Error_Info()

  void PrintMsg() {
    string msg = "Line ";
    stringstream ss;
    ss << mLine_;
    msg += ss.str();
    msg += " : ";
    if ( mType_ == 1 ) 
      msg += "unrecognized token with first char : '";
    else if ( mType_ == 2 )
      msg += "unexpected token : '";
    else msg += "undefined identifier : '";

    msg += mName_;
    msg += "'";
    cout << msg << endl;
  } // PrintMsg()

  int Error_Line() {
    return mLine_;
  } // Error_Line()
};

class PrettyPrint {
private:
  int mType_;
  vector<bool> mIs_New_Def_;
  vector<bool> mIs_Func_;
  vector<string> mId_;
public:
  PrettyPrint() {
    Reset();
  } // PrettyPrint()

  void Reset() {
    mType_ = -1;
    mIs_Func_.clear();
    mIs_New_Def_.clear();
    mId_.clear();
  } // Reset()

  void Stm() {
    mType_ = 0;
  } // Stm()
  // * New func id_name
  void Definition( bool is_new, bool is_func, string name ) {
    mType_ = 1;
    mIs_New_Def_.push_back( is_new );
    mIs_Func_.push_back( is_func );
    mId_.push_back( name );
  } // Definition()
  
  void Print() {
    if ( mType_ == 0 ) {
      cout << "Statement executed ..." << endl;
    } // if 
    else if ( mType_ == 1 ) {
      string msg;
      for ( int i = 0; i < mId_.size() ; i++ ) {
        if ( mIs_New_Def_[i] )
          msg = "New definition of ";
        else 
          msg = "Definition of ";

        msg += mId_[i];
        if ( mIs_Func_[i] )
          msg += "()";
        
        msg += " entered ...";

        cout << msg << endl;
      } // for

    } // else if
    
  } // Print()
};

// * mConstant_Type : TRUE_CONS | FALSE_CONS | INT 
// *                | FLOAT | CHAR | STRING
class Token {
private:
  int mToken_Type_;
  int mConstant_Type_;
  string mName_;
  int mLine_;

public:
  Token() {
    mToken_Type_ = 0;
    mConstant_Type_ = UNDEFINE;
    mName_ = "";
    mLine_ = 1;
  } // Token()

  Token( int type, string name, int line ) {
    mName_ = name;
    mConstant_Type_ = 0;
    mLine_ = line;
    if ( type == INT_CONS || type == STR_CONS || type == CHAR_CONS || type == FLOAT_CONS ||
         type == TRUE_CONS || type == FALSE_CONS ) {
      if ( type == TRUE_CONS || type == FALSE_CONS ) 
        mConstant_Type_ = type;
      else if ( type == INT_CONS )
        mConstant_Type_ = INT;
      else if ( type == FLOAT_CONS )
        mConstant_Type_ = FLOAT;
      else if ( type == CHAR_CONS )
        mConstant_Type_ = CHAR;
      else mConstant_Type_ = STRING;

      mToken_Type_ = CONSTANT;
    } // if
    else mToken_Type_ = type;

  } // Token()

  int Token_Type() {
    return mToken_Type_;
  } // Token_Type()

  int Cons_Type() {
    return mConstant_Type_;
  } // Cons_Type()

  string Name() {
    return mName_;
  } // Name()
  
  int Line() {
    return mLine_;
  } // Line()

  void LineReset() {
    mLine_ = 1;
  } // LineReset()
  // * Print this token
  void Print() {
    if ( mConstant_Type_ != STR_CONS && mConstant_Type_ != CHAR_CONS ) 
      cout << mName_;
    else if ( mConstant_Type_ == CHAR_CONS )
      cout << "'" << mName_ << "'";
    else  cout << "\"" << mName_ << "\"" ;
  } // Print()

};

class StringPool {
private:
  vector<string> mPool_;
public:
  int Get_Addr( string str ) {
    
    for ( int i = 0; i < mPool_.size() ; i++ )
      if ( str == mPool_[i] )
        return i;
    
    mPool_.push_back( str );
    return mPool_.size() - 1;
  } // Get_Addr()

  string Load_Str( int addr ) {
    return mPool_[addr];
  } // Load_Str()
}; 

StringPool gStr_Pool;

// * string(addr) int char float bool array
class VarType {
private:
  int mType_; // string | int | char | flaot
  bool mAddr_; // Addr flag
  int mArray_; // 0 mean not a array
public:
  VarType() {
    mType_ = 0;
    mAddr_ = false;
    mArray_ = 0;
  } // VarType()

  VarType( int type, bool is_addr, int array ) {
    mType_ = type;
    mAddr_ = is_addr;
    mArray_ = array;
  } // VarType()

  int Frame_Space() {
    return ( mArray_ == 0 ) ?  1 :  mArray_;
  } // Frame_Space()

  int Type() {
    return mType_;
  } // Type()
};

class Variable {
private:
  VarType mType_;
  float mValue_;
public:
  Variable() {
    mType_ = VarType();
    mValue_ = 0;
  } // Variable()

  Variable( VarType type, float value ) {
    mType_ = type;
    mValue_ = value;
  } // Variable()
  // * For Constant
  Variable( int type, string str ) {
    if ( type == TRUE_CONS || FALSE_CONS ) {
      VarType var_type( BOOL, false, 0 );
      mType_ = var_type;
      ( type == TRUE_CONS ) ? mValue_ = 1 : mValue_ = 0;
    } // if
    else {
      VarType var_type( type, false, 0 );
      mType_ = var_type;
      if ( type == INT || type == FLOAT ) 
        mValue_ = atof( str.c_str() );
      else if ( type == CHAR )
        mValue_ = str[0];
      else 
        mValue_ = gStr_Pool.Get_Addr( str );

    } // else 
  } // Variable()

  VarType Var_Type() {
    return mType_;
  } // Var_Type()

  float Value() {
    return mValue_;
  } // Value()

};

class VarId {
private:
  VarType mType_;
  string mName_;
  int mAddr_;
public:
  VarId() {
    mType_ = Var_Type();
    mName_ = "";
    mAddr_ = 0;
  } // VarId()

  VarId( VarType type, string name, int addr ) {
    mType_ = type;
    mName_ = name;
    mAddr_ = addr;
  } // VarId()

  VarType Var_Type() {
    return mType_;
  } // Var_Type()

  string Name() {
    return mName_;
  } // Name()

  int Addr() {
    return mAddr_;
  } // Addr()
};

class FuncInfo {
private:
  VarType mType_;
  string mName_;
  vector<VarType> mType_List_;
  int mAddr_;
  vector<Token> mFunc_Code_;
public:
  FuncInfo() {
    mType_ = VarType();
    mName_ = "";
    mAddr_ = 0;
  } // FuncInfo()

  FuncInfo( int type, string name, vector<VarType> type_list, int addr, vector<Token> func_code ) {
    mType_ = VarType( type, false, 0 );
    mName_ = name;
    mType_List_ = type_list;
    mAddr_ = addr;
    mFunc_Code_ = func_code;
  } // FuncInfo()

  VarType Type() {
    return mType_;
  } // Type()

  string Name() {
    return mName_;
  } // Name()

  int Addr() {
    return mAddr_;
  } // Addr()

  int Parm() {
    return mType_List_.size();
  } // Parm()

  vector<Token> Code() {
    return mFunc_Code_;
  } // Code()

};

class ConstantPool {
private:
  vector<string> mNames_;
  vector<Variable> mVars_;
  
public:
  int Get_Addr( Token token ) {
    string name = token.Name();
    int i = 0;
    for ( i = 0 ; i < mNames_.size() ; i++ )
      if ( mNames_[i] == name ) 
        return i;
    
    Variable var( token.Cons_Type(), name );
    mNames_.push_back( name );
    mVars_.push_back( var );
    return i;
  } // Get_Addr()

  Variable Get_Var( int addr ) {
    if ( addr > mNames_.size() ) 
      throw string( "Segmentation fault ( Constant )" );
    else 
      return mVars_[addr];
  } // Get_Var()

};
// +--------------+
// | Stack        | // Local Variable and Function Arguement
// +--------------+ <- Stack_Begin
// | Data segment | // Global Variable
// +--------------+
class Mem {
private:
  vector<Variable> mData_;
  int mStack_Begin_;
public: 
  Mem() {
    mStack_Begin_ = 0;
  } // Mem()

  void MakeStackFrame( int frame_size ) {
    Variable var;
    for ( int i = 0; i < frame_size ; i++ )
      mData_.push_back( var );
  } // MakeStackFrame()

  int Alloc_Data( int frame_size ) {
    Variable var;
    int addr = mStack_Begin_;
    for ( int i = 0; i < frame_size ; i++, mStack_Begin_++ )
      mData_.insert( mData_.begin(), var );

    return addr;
  } // Alloc_Data()

  Variable Load_Var( int addr ) {
    if ( addr > mData_.size() ) 
      throw string( "Segmentation fault" );
    else 
      return mData_[addr];
  } // Load_Var()

  void Store( int addr, Variable var ) {
    if ( addr > mData_.size() )
      throw string( "Segmentation fault" );
    else mData_[addr] = var;
  } // Store()

};

enum Instr {
  // * Alloc space in mem_stack
  STACK_ALLOC = 556,
  // * Alloc space in Data
  DATA_ALLOC,
  // * Load address than push it in to stack
  LOAD_ADDR,
  // * Load Constant than push it in to stack
  LOAD_CONS,
  // * just the names of the (global) variables
  LIST_ALL_VAR,
  // * just the names of the (user-defined)
  LIST_ALL_FUNC,
  // * the definition of a particular variable, arg pop from Stack
  LIST_VAR,
  // * the definition of a particular function, arg pop from Stack
  LIST_FUNC,
  DONE,
  // * Call Func
  CALL

};

class InterCode {
private:
  int mInstruction_;
  bool mAbsolute_;
  int mParm_;
public:
  InterCode() {
    mInstruction_ = 0;
    mAbsolute_ = 0;
    mParm_ = 0;
  } // InterCode()

  InterCode( int instr, bool relative, int parm ) {
    mInstruction_ = instr;
    mAbsolute_ = relative;
    mParm_ = parm;
  } // InterCode()

  int Instr() {
    return mInstruction_;
  } // Instr()

  bool Abs() {
    return mAbsolute_;
  } // Abs()

  int Parm() {
    return mParm_;
  } // Parm()
  
};

class Data {
private:
  int mFunc_Code_End_;
  Mem mMem_;

  void Error_( Token token ) {
    Error_Info info( token.Line(), 3, token.Name() );
    throw info;
  } // Error_()

public:
  PrettyPrint mPretty_Print;
  ConstantPool mCons_Pool;
  vector<InterCode> mCode;
  vector<VarId> mGlobal_Var_Id_Table;
  vector<VarId> mLocal_Var_Id_Table;
  vector<FuncInfo> mFunc_Table;

  Data() {
  } // Data()
  // * New Definition return true, Definition return false
  bool Define_Var( bool global,  VarType var_type, string name ) {
    int addr = 0;
    vector<VarId>::iterator i, end;
    if ( global ) {
      addr = mMem_.Alloc_Data( var_type.Frame_Space() );
      i = mGlobal_Var_Id_Table.begin();
      end = mGlobal_Var_Id_Table.end();
    } // if
    else {
      addr = mLocal_Var_Id_Table.size();
      i = mLocal_Var_Id_Table.begin();
      end = mLocal_Var_Id_Table.end();
    } // else

    VarId var_id( var_type, name, addr ); 
    while ( i != end ) {
      if ( i->Name() == name ) {
        *i = var_id;
        return true;
      } // if
      else if ( name < i->Name() ) {
        if ( global )
          mGlobal_Var_Id_Table.insert( i, var_id );
        else 
          mLocal_Var_Id_Table.insert( i, var_id );
        return false;
      } // else if

      i++;
    } // while
    
    if ( global )
      mGlobal_Var_Id_Table.push_back( var_id );
    else mLocal_Var_Id_Table.push_back( var_id );
    return false;
  } // Define_Var()
  // * If id defined, will return VarId.
  VarId Get_VarId( Token token ) {
    string name = token.Name();
    vector<VarId>::iterator ptr = mLocal_Var_Id_Table.begin(), end = mLocal_Var_Id_Table.end();
    do {
      while ( ptr != end ) {
        if ( ptr->Name() == name )
          return *ptr;
        else ptr++;
      } // while

      end = mGlobal_Var_Id_Table.end();
      ptr = mGlobal_Var_Id_Table.begin();
    } while ( ptr != end );

    Error_( token );
    VarId varid;
    return varid;
  } // Get_VarId()

  bool Define_Func( int type, string name, vector<VarType> type_list, vector<Token> func_code ) {
    int addr = mFunc_Code_End_;
    FuncInfo id( type, name, type_list, addr, func_code );
    int index = 0;
    do {
      if ( index == mFunc_Table.size() ) {
        mFunc_Table.push_back( id );
        return false;
      } // if
      else if ( mFunc_Table[index].Name() == name ) {
        mFunc_Table[index] = id;
        return true;
      } // else if
      else if ( mFunc_Table[index].Name() < name ) {
        mFunc_Table.insert( mFunc_Table.begin() + index, id );
        return true;
      } // else if
  
    } while ( index++ );

    return false;
  } // Define_Func()

  FuncInfo Get_Func_Info( Token token ) {
    string name = token.Name();
    vector<VarType> type_list;
    vector<Token> source_code;
    if ( name == "ListAllVariables" ) {
      FuncInfo info( VOID, name, type_list, -1, source_code );
      return info; 
    } // if
    else if ( name == "ListAllFunctions" ) {
      FuncInfo info( VOID, name, type_list, -2, source_code );
      return info;
    } // else if
    else if ( name == "ListVariable" ) {
      VarType type( STRING, true, 0 );
      type_list.push_back( type );
      FuncInfo info( VOID, name, type_list, -3, source_code );
      return info;
    } // else if
    else if ( name == "ListFunction" ) {
      VarType type( STRING, true, 0 );
      type_list.push_back( type );
      FuncInfo info( VOID, name, type_list, -4, source_code );
      return info;
    } // else if
    else if ( name == "Done" ) {
      FuncInfo info( VOID, name, type_list, -5, source_code );
      return info;
    } // else if
    else {
      for ( int i = 0; i < mFunc_Table.size() ; i++ ) 
        if ( mFunc_Table[i].Name() == name )
          return mFunc_Table[i];
    } // else
    
    Error_( token );
    return FuncInfo();
  } // Get_Func_Info()

  int Local_Table_End() {
    return mLocal_Var_Id_Table.size();
  } // Local_Table_End()

  void LocalIdDelete( int index ) {
    mLocal_Var_Id_Table.erase( mLocal_Var_Id_Table.begin() + index, mLocal_Var_Id_Table.end() );
  } // LocalIdDelete()

  void ResetLocalTable() {
    mLocal_Var_Id_Table.clear();
  } // ResetLocalTable()

  void CodeInsert( bool is_func, vector<InterCode> new_code ) {
    if ( is_func ) {
      mCode.insert( mCode.begin() + mFunc_Code_End_, new_code.begin(), new_code.end() );
      mFunc_Code_End_ += new_code.size();
    } // if
    else mCode.insert( mCode.end(), new_code.begin(), new_code.end() );
  } // CodeInsert()

  int Main_Start() {
    return mFunc_Code_End_;
  } // Main_Start()
};

Data gData;

class Scanner {
private:
  Token mNext_Token;
  vector<char> mLine_Input_;
  int mCurrent_Line_;

  void Error_( string name ) {
    Error_Info info( mCurrent_Line_, 1, name );
    throw info;
  } // Error_()

  char Peek_Char_() {
    char ch = '\0';
    if ( mLine_Input_.empty() ) {
      while ( !cin.eof() && ch != '\n' ) {
        cin.get( ch );
        mLine_Input_.push_back( ch );
      } // while

      if ( ch == '\0' ) mLine_Input_.push_back( '\0' );
    } // if

    return mLine_Input_[0];
  } // Peek_Char_()

  char Get_Char_() {
    char ch = Peek_Char_();
    mLine_Input_.erase( mLine_Input_.begin() );
    if ( ch == '\n' ) 
      mCurrent_Line_++;
    return ch;
  } // Get_Char_()

  int Input_Type_( char ch ) {
    if ( ch == '(' ) return LPAR;
    else if ( ch == ')' ) return RPAR;
    else if ( ch == '[' ) return LSQB;
    else if ( ch == ']' ) return RSQB;
    else if ( ch == '{' ) return LBRACE;
    else if ( ch == '}' ) return RBRACE;
    else if ( ch == '+' ) return PLUS;
    else if ( ch == '-' ) return MINUS;
    else if ( ch == '*' ) return STAR;
    else if ( ch == '/' ) return SLASH;
    else if ( ch == '%' ) return PERCENT;
    else if ( ch == '^' ) return CIRCUMFLEX;
    else if ( ch == '>' ) return GREATER;
    else if ( ch == '<' ) return LESS;
    else if ( ch == '&' ) return AMPER;
    else if ( ch == '|' ) return VBAR;
    else if ( ch == '=' ) return EQ;
    else if ( ch == '!' ) return NOT;
    else if ( ch == ';' ) return SEMI;
    else if ( ch == ',' ) return COMMA;
    else if ( ch == '?' ) return QUE;
    else if ( ch == ':' ) return COLON;
    else if ( ( ch >= 'a' && ch <= 'z' ) || ( ch >= 'A' && ch <= 'Z' ) ) return LETTER;
    else if ( ch >= '0' && ch <= '9' ) return DIGIT;
    else if ( ch == ' ' ) return BLANK;
    else if ( ch == '\t' ) return TAB;
    else if ( ch == '\n' ) return NEWLINE;
    else if ( ch == '\0' ) return PL_EOF;
    else if ( ch == '_' ) return UNDERLINE;
    else if ( ch == '.' ) return DOT;
    else if ( ch == '\'' ) return QUOT;
    else if ( ch == '\\' ) return BACKSLASH;
    else if ( ch == '"' ) return QUOTQUOT;
    else return OTHER;
  } // Input_Type_()

  bool Is_One_Char_Token_Type_( int type ) {
    return type == LPAR || type == RPAR || type == LSQB || 
           type == LBRACE || type == RBRACE || type == SEMI ||
           type == COMMA || type == QUE || type == COLON ||
           type == RSQB;
  } // Is_One_Char_Token_Type_()

  int Reserved_Word_( string str ) {
    if ( str == "int" ) return INT;
    else if ( str == "float" ) return FLOAT;
    else if ( str == "char" ) return CHAR;
    else if ( str == "bool" ) return BOOL;
    else if ( str == "string" ) return STRING;
    else if ( str == "void" ) return VOID;
    else if ( str == "if" ) return IF;
    else if ( str == "else" ) return ELSE;
    else if ( str == "while" ) return WHILE;
    else if ( str == "do" ) return DO;
    else if ( str == "return" ) return RETURN;
    else if ( str == "true" ) return TRUE_CONS;
    else if ( str == "false" ) return FALSE_CONS;
    else return IDENTIFER;
  } // Reserved_Word_()

public:
  Scanner() {
    mCurrent_Line_ = 1;
    Buffer_Clear();
  } // Scanner()
  
  void Buffer_Clear() {
    mLine_Input_.clear();
    mNext_Token = Token(); // Reset token
  } // Buffer_Clear()
  
  void Line_Counter_Reset() {
    mCurrent_Line_ = 1;
    mNext_Token.LineReset();
  } // Line_Counter_Reset()

  Token Peek_Token() {
    int type, state = 0, line = 1;
    bool stop = false, interupt = false;
    string token_name = "";
    if ( mNext_Token.Token_Type() == 0 ) {
      while ( !stop ) {
        if ( state == 0 ) {
          line = mCurrent_Line_;
          type = Input_Type_( Peek_Char_() );
          if ( type == DIGIT )
            state = INT_CONS;
          else if ( type == DOT )
            state = FLOAT_CONS;
          else if ( type == LETTER )
            state = IDENTIFER;
          else if ( Is_One_Char_Token_Type_( type ) ) {
            token_name = Get_Char_();
            mNext_Token = Token( type, token_name, mCurrent_Line_ );
            return mNext_Token;
          } // else if 
          else if ( type == QUOT || type == QUOTQUOT ) {
            Get_Char_();
            type == QUOT ? state = CHAR_CONS : state = STR_CONS;
          } // else if
          else if ( type == BLANK || type == TAB || type == NEWLINE )
            Get_Char_();
          else state = type;
        } // if
        else if ( !interupt ) {
          token_name += Get_Char_();
          type = Input_Type_( Peek_Char_() );
          if ( state == INT_CONS ) {
            if ( type == DOT )
              state = FLOAT_CONS;
            else if ( type != DIGIT )
              interupt = true;
          } // if
          else if ( state == FLOAT_CONS ) {
            if ( type != DIGIT )
              interupt = true;
          } // else if
          else if ( state == CHAR_CONS || state == STR_CONS ) {
            if ( ( state == CHAR_CONS && type == QUOT ) || ( state == STR_CONS && type == QUOTQUOT ) ) {
              Get_Char_(); // Pass " or '
              interupt = true;
            } // if
            else if ( type == BACKSLASH ) {
              // token_name += Get_Char_(); // * Back Slash
              token_name += Get_Char_(); // * Char follow back slash
            } // else if 
            else if ( type == NEWLINE )
              throw string( "Missing Closed" );
          } // else if
          else if ( state == IDENTIFER ) {
            if ( type != LETTER && type != DIGIT && type != UNDERLINE )
              interupt = true;
          } // else if
          else if ( state == GREATER )
            type == EQ ? state = GE : ( type == GREATER ? state = RS : interupt = true ); // >= or >> or >
          else if ( state == LESS )
            type == EQ ? state = LE : ( type == LESS ? state = LS : interupt = true ); // <= or << or <
          else if ( state == EQ )
            type == EQ ? state = EQEQ : interupt = true; // == or =
          else if ( state == NOT )
            type == EQ ? state = NEQ : interupt = true; //  != or !
          else if ( state == AMPER )
            type == AMPER ? state = AND : interupt = true; // && or &
          else if ( state == VBAR )
            type == VBAR ? state = VBAR : interupt = true; // || or |
          else if ( state == PLUS )
            type == EQ ? state = PE : ( type == PLUS ? state = PP : interupt = true );
          else if ( state == MINUS )
            type == EQ ? state = ME : ( type == MINUS ? state = MM : interupt = true );
          else if ( state == STAR )
            type == EQ ? state = TE : interupt = true; // *= or *
          else if ( state == SLASH ) {
            if ( type == SLASH ) { // Comment
              Buffer_Clear();
              return Peek_Token();
            } // if 
            else if ( type == EQ ) // /=
              state = DE;
            else interupt = true; // /
          } // else if
          else if ( state == PERCENT )
            type == EQ ? state = RE : interupt = true;
          else interupt = true;
        } // else if
        else {
          if ( state == OTHER || ( state == FLOAT_CONS && token_name == "." ) ) {
            Error_( token_name );
          } // if
          else if ( state == IDENTIFER ) 
            mNext_Token = Token( Reserved_Word_( token_name ), token_name, line );
          else mNext_Token = Token( state, token_name, line );
          
          stop = true;
        } // else 

      } // while

    } // if

    return mNext_Token;
  } // Peek_Token()

  Token Get_Token() {
    Token token = Peek_Token();
    mNext_Token = Token();
    return token;
  } // Get_Token()

  int Cur_Line() {
    return mCurrent_Line_;
  } // Cur_Line()

};

class Parser {
  // TODO Record Function code
private:
  Scanner mScn_;
  Token mToken_;
  vector<InterCode> mCodes_;
  vector<Token> mRecord_Code_;
  bool mDone_;

  bool Match_( int type ) {
    if ( mScn_.Peek_Token().Token_Type() == type ) {
      mToken_ = mScn_.Get_Token();
      mRecord_Code_.push_back( mToken_ );
      return true;
    } // if
    else return false;
  } // Match_()

  void CallFunc( Token id ) {
    InterCode code;
    FuncInfo info = gData.Get_Func_Info( id );
    if ( info.Addr() == -1 ) // ListAllVariables()
      code = InterCode( LIST_ALL_VAR, false, 0 );
    else if ( info.Addr() == -2 )
      code = InterCode( LIST_ALL_FUNC, false, 0 );
    else if ( info.Addr() == -3 )
      code = InterCode( LIST_VAR, false, 0 );
    else if ( info.Addr() == -4 )
      code = InterCode( LIST_FUNC, false, 0 );
    else if ( info.Addr() == -5 ) 
      code = InterCode( DONE, false, 0 );
    else code = InterCode( CALL, false, info.Addr() );
    
    mCodes_.push_back( code );
  } // CallFunc()

  // * user_input : ( definition | statement ) { definition | statement }
  bool User_Input_() {
    gData.mPretty_Print.Reset();
    mRecord_Code_.clear();
    if ( Definition_() ) {
    } // if
    else if ( Statement_() ) {
      gData.mPretty_Print.Stm();
      gData.CodeInsert( false, mCodes_ );
    } // else if
    else return false;

    return true;
  } // User_Input_()
  // * definition : VOID Identifier function_definition_without_ID
  // *            | type_specifier Identifier function_definition_or_declarators
  bool Definition_() {
    string id;
    int type;
    if ( Match_( VOID ) ) {
      type = mToken_.Token_Type();
      if ( Match_( IDENTIFER ) ) {
        id = mToken_.Name();
        if ( Func_Def_Without_Id_( type, id ) ) {
          
          return true;
        } // if
        else Error_();

      } // if

      Error_();
    } // if
    else if ( Type_Specifier_() ) {
      type = mToken_.Token_Type();
      if ( Match_( IDENTIFER ) ) {
        id = mToken_.Name();
        if ( Func_Def_Or_Decl_( type, id ) ) {
          return true;
        } // if

      } // if

      Error_();
    } // else if

    return false;
  } // Definition_()
  // * type_specifier : INT | CHAR | FLOAT | STRING | BOOL
  bool Type_Specifier_() {
    if ( Match_( INT ) || Match_( CHAR ) || Match_( FLOAT ) || Match_( STRING ) || Match_( BOOL ) )
      return true;
    else return false;
  } // Type_Specifier_()
  // * function_definition_or_declarators : function_definition_without_ID
  // *                                    | rest_of_declarators
  bool Func_Def_Or_Decl_( int type, string id ) {
    if ( Func_Def_Without_Id_( type, id ) ) {
      return true;
    } // if
    else if ( Rest_Of_Delc_( type, id, true ) ) {
      gData.CodeInsert( false, mCodes_ );
      return true;
    } // else if
    else return false;
  } // Func_Def_Or_Decl_()
  // * rest_of_declarators : [ '[' Constant ']' ] 
  // *                       { ',' Identifier [ '[' Constant ']' ] } ';'
  bool Rest_Of_Delc_( int type, string id, bool global ) {
    int array = 0;
    bool is_new;
    vector<string> new_id;
    vector<int> new_array;
    if ( Match_( LSQB ) ) {
      if ( Match_( CONSTANT ) ) {
        array = atoi( mToken_.Name().c_str() );
        if ( Match_( RSQB ) ) {
          new_id.push_back( id );
          new_array.push_back( array );
        } // if
        else Error_();

      } // if
      else Error_();

    } // if
    else {
      new_id.push_back( id );
      new_array.push_back( array );
    } // else

    while ( Match_( COMMA ) ) {
      if ( Match_( IDENTIFER ) ) {
        id = mToken_.Name();
        if ( Match_( LSQB ) ) {
          if ( Match_( CONSTANT ) ) {
            array = atoi( mToken_.Name().c_str() );
            if ( Match_( RSQB ) ) {
              new_id.push_back( id );
              new_array.push_back( array );
            } // if
            else Error_();

          } // if
          else Error_();

        } // if
        else {
          new_id.push_back( id );
          new_array.push_back( array );
        } // else

      } // if
      else Error_();

    } // while

    if ( Match_( SEMI ) ) {
      for ( int i = 0; i < new_id.size() ; i++ ) {
        VarType var_type( type, false, new_array[i] );
        bool is_new = gData.Define_Var( global, var_type, new_id[i] );
        if ( global ) gData.mPretty_Print.Definition( is_new, false, new_id[i] );
        
      } // for

      return true;
    } // if
    else Error_();

    return false;
  } // Rest_Of_Delc_()
  // * function_definition_without_ID : '(' [ VOID | formal_parameter_list ] ')' compound_statement
  bool Func_Def_Without_Id_( int type, string id ) {
    if ( Match_( LPAR ) ) {
      vector<VarType> type_list;
      vector<string> id_list;
      if ( Match_( VOID ) ) {
        // TODO
      } // if
      else if ( Formal_Parameter_List_( type_list, id_list ) ) {
        for ( int i = 0; i < type_list.size() ; i++ ) 
          gData.Define_Var( false, type_list[i], id_list[i] );
        
      } // else if

      if ( Match_( RPAR ) ) {
        if ( Compound_Statement_() ) {
          // TODO compute the address Func should be
          bool is_new = gData.Define_Func( type, id, type_list, mRecord_Code_ );
          gData.mPretty_Print.Definition( is_new, true, id );
          gData.ResetLocalTable();
          gData.CodeInsert( true, mCodes_ );
          return true;
        } // if

      } // if
      
      Error_();
    } // if

    return false;
  } // Func_Def_Without_Id_()
  // * formal_parameter_list : type_specifier [ '&' ] Identifier [ '[' Constant ']' ] 
  // *                         { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
  bool Formal_Parameter_List_( vector<VarType> &type_list, vector<string> &id_list ) {
    int array_num, type_specifier;
    bool second_time = false, amper = false;
    do {
      if ( Type_Specifier_() ) {
        type_specifier = mToken_.Token_Type();
        if ( Match_( AMPER ) ) 
          amper = true;
        else amper = false;
        
        if ( Match_( IDENTIFER ) ) {
          Token id = mToken_;
          if ( Match_( LSQB ) ) {
            if ( Match_( CONSTANT ) ) {
              array_num = atoi( mToken_.Name().c_str() );
              if ( !Match_( RSQB ) ) 
                Error_();
              
            } // if
            else Error_();

          } // if
          else array_num = 0;

          VarType type( type_specifier, amper, array_num );
          type_list.push_back( type );
          id_list.push_back( id.Name() );
        } // if
        else Error_();
      } // if
      else if ( second_time ) 
        Error_();
      else return false;

      second_time = true;
    } while ( Match_( COMMA ) );
    
    return true;
  } // Formal_Parameter_List_()
  // * compound_statement : '{' { declaration | statement } '}'
  bool Compound_Statement_() {
    int local_start = gData.Local_Table_End(); // * The first index of local var for this region
    if ( Match_( LBRACE ) ) {
      while ( Decl_() || Statement_() ) {
        // TODO
      } // while

      if ( Match_( RBRACE ) ) {
        gData.LocalIdDelete( local_start ); // * Restore var define in this region
        return true;
      } // if
      else Error_();

    } // if

    return false;
  } // Compound_Statement_()
  // * declaration : type_specifier Identifier rest_of_declarators
  // * local Variable define
  bool Decl_() {
    int type;
    string id;
    if ( Type_Specifier_() ) {
      type = mToken_.Token_Type();
      if ( Match_( IDENTIFER ) ) {
        id = mToken_.Name();
        if ( Rest_Of_Delc_( type, id, false ) ) {
          // TODO
          return true;
        } // if
        else Error_();

      } // if
      else Error_();

    } // if

    return false;
  } // Decl_()
  // * statement : ';'     // the null statement
  // *           | expression ';'  /* expression here should not be empty */
  // *           | RETURN [ expression ] ';'
  // *           | compound_statement
  // *           | IF '(' expression ')' statement [ ELSE statement ]
  // *           | WHILE '(' expression ')' statement
  // *           | DO statement WHILE '(' expression ')' ';'
  bool Statement_() {
    if ( Match_( SEMI ) ) {
      return true;
    } // if
    else if ( Exp_() ) {
      if ( Match_( SEMI ) ) {
        if ( mDone_ ) 
          throw true;
        return true;
      } // if
      else Error_();

    } // else if
    else if ( Match_( RETURN ) ) {
      if ( Exp_() ) {
        // TODO
      } // if

      if ( Match_( SEMI ) ) {
        return true;
      } // if
      else Error_();
      
    } // else if
    else if ( Compound_Statement_() ) {
      // TODO
      return true;
    } // else if
    else if ( Match_( IF ) ) {
      if ( Match_( LPAR ) ) {
        if ( Exp_() ) {
          if ( Match_( RPAR ) ) {
            // TODO
            if ( Statement_() ) {
              if ( Match_( ELSE ) ) {
                if ( Statement_() ) {
                  // TODO
                } // if
                else Error_();

              } // if

              return true;
            } // if
            else Error_();
          } // if
          else Error_();
        } // if
        else Error_();

      } // if
      else Error_();
    } // else if
    else if ( Match_( WHILE ) ) {
      if ( Match_( LBRACE ) ) {
        if ( Exp_() ) {
          if ( Match_( LBRACE ) ) {
            if ( Statement_() ) {
              return true;
            } // if
            else Error_();

          } // if
          else Error_();
        } // if
        else Error_();
      } // if
      else Error_();

    } // else if
    else if ( Match_( DO ) ) {
      if ( Statement_() ) {
        if ( Match_( WHILE ) ) {
          if ( Match_( LBRACE ) ) {
            if ( Exp_() ) {
              if ( Match_( RBRACE ) ) {
                if ( Match_( SEMI ) )
                  return true;
                else Error_();
              } // if 
              else Error_();
            
            } // if
            else Error_();

          } // if
          else Error_();

        } // if 
        else Error_();

      } // if 
      else Error_();

    } // else if

    return false;
  } // Statement_()
  // * expression : basic_expression { ',' basic_expression }
  bool Exp_() {
    bool second_time = false;
    do {
      if ( Basic_Exp_() ) {
        // TODO
        return true;
      } // if
      else if ( second_time ) Error_(); // basic_expr, ;
      else return false;

      second_time = true;
    } while ( Match_( COMMA ) );

    return true;
  } // Exp_()
  // * basic_expression : Identifier rest_of_Identifier_started_basic_exp
  // *                  | ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
  // *                  | sign { sign } signed_unary_exp romce_and_romloe
  // *                  | ( Constant | '(' expression ')' ) romce_and_romloe
  bool Basic_Exp_() {
    if ( Match_( IDENTIFER ) ) {
      Token id = mToken_;
      if ( Rest_Of_Id_Started_Basic_Exp_( id ) )  {
        // TODO 
        return true;
      } // if
      else Error_();

    } // if
    else if ( Match_( PP ) || Match_( MM ) ) {
      if ( Match_( IDENTIFER ) ) {
        Token id = mToken_;
        if ( Rest_Of_PPMM_Id_Started_Basic_Exp_( id ) ) {
          return true;
        } // if
        else Error_();

      } // if
      else Error_();

    } // else if
    else if ( Sign_() ) {
      while ( Sign_() ) {
        // TODO
      } // while

      if ( Signed_Unary_Exp_() ) {
        if ( Romce_And_Romloe_() ) {
          return true;
        } // if
        else Error_();
      } // if
      else Error_();

    } // else if
    else if ( Match_( CONSTANT ) || Match_( LPAR ) ) {
      if ( mToken_.Token_Type() == LPAR ) {
        if ( Exp_() ) {
          if ( !Match_( RPAR ) ) 
            Error_();

        } // if
        else Error_();

      } // if
      else {
        int addr = gData.mCons_Pool.Get_Addr( mToken_ );
        InterCode code( LOAD_CONS, false, addr );
        mCodes_.push_back( code );
      } // else
      
      if ( Romce_And_Romloe_() ) 
        return true;
      else Error_();

    } // else if
    
    return false;
  } // Basic_Exp_()
  // * rest_of_Identifier_started_basic_exp : [ '[' expression ']' ]
  // *                                        ( assignment_operator basic_expression 
  // *                                          | 
  // *                                          [ PP | MM ] romce_and_romloe 
  // *                                        )
  // *                                      | '(' [ actual_parameter_list ] ')' romce_and_romloe
  bool Rest_Of_Id_Started_Basic_Exp_( Token id ) {
    int type = UNDEFINE;
    if ( Match_( LPAR ) ) {
      if ( Actual_Parameter_List_() ) {
        // TODO
      } // if

      if ( Match_( RPAR ) ) {
        CallFunc( id );
        if ( Romce_And_Romloe_() ) {

          return true;
        } // if
        else Error_();

      } // if
      else Error_();
      
    } // if
    else {
      if ( Match_( LSQB ) ) {
        if ( Exp_() ) {
          if ( !Match_( RSQB ) ) 
            Error_();
        
        } // if
      } // if

      if ( Assignment_Op_() ) {
        if ( Basic_Exp_() )
          return true;
        else Error_();

      } // if
      else if ( Match_( PP ) || Match_( MM ) ) {
        if ( Romce_And_Romloe_() ) {
          return true;
        } // if
        else Error_();
      } // else if
      else if ( Romce_And_Romloe_() ) {
        return true;
      } // else if 
      else return false;

    } // else
    
    return false;
  } // Rest_Of_Id_Started_Basic_Exp_()
  // * rest_of_PPMM_Identifier_started_basic_exp : [ '[' expression ']' ] romce_and_romloe 
  bool Rest_Of_PPMM_Id_Started_Basic_Exp_( Token id ) {
    int type = UNDEFINE;
    if ( Match_( LSQB ) ) {
      if ( Exp_() ) {
        if ( !Match_( RSQB ) )
          Error_();
      } // if

    } // if

    if ( Romce_And_Romloe_() )
      return true;
    return false;
  } // Rest_Of_PPMM_Id_Started_Basic_Exp_()
  // * sign : '+' | '-' | '!'
  bool Sign_() {
    return Match_( PLUS ) || Match_( MINUS ) || Match_( NOT );
  } // Sign_()
  // * actual_parameter_list : basic_expression { ',' basic_expression }
  bool Actual_Parameter_List_() { 
    if ( Basic_Exp_() ) {
      while ( Match_( COMMA ) ) {
        if ( !Basic_Exp_() )
          Error_();
    
      } // while

      return true;
    } // if
    else return false;
  } // Actual_Parameter_List_()
  // * assignment_operator : '=' | TE (*=) | DE (/=) | RE (%=) | PE(+=) | ME(-=)
  bool Assignment_Op_() {
    return Match_( EQ ) || Match_( TE ) || Match_( RE ) || Match_( PE ) || Match_( ME );
  } // Assignment_Op_()
  // * rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp : rest_of_maybe_logical_OR_exp 
  // *            [ '?' basic_expression ':' basic_expression ]
  bool Romce_And_Romloe_() {
    if ( Rest_Of_Maybe_Logical_Or_Exp_() ) {
      if ( Match_( QUE ) ) {
        if ( Basic_Exp_() ) {
          if ( Match_( COLON ) ) {
            if ( !Basic_Exp_() ) 
              Error_();

          } // if
          else Error_();

        } // if
        else Error_();

      } // if 

      return true;
    } // if 
    else return false;
  } // Romce_And_Romloe_()
  // * rest_of_maybe_logical_OR_exp : rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp }
  bool Rest_Of_Maybe_Logical_Or_Exp_() {
    if ( Rest_Of_Maybe_Logical_And_Exp_() ) {
      while ( Match_( OR ) ) {
        if ( !Maybe_Logical_And_Exp_() ) 
          Error_();

      } // while

      return true;
    } // if
    else return false;
  } // Rest_Of_Maybe_Logical_Or_Exp_()
  // * maybe_logical_AND_exp : maybe_bit_OR_exp { AND maybe_bit_OR_exp }
  bool Maybe_Logical_And_Exp_() {
    if ( Maybe_Bit_Or_Exp_() ) {
      while ( Match_( AND ) ) {
        if ( !Maybe_Bit_Or_Exp_() )
          Error_();

      } // while

      return true;
    } // if
    else return false;
  } // Maybe_Logical_And_Exp_()
  // * rest_of_maybe_logical_AND_exp : rest_of_maybe_bit_OR_exp { AND maybe_bit_OR_exp }
  bool Rest_Of_Maybe_Logical_And_Exp_() {
    if ( Rest_Of_Maybe_Bit_Or_Exp_() ) {
      while ( Match_( AND ) ) {
        if ( !Maybe_Bit_Or_Exp_() )
          Error_();
      } // while

      return true;
    } // if
    else return false;
  } // Rest_Of_Maybe_Logical_And_Exp_()
  // * maybe_bit_OR_exp : maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
  bool Maybe_Bit_Or_Exp_() {
    if ( Maybe_Bit_Ex_Or_Exp_() ) {
      while ( Match_( VBAR ) ) {
        if ( !Maybe_Bit_Ex_Or_Exp_() ) 
          Error_();
      } // while

      return true;
    } // if
    else return false;
  } // Maybe_Bit_Or_Exp_()
    // * rest_of_maybe_bit_OR_exp : rest_of_maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
  bool Rest_Of_Maybe_Bit_Or_Exp_() {
    if ( Rest_Of_Maybe_Bit_Ex_Or_Exp_() ) {
      while ( Match_( VBAR ) ) {
        if ( !Maybe_Bit_Ex_Or_Exp_() )
          Error_();
      } // while

      return true;
    } // if
    else return false;
  } // Rest_Of_Maybe_Bit_Or_Exp_()
  // * maybe_bit_ex_OR_exp : maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
  bool Maybe_Bit_Ex_Or_Exp_() {
    if ( Maybe_Bit_And_Exp_() ) {
      while ( Match_( CIRCUMFLEX ) ) {
        if ( !Maybe_Bit_And_Exp_() )
          Error_();
      } // while

      return true;
    } // if
    else return false;
  } // Maybe_Bit_Ex_Or_Exp_()
  // * rest_of_maybe_bit_ex_OR_exp : rest_of_maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
  bool Rest_Of_Maybe_Bit_Ex_Or_Exp_() {
    if ( Rest_Of_Maybe_Bit_And_Exp_() ) {
      while ( Match_( CIRCUMFLEX ) ) {
        if ( !Maybe_Bit_And_Exp_() )
          Error_();
      } // while

      return true;
    } // if
    else return false;
  } // Rest_Of_Maybe_Bit_Ex_Or_Exp_()
  // * maybe_bit_AND_exp : maybe_equality_exp { '&' maybe_equality_exp }
  bool Maybe_Bit_And_Exp_() {
    if ( Maybe_Equality_Exp_() ) {
      while ( Match_( AMPER ) ) {
        if ( !Maybe_Equality_Exp_() )
          Error_();
      } // while
      
      return true;
    } // if
    else return false;
  } // Maybe_Bit_And_Exp_()
  // * rest_of_maybe_bit_AND_exp : rest_of_maybe_equality_exp { '&' maybe_equality_exp }
  bool Rest_Of_Maybe_Bit_And_Exp_() {
    if ( Rest_Of_Maybe_Equality_Exp_() ) {
      while ( Match_( AMPER ) )  {
        if ( !Maybe_Equality_Exp_() )
          Error_();
      } // while

      return true;
    } // if
    else return false;
  } // Rest_Of_Maybe_Bit_And_Exp_()
  // * maybe_equality_exp : maybe_relational_exp 
  // *                      { ( EQ | NEQ ) maybe_relational_exp}
  bool Maybe_Equality_Exp_() {
    if ( Maybe_Relational_Exp_() ) {
      while ( Match_( EQ ) || Match_( NEQ ) ) {
        if ( Maybe_Relational_Exp_() ) {
          // TODO
        } // if
        else Error_();

      } // while

      return true;
    } // if
    else return false;
  } // Maybe_Equality_Exp_()
  // * rest_of_maybe_equality_exp : rest_of_maybe_relational_exp 
  // *                              { ( EQ | NEQ ) maybe_relational_exp }
  bool Rest_Of_Maybe_Equality_Exp_() {
    if ( Rest_Of_Maybe_Relational_Exp_() ) {
      while ( Match_( EQ ) || Match_( NEQ ) ) {
        if ( Maybe_Relational_Exp_() ) {
          // TODO
        } // if
        else Error_();
      } // while

      return true;
    } // if
    else return false;
  } // Rest_Of_Maybe_Equality_Exp_()
  // * maybe_relational_exp : maybe_shift_exp 
  // *                        { ( '<' | '>' | LE | GE ) maybe_shift_exp }
  bool Maybe_Relational_Exp_() {
    if ( Maybe_Shift_Exp_() ) {
      while ( Match_( LESS ) || Match_( GREATER ) || Match_( LE ) || Match_( GE ) ) {
        if ( Maybe_Shift_Exp_() ) {
          // TODO 
        } // if
        else Error_();
      } // while

      return true;
    } // if()
    else return false;
  } // Maybe_Relational_Exp_()
  // * rest_of_maybe_relational_exp : rest_of_maybe_shift_exp 
  // *                                { ( '<' | '>' | LE | GE ) maybe_shift_exp }
  bool Rest_Of_Maybe_Relational_Exp_() {
    if ( Rest_Of_Maybe_Shift_Exp_() ) {
      while ( Match_( LESS ) || Match_( GREATER ) || Match_( LE ) || Match_( GE ) ) {
        if ( Maybe_Shift_Exp_() ) {
          // TODO 
        } // if
        else Error_();
      } // while

      return true;
    } // if
    else return false;
  } // Rest_Of_Maybe_Relational_Exp_()
  // * maybe_shift_exp : maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
  bool Maybe_Shift_Exp_() {
    if ( Maybe_Additive_Exp_() ) {
      while ( Match_( LS ) || Match_( RS ) ) {
        if ( Maybe_Additive_Exp_() ) {
          // TODO
        } // if
        else Error_();
      } // while

      return true;
    } // if
    else return false;
  } // Maybe_Shift_Exp_()
  // * rest_of_maybe_shift_exp : rest_of_maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
  bool Rest_Of_Maybe_Shift_Exp_() {
    if ( Rest_Of_Maybe_Additive_Exp_() ) {
      while ( Match_( LS ) || Match_( RS ) ) {
        if ( Maybe_Additive_Exp_() ) {
          // TODO
        } // if
        else Error_();
      } // while

      return true;
    } // if
    else return false;
  } // Rest_Of_Maybe_Shift_Exp_()
  // * maybe_additive_exp : maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
  bool Maybe_Additive_Exp_() {
    if ( Maybe_Mult_Exp_() ) {
      while ( Match_( PLUS ) || Match_( MINUS ) ) {
        if ( Maybe_Mult_Exp_() ) {
          // TODO
        } // if
        else Error_();
      }  // while

      return true;
    } // if
    else return false;
  } // Maybe_Additive_Exp_()
  // * rest_of_maybe_additive_exp : rest_of_maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
  bool Rest_Of_Maybe_Additive_Exp_() {
    if ( Rest_Of_Maybe_Mult_Exp_() ) {
      while ( Match_( PLUS ) || Match_( MINUS ) ) {
        if ( Maybe_Mult_Exp_() ) {
          // TODO
        } // if
        else Error_();
      }  // while

      return true;
    } // if
    else return false;   // TODO
  } // Rest_Of_Maybe_Additive_Exp_()
  // * maybe_mult_exp : unary_exp rest_of_maybe_mult_exp
  bool Maybe_Mult_Exp_() {
    if ( Unary_Exp_() ) {
      if ( Rest_Of_Maybe_Mult_Exp_() ) {
        // TODO
        return true;
      } // if
      else Error_();
    } // if

    return false;
  } // Maybe_Mult_Exp_()
  // * rest_of_maybe_mult_exp : { ( '*' | '/' | '%' ) unary_exp }  /* could be empty ! */
  bool Rest_Of_Maybe_Mult_Exp_() {
    while ( Match_( STAR ) || Match_( SLASH ) || Match_( PERCENT ) ) {
      if ( Unary_Exp_() ) {
        // TODO
      } // if
      else Error_();
    } // while

    return true;
  } // Rest_Of_Maybe_Mult_Exp_()
  // * unary_exp : sign { sign } signed_unary_exp
  // *           | unsigned_unary_exp
  // *           | ( PP | MM ) Identifier [ '[' expression ']' ]
  bool Unary_Exp_() {
    if ( Sign_() ) {
      while ( Sign_() ) {
        // TODO
      } // while

      if ( Signed_Unary_Exp_() ) {
        // TODO
        return true;
      } // if
      else Error_();
    } // if
    else if ( Unsigned_Unary_Exp_() ) {
      // TODO
      return true;
    } // else if
    else if ( Match_( PP ) || Match_( MM ) ) {
      if ( Match_( IDENTIFER ) ) {
        Token id = mToken_;
        gData.Get_VarId( id );
        if ( Match_( LSQB ) ) {
          if ( Exp_() ) {
            if ( !Match_( RSQB ) )
              Error_();
          } // if
          else Error_();
        } // if

        return true;
      } // if
      else Error_();
    } // else if

    return false;
  } // Unary_Exp_()
  // * signed_unary_exp : Identifier [ '(' [ actual_parameter_list ] ')' 
  // *                               |
  // *                                 '[' expression ']'
  // *                               ]
  // *                  | Constant 
  // *                  | '(' expression ')'
  bool Signed_Unary_Exp_() {
    if ( Match_( IDENTIFER ) ) {
      Token id = mToken_;
      if ( Match_( LPAR ) ) {
        gData.Get_Func_Info( id );
        if ( Actual_Parameter_List_() ) {
          // TODO
        } // if

        if ( Match_( RPAR ) )
          return true;
        else Error_();

      } // if
      else if ( Match_( LSQB ) ) {
        if ( Exp_() ) {
          if ( !Match_( RSQB ) )
            Error_();
        } // if

      } // else if

      return true;
    } // if
    else if ( Match_( CONSTANT ) ) {
      return true;
    } // else if
    else if ( Match_( LPAR ) ) {
      if ( Exp_() ) {
        if ( Match_( RPAR ) )
          return true;
        else
          Error_();
      } // if
      else Error_();
    } // else if

    return false;
  } // Signed_Unary_Exp_()
  // * unsigned_unary_exp : Identifier [ '(' [ actual_parameter_list ] ')' 
  // *                                 |
  // *                                   [ '[' expression ']' ] [ ( PP | MM ) ]
  // *                                 ]
  // *                    | Constant 
  // *                    | '(' expression ')'
  bool Unsigned_Unary_Exp_() {
    if ( Match_( IDENTIFER ) ) {
      Token id = mToken_;
      int type;
      if ( Match_( LPAR ) ) {
        
        if ( Actual_Parameter_List_() ) {
          // TODO
        } // if
        
        if ( !Match_( RPAR ) )
          Error_();

        return true;
      } // if
      else {
        if ( Match_( LSQB ) ) {
          if ( Exp_() ) {
            if ( !Match_( RSQB ) )
              Error_();
          
          } // if
        } // if

        if ( Match_( PP ) || Match_( MM ) ) {
          // TODO
        } // if

      } // else

      return true;
    } // if
    else if ( Match_( CONSTANT ) ) {
      return true;
    } // else if
    else if ( Match_( LPAR ) ) {
      if ( Exp_() ) {
        if ( Match_( RPAR ) )
          return true;
        else
          Error_();
      } // if
      else Error_();
    } // else if

    return false;
  } // Unsigned_Unary_Exp_()

  void Error_() {
    Error_Info info( mToken_.Line(), 2, mToken_.Name() );
    throw info;
  } // Error_()

public:
  Parser() {
    mDone_ = false;
    mToken_ = Token();
    mScn_ = Scanner();
  } // Parser()

  void Reset( int error_line ) {
    mToken_ = Token();
    if ( error_line == mScn_.Cur_Line() )
      mScn_ = Scanner();
    else 
      mScn_.Line_Counter_Reset();
  } // Reset()


  void Parse() {
    mScn_.Line_Counter_Reset();
    mCodes_.clear();
    if ( !User_Input_() )
      Error_();
      
  } // Parse()

};

class Stack {
private:
  vector<Variable> mStack_;
public:
  void Push( Variable var ) {
    mStack_.push_back( var );
  } // Push()

  Variable Pop() {
    Variable var;
    if ( mStack_.empty() )
      throw string( "Stack empty" );
    else 
      var = *mStack_.end();

    mStack_.pop_back();
    return var;
  } // Pop()

};

class Runner {
private:
  int mPc_;
  // * Stack of Virtual manchine, 
  Stack mStack_;

  void ListAllVar_() {
    vector<VarId> table = gData.mGlobal_Var_Id_Table;
    for ( vector<VarId>::iterator i = table.begin(); i != table.end() ; i++ ) 
      cout << i->Name() << endl;
    
    mPc_++;
  } // ListAllVar_()

  void ListAllFunc_() {
    vector<FuncInfo> table = gData.mFunc_Table;
    for ( vector<FuncInfo>::iterator i = table.begin() ; i != table.end() ; i++ )
      cout << i->Name() << "()" << endl;

    mPc_++;  
  } // ListAllFunc_()

  void ListVar_() {
    Variable var = mStack_.Pop();
    vector<VarId> table = gData.mGlobal_Var_Id_Table;
    string id = gStr_Pool.Load_Str( var.Value() );
    vector<VarId>::iterator i;
    bool done = false;
    for ( i = table.begin() ; i != table.end() && !done ; i++ ) {
      if ( i->Name() == id ) {
        int type = i->Var_Type().Type();
        if ( type == INT )
          cout << "int ";
        else if ( type == CHAR ) 
          cout << "char ";
        else if ( type == FLOAT )
          cout << "float ";
        else 
          cout << "string ";        

        cout << i->Name() << " ;" << endl;
        done = true;
      } // if

    } // for

    mPc_++;
  } // ListVar_()

  void ListFunc_() {
    Variable var = mStack_.Pop();
    vector<FuncInfo> table = gData.mFunc_Table;
    string id = gStr_Pool.Load_Str( var.Value() );
    vector<FuncInfo>::iterator i;
    bool done = false;
    for ( i = table.begin() ; i != table.end() && !done ; i++ ) {
      if ( i->Name() == id ) {
        vector<Token> code = i->Code();
        // TODO Print
        done = true;
      } // if

    } // for

    mPc_++;
  } // ListFunc_()

  void PrintSpace( int space ) {
    for ( int i = 0; i < space ; i++ )
      cout << "  ";
  } // PrintSpace()

  void PrintCode_( vector<Token> codes ) {
    vector<Token>::iterator token;
    int type = 0, next_type = 0;
    int space = 0;
    for ( token = codes.begin() ; token != codes.end() ; token++ ) {
      type = token->Token_Type();
      if ( token + 1 != codes.end() )
        next_type = ( token + 1 )->Token_Type();
      else next_type = 0;

      token->Print();
      if ( type == SEMI || type == LBRACE || type == RBRACE ) {
        cout << endl;
        if ( next_type != 0 ) {
          if ( type == LBRACE ) // { \n SPACE
            PrintSpace( ++space );
          else if ( next_type == RBRACE )
            PrintSpace( --space );
          else PrintSpace( space );

        } // if

      } // if
      // id( id[ id++ id-- ++id --id
      else if ( ! ( type == IDENTIFER && 
                    ( next_type == LPAR || next_type == RSQB || 
                      next_type == PP || next_type == MM ) ) || 
                ! ( ( type == PP  || type == MM ) && next_type == IDENTIFER ) ) 
        cout << " ";
      
    } // for

    mPc_++;
  } // PrintCode_()

  void Load_Cons_( int arg ) {
    mStack_.Push( gData.mCons_Pool.Get_Var( arg ) );
    mPc_++;
  } // Load_Cons_()

public:
  Runner() {
    mPc_ = 0;
  } // Runner()

  void Run() {
    int i;
    vector<InterCode> codes = gData.mCode;
    InterCode cur_code;
    for ( i = gData.Main_Start() + mPc_ ; i < codes.size() ; i = mPc_ ) {
      cur_code = codes[i];
      if ( cur_code.Instr() == DONE )
        throw true;
      else if ( cur_code.Instr() == LIST_ALL_VAR ) 
        ListAllVar_();
      else if ( cur_code.Instr() == LIST_ALL_FUNC )
        ListAllFunc_();
      else if ( cur_code.Instr() == LIST_VAR )
        ListVar_();
      else if ( cur_code.Instr() == LIST_FUNC )
        ListFunc_();
      else if ( cur_code.Instr() == LOAD_CONS )
        Load_Cons_( cur_code.Parm() );
      
    } // for
  } // Run()

};

int main() {
  char ch[10];
  cin.getline( ch, 10 );
  Parser parser;
  Runner runner;
  bool done = false;
  cout << "Our-C running ..." << endl;
  do {
    cout << "> ";
    try {
      parser.Parse();
      runner.Run();
      gData.mPretty_Print.Print();
      
    } // try
    catch( Error_Info error_info ) {
      error_info.PrintMsg();
      parser.Reset( error_info.Error_Line() );
    } // catch
    catch( bool stop ) { // * Catch Done();
      done = true;
    } // catch
    catch( string msg ) { // * For debug
      cout << msg << endl;
    } // catch
  } while ( !done ); // TODO solve the problem of EOF
  cout << "Our-C exited ..." << endl;
} // main()
