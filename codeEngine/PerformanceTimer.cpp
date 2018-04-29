#include "PerformanceTimer.h"

#if TOFU_PERFORMANCE_TIMER_ENABLED == 1

namespace codeEngine
{
	int64_t PerformanceTimer::counterFreq = 0;
	int64_t PerformanceTimer::startTick = 0;

	int64_t PerformanceTimer::timerSlots[kMaxPerformanceTimerSlot];
	int64_t PerformanceTimer::deltaTimerSlots[kMaxPerformanceTimerSlot];
}

#endif