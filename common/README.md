


## README


### thread
```C++
class Thread1 : public ThreadHandle {
public:
	Thread1(){}

	~Thread1(){}

	virtual int cycle()
	{
		printf("cycle !\n");
	}

}

Thread1 t1 = new Thread1;
t1.start();
```

### queue

```C++
struct sitem
{
    int seq;
    int id;
};
typedef Queue<sitem> TypeQueue;

TypeQueue* _q  = new TypeQueue;

// thread safe
sitem* i = _q->pop_front();
if (i) delete i;

sitem* i = new sitem();
i->seq = num++;
i->id = 1122;
// thread safe
_q->push_back(i);

```

block queue

```C++

struct sitem
{
    int seq;
    int id;
};
typedef BlockQueue<sitem> TypeQueue;

TypeQueue * _q = new TypeQueue;

// thread safe
_q->push_back(new sitem());


// will hug when queue empty
sitem* i = _q->pop_front();
if(i) delete i;

```


