#pragma once

class ByteBuffer
{
private:
	char* buffer;		// 메인버퍼
	char* readptr;		// 읽을 위치 포인터
	char* writeptr;		// 쓸 위치 포인터
	char* startptr;		// 버퍼의 시작 포인터 (편의를 위해)
	char* endptr;		// 버퍼의 끝 포인터 (편의를 위해)
	int capacity;		// 버퍼의 총 크기

public:
	ByteBuffer(int size);
	~ByteBuffer();

	//void Initialize(char* buf, int len);
	int Capacity();
	int Length();
	int Remain();

	char* ReadPtr();
	char* WritePtr();
	char* StartPtr();
	char* EndPtr();

	void Read(int byte);
	void Write(int byte);
	void Arrange();

	void Copy(ByteBuffer* dest, int len);

	//static ByteBuffer* Create(int len);
	//static void Destroy(ByteBuffer* stream);
};

