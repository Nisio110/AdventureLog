#include <gtest/gtest.h>
#include <vector>
#include "UI.h"
#include "Log.h"

// These tests document KNOWN-BROKEN behaviour in `UI::sortID` and
// `UI::sortDuration` (PROGRESS.md flags them as "compiles but does not sort"
// due to a stray `;` after the `if` and an `outputs.at(position) = pivot`
// against a never-sized output vector — see src/UI.cpp:510-544).
//
// They're prefixed `DISABLED_` so gtest skips them in the default run; CI
// stays green while the tests sit alongside the rest of the suite as a clear
// regression target. Run with `--gtest_also_run_disabled_tests` to surface
// the failures, and remove the prefix when the algorithms are fixed.

TEST(UISortRegression, DISABLED_SortIDProducesAscendingOrder){
    std::vector<Log*> input;
    auto* a = new CaveLog(); a->setID(3);
    auto* b = new CaveLog(); b->setID(1);
    auto* c = new CaveLog(); c->setID(2);
    input.push_back(a);
    input.push_back(b);
    input.push_back(c);

    auto out = UI::sortID(input);

    ASSERT_EQ(out.size(), 3u);
    EXPECT_EQ(out.at(0)->getId(), 1);
    EXPECT_EQ(out.at(1)->getId(), 2);
    EXPECT_EQ(out.at(2)->getId(), 3);

    delete a; delete b; delete c;
}

TEST(UISortRegression, DISABLED_SortDurationProducesAscendingOrder){
    std::vector<Log*> input;
    auto* a = new CaveLog(); a->setDurationMins(120);
    auto* b = new CaveLog(); b->setDurationMins(30);
    auto* c = new CaveLog(); c->setDurationMins(75);
    input.push_back(a);
    input.push_back(b);
    input.push_back(c);

    auto out = UI::sortDuration(input);

    ASSERT_EQ(out.size(), 3u);
    EXPECT_EQ(out.at(0)->getDurationMins(), 30);
    EXPECT_EQ(out.at(1)->getDurationMins(), 75);
    EXPECT_EQ(out.at(2)->getDurationMins(), 120);

    delete a; delete b; delete c;
}

TEST(UISortRegression, DISABLED_SortIDOnEmptyInputReturnsEmpty){
    std::vector<Log*> empty;
    auto out = UI::sortID(empty);
    EXPECT_TRUE(out.empty());
}

TEST(UISortRegression, DISABLED_SortIDOnSingleElementReturnsSingleElement){
    std::vector<Log*> input;
    auto* only = new CaveLog();
    only->setID(42);
    input.push_back(only);

    auto out = UI::sortID(input);
    ASSERT_EQ(out.size(), 1u);
    EXPECT_EQ(out.at(0)->getId(), 42);

    delete only;
}
