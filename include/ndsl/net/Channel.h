/*
 * File: Channel.h
 * brief:
 *
 * Author: gyz
 * Email: mni_gyz@163.com
 * Last Modified: Wednesday, 28th November 2018 10:41:47 pm
 */

#ifndef __CHANNEL_H__
#define __CHANNEL_H__
// #include "EventLoop.h"

namespace ndsl {
namespace net {

class Channel
{
  public:
    virtual ~Channel() {}
    virtual int onRead() = 0;
    virtual int onWrite() = 0;

    virtual int handleEvent() = 0;
    virtual int getFd() = 0;
    virtual uint64_t getEvents() = 0;
    virtual int setRevents(uint64_t revents) = 0;
};

} // namespace net
} // namespace ndsl

#endif // __CHANNEL_H__