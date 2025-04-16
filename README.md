
# 🧠 SSD 테스트 프레임워크 - 전체 프로젝트 명세서

---

## ✅ 1. 프로젝트 개요

이 프로젝트는 가상 SSD 장치를 대상으로 **명령어 기반 테스트 Shell**과 **자동화된 Test Script**를 통해  
정확한 데이터 저장/읽기 기능을 검증하는 프레임워크입니다.

---

## ✅ 2. 시스템 구성도

```
사용자 입력
     ↓
┌──────────────┐
│  Test Shell  │  ← 내부: Test Script 포함
└────┬─────────┘
     ↓ system call
┌──────────────┐
│   ssd.exe    │  ← 별도 실행파일
└──────────────┘
```

---

## ✅ 3. 구성 요소 설명

### 1) Test Shell
- 사용자로부터 명령어 입력을 받고 처리
- 명령어 파싱, 유효성 검사 수행
- `ssd.exe`를 system call로 실행
- 내부에 Test Script 명령어 포함

### 2) Test Script
- Test Shell 내부에 내장된 자동화 테스트 명령어
- 다양한 시나리오 기반 반복 테스트 + 검증 포함
- ReadCompare 기반 PASS/FAIL 판별

### 3) SSD 프로그램 (`ssd.exe`)
- 실질적인 Write / Read 기능 수행
- 데이터는 `ssd_nand.txt`, 결과는 `ssd_output.txt` 사용
- 잘못된 입력은 모두 "ERROR"로 처리 (절대 runtime error 발생하지 않음)

---

## ✅ 4. SSD 동작 사양

| 항목 | 설명 |
|------|------|
| LBA 개수 | 100개 (0 ~ 99) |
| 단위 크기 | 4Byte (총 400Byte SSD) |
| 명령 | `W <LBA> <VALUE>`, `R <LBA>` |
| 데이터 저장 | `ssd_nand.txt` |
| Read 결과 | `ssd_output.txt` (덮어쓰기) |
| 미기록 LBA | `0x00000000` 반환 |
| LBA 오류 | `ssd_output.txt`에 "ERROR" 기록 |
| Value 포맷 | 항상 `0x` + 8자리 HEX (총 10자리) |

---

## ✅ 5. Test Shell 명령어 목록

| 명령어 | 설명 |
|--------|------|
| `write <LBA> <VALUE>` | LBA에 데이터 저장 (system call) |
| `read <LBA>` | LBA에서 데이터 읽기 + 출력 |
| `fullwrite <VALUE>` | LBA 0~99 전체에 동일 값 write |
| `fullread` | LBA 0~99 전체 값 콘솔 출력 |
| `help` | 명령어 설명 + B팀 팀원 출력 |
| `exit` | Shell 종료 |
| `1_`, `2_`, `3_` | 자동화된 Test Script 실행 |

---

## ✅ 6. Test Script 구조 및 규칙

- 구조: 반복적으로 write + read 수행
- 핵심 검증 방식: **ReadCompare**
- 실패 시 즉시 `"FAIL"` 출력 후 종료
- 통과 시 `"PASS"` 출력
- 모든 스크립트는 system call 사용 (`ssd.exe`)

---

## ✅ 7. Test Script 상세 시나리오

### ▶ `1_FullWriteAndReadCompare`
- LBA를 5칸씩 묶어 각 블록에 다른 값 저장
- 각 블록마다 Write 후 ReadCompare 수행

### ▶ `2_PartialLBAWrite`
- 30회 반복
- LBA 4→0→3→1→2 순으로 Write
- LBA 0~4 ReadCompare

### ▶ `3_WriteReadAging`
- 200회 반복
- 매번 랜덤값을 생성하여 LBA 0, 99에 Write
- LBA 0, 99 ReadCompare

---

## ✅ 8. 개발 원칙

- ✅ **TDD 기반 개발**
- ✅ **Test Double (Fake SSD)** 사용 가능
- ✅ **초기엔 기능 위주 구현 후 리팩토링**
- ✅ **system call 기반 구조로 테스트와 SSD 분리**
- ✅ **에러 발생 없는 설계 (모든 입력은 graceful하게 처리)**

---

## ✅ 9. 유효성 검사 기준

| 항목 | 검사 내용 |
|------|------------|
| LBA | 0 이상 99 이하 정수 |
| VALUE | `0x` 접두어 + 8자리 HEX |
| 명령어 | 등록되지 않은 명령 → "INVALID COMMAND" 출력 |
| Test Shell | 어떠한 입력에도 runtime error 없이 종료되지 않음 |

---

## ✅ 10. 확장 계획

- `.tsc` 파일 기반 외부 스크립트 로딩
- 다양한 테스트 시나리오 추가 가능:
  - 랜덤 LBA
  - 패턴 테스트
  - 쓰기 실패 유도 시나리오
  - 지속 에이징 테스트
  - 성능 측정 테스트

---
