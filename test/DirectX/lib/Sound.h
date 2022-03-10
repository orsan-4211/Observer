#ifndef INCLUDED_SOUND
#define INCLUDED_SOUND

#include <windows.h>
#include <xaudio2.h>
#define _USE_VOICECALLBACK_

#define BUFFERQUEUE_MAX         4
#define BUFFERQUEUE_ALLOC       (BUFFERQUEUE_MAX + 1)

extern HANDLE g_hEvent;


class XAudiom {
public:
	 IXAudio2 *g_lpXAudio;

	 //HRESULT CreateXAudio();

};

class CSoundCallback : public IXAudio2VoiceCallback {
public:
	CSoundCallback() { }
	~CSoundCallback() { }
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired) { }
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void) { }
	void STDMETHODCALLTYPE OnStreamEnd(void) { SetEvent(g_hEvent); }
	void STDMETHODCALLTYPE OnBufferStart(void *pBufferContext) { SetEvent(g_hEvent); }
	void STDMETHODCALLTYPE OnBufferEnd(void *pBufferContext) { }
	void STDMETHODCALLTYPE OnLoopEnd(void *pBufferContext) { }
	void STDMETHODCALLTYPE OnVoiceError(void *pBufferContext, HRESULT Error) { }
};

class XMasterVoice {
public:
	IXAudio2MasteringVoice *g_lpMasterVoice;
	void CreateMVoice(HRESULT, XAudiom);
};

class XSoureceVoice {
public:
	IXAudio2SourceVoice *g_lpSourceVoice;
	unsigned char *buf, *ptr;
	UINT32 buflen, cnt;
	LONG readlen;
	LONG totalreadlen;
	int readcount;

	HMMIO g_hmmio;
	MMIOINFO g_mmioinfo;
	MMCKINFO g_riffchunkinfo;
	MMCKINFO g_datachunkinfo;
	WAVEFORMATEX g_wfx;

	void CreateSound(HRESULT, XAudiom);
	void SetSound();
	void SetSoundnext();

};

extern CSoundCallback callback;

extern void SoundStart(HRESULT, XAudiom, XMasterVoice, XSoureceVoice&, XAUDIO2_BUFFER&, TCHAR[]);
extern void SoundCont(HRESULT, XSoureceVoice&, XAUDIO2_BUFFER&, XAUDIO2_VOICE_STATE&);
extern void SoundStop(HRESULT, XSoureceVoice&, XAUDIO2_VOICE_STATE&);
extern void SetALLSound(XSoureceVoice &Sosv, DWORD  &dwFileSize, TCHAR filename[]);
extern void SoundALLStart(HRESULT hr, XAudiom Xaum, XMasterVoice Masv, XSoureceVoice &Sosv, XAUDIO2_BUFFER &bufinfo, DWORD AudioSize, int playbegin, DWORD playlength, int loopbegin, int looplength, int loopcount/*ñ≥å¿ÇÃèÍçáÇÕXAUDIO2_LOOP_INFINITE ÇéwíË*/);
extern bool LoadSound(const TCHAR*, XSoureceVoice&);

#endif
