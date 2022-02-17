#include "Socket.h"
#include <time.h>
#include <errno.h>

TcpPath::TcpPath() {
    m_addr = new std::string;
    m_port = 8080;
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
    close();
}

void TcpSocket::init() {
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    handle_error(m_sock == INVALID_SOCKET
        , (std::string(__FILE__) + ":" __FUNCTION__ + ":").c_str()
        , __LINE__);
}

int TcpSocket::close()
{
    if (m_sock)
        closesocket(m_sock);
    m_sock = 0;
    return 0;
}


int TcpSocket::send(const std::string& str)
{
    int rt = ::send(m_sock, str.c_str(), str.size(), 0);
    fprintf(stdout, "send rt = %d, sock = %d %d\n"
        , rt, m_sock, WSAGetLastError());
    handle_error(rt == SOCKET_ERROR
        , (std::string(__FILE__) + ":" __FUNCTION__ + ":").c_str()
        , __LINE__);
	return rt;
}
int TcpSocket::recv(std::string& str)
{
    memset(m_buffer, 0, 1024);
    int rt = ::recv(m_sock, m_buffer, 1024, 0);
    str = m_buffer;
    fprintf(stdout, "recv rt = %d, sock = %d %d\n"
        , rt, m_sock, WSAGetLastError());
    handle_error(rt == SOCKET_ERROR
        , (std::string(__FILE__) + ":" __FUNCTION__ + ":").c_str()
        , __LINE__);
	return rt;
}



int TcpSocket::connect(Path* addr)
{
    close();
    init();

    *m_path = *(TcpPath*)addr;

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
int TcpSocket::reconnect(uint32_t n_msec)
{
    int time_beg = clock();
    while (clock() - time_beg <= n_msec)
    {
        Sleep(500);
        if (-1 != this->connect(m_path)) {
            time_beg = 0;
            break;
        }         
    }
    if (time_beg == 0)
        return 0;
	return -1;
}

int TcpSocket::bind(Path* addr)
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

void TcpSocket::handle_error(int is_error, const char* msg, int line)
{
    if (is_error) {
        fprintf(stdout, "[TcpSocket ERROR] %s:%d > %s\n"
            , msg, line, strerror(errno));
        if (-1 == reconnect(3000)) {
            fprintf(stdout, "reconnect 3 sec faild!\n");
        }
    }
} 




