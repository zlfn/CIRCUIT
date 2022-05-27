//GNU GPL 3.0 lisence
/*
 * ���� ������ ����
 * Copyright (C) 2022 ������, ������, ����ȯ
 *
 * �� ���α׷��� ���� ����Ʈ�����Դϴ�. ����Ʈ������ �Ǿ絵�ڴ� ���� ����Ʈ����
 * ����� ��ǥ�� GNU �Ϲ� ���� ��� �㰡�� 3�� Ȥ�� �� ���� ���� ���Ƿ� �����Ͽ�
 * �� ������ ���� ���α׷��� �����ϰų� ������� �� �ֽ��ϴ�.
 *
 * �� ���α׷��� �����ϰ� ���� �� ��������� ������� �����ǰ� ������, Ư����
 * ������ �´� ���ռ� ���γ� �Ǹſ����� ����� �� ��������� �������� ������ ������
 * ��� ������ ������ �������� �ʽ��ϴ�. ���� �ڼ��� ���׿� ���ؼ���
 * GNU �Ϲ� ���� �㰡���� �����Ͻñ� �ٶ��ϴ�.
 *
 * GNU �Ϲ� ���� ��� �㰡���� �� ���α׷��� �Բ� �����˴ϴ�. ���� ������ �����Ǿ��ִٸ�
 * <http://www.gnu.org/licenses/>�� �����Ͻñ� �ٶ��ϴ�.
 */


//��Ʈ��ũ ���� �⺻ �Լ����� ��Ƴ��� ����Դϴ�.
//��.��.��.��.��.��.��. �ڵ尡 �˷ϴ޷�!

//UDP ��ε�ĳ�������� �ڽ��� ���� �����Ǹ� ���� �������� �ٸ� ���濡�� �˸���,
//TCP ������ �����ؼ� ���� ������ �����ϰ��� �Ͽ����ϴ�.

#pragma once

/// <summary>
/// IPV4�ּ� �����Դϴ�. 000.000.000.000 �̰�
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
/// �־��� ��Ʈ�� ���� ��Ʈ��ũ���� ��� ��ǻ�͸� ������� �ϴ� UDP ��ε�ĳ��Ʈ�� �۽��մϴ�.
/// </summary>
/// <param name="message">���� �޽���</param>
/// <returns>���������� ����Ǿ��ٸ� 0�� ��ȯ�˴ϴ�.</returns>
extern int sendUDPBroadcast(const char* message, int port);

/// <summary>
/// �־��� ��Ʈ�� ������ UDP ��ε�ĳ��Ʈ�� �����մϴ�.
/// </summary>
/// <param name="buffer">�޼����� ������ ����</param>
/// <param name="ip">�۽� �����Ǹ� ������ ������</param>
/// <param name="timeout">���� Ÿ�Ӿƿ� (ms)</param>
/// <returns>���Ű��� �����Ѵٸ� 0, ���ٸ� -1�� ��ȯ�մϴ�.</returns>
extern int receiveUDPBroadcast(char* buffer, IPV4* ip, int timeout, int port);


/// <summary>
/// �����ν� TCP��û�� �޾� �����͸� ��ȯ�մϴ�.
/// </summary>
/// <param name="message">���� �޽���</param>
/// <param name="buffer">������ ���� ����</param>
/// <param name="bufferSize">�� ������ ������</param>
/// <param name="clientIP">Ŭ���̾�Ʈ�� �����Ǹ� ������ IPV4 ������</param>
/// <param name="port">����� ��Ʈ</param>
/// <returns>���������� ����Ǿ��ٸ� 0�� ��ȯ�˴ϴ�.</returns>
int serveTCP(const char* message, char* buffer, int bufferSize, IPV4* clientIP, int port);

/// <summary>
/// Ŭ���̾�Ʈ�ν� ������ TCP��û�� �����ϴ�.
/// </summary>
/// <param name="message">���� �޽���</param>
/// <param name="buffer">������ ���� ����</param>
/// <param name="bufferSize">�� ������ ������</param>
/// <param name="serverIP">���� ������ ������</param>
/// <param name="port">����� ��Ʈ</param>
/// <returns>���������� ����Ǿ��ٸ� 0�� ��ȯ�˴ϴ�.</returns>
int requestTCP(const char* message, char* buffer, int bufferSize, IPV4 serverIP, int port);
