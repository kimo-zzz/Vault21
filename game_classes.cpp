/* This file is part of LeagueSkinChanger by b3akers, licensed under the MIT license:
*
* MIT License
*
* Copyright (c) b3akers 2020
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#pragma once
#include "stdafx.h"

void character_data_stack::push(const char* model, int32_t skin) {
	static const auto Push = reinterpret_cast<int(__thiscall*)(void*, const char* model, int32_t skinid, int32_t, bool update_spells, bool dont_update_hud, bool, bool change_particle, bool, char, const char*, int32_t, const char*, int32_t)>(baseAddr + CharacterDataStack__Push);
	Push(this, model, skin, 0, false, false, false, true, false, -1, "\x00", 0, "\x00", 0);
}

void character_data_stack::update(bool change) {
	static const auto Update = reinterpret_cast<void(__thiscall*)(void*, bool)>(baseAddr + CharacterDataStack__Update);
	Update(this, change);
}