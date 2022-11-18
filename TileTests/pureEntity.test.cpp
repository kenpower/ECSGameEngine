#include "pch.h"

#include "../PureEntity.h"

struct testComponent: public Component {
	static constexpr const char* NAME = "test";
	virtual std::string name() { return std::string(NAME); }
};

TEST(Entity, AddComponent) {

	PureEntity e;

	testComponent tc;

	EXPECT_EQ("test", tc.name());

	Component* d = &tc;

	EXPECT_EQ("test", d->name());

	e.addComponent(&tc);

	Component* c = e.getComponent("test");

	EXPECT_EQ("test", c->name());

	

}