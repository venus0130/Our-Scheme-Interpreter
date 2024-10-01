# include <iostream>
// # include <fstream>
# include <vector>
# include <stdio.h>
# include <cstring>
# include <iomanip>
# include <map>
# include <string>
# include <cstdlib>
# include <ctype.h>
# include <math.h>
# include <stdlib.h>
# include <sstream>



using namespace std;

// type2 = ���L��'    type3 = �L�O�Qdefine��  type4 = define���_�l�H  
struct DataTree {
  string str, type4  ;
  string type, type2, type3 ;
  int column, line;
  DataTree * left;
  DataTree * right;
};

struct Data {
  char ch;
  int column, line;
  int carNum;
  string str, type ;
  string type2, type3, type4  ;
};

struct Define {
  string temp; // �e�� 
  string object ; // �Ȧs�����~ 
  string type, type4;
};

// string mFileName = "test.txt" ; // Ū�ɥ�  
// fstream cin ; // Ū�ɥ�  
Data gtest ; // Ū�ɥ� 
Data gtreeTemp ; // �Ȧs��Tree��str 
bool gcorrectFileContent = false ; // input END-OF-FILE
int grun = 0 ; // �]�즹�}�C������ch 
int gline = -1, gcolumn = 1;
int gcharacter ;
bool gisDoubleQuoteFunc = false ;
typedef DataTree * TreePtr;
vector <Define*> gdefine; // gdefine
bool gdd = false ;
int gisCond = -1 ;
bool ghaveDoQuote = false ;
int gposition;  
bool gIsBreakForMainQuote = false ;
class Project {
public:

  vector<Data> *mList; // �s���W�� 
  vector<Data> *mcondList ;
  void Cond_fuc() {
    mcondList = new vector<Data>;
  } // Cond_fuc()
  
  void MList_fuc() {
    mList = new vector<Data>;    
  } // MList_fuc()
  
  void DoubleQuote() {
    
    int isBackFlash = 0 ;
    bool isBreak = false ;
    int xx = 0 ;
    
    while ( gtest.ch != '\n'  && gtest.ch != EOF && !isBreak ) {
      
      gtest.column = gcolumn, gtest.line = gline ;
      mList->push_back( gtest ) ;  
      gcolumn++, xx++ ;
      
      if ( gtest.ch == '"' && xx != 1 ) {
        
        for ( int j = mList->size()-2 ; j >= 0 && mList->at( j ).ch == '\\' ; j-- ) {
          isBackFlash++ ;
        } // for

        if ( isBackFlash == 0 || isBackFlash % 2 == 0 ) { // �S���n���ϱ�  
          gisDoubleQuoteFunc = true ;
          isBreak = true ;
        } // if
        else mList->erase( mList->end()-2 ) ; // �R�� '\' �~��Ū  
        
        isBackFlash = 0 ;
      } // if
      
      if ( !isBreak ) gtest.ch = cin.get() ;
    } // while
    
    if ( gtest.ch == '\n' || gtest.ch == EOF ) { // error message  
    
      gcolumn-- ;
      cout << endl << "> ERROR (no closing quote) : END-OF-LINE encountered at Line " 
           << gline << " Column " << gcolumn+1 << endl ;
      gline = -1, gcolumn = 1;
      mList->clear() ; 
      
    } // if
    else if ( gtest.ch == '"' ) {
         
      PrintSExp1( gisDoubleQuoteFunc ) ;
      gline = -1, gcolumn = 1 ;
      mList->clear() ;
      
    } // else if
    
  } // DoubleQuote()
  
  void PrintDoubleQuoteForTree( string str ) {
    
    for ( int i = 0 ; i < str.size() ; i++ ) {
              
      if ( str[i] == '\\' ) {
      
        if ( str[i+1] == '\\' ) {
          i++ ;
          cout << '\\' ;
        } // if
        else if ( str[i+1] == 'n' ) {    // �u���@�ӤϱץB�᭱�On 
          i++ ;
          cout << endl ; 
        } // else if()
        else if ( str[i+1] == 't' ) { // �u���@�ӤϱץB�᭱�Ot
          i++ ;
          cout << '\t' ; 
        } // else if()
        else cout << '\\' ;
            
      } // if
      else cout << str[i] ;
      
    } // for
    
    cout << endl ;
    
  } // PrintDoubleQuoteForTree()
  
  void PrintSExp1( bool gisDoubleQuoteFunc ) {
    
    if ( gisDoubleQuoteFunc ) {
      cout << endl << "> " ;
      for ( int i = 0 ; i < mList->size() ; i++ ) {
              
        if ( mList->at( i ).ch == '\\' ) {
        
          if ( mList->at( i+1 ).ch == '\\' ) {
            i++ ;
            cout << '\\' ;
          } // if
          else if ( mList->at( i+1 ).ch == 'n' ) {    // �u���@�ӤϱץB�᭱�On 
            i++ ;
            cout << endl ; 
          } // else if()
          else if ( mList->at( i+1 ).ch == 't' ) { // �u���@�ӤϱץB�᭱�Ot
            i++ ;
            cout << '\t' ; 
          } // else if()
          else cout << '\\' ;
              
        } // if
        else cout << mList->at( i ).ch ;
        
      } // for
      
      cout << endl ;
    } // if
    else {  
      string str ;
      bool isDefine = false;
      for ( int i = 0 ; i < mList->size() ; i++ ) {
        str += mList->at( i ).ch ;
      } // for

      for ( int i = 0 ; i < gdefine.size() ; i++ ) {
        Define* check = gdefine[i];
        if ( str == check -> temp ) {
          
          if ( check -> object[0] == '"' ) {
            cout << endl << "> " ;
            PrintDoubleQuoteForTree( check -> object ) ;
          } // if
          else if ( check -> object[0] != '\'' && check -> object[0] != '(' )
            cout << endl << "> " << check -> object << endl;  
          else {
            mList->clear() ;
            // ��check->object[]�̪��Ҧ��F�� ��imList ,
            
            stringstream ss( check->object );
            stringstream sss( check->type );
            Data token;
            
            while ( ss >> token.str ) {
              sss >> token.type ;
              
              Data temp;
              if ( token.str[0] == '"' ) {
                
                for ( ; token.str[token.str.size()-1] != '"' ; ) {
                  ss >> temp.str ;
                  token.str = token.str + " " + temp.str ;
                } // for
                
                
              } // if
              
              mList->push_back( token ) ;
              
            } // while
            /*
            // ���𪺹L�{  �]��define�᪺�F��i��Ofunc  
            int k = 0 ; 
            MainForQuote( k ) ;
            */
            // for ( int r = 0 ; r < mList->size() ; r++ ) cout << mList[r].str << "a" ;
            if ( !gIsBreakForMainQuote ) {
              int column = 1, i = -1, now = 1 ; // i for mList->size
              TreePtr mTree = new DataTree ;
              mTree -> type = "first" ;
              mTree -> left = NULL ;
              mTree -> right = NULL ;
              cout << endl << "> ";
              BuildTree( i, mTree, column ) ;
              
              PrintTree( mTree, now ) ;
              mTree = NULL ;
            } // if
            
          } // else
          
          
          isDefine = true;
        } // if
        
      } // for
      
      if ( !isDefine ) {
        cout << endl << "> ERROR (unbound symbol) : " << str << endl ;
      } // if
      
    } // else
    
    gisDoubleQuoteFunc = false ;
    
  } // PrintSExp1()

  
  void Semi() { // �Ĥ@�Ӧr���O���� ���ΦL�X 
  
    while ( gtest.ch != '\n' && gtest.ch != EOF ) {
      gtest.ch = cin.get() ; 
    } // while

    gcolumn = 1 ;
    mList->clear() ;  
    
  } // Semi()
  
  void SemiForTree() { // �Ĥ@�Ӧr���O���� ���ΦL�X 
  
    while ( gtest.ch != '\n' && gtest.ch != EOF ) {
      gtest.ch = cin.get() ; 
    } // while

    gcolumn = 1 ;
    
  } // SemiForTree()
  
  void T() {

    cout << endl << "> " << "#t" << endl;
    mList->clear() ;
    gline = -1, gcolumn = 1 ;

  } // T()

  void Nil() {

    cout << endl << "> " << "nil" << endl;
    mList->clear() ;
    gline = -1, gcolumn = 1 ;
    
  } // Nil()
  
  void Other() {
    
    if ( ( mList->size() == 1 && mList->at( 0 ).ch == 't' ) || 
         ( mList->size() == 2 && mList->at( 0 ).ch == '#' && mList->at( 1 ).ch == 't' ) ) 
      T() ;
    else if ( ( mList->size() == 2 && mList->at( 0 ).ch == '#' && mList->at( 1 ).ch == 'f' ) ||
              ( mList->size() == 2 && mList->at( 0 ).ch == '(' && mList->at( 1 ).ch == ')' ) ||
              ( mList->size() == 3 && mList->at( 0 ).ch == 'n' && mList->at( 1 ).ch == 'i' 
                && mList->at( 2 ).ch == 'l' ) )
      Nil() ;
    else if ( mList->size() == 1 && mList->at( 0 ).ch == '.' ) {
      cout << endl << "> ERROR (unexpected token) : atom or '(' expected when token at Line " 
           << mList->at( 0 ).line << " Column " << mList->at( 0 ).column << " is >>.<<" << endl ;
      
      while ( gtest.ch != '\n' && gtest.ch != EOF ) gtest.ch = cin.get() ;
    } // else if 
    else if ( mList->at( 0 ).ch == '+' || mList->at( 0 ).ch == '-'
              || isdigit( mList->at( 0 ).ch ) || mList->at( 0 ).ch == '.' ) {  
      Sign() ;

        
    } // else if
    else {
      PrintSExp1( gisDoubleQuoteFunc ) ;
    } // else
    
    gline = -1, gcolumn = 1;
    mList->clear() ;
  } // Other()
  
  void Sign() {
    
    int haveDot = 0 ; // --- ���p���I(�ɹs�i��) 
    bool haveStranger = false ; // ���ǩǪ��r���V�J�A���O�Ʀr�ίB�I�� 
    bool haveNumber = false ; //  �T�w���Ʀr 
        
    string s = "" ; // ����mList->at( n ).ch�নstring���O!!! 
    for ( int n = 0 ; n < mList->size() ; n++ ) {
      s = s + mList->at( n ).ch ;
    } // for
      
    const char*TEMP = s.c_str(); // ��string�নconst char���O!!! 
      
    for ( int n = 0 ; n < mList->size() ; n++ ) {
      if ( mList->at( n ).ch == '.' ) haveDot++ ;  // �P�_���L�p���I 
      if ( isdigit( mList->at( n ).ch ) ) haveNumber = true ; // �T�w���Ʀr 
    } // for
      
    for ( int n = 1 ; n < mList->size() ; n++ ) {  
      if ( !isdigit( mList->at( n ).ch ) && mList->at( n ).ch != '.' ) haveStranger = true; // +-123,-1a, 1a
    } // for
    
    if ( haveDot == 1 && !haveStranger && haveNumber ) {
      double decimal = atof( TEMP ) ; // �A��const char�নdouble���O!!! 
      cout << endl << "> " << fixed  << setprecision( 3 ) << decimal << endl ;
    } // if() ���p���I 
    else if ( haveDot > 1 || haveStranger || !haveNumber ) {
      cout << endl << "> " << TEMP << endl ; 
    } // else if
    // else if() ���D�Ʀr�Τp�ƪ��F��b�Ĥ@���
    else { 
      int numbers = atof( TEMP ) ;
      cout << endl << "> " << numbers << endl ;
    } // else ��� 
      
  } // Sign()
  
  bool SignForTree() {
    
    int haveDot = 0 ; // --- ���p���I(�ɹs�i��) 
    bool haveStranger = false ; // ���ǩǪ��r���V�J�A���O�Ʀr�ίB�I�� 
    bool haveNumber = false ; //  �T�w���Ʀr 
      
    const char*TEMP = gtreeTemp.str.c_str(); // ��string�নconst char���O!!! 
      
    for ( int n = 0 ; n < gtreeTemp.str.size() ; n++ ) {
      if ( gtreeTemp.str[n] == '.' ) haveDot++ ;  // �P�_���L�p���I 
      if ( isdigit( gtreeTemp.str[n] ) ) haveNumber = true ; // �T�w���Ʀr 
    } // for
      
    for ( int n = 1 ; n < gtreeTemp.str.size() ; n++ ) {  
      if ( !isdigit( gtreeTemp.str[n] ) && gtreeTemp.str[n] != '.' ) haveStranger = true; // +-123,-1a, 1a
    } // for
    
    if ( haveDot == 1 && !haveStranger && haveNumber ) {
      double decimal = atof( TEMP ) ; // ��const char�নdouble���O!!! 
      char buffer[20]; // Adjust the buffer size accordingly to accommodate your string
      sprintf( buffer, "%.3f", decimal );
      string result( buffer );
      
      gtreeTemp.str = result ;
      return true ;
    } // if() ���p���I 
    else if ( haveDot > 1 || haveStranger || !haveNumber ) return false ; 
    // else if() ���D�Ʀr�Τp�ƪ��F��b�Ĥ@���
    else { 
      int numbers = atof( TEMP ) ;
      stringstream ss;
      ss << numbers;
      gtreeTemp.str = ss.str();

      return true ;
    } // else ��� 
      
  } // SignForTree()
  
  bool ReadBracket() {
    
    int left_bracket = -1, right_bracket = 0 ;
    bool isbreak = false ;
    
    while ( ( left_bracket == -1 || left_bracket != right_bracket ) 
            && gtest.ch != EOF && !isbreak ) { // ����Ū�i�� 
      
      if ( gtest.ch == '\'' ) {
        gtreeTemp.column = gcolumn, gtreeTemp.line = gline ;
        gtreeTemp.str += gtest.ch, gtreeTemp.type = "quote" ;
        mList->push_back( gtreeTemp ) ;  
        gtreeTemp.str = ""; 
        gcolumn++ ;   
      } // if       
      else if ( gtest.ch == '(' ) {
        if ( left_bracket == -1 ) left_bracket++ ;
        left_bracket++ ;
        gtreeTemp.column = gcolumn, gtreeTemp.line = gline ;
        gtreeTemp.str += gtest.ch, gtreeTemp.type = "left_bracket" ;
        mList->push_back( gtreeTemp ) ;  
        gtreeTemp.str = ""; 
        gcolumn++ ;
        if ( !IsCorrectBracket() ) isbreak = true ;
      } // else if
      else if ( gtest.ch == ')' ) {
        right_bracket++ ;
        gtreeTemp.column = gcolumn, gtreeTemp.line = gline ;
        gtreeTemp.str += gtest.ch, gtreeTemp.type = "right_bracket" ;
        mList->push_back( gtreeTemp ) ; 
        gtreeTemp.str = "";  
        gcolumn++ ;
        if ( !IsCorrectBracket() ) isbreak = true ;
      } // else if 
      else if ( gtest.ch == '"' ) {
        gtreeTemp.column = gcolumn, gtreeTemp.line = gline, gtreeTemp.type = "string" ;
        if ( !DoubleQuoteForTree() ) isbreak = true ;
        else {
          mList->push_back( gtreeTemp ) ; 
          gtreeTemp.str = "";  
        } // else
        
        if ( !IsCorrectBracket() ) isbreak = true ;
      } // else if
      else if ( gtest.ch == ' ' || gtest.ch == '\t' ) {
        gcolumn++ ;
      } // else if
      else if ( gtest.ch == '\n' ) {
        gline++, gcolumn = 1 ;
      } // else if
      else if ( gtest.ch == ';' ) {
        SemiForTree() ;
        gline++ ;
      } // else if
      else if ( gtest.ch == '.' && ( cin.peek() == ' ' || cin.peek() == '\t' || 
                                     cin.peek() == '(' || cin.peek() == ')' || cin.peek() == '\'' ||
                                     cin.peek() == '"' || cin.peek() == ';' || cin.peek() == '\n' ) ) {
        
        gtreeTemp.column = gcolumn, gtreeTemp.line = gline ;
        gtreeTemp.type = "dot", gtreeTemp.str += gtest.ch ;
        mList->push_back( gtreeTemp ) ;  
        gtreeTemp.str = ""; 
        gcolumn++ ;
        if ( !IsCorrectBracket() ) isbreak = true ;
      } // else if
      else {
        if ( gtreeTemp.str == "" ) gtreeTemp.column = gcolumn, gtreeTemp.line = gline ;
        gtreeTemp.str += gtest.ch ; 
        gcolumn++ ;
        
        gcharacter = cin.peek() ;
        while ( gcharacter != EOF && cin.peek() != ')' && cin.peek() != '"' 
                && cin.peek() != '\t' && cin.peek() != ' ' && cin.peek() != '\n' 
                && cin.peek() != ';' && cin.peek() != '(' && cin.peek() != '\'' ) {
          gtest.ch = cin.get() ;
          gtreeTemp.str += gtest.ch ; 
          gcolumn++ ;
        } // while
        
        if ( cin.peek() == '(' || cin.peek() == ')' || cin.peek() == '"' || cin.peek() == '\t' 
             || cin.peek() == ' ' || cin.peek() == '\n' || cin.peek() == ';' || cin.peek() == '\'' ) {
          
          if ( SignForTree() ) gtreeTemp.type = "sign" ;
          else {
            if ( gtreeTemp.str == "#f" || gtreeTemp.str == "()" || gtreeTemp.str == "nil" )
              gtreeTemp.str = "nil", gtreeTemp.type = "nil" ;
            else if ( gtreeTemp.str == "t" || gtreeTemp.str == "#t"  ) 
              gtreeTemp.str = "#t", gtreeTemp.type = "#t" ;  
            else gtreeTemp.type = "symbol" ;
          } // else 
          
          mList->push_back( gtreeTemp ) ; 
          if ( !IsCorrectBracket() ) isbreak = true ;
          gtreeTemp.str = ""; 
        } // if
        
      } // else
      
      if ( mList->at( 0 ).type == "quote" ) { // ' string/symbol/int
        if ( 1 < mList->size() ) {
          if ( mList->at( 1 ).type != "left_bracket" && mList->at( 1 ).type != "quote" ) {
            return true ;
          } // if
          
        } // if
        
      } // if
      
      if ( !isbreak && left_bracket != right_bracket && gtest.ch != EOF ) gtest.ch = cin.get() ;
    
    } // while 
    
    for ( int i = 0 ; i < mList->size() ; i++ ) {
      if ( mList->at( i ).type == "left_bracket" && i+1 < mList->size() 
           && mList->at( i+1 ).type == "right_bracket" ) {
        mList->at( i ).type = "nil", mList->at( i ).str = "nil" ;
        mList->erase( mList->begin()+i+1 ) ;
      } // if
      
    } // for
    
    if ( mList->size() == 3 && mList->at( 0 ).type == "left_bracket" && mList->at( 1 ).str == "exit" 
         && mList->at( 2 ).type == "right_bracket" )
      gcorrectFileContent = true ;
      
    if ( mList->size() == 3 && mList->at( 0 ).type == "left_bracket" 
         && mList->at( 1 ).str == "clean-environment" 
         && mList->at( 2 ).type == "right_bracket" ) {
      gdefine.clear() ;
      cout << endl << "> environment cleaned" << endl ;
      return false ;
    } // if
      
    
    if ( isbreak || gcorrectFileContent ) return false ;
    else return true ;
    
  } // ReadBracket()
  
  bool IsCorrectBracket() { 
  
    for ( int i = 0 ; i < mList->size() ; i++ ) { // ') '.
      if ( mList->at( i ).type == "quote" ) {
        if ( i+1 < mList->size() && mList->at( i+1 ).type == "right_bracket" ) {
          cout << endl << "> ERROR (unexpected token) : atom or '(' expected when token at Line " 
               << mList->at( i+1 ).line << " Column " << mList->at( i+1 ).column << " is >>)<<" << endl ;
          gline = -1, gcolumn = 1;
          mList->clear() ;
          while ( gtest.ch != '\n' && gtest.ch != EOF ) gtest.ch = cin.get() ;
          return false ;
        } // if
        else if ( i+1 < mList->size() && mList->at( i+1 ).type == "dot" ) {
          cout << endl << "> ERROR (unexpected token) : atom or '(' expected when token at Line " 
               << mList->at( i+1 ).line << " Column " << mList->at( i+1 ).column << " is >>.<<" << endl ;
          gline = -1, gcolumn = 1;
          mList->clear() ;
          while ( gtest.ch != '\n' && gtest.ch != EOF ) gtest.ch = cin.get() ;
          return false ;
        } // else if
        
      } // if
      
    } // for
  
    for ( int i = 0 ; i < mList->size() ; i++ ) { // .�᭱�b�J��A���e�S��. 
        
      if ( mList->at( i ).type == "dot" ) { // ..
        if ( i+1 < mList->size() ) {
          if ( mList->at( i+1 ).type == "dot" ) {
            cout << endl << "> ERROR (unexpected token) : atom or '(' expected when token at Line " 
                 << mList->at( i+1 ).line << " Column " << mList->at( i+1 ).column << " is >>.<<" << endl ;
            gline = -1, gcolumn = 1;
            mList->clear() ;
            while ( gtest.ch != '\n' && gtest.ch != EOF ) gtest.ch = cin.get() ;
            return false ;
          } // if
          
        } // if
        
      } // if
      else if ( mList->at( i ).type == "dot" ) { // . xx .
        i++ ;
        while ( i < mList->size() && ( mList->at( i ).type != "left_bracket" || 
                                       mList->at( i ).type != "right_bracket" ) ) {
          if ( mList->at( i ).type == "dot" ) {
            cout << endl << "> ERROR (unexpected token) : ')' expected when token at Line " 
                 << mList->at( i ).line << " Column " << mList->at( i ).column << " is >>.<<" << endl ;
            gline = -1, gcolumn = 1;
            mList->clear() ;
            while ( gtest.ch != '\n' && gtest.ch != EOF ) gtest.ch = cin.get() ;
            return false ;
          } // if 
          
          i++ ;
        } // while

      } // else if

    } // for
    
    for ( int i = 0 ; i < mList->size() ; i++ ) { // .)  (.
      
      if ( mList->at( i ).type == "dot" ) {
        if ( mList->at( i-1 ).type == "left_bracket" ) {
          cout << endl << "> ERROR (unexpected token) : atom or '(' expected when token at Line " 
               << mList->at( i ).line << " Column " << mList->at( i ).column << " is >>.<<" << endl ;
          gline = -1, gcolumn = 1;
          mList->clear() ;
          while ( gtest.ch != '\n' && gtest.ch != EOF ) gtest.ch = cin.get() ;
          return false ;
        } // if
        else if ( i+1 < mList->size() ) {
          if ( mList->at( i+1 ).type == "right_bracket" ) {
            cout << endl << "> ERROR (unexpected token) : atom or '(' expected when token at Line " 
                 << mList->at( i+1 ).line << " Column " << mList->at( i+1 ).column << " is >>)<<" << endl ;
            gline = -1, gcolumn = 1;
            mList->clear() ;
            while ( gtest.ch != '\n' && gtest.ch != EOF ) gtest.ch = cin.get() ;
            return false ;
          } // if
        } // else if
 
      } // if
      
    } // for
    
    for ( int i = 0 ; i < mList->size() ; i++ ) { // .�᭱�u�঳�@��string symbol digit () 
      
      if ( i+1 < mList->size() && mList->at( i+1 ).type == "quote" ) return true ; // .'
      
      int many = 0, temp = i ;
      if ( mList->at( i ).type == "dot" ) {
        if ( i+2 < mList->size() && mList->at( i+1 ).type != "left_bracket" 
             && mList->at( i+2 ).type != "right_bracket" ) {
          cout << endl << "> ERROR (unexpected token) : ')' expected when token at Line " 
               << mList->at( i+2 ).line << " Column " << mList->at( i+2 ).column << " is >>" 
               << mList->at( i+2 ).str << "<<" << endl ;
          gline = -1, gcolumn = 1;
          mList->clear() ;
          while ( gtest.ch != '\n' && gtest.ch != EOF ) gtest.ch = cin.get() ;
          return false ;
        } // if
        
        i++ ;
        if ( i < mList->size() && mList->at( i ).type == "left_bracket" ) {
          i++ ;
          int left = 1, right = 0 ;
          while ( left != right && i < mList->size() ) {
            if ( mList->at( i ).type == "left_bracket" ) left++ ;
            else if ( mList->at( i ).type == "right_bracket" ) right++ ;
            
            i++ ;
          } // while
          
          if ( i < mList->size() ) {
            if ( mList->at( i ).type != "right_bracket" ) {
              cout << endl << "> ERROR (unexpected token) : ')' expected when token at Line " 
                   << mList->at( i ).line << " Column " << mList->at( i ).column << " is >>" 
                   << mList->at( i ).str << "<<" << endl ;
              gline = -1, gcolumn = 1;
              mList->clear() ;
              while ( gtest.ch != '\n' && gtest.ch != EOF ) gtest.ch = cin.get() ;
              return false ;
            } // if
            
          } // if
              
        } // if

        i = temp ;   
      } // if
      
    } // for
    
    return true ;

  } // IsCorrectBracket()
  
  bool DeleteDotBracketAndTNil() { // .(xxxx) = xxxx   . #t = ��    . nil = ��     ()
    
    int forDeleteBracket = 0 ;
    
    for ( int i = 0 ; i < mList->size() ; i++ ) {
      
      if ( mList->at( i ).str == "(" && mList->at( i+1 ).str == ")" ) {
        Data temp ;
        temp.type3 = mList->at( i ).type3 ;
        temp.type4 = mList->at( i ).type4 ;
        mList->erase( mList->begin()+i ) ; // erase 
        mList->erase( mList->begin()+i ) ; // erase 
        temp.str = "nil", temp.type = "nil" ;
        mList->insert( mList->begin()+i, temp ) ;
      } // if
      
    } // for
    
    for ( int i = 0 ; i < mList->size() ; i++ ) {
      
      if ( mList->at( i ).type == "dot" ) {
        if ( i+1 < mList->size() && mList->at( i+1 ).type == "left_bracket" ) {
          forDeleteBracket++ ;
          mList->erase( mList->begin()+i ) ; // erase .
          mList->erase( mList->begin()+i ) ; // erase (
          
          int j = i ;
          bool isbreak = false ;
          for ( ; j < mList->size() && !isbreak ; j++ ) {
            if ( mList->at( j ).type == "left_bracket" ) {
              forDeleteBracket++ ;
            } // if
            else if ( mList->at( j ).type == "right_bracket" ) {
              if ( forDeleteBracket == 1 ) {
                mList->erase( mList->begin()+j ) ; // erase )
                isbreak = true ;
              } // if
              
              forDeleteBracket-- ;
            } // else if  
            
          } // for
          
          i-- ;
        } // if
        else if ( i+1 < mList->size() && mList->at( i+1 ).type == "quote" ) { // .'
          mList->erase( mList->begin()+i ) ; // erase .
        } // else if
        
      } // if
      
    } // for
    
    for ( int i = 0 ; i < mList->size() ; i++ ) {
      
      if ( mList->at( i ).type == "dot" ) {
        if ( i+1 < mList->size() && ( mList->at( i+1 ).str == "nil" || mList->at( i+1 ).str == "#t" ) ) {
          mList->erase( mList->begin()+i ) ; // erase .
          mList->erase( mList->begin()+i ) ; // erase nil or #t
          i-- ;
        } // if
        
      } // if
      
    } // for
    
    if ( mList->size() == 3 && mList->at( 0 ).type == "left_bracket" && mList->at( 1 ).str == "exit" 
         && mList->at( 2 ).type == "right_bracket" )
      gcorrectFileContent = true ;
    if ( mList->size() == 3 && mList->at( 0 ).type == "left_bracket" 
         && mList->at( 1 ).str == "clean-environment" 
         && mList->at( 2 ).type == "right_bracket" ) {
      gdefine.clear() ;
      cout << endl << "> environment cleaned" << endl ;
      return false ;
    } // if
    
    return true ;
    
  } // DeleteDotBracketAndTNil()
  
  bool DoubleQuoteForTree() {
    
    int isBackFlash = 0 ;
    bool isBreak = false ;
    int xx = 0 ;
    
    while ( gtest.ch != '\n'  && gtest.ch != EOF && !isBreak ) {
      
      gtreeTemp.str += gtest.ch ;  
      gcolumn++, xx++ ;
      
      if ( gtest.ch == '"' && xx != 1 ) {
        
        for ( int j = gtreeTemp.str.size()-2 ; j >= 0 && gtreeTemp.str[j] == '\\' ; j-- ) {
          isBackFlash++ ;
        } // for

        if ( isBackFlash == 0 || isBackFlash % 2 == 0 ) { // �S���n���ϱ�  
          // gisDoubleQuoteFunc = true ;
          isBreak = true ;
        } // if
        else {
          int k = gtreeTemp.str.size() - 1 ;
          gtreeTemp.str.erase( k-1, 1 ) ; // �R�� '\' �~��Ū  
        } // else 
        
        isBackFlash = 0 ;
      } // if
      
      if ( !isBreak ) gtest.ch = cin.get() ;
    } // while
    
    if ( gtest.ch == '\n' ) { // error message  
    
      gcolumn-- ;
      cout << endl << "> ERROR (no closing quote) : END-OF-LINE encountered at Line " 
           << gline << " Column " << gcolumn+1 << endl ;
      gline = -1, gcolumn = 1;
      gtreeTemp.str.clear() ; 
      return false ;
      
    } // if
    else if ( gtest.ch == '"' ) {      
      return true ;
    } // else if
    else return false ;
    
  } // DoubleQuoteForTree()
  
  void BuildTree( int& i, TreePtr &mTree, int &column ) {

    i++;

    // �p�G�r���O���A���A�إߥ��l�� 
    if ( mList->at( i ).type == "left_bracket" || mList->at( i ).type == "quote" ) { 

      if ( mList->at( i ).type == "quote" ) mTree->type2 = "quote" ;
        
      mTree->column = column ;
      column += 2 ;
      mTree->left = new DataTree();
      mTree->left->left = NULL ;
      mTree->left->right = NULL ;
      BuildTree( i, mTree->left, column );
    
    } // if

    // �s�� 
    if ( i < mList->size() && mList->at( i ).type != "left_bracket" && mList->at( i ).type != "quote" 
         && mList->at( i ).type != "right_bracket" ) {
      mTree->str = mList->at( i ).str;
      mTree->type = mList->at( i ).type;
      mTree->column = column; 
      if ( i+1 >= mList->size() ) return ;
    } // if

    // �p�G�r���O�k�A���A�^��W�@�ӥ��A�� 
    if ( i < mList->size() && mList->at( i ).type == "right_bracket" ) {
      mTree->right = new DataTree();
      mTree->right->left = NULL ;
      mTree->right->right = NULL ;
      mTree->column = column; 

      column -= 2 ;
      
      i++ ;
      return;
    } // if

    if ( i >= mList->size() ) return ;
    // �إߥk�l�� 
    else {
      if ( mList->at( i ).type == "left_bracket" || mList->at( i ).type == "quote" ) i-- ;
      mTree->right = new DataTree();
      mTree->right->left = NULL ;
      mTree->right->right = NULL ;
      BuildTree( i, mTree->right, column );
    } // else 

  } // BuildTree()
  
  void PrintTree( TreePtr mTree, int &now ) {
    
    if ( mTree == NULL ) {
      return;
    } // if
    
    // �����j��̥��`�I 
    if ( mTree->left != NULL ) {
        
      while ( now < mTree->column ) {  // mTree->left->column-2
        now++ ;
        cout << " " ;
      } // while
        
      if ( mTree->type2 == "quote" ) {
        cout << "( quote" << endl ;
        now = 1 ;
      } // if
      else {
        cout << "( " ;
        now += 2 ;
      } // else

    } // if
    
    PrintTree( mTree->left, now );

    // �}�l�L�X�`�I���ƭ� 
    if ( mTree != NULL && mTree->str != "" ) {
      while ( now < mTree->column ) {
        now++ ;
        cout << " " ;
      } // while
      
      if ( mTree->type == "string" ) {  
        PrintDoubleQuoteForTree( mTree->str ) ; 
        now = 1 ;
      } // if
      else {
        cout << mTree->str << endl ;
        now = 1 ;
      } // else
      
    } // if

    // ���j�L�X�k�l�� 
    PrintTree( mTree->right, now );
    
    if ( mTree != NULL && mTree->right != NULL && mTree->right->right == NULL ) {

      while ( now < mTree->column-2 ) {
        now++ ;
        cout << " " ;
      } // while
        
      cout << ")" << endl ;
      now = 1 ;
    
    } // if

  } // PrintTree()
   
  void DoQuote( int start, int end ) { // ��qupte�����F�F�аO�_�� 
    
    for ( int i = start ; i < end ; i++ ) {
      
      int bracket = 0, j = i ; 
      bool isbreak = false ;
      
      if ( mList->at( j ).type == "quote" ) {
        j++ ;
        
        for ( ; !isbreak && j < end ; j++ ) {  // '�᭱���F���ݩ�L  
          if ( mList->at( j ).type == "quote" ) ;
          else if ( mList->at( j ).type == "left_bracket" ) bracket++ ;
          else if ( mList->at( j ).type == "right_bracket" ) bracket-- ;
          else {
            if ( bracket == 0 ) {
              Data temp ;
              temp.str = ")", temp.type = "right_bracket" ;
              mList->insert( mList->begin()+j+1, temp ) ;
              isbreak = true ;
            } // if
            
          } // else
          
          if ( !isbreak ) {
            if ( bracket == 0 && mList->at( j ).type != "quote" ) {
              Data temp ;
              temp.str = ")", temp.type = "right_bracket" ;
              mList->insert( mList->begin()+j+1, temp ) ;
              isbreak = true ;
            } // if
            
          } // if
          
        } // for
        
      }  // if
      
    } // for

  } // DoQuote()
  
  void DoDefine() {
    gdd = true ;
    Define* objPtr = new Define();
    objPtr -> temp = mList->at( 2 ).str; // �e�� 
    
    bool definedefine = false ; // ����define  
    
    for ( int i = 0 ; gdefine.size() > 0 && i < gdefine.size() ; i++ ) {
      Define* search = gdefine[i];
      if ( objPtr -> temp == search -> temp ) {
        definedefine = true ;
        gdefine.erase( gdefine.begin()+i );
      } // if
    } // for // ���]���ea�Qdefine�L�F�A�S���sdefine�@���A�n�R�����e�� 
      
    if ( mList->at( 3 ).str != "(" ) {
      objPtr -> object = mList->at( 3 ).str; // ���e = �e�� 
      objPtr -> type = mList->at( 3 ).type ;
      if ( mList->at( 3 ).type4 != "" ) objPtr -> type4 = mList->at( 3 ).type4;
      else objPtr -> type4 = mList->at( 2 ).str;
    } // if
    else {
      string special, type ; 
      int left = 0;
      int right = 0; 
      
      bool isbreak = false ;
      for ( int j = 3 ; j < mList->size() && !isbreak ; j++ ) { 
        if ( mList->at( j ).str == "(" ) left++;
        else if ( mList->at( j ).str == ")" ) right++;
        special += mList->at( j ).str ;
        special += " ";
        type += mList->at( j ).type ;
        type += " ";
        
        if ( mList->at( j ).str != "'" && left == right ) isbreak = true ;
      } // for // gdefine a (...) or '(...)�����p 
      
      objPtr -> object = special ;
      objPtr -> type = type ;
      if ( mList->at( 3 ).type4 != "" ) objPtr -> type4 = mList->at( 3 ).type4;
      else objPtr -> type4 = mList->at( 2 ).str;
      // cout << "6" << mList->at( 3 ).type4 ;
    } // else 
    
    for ( int i = 0 ; i < gdefine.size() ; i++ ) { // ����define (define a 5) (define c a ) c = 5 
      
      if ( objPtr -> object == gdefine[i]->temp ) {
        definedefine = false ;
        objPtr -> object = gdefine[i]->object ;
        objPtr -> type4 = gdefine[i]->type4;
        objPtr -> type = gdefine[i]->type ;
        i = 0 ;
      } // if
      
    } // for
    
    if ( definedefine ) objPtr -> type4 += "new" ;
    gdefine.push_back( objPtr );
    cout << endl << "> " << objPtr -> temp << " defined" << endl ;

  } // DoDefine()
  
  void MainForQuote( int k ) {
    
    bool checkIfNeedDoFuncAgain = false ;
    
    if ( k-1 >= 0 && mList->at( k-1 ).str != "define" ) {
      CheckDefine( k, k+1 ) ;
    } // if
    
    if ( mList->at( k ).str == "\'" ) {
      ghaveDoQuote = true ;
      mList->erase( mList->begin()+k ) ;
      
      int j = k, bracket = 0 ;
      bool isbreak = false ;
      for ( ; !isbreak && j < mList->size() ; j++ ) {  // �@����'������) ������  
        if ( mList->at( j ).str == "\'" ) bracket++ ;
        else if ( mList->at( j ).str == "(" ) bracket++ ;
        else if ( mList->at( j ).str == ")" ) bracket-- ;
        else {

          if ( bracket == 0 ) {
            isbreak = true ;
          } // if
          
        } // else
        
        if ( !isbreak ) {
          
          if ( bracket == 0 && mList->at( j ).str != "\'" ) {
            isbreak = true ;
          } // if
          
        } // if
          
      } // for
      
      DoQuote( k, j ) ;
      
      k = j-1 ;
      
      if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
      
    } // if
    else if ( mList->at( k ).str == "quote" ) { // ��W���P�z  
      ghaveDoQuote = true ;
      int end = Quote( k ) ;
      
      DoQuote( k, end ) ;
      
      k = end-1 ;
      
      if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
      
    } // else if
    else {
      /*
      if ( k-1 >= 0 && mList->at( k-1 ).str != "define" ) {
        CheckDefine( k, k+1 ) ;
      } // if
      */
      if ( k-1 >= 0 && mList->at( k ).str == "define" && mList->at( k-1 ).str == "(" ) {
      
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+2, EndOfCheckDefine( k+2 ) ) ;
        DoDefine() ;
        
      } // if
      else if ( k-1 >= 0 && mList->at( k ).str == "cons" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Cons( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "list" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        List( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "car" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Car( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "cdr" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Cdr( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "pair?" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Pair_is( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "null?" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Null_is( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "integer?" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Integer_is( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "atom?" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Atom_is( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k-1 ).str == "(" && 
                ( mList->at( k ).str == "real?" || mList->at( k ).str == "number?" ) ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Real_Number_is( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "string?" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        String_is( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "boolean?" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Boolean_is( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "symbol?" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Symbol_is( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "list?" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        List_is( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "equal?" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Equal_is( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "eqv?" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Eqv_is( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && 
                ( mList->at( k ).str == "+" || mList->at( k ).str == "-" 
                  || mList->at( k ).str == "*" || mList->at( k ).str == "/" ) 
                && mList->at( k-1 ).str == "(" ) {
                  
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Caculate( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "not" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Not( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && 
                ( mList->at( k ).str == ">" || mList->at( k ).str == "<" || mList->at( k ).str == "="
                  || mList->at( k ).str == ">=" || mList->at( k ).str == "<=" ) 
                && mList->at( k-1 ).str == "(" ) {
                  
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Compare( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "string-append" && mList->at( k-1 ).str == "(" ) {
                  
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        String_append( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if    
      
      else if ( k-1 >= 0 && 
                ( mList->at( k ).str == "string>?" || mList->at( k ).str == "string<?"
                  || mList->at( k ).str == "string=?" ) 
                && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        String_compare( k ) ;            
        checkIfNeedDoFuncAgain = true ;
      } // else if 
      else if ( k-1 >= 0 && mList->at( k ).str == "cond" && mList->at( k-1 ).str == "(" ) {
        
        
        gisCond = k ;
        // if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        CheckDefine( k-1, EndOfCheckDefine( k+1 ) ) ;
        /*  
        for( int n = 0 ; n <mList->size() ; n ++ )
            cout << mList[n].str << " " ; 
          cout << endl; 
        */

        Cond( k ) ;      
   
        
        gisCond = -1 ;
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
               
                
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "if" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        If( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if    
      else if ( k-1 >= 0 && mList->at( k ).str == "begin" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Begin( k ) ;    
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mList->at( k ).str == "and" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        And( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if  
      else if ( k-1 >= 0 && mList->at( k ).str == "or" && mList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Or( k ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else {
        
        // ��procedure
        if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
        
      } // else
      
      if ( checkIfNeedDoFuncAgain ) { // �e��������^�Ǫ��F��O�@��func�W�r�A�N�n�A��func  
        k -= 1 ;
        if ( k-1 >= 0 && mList->at( k-1 ).str == "(" ) {
          
          if ( k-1 >= 0 && mList->at( k-1 ).str != "define" ) {
            CheckDefine( k, k+1 ) ;
          } // if
          
          if ( k-1 >= 0 && mList->at( k ).str == "define" && mList->at( k-1 ).str == "(" ) {
          
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+2, EndOfCheckDefine( k+2 ) ) ;
            DoDefine() ;
          
          } // if
          else if ( k-1 >= 0 && mList->at( k ).str == "cons" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Cons( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "list" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            List( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "car" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Car( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "cdr" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Cdr( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "pair?" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Pair_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "null?" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Null_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "integer?" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Integer_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "atom?" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Atom_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k-1 ).str == "(" && 
                    ( mList->at( k ).str == "real?" || mList->at( k ).str == "number?" ) ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Real_Number_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "string?" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            String_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "boolean?" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Boolean_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "symbol?" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Symbol_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "list?" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            List_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "equal?" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Equal_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "eqv?" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Eqv_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && 
                    ( mList->at( k ).str == "+" || mList->at( k ).str == "-" 
                      || mList->at( k ).str == "*" || mList->at( k ).str == "/" ) 
                    && mList->at( k-1 ).str == "(" ) {
                      
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Caculate( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "not" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Not( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && 
                    ( mList->at( k ).str == ">" || mList->at( k ).str == "<" || mList->at( k ).str == "="
                      || mList->at( k ).str == ">=" || mList->at( k ).str == "<=" ) 
                    && mList->at( k-1 ).str == "(" ) {
                      
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Compare( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "string-append" && mList->at( k-1 ).str == "(" ) {
                      
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            String_append( k ) ;
            
          } // else if    
          
          else if ( k-1 >= 0 && 
                    ( mList->at( k ).str == "string>?" || mList->at( k ).str == "string<?"
                      || mList->at( k ).str == "string=?" ) 
                    && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            String_compare( k ) ;            
            
          } // else if 
          else if ( k-1 >= 0 && mList->at( k ).str == "cond" && mList->at( k-1 ).str == "(" ) {
        
            // gisCond = k ;
            Cond( k ) ;
            gisCond = -1 ;
            if ( k+1 < mList->size() ) MainForQuote( EndOfCheckDefine( k+1 ) ) ;
            
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "if" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            If( k ) ;
            
          } // else if    
          else if ( k-1 >= 0 && mList->at( k ).str == "begin" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Begin( k ) ;    
              
          } // else if
          else if ( k-1 >= 0 && mList->at( k ).str == "and" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            And( k ) ;
            
          } // else if  
          else if ( k-1 >= 0 && mList->at( k ).str == "or" && mList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mList->size() ) MainForQuote( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Or( k ) ;
            
          } // else if
          
        } // if
      
      } // if
      
    } // else

  } // MainForQuote()
  
  void Or( int k ) {
    
    int i = k+1 ;
    
    if ( mList->at( i ).str != "nil" ) {
      
      Data temp ;
      temp.str = mList->at( i ).str, temp.type = mList->at( i ).type ;
      temp.type3 = mList->at( i ).type3, temp.type4 = mList->at( i ).type4 ;
      
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (or �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (or .....) �� )  
        
      } // while
      
      mList->insert( mList->begin()+k, temp ) ;
      
    } // if
    else {

      string str, type ;
      string type3, type4 ;
      bool isbreak = false ;
      int bracket = 0 ;
      for ( ; !isbreak ; i++ ) {
        if ( mList->at( i ).str == "(" || mList->at( i ).str == "'" ) bracket++ ;
        else if ( mList->at( i ).str == ")" ) bracket-- ;
        
        str = str + mList->at( i ).str + " " ;
        type3 = mList->at( i ).type3 + " " ;
        type4 = mList->at( i ).type4 + " " ;
        type = type + mList->at( i ).type + " " ;
        
        if ( bracket == 0 ) isbreak = true ;
        if ( isbreak && i+1 < mList->size() && mList->at( i+1 ).str != ")" ) {
          str = "", type = "" ;
          type3 = "", type4 = "" ;
          isbreak = false ;
        } // if
        
      } // for
      
      // erase
      bracket = 1 ;
      mList->erase( mList->begin() + k-1 );
      k-- ; 
      for ( ; bracket != 0 ; ) {
        if ( mList->at( k ).str == ")" ) bracket--;
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++;
        
        mList->erase( mList->begin() + k );
  
      } // for
  
      // insert
      stringstream ss( str );
      stringstream sss( type );
      stringstream ssss( type3 );
      stringstream sssss( type4 );
      Data token;
      
      while ( ss >> token.str ) {
        
        sss >> token.type ;
        ssss >> token.type3 ;
        sssss >> token.type4 ;
        Data temp;
        if ( token.str[0] == '"' ) {
          
          for ( ; token.str[token.str.size()-1] != '"' ; ) {
            ss >> temp.str ;
            token.str = token.str + " " + temp.str ;
          } // for
          
          
        } // if
        
        token.type4 = type4 ;
        token.type3 = type3 ;
        mList->insert( mList->begin()+k, token ) ;
        k++ ;
        
      } // while

    } // else
    
  } // Or()  
  
  void And( int k ) {
    
    int i = k+1 ;
    
    if ( mList->at( i ).str == "nil" ) {
      
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (and �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (and .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "nil", temp.type = "nil" ;
      mList->insert( mList->begin()+k, temp ) ;
      
    } // if
    else {

      string str, type ;
      string type3, type4 ;
      bool isbreak = false ;
      int bracket = 0 ;
      for ( ; !isbreak ; i++ ) {
        if ( mList->at( i ).str == "(" || mList->at( i ).str == "'" ) bracket++ ;
        else if ( mList->at( i ).str == ")" ) bracket-- ;
        
        str = str + mList->at( i ).str + " " ;
        type3 = mList->at( i ).type3 + " " ;
        type4 = mList->at( i ).type4 + " " ;
        type = type + mList->at( i ).type + " " ;
        
        if ( bracket == 0 ) isbreak = true ;
        if ( isbreak && i+1 < mList->size() && mList->at( i+1 ).str != ")" ) {
          str = "", type = "" ;
          type3 = "", type4 = "" ;
          isbreak = false ;
        } // if
        
      } // for
      
      // erase
      bracket = 1 ;
      mList->erase( mList->begin() + k-1 );
      k-- ; 
      for ( ; bracket != 0 ; ) {
        if ( mList->at( k ).str == ")" ) bracket--;
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++;
        
        mList->erase( mList->begin() + k );
  
      } // for
  
      // insert
      stringstream ss( str );
      stringstream sss( type );
      stringstream ssss( type3 );
      stringstream sssss( type4 );
      Data token;
      
      while ( ss >> token.str ) {
        
        sss >> token.type ;
        ssss >> token.type3 ;
        sssss >> token.type4 ;
        Data temp;
        if ( token.str[0] == '"' ) {
          
          for ( ; token.str[token.str.size()-1] != '"' ; ) {
            ss >> temp.str ;
            token.str = token.str + " " + temp.str ;
          } // for
          
          
        } // if
        
        token.type4 = type4 ;
        token.type3 = type3 ;
        mList->insert( mList->begin()+k, token ) ;
        k++ ;
        
      } // while

    } // else
    
  } // And()
  
  void Begin( int k ) {
    vector<string> bList;
    vector<string> typeList;
    vector<string> type3List;
    vector<string> type4List;
    string ans = "";
    string type = "";
    string type3 = "";
    string type4 = "";
    string temp = "";
    bool exp = false;
    int left = 1;
    int right = 0;
    int bracket = 0;
    int i = k+1 ;
    
    for ( ; i < mList->size() && left != right ; i++ ) { 
      if ( mList->at( i ).str == "(" ) {
        left++;
        bracket++;
        exp = true;
        temp += "( ";
        type += "left_bracket ";
        type3 += mList->at( i ).type3 + " ";
        type4 += mList->at( i ).type4 + " ";
      } // if
      else if ( mList->at( i ).str == ")" ) {
        right++;
        bracket--;
        if ( temp[temp.size()-1] == ' ' ) temp.erase( temp.end()-1 ); // �R�� )�e���Ů� 
        temp += " )";
        type += " right_bracket";  
        type3 += " " + mList->at( i ).type3;    
        type4 += " " + mList->at( i ).type4;        
        if ( right != left - 1 ) {
          temp += " ";
          type += " ";
          type3 += " ";
          type4 += " ";
        } // if  
      } // else if
      
      else if ( exp ) {
        temp += mList->at( i ).str;
        temp += " "; 
        type += mList->at( i ).type;
        type += " ";  
        type3 += mList->at( i ).type3 + " ";
        type4 += mList->at( i ).type4 + " "; 
      } // else if (cons 1 3)
      
      else if ( !exp ) {
        temp += mList->at( i ).str;
        type += mList->at( i ).type;
        type3 = mList->at( i ).type3;
        type4 = mList->at( i ).type4;
      } // else if  a
      
      
      if ( bracket == 0 || bracket == -1 ) {
        bList.push_back( temp ); 
        typeList.push_back( type );
        type3List.push_back( type3 );
        type4List.push_back( type4 );
        temp = "", type = "" ;
        type3 = "", type4 = "" ;
      } // if
      
    } // for  �s����   
    
    if ( bList.size() == 1 ) {
      string lastElement = bList[0] ; // 3 4 5
      stringstream iss( lastElement );
      string lastNum;
      while ( iss >> lastNum ) ;
      ans = lastNum ;
      type = mList->at( i ).type;
      type3 = mList->at( i ).type3;
      type4 = mList->at( i ).type4;
    } // if (begin 3 4 5) ��5 
    
    else {
      ans = bList[bList.size()-2];
      type = typeList[typeList.size()-2];
      type3 = type3List[type3List.size()-2];
      type4 = type4List[type4List.size()-2];
      
    } // else  (begin (3 4) (5 6) )  ��(5 6) 
    
    // erase
    bracket = 1 ;
    mList->erase( mList->begin() + k-1 );
    k-- ; 
    for ( ; bracket != 0 ; ) {
      if ( mList->at( k ).str == ")" ) bracket--;
      if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++;
      
      mList->erase( mList->begin() + k );

    } // for
    
    // insert

    stringstream ss( ans );
    stringstream sss( type );
    stringstream ssss( type3 );
    stringstream sssss( type4 );
    Data token;


    while ( ss >> token.str ) {
      sss >> token.type ;
      ssss >> token.type3 ;
      ssss >> token.type4 ;
      
      Data temp;
      if ( token.str[0] == '"' ) {
        
        for ( ; token.str[token.str.size()-1] != '"' ; ) {
          ss >> temp.str ;
          token.str = token.str + " " + temp.str ;
        } // for
        
        
      } // if
      
      mList->insert( mList->begin()+k, token );
      k++; 
    } // while    
    
      
     
  } // Begin()
  
  void If( int k ) {
    vector<string> ifList;
    vector<string> typeList;
    vector<string> type3List;
    vector<string> type4List;
    string ans = "";
    string type = "";
    string type3 = "" ;
    string type4 = "" ;
    string temp = "";
    bool exp = false;
    int left = 1;
    int right = 0;
    int bracket = 0;
    int i = k+1 ;
    
    for ( ; i < mList->size() && left != right ; i++ ) { 
      if ( mList->at( i ).str == "(" ) {
        left++;
        bracket++;
        exp = true;
        temp += "( ";
        type += "left_bracket ";
        type3 += mList->at( i ).type3 + " ";
        type4 += mList->at( i ).type4 + " ";
      } // if
      else if ( mList->at( i ).str == ")" ) {
        right++;
        bracket--;
        if ( temp[temp.size()-1] == ' ' ) temp.erase( temp.end()-1 ); // �R�� )�e���Ů� 
        temp += " )";
        type += " right_bracket";        
        if ( right != left - 1 ) {
          temp += " ";
          type += " ";
          type3 += " " + mList->at( i ).type3 ;
          type4 += " " + mList->at( i ).type4 ;
        } // if  
      } // else if
      
      else if ( exp ) {
        temp += mList->at( i ).str;
        temp += " "; 
        type += mList->at( i ).type;
        type += " ";    
        type3 += mList->at( i ).type3 + " ";
        type4 += mList->at( i ).type4 + " ";     
      } // else if (cons 1 3)
      
      else if ( !exp ) {
        temp += mList->at( i ).str;
        type += mList->at( i ).type;
        type3 += mList->at( i ).type3 ;
        type4 += mList->at( i ).type4 ; 
      } // else if  a
      
      
      if ( bracket == 0 ) {
        ifList.push_back( temp ); 
        typeList.push_back( type );
        type3List.push_back( type3 );
        type4List.push_back( type4 );
        temp = "", type = "" ;
        type3 = "", type4 = "" ;
      } // if
      
    } // for  �s���� 
     
     
    if ( ifList.size() == 3 ) {
      if ( ifList[0] == "nil" ) {
        ans = ifList[2];
        type = typeList[2];
        type3 = type3List[2];
        type4 = type4List[2];
      } // if   
      
      else {
        ans = ifList[1];
        type = typeList[1]; 
        type3 = type3List[1];
        type4 = type4List[1];
      } // else
      
    } // if ��nil���� 
    
    else if ( ifList.size() == 2 ) {
      if ( ifList[0] == "nil" ) {
        ans = "error";
        type = "error";
      } // if 
      else {
        ans = ifList[1];
        type = typeList[1];
        type3 = type3List[1];
        type4 = type4List[1];  
      } // else
    } // else if �Snil���� 
    

    // erase
    bracket = 1 ;
    mList->erase( mList->begin() + k-1 );
    k-- ; 
    for ( ; bracket != 0 ; ) {
      if ( mList->at( k ).str == ")" ) bracket--;
      if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++;
      
      mList->erase( mList->begin() + k );

    } // for
    
    // insert

    stringstream ss( ans );
    stringstream sss( type );
    stringstream ssss( type3 );
    stringstream sssss( type4 );
    Data token;


    while ( ss >> token.str ) {
      sss >> token.type ;
      ssss >> token.type3 ;
      sssss >> token.type4 ;
      
      Data temp;
      if ( token.str[0] == '"' ) {
        
        for ( ; token.str[token.str.size()-1] != '"' ; ) {
          ss >> temp.str ;
          token.str = token.str + " " + temp.str ;
        } // for
        
        
      } // if
      
      mList->insert( mList->begin()+k, token );
      k++; 
    } // while

    
    
  } // If()  
  
  void Cond( int k ) {
    // ========p1�O1�Ae1�O0============ // 
    int barcket = 0 ;
    int leftBarck = 0;
    int object = -1;
    int p1 = 0;
    int startCh = k-1;
    Data temp;
    bool hasBeenCheck = true; // ��@��()��p1�w�g�ˬd�L���Dtrue�A���ֶi�J�U�@��p1�A��L�O�� 
    bool isElse = false ;
    bool startIsbar = true; // �ѨM�i�j��@�}�l�N�O�A������A++�����D 
    bool isExp = false;
    bool thisExpIsNil = false;
    bool isbreak = false ;
    string ans ;
    string type;
    string type3;
    string type4;
    

    
    k++; // (cond here
    
    if ( mList->at( k ).str == "(" ) { 
      barcket++;
      k++;
      hasBeenCheck = false;


    
      for ( ; !isbreak && 
            ( mList->at( k ).str == "nil" || mList->at( k ).str == "(" 
              || mList->at( k ).str == "\'" ) ; k++ ) {
        // if ( !thisExpIsNil ) cout << "xx" << k << " " << mList->at( k ).str << endl ;
        mcondList->clear(); // ��l�� 
        if ( mList->at( k ).str == "(" ) barcket++;
        else if ( mList->at( k ).str == ")" ) barcket--;
        
        

        if ( !hasBeenCheck && mList->at( k ).str == "\'" ) {
          // hasBeenCheck = true;
          gposition = k ; 
                    
          DoQuoteForCond( mList->at( gposition ).str, gposition, 1 );
          if ( mList->at( k ).str == "(" ) barcket++;
          else if ( mList->at( k ).str == ")" ) barcket--;          
        } // if
        
        if ( !hasBeenCheck && barcket != 1 && mList->at( k ).str == "(" ) {
          
          hasBeenCheck = true;
          isExp = true;
          // ==break����O�]����A��F)��A�i�J�j��~�|barcket--�A�o�˷|�~�򰵬O���|���X==//
          
          
          for ( ; !isbreak && barcket != 1 ; k++ ) {
            object++;
            
            mcondList->push_back( mList->at( k ) ) ;
            if ( mList->at( k ).str == "(" && !startIsbar ) barcket++;
            else if ( mList->at( k ).str == ")" ) barcket--;
            startIsbar = false; 
            if ( barcket == 1 && mList->at( k ).str == ")" ) {
              isbreak = true;  
              k-- ;
            } // if     
                  
          } // for ���F�����j�A��n���j���F��s�i�s��List�A�åB�R��mList�̪��o�ǪF�� 
          
          isbreak = false ;
          startIsbar = true; // ��l�� 
          k = k - object;             
        
                        
          MainForCond( k-2 ) ;   
   

        } // if (cond ((...) e1) 

  
        if ( mList->at( k ).str == "nil" ) {
          k--; // �t��k!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
          while ( barcket != 0 ) {
            if ( mList->at( k ).str == "(" && !startIsbar ) barcket++;  
            else if ( mList->at( k ).str == ")" ) barcket--;
            startIsbar = false; 
            mList->erase( mList->begin()+k );
          } // while
          
          thisExpIsNil = true;
          barcket++; 
          // �n�٤@��barcket����� (cond (nil 8) (3 (+ 7 8)))�Ak�O(�A���ݷ|����k++  
        } // if (cond (nil...)(...) )
        // ====�J��nil�A������էR��==== //  
        

        // cout << mList->at( k ).str << " "<<k << "xxx"<<endl;
          

        if ( barcket == 1 && mList->at( k ).str == "(" ) {          
          hasBeenCheck = false; // �U�@�� k�Op1 
          leftBarck = k; // �аO�ۥثe�����A����m 
        } // if

        // cout << mList->at( leftBarck+1 ).str<< endl;
        
              
        /*
        if ( mList->at( leftBarck+1 ).str == "else" ) {
          // cout << "ccc" << mList->at( leftBarck+1 ).str << " " << leftBarck+1;
          isElse = true ;
        } // if
        */
        startIsbar = true; // ��l�� 
        hasBeenCheck = false; // ��l�� 
        isExp = false; // ��l��       
        object = -1; // ��l��   
        // cout << k <<" "<< thisExpIsNil << " "<<barcket<<" "<< mList->at( k ).str<<endl;                
        
        if ( !thisExpIsNil ) {
          isbreak = true;
          k-- ;
        } // if
              
        thisExpIsNil = false; 
        // ��ڽT�w�ڤ��O�]��nil��էR���A��ܧڬO�Dnil�F�A�i�H����break�A�H�K�v�Te1 
        
      } // for 
      
      isbreak = false;  
      // =====================�W���u�O�b�B�zp1========================= // 
      // =====================�X�Ӫ�p1�i��|�O(<78)�o�تF��============ // 
      // ======================�B(<78)�|���mList��k��m================ // 
      /*
          for( int n = 0 ; n <mList->size() ; n ++ )
            cout << mList->at(n).str << " " ; 
          cout << endl;   
          
          for( int n = 0 ; n <mcondList->size() ; n ++ )
            cout << mcondList[n].str << " " ; 
          cout << endl;   
      */
 

      if ( mList->at( k ).str == "(" ) {
        for ( int q = 0 ; q < mcondList->size() ; q++ ) {
          if ( mcondList->at( q ).str == mList->at( k ).str ) mList->erase( mList->begin()+k ) ;
        } // for
        
      } // if  P1�Oexp 
      else {
        mList->erase( mList->begin()+k ) ;
      } // else

 

      // =====�W���A��p1�qmList���R���A�W�ũt��k =====//
      mcondList->clear();
      /*
      if ( isElse ) {
        leftBarck++;
        if ( mList->at( leftBarck ).str == "else" ) 
          mList->erase( mList->begin()+leftBarck ) ; // �R��p1��else
        if ( mList->at( leftBarck ).str == "(" ) {
          for (  ; !isbreak && barcket != 0 ; leftBarck++ ) {         
            if ( mList->at( leftBarck ).str == "(" ) barcket++;
            else if ( mList->at( leftBarck ).str == ")" ) barcket--; 
            cout << barcket << mList->at( leftBarck ).str << endl; 
          
            temp.str += mList->at( leftBarck ).str;
            temp.str += " "; 
            temp.type += mList->at( leftBarck ).type;  
            temp.type += " ";
            if ( barcket == 1 && mList->at( leftBarck+1 ).str == ")" ) {
              isbreak = true ;        
              leftBarck-- ;
            } // if    
          } // for ���]���X�ӬO(7)�n��(,7,)���s�i mList->at( k ).str      
          cout <<   temp.str;
          
          isbreak = false ;
        } // if
        
        else if ( mList->at( leftBarck ).str != "(" ) {
          temp.str = mList->at( leftBarck ).str;
          temp.type = mList->at( leftBarck ).type;         
        } // else if
      } // if
      */
      // cout << barcket << "" << k <<endl;

      
      // k = 3; // �W�t��A�ڪ���é�@��e1���Y���O�ĤT�� 

          
      for ( ; !isbreak && barcket != 0 && !isElse ; k++ ) {
        
        if ( mList->at( k ).str == "(" ) barcket++;
        else if ( mList->at( k ).str == ")" ) barcket--;

        if ( mList->at( k ).str == ")" && barcket == 0 ) {
          isbreak = true;
          k-- ;
        } // if
        else {
          // �R��
          if ( hasBeenCheck ) {           
  
            if ( mList->at( p1 ).str == "(" ) {
              bool isbreak0 = false ;
              while ( !isbreak0 && barcket != 1 ) {
                if ( mList->at( k ).str == "(" && !startIsbar ) barcket++;
                else if ( mList->at( k ).str == ")" ) barcket--;
                if ( barcket == 1 ) {
                  isbreak0 = true ;
                  k = p1;
                } // if
                else {
                  startIsbar = false;
                  mList->erase( mList->begin()+p1 ) ;
                } // else
                
              } // while
              
            } // if  P1�Oexp 
            else {
              mList->erase( mList->begin()+p1 ) ;
            } // else
            
            temp.str = "";
            temp.type = "" ;
            k = p1;
            // k = 3; // �W�t��A�ڪ���é�@��e1���Y���O�ĤT�� 
          } // if 
          // �R��
 

          if ( mList->at( k ).str == "\'" ) {
            gposition = k ;
            DoQuoteForCond( mList->at( gposition ).str, gposition, 0 );            
            if ( mList->at( k ).str == "(" ) barcket++;
            else if ( mList->at( k ).str == ")" ) barcket--;            
          } // if 
          /*
          for( int n = 0 ; n <mList->size() ; n ++ )
            cout << mList->at(n).str << " " ; 
          cout << endl; 
          */
          if ( mList->at( k ).str == "(" ) {
            if ( mList->at( k ).str == "(" ) barcket++;
            else if ( mList->at( k ).str == ")" ) barcket--;
            isExp = true;
            bool isbreak2 = false ;
            for ( ; !isbreak2 && barcket != 1 ; k++ ) {
              object++;
              if ( mList->at( k ).str == "(" && !startIsbar ) barcket++;
              else if ( mList->at( k ).str == ")" ) barcket--;              
              if ( barcket == 1 && mList->at( k ).str == ")" ) {
                isbreak2 = true ;
                k-- ;
              } // if
              else {     
                if ( mList->at( k ).str == "\'" ) {
                  gposition = k;
                  DoQuoteForCond( mList->at( gposition ).str, gposition, 0 );  
                } // if          
  
                mcondList->push_back( mList->at( k ) ) ;
              } // else
              
              startIsbar = false; 
            

            } // for ���F�����j�A��n���j���F��s�i�s��List�A�åB�R��mList�̪��o�ǪF�� 
            
          k = k - object;   
          
          /*
          for( int n = 0 ; n < mcondList->size() ; n ++ )
            cout << mcondList->at(n).str << " " ; 
          cout << endl; 
          cout << k << endl;         
          */
        
          
            MainForCond( k-2 ) ;     
                        
             
          } // if  

       
          // cout << k << " " << barcket ;
          if ( mList->at( k ).str == "(" ) {
            int y = k;
            p1 = k;
            bool isbreak2 = false ;
            for (  ; !isbreak2 && barcket != 0 ; y++ ) {
              if ( mList->at( y ).str == "(" ) barcket++;
              else if ( mList->at( y ).str == ")" ) barcket--;  
              temp.str += mList->at( y ).str;
              temp.str += " "; 
              temp.type += mList->at( y ).type;  
              temp.type += " ";               
              if ( barcket == 1 ) {
                isbreak2 = true ;
                y-- ;
              } // if 
              
                       
            } // for ���]���X�ӬO(7)�n��(,7,)���s�i mList->at( k ).str        
 
            k = y;
          } // if
        
          else if ( mList->at( k ).str != "(" ) {
            p1 = k;
            temp.str = mList->at( k ).str;
            temp.type = mList->at( k ).type;           
          } // else if


          /*
          if ( !isExp && mList->at( 3 ).str != "(") {
            temp.str = mList->at( 3 ).str;
            temp.type = mList->at( 3 ).type; 
            k = 3;        
          } // else
          */
          hasBeenCheck = true; // �p�G�o��e1���O�̫�@�ӡA�n�R��       
          
          startIsbar = true; // ��l�� 
          isExp = false; // ��l��        
          object = -1; // ��l�� 
          mcondList->clear(); // ��l�� 
        }  // else       
      } // for


    } // if ( cond (...) )
    
    
    else if ( mList->at( k ).str == "\'" ) {
      mcondList->clear(); // ��l�� 
      gposition = k;
      DoQuoteForCond( mList->at( gposition ).str, gposition, 1 ); 
      // =============(cond 'x) �R��'x�A���� (quote x)=============== 
  
              
      if ( mList->at( k ).str == "(" ) {
        barcket++;
        k++; // ( cond ( quote (+ 1 2 3) ))�� quote
        mList->erase( mList->begin()+k ); // �R��quote�A��(cond ((+ 1 2 3))) 
        if ( mList->at( k ).str == "(" ) barcket++;
        else if ( mList->at( k ).str == ")" ) barcket--;        
        
  
        if ( mList->at( k ).str == "(" ) {
          isExp = true;
          bool isbreak2 = false ;
          for ( ; !isbreak2 && barcket != 1 ; k++ ) {
            object++;
            mcondList->push_back( mList->at( k ) ) ;
            if ( mList->at( k ).str == "(" && !startIsbar ) barcket++;
            else if ( mList->at( k ).str == ")" ) barcket--;
            startIsbar = false; 
            if ( barcket == 1 && mList->at( k ).str == ")" ) {
              isbreak2 = true ;
              k-- ;
            } // if             
          } // for ���F�����j�A��n���j���F��s�i�s��List�A�åB�R��mList�̪��o�ǪF�� 
          
           k = k - object;  
            
          MainForCond( k - 2 ) ;        
          
        } // if  
  
        /*
        if ( mList->at( k ).str == "(" ) {
          int y = k;
          bool isbreak2 = false ;
          for (  ; !isbreak2 && barcket != 0 ; y++ ) {
            if ( mList->at( y ).str == "(" ) barcket++;
            else if ( mList->at( y ).str == ")" ) barcket--;  
            temp.str += mList->at( y ).str;
            temp.str += " "; 
            temp.type += mList->at( y ).type;  
            temp.type += " ";
            if ( barcket == 1 ) {
              isbreak2 = true ;
              y-- ;
            } // if                        
          } // for ���]���X�ӬO(7)�n��(,7,)���s�i mList->at( k ).str        
      
          k = y;
        } // if
        */
        
        if ( mList->at( k ).str != "(" ) {
          temp.str = mList->at( k ).str;
          temp.type = mList->at( k ).type;           
        } // else if
      
        /*
        if ( !isExp && mList->at( 3 ).str != "(") {
          temp.str = mList->at( 3 ).str;
          temp.type = mList->at( 3 ).type; 
          k = 3;        
        } // else
        */
        hasBeenCheck = true; // �p�G�o��e1���O�̫�@�ӡA�n�R��       
        
        
        
        startIsbar = true; // ��l�� 
        isExp = false; // ��l��        
        object = -1; // ��l�� 
                
        
        /*
        for ( ; barcket != 0 ; k++ ) 
          if (  mList->at( k ).str == ")" ) barcket--;  
        
        if ( mList->at( k-1 ).str == "?" ) {
          temp.str = "< procedure " + mList->at( k-1 ).str + " >" ; 
          temp.type = "?" ;
        } // if  e1 : function�B�z procedure������ 
        
        else {
          temp.str = mList->at( k-1 ).str ; 
          temp.type = mList->at( k-1 ).type;
        } // else e1
        
        } // if �z�פW���i�ण�O�A��
        */
      } // if 
      
    } // else if ( cond '3 ) 
     
    // erase  
    k = 1;
    int bracket = 1 ;
    mList->erase( mList->begin() + k-1 );
    k-- ; 
    for ( ; bracket != 0 ; ) {
      if ( mList->at( k ).str == ")" ) bracket--;
      if ( mList->at( k ).str == "(" ) bracket++;
      
      mList->erase( mList->begin() + k );

    } // for

    if ( mList->size() == 0 ) {
      MList_fuc();
    } // if

    // insert

    stringstream ss( temp.str );
    stringstream sss( temp.type );
    temp.str = ""; // ��l�� 
    temp.type = "" ; // ��l�� 
    Data token;
    while ( ss >> token.str ) {
      
      sss >> token.type ;
                
      Data temp;
      if ( token.str[0] == '"' ) {
        
        for ( ; token.str[token.str.size()-1] != '"' ; ) {
          ss >> temp.str ;
          token.str = token.str + " " + temp.str ;
        } // for
        
        
      } // if
      
      token.type4 = type4 ;
      token.type3 = type3 ;
      mList->insert( mList->begin()+k, token ) ;

      k++ ;

      
    } // while
       
  } // Cond()
  
  void DoQuoteForCond( string temp, int gposition, int p1_or_e1 ) { 
    Data qq ;
    if ( p1_or_e1 == 1 ) {
      if ( ( gposition-1 < mList->size() ) && ( mList->at( gposition-1 ).str == "(" ) ) {
        mList->erase( mList->begin()+gposition ); // �R' 
      } // if
      
      else {
        
        if ( mList->size() != 0 && gposition != 0 ) {
          mList->erase( mList->begin()+gposition ); // �R'
          qq.str = "quote";
          qq.type = "quote";        
          mList->insert( mList->begin()+gposition, qq ) ;
          qq.str = "";
          qq.type = "";
          qq.str = "(";
          qq.type = "left_bracket";        
          mList->insert( mList->begin()+gposition, qq ) ;
          qq.str = "";
          qq.type = "";        
          qq.str = ")";
          qq.type = "right_bracket";         
          mList->push_back( qq ) ;
        } // if
        
      } // else
    } // if
    else mList->erase( mList->begin()+gposition ); // �R' 
  
  } // DoQuoteForCond()
  
  void String_compare( int k ) {
    
    int i = k+1 ;
    bool isRight = true ;
    
    // cout << mList->at( i ).str << "_" << mList->at( i+1 ).str << "_" ;
    for ( ; mList->at( i+1 ).str != ")" ; i++ ) {
        
      if ( mList->at( k ).str == "string>?" ) {
        if ( mList->at( i ).str <= mList->at( i+1 ).str ) isRight = false ;
      } // if  
      else if ( mList->at( k ).str == "string<?" ) {
        if ( mList->at( i ).str >= mList->at( i+1 ).str ) isRight = false ;
      } // else if
      else if ( mList->at( k ).str == "string=?" ) {
        if ( mList->at( i ).str != mList->at( i+1 ).str ) isRight = false ;
      } // else if

    } // for
    
    if ( isRight ) {
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (> �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (> .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
    } // if
    else {
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (> �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (> .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "nil", temp.type = "nil" ;
      mList->insert( mList->begin()+k, temp ) ;
    } // else
    
  } // String_compare()
  
  void String_append( int k ) {

    string tmp = "";
    string special = "";
    string type = "";

    for ( int i = k+1 ; i < mList->size() && mList->at( i ).str != ")" ; i++ ) {
      string temp = "";
      int a = 0 ;
      for ( int n = 0 ; n < mList->at( i ).str.size() ; n++ ) {
        if ( ( mList->at( i ).str[n] == '"' &&  ( n == 0 || n == mList->at( i ).str.size()-1 ) ) ) ; // ���n�s 
        else {
          temp.push_back( mList->at( i ).str[n] );
        } // else

      } // for
      
      special += temp;
      type = mList->at( i ).type ;
    } // for

    // erase
    int bracket = 1 ;
    mList->erase( mList->begin() + k-1 );
    k-- ; 
    for ( ; bracket != 0 ; ) {
      if ( mList->at( k ).str == ")" ) bracket--;
      if ( mList->at( k ).str == "(" || mList->at( k ).str == "'" ) bracket++;

      mList->erase( mList->begin() + k );
    } // for
    
    // insert
    special.insert( 0, "\"" ) ;
    special.push_back( '"' ) ;
    stringstream ss( special );
    stringstream sss( type );
    Data token;

    ss >> token.str ;

    Data temp;
    if ( token.str[0] == '"' ) {
      
      for ( ; token.str[token.str.size()-1] != '"' ; ) {
        ss >> temp.str ;
        token.str = token.str + " " + temp.str ;
      } // for
      
      
    } // if
    
    mList->insert( mList->begin()+k, token ) ;  

  } // String_append()
  
  void Compare( int k ) {
    
    int i = k+1 ;
    bool isRight = true ;
    
    
    for ( ; mList->at( i+1 ).str != ")" ; i++ ) {
        
      stringstream ss1( mList->at( i ).str ) ;
      stringstream ss2( mList->at( i+1 ).str ) ;
      double num1, num2 ;
      ss1 >> num1 ;
      ss2 >> num2 ;
        
      if ( mList->at( k ).str == ">" ) {
        if ( num1 <= num2 ) isRight = false ;
      } // if  
      else if ( mList->at( k ).str == "<" ) {
        if ( num1 >= num2 ) isRight = false ;
      } // else if
      else if ( mList->at( k ).str == "=" ) {
        if ( num1 != num2 ) isRight = false ;
      } // else if
      else if ( mList->at( k ).str == ">=" ) {
        if ( num1 < num2 ) isRight = false ;
      } // else if
      else if ( mList->at( k ).str == "<=" ) {
        if ( num1 > num2 ) isRight = false ;
      } // else if 

    } // for
    

              
    if ( isRight ) {
      k = k - 1 ;
               
      mList->erase( mList->begin()+k ) ; // delete (> �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (> .....) �� )  
        
      } // while
      
      Data temp ;
      
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
      
    } // if
    else {
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (> �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (> .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "nil", temp.type = "nil" ;
      
      mList->insert( mList->begin()+k, temp ) ;
    } // else
    
  } // Compare()
  
  void Not( int k ) {
    
    if ( mList->at( k+1 ).str == "nil" && mList->at( k+2 ).str == ")" ) { // ���T���ܤ@�w�|�i  
    
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (symbol? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (symbol? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
    
    } // if
    else {
      
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (symbol? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (symbol? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "nil", temp.type = "nil" ;
      mList->insert( mList->begin()+k, temp ) ;
      
    } // else
    
  } // Not()
  
  void Caculate( int k ) { // ( + 3 5 6 )
  
    int haveDot = 0 ; // ���p���I(�ɹs�i��)
    vector<Data> cacul ; // �s�Ʀr�ǦC 
    bool isZero = false;
    int isInt = 0; // �P�_�O���O��� 
    double temp = 0; // �s�Jtree�� 
    double number = 0;
    stringstream oss;
    string sValue ; 

    bool isbreak = false ;
    for ( int i = k ; !isbreak && i < mList->size() ; i++ ) {
      if ( mList->at( i ).str == ")" ) isbreak = true;
      else cacul.push_back( mList->at( i ) ) ;
        
    } // for
    
    int n = 1; 
    for (  ; n < cacul.size() ; n++ ) {
      for ( int p = 0 ; p < cacul[n].str.size() ; p++ ) 
        if ( cacul[n].str[p] == '.' ) haveDot++ ;  // �P�_���L�p���I 
  
      stringstream ss( cacul[n].str );
      ss >> number;

      if ( number == ( int ) number && haveDot == 0 ) isInt++; 
      // �̫�n��isInt�ƶq���S������n���ƶq(�����Oint)�A�n��temp�qdouble->int 
      
      if ( n == 1 ) temp = number; // temp��l�Ȭ��Ĥ@�ӼƦr       
      else if ( cacul[0].str == "+" ) temp += number;
      else if ( cacul[0].str == "-" ) temp -= number;
      else if ( cacul[0].str == "*" ) temp *= number;
      else if ( cacul[0].str == "/" && !isZero ) {
        if ( number == 0 || number == 0.0 || number == 0.00 || number == 0.000 ) {
          temp = -1; // �ȮɦL-1�A����error 
          isZero = true; // ��0�A���X 
        } // if
        else temp /= number;
      } // else if
      
      haveDot = 0;
    } // for
    
        
    // erase
    int bracket = 1 ;
    mList->erase( mList->begin() + k-1 );
    k-- ; 
    for ( ; bracket != 0 ; ) {
      if ( mList->at( k ).str == ")" ) bracket--;
      if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++;
      
      mList->erase( mList->begin() + k );

    } // for
    
    // insert
    Data token;
    if ( isInt == n-1 ) {
      int intValue = ( int ) temp; // double->int
      stringstream ss;
      ss << intValue;
      token.str = ss.str();  // int->string
    } // if 
    
    else {
      stringstream ss;
      ss << fixed << setprecision( 3 ) << temp; // �վ� 
      token.str = ss.str();
    } // else 
        
    mList->insert( mList->begin()+k, token ) ;
 
    k++ ;
    cacul.clear();
    
  } // Caculate()
  
  void Eqv_is( int k ) {
    
    if ( mList->at( k+1 ).type == "sign" && mList->at( k+2 ).type == "sign" 
         && mList->at( k+1 ).str == mList->at( k+2 ).str && mList->at( k+1 ).type3 != "isDefine"
         && mList->at( k+3 ).str == ")" ) {
    
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (eqv? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (eqv? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
           
    } // if
    else if ( mList->at( k+1 ).str == "nil" && mList->at( k+2 ).str == "nil" 
              && mList->at( k+1 ).str == mList->at( k+2 ).str && mList->at( k+1 ).type3 != "isDefine"
              && mList->at( k+3 ).str == ")" ) {
           
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (eqv? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (eqv? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
           
    } // else if
    else if ( mList->at( k+1 ).str == "#t" && mList->at( k+2 ).str == "#t" 
              && mList->at( k+1 ).str == mList->at( k+2 ).str && mList->at( k+1 ).type3 != "isDefine"
              && mList->at( k+3 ).str == ")" ) {
           
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (eqv? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (eqv? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
           
    } // else if
    else { // �O�ۦP�ڷ�define���ܤ~��  
      
      bool isDefine = true, isbreak = false ;
      string compare1, compare2 ;
      string type4, type44 ;
      int bracket = 0 ;
      int i = k+1 ;
      
      for ( ; !isbreak && i < mList->size() ; i++ ) {
        // cout << "_"<<mList->at( i ).str<<"_";
        if ( mList->at( i ).str == "(" || mList->at( i ).str == "\'" ) bracket++ ;
        else if ( mList->at( i ).str == ")" ) bracket-- ;
        
        compare1 += mList->at( i ).str ;
        if ( bracket == 0 ) isbreak = true ;
        
        if ( mList->at( i ).type3 != "isDefine" ) isDefine = false ;
        type4 = mList->at( i ).type4 ;
         
      } // for
      
      bracket = 0 ;
      isbreak = false ;
      for ( ; !isbreak && i < mList->size() ; i++ ) {
        
        if ( mList->at( i ).str == "(" || mList->at( i ).str == "\'" ) bracket++ ;
        else if ( mList->at( i ).str == ")" ) bracket-- ;
        
        compare2 += mList->at( i ).str ;
        if ( bracket == 0 ) isbreak = true ;
        
        if ( mList->at( i ).type3 != "isDefine" ) isDefine = false ;
        type44 = mList->at( i ).type4 ;
        
      } // for

      if ( type4 != type44 ) isDefine = false ;
      // cout << "_" << type4<<"_"<<type44 << "_" ;
      if ( isDefine && compare1 == compare2 ) {
        k = k - 1 ;
        mList->erase( mList->begin()+k ) ; // delete (eqv? �� (  
        int bracket = 1 ;
        while ( bracket != 0 ) {
          
          if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
          else if ( mList->at( k ).str == ")" ) bracket-- ;
          
          mList->erase( mList->begin()+k ) ; // until delete (eqv? .....) �� )  
          
        } // while
        
        Data temp ;
        temp.str = "#t", temp.type = "#t" ;
        mList->insert( mList->begin()+k, temp ) ;
      } // if
      else {
        k = k - 1 ;
        mList->erase( mList->begin()+k ) ; // delete (eqv? �� (  
        int bracket = 1 ;
        while ( bracket != 0 ) {
          
          if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
          else if ( mList->at( k ).str == ")" ) bracket-- ;
          
          mList->erase( mList->begin()+k ) ; // until delete (eqv? .....) �� )  
          
        } // while
        
        Data temp ;
        temp.str = "nil", temp.type = "nil" ;
        mList->insert( mList->begin()+k, temp ) ;
      } // else
      
    } // else
    
  } // Eqv_is()
  
  void Equal_is( int k ) {
    
    string compare1, compare2 ;
    bool isbreak = false ;
    int bracket = 0 ;
    int i = k+1 ;
    
    for ( ; !isbreak && i < mList->size() ; i++ ) {
      
      if ( mList->at( i ).str == "(" || mList->at( i ).str == "\'" ) bracket++ ;
      else if ( mList->at( i ).str == ")" ) bracket-- ;
      
      compare1 += mList->at( i ).str ;
      if ( bracket == 0 ) isbreak = true ;
      
    } // for
    
    isbreak = false ;
    for ( ; !isbreak && i < mList->size() ; i++ ) {
      
      if ( mList->at( i ).str == "(" || mList->at( i ).str == "\'" ) bracket++ ;
      else if ( mList->at( i ).str == ")" ) bracket-- ;
      
      compare2 += mList->at( i ).str ;
      if ( bracket == 0 ) isbreak = true ;
      
    } // for
    
    if ( compare1 == compare2 ) {
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (equal? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (equal? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
    } // if
    else {
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (equal? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (equal? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "nil", temp.type = "nil" ;
      mList->insert( mList->begin()+k, temp ) ;
    } // else
    
  } // Equal_is()
  
  void List_is( int k ) {
    
    bool isIf = false ;
    if ( mList->at( k+1 ).str == "(" ) { 
      
      bool isList = true ;
      int bracket = 1 ;
      for ( int i = k+2 ; i < mList->size() && bracket != 0 ; i++ ) {  
        if ( mList->at( i ).str == "(" || mList->at( i ).str == "\'" ) bracket++ ;
        else if ( mList->at( i ).str == ")" ) bracket-- ;
        
        if ( mList->at( i ).str == "." ) isList = false ;
      } // for
      
      if ( isList ) {
        isIf = true ;
        k = k - 1 ;
        mList->erase( mList->begin()+k ) ; // delete (list? �� (  
        int bracket = 1 ;
        while ( bracket != 0 ) {
          
          if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
          else if ( mList->at( k ).str == ")" ) bracket-- ;
          
          mList->erase( mList->begin()+k ) ; // until delete (list? .....) �� )  
          
        } // while
        
        Data temp ;
        temp.str = "#t", temp.type = "#t" ;
        mList->insert( mList->begin()+k, temp ) ;
      } // if
            
    } // if
    
    if ( mList->at( k+1 ).str == "nil" && mList->at( k+2 ).str == ")" ) { // (list? nil)
      
      isIf = true ;
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (list? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (list? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
      
    } // if
    
    if ( !isIf ) {
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (list? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (list? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "nil", temp.type = "nil" ;
      mList->insert( mList->begin()+k, temp ) ;
    } // if
    
  } // List_is()
  
  void Symbol_is( int k ) {
    
    if ( mList->at( k+1 ).type == "symbol" && mList->at( k+2 ).str == ")" ) { // ���T���ܤ@�w�|�i  
    
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (symbol? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (symbol? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
    
    } // if
    else {
      
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (symbol? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (symbol? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "nil", temp.type = "nil" ;
      mList->insert( mList->begin()+k, temp ) ;
      
    } // else
    
  } // Symbol_is()
  
  void Boolean_is( int k ) {
    
    if ( ( mList->at( k+1 ).str == "#t" || mList->at( k+1 ).str == "nil" )
         && mList->at( k+2 ).str == ")" ) { // ���T���ܤ@�w�|�i  
    
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (boolean? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (boolean? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
    
    } // if
    else {
      
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (boolean? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (boolean? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "nil", temp.type = "nil" ;
      mList->insert( mList->begin()+k, temp ) ;
      
    } // else
    
  } // Boolean_is()
  
  void String_is( int k ) {
    
    if ( mList->at( k+1 ).type == "string" && mList->at( k+2 ).str == ")" ) { // ���T���ܤ@�w�|�i  
    
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (string? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (string? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
    
    } // if
    else {
      
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (string? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (string? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "nil", temp.type = "nil" ;
      mList->insert( mList->begin()+k, temp ) ;
      
    } // else
    
  } // String_is()
  
  void Real_Number_is( int k ) {
    
    if ( mList->at( k+1 ).type == "sign" && mList->at( k+2 ).str == ")" ) { // ���T���ܤ@�w�|�i  
    
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (Real_Number? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (Real_Number? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
    
    } // if
    else {
      
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (Real_Number? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (Real_Number? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "nil", temp.type = "nil" ;
      mList->insert( mList->begin()+k, temp ) ;
      
    } // else
    
  } // Real_Number_is()
  
  void Atom_is( int k ) {
    
    if ( ( mList->at( k+1 ).type == "sign" || mList->at( k+1 ).type == "string" 
           || mList->at( k+1 ).str == "#t" || mList->at( k+1 ).str == "nil" ) 
         && mList->at( k+2 ).str == ")" ) { // ���T���ܤ@�w�|�i  
           
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (atom? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (atom? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
    
    } // if
    else {
      
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (atom? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (atom? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "nil", temp.type = "nil" ;
      mList->insert( mList->begin()+k, temp ) ;
      
    } // else
    
  } // Atom_is()
  
  void Integer_is( int k ) {
    
    bool isIf = false ;
    if ( mList->at( k+1 ).type == "sign" && mList->at( k+2 ).str == ")" ) { // ���T���ܤ@�w�|�i  
      
      bool isInteger = true ;
      for ( int i = 0 ; i < mList->at( k+1 ).str.size() ; i++ ) {   
        if ( mList->at( k+1 ).str[i] == '.' ) isInteger = false ;
      } // for
      
      if ( isInteger ) {
        isIf = true ;
        k = k - 1 ;
        mList->erase( mList->begin()+k ) ; // delete (interger? �� (  
        int bracket = 1 ;
        while ( bracket != 0 ) {
          
          if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
          else if ( mList->at( k ).str == ")" ) bracket-- ;
          
          mList->erase( mList->begin()+k ) ; // until delete (interger? .....) �� )  
          
        } // while
        
        Data temp ;
        temp.str = "#t", temp.type = "#t" ;
        mList->insert( mList->begin()+k, temp ) ;
      } // if
            
    } // if
    
    if ( !isIf ) {
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (interger? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (interger? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "nil", temp.type = "nil" ;
      mList->insert( mList->begin()+k, temp ) ;
    } // if
    
  } // Integer_is()
  
  void Pair_is( int k ) {
    
    if ( mList->at( k+1 ).str == "(" ) { // ���T���ܤ@�w�|�i  
  
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (pair? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (pair? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
      
    } // if
    else { // pair?�᭱���O ( 
        
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (pair? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (pair? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "nil", temp.type = "nil" ;
      mList->insert( mList->begin()+k, temp ) ;
        
    } // else 
    
  } // Pair_is()
  
  void Null_is( int k ) {
    
    if ( mList->at( k+1 ).str == "nil" && mList->at( k+2 ).str == ")" ) { // (null? nil )
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (pair? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (pair? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "#t", temp.type = "#t" ;
      mList->insert( mList->begin()+k, temp ) ;
    } // if
    else {
      k = k - 1 ;
      mList->erase( mList->begin()+k ) ; // delete (pair? �� (  
      int bracket = 1 ;
      while ( bracket != 0 ) {
        
        if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        mList->erase( mList->begin()+k ) ; // until delete (pair? .....) �� )  
        
      } // while
      
      Data temp ;
      temp.str = "nil", temp.type = "nil" ;
      mList->insert( mList->begin()+k, temp ) ;
    } // else
    
  } // Null_is()
  
  void Car( int k ) {  
    string special = "";
    string type = "" ;
    string type4 = "" ;
    string type3 = "" ;
    bool hasIn = false;
    
    if ( mList->at( k+1 ).str == "(" ) {
      
      int j = k+2, bracket = 1 ;
      bool isbreak = false ;
      for ( ; !isbreak ; j++ ) {
        
        if ( mList->at( j ).str == "(" || mList->at( j ).str == "\'" ) bracket++ ;
        else if ( mList->at( j ).str == ")" ) bracket-- ;
        
        if ( bracket >= 1 ) {
          special = special + mList->at( j ).str + " " ;
          type = type + mList->at( j ).type + " " ;
          if ( bracket == 1 ) isbreak = true ;
        } // if
        else isbreak = true ;

      } // for

    } // if
    else {
      special = mList->at( k+1 ).str ;
      type += mList->at( k+1 ).type ;
      type += " " ;
      type4 = mList->at( k+1 ).type4 ;
      type3 = mList->at( k+1 ).type3 ;
    } // else  (car a)
    
    // erase
    int bracket = 1 ;
    mList->erase( mList->begin() + k-1 );
    k-- ; 
    for ( ; bracket != 0 ; ) {
      if ( mList->at( k ).str == ")" ) bracket--;
      if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++;
      
      mList->erase( mList->begin() + k );

    } // for

    // insert
    stringstream ss( special );
    stringstream sss( type );
    Data token;
    
    while ( ss >> token.str ) {
      
      sss >> token.type ;
      Data temp;
      if ( token.str[0] == '"' ) {
        
        for ( ; token.str[token.str.size()-1] != '"' ; ) {
          ss >> temp.str ;
          token.str = token.str + " " + temp.str ;
        } // for
        
        
      } // if
      
      token.type4 = type4 ;
      token.type3 = type3 ;
      mList->insert( mList->begin()+k, token ) ;
      k++ ;
      
    } // while
    
  } // Car()  
  
  void Cdr( int k ) { 
  
    string special = "";
    string type = "" ;
    string type4 = "" ;
    string type3 = "" ;
    bool hasIn = false;
    
    if ( mList->at( k+1 ).str == "(" ) { // ���T���ܤ@�w�|�i  
      
      int j = k+2, bracket = 1 ;
      bool isbreak = false ;
      for ( ; !isbreak ; ) {
        
        if ( mList->at( j ).str == "(" || mList->at( j ).str == "\'" ) bracket++ ;
        else if ( mList->at( j ).str == ")" ) bracket-- ;

        if ( bracket == 1 ) {
          mList->erase( mList->begin()+j ) ;
          isbreak = true ;
        } // if
        else if ( bracket >= 1 ) {
          mList->erase( mList->begin()+j ) ;
          if ( bracket == 1 ) isbreak = true ;
        } // else if
        else isbreak = true ;

      } // for
      
      j-- ; // �^��(  
      
      if ( mList->at( j ).str == "(" ) { // ���T���ܤ@�w�|�i  
        
        j++ ;
        type4 = mList->at( j ).type4 ;
        // cout << "x" << type4<< "x" ;
        type3 = mList->at( j ).type3 ;
        
        if ( mList->at( j ).str == "." ) {
          special = ". ( " ;
          type = "dot left_bracket " ;
          j++ ;
        } // if
        else {
          special = "( " ;
          type = "left_bracket " ;
        } // else
    
        bracket = 1 ;
        isbreak = false ;
        for ( ; !isbreak ; j++ ) {
          
          if ( mList->at( j ).str == "(" || mList->at( j ).str == "\'" ) bracket++ ;
          else if ( mList->at( j ).str == ")" ) bracket-- ;
          
          if ( bracket >= 0 ) {
            special = special + mList->at( j ).str + " " ;
            type = type + mList->at( j ).type + " " ;
            if ( bracket == 0 ) isbreak = true ;
          } // if
          else isbreak = true ;
  
        } // for
        
      } // if

    } // if
    else ; // error
    
    // erase
    int bracket = 1 ;
    mList->erase( mList->begin() + k-1 );
    k-- ; 
    for ( ; bracket != 0 ; ) {
      if ( mList->at( k ).str == ")" ) bracket--;
      if ( mList->at( k ).str == "(" || mList->at( k ).str == "\'" ) bracket++;
      
      mList->erase( mList->begin() + k );

    } // for

    // insert
    stringstream ss( special );
    stringstream sss( type );
    Data token;
    
    while ( ss >> token.str ) {
      
      sss >> token.type ;
      Data temp;
      if ( token.str[0] == '"' ) {
        
        for ( ; token.str[token.str.size()-1] != '"' ; ) {
          ss >> temp.str ;
          token.str = token.str + " " + temp.str ;
        } // for
        
        
      } // if
      
      token.type4 = type4 ;
      token.type3 = type3 ;
      
      mList->insert( mList->begin()+k, token ) ;
      k++ ;
      
    } // while
    
    DeleteDotBracketAndTNil() ;
    
  } // Cdr()  

  int EndOfCheckDefine( int i ) {
    
    bool isbreak = false ;
    int bracket = 1 ;
    
    while ( i < mList->size() && !isbreak ) {
      
      if ( mList->at( i ).str == "'" ) bracket++ ;
      else if ( mList->at( i ).str == "(" ) bracket++ ;
      else if ( mList->at( i ).str == ")" ) bracket-- ;
      
      if ( bracket == 0 ) isbreak = true ;
      
      i++ ;
    } // while
    
    // i-- ;
    return i ;
    
  } // EndOfCheckDefine()

  void CheckDefine( int i, int end ) {  // �̭����F�観�S���Qdefine��  �����ܴN��define�����e�л\���L  
    
    bool isbreak = false ;
    /*
    if ( !ghaveDoQuote && mList->at( i+1 ).str == "'" ) {
      mList->erase( mList->begin()+i+1 ) ;
      int j = i, bracket = 0 ;
      bool isbreak = false ;
      for ( ; !isbreak && j < mList->size() ; j++ ) {  // �@����'������) ������  
        if ( mList->at( j ).str == "\'" ) bracket++ ;
        else if ( mList->at( j ).str == "(" ) bracket++ ;
        else if ( mList->at( j ).str == ")" ) bracket-- ;
        else {

          if ( bracket == 0 ) {
            if ( j+1 < mList->size() && mList[j+1].str == ")" ) isbreak = true ;
          } // if
          
        } // else
        
        if ( !isbreak ) {
          
          if ( bracket == 0 && mList->at( j ).str != "\'" ) {
            isbreak = true ;
          } // if
          
        } // if
        
      } // for
        
    } // if
    */
    /*
    if ( i-1 >= 0 && mList->at( i ).str == "cond" && mList[i-1].str == "(" ) {
      gisCond = i ;
    } // if
    */
    
    if ( gisCond == -2 ) ;
    else if ( gisCond != -1 ) {
      
      i = gisCond+1 ;
      int iselse = -1, bracket = 0, k = i ;
      
      while ( k < mList->size() && !isbreak ) {
      
        if ( mList->at( k ).str == "(" && bracket == 0 && mList->at( k+1 ).str == "else" ) {
          iselse = k+1 ;
          
        } // if
          
        else if ( mList->at( k ).str == "(" && bracket == 0 && mList->at( k+1 ).str != "else" ) iselse = -1 ;
      
        // if ( mList->at( k ).str == "'" ) bracket++ ;
        if ( mList->at( k ).str == "(" ) bracket++ ;
        else if ( mList->at( k ).str == ")" ) bracket-- ;
        
        if ( bracket == 0 && mList->at( k+1 ).str == ")" ) isbreak = true ;
        
        k++ ;
      } // while
      
      isbreak = false ;
      // cout << iselse << "xx" ;
      for ( ; !isbreak && i < end ; i++ ) {
      
        if ( iselse == i ) i++ ;
        // if ( mList->at( i ).str == "\'" ) isbreak = true ;
        for ( int j = 0 ; !isbreak && j < gdefine.size() ; j++ ) {
          
          if ( mList->at( i ).str == gdefine[j]->temp ) {
            stringstream ss( gdefine[j]->object );
            stringstream sss( gdefine[j]->type );
            Data token;
            mList->erase( mList->begin()+i ) ;
            if ( iselse >= i ) iselse -= 1 ;
            end-- ;
            
            while ( ss >> token.str ) {
              sss >> token.type ;
              
                            
              Data temp;
              if ( token.str[0] == '"' ) {
                
                for ( ; token.str[token.str.size()-1] != '"' ; ) {
                  ss >> temp.str ;
                  token.str = token.str + " " + temp.str ;
                } // for
                
                
              } // if
              
              token.type3 = "isDefine" ;
              token.type4 = gdefine[j]->type4 ;
              
              mList->insert( mList->begin()+i, token ) ;
              if ( iselse >= i ) iselse += 1 ;
              end++ ;
              i++ ;
            } // while
            
            i-- ;
            j = gdefine.size() ;
          } // if
  
        } // for
        
      } // for
      
      gisCond = -2 ;
    } // else if
    
     // �@�뱡�p  
    else { 
      for ( ; !isbreak && i < end ; i++ ) {
      
        if ( mList->at( i ).str == "\'" ) isbreak = true ;
        for ( int j = 0 ; !isbreak && j < gdefine.size() ; j++ ) {
          
          if ( mList->at( i ).str == gdefine[j]->temp ) {
            stringstream ss( gdefine[j]->object );
            stringstream sss( gdefine[j]->type );
            Data token;
            mList->erase( mList->begin()+i ) ;
            end-- ;
            
            while ( ss >> token.str ) {
              sss >> token.type ;
              
                            
              Data temp;
              if ( token.str[0] == '"' ) {
                
                for ( ; token.str[token.str.size()-1] != '"' ; ) {
                  ss >> temp.str ;
                  token.str = token.str + " " + temp.str ;
                } // for
                
                
              } // if
              
              token.type3 = "isDefine" ;
              token.type4 = gdefine[j]->type4 ;
              
              mList->insert( mList->begin()+i, token ) ;
              end++ ;
              i++ ;
            } // while
            
            i-- ;
            j = gdefine.size() ;
          } // if
  
        } // for
        
      } // for
      
    } // else
    
  } // CheckDefine()
  
  void Cons( int i ) {
    
      
    if ( mList->at( i ).str == "cons" ) {
      
      mList->erase( mList->begin()+i ) ;
      int bracket = 0, j = i ; 
      
      bool isbreak = false ;
      for ( ; !isbreak && j < mList->size() ; j++ ) {  // cons�᭱���list���['.'  
        if ( mList->at( j ).str == "'" ) bracket++ ;
        else if ( mList->at( j ).str == "(" ) bracket++ ;
        else if ( mList->at( j ).str == ")" ) bracket-- ;
        else {
          if ( bracket == 0 ) {
            Data temp ;
            temp.str = ".", temp.type = "dot" ;
            mList->insert( mList->begin()+j+1, temp ) ;
            isbreak = true ;
          } // if
          
        } // else
        
        if ( !isbreak ) {
          if ( bracket == 0 && mList->at( j ).type != "quote" ) {
            Data temp ;
            temp.str = ".", temp.type = "dot" ;
            mList->insert( mList->begin()+j+1, temp ) ;
            isbreak = true ;
          } // if
          
        } // if
        
      } // for
      
    } // if
      
    DeleteDotBracketAndTNil() ;
    
  } // Cons()
  
  void List( int i ) {

    if ( mList->at( i ).str == "list" ) {
      mList->erase( mList->begin()+i ) ;
    } // if
      
  } // List()
  
  int Quote( int i ) { // str == quote
    

    int forDeleteBracket = 0 ;
    
    if ( mList->at( i ).str == "quote" ) {
      
      mList->erase( mList->begin()+i ) ; // erase quote
      
      mList->erase( mList->begin()+i-1 ) ; // erase (
      
      i -= 2 ;
      int j = i+1 ;
      bool isbreak = false ;
      for ( ; !isbreak && j < mList->size() ; j++ ) {  // do�R��quote�᭱��')'  
        if ( mList->at( j ).type == "quote" ) forDeleteBracket++ ;
        else if ( mList->at( j ).type == "left_bracket" ) forDeleteBracket++ ;
        else if ( mList->at( j ).type == "right_bracket" ) forDeleteBracket-- ;
        else {
          if ( forDeleteBracket == 0 ) {
            mList->erase( mList->begin()+j+1 ) ; // erase ')'
            isbreak = true ;
          } // if
          
        } // else
        
        if ( !isbreak ) {
          if ( forDeleteBracket == 0 && mList->at( j ).type != "quote" ) {
            mList->erase( mList->begin()+j+1 ) ; // erase ')'
            isbreak = true ;
          } // if
          
        } // if
        
      } // for   
      
      i = j ; 
      
    } // if

    return i ;  
    
  } // Quote()
  
  bool IsEvalSExp() {
    
    
    return true ;
    
  } // IsEvalSExp()
  
  void MainForCond( int k ) {
    bool checkIfNeedDoFuncAgain = false ;

    if ( k-1 >= 0 && mcondList->at( k-1 ).str != "define" ) {
      CheckDefine( k, k+1 ) ;
    } // if
    
    if ( mcondList->at( k ).str == "\'" ) {
      ghaveDoQuote = true ;
      mcondList->erase( mcondList->begin()+k ) ;
      
      int j = k, bracket = 0 ;
      bool isbreak = false ;
      for ( ; !isbreak && j < mcondList->size() ; j++ ) {  // �@����'������) ������  
        if ( mcondList->at( j ).str == "\'" ) bracket++ ;
        else if ( mcondList->at( j ).str == "(" ) bracket++ ;
        else if ( mcondList->at( j ).str == ")" ) bracket-- ;
        else {

          if ( bracket == 0 ) {
            isbreak = true ;
          } // if
          
        } // else
        
        if ( !isbreak ) {
          
          if ( bracket == 0 && mcondList->at( j ).str != "\'" ) {
            isbreak = true ;
          } // if
          
        } // if
          
      } // for
      
      DoQuote( k, j ) ;
      
      k = j-1 ;
      
      if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
      
    } // if
    else if ( mcondList->at( k ).str == "quote" ) { // ��W���P�z  
      ghaveDoQuote = true ;
      int end = Quote( k+3 ) ;
      
      DoQuote( k+3, end ) ;
      
      k = end-1 ;
      
      if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
      
    } // else if
    else {
      /*
      if ( k-1 >= 0 && mcondList->at( k-1 ).str != "define" ) {
        CheckDefine( k, k+1 ) ;
      } // if
      */
      if ( k-1 >= 0 && mcondList->at( k ).str == "define" && mcondList->at( k-1 ).str == "(" ) {
      
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+2, EndOfCheckDefine( k+2 ) ) ;
        DoDefine() ;
        
      } // if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "cons" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
       
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        

        Cons( k+3 ) ;

        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "list" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;

        List( k+3 ) ;       
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "car" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Car( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "cdr" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Cdr( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "pair?" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Pair_is( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "null?" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Null_is( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "integer?" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Integer_is( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "atom?" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Atom_is( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k-1 ).str == "(" && 
                ( mcondList->at( k ).str == "real?" || mcondList->at( k ).str == "number?" ) ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Real_Number_is( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "string?" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        String_is( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "boolean?" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Boolean_is( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "symbol?" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Symbol_is( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "list?" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        List_is( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "equal?" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Equal_is( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "eqv?" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Eqv_is( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && 
                ( mcondList->at( k ).str == "+" || mcondList->at( k ).str == "-" 
                  || mcondList->at( k ).str == "*" || mcondList->at( k ).str == "/" ) 
                && mcondList->at( k-1 ).str == "(" ) {
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Caculate( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "not" && mcondList->at( k-1 ).str == "(" ) {
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Not( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && 
                ( mcondList->at( k ).str == ">" || mcondList->at( k ).str == "<" 
                  || mcondList->at( k ).str == "="
                  || mcondList->at( k ).str == ">=" || mcondList->at( k ).str == "<=" ) 
                && mcondList->at( k-1 ).str == "(" ) {
            
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Compare( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "string-append" && mcondList->at( k-1 ).str == "(" ) {
                  
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        String_append( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if    
      
      else if ( k-1 >= 0 && 
                ( mcondList->at( k ).str == "string>?" || mcondList->at( k ).str == "string<?"
                  || mcondList->at( k ).str == "string=?" ) 
                && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        String_compare( k+3 ) ;            
        checkIfNeedDoFuncAgain = true ;
      } // else if 
      else if ( k-1 >= 0 && mcondList->at( k ).str == "cond" && mcondList->at( k-1 ).str == "(" ) {
        
        gisCond = k ;
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Cond( k+3 ) ;
        gisCond = -1 ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "if" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        If( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if    
      else if ( k-1 >= 0 && mcondList->at( k ).str == "begin" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Begin( k+3 ) ;    
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else if ( k-1 >= 0 && mcondList->at( k ).str == "and" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        And( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if  
      else if ( k-1 >= 0 && mcondList->at( k ).str == "or" && mcondList->at( k-1 ).str == "(" ) {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
        Or( k+3 ) ;
        checkIfNeedDoFuncAgain = true ;
      } // else if
      else {
        
        if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
        
      } // else
      
      if ( checkIfNeedDoFuncAgain ) { // �e��������^�Ǫ��F��O�@��func�W�r�A�N�n�A��func  
        k -= 1 ;
        if ( k-1 >= 0 && mcondList->at( k-1 ).str == "(" ) {
          
          if ( k-1 >= 0 && mcondList->at( k-1 ).str != "define" ) {
            CheckDefine( k, k+1 ) ;
          } // if
          
          if ( k-1 >= 0 && mcondList->at( k ).str == "define" && mcondList->at( k-1 ).str == "(" ) {
          
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+2, EndOfCheckDefine( k+2 ) ) ;
            DoDefine() ;
          
          } // if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "cons" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Cons( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "list" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            List( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "car" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Car( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "cdr" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Cdr( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "pair?" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Pair_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "null?" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Null_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "integer?" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Integer_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "atom?" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Atom_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k-1 ).str == "(" && 
                    ( mcondList->at( k ).str == "real?" || mcondList->at( k ).str == "number?" ) ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Real_Number_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "string?" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            String_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "boolean?" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Boolean_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "symbol?" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Symbol_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "list?" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            List_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "equal?" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Equal_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "eqv?" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Eqv_is( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && 
                    ( mcondList->at( k ).str == "+" || mcondList->at( k ).str == "-" 
                      || mcondList->at( k ).str == "*" || mcondList->at( k ).str == "/" ) 
                    && mcondList->at( k-1 ).str == "(" ) {
                      
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Caculate( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "not" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Not( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && 
                    ( mcondList->at( k ).str == ">" || mcondList->at( k ).str == "<" 
                      || mcondList->at( k ).str == "="
                      || mcondList->at( k ).str == ">=" || mcondList->at( k ).str == "<=" ) 
                    && mcondList->at( k-1 ).str == "(" ) {
                      
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Compare( k ) ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "string-append" 
                    && mcondList->at( k-1 ).str == "(" ) {
                      
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            String_append( k ) ;
            
          } // else if    
          
          else if ( k-1 >= 0 && 
                    ( mcondList->at( k ).str == "string>?" || mcondList->at( k ).str == "string<?"
                      || mcondList->at( k ).str == "string=?" ) 
                    && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            String_compare( k ) ;            
            
          } // else if 
          else if ( k-1 >= 0 && mcondList->at( k ).str == "cond" && mcondList->at( k-1 ).str == "(" ) {
            
            gisCond = k ;
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Cond( k ) ;
            gisCond = -1 ;
            
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "if" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            If( k ) ;
            
          } // else if    
          else if ( k-1 >= 0 && mcondList->at( k ).str == "begin" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Begin( k ) ;    
              
          } // else if
          else if ( k-1 >= 0 && mcondList->at( k ).str == "and" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            And( k ) ;
            
          } // else if  
          else if ( k-1 >= 0 && mcondList->at( k ).str == "or" && mcondList->at( k-1 ).str == "(" ) {
            
            if ( k+1 < mcondList->size() ) MainForCond( k+1 ) ;
            // CheckDefine( k+1, EndOfCheckDefine( k+1 ) ) ;
            Or( k ) ;
            
          } // else if
          
        } // if
      
      } // if
      
    } // else

  } // MainForCond()
  
};

int main() {
  
  
  Project first ;
  bool isbreak = false ;
  cout << "Welcome to OurScheme!" << endl ;
  // cin.open( mFileName.c_str(), ios::in ) ;
  gtest.ch = cin.get() ; // Ū���D��  
  gtest.ch = cin.get() ; // Ū���D����\n  
  
  gtest.ch = cin.get() ;
  first.Cond_fuc() ;
  first.MList_fuc();
  while ( gtest.ch != EOF && !isbreak && !gcorrectFileContent ) {
    
    while ( gtest.ch == ' ' || gtest.ch == '\t' ) {
      gcolumn++ ;
      gtest.ch = cin.get() ;
    } // while
    
    if ( gtest.ch == '"' ) {
      if ( gline == -1 ) gline++ ;
      gline++ ;
      first.DoubleQuote() ;  
    } // if
    else if ( gtest.ch == ';' ) {
      if ( gline == -1 ) gline++ ;
      gline++ ;
      first.Semi() ;
    } // else if
    else if ( gtest.ch == '(' || gtest.ch == '\'' ) {
      if ( gline == -1 ) gline++ ;
      gline++ ;
      
      int k = 0 ;
      if ( first.ReadBracket() ) {
      
        bool isbreak = false ;  
        if ( first.IsCorrectBracket() ) {
          
          
          if ( !first.DeleteDotBracketAndTNil() ) isbreak = true ;
  
          if ( gcorrectFileContent || gtest.ch == EOF ) isbreak = true ;
          
          if ( !isbreak ) {
            
            if ( first.IsEvalSExp() ) {
           
               // first.CheckDefine( 0 ) ;
                
              first.Cond_fuc() ;
              first.MainForQuote( k ) ;              
 
              int column = 1, i = -1, now = 1 ; // i for mList->size
              TreePtr mTree = new DataTree ;
              mTree -> type = "first" ;
              mTree -> left = NULL ;
              mTree -> right = NULL ;
              if ( !gdd ) {     
                
                cout << endl << "> ";
              
                first.BuildTree( i, mTree, column ) ;
  
                // cout << mList->at( 2 ).str << " " ;
                first.PrintTree( mTree, now ) ;
              } // if
              
              mTree = NULL ;
              gdd = false ;
              gisCond = -1 ;
              
            } // if
            else {
              // error
            } // else   
            
          } // if
          
        } // if
        
      } // if
      
      first.mList->clear() ;
      gline = -1, gcolumn = 1 ;
    } // else if
    else if ( gtest.ch == ')' ) {
      if ( gline == -1 ) gline++ ;
      gline++ ;
      cout << endl << "> ERROR (unexpected token) : atom or '(' expected when token at Line " 
           << gline << " Column " << gcolumn << " is >>)<<" << endl ;
      
      first.mList->clear() ; 
      while ( gtest.ch != '\n' && gtest.ch != EOF ) gtest.ch = cin.get() ;
      gline = -1, gcolumn = 1 ;
    } // else if
    else if ( gtest.ch == '\n' ) {
      gline++ ;
      gcolumn = 1 ;
    } // else if
    else {
      bool isisbreak = false ;
      if ( gline == -1 ) gline++ ;
      gline++ ;
      gcolumn-- ; // �b�o�̱q0�}�l    
      while ( gtest.ch != EOF && !isisbreak && gtest.ch != ' ' && gtest.ch != '\t' && gtest.ch != '\n' ) {
        if ( cin.peek() == '"' || cin.peek() == ';' || cin.peek() == '('
             || cin.peek() == ')' || cin.peek() == ' ' || cin.peek() == '\t'
             || cin.peek() == '\n' ) {
          isisbreak = true ;
        } // if
        
        gcolumn++ ;
        gtest.column = gcolumn, gtest.line = gline ;
        first.mList->push_back( gtest ) ;  
        
        if ( !isisbreak ) gtest.ch = cin.get() ;
      } // while 
      
      first.Other() ;
      
      if ( gtest.ch == '\n' ) gline = -1, gcolumn = 1 ;
      
    } // else
    
    if ( !isbreak && gtest.ch != EOF && !gcorrectFileContent ) gtest.ch = cin.get() ;
    gisDoubleQuoteFunc = false, ghaveDoQuote = false ;
    
  } // while
  
  // cin.close();
  
  if ( !gcorrectFileContent ) {
    cout << endl << "> " << "ERROR (no more input) : END-OF-FILE encountered" << endl ;
    cout << "Thanks for using OurScheme!" ;
  } // if
  else cout << endl << "> " << endl << "Thanks for using OurScheme!" ;

} // main()
