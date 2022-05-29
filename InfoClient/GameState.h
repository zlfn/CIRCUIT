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

//게임의 전역 상태와 관련된 내용을 담은 헤더입니다.
/* 이 게임은 통상의 다른 텍스트기반 게임과 달리, 입력시만 화면이 변경되는 방식이 아니라, 미친듯이 렌더링 루프를 돌리며 화면을 바꿉니다.
* 그러면 화면에 어떤 모양을 그려야 할지는 무엇으로 결정해야 할까요? 화면이 한번 돌때마다 메인함수 전체가 순환하는데 말이죠.
* 메인함수안에 조건 분기에분기에분기에분기를 넣어서 결정할까요?
* 이 게임은 이 문제를 전역 게임 상태 변수로 해결합니다. 전역 게임 상태 변수에는 현재 장면이 어떤 장면인지,
* 지금 게임의 상태는 어떤지 등등을 담고 있어 화면을 렌더링하는데 필요한 모든 정보를 제공합니다. */

//전역 상태 변수가 필요 없는 장면 전용 변수들은 그냥 전역 변수를 이용합니다.

#pragma once
#include "Chars.h"
#include "Network.h"

/// <summary>
/// 게임의 장면 목록입니다.
/// </summary>
enum Scenes
{
	Main,
	Setting,
	Host,
	Find,
	Game,
};

/// <summary>
/// 게임의 설정 목록입니다.
/// </summary>
struct Settings
{
	/// <summary>
	/// 렌더링 스레드 개수
	/// <para>렌더링에 멀티스레딩을 이용할때 스레드의 개수입니다.</para>
	/// <para>2개 이상의 스레드를 이용하면 화면이 불안정해지지만 10%내외의 프레임률 향상이 있습니다.</para>
	/// </summary>
	int renderThreadsCount;

	/// <summary>
	/// 리프레시
	/// <para>화면을 주기적으로 리프레싱하여 잔상을 제거합니다.</para>
	/// <para>프론트 버퍼 디더링과는 병행하지 않는 것을 권장합니다.</para>
	/// </summary>
	bool refresh;

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
	/// 프론트 버퍼 디더링
	/// <para>렌더링 과정에 프론트버퍼에 랜덤한 노이즈를 추가합니다.</para>
	/// <para>평균 프레임률이 낮아지지만 프레임 드랍이 전혀 없는 잔상제거가 가능합니다.</para>
	/// <para>리프레시 설정과 병행하지 않는 것을 추천합니다.</para>
	/// </summary>
	bool frontBufferDithering;

	/// <summary>
	/// 디더링 사이즈
	/// <para>디더링 과정에 첨가할 노이즈 수를 결정합니다.</para>
	/// <para>값이 클 수록 잔상제거가 빨라지지만, 평균 프레임률이 낮아집니다.</para>
	/// </summary>
	int ditheringSize;

	/// <summary>
	/// 여백없는 창
	/// <para>창에 여백이 없도록 출력하여, 스크롤바를 감춥니다.</para>
	/// <para>활성화 할 경우 릴리스 빌드에서 버그가 발생하여 기본적으로 비활성화됩니다.</para>
	/// </summary>
	bool noSpaceWindow;

	/// <summary>
	/// FPS 표시
	/// <para>화면 왼쪽 아래에 프레임률을 표시합니다.</para>
	/// </summary>
	bool showFPS;
};

/// <summary>
/// 게임의 전역상태 변수입니다.
/// 장면간 데이터 전달, 게임 상태등의 
/// 전역변수가 필요한 모든 상황에서 이용됩니다.
/// </summary>
struct GameState
{
	Scenes scene;
	Settings setting;

	IPV4 commIP;
	bool turn;
};

/// <summary>
/// 게임 상태를 받아옵니다.
/// </summary>
/// <returns>전역 GameState 주소가 반환됩니다.</returns>
extern GameState* getGameState();

/// <summary>
/// 게임 상태를 초기화합니다.
/// </summary>
/// <returns>정상적으로 초기화 되었다면 0이 반환됩니다.</returns>
extern int resetGameState();
