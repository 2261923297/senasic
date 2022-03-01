#include "Socket.h"
#include "HandleError.h"

#include <time.h>
#include <errno.h>

#define ERR_DESC STR_LINE_LOCAL(std::string("err code = ")      \
                        + std::to_string(WSAGetLastError()))    \
//

namespace senasic {
namespace system {
namespace tt { 

TcpPath::TcpPath() {
    m_addr = new std::string;
    m_port = 8080;
}

Socket::Socket()
{
    m_he = nullptr;
}

Socket::~Socket()
{
    set_handle_error(nullptr);
}

bool Socket::socket()
{
    int ret = _socket();
    int rt = ret != INVALID_SOCKET;

    m_he->handle_error(!rt, ERR_DESC);
    return rt;
}

bool Socket::send(const std::string& str)
{
    int ret = _send(str);
    int rt = ret != SOCKET_ERROR;

    m_he->handle_error(!rt, ERR_DESC);
    return rt;
}

bool Socket::recv(std::string& str)
{
    int ret = _recv(str);
    int rt = ret != SOCKET_ERROR;

    m_he->handle_error(!rt, ERR_DESC);
    return rt;
}

bool Socket::connect(const Path* addr)
{
    close();
    socket();
    int ret = _connect(addr);
    int rt = ret != SOCKET_ERROR;

    m_he->handle_error(!rt, ERR_DESC);
    return rt;
}

bool Socket::reconnect(uint32_t n_msec)
{
    int ret = _reconnect(n_msec);
    int rt = ret != SOCKET_ERROR;

    m_he->handle_error(!rt, ERR_DESC, 0);
    return rt;
}

bool Socket::bind(const Path* addr)
{
    int ret = _bind(addr);
    int rt = ret != SOCKET_ERROR;

    m_he->handle_error(!rt, ERR_DESC, 0);
    return rt;
}

bool Socket::close()
{
    int ret = _close();
    int rt = ret != SOCKET_ERROR;

    m_he->handle_error(!rt, ERR_DESC, 0);
    return rt;
}

void HandleSocketError::handle()
{
    m_sock->reconnect(3000);
}

void Socket::set_handle_error(HandleError* he)
{
    if (m_he != nullptr) {
        delete m_he;
        m_he = nullptr;
    }
    printf("Socket::set_handle_error he: %x\n", he);
    m_he = he;
    if (m_he != nullptr)
        m_he->set_args(this);
}

TcpSocket::TcpSocket()
{
    m_sock = 0;
    memset(m_buffer, 0, 1024);

    m_path = new TcpPath;
    m_path->set_addr("0.0.0.0");
    m_path->set_port(8080);

    m_bind_path = new TcpPath;
    m_path->set_addr("0.0.0.0");
    m_path->set_port(8081);
}

TcpSocket::~TcpSocket()
{
    if(m_path != nullptr)
        delete m_path;
    if (m_bind_path != nullptr)
        delete m_bind_path;
    close();
}

int TcpSocket::_socket() {
    m_sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return (int)m_sock;
}

int TcpSocket::_close()
{
    int rt = 0;
    if (m_sock)
        rt = closesocket(m_sock);
    m_sock = 0;
    return rt;
}

void TcpSocket::set_path(const Path* path)
{
    if (m_path == nullptr) {
        m_path = new TcpPath;
    }
    const TcpPath* tp = (const TcpPath*)path;
    m_path->set_addr(tp->get_addr());
    m_path->set_port(tp->get_port());
}

const Path* TcpSocket::get_path() const
{
    return m_path;
}

int TcpSocket::_send(const std::string& str)
{
    int rt = ::send(m_sock, str.c_str(), str.size(), 0);
	return (int)rt;
}
int TcpSocket::_recv(std::string& str)
{
    memset(m_buffer, 0, 1024);
    int rt = ::recv(m_sock, m_buffer, 1024, 0);
    str = m_buffer;
	return rt;
}
int TcpSocket::_connect(const Path* addr)
{
    set_path(addr);

    sockaddr_in sock_in;
    sock_in.sin_family = AF_INET;
    sock_in.sin_port = htons(m_path->get_port());
    sock_in.sin_addr.S_un.S_addr = 
        inet_addr(m_path->get_addr().c_str());

    int rt = 0;
    rt = ::connect(m_sock
        , (sockaddr*)&sock_in
        , sizeof(sock_in));
    
    if (SOCKET_ERROR == rt) {
        char buffer[256] = { 0 };
        fprintf(stdout, "can^t connect %s:%d %s\n"
            , m_path->get_addr().c_str()
            , m_path->get_port()
            , strerror(errno));
    }
    else {
        fprintf(stdout, "connected %s:%d\n"
            , m_path->get_addr().c_str()
            , m_path->get_port());
    }
    return rt;
}

// 每500毫秒connect一次
int TcpSocket::_reconnect(uint32_t n_msec)
{
    int time_beg = clock();
    while ((uint32_t)(clock() - time_beg) <= n_msec)
    {
        Sleep(500);
        if (-1 != this->_connect(m_path)) {
            time_beg = 0;
            break;
        }         
    }
    if (time_beg == 0)
        return 0;
	return -1;
}

int TcpSocket::_bind(const Path* addr)
{
    //绑定套接字
    sockaddr_in sockAddr;
    sockAddr.sin_family = 
        inet_addr(m_bind_path->get_addr().c_str());
    sockAddr.sin_port = htons(m_bind_path->get_port());
    sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;

    if (::bind(m_sock, (sockaddr*)&sockAddr
        , sizeof(sockAddr)) == SOCKET_ERROR) {
        return -1;
    }
	return 0;
} 

} // namespace tt
} // namespace system
} // namespace senasic

void init_win_network()
{
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsdata;
    if (WSAStartup(sockVersion, &wsdata) != 0)
    {
        return;
    }

    
}

void destory_win_network()
{
    WSACleanup();
}
