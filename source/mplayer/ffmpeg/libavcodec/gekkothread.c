/*
 * Copyright (c) 2009 rodries <rodries@gmail.com>
 *
 */

#include "avcodec.h"

#include <ogc/lwp.h>
#include <ogc/semaphore.h>
//#include "mp_msg.h"

typedef struct ThreadContext{
    AVCodecContext *avctx;
    lwp_t thread;
    sem_t work_sem;
    sem_t done_sem;
    int (*func)(AVCodecContext *c, void *arg);
    void *arg;
    int ret;
}ThreadContext;


static void * thread_func(void *v)
{
    ThreadContext *c= v;

    while(1)
    {
        //mp_msg(MSGT_VO, MSGL_ERR,"thread_func Lock work_sem %i\n",c->work_sem);
        LWP_SemWait(c->work_sem);
        //mp_msg(MSGT_VO, MSGL_ERR,"thread_func pass Lock work_sem %i, exec\n",c->work_sem);
        if(c->func)
            c->ret= c->func(c->avctx, c->arg);
        else
        {
            return NULL;
        }
        //mp_msg(MSGT_VO, MSGL_ERR,"thread_func UnLock done_sem %i\n",c->thread);
        LWP_SemPost(c->done_sem);
    }
    return NULL;
}

/**
 * Free what has been allocated by avcodec_thread_init().
 * Must be called after decoding has finished, especially do not call while avcodec_thread_execute() is running.
 */
void avcodec_thread_free(AVCodecContext *s)
{
    ThreadContext *c= s->thread_opaque;
    int i;

    for(i=0; i<s->thread_count; i++)
	{
        c[i].func= NULL;
        LWP_SemPost(c[i].work_sem);
        LWP_JoinThread(c[i].thread, NULL);
        LWP_SemDestroy(c[i].work_sem);
        LWP_SemDestroy(c[i].done_sem);
    }
    av_freep(&s->thread_opaque);  
}

int avcodec_thread_execute(AVCodecContext *s, int (*func)(AVCodecContext *c2, void *arg2),void *arg, int *ret, int count, int size){
    ThreadContext *c= s->thread_opaque;
    int i;


    assert(s == c->avctx);
    assert(count <= s->thread_count);

    /* note, we can be certain that this is not called with the same AVCodecContext by different threads at the same time */

    for(i=0; i<count; i++){
        c[i].arg= (char*)arg + i*size;
        c[i].func= func;
        c[i].ret= 12345;
//        mp_msg(MSGT_VO, MSGL_ERR,"c[%i].func\n",i);
//        mp_msg(MSGT_VO, MSGL_ERR,"UnLock work_sem %i\n",c[i].work_sem);
        LWP_SemPost(c[i].work_sem);
    }
    for(i=0; i<count; i++){
//        mp_msg(MSGT_VO, MSGL_ERR,"Lock done_sem %i\n",c[i].done_sem);
        LWP_SemWait(c[i].done_sem);
        c[i].func= NULL;
        if(ret) ret[i]= c[i].ret;
    }
    return 0;
}

int avcodec_thread_init(AVCodecContext *s, int thread_count){
    int i;
    ThreadContext *c;
    s->thread_count = thread_count;

    assert(!s->thread_opaque); 
    c= av_mallocz(sizeof(ThreadContext)*thread_count);
    s->thread_opaque= c;

    for(i=0; i<thread_count; i++){
        c[i].avctx= s;

        if(LWP_SemInit(&c[i].work_sem,0,1)==-1) goto fail;
        if(LWP_SemInit(&c[i].done_sem,0,1)==-1) goto fail;

        //mp_msg(MSGT_VO, MSGL_ERR,"avcodec_thread_init lock %i %i \n",c[i].work_sem,c[i].done_sem);
      	if(LWP_CreateThread(&c[i].thread, thread_func, &c[i], NULL, 0, 80)==-1) goto fail;
    }

    s->execute= avcodec_thread_execute;
//mp_msg(MSGT_VO, MSGL_ERR,"threads ok\n");sleep(8);
    return 0;
fail:
//mp_msg(MSGT_VO, MSGL_ERR,"error threads\n");sleep(4);
    avcodec_thread_free(s);
    return -1;
}
