#ifndef _CG_FRAME_TIMER_
#define _CG_FRAME_TIMER_


class CGFrameTimer
{
    float lastFrameTime_;

public:
    CGFrameTimer();

    float update();
    float getLastFrameTime();

    void start();
    void stop();
    void pause();

};

#endif
