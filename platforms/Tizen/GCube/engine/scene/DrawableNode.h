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

#ifndef __GCube__DrawableNode__
#define __GCube__DrawableNode__

#include "Node.h"
#include "Window.h"

namespace GCube {

typedef enum {
	DrawTypeNormal,
	DrawTypeLine,
	DrawTypeHitTest,
	NUM_DrawType
} DrawType;

class DrawableNode : public Node {
public:
	DrawableNode(const char* name = NULL) : Node(name), isVisible(true) {};
	virtual ~DrawableNode() {};
	
	virtual void draw(const Window &window, DrawType type=DrawTypeNormal) = 0;
public:
	bool isVisible;
	
private:
	
};
}

#endif /* defined(__GCube__Drawable__) */
