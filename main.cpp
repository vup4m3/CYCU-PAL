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

class Variable {
private:
  int mType_;
  float mValue_;
  string mStr_;

public:
  Variable() {
    mType_ = UNDEFINE;
    mValue_ = 0;
    mStr_ = "";
  } // Variable()

  Variable( int type, float value, string str ) {
    mType_ = type;
    mStr_ = str;
    if ( type == INT )
      mValue_ = int( value );
    else mValue_ = value;
  } // Constant()

  int Type() {
    return mType_;
  } // Tpye()

  float Value() {
    return mValue_;
  } // Value()

  string Str() {
    return mStr_;
  } // Str()

};

class Identifer {
private:
  string mName_;
  int mAddr_;
  int mType_;
  Identifer *mNext_Id_;
};

class Token {
private:
  int mToken_Type_;
  string mName_;
  Variable mVar_;

public:
  Token( int type, string name ) {
    float var_value;
    int var_type = FLOAT;
    mToken_Type_ = CONSTANT;
    mName_ = name;
    if ( type == INT_CONS || type == FLOAT_CONS ) {
      var_value = atof( name.c_str() );
      if ( type == INT_CONS ) {
        var_value = int( var_value );
        var_type = INT;
      } // if
      mVar_ = Variable( var_type, var_value, "" );
    } // if
    else if ( type == STR_CONS || type == CHAR_CONS ) {
      type == STR_CONS ? var_type = STRING : var_type = CHAR;
      mVar_ = Variable( var_type, 0, name );
    }
    else {
      mToken_Type_ = type;
      mVar_ = Variable();
    } // else 
  } // Token()

  int Type() {
    return mToken_Type_;
  } // Type()

  string Name() {
    return mName_;
  } // Name()

  Variable Var() {
    return mVar_;
  } // Var()

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

public:
  Scanner() {
    Reset();
  } // Scanner()

  void Reset() {
    if ( mNext_Token != NULL )
      delete mNext_Token;
    mNext_Token = NULL;
    mLine_Input_.clear();
  } // Reset()

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
            *mNext_Token = Token( type, token_name );
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
          else 
            *mNext_Token = Token( state, token_name );
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
  int Operater;
  int Parameter;
};

class Parser {
private:
  Scanner mScn_;
  Token mToken_;
  bool Match_( int type ) {
    if ( mScn_.Peek_Token().Type() == type )
      return true;
    else {
      mToken_ = mScn_.Get_Token();
      return true;
    } // else
  } // Match_()
  // * user_input : ( definition | statement ) { definition | statement }
  bool User_Input_() {
    if ( Definition_() ) {
      // TODO
    } // if
    else if ( Statement_() ) {
      // TODO
    } // else if
    else return false;

    while ( Definition_() || Statement_() ) {
      // TODO
    } // while()
  } // User_Input_()
  // * definition : VOID Identifier function_definition_without_ID
  // *            | type_specifier Identifier function_definition_or_declarators
  bool Definition_() {
    if ( Match_( VOID ) ) {
      if ( Match_( IDENTIFER ) )
    }
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
    // TODO
  } // Func_Def_Or_Decl_()
  // * rest_of_declarators : [ '[' Constant ']' ] 
  // *                       { ',' Identifier [ '[' Constant ']' ] } ';'
  bool Rest_Of_Delc_() {
    // TODO
  } // Rest_Of_Delc_()
  // * function_definition_without_ID : '(' [ VOID | formal_parameter_list ] ')' compound_statement
  bool Func_Def_Without_Id_() {
    // TODO
  } // Func_Def_Without_Id_()
  // * formal_parameter_list : type_specifier [ '&' ] Identifier [ '[' Constant ']' ] 
  // *                         { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
  bool Formal_Parameter_List_() {
    // TODO
  } // Formal_Parameter_List_()
  // * compound_statement : '{' { declaration | statement } '}'
  bool Compound_Statement_() {
    // TODO
  } // Compound_Statement_()
  // * declaration : type_specifier Identifier rest_of_declarators
  bool Decl_() {
    // TODO
  } // Decl_()
  // * statement : ';'     // the null statement
  // *           | expression ';'  /* expression here should not be empty */
  // *           | RETURN [ expression ] ';'
  // *           | compound_statement
  // *           | IF '(' expression ')' statement [ ELSE statement ]
  // *           | WHILE '(' expression ')' statement
  // *           | DO statement WHILE '(' expression ')' ';'
  bool Statement_() {
    // TODO
  } // Statement_()
  // * expression : basic_expression { ',' basic_expression }
  bool Exp_() {
    // TODO
  } // Exp_()
  // * basic_expression : Identifier rest_of_Identifier_started_basic_exp
  // *                  | ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
  // *                  | sign { sign } signed_unary_exp romce_and_romloe
  // *                  | ( Constant | '(' expression ')' ) romce_and_romloe
  bool Basic_Exp_() {
    // TODO
  } // Basic_Exp_()
  // * rest_of_Identifier_started_basic_exp : [ '[' expression ']' ]
  // *                                        ( assignment_operator basic_expression 
  // *                                          | 
  // *                                          [ PP | MM ] romce_and_romloe 
  // *                                        )
  // *                                      | '(' [ actual_parameter_list ] ')' romce_and_romloe
  bool Rest_Of_Id_Stated_Basic_Exp_() {
    // TODO
  } // Rest_Of_Id_Stated_Basic_Exp_()
  // * rest_of_PPMM_Identifier_started_basic_exp : [ '[' expression ']' ] romce_and_romloe 
  bool Rest_Of_PPMM_Id_Started_Basic_Exp_() {
    // TODO
  } // Rest_Of_PPMM_Id_Started_Basic_Exp_()
  // * sign : '+' | '-' | '!'
  bool Sign_() {
    // TODO
  } // Sign_()
  // * actual_parameter_list : basic_expression { ',' basic_expression }
  bool Actual_Parameter_List_() {
    // TODO
  } // Actual_Parameter_List_()
  // * assignment_operator : '=' | TE | DE | RE | PE | ME
  bool Assignment_Op_() {
    // TODO
  } // * Assignment_Op_()
  // * rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp : rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ]
  bool Romce_And_Romloe_() {
    // TODO
  } // Romce_And_Romloe_()
  // * rest_of_maybe_logical_OR_exp : rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp }
  bool Rest_Of_Maybe_Logical_Or_Exp_() {
    // TODO
  } // Rest_Of_Maybe_Logical_Or_Exp_()
  // * maybe_logical_AND_exp : maybe_bit_OR_exp { AND maybe_bit_OR_exp }
  bool Maybe_Logical_And_Exp_() {
    // TODO
  } // Maybe_Logical_And_Exp_()
  // * rest_of_maybe_logical_AND_exp : rest_of_maybe_bit_OR_exp { AND maybe_bit_OR_exp }
  bool Rest_Of_Maybe_Logical_And_Exp_() {
    // TODO
  } // Rest_Of_Maybe_Logical_And_Exp_()
  // * maybe_bit_OR_exp : maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
  bool Maybe_bit_Or_Exp_() {
    // TODO
  } // Maybe_bit_Or_Exp_()
  // * rest_of_maybe_bit_OR_exp : rest_of_maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
  bool Rest_Of_Maybe_Bit_Or_Exp_() {
    // TODO
  } // Rest_Of_Maybe_Bit_Or_Exp_()
  // * maybe_bit_ex_OR_exp : maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
  bool Maybe_Bit_Ex_Or_Exp_() {
    // TODO
  } // Maybe_Bit_Ex_Or_Exp_()
  // * rest_of_maybe_bit_ex_OR_exp : rest_of_maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
  bool Rest_Of_Maybe_Bit_Ex_Or_Exp_() {
    // TODO
  } // Rest_Of_Maybe_Bit_Ex_Or_Exp_()
  // * maybe_bit_AND_exp : maybe_equality_exp { '&' maybe_equality_exp }
  bool Maybe_Bit_And_Exp_() {
    // TODO
  } // Maybe_Bit_And_Exp_()
  // * rest_of_maybe_bit_AND_exp : rest_of_maybe_equality_exp { '&' maybe_equality_exp }
  bool Rest_Of_Maybe_Bit_And_Exp_() {
    // TODO
  } // Rest_Of_Maybe_Bit_And_Exp_()
  // * maybe_equality_exp : maybe_relational_exp 
  // *                      { ( EQ | NEQ ) maybe_relational_exp}
  bool Maybe_Equality_Exp_() {
    // TODO
  } // Maybe_Equality_Exp_()
  // * rest_of_maybe_equality_exp : rest_of_maybe_relational_exp 
  // *                              { ( EQ | NEQ ) maybe_relational_exp }
  bool Rest_Of_Maybe_Equality_Exp_() {
    // TODO
  } // Rest_Of_Maybe_Equality_Exp_()
  // * maybe_relational_exp : maybe_shift_exp 
  // *                        { ( '<' | '>' | LE | GE ) maybe_shift_exp }
  bool Maybe_Relational_Exp_() {
    // TODO
  } // Maybe_Relational_Exp_()
  // * rest_of_maybe_relational_exp : rest_of_maybe_shift_exp 
  // *                                { ( '<' | '>' | LE | GE ) maybe_shift_exp }
  bool Rest_Of_Maybe_Relational_Exp_() {
    // TODO
  } // Rest_Of_Maybe_Relational_Exp_()
// * maybe_shift_exp : maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
  bool Maybe_Shift_Exp_() {
    // TODO
  } // Maybe_Shift_Exp_()
// * rest_of_maybe_shift_exp : rest_of_maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
  bool Rest_Of_Maybe_Shift_Exp_() {
    // TODO
  } // Rest_Of_Maybe_Shift_Exp_()
// * maybe_additive_exp : maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
  bool Maybe_Additive_Exp_() {
    // TODO
  } // Maybe_Additive_Exp_()
// * rest_of_maybe_additive_exp : rest_of_maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
  bool Rest_Of_Maybe_Additive_Exp_() {
    // TODO
  } // Rest_Of_Maybe_Additive_Exp_()
// * maybe_mult_exp : unary_exp rest_of_maybe_mult_exp
  bool Maybe_Mult_Exp_() {
    // TODO
  } // Maybe_Mult_Exp_()
// * rest_of_maybe_mult_exp : { ( '*' | '/' | '%' ) unary_exp }  /* could be empty ! */
  bool Rest_Of_Maybe_Mult_Exp_() {
    // TODO
  } // Rest_Of_Maybe_Mult_Exp_()
// * unary_exp : sign { sign } signed_unary_exp
// *           | unsigned_unary_exp
// *           | ( PP | MM ) Identifier [ '[' expression ']' ]
  bool Unary_Exp_() {
    // TODO
  } // Unary_Exp_()
// * signed_unary_exp : Identifier [ '(' [ actual_parameter_list ] ')' 
// *                               |
// *                                 '[' expression ']'
// *                               ]
// *                  | Constant 
// *                  | '(' expression ')'
  bool Signed_Unary_Exp_() {
    // TODO
  } // Signed_Unary_Exp_()
// * unsigned_unary_exp : Identifier [ '(' [ actual_parameter_list ] ')' 
// *                                 |
// *                                   [ '[' expression ']' ] [ ( PP | MM ) ]
// *                                 ]
// *                    | Constant 
// *                    | '(' expression ')'
  bool Unsigned_Unary_Exp_() {
    // TODO
  } // UNsigned_Unary_Exp_()
public:
  void Error() {
    string msg = mScn_.Line_String();
    msg += "unexpected token : '";
    msg += mScn_.Get_Token().Name();
    msg += "'";
    throw msg;
  } // Error()

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