#QT       += core

TARGET = qtLearnOpengl
TEMPLATE = app

INCLUDEPATH += $$PWD/3rdparty/include

LIBS += user32.lib \
        gdi32.lib \
        opengl32.lib \
        $$PWD/3rdparty/lib/glfw3.lib \
        $$PWD/3rdparty/lib/glew32s.lib \
        $$PWD/3rdparty/lib/SOIL.lib \
        $$PWD/3rdparty/lib/assimp.lib

SOURCES += \
#    basicdraw/triangle.cpp \
#    basicdraw/calllist.cpp \
#    basicdraw/vertexarray.cpp \
#    basicdraw/vertexbufferobject.cpp \
#    createcontext.cpp \
#    shaderdraw/triangle_shader.cpp \
#    shaderdraw/triangle_colored.cpp \
#    shaderdraw/triangle_index.cpp \
#    textures/2dtextures.cpp \
#    modeltransformation/modeltransformation.cpp \
#    viewtransformation/viewtransformation.cpp \
#    fpscameraeuler/fpscameraeuler.cpp \
#    lighting/lightinworldspace/basiclighting.cpp \
#    lighting/lightinviewspace/lightinviewspace.cpp \
#    lighting/gouraud/gouraud.cpp \
#    lighting/material.cpp \
#    lighting/diffusemap/diffusemap.cpp \
#    lighting/directionallight/directionallight.cpp \
#    lighting/spotlight/spotlight.cpp \
#    lighting/spotlightsoftedge/spotlightsoftedge.cpp \
#    lighting/multilight/multilight.cpp \
#    modelloading/simpleobjloader/loadobj.cpp \
#    modelloading/loadwithassimp/loadmodel.cpp \
#    modelloading/loadwithassimpwithlighton/loadwithassimpwithlighton.cpp \
#    stenciltest/stenciltest/stenciltest.cpp \
#    stenciltest/stenciloutline/stencilteststenciloutline.cpp \
#    stenciltest/planarreflection/planarreflection.cpp \
#    depthtest/depthtest.cpp \
#    blend/transparent/transparent.cpp \
#    blend/semitransparent/semitransparent.cpp \
#    faceculling/faceculling.cpp \
#    framebufferobject/framebufferobject.cpp \
#    skybox/skybox/skybox.cpp \
#    skybox/skyboxoptimized/skyboxoptimized.cpp \
#    skybox/skyboxscale/skyboxscale.cpp \
#    skybox/skyboxclass/skyboxclass.cpp \
#    environmentmapping/reflectio/envreflection.cpp \
#    environmentmapping/reflectionmap/envreflectionmap.cpp \
#    environmentmapping/reflectionmodel/envreflectionmap.cpp \
#    environmentmapping/refraction/refraction.cpp \
#    environmentmapping/refractionmodel/refractionmodel.cpp
    bufferobject/bufferobject.cpp

HEADERS += \
#    shader.h
#    texture.h
#    camera.h
#    simpleobjloader.h
#    mesh.h \
#    model.h
#    skybox.h
