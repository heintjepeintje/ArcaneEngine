#include "WindowsSocket.hpp"

#define AR_WINDOWS_ADDRESS_FAMILY AF_INET
#define AR_WINDOWS_SOCKTYPE SOCK_STREAM
#define AR_WINDOWS_PROTOCOL IPPROTO_TCP

#define AR_SWITCH_MAP(case, ret) case ##case: return ##ret

namespace Arcane {

	bool sWSAInitialized = false;
	WSADATA sWSAData;

	void InitWSA() {
		WORD version = MAKEWORD(2, 2);

		AR_WINDOWS_ASSERT(WSAStartup(version, &sWSAData) == 0, "Failed to initialize WSA: %s", GetWindowsErrorMessageString(WSAGetLastError()).c_str());
		sWSAInitialized = true;
	}
	
	WindowsSocket::WindowsSocket() : mSocket(INVALID_SOCKET) {
		if (!sWSAInitialized) InitWSA();
	}

	WindowsSocket::WindowsSocket(SOCKET socket) : mSocket(socket) {
		if (!sWSAInitialized) InitWSA();
	}

	WindowsSocket::~WindowsSocket() {
		Close();
	}

	void WindowsSocket::Connect(const std::string &address, uint32_t port) {
		AR_ASSERT(mSocket == INVALID_SOCKET, "Socket is already bound or connected");
		
		struct addrinfo *results = NULL;
		struct addrinfo hints = {};

		hints.ai_family = AR_WINDOWS_ADDRESS_FAMILY;
		hints.ai_socktype = AR_WINDOWS_SOCKTYPE;
		hints.ai_protocol = AR_WINDOWS_PROTOCOL;

		AR_WINDOWS_ASSERT(getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &results) == 0, "Failed to resolve addrinfo: %s", GetWindowsErrorMessageString(WSAGetLastError()).c_str());

		mSocket = socket(AR_WINDOWS_ADDRESS_FAMILY, SOCK_STREAM, AR_WINDOWS_PROTOCOL);

		AR_WINDOWS_ASSERT(mSocket != INVALID_SOCKET, "Failed to create socket: %s", GetWindowsErrorMessageString(WSAGetLastError()).c_str());

		AR_WINDOWS_ASSERT(connect(mSocket, results->ai_addr, (int)results->ai_addrlen) != SOCKET_ERROR, "Failed to connect socket: %s", GetWindowsErrorMessageString(WSAGetLastError()).c_str());
		
		freeaddrinfo(results);
	}

	void WindowsSocket::Bind(const std::string &address, uint32_t port) {
		AR_ASSERT(mSocket == INVALID_SOCKET, "Socket is already bound or connected");

		struct addrinfo *result = NULL;
		struct addrinfo hints = {};

		hints.ai_family = AR_WINDOWS_ADDRESS_FAMILY;
		hints.ai_socktype = AR_WINDOWS_SOCKTYPE;
		hints.ai_protocol = AR_WINDOWS_PROTOCOL;
		hints.ai_flags = AI_PASSIVE;

		AR_WINDOWS_ASSERT(getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &result) == 0, "Failed to resolve addrinfo: %s", GetWindowsErrorMessageString(WSAGetLastError()).c_str());

		mSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		AR_WINDOWS_ASSERT(mSocket != INVALID_SOCKET, "Failed to create socket: %d", WSAGetLastError());

		AR_WINDOWS_ASSERT(bind(mSocket, result->ai_addr, (int)result->ai_addrlen) != SOCKET_ERROR, "Failed to bind socket: %s", GetWindowsErrorMessageString(WSAGetLastError()).c_str());

		freeaddrinfo(result);
	}

	void WindowsSocket::Close() {
		if (mSocket == INVALID_SOCKET) return;
		shutdown(mSocket, SD_BOTH);
		closesocket(mSocket);
		mSocket = INVALID_SOCKET;
	}

	void WindowsSocket::Listen(uint32_t backlog) {
		AR_WINDOWS_ASSERT(listen(mSocket, SOMAXCONN) != SOCKET_ERROR, "Failed to listen on socket: %s", GetWindowsErrorMessageString(WSAGetLastError()).c_str());
	}

	Ref<NativeSocket> WindowsSocket::Accept() {
		SOCKET clientSocket = accept(mSocket, NULL, NULL);
		AR_WINDOWS_ASSERT(clientSocket != INVALID_SOCKET, "Failed to accept connection: %s", GetWindowsErrorMessageString(WSAGetLastError()).c_str());

		return CastRef<NativeSocket>(CreateRef<WindowsSocket>(clientSocket));
	}

	void WindowsSocket::Send(const void *data, size_t size) {
		AR_WINDOWS_ASSERT(mSocket != INVALID_SOCKET, "Socket is not connected");
		AR_WINDOWS_ASSERT(send(mSocket, static_cast<const char*>(data), static_cast<int>(size), 0) != SOCKET_ERROR, "Failed to send data: %s", GetWindowsErrorMessageString(WSAGetLastError()).c_str());
	}

	size_t WindowsSocket::Receive(void *buffer, size_t size) {
		AR_WINDOWS_ASSERT(mSocket != INVALID_SOCKET, "Socket is not connected");

		int bytesReceived = recv(mSocket, static_cast<char*>(buffer), static_cast<int>(size), 0);
		AR_WINDOWS_ASSERT(bytesReceived != SOCKET_ERROR, "Failed to receive data: %s", GetWindowsErrorMessageString(WSAGetLastError()).c_str());
		
		return bytesReceived;
	}

	size_t WindowsSocket::GetBytesAvailable() const {
		AR_WINDOWS_ASSERT(mSocket != INVALID_SOCKET, "Socket is not connected");

		u_long bytesAvailable = 0;
		AR_WINDOWS_ASSERT(ioctlsocket(mSocket, FIONREAD, &bytesAvailable) != SOCKET_ERROR, "Failed to get bytes available: %s", GetWindowsErrorMessageString(WSAGetLastError()).c_str());
		return static_cast<size_t>(bytesAvailable);
	}

}