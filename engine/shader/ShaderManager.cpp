/*
 * The MIT License (MIT)
 * Copyright (c) 2013 GClue, inc.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "../ApplicationController.h"
#include "ShaderManager.h"
#include "ColorShader.h"
#include "TexShader.h"
#include "FlatShader.h"
#include "HitTestShader.h"

using namespace GCube;

Shader_ptr ShaderManager::GetShader(ShaderType type) {
	ApplicationController *ctr = ApplicationController::SharedInstance();
	return ctr->shaderManager.getShader(type);
}

Shader_ptr ShaderManager::getShader(ShaderType type) {
	Shader_ptr ptr = cache[type];
	if (!ptr) {
		switch (type) {
			case ShaderTypeColor:
				ptr = ColorShader_ptr(new ColorShader());
				break;
			case ShaderTypeTex:
				ptr = TexShader_ptr(new TexShader());
				break;
			case ShaderTypeFlat:
				ptr = FlatShader_ptr(new FlatShader());
				break;
			case ShaderTypeHitTest:
				ptr = HitTestShader_ptr(new HitTestShader());
				break;
			default:
				break;
		}
		cache[type] = ptr;
	}
	return ptr;
}

void ShaderManager::reloadAllData() {
	std::map<int, Shader_ptr>::iterator it = cache.begin();
	while (it != cache.end()) {
		(*it).second->reload();
		it++;
	}
}
