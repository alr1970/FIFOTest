#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "fifo.hpp"
#include "arbitraryStruct.hpp"

TEST_CASE("Fifo int", "Basic queue behaviour using ints") {
    // zero capacity
    TSFifo<int>* f = new TSFifo<int>(0);
    REQUIRE(f != nullptr);
    REQUIRE(!f->push(0));

    // capacity 1
    TSFifo<int>* f2 = new TSFifo<int>(1);
    REQUIRE(f2 != nullptr);
    REQUIRE(f2->push(0));
    REQUIRE(!f2->push(1));

    // capacity 2
    TSFifo<int>* f3 = new TSFifo<int>(2);
    REQUIRE(f3 != nullptr);
    REQUIRE(f3->push(0));
    REQUIRE(f3->push(1));
    REQUIRE(!f3->push(2));    // Nope, full
    REQUIRE(f3->pop() == 0);
    REQUIRE(f3->push(2));    // Now there's space
    REQUIRE(f3->pop() == 1);
    REQUIRE(f3->pop() == 2);
}

TEST_CASE("Fifo struct", "Basic queue behaviour using an arbitrary struct") {
    // capacity 2
    TSFifo<arbitrary>* f3 = new TSFifo<arbitrary>(2);
    REQUIRE(f3 != nullptr);
    REQUIRE(f3->push(0));
    REQUIRE(f3->push(1));
    REQUIRE(!f3->push(2));    // Nope, full
    REQUIRE(f3->pop().anInt == 0);
    REQUIRE(f3->push(2));    // Now there's space
    REQUIRE(f3->pop().anInt == 1);
    REQUIRE(f3->pop().anInt == 2);
}

TEST_CASE("Fifo pop_try", "Calling pop_try instead of pop") {
    // capacity 2
    TSFifo<int>* f3 = new TSFifo<int>(3);
    REQUIRE(f3 != nullptr);
    REQUIRE(f3->push(0));
	REQUIRE(f3->push(1));
	REQUIRE(f3->push(2));
	REQUIRE(!f3->push(3));    // Nope, full
    std::pair<bool, arbitrary> returned = f3->pop_try();
    REQUIRE(returned.first);
    REQUIRE(returned.second.anInt == 0);
    REQUIRE(f3->push(3));    // Now there's space
    returned = f3->pop_try();
    REQUIRE(returned.first);
    REQUIRE(returned.second.anInt == 1);
	returned = f3->pop_try();
	REQUIRE(returned.first);
	REQUIRE(returned.second.anInt == 2);
	returned = f3->pop_try();
	REQUIRE(returned.first);
	REQUIRE(returned.second.anInt == 3);
	// Queue is empty now
    returned = f3->pop_try();
    REQUIRE(!returned.first);
}

TEST_CASE("Fifo move operations", "Test move constructor and operator") {
	TSFifo<int> f1(6);
    f1.push(0);
    f1.push(1);
    f1.push(2);
    f1.push(3);
    f1.push(4);
	f1.push(5);
    TSFifo<int> f2(std::move(f1));
    // f1 should be empty now
    std::pair<bool, int> returned = f1.pop_try();
    REQUIRE(!returned.first);
    // and its contents are now in f2
    returned = f2.pop_try();
    REQUIRE(returned.first);
    REQUIRE(returned.second == 0);
    returned = f2.pop_try();
    REQUIRE(returned.first);
    REQUIRE(returned.second == 1);
    returned = f2.pop_try();
    REQUIRE(returned.first);
    REQUIRE(returned.second == 2);
    returned = f2.pop_try();
    REQUIRE(returned.first);
    REQUIRE(returned.second == 3);
    returned = f2.pop_try();
    REQUIRE(returned.first);
    REQUIRE(returned.second == 4);
	returned = f2.pop_try();
	REQUIRE(returned.first);
	REQUIRE(returned.second == 5);

    TSFifo<int> f3(5);
    f3.push(0);
    f3.push(1);
    f3.push(2);
    f3.push(3);
    f3.push(4);
    TSFifo<int> f4 = std::move(f3);
    // f3 should be empty now
    returned = f3.pop_try();
    REQUIRE(!returned.first);
    // and its contents are now in f4
    returned = f4.pop_try();
    REQUIRE(returned.first);
    REQUIRE(returned.second == 0);
    returned = f4.pop_try();
    REQUIRE(returned.first);
    REQUIRE(returned.second == 1);
    returned = f4.pop_try();
    REQUIRE(returned.first);
    REQUIRE(returned.second == 2);
    returned = f4.pop_try();
    REQUIRE(returned.first);
    REQUIRE(returned.second == 3);
    returned = f4.pop_try();
    REQUIRE(returned.first);
    REQUIRE(returned.second == 4);
}
