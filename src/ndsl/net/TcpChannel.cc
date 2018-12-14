/*
 * File: TcpChannel.cc
 * brief:
 *
 * Author: gyz
 * Email: mni_gyz@163.com
 * Last Modified: Thursday, 29th November 2018 1:30:56 pm
 */

#include "ndsl/net/TcpChannel.h"
#include <sys/epoll.h>

#include <iostream>
using namespace std;

namespace ndsl {
namespace net {

TcpChannel::TcpChannel(int sockfd, EventLoop *loop)
    : sockfd_(sockfd)
    , pLoop_(loop)
{
    registOnReadWrite();
}

// Channel::~Channel() {}

// int TcpChannel::onRead(void *inBuf) {}

// int TcpChannel::onWrite(void *param) { return S_OK; }

// 注册onRead函数和onWrite函数
int TcpChannel::registOnReadWrite()
{
    onRead = &onRead;
    onWrite = &onWrite;
    return S_OK;
}

int TcpChannel::handleEvent()
{
    // TcpConnection *pThis = reinterpret_cast<TcpConnection *>(param);
    if (getRevents() & EPOLLIN) { pCon_->handleRead(); }
    if (getRevents() & EPOLLOUT) { pCon_->handleWrite(); }
    return S_OK;
}

int TcpChannel::setCallBack(TcpConnection *pCon)
{
    pCon_ = pCon_;
    return S_OK;
}

uint32_t TcpChannel::getRevents() { return revents_; }

int TcpChannel::setRevents(uint32_t revents)
{
    revents_ = revents;
    return S_OK;
}

EventLoop *TcpChannel::getEventLoop() { return pLoop_; }

uint32_t TcpChannel::getEvents() { return events_; }

int TcpChannel::enableReading()
{
    events_ |= EPOLLIN;
    regist();
    return S_OK;
}

int TcpChannel::enableWriting()
{
    events_ |= EPOLLOUT;
    update();
    return S_OK;
}

int TcpChannel::disableWriting()
{
    events_ &= ~EPOLLOUT;
    update();
    return S_OK;
}

int TcpChannel::isWriting() { return events_ & EPOLLOUT; }

int TcpChannel::update()
{
    getEventLoop()->update(this);
    return S_OK;
}

int TcpChannel::regist()
{
    cout << "TcpChannel::regist" << endl;

    getEventLoop()->regist(this);
    return S_OK;
}

int TcpChannel::del()
{
    getEventLoop()->del(this);
    return S_OK;
}

int TcpChannel::getFd() { return sockfd_; }

} // namespace net
} // namespace ndsl
