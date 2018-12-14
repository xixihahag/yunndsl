/*
 * @file TcpConnection.cc
 * @brief
 *
 * @author gyz
 * @email mni_gyz@163.com
 */
#include "ndsl/net/TcpConnection.h"
#include "ndsl/utils/temp_define.h"
#include "ndsl/net/Multiplexer.h"
#include "ndsl/net/CallbackIoChannel.h"
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

namespace ndsl {
namespace net {

TcpConnection::TcpConnection(int sockfd, EventLoop *pLoop)
{
    createChannel(sockfd, pLoop);
}
TcpConnection::~TcpConnection() {}

int TcpConnection::createChannel(int sockfd, EventLoop *pLoop)
{
    pTcpChannel_ = new TcpChannel(sockfd, pLoop);
    pTcpChannel_->enableReading();
    pTcpChannel_->setCallBack(this);

    cout << "TcpConnection::createChannel" << endl;

    return S_OK;
}

int TcpConnection::handleRead()
{
    memset(inBuf_, 0, sizeof(inBuf_));
    int sockfd = pTcpChannel_->getFd();
    if (sockfd < 0) { return -1; }
    int length;
    char line[MAXLINE];
    memset(line, 0, sizeof(line));
    if ((length = read(sockfd, line, MAXLINE)) < 0) {
        if (errno == ECONNRESET) {
            // cout << "ECONNREST closed socket fd:" << sockfd << endl;
            close(sockfd);
        }
    } else if (length == 0) {
        // cout << "read 0 closed socket fd:" << sockfd << endl;
        close(sockfd);
    } else {
        strcat(inBuf_, line);
        if (onRead) pTcpChannel_->onRead(inBuf_);
    }
    // 设置非阻塞
    fcntl(sockfd_, F_SETFL, O_NONBLOCK);
    return S_OK;
}

int TcpConnection::handleWrite()
{
    int sockfd = pTcpChannel_->getFd();
    if (sockfd < 0) { return -1; }
    int n, length;
    char temp[MAXLINE];
    memset(temp, 0, sizeof(temp));
    length = strlen(outBuf_);
    if (pTcpChannel_->isWriting()) {
        n = write(sockfd, outBuf_, strlen(outBuf_));
        if (n == length) {
            // log(write n bytes);
            pTcpChannel_->disableWriting();
            if (onWrite) pTcpChannel_->onWrite();
        } else {
            // 这一次还没传完
            strncpy(temp, outBuf_ + n, length - n);
            memset(outBuf_, 0, sizeof(outBuf_));
            strcpy(outBuf_, temp);
        }
    }
    return S_OK;
}

int TcpConnection::send(char *outBuf)
{
    int sockfd = pTcpChannel_->getFd();
    if (sockfd < 0) { return -1; }
    int length = strlen(outBuf);
    int n = 0;
    if ((n = write(sockfd, outBuf, strlen(outBuf))) < 0) {
        // log(write error);
    } else if (n == length) {
        if (onWrite) pTcpChannel_->onWrite();
    } else if (n < length) {
        // 一次write没写完
        if (!pTcpChannel_->isWriting()) pTcpChannel_->enableWriting();
        strcat(outBuf_, outBuf + n);
        // memset(outBuf, 0, sizeof(outBuf));
    }
    return S_OK;
}

ssize_t TcpConnection::onSend(
    int sockfd,
    const void *buf,
    size_t len,
    int flags,
    Callback cb,
    void *param)
{
    // Memory leak
    CallbackIoChannel *ioChannel = new CallbackIoChannel(
        sockfd, buf, len, flags, cb, param, pTcpChannel_->getEventLoop());
    ioChannel->enableWriting();
}

// 实现个用户的接口
int TcpConnection::onRecv(char *buffer, int &length, Callback cb, void *param)
{
    // Memory leak
    CallbackIoChannel *ioChannel = new CallbackIoChannel(
        pTcpChannel_->getFd(),
        buffer,
        length,
        -1,
        cb,
        param,
        pTcpChannel_->getEventLoop());
    ioChannel->enableReading();

    // 先返回，最终的处理在onRead()里面
    return S_OK;
}

int onSendmsg(
    ssize_t &length,
    const struct msghdr *msg,
    int flags,
    Callback cb,
    void *param)
{}

} // namespace net
} // namespace ndsl
