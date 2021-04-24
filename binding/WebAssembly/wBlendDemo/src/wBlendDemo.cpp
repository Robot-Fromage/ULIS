#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <emscripten.h>
#include <SDL.h>
#include <SDL_opengles2.h>
#include <ULIS>

SDL_Window*     g_Window    = NULL;
SDL_GLContext   g_GLContext = NULL;

std::vector< std::string >      g_sampleImagesNames;
std::vector< ::ULIS::FBlock* >  g_sampleImages;
::ULIS::FThreadPool*            g_threadPool;
::ULIS::FCommandQueue*          g_queue;
constexpr ::ULIS::eFormat       g_format = ::ULIS::eFormat::Format_RGBA8
::ULIS::FContext*               g_ctx;

void main_loop( void* );
void InitStyle();
void InitULIS();
void BuildGUI();

// Sample Texture Stuff
int g_width;
int g_height;
GLuint g_texture;
bool InitTexture()
{
    // Load from file
    int image_width = 160;
    int image_height = 160;
    unsigned char* image_data = new unsigned char[ image_width * image_height * 4 ];
    for( int i = 0; i < 160 * 160 * 4; i++ ) {
        image_data[i] = 255;
    }

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture( GL_TEXTURE_2D, image_texture );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glBindTexture(GL_TEXTURE_2D, 0);

    g_texture = image_texture;
    g_width = image_width;
    g_height = image_height;

    delete [] image_data;

    return true;
}

int main( int, char** ) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    const char* glsl_version = "#version 100";
    //const char* glsl_version = "#version 300 es";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    g_Window = SDL_CreateWindow("ULIS Web Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 342, 328, window_flags);
    g_GLContext = SDL_GL_CreateContext(g_Window);
    if (!g_GLContext) {
        fprintf(stderr, "Failed to initialize WebGL context!\n");
        return 1;
    }
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = NULL;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(g_Window, g_GLContext);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    io.Fonts->AddFontFromFileTTF("resources/fonts/Roboto-Medium.ttf", 16.0f);

    // Init Style
    InitStyle();

    // InitULIS
    InitULIS();

    InitTexture();

    // This function call won't return, and will engage in an infinite loop, processing events from the browser, and dispatching them.
    emscripten_set_main_loop_arg(main_loop, NULL, 0, true);
}

void main_loop( void* iArg ) {
    ImGuiIO& io = ImGui::GetIO();
    IM_UNUSED( iArg );

    // Poll and handle events (inputs, window resize, etc.)
    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
        ImGui_ImplSDL2_ProcessEvent(&event);
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(g_Window);
    ImGui::NewFrame();

    // 0. Show the big demo window.
    //ImGui::ShowDemoWindow();

    BuildGUI();

    // Rendering
    ImGui::Render();
    SDL_GL_MakeCurrent(g_Window, g_GLContext);
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(g_Window);
}

void InitStyle() {
    ImVec4* colors = ImGui::GetStyle().Colors;
    auto &style = ImGui::GetStyle();

    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_ChildBg]                = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
    colors[ImGuiCol_ModalWindowDarkening]   = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);

    style.WindowPadding            = ImVec2(6, 4);
    style.WindowRounding           = 0.0f;
    style.FramePadding             = ImVec2(5, 2);
    style.FrameRounding            = 0.0f;
    style.ItemSpacing              = ImVec2(7, 1);
    style.ItemInnerSpacing         = ImVec2(1, 1);
    style.TouchExtraPadding        = ImVec2(0, 0);
    style.IndentSpacing            = 6.0f;
    style.ScrollbarSize            = 12.0f;
    style.ScrollbarRounding        = 0.0f;
    style.GrabMinSize              = 20.0f;
    style.GrabRounding             = 2.0f;

    style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
    style.FrameBorderSize = 0.0f;
    style.WindowBorderSize = 1.0f;
}

void InitULIS() {
    g_threadPool = new ::ULIS::FThreadPool();
    g_queue = new ::ULIS::FCommandQueue( *g_threadPool );
    g_ctx = new ::ULIS::FContext( *g_queue, g_format );
    
    ::ULIS::FFilePathRegistry sampleImageRegistry;
    sampleImageRegistry.AddLookupPath( "resources/img/" );
    sampleImageRegistry.AddFilter( ".png" );
    sampleImageRegistry.Parse();
    for( auto it : sampleImageRegistry.GetMap() ) {
        g_sampleImages.push_back( new FBlock() );
        g_ctx->XLoadBlockFromDisk( *( g_sampleImages.back() ), it.second );
        g_sampleImagesNames.push_back( it.first );
    }
    g_ctx->Finish();
}

void BuildGUI() {
    ImGui::SetNextWindowPos( ImVec2( 0.f, 0.f ) );
    ImGui::SetNextWindowSize( ImVec2( 342.f, 328.f ) );
    ImGui::Begin( "ULIS Blend Demo", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar );
    static ImGuiComboFlags flags = 0;
    static int over_item_current_idx = 3;
    const char* over_combo_label = g_sampleImagesNames[ over_item_current_idx ].c_str();
    if( ImGui::BeginCombo( "Over", over_combo_label, flags ) ) {
        for( int n = 0; n < g_sampleImages.size(); n++ ) {
            const bool is_selected = ( over_item_current_idx == n );
            if( ImGui::Selectable( g_sampleImagesNames[ n ].c_str(), is_selected ) )
                over_item_current_idx = n;

            if( is_selected )
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    static int base_item_current_idx = 4;
    const char* base_combo_label = g_sampleImagesNames[ base_item_current_idx ].c_str();
    if( ImGui::BeginCombo( "Base", base_combo_label, flags ) ) {
        for( int n = 0; n < g_sampleImages.size(); n++ ) {
            const bool is_selected = ( base_item_current_idx == n );
            if( ImGui::Selectable( g_sampleImagesNames[ n ].c_str(), is_selected ) )
                base_item_current_idx = n;

            if( is_selected )
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    static int bm_item_current = 0;
    ImGui::Combo( "Blend Mode", &bm_item_current, ::ULIS::kwBlendMode, ::ULIS::eBlendMode::NumBlendModes );

    static int am_item_current = 0;
    ImGui::Combo( "Alpha Mode", &am_item_current, ::ULIS::kwAlphaMode, ::ULIS::eAlphaMode::NumAlphaModes );

    static float opacity = 0.5f;
    ImGui::SliderFloat( "Opacity", &opacity, 0.0f, 1.0f, "%.3f" );

    ImGui::Image((void*)(intptr_t)g_texture, ImVec2( g_width, g_height ));

    {
        ::ULIS::FBlock result( 160, 160, g_format );
        ctx->Copy( *( g_sampleImages[ base_item_current_idx ] ), result );
        ctx->Finish();

        ctx->Blend( *( g_sampleImages[ over_item_current_idx ] ), result );
        ctx->Finish();

        glBindTexture( GL_TEXTURE_2D, g_texture );
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 160, 160, GL_RGBA, GL_UNSIGNED_BYTE, result.DataPtr() );
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    ImGui::Text("size = %d x %d", g_width, g_height);
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}