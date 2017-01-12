#ifndef JOURNAL_READER_H
#define JOURNAL_READER_H

#include <memory>
#include <thread>
#include "../common/blocking_queue.h"
#include "message.h"
#include "cache/cache_proxy.h"

using namespace std;

namespace Journal{

class JournalReader
{
public:
    explicit JournalReader(BlockingQueue<struct IOHookRequest>& read_queue, 
                           BlockingQueue<struct IOHookReply*>&  reply_queue); 
    virtual ~JournalReader();
    
    JournalReader(const JournalReader& r) = delete;
    JournalReader& operator=(const JournalReader& r) = delete;

    bool init(shared_ptr<CacheProxy> cacheproxy);
    bool deinit();
    
    void work();

private:
    /*in queue*/
    BlockingQueue<struct IOHookRequest>& m_read_queue; 
    /*out queue*/
    BlockingQueue<struct IOHookReply*>&  m_reply_queue; 

    shared_ptr<CacheProxy>  m_cacheproxy;  

    bool               m_run;
    shared_ptr<thread> m_thread;
};

}

#endif

