/*
 * Created on Wed Feb 05 2025
 *
 * Author: Peter Gusev
 * Copyright (c) 2025 Peter Gusev. All rights reserved.
 */

#include "track.hpp"

#include <emscripten/emscripten.h>

extern "C" {
extern void rtcStopMediaTrack(int mt);
extern void rtcSetUserPointer(int i, void *ptr);
extern void rtcSetMediaTrackEndedCallback(int mt, void (*callback)(void *));
extern void rtcSetMuteCallback(int mt, void (*callback)(void *));
extern void rtcSetUnmuteCallback(int mt, void (*callback)(void *));
extern int rtcGetMediaTrackId(int dc, char *buffer, int size);
extern int rtcGetMediaTrackLabel(int dc, char *buffer, int size);
extern int rtcGetMediaTrackKind(int dc, char *buffer, int size);
extern int rtcGetMediaTrackReadyState(int dc, char *buffer, int size);
extern int rtcGetMediaTrackIsMuted(int dc);
}

namespace rtc {

Track::Track(int id) : mId(id) {
	rtcSetUserPointer(mId, this);
	rtcSetMediaTrackEndedCallback(mId, EndedCallback);
	rtcSetMuteCallback(mId, MuteCallback);
	rtcSetUnmuteCallback(mId, UnmuteCallback);

	char str[256];
	rtcGetMediaTrackLabel(mId, str, 256);
	mLabel = str;

	rtcGetMediaTrackId(mId, str, 256);
	mRtcId = str;

	rtcGetMediaTrackKind(mId, str, 256);
	mKind = str;
}

Track::~Track() { stop(); }

void Track::stop() { rtcStopMediaTrack(mId); }

std::string Track::id() const { return mRtcId; }

std::string Track::label() const { return mLabel; }

std::string Track::kind() const { return mKind; }

std::string Track::readyState() const {
	if (!mId)
		return 0;

	char buffer[256];
	rtcGetMediaTrackReadyState(mId, buffer, 256);
	return buffer;
}

bool Track::muted() const { return rtcGetMediaTrackIsMuted(mId); }

void Track::triggerEnded() {
	if (mEndedCallback)
		mEndedCallback();
}

void Track::triggerMute() {
	if (mMuteCallback)
		mMuteCallback();
}

void Track::triggerUnmute() {
	if (mUnmuteCallback)
		mUnmuteCallback();
}

void Track::EndedCallback(void *ptr) {
	Track *t = static_cast<Track *>(ptr);
	if (t)
		t->triggerEnded();
}

void Track::MuteCallback(void *ptr) {
	Track *t = static_cast<Track *>(ptr);
	if (t)
		t->triggerMute();
}

void Track::UnmuteCallback(void *ptr) {
	Track *t = static_cast<Track *>(ptr);
	if (t)
		t->triggerUnmute();
}

} // namespace rtc