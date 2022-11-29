#define NOMINMAX //https://stackoverflow.com/a/2789510/488802
#include<windows.h>
#include"ECSSystems.h"

void renderCharOnConsoleSystem(Components& components, ConsoleRenderWindow& crw) {
	for (auto& id_charSprite : components.charSprites) {
		EntityID id = id_charSprite.first;
		auto charSprite = id_charSprite.second;

		auto pos = components.positions[id];

		if (!charSprite || !pos) continue;

		crw.Draw(lround(pos->x), lround(pos->y), charSprite->c);
	}
}

void renderStringOnConsoleSystem(Components& components, ConsoleRenderWindow& crw) {
	for (auto& id_stringSprite : components.stringSprites) {
		EntityID id = id_stringSprite.first;
		auto stringSprite = id_stringSprite.second;

		auto pos = components.positions[id];
		if (!stringSprite || !pos) continue;

		for (int i = 0; stringSprite->c[i] != '\0'; i++)
			crw.Draw(lround(pos->x + i), lround(pos->y), stringSprite->c[i]);
	}
}