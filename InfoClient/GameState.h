#pragma once

/// <summary>
/// ������ ��� ����Դϴ�.
/// </summary>
enum Scenes
{
	Main,
};

/// <summary>
/// ������ �������� �����Դϴ�.
/// ��鰣 ������ ����, �ִϸ��̼�, ���� ���µ��� 
/// ���������� �ʿ��� ��� ��Ȳ���� �̿�˴ϴ�.
/// </summary>
struct GameState
{
	Scenes scene;
};
