#pragma once

/// <summary>
/// 게임의 장면 목록입니다.
/// </summary>
enum Scenes
{
	Main,
};

struct Settings
{
	/// <summary>
	/// 렌더링 스레드 개수
	/// <para>렌더링에 멀티스레딩을 이용할때 스레드의 개수입니다.</para>
	/// <para>2개 이상의 스레드를 이용하면 화면이 불안정해지지만 10%내외의 프레임률 향상이 있습니다.</para>
	/// </summary>
	int renderThreadsCount;

	/// <summary>
	/// 병렬 리프레시
	/// <para>화면 리프레싱을 메인 스레드와 병렬로 수행할 것인지 여부입니다.</para>
	/// <para>이용할 경우 화면이 불안정해지고 화면 리프레싱이 여러 단계에 걸쳐서 일어나지만,</para>
	/// <para>리프레시시 일어나는 간헐적인 프레임 드랍이 최소화됩니다.</para>
	/// </summary>
	bool parallelRefresh;

	/// <summary>
	/// 리프레시 간격
	/// <para>화면 리프레싱의 간격을 조정합니다.</para>
	/// <para>기본적으로 초 단위입니다.</para>
	/// </summary>
	int refreshInterval;

	/// <summary>
	/// 리프레시 스레드 개수
	/// <para>리프레시에 멀티 스레드를 사용할 시 스레드의 개수입니다.</para>
	/// <para>리프레시 과정 자체가 상당히 불안정하므로, 병렬 리프세시 사용시 2개이상의 스레드 사용을 추천하지 않습니다.</para>
	/// </summary>
	int refreshThreadsCount;

	/// <summary>
	/// 여백없는 창
	/// <para>창에 여백이 없도록 출력하여, 스크롤바를 감춥니다.</para>
	/// <para>활성화 할 경우 릴리스 빌드에서 버그가 발생하여 기본적으로 비활성화됩니다.</para>
	/// </summary>
	bool noSpaceWindow;
};

/// <summary>
/// 게임의 전역상태 변수입니다.
/// 장면간 데이터 전달, 애니메이션, 게임 상태등의 
/// 전역변수가 필요한 모든 상황에서 이용됩니다.
/// </summary>
struct GameState
{
	Scenes scene;
	Settings setting;
};
