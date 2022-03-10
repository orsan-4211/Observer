#include <windows.h>
#include <WindowsX.h>
#include <stdio.h>
#include <shlobj.h>
#include <tchar.h>
#include <crtdbg.h>
#include <time.h>

#include "Sound.h"

HANDLE g_hEvent;
CSoundCallback callback;

void SoundStart(HRESULT hr, XAudiom Xaum, XMasterVoice Masv, XSoureceVoice &Sosv, XAUDIO2_BUFFER &bufinfo, TCHAR filename[]) {

		Sosv.buf = NULL; Sosv.cnt = 0;
		if (LoadSound(filename, Sosv) != false) {
			Sosv.CreateSound(hr, Xaum);
			Sosv.SetSound();
			memset(&bufinfo, 0x00, sizeof(bufinfo));
			bufinfo.Flags = ((UINT32)Sosv.readlen >= Sosv.buflen) ? 0 : XAUDIO2_END_OF_STREAM;
			bufinfo.AudioBytes = Sosv.readlen;
			bufinfo.pAudioData = Sosv.ptr;
			bufinfo.PlayBegin = 0;
			bufinfo.PlayLength = Sosv.readlen / Sosv.g_wfx.nBlockAlign;
			bufinfo.LoopBegin = 0;
			bufinfo.LoopLength = 0;
			bufinfo.LoopCount = 0;
			hr = Sosv.g_lpSourceVoice->SubmitSourceBuffer(&bufinfo, NULL);
			if (FAILED(hr)) return;
			hr = Sosv.g_lpSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
			if (FAILED(hr)) return;
		}

}

void SoundResta(HRESULT hr, XAudiom Xaum, XMasterVoice Masv, XSoureceVoice &Sosv, XAUDIO2_BUFFER &bufinfo) {

	Sosv.buf = NULL; Sosv.cnt = 0;
		Sosv.SetSound();
		memset(&bufinfo, 0x00, sizeof(bufinfo));
		bufinfo.Flags = ((UINT32)Sosv.readlen >= Sosv.buflen) ? 0 : XAUDIO2_END_OF_STREAM;
		bufinfo.AudioBytes = Sosv.readlen;
		bufinfo.pAudioData = Sosv.ptr;
		bufinfo.PlayBegin = 0;
		bufinfo.PlayLength = Sosv.readlen / Sosv.g_wfx.nBlockAlign;
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = 0;
		bufinfo.LoopCount = 0;
		hr = Sosv.g_lpSourceVoice->SubmitSourceBuffer(&bufinfo, NULL);
		if (FAILED(hr)) return;
		hr = Sosv.g_lpSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
		if (FAILED(hr)) return;

}

void SoundCont(HRESULT hr, XSoureceVoice &Sosv, XAUDIO2_BUFFER &bufinfo, XAUDIO2_VOICE_STATE &state) {
	if (Sosv.g_hmmio != NULL) {
		Sosv.g_lpSourceVoice->GetState(&state);
		if (state.BuffersQueued < BUFFERQUEUE_MAX && Sosv.g_hmmio != NULL) {
			Sosv.SetSoundnext();
			bufinfo.Flags = ((UINT32)Sosv.readlen >= Sosv.buflen) ? 0 : XAUDIO2_END_OF_STREAM;
			bufinfo.AudioBytes = Sosv.readlen;
			bufinfo.pAudioData = Sosv.ptr;
			bufinfo.PlayBegin = 0;
			bufinfo.PlayLength = Sosv.readlen / Sosv.g_wfx.nBlockAlign;
			Sosv.g_lpSourceVoice->SubmitSourceBuffer(&bufinfo, NULL);
			state.BuffersQueued++;
			if (bufinfo.Flags & XAUDIO2_END_OF_STREAM) {
				mmioClose(Sosv.g_hmmio, 0);
				state.BuffersQueued = NULL;
				state.SamplesPlayed = NULL;
				Sosv.g_hmmio = NULL;
			}
		}
	}
}

void SetALLSound(XSoureceVoice &Sosv, DWORD  &dwFileSize, TCHAR filename[]) {
	if (LoadSound(filename, Sosv) != false) {
		dwFileSize = mmioSeek(Sosv.g_hmmio, 0, SEEK_END);
		Sosv.ptr = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, dwFileSize);
		mmioSeek(Sosv.g_hmmio, 0, SEEK_SET);
		Sosv.readlen = mmioRead(Sosv.g_hmmio, (HPSTR)Sosv.ptr, dwFileSize);
	}
}

void SoundALLStart(HRESULT hr, XAudiom Xaum, XMasterVoice Masv, XSoureceVoice &Sosv, XAUDIO2_BUFFER &bufinfo, DWORD AudioSize, int playbegin, DWORD playlength , int loopbegin, int looplength, int loopcount/*–³ŒÀ‚Ìê‡‚ÍXAUDIO2_LOOP_INFINITE ‚ðŽw’è*/) {
	Sosv.CreateSound(hr, Xaum);
	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.Flags = 0;
	bufinfo.AudioBytes = AudioSize;
	bufinfo.pAudioData = Sosv.ptr;
	bufinfo.PlayBegin = playbegin / Sosv.g_wfx.nBlockAlign;
	bufinfo.PlayLength = playlength / Sosv.g_wfx.nBlockAlign;
	bufinfo.LoopBegin = loopbegin / Sosv.g_wfx.nBlockAlign;
	bufinfo.LoopLength = looplength / Sosv.g_wfx.nBlockAlign;
	bufinfo.LoopCount = loopcount;

	hr = Sosv.g_lpSourceVoice->SubmitSourceBuffer(&bufinfo, NULL);
	if (FAILED(hr)) return;

	hr = Sosv.g_lpSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
	if (FAILED(hr)) return;
}

void SoundStop(HRESULT hr, XSoureceVoice &Sosv, XAUDIO2_VOICE_STATE &state) {
	Sosv.g_lpSourceVoice->Stop(0);
	if (Sosv.g_hmmio != NULL) {
		mmioClose(Sosv.g_hmmio, 0);
		state.BuffersQueued = NULL;
		state.SamplesPlayed = NULL;
		Sosv.g_hmmio = NULL;
	}
	Sosv.g_lpSourceVoice->FlushSourceBuffers();
}

void XSoureceVoice::CreateSound(HRESULT hr, XAudiom Xaum) {
#ifdef _USE_VOICECALLBACK_
	hr = Xaum.g_lpXAudio->CreateSourceVoice(&g_lpSourceVoice, &g_wfx, XAUDIO2_VOICE_NOPITCH, XAUDIO2_DEFAULT_FREQ_RATIO, &callback, NULL, NULL);
#else
	hr = Xaum.g_lpXAudio->CreateSourceVoice(&g_lpSourceVoice, &g_wfx, XAUDIO2_VOICE_NOPITCH, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
#endif
	if (FAILED(hr)) return;
}

void XSoureceVoice::SetSound() {
	buflen = g_wfx.nAvgBytesPerSec / 10;//100ms‚¸‚ÂŠm•Û
	buf = new unsigned char[buflen * BUFFERQUEUE_ALLOC];
	ptr = buf; cnt = (cnt + 1) % BUFFERQUEUE_ALLOC;
	readlen = mmioRead(g_hmmio, (HPSTR)ptr, buflen);

	if (readlen <= 0) {
		return;
	} else{
		readcount++;
		totalreadlen += readlen;
	}

	return;
}

void XSoureceVoice::SetSoundnext() {
	ptr = buf + buflen * cnt; cnt = (cnt + 1) % BUFFERQUEUE_ALLOC;
	readlen = mmioRead(g_hmmio, (HPSTR)ptr, buflen);

	if (readlen <= 0) {
		return;
	}else {
		readcount++;
		totalreadlen += readlen;
	}
}

void XMasterVoice::CreateMVoice(HRESULT hr, XAudiom Xaum) {
	hr = Xaum.g_lpXAudio->CreateMasteringVoice(&g_lpMasterVoice, 5.1/*XAUDIO2_DEFAULT_CHANNELS*/, 44100/*XAUDIO2_DEFAULT_SAMPLERATE*/, 0, 0, NULL);
	if (FAILED(hr)) return;
}

bool LoadSound(const TCHAR *lpFileName, XSoureceVoice &Sosv)
{
	MMCKINFO mmckinfo; PCMWAVEFORMAT pcmwf; MMRESULT mmret;
	memset(&Sosv.g_mmioinfo, 0x00, sizeof(Sosv.g_mmioinfo));
	Sosv.g_hmmio = mmioOpen(const_cast<TCHAR *>(lpFileName), &Sosv.g_mmioinfo, MMIO_READ);

	if (Sosv.g_hmmio == NULL) return false;

	memset(&Sosv.g_riffchunkinfo, 0x00, sizeof(Sosv.g_riffchunkinfo));
	Sosv.g_riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmret = mmioDescend(Sosv.g_hmmio, &Sosv.g_riffchunkinfo, NULL, MMIO_FINDRIFF);

	if (mmret != MMSYSERR_NOERROR) return false;

	memset(&mmckinfo, 0x00, sizeof(mmckinfo));
	mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmret = mmioDescend(Sosv.g_hmmio, &mmckinfo, &Sosv.g_riffchunkinfo, MMIO_FINDCHUNK);
	if (mmret != MMSYSERR_NOERROR) return false;

	if (mmckinfo.cksize >= sizeof(WAVEFORMATEX)) {
		mmioRead(Sosv.g_hmmio, (HPSTR)&Sosv.g_wfx, sizeof(Sosv.g_wfx));
	} else {
		mmioRead(Sosv.g_hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
		memset(&Sosv.g_wfx, 0x00, sizeof(Sosv.g_wfx)); memcpy(&Sosv.g_wfx, &pcmwf, sizeof(pcmwf)); Sosv.g_wfx.cbSize = 0;
	}

	mmioAscend(Sosv.g_hmmio, &mmckinfo, 0);
	memset(&Sosv.g_datachunkinfo, 0x00, sizeof(Sosv.g_datachunkinfo));
	Sosv.g_datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmret = mmioDescend(Sosv.g_hmmio, &Sosv.g_datachunkinfo, &Sosv.g_riffchunkinfo, MMIO_FINDCHUNK);

	if (mmret != MMSYSERR_NOERROR) return false;

	return true;
}