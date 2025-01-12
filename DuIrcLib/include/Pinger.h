#pragma once
#include <winsock2.h>
#include <iphlpapi.h>
#include <windows.h>
#include <icmpapi.h>
#include <string_view>
#include <array>
#include <thread>
#include <iostream>
#include <atomic>

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Ws2_32.lib")
class Pinger final
{
	HANDLE m_icmpFileHande;
	std::atomic<bool> m_stopPing;

	static void ping(std::string&& ip, Pinger& pinger)
	{
		static constexpr int dataSize = 64;
		uint8_t dataToSend[dataSize];
		static constexpr DWORD replySize = sizeof(ICMP_ECHO_REPLY) + dataSize;
		std::array<uint8_t, replySize> reply = {};


		auto saveCursorPosition = [](COORD& pos)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
					pos = csbi.dwCursorPosition;
				}
			};

		auto restoreCursorPosition = [](COORD& pos)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleCursorPosition(hConsole, pos);
			};


		uint64_t it = 0;
		while (!pinger.m_stopPing.load(std::memory_order_acquire))
		{
			if (pinger.m_icmpFileHande == INVALID_HANDLE_VALUE) {
				return;
			}
			memset(dataToSend, 0, dataSize);
			memset(reply.data(), 0, replySize);
			IP_OPTION_INFORMATION optionInfo = {};
			auto _ip = inet_addr(ip.c_str());
			if (_ip == INADDR_NONE)
			{
				std::cerr << ip.c_str() << " : " << "INADDR_FAILED\n";
				pinger.stopPing();
				continue;
			}
			DWORD ret = IcmpSendEcho2(pinger.m_icmpFileHande, NULL, NULL, NULL, _ip, (PVOID)dataToSend, dataSize, NULL, reply.data(), replySize, 10000);

			if (ret)
			{
				ICMP_ECHO_REPLY* _reply = reinterpret_cast<ICMP_ECHO_REPLY*>(reply.data());
				COORD cursorPos;
				saveCursorPosition(cursorPos);
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),{0, static_cast<short>(cursorPos.Y - 1)});
				std::cout << "Reply from " << ip << ": bytes=" << _reply->DataSize
					<< " time=" << _reply->RoundTripTime << "ms TTL=" << _reply->Options.Ttl << " it = " << it++ << std::endl;
				restoreCursorPosition(cursorPos);
			}
			if (ret == 0) {
				DWORD error = GetLastError();
				std::cerr << "IcmpSendEcho failed with error code " << error << std::endl;
			}
			Sleep(10);
		}
	}
public:
	Pinger(std::string ip) : m_icmpFileHande(NULL)
	{
		m_stopPing.store(false, std::memory_order_relaxed);
		m_icmpFileHande = IcmpCreateFile();
		std::thread t(ping, std::move(ip), std::ref(*this));
		t.detach();
	}
	void stopPing()
	{
		m_stopPing.store(true, std::memory_order_release);
	}
};