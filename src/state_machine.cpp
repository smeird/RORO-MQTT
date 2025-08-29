#include "state_machine.h"

namespace roro {

RoofStateMachine::RoofStateMachine() : state_(RoofState::UNKNOWN) {}

bool RoofStateMachine::command_open() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (state_ == RoofState::OPEN || state_ == RoofState::OPENING ||
        state_ == RoofState::ERROR)
        return false;
    state_ = RoofState::OPENING;
    return true;
}

bool RoofStateMachine::command_close() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (state_ == RoofState::CLOSED || state_ == RoofState::CLOSING ||
        state_ == RoofState::ERROR)
        return false;
    state_ = RoofState::CLOSING;
    return true;
}

void RoofStateMachine::command_stop() {
    std::lock_guard<std::mutex> lock(mutex_);
    state_ = RoofState::ERROR;
}

void RoofStateMachine::limit_open_triggered() {
    std::lock_guard<std::mutex> lock(mutex_);
    state_ = RoofState::OPEN;
}

void RoofStateMachine::limit_closed_triggered() {
    std::lock_guard<std::mutex> lock(mutex_);
    state_ = RoofState::CLOSED;
}

RoofState RoofStateMachine::state() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return state_;
}

} // namespace roro
