#include "arthook_manager.h"

static struct arthook_t *h = NULL;

pthread_rwlock_t lock;

int arthook_manager_init(JNIEnv* env)
{
    if (pthread_rwlock_init(&lock,NULL) != 0) return 1;
    
    hook_demo_init(env);
}

int add_hook(arthook_t* new)
{
    if (pthread_rwlock_wrlock(&lock) != 0) return 1;
    HASH_ADD_STR(h, key, new);
    pthread_rwlock_unlock(&lock);
}
arthook_t* get_hook_by_key(char* key)
{
    arthook_t* myhook = NULL;
    if (pthread_rwlock_rdlock(&lock) != 0) return NULL;
    HASH_FIND_STR(h, key, myhook);
    pthread_rwlock_unlock(&lock);
    return myhook;
}

void* get_method_from_hashtable(unsigned int* target)
{
    arthook_t* tmp = NULL;

    for(tmp = h; tmp != NULL; tmp = tmp->hh.next){
        LOGI("elemento: %s \n", tmp->clsname);
        LOGI("original : %x \n", tmp->original_meth_ID);
        LOGI("sto cercando : %x \n", *target);
        int res = memcmp(&tmp->original_meth_ID, target,4);
        LOGI("risultato: %d \n", res);
        if(res == 0)
            return tmp;
        else return NULL;
    }
}

int is_method_in_hashtable(unsigned int* target)
{
    arthook_t* tmp = NULL;

    for(tmp = h; tmp != NULL; tmp = tmp->hh.next){
        LOGI("elemento: %s \n", tmp->clsname);
        LOGI("original : %x \n", tmp->original_meth_ID);
        LOGI("sto cercando : %x \n", *target);
        int res = memcmp(&tmp->original_meth_ID, target,4);
        LOGI("risultato: %d \n", res);
        if(res == 0)
            return 1;
        else return 0;
    }
}



