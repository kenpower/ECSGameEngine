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