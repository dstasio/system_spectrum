/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Davide Stasio $
   $Notice: (C) Copyright 2014 by Davide Stasio. All Rights Reserved. $
   ======================================================================== */

#include <windows.h>
#include <d3d11.h>
#include <dxgidebug.h>
#include <dxgidebug.h>

#include "syspec.h"
#include "syspec_platform.h"

#include "win32_layer.h"
#include <stdio.h>
//#include <d3d10.h>

#if SYSPEC_INTERNAL
#define output_string(s, ...)        {char Buffer[100];sprintf_s(Buffer, s, __VA_ARGS__);OutputDebugStringA(Buffer);}
#define throw_error_and_exit(e, ...) {output_string(" ------------------------------[ERROR] " ## e, __VA_ARGS__); getchar(); global_error = true;}
#define throw_error(e, ...)           output_string(" ------------------------------[ERROR] " ## e, __VA_ARGS__)
#define inform(i, ...)                output_string(" ------------------------------[INFO] " ## i, __VA_ARGS__)
#define warn(w, ...)                  output_string(" ------------------------------[WARNING] " ## w, __VA_ARGS__)
#else
#define output_string(s, ...)
#define throw_error_and_exit(e, ...)
#define throw_error(e, ...)
#define inform(i, ...)
#define warn(w, ...)
#endif
#define key_down(code, key)    {if(Message.wParam == (code))  input.held.key = 1;}
#define key_up(code, key)      {if(Message.wParam == (code)) {input.held.key = 0;input.pressed.key = 1;}}
#define file_time_to_u64(wt) ((wt).dwLowDateTime | ((u64)((wt).dwHighDateTime) << 32))

global b32 global_running;
global b32 global_error;

inline u64
win32_get_last_write_time(char *Path)
{
    WIN32_FILE_ATTRIBUTE_DATA FileAttribs = {};
    GetFileAttributesExA(Path, GetFileExInfoStandard, (void *)&FileAttribs);
    FILETIME last_write_time = FileAttribs.ftLastWriteTime;

    u64 result = file_time_to_u64(last_write_time);
    return result;
}

internal
PLATFORM_READ_FILE(win32_read_file)
{
    Input_File Result = {};
    HANDLE FileHandle = CreateFileA(Path, GENERIC_READ, FILE_SHARE_READ, 0,
                                    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        // TODO(dave): Currently only supports up to 4GB files
        u32 FileSize = GetFileSize(FileHandle, 0);
        DWORD BytesRead;

        // TODO(dave): Remove this allocation
        u8 *Buffer = (u8 *)VirtualAlloc(0, FileSize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
        if(ReadFile(FileHandle, Buffer, FileSize, &BytesRead, 0))
        {
            Result.data = Buffer;
            Result.size = (u32)BytesRead;
            Result.path = Path;
            Result.write_time = win32_get_last_write_time(Path);
        }
        else
        {
            throw_error("Unable to read file: %s\n", Path);
        }

        CloseHandle(FileHandle);
    }
    else
    {
        throw_error("Unable to open file: %s\n", Path);
        DWORD error = GetLastError();
        error = 0;
    }

    return(Result);
}

// NOTE(dave): This requires char arrays of length 'MAX_PATH'
internal void
get_syspec_paths(char *OriginalPath, char *TempPath)
{
    u32 PathLength = GetModuleFileNameA(0, OriginalPath, MAX_PATH);

    char *c = OriginalPath + PathLength;
    while(*c != '\\') {c--; PathLength--;}

    char *DLLName = "syspec.dll";

    while(*DLLName != '\0')
    {
        *(++c) = *(DLLName++);
        PathLength++;
    }
    OriginalPath[++PathLength] = '\0';

    for(u32 i = 0;
        i <= PathLength;
        ++i)
    {
        TempPath[i] = OriginalPath[i];
    }
    TempPath[PathLength] = '0';
    TempPath[PathLength+1] = '\0';
}

internal Win32_Game_Code
load_syspec()
{
    char orig_path[MAX_PATH];
    char temp_path[MAX_PATH];
    get_syspec_paths(orig_path, temp_path);
    while(!CopyFile(orig_path, temp_path, false)) {}

    Win32_Game_Code game_code = {};
    game_code.dll = LoadLibraryA("syspec.dll0");
    if(game_code.dll)
    {
        game_code.game_update_and_render = (Game_Update_And_Render *)GetProcAddress(game_code.dll, "UpdateAndRender");
    }
    else
    {
        throw_error_and_exit("Could not load 'syspec.dll0'\n");
    }

    return(game_code);
}

internal void
unload_syspec(Win32_Game_Code *game_code)
{
    game_code->game_update_and_render = 0;
    FreeLibrary(game_code->dll);
}

internal void
reload_syspec(Win32_Game_Code *game_code)
{
    char orig_path[MAX_PATH];
    char temp_path[MAX_PATH];
    get_syspec_paths(orig_path, temp_path);
    u64 current_write_time = win32_get_last_write_time(orig_path);

    if(current_write_time != game_code->write_time)
    {
        unload_syspec(game_code);

        *game_code = load_syspec();
        game_code->write_time = current_write_time;
    }
}

internal 
PLATFORM_RELOAD_CHANGED_FILE(win32_reload_file_if_changed)
{
    b32 has_changed = false;
    u64 current_write_time = win32_get_last_write_time(file->path);

    Input_File new_file = {};
    if(current_write_time != file->write_time)
    {
        u32 trial_count = 0;
        while(!new_file.size)
        {
            VirtualFree(new_file.data, 0, MEM_RELEASE);
            new_file = win32_read_file(file->path);
            new_file.write_time = current_write_time;
            has_changed = true;

            if (trial_count++ >= 100) // Try to read until it succeeds, or until trial count reaches max.
            {
                has_changed = false;
                throw_error_and_exit("Couldn't read changed file '%s'!", file->path);
                break;
            }
        }

        if (has_changed) {
            VirtualFree(file->data, 0, MEM_RELEASE);
            *file = new_file;
        }
    }

    return(has_changed);
}

#include "win32_renderer_d3d11.cpp"
#include "win32_audio_capture.cpp"

LRESULT CALLBACK syspec_proc(
    HWND   WindowHandle,
    UINT   Message,
    WPARAM wParam,
    LPARAM lParam
)
{
    LRESULT Result = 0;
    switch(Message)
    {
        case WM_CLOSE:
        {
            DestroyWindow(WindowHandle);
        } break;
            
        case WM_DESTROY:
        {
            global_running = false;
            PostQuitMessage(0);
        } break;

        default:
        {
          Result = DefWindowProcA(WindowHandle, Message, wParam, lParam);
        } break;
    }
    return(Result);
}


int
WinMain(
    HINSTANCE Instance,
    HINSTANCE PrevInstance,
    LPSTR CommanLine,
    int ShowFlag
)
{
    // @todo add support for low-resolution performance counters
    // @todo maybe add support for 32-bit
    i64 performance_counter_frequency = 0;
    Assert(QueryPerformanceFrequency((LARGE_INTEGER *)&performance_counter_frequency));
    // @todo make this hardware-dependant
    r32 target_ms_per_frame = 1.f/60.f;

    WNDCLASSA syspec_window_class = {};
    syspec_window_class.style = CS_OWNDC|CS_VREDRAW|CS_HREDRAW;
    syspec_window_class.lpfnWndProc = syspec_proc;
    //syspec_window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    syspec_window_class.hInstance = Instance;
    syspec_window_class.lpszClassName = "syspec_window_class";
    ATOM Result = RegisterClassA(&syspec_window_class);

    RECT WindowDimensions = {0, 0, WIDTH, HEIGHT};
    AdjustWindowRect(&WindowDimensions, WS_OVERLAPPEDWINDOW, FALSE);
    WindowDimensions.right -= WindowDimensions.left;
    WindowDimensions.bottom -= WindowDimensions.top;

    HWND MainWindow = CreateWindowA("syspec_window_class", "Spectrum",
                                    WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                    CW_USEDEFAULT, CW_USEDEFAULT,
                                    WindowDimensions.right,
                                    WindowDimensions.bottom,
                                    0, 0, Instance, 0);

    if(MainWindow)
    {
        RECT window_rect = {};
        GetWindowRect(MainWindow, &window_rect);
        u32 window_width  = window_rect.right - window_rect.left;
        u32 window_height = window_rect.bottom - window_rect.top;

        //
        // raw input set-up
        //
        RAWINPUTDEVICE raw_in_devices[] = {
            {0x01, 0x02, 0, MainWindow}
        };
        RegisterRawInputDevices(raw_in_devices, 1, sizeof(raw_in_devices[0]));

#if SYSPEC_INTERNAL
        LPVOID BaseAddress = (LPVOID)Terabytes(2);
#else
        LPVOID BaseAddress = 0;
#endif
        global_running = true;
        global_error = false;
        game_memory GameMemory = {};
        GameMemory.storage_size = Megabytes(500);
        GameMemory.storage = VirtualAlloc(BaseAddress, GameMemory.storage_size,
                                           MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
        GameMemory.read_file = win32_read_file;
        GameMemory.reload_if_changed = win32_reload_file_if_changed;

        Win32_Game_Code syspec = load_syspec();

        // @todo: probably a global variable is a good idea?
        Platform_Renderer renderer = {};
        D11_Renderer d11 = {};
        renderer.load_renderer    = win32_load_d3d11;
        renderer.reload_shader    = d3d11_reload_shader;
        renderer.init_texture     = d3d11_init_texture;
        renderer.load_wexp        = d3d11_load_wexp;
        renderer.init_square_mesh = d3d11_init_square_mesh; // @todo: this is to be removed; called in a general init function for the renderer
        renderer.set_active_mesh    = d3d11_set_active_mesh;
        renderer.set_active_texture = d3d11_set_active_texture;
        renderer.set_active_shader  = d3d11_set_active_shader;
        renderer.draw_rect = d3d11_draw_rect;
        renderer.draw_text = d3d11_draw_text;
        renderer.platform = (void *)&d11;

        DXGI_MODE_DESC display_mode_desc = {};
        //DisplayModeDescriptor.Width = WIDTH;
        //DisplayModeDescriptor.Height = HEIGHT;
        display_mode_desc.RefreshRate = {60, 1};
        display_mode_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        display_mode_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
        display_mode_desc.Scaling = DXGI_MODE_SCALING_CENTERED;

        DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
        swap_chain_desc.BufferDesc = display_mode_desc;
        swap_chain_desc.SampleDesc = {1, 0};
        swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.BufferCount = 2;
        swap_chain_desc.OutputWindow = MainWindow;
        swap_chain_desc.Windowed = true;
        swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        swap_chain_desc.Flags;

        D3D11CreateDeviceAndSwapChain(
            0,
            D3D_DRIVER_TYPE_HARDWARE,
            0,
            D3D11_CREATE_DEVICE_BGRA_SUPPORT|D3D11_CREATE_DEVICE_DEBUG,
            0,
            0,
            D3D11_SDK_VERSION,
            &swap_chain_desc,
            &d11.swap_chain,
            &d11.device,
            0,
            &d11.context
        );

        d11.swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&d11.backbuffer);

        Input input = {};
        MSG Message = {};
        u32 Count = 0;

        b32 in_menu = false;
        i64 last_performance_counter = 0;
        i64 current_performance_counter = 0;
        Assert(QueryPerformanceCounter((LARGE_INTEGER *)&last_performance_counter));
        while(global_running && !global_error && !(input.pressed.esc && in_menu && 0))
        {
            input.pressed = {};
            input.dmouse = {};
            input.dwheel = 0;

            Assert(QueryPerformanceCounter((LARGE_INTEGER *)&current_performance_counter));
            r32 dtime = (r32)(current_performance_counter - last_performance_counter) / (r32)performance_counter_frequency;
            while(dtime <= target_ms_per_frame)
            {
                while(PeekMessageA(&Message, MainWindow, 0, 0, PM_REMOVE))
                {
                    switch(Message.message)
                    {
                        case WM_KEYDOWN:
                        {
                            key_down(VK_UP,      up);
                            key_down(VK_DOWN,    down);
                            key_down(VK_LEFT,    left);
                            key_down(VK_RIGHT,   right);
                            key_down(VK_W,       w);
                            key_down(VK_A,       a);
                            key_down(VK_S,       s);
                            key_down(VK_D,       d);
                            key_down(VK_F,       f);
                            key_down(VK_SPACE,   space);
                            key_down(VK_SHIFT,   shift);
                            key_down(VK_CONTROL, ctrl);
                            key_down(VK_ESCAPE,  esc);
                            key_down(VK_MENU,    alt);
                        } break;

                        case WM_KEYUP:
                        {
                            key_up(VK_UP,      up);
                            key_up(VK_DOWN,    down);
                            key_up(VK_LEFT,    left);
                            key_up(VK_RIGHT,   right);
                            key_up(VK_W,       w);
                            key_up(VK_A,       a);
                            key_up(VK_S,       s);
                            key_up(VK_D,       d);
                            key_up(VK_F,       f);
                            key_up(VK_SPACE,   space);
                            key_up(VK_SHIFT,   shift);
                            key_up(VK_CONTROL, ctrl);
                            key_up(VK_ESCAPE,  esc);
                            key_up(VK_MENU,    alt);
                        } break;

                        case WM_INPUT:
                        {
                            if (!in_menu)
                            {
                                RAWINPUT raw_input = {};
                                u32 raw_size = sizeof(raw_input);
                                GetRawInputData((HRAWINPUT)Message.lParam, RID_INPUT, &raw_input, &raw_size, sizeof(RAWINPUTHEADER));
                                RAWMOUSE raw_mouse = raw_input.data.mouse;
                                if (raw_mouse.usFlags == MOUSE_MOVE_RELATIVE)
                                {
                                    input.dmouse.x = raw_mouse.lLastX;
                                    input.dmouse.y = raw_mouse.lLastY;
                                }

                                if (raw_mouse.usButtonFlags & RI_MOUSE_WHEEL)
                                {
                                    input.dwheel = raw_mouse.usButtonData;
                                }
                                SetCursorPos((window_rect.right - window_rect.left)/2, (window_rect.bottom - window_rect.top)/2);
                            }
                        } break;

                        //case WM_MOUSEMOVE:
                        //{
                        //    i16 m_x = ((i16*)&Message.lParam)[0];
                        //    i16 m_y = ((i16*)&Message.lParam)[1];
                        //    NewInput->dm_x = NewInput->m_x - m_x;
                        //    NewInput->dm_y = NewInput->m_y - m_y;
                        //    NewInput->m_x = m_x;
                        //    NewInput->m_y = m_y;

                        //    //SetCursorPos((window_rect.right - window_rect.left)/2, (window_rect.bottom - window_rect.top)/2);
                        //} break;

                        default:
                        {
                            TranslateMessage(&Message);
                            DispatchMessage(&Message);
                        } break;
                    }
                    if (input.pressed.esc)
                    {
                        in_menu = !in_menu;
                    }
                }
                Assert(QueryPerformanceCounter((LARGE_INTEGER *)&current_performance_counter));
                dtime = (r32)(current_performance_counter - last_performance_counter) / (r32)performance_counter_frequency;
            }

#if SYSPEC_INTERNAL
            reload_syspec(&syspec);
#endif

            if(syspec.game_update_and_render)
            {
                syspec.game_update_and_render(&input, dtime, &renderer, &GameMemory);
            }
            last_performance_counter = current_performance_counter;
            inform("Frametime: %f     FPS:%d\n", dtime, (u32)(1/dtime));

            d11.swap_chain->Present(0, 0);
        }
    }
    else
    {
        // TODO: Logging
        return 1;
    }

    if (global_error)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
