#include "pch.h"

#include "../Entity.h"

struct testComponent: public Component {
	static constexpr const char* NAME = "test";
	virtual std::string name() { return std::string(NAME); }
};

TEST(Entity, _AddComponent) {

	Entity e("");

	auto tc =make_shared<testComponent>();

	EXPECT_EQ("test", tc->name());

	e.addComponent(tc);

	auto c = e.getComponent("test");
	EXPECT_EQ("test", c->name());

	shared_ptr<testComponent> sc = e.getComponent<testComponent>();
	EXPECT_EQ("test", sc->name());

}

#include <iostream>
#include <chrono>
#include <ratio>
#include <thread>


TEST(Entity, TIMECALC) {
	
	auto t1 = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(std::chrono::milliseconds(750));
	auto t2 = std::chrono::high_resolution_clock::now();

	// floating-point duration: no duration_cast needed
	std::chrono::duration<double> fp_ms = t2 - t1;
	EXPECT_NEAR(0.75, fp_ms.count(), 0.2);

}