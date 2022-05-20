#pragma once

/// <summary>
/// 게임의 장면 목록입니다.
/// </summary>
enum Scenes
{
	Main,
};

/// <summary>
/// 게임의 전역상태 변수입니다.
/// 장면간 데이터 전달, 애니메이션, 게임 상태등의 
/// 전역변수가 필요한 모든 상황에서 이용됩니다.
/// </summary>
struct GameState
{
	Scenes scene;
};
