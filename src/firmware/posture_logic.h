#pragma once
#include <stdint.h>
#include <math.h>

namespace Posture {
constexpr float kPitchThreshold = 30.0f;
constexpr float kRollThreshold = 35.0f;
inline bool IsAbnormal(float pitch, float roll) {
    return isfinite(pitch) && isfinite(roll) &&
           (fabsf(pitch) > kPitchThreshold || fabsf(roll) > kRollThreshold);
}

// GPIO2 is active low. Invalid samples keep the indicator off rather than
// presenting an old sample as a normal posture. Intervals are nominal targets.
class Feedback {
    int intensity_ = 0, step_ = 5;
    uint32_t last_ = 0;
    bool started_ = false, alert_ = false;
public:
    int Update(uint32_t now, bool abnormal, bool valid) {
        if (!valid) {
            started_ = false; intensity_ = 0; last_ = now;
            return 1023;
        }
        if (!started_ || alert_ != abnormal) {
            started_ = true; alert_ = abnormal;
            intensity_ = 0; step_ = 5; last_ = now;
            return 1023;
        }
        uint32_t interval = alert_ ? 50 : 25;
        if (uint32_t(now - last_) < interval) return 1023 - intensity_;
        last_ = now;
        if (alert_) intensity_ = intensity_ == 0 ? 1023 : 0;
        else {
            intensity_ += step_;
            if (intensity_ >= 1023) { intensity_ = 1023; step_ = -5; }
            else if (intensity_ <= 0) { intensity_ = 0; step_ = 5; }
        }
        return 1023 - intensity_;
    }
};
}
