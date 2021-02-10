#include <stdio.h>
#include <SDL.h>

//定义屏幕尺寸常量
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] )
{
    //将要渲染的窗口
    SDL_Window* window = NULL;

    //窗口含有的surface
    SDL_Surface* screenSurface = NULL;
    
    //初始化SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        //创建 window
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            //获取 window surface
            screenSurface = SDL_GetWindowSurface( window );

            //用白色填充surface
            SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );

            //更新surface
            SDL_UpdateWindowSurface( window );

            //延迟两秒
            SDL_Delay( 2000 );
        }
    }
    //销毁 window
    SDL_DestroyWindow( window );
    //退出 SDL subsystems
    SDL_Quit();
    return 0;
}