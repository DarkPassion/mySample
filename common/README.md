


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

sitem* i = _q->pop_front();

sitem* i = new sitem();
i->seq = num++;
i->id = 1122;
_q->push_back(i);
```

