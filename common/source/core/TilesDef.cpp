/*
 * =====================================================================================
 *
 *       Filename:  TilesDef.cpp
 *
 *    Description:
 *
 *        Created:  30/01/2020 17:27:16
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/core/Exception.hpp>

#include "NetworkUtils.hpp"
#include "TilesDef.hpp"

const std::string &TilesDef::getTextureForFace(u8 face, bool useAltTiles) const {
	u8 size = (!useAltTiles) ? textureFilenames.size() : altTextureFilenames.size();
	if (size == 0)
		throw EXCEPTION("Trying to get texture from empty tiles definition");

	u8 outFace = face;

	if (size == 1)
		outFace = BlockFace::Top;
	else if (size == 2)
		outFace = (face == BlockFace::Top || face == BlockFace::Bottom) ? 0 : 1;
	else if (size == 3 && face >= size)
		outFace = BlockFace::Left;
	else if (size == 4 && face >= size)
		outFace = BlockFace::Right;
	else if (size == 5 && face >= size)
		outFace = BlockFace::Front;

	if (!useAltTiles)
		return textureFilenames.at(outFace);
	else {
		const std::string &filename = altTextureFilenames.at(outFace);
		return (!filename.empty()) ? filename : getTextureForFace(face, false);
	}
}

void TilesDef::serialize(sf::Packet &packet) const {
	packet << textureFilenames; // << altTextureFilenames;
}

void TilesDef::deserialize(sf::Packet &packet) {
	packet >> textureFilenames; // >> altTextureFilenames;
}

void TilesDef::loadFromLuaTable(const sol::table &table) {
	if (table["tiles"].get_type() == sol::type::table) {
		sol::as_table_t<std::vector<std::string>> t = table["tiles"];
		textureFilenames = t.source;
	}
	else
		textureFilenames.emplace_back(table["tiles"].get<std::string>());

	if (table["alt_tiles"].get_type() != sol::type::none) {
		if (table["alt_tiles"].get_type() == sol::type::table) {
			sol::as_table_t<std::vector<std::string>> t = table["alt_tiles"];
			altTextureFilenames = t.source;
		}
		else
			altTextureFilenames.emplace_back(table["alt_tiles"].get<std::string>());
	}
}

