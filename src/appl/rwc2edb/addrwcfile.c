// load rwc as TString    DO NOT WORK !!!
/*TString LoadRwcFile(char* rwcname)
{
   TString str;
   Char_t ch;

   ifstream in;\
   in.open(rwcname,ios_base::in | ios_base::binary);
   while (!in.eof() ) {
      in >> ch;
      str.Append(ch);
   }
   in.close();
   return str;
}
*/
// load rwc as TString     GOOD 
TString LoadRwcFile(char* rwcname)
{
   TString str ;
   int  i, ch;
   FILE *stream;
   if( (stream = fopen( rwcname, "rb" )) == NULL )
      exit( 0 );
   ch = fgetc( stream );
   for( i=0; (i < 200000 ) && ( feof( stream ) == 0 ); i++ )
   {
      str.Append( ch ); 
      ch = fgetc( stream );
   }
   fclose( stream );
   return str ;
}

// load rwc as TArrayC     GOOD 
TArrayC* LoadRwcFileC(char* rwcname)
{
   Long_t maxsize(300000), size ;
   char* byte= new char[maxsize];
   FILE *stream;
   if( (stream = fopen( rwcname, "rb" )) == NULL ) exit( 0 );
   byte = fgetc( stream );
   for( size=0; (size < maxsize ) && ( feof( stream ) == 0 ); size++ )
   {
      byte = fgetc( stream );
   }
   fclose( stream );
   cout << size << endl;
   TArrayC*  eBuffer = new TArrayC();
   eBuffer->Adopt(size,byte);
   return eBuffer ;
}

// load rwc as TArrayD     GOOD 
TArrayD* LoadRwcFileD(char* rwcname)
{
   Long_t maxsize(300000), size ;
   char* byte= new char[maxsize];
   FILE *stream;
   if( (stream = fopen( rwcname, "rb" )) == NULL ) exit( 0 );
   byte = fgetc( stream );
   for( size=0; (size < maxsize ) && ( feof( stream ) == 0 ); size++ )
   {
      byte = fgetc( stream );
   }
   fclose( stream );
   cout << size << endl;
   TArrayD*  eBuffer = new TArrayD();
   eBuffer->Adopt(size,byte);
   return eBuffer ;
}

// load rwc as ntuple
void AddRWCFile(EdbRun* run, char* rwcname)
{

   Char_t ch;
   TNtuple *ntuple = new TNtuple("rwcfile","raw data catalog file","ch");

   ifstream in;
   in.open(rwcname,ios::in | ios::binary);
   while (!in.eof() ) {
      in >> ch;
      ntuple->Fill(ch);
   }
   in.close();

   run->Write();
}
