#pragma once
#include "ofMain.h"
#include "AutoShader.h"

namespace Shader_lib 
{    
    static shared_ptr<AutoShader> get_shader(string _vert_path, string _frag_path) {
        shared_ptr<AutoShader> shader = make_shared<AutoShader>();
        shader->enableWatchFiles();
        
        bool b_success = true;
        b_success &= shader->load( _vert_path, _frag_path);

        if (!b_success) ofLogError() << "Shader failed to load";

        return shader;
    }
    static shared_ptr<AutoShader> get_passthrough_shader()  { return get_shader("shaders/vert.vert"     , "shaders/frag.frag"                 ); };
    static shared_ptr<AutoShader> get_collage_shader()      { return get_shader("shaders/vert.vert"     , "shaders/collage.frag"              ); };
    static shared_ptr<AutoShader> get_chromatic_shader()    { return get_shader("shaders/vert.vert"     , "shaders/chromatic_aberation.frag"  ); };
    static shared_ptr<AutoShader> get_mpeg_glitch_shader()  { return get_shader("shaders/vert.vert"     , "shaders/mpeg_glitch.frag"          ); };
    static shared_ptr<AutoShader> get_uv_distort_shader()   { return get_shader("shaders/vert.vert"     , "shaders/uv_distort.frag"           ); };
    static shared_ptr<AutoShader> get_uv_draw_shader()      { return get_shader("shaders/vert.vert"     , "shaders/uv_draw.frag"              ); };
    static shared_ptr<AutoShader> get_alpha_multi_shader()  { return get_shader("shaders/vert.vert"     , "shaders/multi_alpha_mask.frag"     ); };
    static shared_ptr<AutoShader> get_mask_shader()         { return get_shader("shaders/vert.vert"     , "shaders/layer_mask.frag"           ); };
    static shared_ptr<AutoShader> get_alpha_shader()        { return get_shader("shaders/vert.vert"     , "shaders/alpha_mask.frag"           ); };
    static shared_ptr<AutoShader> get_thresh_shader()       { return get_shader("shaders/vert.vert"     , "shaders/threshold.frag"            ); };
    static shared_ptr<AutoShader> get_blur_shader()         { return get_shader("shaders/vert.vert"     , "shaders/blur.frag"                 ); };
    static shared_ptr<AutoShader> get_face_shader()         { return get_shader("shaders/face.vert"     , "shaders/face.frag"                 ); };
    static shared_ptr<AutoShader> get_mask_compose_shader() { return get_shader("shaders/vert.vert"     , "shaders/maskCompose.frag"          ); };



};



