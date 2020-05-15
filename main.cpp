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
    if ( ch == '\n' ) {
      mCurrent_Line_++;
    } // if
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
    mCurrent_Line_ = 1;
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

};

struct InterCode {
  int Operater;
  int Parameter;
};

class Runner {
private:
  int mPc,mAx;
};

int main() {
  char ch[10];
  cin.getline( ch, 10 );
  cout << "Our-C running ..." << endl;
  do {
    cout << "> ";
    try {
      // TODO Main function
    } // try
    catch( string error_info ) {
      cout << error_info << endl;
    } // catch
  } while ( true ); // TODO solve the problem of EOF
  cout << "Our-C exited ..." << endl;
} // main()