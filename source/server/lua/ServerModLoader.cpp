/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <gk/core/Debug.hpp>

#include <filesystem.hpp>

#include "LuaMod.hpp"
#include "ScriptEngine.hpp"
#include "ServerModLoader.hpp"

namespace fs = ghc::filesystem;

void ServerModLoader::loadMods() {
	m_scriptEngine.init();
	m_scriptEngine.luaCore().setModLoader(this);

	try {
		fs::directory_iterator dir("mods/");
		for (const auto &entry : dir) {
			if (fs::exists(entry.path().string() + "/init.lua")) {
				m_scriptEngine.lua().safe_script_file(entry.path().string() + "/init.lua");
				std::cout << "Mod '" + entry.path().filename().string() + "' loaded" << std::endl;
			}
			else
				DEBUG("WARNING: The mod '" + entry.path().filename().string() + "' doesn't contain an 'init.lua' file.");
		}
	}
	catch (const sol::error &e) {
		std::cerr << e.what() << std::endl;
		return;
	}
}

void ServerModLoader::registerMod(const LuaMod &mod) {
	DEBUG("Registering mod", mod.id());
}

