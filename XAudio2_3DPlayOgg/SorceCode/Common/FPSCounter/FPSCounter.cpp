#include "FPSCounter.h"
#include <crtdbg.h>

float CFPSCounter::m_fFrameTime = 0.0f;

CFPSCounter::CFPSCounter()
	: m_fFPS		( 0.0f )
	, m_TimeStart	()
	, m_TimeEnd		()
	, m_TimeFreq	()
{

	Init();
}

CFPSCounter::~CFPSCounter()
{
}

bool CFPSCounter::UpdateFPS()
{
	// ���̎��Ԃ��擾
	QueryPerformanceCounter(&m_TimeEnd);
	// (���̎��� - �O�t���[���̎���) / ���g�� = �o�ߎ���(�b�P��)
	m_fFrameTime = static_cast<float>(m_TimeEnd.QuadPart - m_TimeStart.QuadPart) / static_cast<float>(m_TimeFreq.QuadPart);
	// �f�o�b�K�[�ȂǂŁA���������~�܂����ꍇ�A�l��������Ԃ̂�h�~.
	// �����Z�ȂǂɎg�p���Ă���ꍇ���邽��.
	if (m_fFrameTime > 0.05f) {
		m_fFrameTime = 0.05f;
	}

	if (m_fFrameTime < MIN_FRAME_TIME) { // ���Ԃɗ]�T������
		// �~���b�ɕϊ�
		DWORD sleepTime = static_cast<DWORD>((MIN_FRAME_TIME - m_fFrameTime) * 1000);

		timeBeginPeriod(1); // ����\���グ��(�������Ȃ���Sleep�̐��x�̓K�^�K�^)
		Sleep(sleepTime);   // �Q��
		timeEndPeriod(1);   // �߂�

		// ���T�Ɏ����z��(�������Ȃ���fps���ςɂȂ�?)
		return false;
	}

	if (m_fFrameTime > 0.0) { // �o�ߎ��Ԃ�0���傫��(�������Ȃ��Ɖ��̌v�Z�Ń[�����Z�ɂȂ�Ǝv���)
		m_fFPS = (m_fFPS * 0.99f) + (0.01f / m_fFrameTime); // ����fps���v�Z
	}

	m_TimeStart = m_TimeEnd; // ����ւ�
	return true;
}

void CFPSCounter::Init()
{
	// ���C�����[�v�ɓ���O�ɐ��x���擾���Ă���
	if (QueryPerformanceFrequency(&m_TimeFreq) == FALSE) { // ���̊֐���0(FALSE)���A�鎞�͖��Ή�
		_ASSERT_EXPR(false, "FPSCounter�̏����ݒ莸�s");
	}
	// 1�x�擾���Ă���(����v�Z�p)
	QueryPerformanceCounter(&m_TimeStart);
}
