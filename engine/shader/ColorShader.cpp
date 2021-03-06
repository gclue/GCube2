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

#include "ColorShader.h"
#include "../scene/Mesh.h"
#include "../scene/Camera.h"
#include "../scene/Figure.h"
#include "../util/Log.h"

using namespace GCube;

//////////////////////////////////////////////////////////
// vertex shader
CONST_STR(gVertexShader,
		  
uniform mat4 u_ModelViewProjectionMatrix;
uniform vec4 u_Color;
attribute vec3 a_Vertex;
varying vec4 v_Color;

void main()
{
	v_Color = u_Color;
	gl_Position = u_ModelViewProjectionMatrix * vec4(a_Vertex, 1.0);
}

);

// fragment shader
CONST_STR(gFragmentShader,
		  
precision mediump float;
varying vec4 v_Color;

void main()
{
	gl_FragColor = v_Color;
}

);

//////////////////////////////////////////////////////////

ColorShader::ColorShader() {
	this->reload();
}

void ColorShader::reload() {
	gProgram = loadShader(gVertexShader, gFragmentShader);
}

void ColorShader::setExtraInfo(DrawContext &context, Figure &figure) {
	if (figure.material) {
		Colorf color = figure.material->ambientColor;
		glUniform4f(colorUniform, color.r, color.g, color.b, color.a);
	}
}

void ColorShader::prepareExtraShader(GLuint program) {
	colorUniform = glGetUniformLocation(program, "u_Color");
}

