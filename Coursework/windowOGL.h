#pragma once

class windowOGL 
{
public:
    windowOGL();

	bool initOGL(int width, int height);
	void renderOGL(float rotAngle);
    void shutdown();

    void onResize(int width, int height);

};
