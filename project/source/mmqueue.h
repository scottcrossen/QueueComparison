// @Copyright 2017 Scott Leland Crossen

typedef struct mm_queue_t {
} mm_queue_t;

mm_queue_t* createMMQueue(unsigned int size);

void destroyMMQueue(mm_queue_t* this);

void enqueueMM(mm_queue_t* this, char data);

void dequeueMM(mm_queue_t* this, char* data);
