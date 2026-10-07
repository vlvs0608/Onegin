#include <TXlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>

typedef struct
{
    char * buffer;
    char ** index;
    int sizeofbuf;
    int sumstrings;
} Massives;

Massives * StructInit( char * file );

void DestroyStruct( Massives * st );

int MyStrlen(const char * str);

char * NewString( char * strings);

void MyStrcpy( char *from, char *to);

int CountingStrings( char *strings, int n);

int Sravn(const char * s1, const char * s2);

int SizeOfFile( char * file);

void ReplaceSymbols( char * strings, int length);

void CollectMassive( Massives * st);

FILE * FileOpen( char * file);

char * CreateBuffer ( char * file);

void Department( void );

int ComparZ(char * s1, char * s2);

int CompareFromBegin( const void * a, const void * b);

int CompareFromEnd( const void * a, const void * b);

void PrintText( Massives * st );

int MoreOrLess( const char * s1, const char * s2 );

void ItsSimbolsA( const char ** s1, const char ** s2 );

void ItsSimbolsZ( const char ** s1, const char ** s2 );

int main(void)
{
    freopen( "output.txt", "w", stdout);

    Massives * st = StructInit( "Onegin.txt" );

    CollectMassive(st);

    Department();

    qsort( st->index, st->sumstrings, sizeof(st->index[0]), CompareFromBegin);

    PrintText( st );

    Department();

    qsort( st->index, st->sumstrings, sizeof(st->index[0]), CompareFromEnd);

    PrintText( st );

    for( int i = 0; i < st->sizeofbuf; i++)
        if( st->buffer[i] == '\0')
            st->buffer[i] = '\n';

    Department();

    printf("%s\n", st->buffer);

    DestroyStruct( st );

    fclose(stdout);
    return 0;

}

int MyStrlen(const char * str)
{
    int n = 0;
    while(*str != '\0')
    {
        str++;
        n++;
    }
    return n;
}

char * NewString( char * strings)
{
    int l = MyStrlen(strings) + 1;
    char * adr_buf = (char*)calloc(l, sizeof(char));
    MyStrcpy(strings, adr_buf);
    return adr_buf;
}

void MyStrcpy( char *from, char *to)
{
    do
    {
        *to = *from;
        to++;
    }
    while(*from++ != '\0');
}

int CountingStrings( char *strings, int n)
{
    int count = 0;
    for( int i = 1; i <= n; i++)
        if ( strings[i - 1] == '\0' && strings[i] != '\0')
            count++;
    return count;
}

int Sravn(const char * s1, const char * s2)
{
    ItsSimbolsA( &s1, &s2 );
    while (*s1 && *s2 && tolower((unsigned char)*s1) == tolower((unsigned char)*s2))
    {
        s1++;
        s2++;
        ItsSimbolsA( &s1, &s2 );
    }
    return MoreOrLess( s1, s2 );
}

int SizeOfFile( char * file)
{
    struct stat st = {};
    stat( file , &st);
    int n = st.st_size;    //TODO: long long vs size_t vs unsigned long
    return n;
}

void ReplaceSymbols( char * buf, int length)
{
    for( int i = 1; i <= length; i++)
    {
        if( buf[i] == '\n')
            buf[i] = '\0';
        else if( buf[i] == '\r')
            buf[i] = '\0';
    }
}

void CollectMassive( Massives * st)
{
    for( int i = 1, j = 1; i < st->sizeofbuf - 1 && j < st->sumstrings; i++)
        if(st->buffer[i] == '\0' && st->buffer[i + 1] != '\0')
        {
            int p = 0;
            while(!isalpha( st->buffer[i + 1 + p] ))
                p++;
            st->index[j++] = &st->buffer[i + 1 + p];
        }
}

FILE * FileOpen( char * filename)
{
    FILE * file_ptr = fopen( filename , "rb");    //TODO: FOPEN VS OPEN

    if( file_ptr == NULL)
    {
        printf("Ошибка чтения файла");
        return NULL;
    }

    return file_ptr;
}

char * CreateBuffer ( char * file)
{
    FILE * file_ptr = FileOpen(file);
    const int n = SizeOfFile(file);
    char * Buffer = ( char *)calloc( n + 2, sizeof(char));
    if( Buffer == NULL)
    {
        printf("Ошибка cоздания буффера");
        return NULL;
    }
    Buffer[0] = '\0';
    fread( &Buffer[1], 1, n, file_ptr);
    Buffer[n + 1] = '\0';
    ReplaceSymbols( Buffer, n );
    fclose(file_ptr);
    return Buffer;
}

void Department( void )
{
    putchar('\n');
    for( int i = 0; i < 50; i++)
        putchar( '=' );
    putchar('\n');
    putchar('\n');
}

int ComparZ(const char * s1, const char * s2)
{
    while( *s1 != '\0')
        s1++;
    while( *s2 != '\0')
        s2++;
    s1--;
    s2--;
    ItsSimbolsZ( &s1,&s2 );
    while (*s1 && *s2 && tolower((unsigned char)*s1) == tolower((unsigned char)*s2))
    {
        s1--;
        s2--;

        ItsSimbolsZ( &s1, &s2 );
    }

    return MoreOrLess( s1, s2 );
}

int CompareFromBegin( const void * a, const void * b)
{
    const char * str1 = *(const char **) a;
    const char * str2 = *(const char **) b;
    return Sravn( str1, str2);
}

int CompareFromEnd( const void * a, const void * b)
{
    const char * str1 = *( char **) a;
    const char * str2 = *( char **) b;
    return ComparZ( str1, str2);
}

Massives * StructInit( char * file )
{
    Massives * st = (Massives *) calloc(1, sizeof(Massives));
    if (!st)
    {
        fprintf( stderr, "Ошибка создания структуры");
        return NULL;
    }
    st->buffer = CreateBuffer( file );
    st->sizeofbuf = SizeOfFile( file );
    st->sumstrings = CountingStrings(st->buffer, st->sizeofbuf);

    st->index = ( char * * )calloc( st->sumstrings, sizeof(char *));

    st->index[0] = &st->buffer[1];
    return st;
}

void PrintText( Massives * st )
{
    for( int i = 0; i < st->sumstrings  ; i++)
            printf("<<<%d||%s>>>\n\n\n", i, st->index[i]);
}

int MoreOrLess( const char * s1, const char * s2 )
{
    if (tolower(((unsigned char)*s1)) < tolower(((unsigned char)*s2)))
        return -1;
    if (tolower(((unsigned char)*s1)) > tolower(((unsigned char)*s2)))
        return 1;
    return 0;
}

void ItsSimbolsA( const char ** s1, const char ** s2 )
{
    while (**s1 && !isalpha(**s1))
            (*s1)++;
    while (**s2 && !isalpha(**s2))
            (*s2)++;
}

void ItsSimbolsZ( const char ** s1, const char ** s2 )
{
    while (**s1 && !isalpha(**s1))
            (*s1)--;
    while (**s2 && !isalpha(**s2))
            (*s2)--;
}

void DestroyStruct( Massives * st )
{
    free( st->buffer );

    free( st->index );

    free( st );
}
