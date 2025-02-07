/*
 * Created on Wed Feb 05 2025
 *
 * Author: Peter Gusev
 * Copyright (c) 2025 Peter Gusev. All rights reserved.
 */

#ifndef RTC_TRACK_H
#define RTC_TRACK_H

#include "common.hpp"

#include <functional>

namespace rtc {

class Track final {
public:
	explicit Track(int id);
	~Track();

	void stop();

	void onEnded(std::function<void()> callback);
	void onMute(std::function<void()> callback);
	void onUnmute(std::function<void()> callback);

	const string &id() const;
	const string &kind() const;
	const string &label() const;
	bool muted() const;
	string readyState() const;

	__externref_t getJsHandle() const;

protected:
	int mId;
	string mRtcId;
	string mLabel;
	string mKind;

	virtual void triggerEnded();
	virtual void triggerMute();
	virtual void triggerUnmute();

private:
	std::function<void()> mEndedCallback;
	std::function<void()> mMuteCallback;
	std::function<void()> mUnmuteCallback;

	static void EndedCallback(void *ptr);
	static void MuteCallback(void *ptr);
	static void UnmuteCallback(void *ptr);
};
} // namespace rtc

#endif
