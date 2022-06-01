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

//게임에 쓰이는 타일들에 관한 함수와 데이터형이 모여있는 헤더입니다.

#pragma once
#include "Tiles.h"
#include "Graphic.h"
#include "Chars.h"

/// <summary>
/// 타일의 종류를 핸들하는 열거형입니다.
/// </summary>
enum Tile
{
	LR,
	UD,
	UR,
	RD,
	LD,
	UL,
	BLANK,
};

/// <summary>
/// 이번턴에 설치한 타일을 저장하기 위한 구조체입니다.
/// </summary>
struct RecentTile
{
	int x;
	int y;
	Tile type;
};

/// <summary>
/// 타일이 열려있는 방향을 이용하기 위한 열거형입니다.
/// </summary>
enum Direction
{
	U = -2, L = -1, R = 1, D = 2
};

/// <summary>
/// 회로가 완성되었는지 확인합니다.
/// </summary>
/// <param name="tiles">10*7 타일 배열</param>
/// <param name="placed">총 설치한 타일 개수</param>
/// <returns>회로 완성 여부가 반환됩니다.</returns>
bool isCompleted(Tile (*tiles)[7], int placed);

/// <summary>
/// 타일의 그래픽 리소스 파일명을 얻습니다.
/// </summary>
/// <param name="t">타일 종류</param>
/// <returns>그래픽 리소스 파일 명</returns>
const wchar* getTileName(Tile t);

/// <summary>
/// 타일의 노란색버전 그래픽 리소스 파일명을 얻습니다.
/// </summary>
/// <param name="t">타일 종류</param>
/// <returns>그래픽 리소스 파일명</returns>
const wchar* getTileNameYellow(Tile t);

/// <summary>
/// 특정 위치에 타일을 그립니다.
/// </summary>
/// <param name="t">타일 종류</param>
/// <param name="buf">그릴 버퍼</param>
/// <param name="x">타일 좌표 (타일 좌표계 기준)</param>
/// <param name="y">타일 좌표 (타일 좌표계 기준)</param>
/// <returns>정상적으로 실행되었다면 0이 반환됩니다.</returns>
int drawTile(Tile t, Buffer buf, int x, int y);

/// <summary>
/// 특정 위치에 노란색 타일을 그립니다.
/// </summary>
/// <param name="t">타일 종류</param>
/// <param name="buf">그릴 버퍼</param>
/// <param name="x">타일 좌표 (타일 좌표계 기준)</param>
/// <param name="y">타일 좌표 (타일 좌표계 기준)</param>
/// <returns>정상적으로 실행되었다면 0이 반환됩니다.</returns>
int drawTileYellow(Tile t, Buffer buf, int x, int y);

/// <summary>
/// 특정 위치에 타일을 그리며, 타일의 상하좌우 타일을 인식하여 타일들의 리소스를 연결합니다.
/// </summary>
/// <param name="t">타일 종류</param>
/// <param name="buf">그릴 버퍼</param>
/// <param name="x">타일 좌표</param>
/// <param name="y">타일 좌표</param>
/// <param name="up">위쪽 타일 종류</param>
/// <param name="left">왼쪽 타일 종류</param>
/// <param name="right">오른쪽 타일 종류</param>
/// <param name="down">아래쪽 타일 종류</param>
/// <param name="placed">설치한 타일 개수 (빈칸의 오브젝트 id 할당 체크용)</param>
/// <returns>정상적으로 실행되었다면 0이 반환됩니다.</returns>
int drawTile(Tile t, Buffer buf, int x, int y, Tile up, Tile left, Tile right, Tile down, int placed);
