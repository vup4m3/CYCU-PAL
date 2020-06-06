// TODO EOF process 
# include <string>
# include <vector>
# include <iostream>
# include <iomanip>
# include <stdlib.h>

using namespace std;

enum Type {
  LETTER = 224, DIGIT, BLANK,
  TAB, NEWLINE, NONE,
  OTHER, PL_EOF, UNDERLINE,
  DOT, QUOT, QUOTQUOT,
  // Reserved word
  INT, FLOAT,  CHAR, 
  BOOL, STRING, VOID,
  IF, ELSE, WHILE,
  DO, RETURN,
  // Identifier Type
  IDENTIFER,
  FUNCTION, UNDEFINE, CONSTANT,
  // Constant Type
  INT_CONS, FLOAT_CONS, STR_CONS,
  CHAR_CONS,
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
  COMMA,       // ,
  QUE,         // ?
  COLON,       // :
  BACKSLASH    
};
// * Variable contain Type, Value(int, float, char), String(For string)
class Variable {
private:
  int mType_;
  float mValue_;
  string mStr_;
public:
  Variable() {
    mType_ = 0;
    mValue_ = 0;
    mStr_ = "";
  } // Variable()

  Variable( int type, float value, string str ) {
    mType_ = type;
    mValue_ = value;
    mStr_ = str;
  } // Variable()

  int Type() {
    return mType_;
  } // Type()

  float Value() {
    return mValue_;
  } // Value()

  string Str() {
    return mStr_;
  } // Str()

};
// * Pool of Constant
class ConstantPool {
private:
  vector<Variable> mPool_;
public:
  int Get_Addr( Token token ) {
    int value = 0, type = token.Cons_Type();
    string str = token.Name();
    if ( type == INT || type == FLOAT )
      value = atof( str.c_str() );
    else if ( type == CHAR )
      value = str[0];
    else value = 0;

    if ( type != STRING )
      str = "";
    int i = 0;
    while ( i < mPool_.size() ) {
      if ( mPool_[i].Type() == type ) {
        if ( type == STRING && mPool_[i].Str() == str )
          return i;
        else if ( ( type == INT || type == FLOAT || type == CHAR ) && mPool_[i].Value() == value )
          return i;
        
      } // if
      
      i++;
    } // while()

    Variable var = Variable( type, value , str );
    mPool_.push_back( var );
  } // Get_Addr()

  Variable Load( int addr ) {
    return mPool_[addr];
  } // Load()
};

class Identifer {
private:
  string mName_;
  int mAddr_;
public:
  Identifer( string name, int addr ) {
    mName_ = name;
    mAddr_ = addr;
  } // Identifer()

  string Name() {
    return mName_;
  } // Name()

  int Addr() {
    return mAddr_;
  } // Addr()
};
// * Table of Identifer
class IdTable {
private:
  vector<Identifer> mTable_;
  void Error_( Token token ) {
    string msg = token.Line() + "undefined identifer: '";
    msg += token.Name();
    msg += "'";
    throw msg;
  } // Error_()
public:
  // * Load address of Identifer by token, if token undefine throw error.
  int Load_Addr( Token token ) {
    string str = token.Name();
    for ( int i = 0; i < mTable_.size() && str < mTable_[i].Name(); i++ ) {
      if ( mTable_[i].Name() == str )
        return mTable_[i].Addr();
    } // for

    Error_( token ); // Can't find this token in Table
  } // Load_Addr()
  // * Define new Identifer in Id-Table
  void Define( Identifer id, int i ) {
    if ( mTable_.size() < i ) {
      mTable_.push_back( id ); 
    }
    else if ( mTable_[i].Name() < id.Name() ) 
      mTable_.insert( mTable_.begin() + i , id );
    else 
      Define( id, ++i );
  } // Define()
};

class Data {
private:
  vector<Variable> mData_;
public:
  Data() {
    mData_.clear();
  } // Data()
  // * Make a Data frame for function
  Data( vector<Variable> arg,  int num_of_var ) {
    for ( int i = 0; i < num_of_var; i++) {
      if ( arg.size() > i )
        mData_.push_back( arg[i] );
      else {
        Variable var;
        mData_.push_back( var );
      } // else

    } // for

  } // Data()
  // * Reset Data frame
  void Reset() {
    mData_.clear();
  } // Reset()
  // * Load Variable from Data by address
  Variable Load( int addr ) {
    return mData_[addr];
  } // Load()
  // * New Variable in Data
  int New() {
    Variable var;
    mData_.push_back( var );
    return mData_.size();
  } // New()
  // * Store Variable in Data by address
  void Store( Variable var, int addr ) {
    mData_[addr] = var;
  } // Store()
};

class Token {
private:
  int mToken_Type_;
  int mConstant_Type_;
  string mName_;
  int mLine_;

public:
  Token( int type, string name, int line ) {
    mName_ = name;
    mConstant_Type_ = 0;
    mLine_ = line;
    if ( type == INT_CONS || type == STR_CONS || type == CHAR_CONS || type == FLOAT_CONS ) {
      if ( type == INT_CONS )
        mConstant_Type_ = INT;
      else if ( type == FLOAT_CONS )
        mConstant_Type_ = FLOAT;
      else if ( type == CHAR_CONS )
        mConstant_Type_ = CHAR;
      else mConstant_Type_ = STR_CONS;

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

  string Line() {
    string str = "Line ";
    str += to_string( mLine_ );
    str += " :";
    return str;
  } // Line()

};

class Scanner {
private:
  Token *mNext_Token;
  vector<char> mLine_Input_;
  int mCurrent_Line_;

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
    else if (ch == '+' ) return PLUS;
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
    else if ( ch == '\'') return QUOT;
    else if ( ch == '\\' ) return BACKSLASH;
    else if ( ch == '"' ) return QUOTQUOT;
    else return OTHER;
  } // Input_Type_()

  bool Is_One_Char_Token_Type_( int type ) {
    return type == LPAR || type == RPAR || type == LSQB || 
           type == LBRACE || type == RBRACE || type == SEMI ||
           type == COMMA || type == QUE || type == COLON;
  } // Is_One_Char_Token_Type_()

  string Back_Slash_() {
    char ch = Get_Char_(), next_ch = Get_Char_();
    if ( next_ch == 'n' )
      return "\n";
    else if ( next_ch == 't' )
      return "\t";
    else if ( next_ch == '\'' ) 
      return "\'";
    else if ( next_ch == '\"' )
      return "\"";
    else return "";
  } // Back_Slash_()

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
    else return IDENTIFER;
  } // Reserved_Word_()

public:
  Scanner() {
    Reset();
  } // Scanner()
  // * Reset for error happened
  void Reset() {
    if ( mNext_Token != NULL )
      delete mNext_Token;
    mNext_Token = NULL;
    mLine_Input_.clear();
    mCurrent_Line_ = 0;
  } // Reset()
  // * Set Current Line to 0 for new instruction
  void Zero() {
    mCurrent_Line_ = 0;
  } // Zero()

  Token Peek_Token() {
    int type, state = 0;
    bool stop = false, interupt = false;
    string token_name = "";
    if ( mNext_Token == NULL ) {
      while ( !stop ) {
        if ( state == 0 ) {
          type = Input_Type_( Peek_Char_() );
          if ( type == DIGIT )
            state = INT_CONS;
          else if ( type == DOT )
            state = FLOAT_CONS;
          else if ( type == LETTER )
            state = IDENTIFER;
          else if ( Is_One_Char_Token_Type_( type ) ) {
            token_name = Get_Char_();
            *mNext_Token = Token( type, token_name, mCurrent_Line_ );
            return *mNext_Token;
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
          if ( state != CHAR_CONS && state != STR_CONS )
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
            else if ( type == BACKSLASH ) 
              token_name += Back_Slash_();
            else if ( type == NEWLINE )
              throw "Missing Closed '";
            else token_name += Get_Char_();
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
          else if ( state = PLUS )
            type == EQ ? state = PE : ( type == PLUS ? state = PP : interupt = true );
          else if ( state = MINUS )
            type == EQ ? state = ME : ( type == MINUS ? state = MM : interupt = true );
          else if ( state = STAR )
            type == EQ ? state = TE : interupt = true; // *= or *
          else if ( state = SLASH ) {
            if ( type == SLASH ) { // Comment
              Reset();
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
            string error_msg = "Unrecognized token with first char : '";
            error_msg += token_name;
            error_msg += "'";
            Reset();
            throw error_msg;
          } // if
          else if ( state == IDENTIFER ) {
            *mNext_Token = Token( Reserved_Word_( token_name ), token_name, mCurrent_Line_ );
          } // else if
          else 
            *mNext_Token = Token( state, token_name, mCurrent_Line_ );
          stop = true;
        } // else 

      } // while

    } // if

    return *mNext_Token;
  } // Peek_Token()

  Token Get_Token() {
    Token token = Peek_Token();
    delete mNext_Token;
    mNext_Token = NULL;
    return token;
  } // Get_Token()

  string Line_String() {
    string str = "Line ";
    str += to_string( mCurrent_Line_ );
    str += " :";
    return str;
  } // Current_Line()

};

struct InterCode {
  int type;
  int operater;
  float parameter;
};

class PrettyPrint {
private:
  int mType;
  string mName;
public:
  PrettyPrint() {
    mType = 0;
    mName = "";
  } // PrettyPrint()
  // * type == 0, Definition
  // * type == 1, Statement executed
  PrettyPrint( int type, string name ) {
    mType = type;
    mName = name;
  } // PrettyPrint()
  
  void Print() {
    if ( mType == 0 ) {
      cout << "Definition of ";
      cout << mName;
      cout << " entered ..." << endl;
    } // if
    else if ( mType == 1 ) {
      cout << "Statement executed ...";
    } // else if 
    else {
      cout << "No other case !!" << endl;
    } // else 
    
  } // Print
};

class Parser {
  // TODO Record Function code
private:
  Scanner mScn_;
  Token *mToken_;
  PrettyPrint mPretty_Print_;

  bool Match_( int type ) {
    if ( mScn_.Peek_Token().Token_Type() == type ) {
      *mToken_ = mScn_.Get_Token();
      return true;
    }
    else return false;
  } // Match_()
  // * user_input : ( definition | statement ) { definition | statement }
  bool User_Input_() {
    if ( Definition_() ) {
      // TODO
    } // if
    else if ( Statement_() ) {
      // TODO 
      mPretty_Print_ = PrettyPrint( 1, "" );
    } // else if
    else return false;

    while ( Definition_() || Statement_() ) {
      // TODO
    } // while()

    return true;
  } // User_Input_()
  // * definition : VOID Identifier function_definition_without_ID
  // *            | type_specifier Identifier function_definition_or_declarators
  bool Definition_() {
    if ( Match_( VOID ) ) {
      // TODO
      if ( Func_Def_Without_Id_() ) {
        // TODO
        return true;
      } // if
    } // if
    else if ( Type_Specifier_() ) {
      if ( Func_Def_Or_Decl_() ) {
        // TODO
        return true;
      } // if
    } // else
    else return false;
  } // Definition_()
  // * type_specifier : INT | CHAR | FLOAT | STRING | BOOL
  bool Type_Specifier_() {
    if ( Match_( INT ) || Match_( CHAR ) || Match_( FLOAT ) || Match_( STRING ) || Match_( BOOL ) )
      return true;
    else return false;
  } // Type_Specifier_()
  // * function_definition_or_declarators : function_definition_without_ID
  // *                                    | rest_of_declarators
  bool Func_Def_Or_Decl_() {
    if ( Func_Def_Without_Id_() ) {
      // TODO
      return true;
    } // if
    else if ( Rest_Of_Delc_() ) {
      // TODO
      return true;
    } // else if
    else return false;
  } // Func_Def_Or_Decl_()
  // * rest_of_declarators : [ '[' Constant ']' ] 
  // *                       { ',' Identifier [ '[' Constant ']' ] } ';'
  bool Rest_Of_Delc_() {
    if ( Match_( LSQB ) ) {
      if ( Match_( CONSTANT ) ) {
        if ( Match_( RSQB ) ) {
          // TODO
        } // if
        else Error_();

      } // if
      else Error_();

    } // if

  } // Rest_Of_Delc_()
  // * function_definition_without_ID : '(' [ VOID | formal_parameter_list ] ')' compound_statement
  bool Func_Def_Without_Id_() {
    if ( Match_( LPAR ) ) {
      if ( Match_( VOID ) ) {
        // TODO
      } // if
      else if ( Formal_Parameter_List_() ) {

      } // else if

      if ( Match_( RPAR ) ) {
        // TODO
      } // if
      else {
        Error_();
      } // else 
    } // if
    if ( Compound_Statement_() ) {
      // TODO
      return true;
    } // if
    else
      return false;
  } // Func_Def_Without_Id_()
  // * formal_parameter_list : type_specifier [ '&' ] Identifier [ '[' Constant ']' ] 
  // *                         { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
  bool Formal_Parameter_List_() {
    if ( Type_Specifier_() ) {
      if ( Match_( AMPER ) ) {
        // TODO
      } // if
      
      if ( Match_( IDENTIFER ) ) {
        if ( Match_( LSQB ) ) {
          if ( Match_( CONSTANT ) ) {
            if ( Match_( RSQB ) ) {
            // TODO
            } // if
            else Error_();

          } // if
          else Error_();

        } // if

      } // if

      while ( Match_( COMMA ) ) {
        if ( Type_Specifier_() ) {
          if ( Match_( AMPER ) ) {
            // TODO
          } // if

          if ( Match_( IDENTIFER ) ) {
            if ( Match_( LSQB ) ) {
              if ( Match_( CONSTANT ) ) {
                if ( Match_( RSQB ) ) {
                // TODO
                } // if
                else Error_();

              } // if
              else Error_();

            } // if

          } // if

        }// if

      } // while

      return true;
    } // if
    else return false;
  } // Formal_Parameter_List_()
  // * compound_statement : '{' { declaration | statement } '}'
  bool Compound_Statement_() {
    if ( Match_( LBRACE ) ) {
      while ( Decl_() || Statement_() ) {
        // TODO
      } // while

      if ( Match_( RBRACE ) ) {
        // TODO
      } // if
      else Error_();

    } // if
  } // Compound_Statement_()
  // * declaration : type_specifier Identifier rest_of_declarators
  bool Decl_() {
    if ( Type_Specifier_() ) {
      if ( Match_( IDENTIFER ) ) {
        if ( Rest_Of_Delc_() ) {
          // TODO
        } // if
        else Error_();

      } // if
      else Error_();

    } // if
    else Error_();
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
      // TODO
      return true;
    } // if
    else if ( Exp_() ) {
      if ( Match_( SEMI ) ) {
        // TODO
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
                } /// if
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
      if( Match_( LBRACE ) ) {
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
    else return false;

  } // Statement_()
  // * expression : basic_expression { ',' basic_expression }
  bool Exp_() {
    if ( Basic_Exp_() ) {
      while ( Match_( COMMA ) ) {
        if ( Basic_Exp_() )
          ; // TODO
        else Error_();

      } // while

      return true;
    } // if
    else return false;
  } // Exp_()
  // * basic_expression : Identifier rest_of_Identifier_started_basic_exp
  // *                  | ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
  // *                  | sign { sign } signed_unary_exp romce_and_romloe
  // *                  | ( Constant | '(' expression ')' ) romce_and_romloe
  bool Basic_Exp_() {
    if ( Match_( IDENTIFER ) ) {
      if ( Rest_Of_Id_Stated_Basic_Exp_() )  {
        // TODO 
        return true;
      } // if
      else Error_();

    } // if
    else if ( Match_( PP ) || Match_( MM ) ) {
      if ( Match_( IDENTIFER ) ) {
        if ( Rest_Of_PPMM_Id_Started_Basic_Exp_() ) {
          return true;
        } // if
        else Error_();

      } // if
      else Error_();

    } // else if
    else if ( Sign_() ) {
      while( Sign_() ) {
        // TODO
      } // while
      if ( Signed_Unary_Exp_() ) {
        if ( Romce_And_Romloe_() ) {
          return true;
        } // if
        else Error_();
      } // if
      else Error_();

    } // else 
    else if ( Match_( CONSTANT ) || Match_( LPAR ) ) {
      if( mToken_->Token_Type() == LPAR ) {
        if ( Exp_() ) {
          if ( !Match_( RPAR ) ) 
            Error_();

        } // if
        else Error_();

      } // if
      
      if ( Romce_And_Romloe_() ) 
        return true;
      else Error_();

    } // else if
    else return false;

  } // Basic_Exp_()
  // * rest_of_Identifier_started_basic_exp : [ '[' expression ']' ]
  // *                                        ( assignment_operator basic_expression 
  // *                                          | 
  // *                                          [ PP | MM ] romce_and_romloe 
  // *                                        )
  // *                                      | '(' [ actual_parameter_list ] ')' romce_and_romloe
  bool Rest_Of_Id_Stated_Basic_Exp_() {
    if ( Match_( LPAR ) ) {
      if( Actual_Parameter_List_() ) {
        // TODO
      } // if
      if ( Match_( RPAR ) ) {
        if ( Romce_And_Romloe_() ) {
          return true;
        } // if
        else Error_();

      } // if
      else Error_();
      
    } // if
    else {
      bool front = false;
      if ( Match_( LSQB ) ) {
        if ( Exp_() ) {
          if ( !Match_( RSQB ) ) 
            Error_();
          else front = true;
        
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
      else if ( front == true )
        Error_();
      else return false;

    } // else
    
  } // Rest_Of_Id_Stated_Basic_Exp_()
  // * rest_of_PPMM_Identifier_started_basic_exp : [ '[' expression ']' ] romce_and_romloe 
  bool Rest_Of_PPMM_Id_Started_Basic_Exp_() {
    if ( Match_( LSQB ) ) {
      if ( Exp_() ) {
        if ( !Match_( RSQB ) )
          Error_();
      } // if

    } // if

    if ( Romce_And_Romloe_() )
      return true;
    else return false;

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
    } //if
    else return false;
  } // Actual_Parameter_List_()
  // * assignment_operator : '=' | TE | DE | RE | PE | ME
  bool Assignment_Op_() {
    return Match_( EQ ) || Match_( TE ) || Match_( RE ) || Match_( PE ) || Match_( ME );
  } // Assignment_Op_()
  // * rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp : rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ]
  bool Romce_And_Romloe_() {
    if ( Rest_Of_Maybe_Logical_Or_Exp_() ) {
      if ( Match_( QUE ) ) {
        if ( Basic_Exp_() ) {
          if ( Match_( COLON ) ) {
            if (!Basic_Exp_() ) 
              Error_();

          } // if
          else Error_();

        } // if
        else Error_();

      } //if 

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
        if ( !Maybe_Bit_Or_Exp_ )
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
        if ( !Maybe_Bit_Ex_Or_Exp_ ) 
          Error_();
      } // while

      return true;
    } // if
    else return false;
  } // Maybe_bit_Or_Exp_()
  // * rest_of_maybe_bit_OR_exp : rest_of_maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
  bool Rest_Of_Maybe_Bit_Or_Exp_() {
    if ( Rest_Of_Maybe_Bit_Ex_Or_Exp_() ) {
      while( Match_( VBAR ) ) {
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
    } // if
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
    } //if
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
    } //if
    else return false;
  } // Rest_Of_Maybe_Shift_Exp_()
// * maybe_additive_exp : maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
  bool Maybe_Additive_Exp_() {
    if ( Maybe_Mult_Exp_() ) {
      while ( Match_( PLUS) || Match_( MINUS ) ) {
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
      while ( Match_( PLUS) || Match_( MINUS ) ) {
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
      } // if
      else Error_();
    } // if
    else return false;
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
    else return false;
  } // Unary_Exp_()
// * signed_unary_exp : Identifier [ '(' [ actual_parameter_list ] ')' 
// *                               |
// *                                 '[' expression ']'
// *                               ]
// *                  | Constant 
// *                  | '(' expression ')'
  bool Signed_Unary_Exp_() {
    if ( Match_( IDENTIFER ) ) {
      if ( Match_( LPAR ) ) {
        if ( Actual_Parameter_List_() ) {
          // TODO
        } // if

        if ( !Match_( RPAR ) )
          Error_();
      } // if
      else if ( Match_( LSQB ) ) {
        if ( Exp_() ) {
          if ( !Match_( RSQB ) )
            Error_();
        } // if
      }

      return true;
    } // i
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
    else return false;
  } // Signed_Unary_Exp_()
// * unsigned_unary_exp : Identifier [ '(' [ actual_parameter_list ] ')' 
// *                                 |
// *                                   [ '[' expression ']' ] [ ( PP | MM ) ]
// *                                 ]
// *                    | Constant 
// *                    | '(' expression ')'
  bool Unsigned_Unary_Exp_() {
    if ( Match_( IDENTIFER ) ) {
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
            if ( !Match_(RSQB) )
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
    else return false;
  } // UNsigned_Unary_Exp_()

  void Error_() {
    string msg = mScn_.Line_String();
    msg += "unexpected token : '";
    msg += mScn_.Get_Token().Name();
    msg += "'";
    throw msg;
  } // Error()
public:
  Parser() {
    Reset();
  } // Parser()

  void Reset() {
    if ( mToken_ != NULL )
      delete mToken_;
    mToken_ = NULL;
    mScn_.Reset();
  } // Reset()


  vector<InterCode> Parse() {
    // TODO
  } // Parse()
};

class Runner {
private:
  int mPc,mAx;
public:
  bool Eval( vector<InterCode> new_codes ) {
    // TODO
  } // Eval()
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
      done = runner.Eval( parser.Parse() );
    } // try
    catch( string error_info ) {
      cout << error_info << endl;
    } // catch
  } while ( !done ); // TODO solve the problem of EOF
  cout << "Our-C exited ..." << endl;
} // main()