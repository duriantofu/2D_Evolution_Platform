#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RBUF_SIZE 16

typedef struct point_t
{
	int x;
	int y;
	int r;
} point_t;

typedef struct rbuf_t
{
	short head;
	short tail;
	short count;
	point_t buf[RBUF_SIZE];
} rbuf_t;

unsigned int modulo_inc (const short value, const short modulus);
void rbuf_init	(rbuf_t *_rbuf);
int rbuf_empty	(rbuf_t *_rbuf);
int rbuf_full		(rbuf_t *_rbuf);
point_t rbuf_get		(rbuf_t *_rbuf);
void rbuf_flush	(rbuf_t *_rbuf, const int clearBuffer);
void rbuf_add		(rbuf_t *_rbuf, const short x,
																const short y,
																const short z);

//main function used for testing
int main(int argc, char *argv[])
{
	rbuf_t buf;
	rbuf_t *pbuf = &buf;
	point_t xypoint;
	rbuf_init(pbuf);
	int x,y,r;
	int i;
	short arg = (argc-1)%3;
	if(arg!=0) {
		printf("Need multiple of 3, current mult: %d\n",arg);
		return -1;
	}
	for(i=1;i<=argc-1;i+=3) {
		x = atoi(argv[i]);
		y = atoi(argv[i+1]);
		r = atoi(argv[i+2]);
		if(!rbuf_full(pbuf)) {
			rbuf_add(pbuf, x, y, r);
		}
		else printf("Can't add %d,%d,%d, Buffer is full.\n",x,y,r);
	}
	while(!rbuf_empty(pbuf)) {
		xypoint = rbuf_get(pbuf);
		printf("x: %d, y: %d, r: %d\n",xypoint.x,xypoint.y,xypoint.r);
		if(rbuf_empty(pbuf)) printf("All done!\n");
	}
}

unsigned int modulo_inc (const short value, const short modulus)
{
    short my_value = value + 1;
    if (my_value >= modulus)
    {
      my_value  = 0;
    }
    return (my_value);
}

void rbuf_init (rbuf_t *_rbuf)
{
	/*
		The following clears:
			-> buf
			-> head
			-> tail
			-> count
		and sets head = tail
	*/
	memset (_rbuf, 0, sizeof(*_rbuf));
}

int rbuf_empty (rbuf_t *_rbuf)
{
	return (0==_rbuf->count);
}

int rbuf_full (rbuf_t *_rbuf)
{
	return (_rbuf->count>=RBUF_SIZE);
}

point_t rbuf_get (rbuf_t *_rbuf)
{
	point_t c  = _rbuf->buf[_rbuf->tail];
	_rbuf->tail = modulo_inc(_rbuf->tail, RBUF_SIZE);
	--_rbuf->count;
	return c;
}

void rbuf_add (rbuf_t *_rbuf, const short x, const short y, const short r)
{
	if (_rbuf->count < RBUF_SIZE)
	{
		point_t *xyr = &_rbuf->buf[_rbuf->head];
		xyr->x = x;
		xyr->y = y;
		xyr->r = r;
		_rbuf->head = modulo_inc(_rbuf->head, RBUF_SIZE);
		++_rbuf->count;
	}
}

void rbuf_flush (rbuf_t *_rbuf, const int clearBuffer)
{
	_rbuf->count = 0;
	_rbuf->head = 0;
	_rbuf->tail = 0;
	if (clearBuffer)
	{
		memset(_rbuf->buf, 0, sizeof(_rbuf->buf));
	}
}
