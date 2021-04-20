# SDL_mouse 接口移植
* CreateCursor: 创建一个鼠标光标状态，缓冲区，并复制SDL_surface到缓冲区中，cursor的driver_data记录主机光标的信息
* GetGlobalMouseState: 获取鼠标的位置和按键状态，参数是鼠标未知，返回值是鼠标按键状态
* SetRelativeMouseMode：锁定鼠标的位置，不让鼠标移动，但是要获取鼠标移动的信息
* WarpMouseGlobal： 移动鼠标位置为全局（整个屏幕）位置
* WarpMouse：移动鼠标位置为局部（相对于窗口）位置
* CreateSystemCursor：创建根据光标ID创建一个系统光标，并将光标的内容复制到SDL_cursor中
* FreeCursor： 释放一个SDL_cursor的资源以及各种缓冲区
* ShowCursor：显示/隐藏光标，把创建好的光标复制到真正的光标上面
