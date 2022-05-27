//GNU GPL 3.0 lisence
/*
 * 정보 수행평가 게임
 * Copyright (C) 2022 박찬웅, 김진서, 박지환
 *
 * 이 프로그램은 자유 소프트웨어입니다. 소프트웨어의 피양도자는 자유 소프트웨어
 * 재단이 공표한 GNU 일반 공중 사용 허가서 3판 혹은 그 이후 판을 임의로 선택하여
 * 그 규정에 따라 프로그램을 개작하거나 재배포할 수 있습니다.
 *
 * 이 프로그램은 유용하게 사용될 수 있으리라는 희망에서 배포되고 있지만, 특정한
 * 목적에 맞는 적합성 여부나 판매용으로 사용할 수 있으리라는 묵시적인 보증을 포함한
 * 어떠한 형태의 보증도 제공하지 않습니다. 보다 자세한 사항에 대해서는
 * GNU 일반 공중 허가서를 참고하시기 바랍니다.
 *
 * GNU 일반 공중 사용 허가서는 이 프로그램과 함께 제공됩니다. 만약 문서가 누락되어있다면
 * <http://www.gnu.org/licenses/>을 참조하시기 바랍니다.
 */


//네트워크 관련 기본 함수들을 모아놓은 헤더입니다.
//정.말.어.렵.습.니.다. 코드가 알록달록!

//UDP 브로드캐스팅으로 자신의 내부 아이피를 내부 아이피의 다른 상대방에게 알리며,
//TCP 소켓을 연결해서 턴제 게임을 진행하고자 하였습니다.

#pragma once

/// <summary>
/// IPV4주소 형식입니다. 000.000.000.000 이거
/// </summary>
struct IPV4
{
	int b1;
	int b2;
	int b3;
	int b4;

	IPV4(int b1, int b2, int b3, int b4)
	{
		this->b1 = b1;
		this->b2 = b2;
		this->b3 = b3;
		this->b4 = b4;
	}
};

/// <summary>
/// 주어진 포트로 로컬 네트워크상의 모든 컴퓨터를 대상으로 하는 UDP 브로드캐스트를 송신합니다.
/// </summary>
/// <param name="message">보낼 메시지</param>
/// <returns>정상적으로 수행되었다면 0이 반환됩니다.</returns>
extern int sendUDPBroadcast(const char* message, int port);

/// <summary>
/// 주어진 포트로 들어오는 UDP 브로드캐스트를 수신합니다.
/// </summary>
/// <param name="buffer">메세지를 저장할 버퍼</param>
/// <param name="ip">송신 아이피를 저장할 포인터</param>
/// <param name="timeout">수신 타임아웃 (ms)</param>
/// <returns>수신값이 존재한다면 0, 없다면 -1을 반환합니다.</returns>
extern int receiveUDPBroadcast(char* buffer, IPV4* ip, int timeout, int port);


/// <summary>
/// 서버로써 TCP요청을 받아 데이터를 교환합니다.
/// </summary>
/// <param name="message">보낼 메시지</param>
/// <param name="buffer">답장을 받을 버퍼</param>
/// <param name="bufferSize">그 버퍼의 사이즈</param>
/// <param name="clientIP">클라이언트의 아이피를 저장할 IPV4 포인터</param>
/// <param name="port">통신할 포트</param>
/// <returns>정상적으로 실행되었다면 0이 반환됩니다.</returns>
int serveTCP(const char* message, char* buffer, int bufferSize, IPV4* clientIP, int port);

/// <summary>
/// 클라이언트로써 서버에 TCP요청을 보냅니다.
/// </summary>
/// <param name="message">보낼 메시지</param>
/// <param name="buffer">답장을 받을 버퍼</param>
/// <param name="bufferSize">그 버퍼의 사이즈</param>
/// <param name="serverIP">보낼 서버의 아이피</param>
/// <param name="port">통신할 포트</param>
/// <returns>정상적으로 실행되었다면 0이 반환됩니다.</returns>
int requestTCP(const char* message, char* buffer, int bufferSize, IPV4 serverIP, int port);
