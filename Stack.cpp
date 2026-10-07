typedef double stack_elem_t;
#include <TXlib.h>
#include <stdio.h>
#include <stdbool.h>
#define CANARY_LEFT 0xBEDA
#define CANARY_RIGHT 0xBAD
#define STRUCT_CHECK 0xBADEDA

void StackPush( struct stack_t * s, double value);

void StackResizeUp( struct stack_t * s );

stack_elem_t StackPop( struct stack_t * s);

struct stack_t * StackInit( int cap);

void StackDestroy( struct stack_t * s);

bool StackOK( struct stack_t * s );

void StackDump( struct stack_t * stk );

void StackResizeDown( struct stack_t * stk );

struct stack_t
    {
        stack_elem_t * deita;
        int check;
        int capacity;
        int size;
    };

int main(void)
{
    struct stack_t * stk1 = StackInit(2);

    StackPush( stk1, 10);
    StackPush( stk1, 20);
    StackPush( stk1, 30);

    stack_elem_t x = StackPop(stk1);
    stack_elem_t y = StackPop(stk1);
    stack_elem_t z = StackPop(stk1);

    stack_elem_t c = StackPop(stk1);

    printf("<%.2f> \n<%.2f> \n<%.2f>\n", x, y, z);

    printf("<%.3f>\n", c);

    printf("<%.f>\n", stk1->deita[0]);

    StackDestroy( stk1 );

    return 0;


}

void StackPush( struct stack_t * s, stack_elem_t value)
{
    if( !StackOK( s ) )
    {
        StackDump( s );
        return;
    }
    if ( s->size == s->capacity)
        StackResizeUp( s );
    s->deita[1 + s->size++] = value;
}

stack_elem_t StackPop( struct stack_t * s)
{
    if( !StackOK( s ) || s->size == 0 )
    {
        fprintf( stderr, "POP при нулевом size\n");
        StackDump( s );
        return 666.666;
    }

    if( s->capacity >= 4*( s->size) )
        StackResizeDown( s );

    printf( "%d", s->size);
    stack_elem_t a = s->deita[1 + (--s->size)];
    printf( "%d\n", s->size);
    return a;
}

struct stack_t * StackInit( int cap)
{
    struct stack_t * stk = (struct stack_t *)calloc( 1, sizeof(struct stack_t) );

    stk->deita = ( stack_elem_t *)calloc( cap + 2, sizeof(stack_elem_t));

    stk->deita[0] = CANARY_LEFT;

    stk->deita[cap + 1] = CANARY_RIGHT;

    stk->size = 0 ;

    stk->capacity = cap;

    stk->check = STRUCT_CHECK;

    if( !StackOK(stk) )
    {
        fprintf( stderr, "Ошибка инициализации стека");
        return 0;
    }
    return stk;
}

void StackResizeUp( struct stack_t * s )
{
    stack_elem_t * temp = ( stack_elem_t * )realloc( s->deita, sizeof(stack_elem_t)*((s->capacity)*2 + 2) );

    if ( !temp )
    {
        fprintf( stderr, "Ошибка увеличения размера стека");
        StackDump( s );
        free(s->deita);
        return;
    }

    s->deita = temp;

    s->capacity = (s->capacity)*2;

    s->deita[s->capacity + 1] = CANARY_RIGHT;
}

void StackDestroy( struct stack_t * s)
{
    free( s->deita );
    free(s);
}

bool StackOK( struct stack_t * s )
{
    if (s == NULL)
    {
        fprintf(stderr, "Обнаружен нулевой указатель на структуру стека\n");
        return false;
    }
    if( s->deita == NULL )
    {
        fprintf( stderr, "Обнаружен нулевой указатель на структуру");
        StackDump(s);
        return  false;
    }
    if( s->size > s->capacity )
    {
        fprintf( stderr, "size > capacity ");
        StackDump(s);
        return false;
    }
    if( s->size < 0 )
    {
        fprintf( stderr, "Отрицательный size");
        StackDump(s);
        return false;
    }
    if( s->capacity <= 0 )
    {
        fprintf( stderr,"Отрицательный capacity");
        StackDump(s);
        return false;
    }
    if( s->deita[0] != CANARY_LEFT )
    {
        fprintf( stderr, "Ошибка левой канарейки");
        StackDump(s);
        return false;
    }
    if( s->deita[1 + s->capacity] != CANARY_RIGHT )
    {
        fprintf( stderr, "Ошибка правой канарейки");
        StackDump(s);
        return false;
    }
    if( s->check != STRUCT_CHECK )
    {
        fprintf( stderr, "Перезапись значений в структуре" );
        StackDump(s);
        return false;
    }
    return true;

}

void StackDump( struct stack_t * stk )
{
    fprintf( stderr, "STACKDUMP: \n");

    if( stk == NULL )
    {
        fprintf( stderr, "Обращение к нулевому указателю" );

    }

    fprintf( stderr, "Адрес структуры: %p \n", stk);
    fprintf( stderr, "size: %d \n", stk->size);
    fprintf( stderr, "capacity: %d \n", stk->capacity);
    fprintf( stderr, "Адрес стека: %p \n", stk->deita);

}

void StackResizeDown( struct stack_t * stk )
{
    stack_elem_t * tempstk = ( stack_elem_t * )realloc( stk->deita, sizeof( double )*(( stk->capacity )/2 + 2));
    if ( tempstk == NULL )
    {
        fprintf( stderr, "Ошибка при уменьшении размера стека");
        StackDump( stk );
        return;
    }

    stk->deita = tempstk;

    stk->capacity = ( stk->capacity ) / 2;

    stk->deita[1 + stk->capacity] = CANARY_RIGHT;
}

