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
SSD 소프트웨어(예: ssd.py, ssd.exe, ssd.jar)
Test Shell 프로그램
Test Script 코드
ssd_nand.txt, ssd_output.txt 파일
사용자 매뉴얼 및 개발 문서

## 5. 기타
모든 소스코드 및 문서는 버전 관리 시스템(Git 등)으로 관리
프로젝트 일정 및 마일스톤은 별도 문서로 관리

문의 및 피드백:
민코딩 프로젝트 담당자
(팀명, 팀원명, 연락처 등 기재)

이 명세서는 프로젝트의 모든 개발 및 테스트 단계에서 기준 문서로 활용됩니다. 추가 요구사항이나 변경사항은 별도 협의 후 반영합니다.


# 🧑‍💻 SSD 프로젝트 역할 분배 

---

## ✅ 역할 요약표

| 역할 번호 | 세부 역할 | 담당자 (예시) | 주요 책임 |
|-----------|-----------|----------------|-----------|
| **1A** | SSD Core - Write 처리 | ✍ A번 개발자 | `W` 명령 처리, `ssd_nand.txt`에 데이터 저장 |
| **1B** | SSD Core - Read 처리 | 👓 B번 개발자 | `R` 명령 처리, `ssd_output.txt`에 덮어쓰기 |
| **2A** | Shell 명령어 파서 | 🎮 C번 개발자 | 사용자 입력 파싱 및 명령 디스패치 (`write`, `read`, `exit`, `help`) |
| **2B** | Shell - system call 처리 | 🔁 D번 개발자 | `ssd.exe`를 호출하는 system() 로직 작성 및 연결 |
| **3**  | Test Script 구현 | 🧪 E번 개발자 | Test Script 1~3 자동 실행 루틴 및 ReadCompare 기반 PASS/FAIL 구현 |

---

## ✅ 각 역할 세부 설명

### ✍ 1A. SSD Core - Write 처리 담당
- `ssd W <LBA> <VALUE>` 명령 처리
- `ssd_nand.txt`에 데이터 저장 (기록 방식 설계 포함)
- 유효하지 않은 LBA 처리 시 `"ERROR"` 출력

### 👓 1B. SSD Core - Read 처리 담당
- `ssd R <LBA>` 명령 처리
- `ssd_output.txt`에 덮어쓰기
- 기록된 값이 없으면 `0x00000000`, 오류 시 `"ERROR"` 출력

### 🎮 2A. Test Shell - 명령어 파싱 담당
- 사용자로부터 입력을 받고 명령어 파싱
- 유효성 검사 전 단계 처리
- `write`, `read`, `fullwrite`, `fullread`, `exit`, `help` 명령 처리

### 🔁 2B. Test Shell - SSD 호출 처리 담당
- `system("ssd W ...")`, `system("ssd R ...")` 호출 담당
- Shell에서 SSD와 연결되는 통신 구조 담당

### 🧪 3. Test Script 담당
- Test Script 1~3 (`1_`, `2_`, `3_`) 명령어 등록 및 처리
- 각 Script 내 반복 루프 및 ReadCompare 구현
- `PASS` / `FAIL` 판단 및 출력 구현

---

## ✅ 작업 흐름 요약

```
[사용자 입력] ─▶ Shell 명령 파싱 (2A)
                      └▶ system call (2B)
                             └▶ ssd.exe
                                  ├▶ Write (1A)
                                  └▶ Read (1B)
[자동화 실행] ─▶ Test Script (3)
```

---

## ✅ 추천 작업 순서

1. 1A/1B 개발자가 `ssd.exe` 기본 동작 완성
2. 2A/2B 개발자가 Shell 명령어 구조와 system 연동 구현
3. 3번 개발자가 자동화 스크립트 구현 및 검증 구조 작성

> 이후 역할 4(유효성 검사), 5(문서화/리팩토링/TDD)는 후속 작업으로 추가 가능합니다.

