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

int TcpChannel::onRead(void *inBuf)
{
    // 在这相应的做用户调用onRecv的处理
    if (callOnRecv) {
        pCon_->sRecvParam_.buffer = static_cast<char *>(inBuf);
        pCon_->sRecvParam_.length = strlen(static_cast<char *>(inBuf));

        // 塞给loop做回调的处理
        struct work_struct *recvStruct = new struct work_struct();
        recvStruct->doit = pCon_->sRecvParam_.cb;
        recvStruct->param = pCon_->sRecvParam_.para;
        pLoop_->addWork(recvStruct);
    } else {
        //强制类型转换成实际的类
        pCon_->send(static_cast<char *>(inBuf));
        return S_OK;

        // 测试可以在这动手脚
        // char temp[MAXLINE] = "received";
        // ((TcpConnection *) pCallBack_)->send(temp);
        // return S_OK;
    }
}

int TcpChannel::onWrite(void *param) { return S_OK; }

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
