/**********************************************
*  Copyright (c) 2016 Huawei Technologies Co., Ltd. All rights reserved.
*  
*  File name:    backup_proxy.h
*  Author: 
*  Date:         2016/11/03
*  Version:      1.0
*  Description:  backup entry
*  
*************************************************/
#ifndef SRC_SG_CLIENT_CACHE_JCACHE_H_
#define SRC_SG_CLIENT_CACHE_JCACHE_H_
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include "common.h"

class Jkey {
 public:
    Jkey() = default;
    explicit Jkey(IoVersion seq);
    Jkey(const Jkey& other);
    Jkey(Jkey&& other);
    Jkey& operator=(const Jkey& other);
    Jkey& operator=(Jkey&& other);

    friend bool operator<(const Jkey& a, const Jkey& b);
    friend ostream& operator<<(ostream& cout, const Jkey& key);
    /*io sequence */
    IoVersion m_seq;
};

class Jcache {
 public:
    Jcache() = default;
    explicit Jcache(std::string blk_dev);

    Jcache(const Jcache& other) = delete;
    Jcache(Jcache&& other) = delete;
    Jcache& operator=(const Jcache& other) = delete;
    Jcache& operator=(Jcache&& other) = delete;
    ~Jcache();

    /*act as queue*/
    void push(shared_ptr<CEntry> value);
    shared_ptr<CEntry> pop();

    /*travel use*/
    class Iterator {
     public:
        explicit Iterator(SharedMutex& lock, map<Jkey, shared_ptr<CEntry>>::iterator it);

        bool operator==(const Iterator& other);
        bool operator!=(const Iterator& other);

        Iterator& operator++();
        Iterator  operator++(int);

        Jkey first();
        shared_ptr<CEntry> second();
     private:
        SharedMutex& m_lock;
        map<Jkey, shared_ptr<CEntry>>::iterator m_it;
    };

    Jcache::Iterator begin();
    Jcache::Iterator end();

    /*CRUD*/
    bool add(Jkey key, shared_ptr<CEntry> value);
    shared_ptr<CEntry> get(Jkey key);
    bool update(Jkey key, shared_ptr<CEntry> value);
    bool del(Jkey key);

    int size()const;
    bool empty()const;
    /*debug*/
    void trace();

 private:
    /*original block device*/
    std::string m_blkdev;
    /*read and write lock*/
    mutable SharedMutex m_mutex;
    /*act the same function as queue*/
    map<Jkey, shared_ptr<CEntry>> m_cache;
};

#endif  // SRC_SG_CLIENT_CACHE_JCACHE_H_
