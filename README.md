# CIRCUIT
<img style="display:inline" src="https://github.com/zlfn/CIRCUIT/assets/33383685/ec472486-426f-4b1d-8b9a-efc83ba1da14" width="400" height="400">
<img style="display:inline" src="https://github.com/zlfn/CIRCUIT/assets/33383685/276bd056-9c35-4ad1-acac-4f811245fddc" width="400" height="400">


경기북과학고 정보 수행평가로 제작한 게임  
더 지니어스의 **모노레일**로도 잘 알려진 보드게임, **베니치아 커넥션**을 ~~배껴~~ 모티브를 얻어 만들었습니다.  
전반적으로 **학교에서 배운 C언어 만으로 어디까지 할 수 있냐** 에 도전한 게임인데 Windows API 쓰는 것 부터 이미 탈락인 것 같기는 하네요.  

혹시라도 이 코드를 해석할 사람들이나 미래의 저를 위해 아래 간단한 파일별 설명을 남깁니다.
### main.cpp
```
//사용한 VisualStudio버전은 VisualStudio Professional 2022 입니다.
//WindowsAPI를 아주 아주 많-이 사용하기 때문에 Windows 10, 11 네이티브 환경을 
//제외한 환경에서의 정상작동을 보증하지 않습니다.

//가상머신, Windows 8.1 이하, Wine, MacOS Parallels Desktop 에서는 문제생겨도 안 고쳐줄거에요
```
게임의 메인 사이클을 돌리는 메인 파일입니다.  
게임의 초기 설정 후 계속 메인 사이클을 돌리게 됩니다.  
사이클 과정은 각 장면의 DRAW 함수로 백버퍼 설정 -> PLAY 함수로 입력 감지, 한 프레임 실행 -> 백 버퍼와 프론트 버퍼 교체  

### Windows.h
```
//윈도우 창과 관련된 함수들을 모아놓은 헤더입니다.
/* 윈도우 창은 좀 많이 거지같습니다. Client영역과 Window영역이 분리되어 있고...
* 윈도우 창 크기 조정하는 함수만 네다섯개가 있는 데다가.. 제대로 작동하지도 않고...
* 창 크기 조정할때마다 잔상 생기고...
* 아무튼 최대한 정상적으로 작동하도록 했지만 아마 아직도 에러가 좀 있을 거라고 생각합니다.
* 어쩔 수 없어요. 받아들이죠. */
```
콘솔 윈도우 창과 관련해서 이것저것 하는 코드입니다.  
사실 스크린 크기랑 창 크기 맞춰주는 것 하나 밖에 없지만 그래도 중요해요

### Input.h
```
//사용자 입력과 관련된 내용을 담은 헤더파일입니다.
/* 다들 콘솔에서 메뉴선택을 방향키나 뭐 그런걸 가지고 입력하는데, 
* 제 키보드 (해피해킹)에는 방향키가 없습니다. 그래서 싫어요.
* 그래서 WindowsAPI를 이용해서 마우스 클릭을 받기로 했습니다.
* 근데 분명 입력버퍼 방식으로 받는 거면 대충 메인 루프 안에 넣어도 연속적인 마우스 입력이 가능해야 할거 같은데,
* 요상하게도 그러지 않더라고요. 그래서 마우스 입력만 무한정 받는 스레드를 만들어서 detach() 하는 방식으로 마우스 클릭을 받습니다.
*/
```
콘솔에서 마우스와 키보드 입력을 받는 코드입니다.  
더블버퍼링을 사용하는 이상 `getch()` 함수의 기존 구현이 작동하지 않아서, 키보드 입력도 직접 만들었습니다.

### Graphic.h
```
//그래픽 관련 함수를 모아놓은 헤더입니다.
/*이 게임은 cout, printf를 비롯한 표준 입출력을 사용하지 않습니다.
대신,WindowsAPI를 적극 활용하여 TUI 그래픽을 더블버퍼링 기법으로 출력합니다.
사용되는 렌더링 기법에 대한 설명이 아래에 있습니다.*/

//게임의 렌더링 순서
/* 게임에는 데이터 버퍼 2개와 스크린 버퍼 2개가 있습니다. D1, D2, S1, S2로 부릅시다.
* 데이터 버퍼는 화면에 표시될 텍스트와 색을 임시로 담은 정수 배열이고, 스크린 버퍼는 실제로 출력될 수 있는 WindowsAPI HANDLE입니다.
*
* 1. 가장 처음에는, D1, D2, S1, S2가 getBuffer()에 의해 초기화되어 모두 검은화면입니다.
* 2. D1에 화면에 표시될 텍스트를 빌드합니다. 프로그램 내부 정수배열에 접근하는 것이므로 빠른 속도로 수행할 수 있습니다.
* 3. D2와 D1을 비교하여 다른 부분만을 S1에 그립니다. 비교과정은 신속하지만, 그리는 과정은 콘솔에 한칸한칸 찍으므로 상당히 느립니다.
* 4. S1을 화면에 출력합니다. 이미 모두 그려져 있으므로 티어링이나 깜박임이 발생하지 않습니다.
* 5. D1을 D2에 복사, S1을 S2에 복사합니다.
* 6. D1과 D2, S1과 S2를 바꿔 2-5를 시행합니다. 이후 무한반복.
*
* 이 과정이 게임내에서 1프레임입니다. 변경부분이 적을때는 초당 1000프레임 이상, 
* 클 때에는 초당 10프레임 내외로 렌더링됩니다.
* 
* 이때 문제가 생깁니다. 멀티스레딩 버그와 창 크기 조절에 의한 글자 깨짐, 설계상의 실수 등으로 인해 
* 간혹 게임 진행 중 D1과 D2의 내용과 S1과 S2의 내용이 다른 일명 '잔상'이 생깁니다.
* 잔상이 생기면 D1과 D2의 데이터는 서로 같기 때문에 렌더링을 하더라도 S1과 S2에는 변화가 없습니다.
* 따라서 주기적으로 데이터 버퍼의 내용을 서로 비교하지 않고 스크린 버퍼에 그리는 '리프레시'가 필요합니다.
* 리프레시는 약 0.1초가 소요되며, 이를 메인 함수에서 실행하게 되면 리프레시할때마다 프레임드랍이 발생하므로,
* 실행시 스레드를 detach() 할 수 있습니다. 리프레시가 느려지지만 프레임드랍이 꽤 많이 줄어듭니다.
*
* 추가로, 리프레시 대신 게임 개발 과정 중에 개발된 잔상 제거 기법인 프론트 버퍼 디더링을 이용할 수 있습니다.
* 인위적으로 프론트버퍼에 랜덤한 노이즈를 만듬으로써 매 프레임마다 잔상제거가 이루어집니다.
* 단, 평균 프레임이 낮을 때는 잔상제거가 상당히 느리게 이루어지며, 평균 프레임 또한 더더욱 낮아집니다.
* 
* 상기의 과정을 통해 빠르면서도 깜박임 현상이 없는 콘솔 그래픽을 표시할 수 있습니다.
*/
```
그래픽 출력을 위한 코드입니다.  
크게 GRES 형식의 아스키아트를 특정한 위치에 출력하는 함수 / 한글을 포함한 텍스트를 특정한 위치에 출력하는 함수  
두가지를 구현합니다.

### Char.h
```
//wchar를 비롯한 문자와 관련된 내용을 모아둔 헤더파일입니다.
/* 터미널에서 한글 출력은 가능하지만, char 변수는 좁디 좁기 때문에 한글은 커녕 텍스트 이모티콘도 표시하기 어렵습니다.
* 하지만 이 게임의 렌더링 방식 특성상 (Graphics.h 참고) 반드시 한글을 변수로써 저장해야합니다.
* 따라서 wchar을 이용합니다. 유니코드, wide character를 저장할 수 있죠.
* 
* 그 외에 터미널 색깔 상수 등 다양한 문자 편의 기능이 이 헤더파일에 정의되어있습니다.*/
```
wchar를 편하게 다루기 위한 코드입니다.

### GameState.h
```
//게임의 전역 상태와 관련된 내용을 담은 헤더입니다.
/* 이 게임은 통상의 다른 텍스트기반 게임과 달리, 입력시만 화면이 변경되는 방식이 아니라, 미친듯이 렌더링 루프를 돌리며 화면을 바꿉니다.
* 그러면 화면에 어떤 모양을 그려야 할지는 무엇으로 결정해야 할까요? 화면이 한번 돌때마다 메인함수 전체가 순환하는데 말이죠.
* 메인함수안에 조건 분기에분기에분기에분기를 넣어서 결정할까요?
* 이 게임은 이 문제를 전역 게임 상태 변수로 해결합니다. 전역 게임 상태 변수에는 현재 장면이 어떤 장면인지,
* 지금 게임의 상태는 어떤지 등등을 담고 있어 화면을 렌더링하는데 필요한 모든 정보를 제공합니다. */

//전역 상태 변수가 필요 없는 장면 전용 변수들은 그냥 전역 변수를 이용합니다.
```
이 게임은 Global State를 각 장면에서 공유하는 방식으로 동작합니다.
그걸 관리하기 위한 코드들입니다.  
여담으로... 장면 전용 변수들을 그냥 전역 변수로 만든 건 이 게임을 만들면서 한 최악의 선택이었습니다.

### Network.h
```
//네트워크 관련 기본 함수 구현들을 모아놓은 헤더입니다.
//정.말.어.렵.습.니.다. 코드가 알록달록!
//sendUDP계열을 제외하면 기본적으로 응답 대기시간이 존재하기 때문에
//메인스레드와 분리된 스레드에서 실행하여야 합니다.

//UDP 브로드캐스팅으로 자신의 내부 아이피를 내부 아이피의 다른 상대방에게 알리며,
//TCP 소켓을 연결해서 턴제 게임을 진행하고자 하였습니다.
```
네트워크 관련 코드들입니다.  
소켓 프로그래밍은 처음이었는데, 매칭까지 구현 하려니까 죽을맛이었습니다.  
결과적으로 한번에 여러명이 서로 연결을 시도하더라도 서로 매칭이 꼬이지 않는 알고리즘을 만들었습니다만...  
한 10% 확률로 동작하더라고요..

이정도면 기본적인 게임의 기반 코드들의 설명은 모두 끝났습니다.  
Scene.h, Game.h 부터는 본격적인 게임의 동작을 다루는데, 여기부터는 설명하기 귀찮네요... 읽을 사람이 몇명이나 될까 싶기도 하고...  
직접 들어가서 주석 읽어보시길...
