#ifndef VOICE_CALLBACK
#define VOICE_CALLBACK


#include "..\XAudio2.h"
#include <stdio.h>

class VoiceCallback : public IXAudio2VoiceCallback
{
public:
	HANDLE event; // �C�x���g
	// �R���X�g���N�^
	VoiceCallback() : event(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
	// �f�X�g���N�^
	~VoiceCallback() { CloseHandle(event); }

	//
	// �ȉ��R�[���o�b�N
	//
	// �{�C�X���A���I�[�f�B�I�X�g���[���̍Đ����I�������Ƃ��ɌĂяo�����
	void STDMETHODCALLTYPE OnStreamEnd() { printf("%s\n", __func__); }
	// �{�C�X�̏I���̃v���Z�b�V���O�p�X�̒���ɌĂяo�����
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() {}
	// �{�C�X�J�n�̃v���Z�b�V���O�p�X�̒��O�ɌĂяo�����
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
	// �{�C�X���o�b�t�@�[�̏������I�������Ƃ��ɌĂяo�����
	void STDMETHODCALLTYPE OnBufferEnd(void * pBufferContext) { SetEvent(event); }
	// �{�C�X���V�����I�[�f�B�I�o�b�t�@�[�̏������J�n���悤�Ƃ���Ƃ��ɌĂяo�����
	void STDMETHODCALLTYPE OnBufferStart(void * pBufferContext) {}
	// �{�C�X�����[�v�̍Ō�ɓ��B�����Ƃ��ɌĂяo�����
	void STDMETHODCALLTYPE OnLoopEnd(void * pBufferContext) { printf("%s\n", __func__); }
	// �{�C�X�̏������ɏd��ȃG���[�����������Ƃ��ɌĂяo�����
	void STDMETHODCALLTYPE OnVoiceError(void * pBufferContext, HRESULT Error) { printf("%s\n", __func__); }
};

#endif // #ifndef VOICE_CALLBACK