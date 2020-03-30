import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "../../../libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: "../../.."

    ofApp {
        name: { return FileInfo.baseName(sourceDirectory) }

        files: [
            'src/Canvas.cpp',
            'src/Canvas.h',
            'src/GUI/GuiTheme.cpp',
            'src/GUI/GuiTheme.h',
            'src/GUI/IconsMaterialDesignIcons.h',
            'src/GUI/ImGuiHelpers.cpp',
            'src/GUI/ImGuiHelpers.h',
            'src/GUI/LayerGui.cpp',
            'src/GUI/LayerGui.h',
            'src/GUI/SingleLayerGui.cpp',
            'src/GUI/SingleLayerGui.h',
            'src/LayerVisualiser.cpp',
            'src/LayerVisualiser.h',
            'src/Layer_Manager.cpp',
            'src/Layer_Manager.h',
            'src/Layers/Layer_filter_adjustment.cpp',
            'src/Layers/Layer_filter_adjustment.h',
            'src/Layers/layer.h',
            'src/Layers/layer_aiCollage.cpp',
            'src/Layers/layer_aiCollage.h',
            'src/Layers/layer_alpha_replace_chanel.cpp',
            'src/Layers/layer_alpha_replace_chanel.h',
            'src/Layers/layer_alpha_replace_face.cpp',
            'src/Layers/layer_alpha_replace_face.h',
            'src/Layers/layer_base.cpp',
            'src/Layers/layer_base.h',
            'src/Layers/layer_collage.cpp',
            'src/Layers/layer_collage.h',
            'src/Layers/layer_collage_generative.cpp',
            'src/Layers/layer_collage_generative.h',
            'src/Layers/layer_collage_manual.cpp',
            'src/Layers/layer_collage_manual.h',
            'src/Layers/layer_collage_single.cpp',
            'src/Layers/layer_collage_single.h',
            'src/Layers/layer_file.cpp',
            'src/Layers/layer_file.h',
            'src/Layers/layer_file_image.cpp',
            'src/Layers/layer_file_image.h',
            'src/Layers/layer_filter_alpha_replace.cpp',
            'src/Layers/layer_filter_alpha_replace.h',
            'src/Layers/layer_filter_chromatic_aberation.cpp',
            'src/Layers/layer_filter_chromatic_aberation.h',
            'src/Layers/layer_filter_distort.cpp',
            'src/Layers/layer_filter_distort.h',
            'src/Layers/layer_filter_shader.cpp',
            'src/Layers/layer_filter_shader.h',
            'src/Layers/layer_fliter_mpeg_glitch.cpp',
            'src/Layers/layer_fliter_mpeg_glitch.h',
            'src/Layers/layer_image_advanced.cpp',
            'src/Layers/layer_image_advanced.h',
            'src/Layers/layer_paint.cpp',
            'src/Layers/layer_paint.h',
            'src/Paint/brush/Brush.cpp',
            'src/Paint/brush/Brush.h',
            'src/Paint/brush/BrushShaders.h',
            'src/Paint/brush/wbBrushShader.h',
            'src/Paint/brush/wbSampleShader.h',
            'src/Paint/fluid/wbDensityAddShader.h',
            'src/Paint/fluid/wbFluidFlow.cpp',
            'src/Paint/fluid/wbFluidFlow.h',
            'src/Paint/fluid/wbPaintShader.h',
            'src/Paint/textureSource/textureSource.cpp',
            'src/Paint/textureSource/textureSource.h',
            'src/Preset_layers.cpp',
            'src/Preset_layers.h',
            'src/ProjectManager.cpp',
            'src/ProjectManager.h',
            'src/Utils/AutoShader.cpp',
            'src/Utils/AutoShader.h',
            'src/Utils/FaceExtractor.cpp',
            'src/Utils/FaceExtractor.h',
            'src/Utils/Histogram.cpp',
            'src/Utils/Histogram.h',
            'src/Utils/LayerUtils.cpp',
            'src/Utils/LayerUtils.h',
            'src/Utils/pingPongFbo.h',
            'src/Utils/shader_base.h',
            'src/Utils/threadedImageLoader.cpp',
            'src/Utils/threadedImageLoader.h',
            'src/main.cpp',
            'src/ofApp.cpp',
            'src/ofApp.h',
            'src/resource.h',
        ]

        of.addons: [
            'ofxCollageViewer',
            'ofxCv',
            'ofxFaceTracker2',
            'ofxImGui',
            'ofxOpenCv',
            'ofxMonitor',
            'ofxFlowTools',
        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include
        of.includePaths: []     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
                                // and can be checked with #ifdef or #if in the code
        of.frameworks: []       // osx only, additional frameworks to link with the project
        of.staticLibraries: []  // static libraries
        of.dynamicLibraries: [] // dynamic libraries

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }
    }

    property bool makeOF: true  // use makfiles to compile the OF library
                                // will compile OF only once for all your projects
                                // otherwise compiled per project with qbs
    

    property bool precompileOfMain: false  // precompile ofMain.h
                                           // faster to recompile when including ofMain.h 
                                           // but might use a lot of space per project

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
