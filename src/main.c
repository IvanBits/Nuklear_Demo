// main.c
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//#define GLEW_STATIC
#include "../libs/glew/include/GL/glew.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include "../include/nuklear.h"

#include "../libs/glfw/include/GLFW/glfw3.h"

#define NK_GLFW_GL4_IMPLEMENTATION
#include "../include/nuklear_glfw_gl4.h"

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 300
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

int main(void) {
    GLFWwindow *win;
    struct nk_context *ctx;
    float num1 = 0.0f, num2 = 0.0f, result = 0.0f;
    char num1_str[32] = "0";
    char num2_str[32] = "0";
    char res_str[64] = "";    
    GLenum err;

    /* Inicializar GLFW */
    if (!glfwInit()) {
        MessageBoxA(NULL, "No se pudo inicializar GLFW", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    /* Configuración de GLFW */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    /* Crear ventana */
    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Calculadora Nuklear", NULL, NULL);
    if (!win) {
        MessageBoxA(NULL, "No se pudo crear la ventana", "Error", MB_OK | MB_ICONERROR);
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(win);
    
    /* Inicializar GLEW */
    err = glewInit();
    if (err != GLEW_OK) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "Error de GLEW: %s", glewGetErrorString(err));
        MessageBoxA(NULL, error_msg, "Error", MB_OK | MB_ICONERROR);
        glfwDestroyWindow(win);
        glfwTerminate();
        return -1;
    }

    ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
    
    /* Inicializar fuente por defecto */
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&atlas);
    struct nk_font *font = nk_font_atlas_add_default(atlas, 13, NULL);
    nk_glfw3_font_stash_end();
    if (font)
        nk_style_set_font(ctx, &font->handle);

    struct nk_colorf bg;
    bg.r = 0.10f; bg.g = 0.18f; bg.b = 0.24f; bg.a = 1.0f;

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        nk_glfw3_new_frame();

        if (nk_begin(ctx, "Calculadora", nk_rect(20, 20, 360, 260), NK_WINDOW_BORDER|NK_WINDOW_TITLE)) {
            nk_layout_row_dynamic(ctx, 30, 2);
            nk_label(ctx, "Numero 1:", NK_TEXT_LEFT);
            if (nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, num1_str, sizeof(num1_str), nk_filter_float)) {
                num1 = atof(num1_str);
            }
            
            nk_label(ctx, "Numero 2:", NK_TEXT_LEFT);
            if (nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, num2_str, sizeof(num2_str), nk_filter_float)) {
                num2 = atof(num2_str);
            }

            nk_layout_row_dynamic(ctx, 30, 4);
            if (nk_button_label(ctx, "Suma")) {
                result = num1 + num2;
                snprintf(res_str, sizeof(res_str), "%.2f", result);
            }
            if (nk_button_label(ctx, "Resta")) {
                result = num1 - num2;
                snprintf(res_str, sizeof(res_str), "%.2f", result);
            }
            if (nk_button_label(ctx, "Multiplica")) {
                result = num1 * num2;
                snprintf(res_str, sizeof(res_str), "%.2f", result);
            }
            if (nk_button_label(ctx, "Division")) {
                if (num2 != 0.0f) {
                    result = num1 / num2;
                    snprintf(res_str, sizeof(res_str), "%.2f", result);
                } else {
                    snprintf(res_str, sizeof(res_str), "Error: div por 0");
                }
            }
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, res_str, NK_TEXT_RIGHT);
        }
        nk_end(ctx);

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        glClear(GL_COLOR_BUFFER_BIT);
        nk_glfw3_render(NK_ANTI_ALIASING_ON);
        glfwSwapBuffers(win);
    }
    nk_glfw3_shutdown();
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
