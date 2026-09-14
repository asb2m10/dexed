/*
 * Dexed - MemoryLogger
 *
 * A JUCE Logger subclass that retains log messages in memory and also
 * echoes them to stdout. Messages are stored in a bounded ring buffer so
 * the history can be inspected from the debugger/UI without flooding the
 * filesystem.
 *
 * Typical usage:
 *
 *   MemoryLogger::getInstance()->install();
 *   DBG("something happened"); // or Logger::writeToLog(...)
 *   String history = MemoryLogger::getInstance()->getLog();
 */

#pragma once

#include <juce_core/juce_core.h>

#include <cstdarg>
#include <iostream>

class MemoryLogger final : public juce::Logger {
public:
    static constexpr int DefaultCapacity = 1024;

    static MemoryLogger* getInstance() {
        static MemoryLogger instance;
        return &instance;
    }

    void install() {
        Logger::setCurrentLogger(this);
    }

    void uninstall() {
        if (Logger::getCurrentLogger() == this)
            Logger::setCurrentLogger(nullptr);
    }

    void logFormat(const juce::String& format, ...) {
        va_list args;
        va_start(args, format);
        logMessage(juce::String::formatted(format, args));
        va_end(args);
    }

    void setCapacity(int newCapacity) {
        const juce::ScopedLock lock(mMutex);
        mCapacity = juce::jmax(1, newCapacity);
        trimLocked();
    }

    int capacity() const {
        const juce::ScopedLock lock(mMutex);
        return mCapacity;
    }

    int size() const {
        const juce::ScopedLock lock(mMutex);
        return mLog.size();
    }

    void clear() {
        const juce::ScopedLock lock(mMutex);
        mLog.clear();
    }

    juce::String getLog() const {
        const juce::ScopedLock lock(mMutex);
        return juce::StringArray(mLog).joinIntoString(juce::StringRef("\n"));
    }

    juce::String getTail(int count) const {
        const juce::ScopedLock lock(mMutex);
        const int start = juce::jmax(0, mLog.size() - count);
        juce::StringArray tail;
        for (int i = start; i < mLog.size(); ++i)
            tail.add(mLog[i]);
        return tail.joinIntoString(juce::StringRef("\n"));
    }

private:
    MemoryLogger() = default;
    ~MemoryLogger() override {
        uninstall();
    }

    void logMessage(const juce::String& message) override {
        std::cout << message << std::endl;

        const juce::ScopedLock lock(mMutex);
        mLog.add(message);
        trimLocked();
    }

    void trimLocked() {
        while (mLog.size() > mCapacity)
            mLog.remove(0);
    }

    int mCapacity = DefaultCapacity;
    juce::StringArray mLog;
    mutable juce::CriticalSection mMutex;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MemoryLogger);
};
