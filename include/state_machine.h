#pragma once
#include <mutex>

namespace roro {

enum class RoofState {
    UNKNOWN,
    OPEN,
    CLOSED,
    OPENING,
    CLOSING,
    ERROR
};

class RoofStateMachine {
public:
    RoofStateMachine();

    bool command_open();
    bool command_close();
    void command_stop();

    void limit_open_triggered();
    void limit_closed_triggered();

    RoofState state() const;

private:
    mutable std::mutex mutex_;
    RoofState state_;
};

} // namespace roro
