/*
 * File: ChannelCallBack.h
 * brief:
 *
 * Author: gyz
 * Email: mni_gyz@163.com
 * Last Modified: Wednesday, 28th November 2018 4:56:39 pm
 * -----
 * Copyright 2018 - 2018
 */

#ifndef __CHANNELCALLBACK__
#define __CHANNELCALLBACK__

namespace ndsl {
namespace net {

class ChannelCallBack
{
    virtual int handleRead() = 0;
    virtual int handleWrite() = 0;
};

} // namespace net
} // namespace ndsl

#endif // __CHANNELCALLBACK__