# SSD 프로젝트 요구사항 명세서

## 1. 개요
본 문서는 민코딩에서 진행하는 SSD + Test Shell 개발 프로젝트의 요구사항 명세서입니다. 본 프로젝트는 실제 하드웨어 SSD를 소프트웨어로 가상 구현하고, 이를 검증할 수 있는 Test Shell 및 Test Script를 개발하는 것을 목표로 합니다.

## 2. 기능적 요구사항
### 2.1. SSD 소프트웨어 구현
* 가상 SSD 구현
  - LBA(Logical Block Address) 0~99까지 총 100개 블록 지원
  - 각 LBA는 4 Byte(32bit) 크기, 총 400 Byte 저장 가능
  - 저장 단위는 4 Byte, 미기록 LBA는 0x00000000으로 간주
> ssd.exe가 산출물이다.
#### Write: ssd W [LBA] [Value]
  - LBA 범위: 0~99, 10진수
  - Value: 0x로 시작하는 8자리 16진수(총 10글자, 예: 0x1234ABCD)
  - ssd_nand.txt 파일에 해당 LBA 위치에 값 저장
  - 잘못된 LBA/Value 입력 시 ssd_output.txt에 "ERROR" 기록
  - 콘솔 출력 없음
#### Read: ssd R [LBA]
  - LBA 범위: 0~99
  - ssd_nand.txt에서 해당 LBA 값 읽어 ssd_output.txt에 기록
  - 미기록 LBA는 0x00000000 기록 --> (ssd_nand.txt의 400byte를 전부 0x00000000으로 초기화해도 된다.)
  - 잘못된 LBA 입력 시 ssd_output.txt에 "ERROR" 기록
    > debugging이 쉽도록 debug 빌드시에는 상세 ERROR를 기록또는 출력, Release 빌드시에는 콘솔 출력하지 않고 ERROR만 출력 - 콘솔에는 출력되면 안됩니다.(강사님과 확인)
  - 콘솔 출력 없음(강사님과 확인함)
  - 파일 입출력
  - ssd_nand.txt: SSD 데이터 저장 파일, 없으면 생성
    > text 형식과 binary 형식 모두 가능
  - ssd_output.txt: 마지막 Read 결과 저장, 덮어쓰기 방식
    > text 형식만 가능

** read write에 대한 성능은 기능 구현 이후 refactoring으로 진행한다.

### 2.2. Test Shell 개발
#### 명령어 지원

- write [LBA] [Value]: ssd 프로그램을 실행해 해당 LBA에 값 기록
> LBA에 대한 처리는 SSD 가 responsibility를 가진다.
- read [LBA]: ssd 프로그램을 실행해 해당 LBA 값 읽고 결과 출력
- fullwrite [Value]: 모든 LBA에 동일 값 기록
> 새로운 기능 구현이 아니라 write 명령을 반복하는 것으로 구현
- fullread: 모든 LBA 값 읽어 화면에 출력
- help: 제작자(팀명, 팀원) 및 명령어 사용법 출력
- exit: Test Shell 종료
- 기타: 없는 명령어 입력 시 "INVALID COMMAND" 출력

#### 입력 유효성 검사
- LBA: 0~99, 10진수
> 이 유효성 검사는 여기서는 생략하고, ssd의 responsibility로 간
- Value: 0x로 시작하는 8자리 16진수
- 명령어 및 파라미터 형식 오류 시 적절한 에러 메시지 출력
- 모든 입력에 대해 런타임 에러 없이 동작
> 위에서 언급한 것 과 같이 디버그 빌드시는 디버깅을 위해 상세 에러를 기록하고, Release 시에는 "ERROR"를 출력

### 2.3. Test Script 개발
#### Test Script 실행
- Test Shell에서 스크립트명 입력 시 실행(예: 1_FullWriteAndReadCompare)
> 오프라인 리뷰 강사님에게 확인한 결과, script 를 parsing하는 것이 아니라, Test script는 Test shell의 일부로서 동작하면 됩니다.
- 스크립트명 일부(예: 1_)만 입력해도 실행 가능
- Test Script 종류 및 시나리오
- 1_FullWriteAndReadCompare
  LBA 0~99까지 구간별로 Write 후 ReadCompare 수행
- 2_PartialLBAWrite
  30회 반복, LBA 4→0→3→1→2 순서로 Write 후 ReadCompare
> (1) 4번 LBA에 값을 적는다.
> (2) 0번 LBA에 같은 값을 적는다.
> (3) 3번 LBA에 같은 값을 적는다.
> (4) 1번 LBA에 같은 값을 적는다.
> (5) 2번 LBA에 같은 값을 적는다.
> (6) LBA 0 ~ 4번, ReadCompare
- 3_WriteReadAging
  200회 반복, LBA 0/99에 랜덤 값 Write 후 ReadCompare
> (1) 0번 LBA에 랜덤 값을 적는다.
(2) 99번 LBA에 랜덤 값을 적는다.
(3) LBA 0번과 99번, ReadCompare를 수행

- ReadCompare
Write한 값과 Read 결과 비교
불일치 시 즉시 FAIL 출력 후 종료
모든 비교 통과 시 PASS 출력

### 3. 비기능적 요구사항
#### 3.1. 신뢰성 및 안정성
- 모든 명령어 및 입력에 대해 런타임 에러 없이 동작해야 함
> 강사님이 여러번 강조한 내용입니다. 
- 파일 입출력 오류, 잘못된 입력 등 예외 상황에 대한 처리 필수

#### 3.2. 사용성
- 명령어 및 파라미터 형식이 명확해야 하며, help 명령어로 사용법 안내
- Test Shell은 사용자 친화적 인터페이스 제공
- 결과 출력은 명확하고 일관성 있게 제공

#### 3.3. 확장성
SSD LBA 개수, 블록 크기 등 주요 파라미터는 추후 확장 가능하도록 설계
Test Script 추가 및 수정이 용이해야 함

#### 3.4. 유지보수성
코드 및 파일 구조는 명확하게 분리
주석 및 문서화 철저

### 3.5. 성능
SSD 파일 입출력은 명령어 실행 시 전체 데이터 읽기/쓰기 방식으로 구현
Test Script 실행 시 성능 저하가 없도록 최적화

## 4. 프로젝트 산출물
SSD 소프트웨어작
```

---


