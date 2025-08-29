#include <gtest/gtest.h>
#include "state_machine.h"

using namespace roro;

TEST(StateMachine, OpenClose) {
    RoofStateMachine fsm;
    EXPECT_TRUE(fsm.command_open());
    fsm.limit_open_triggered();
    EXPECT_EQ(fsm.state(), RoofState::OPEN);
    EXPECT_TRUE(fsm.command_close());
    fsm.limit_closed_triggered();
    EXPECT_EQ(fsm.state(), RoofState::CLOSED);
}

TEST(StateMachine, Stop) {
    RoofStateMachine fsm;
    EXPECT_TRUE(fsm.command_open());
    fsm.command_stop();
    EXPECT_EQ(fsm.state(), RoofState::ERROR);
}

TEST(StateMachine, CommandsIgnoredInErrorState) {
    RoofStateMachine fsm;
    EXPECT_TRUE(fsm.command_open());
    fsm.command_stop();
    EXPECT_FALSE(fsm.command_open());
    EXPECT_FALSE(fsm.command_close());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
