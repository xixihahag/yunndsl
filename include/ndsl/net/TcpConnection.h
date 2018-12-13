/*
 * File: TcpConnection.h
 * brief:
 *
 * Author: gyz
 * Email: mni_gyz@163.com
 * Last Modified: Wednesday, 28th November 2018 10:57:51 pm
 */

#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__
#include "TcpChannel.h"
#include "EventLoop.h"
#include "ChannelCallBack.h"
#include "Multiplexer.h"
#include "../utils/temp_define.h"

namespace ndsl {
namespace net {

class TcpChannel;

class TcpConnection
{
  private:
    TcpChannel *pTcpChannel_;
    char inBuf_[MAXLINE];
    char outBuf_[MAXLINE];

    int createChannel(int sockfd_, EventLoop *pLoop);

  public:
    TcpConnection(int sockfd, EventLoop *pLoop);
    ~TcpConnection();

    int handleRead();
    int handleWrite();
    int send(char *outBuf);

    // 判断用户是否调用了onRecv
    // bool callOnRecv;

    // // 用户调用onRecv后的参数放在这
    // struct onRecvParam
    // {
    //     char *buffer;
    //     int &length;
    //     Callback cb;
    //     void *param;
    // } sRecvParam_;

    // 提供给用户的发送接口

    // 原型 ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
    int onSendmsg(
        ssize_t &length,
        const struct msghdr *msg,
        int flags,
        Callback cb,
        void *param)
    {}

    // onSend onRecv 的语义是异步通知

    // 提供给用户的接收接口
    // onRecv客户端调用
    // 调用后，如果有数据，则loop回调onRecv参数的回调函数
    int onRecv(char *buffer, int &length, Callback cb, void *param);

    // onSend，如果系统buffer够，则会立即发送
    // 如果系统buffer不够，则需要回调，在回调之前，无法使用bufferz
    ssize_t onSend(
        int sockfd,
        const void *buf,
        size_t len,
        int flags,
        Callback cb,
        void *param);

    // 不知道要干嘛
    int onAccept(
        int sockfd,
        struct sockaddr *addr,
        socklen_t *addrlen,
        Callback cb,
        void *param);
};

} // namespace net
} // namespace ndsl

#endif // __TCPCONNECTION_H__