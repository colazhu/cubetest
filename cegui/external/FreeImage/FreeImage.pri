FREEIMAGE_PATH = \
        $$(NXUI_ROOT)/external/FreeImage/ \
        $$(NXUI_ROOT)/external/FreeImage/LibTIFF4 \
        $$(NXUI_ROOT)/external/FreeImage/LibRawLite \
        $$(NXUI_ROOT)/external/FreeImage/DeprecationManager \
        $$(NXUI_ROOT)/external/FreeImage/OpenEXR \
        $$(NXUI_ROOT)/external/FreeImage/OpenEXR/Half \
        $$(NXUI_ROOT)/external/FreeImage/OpenEXR/Iex \
        $$(NXUI_ROOT)/external/FreeImage/OpenEXR/IlmImf \
        $$(NXUI_ROOT)/external/FreeImage/OpenEXR/Imath \
        $$(NXUI_ROOT)/external/FreeImage/OpenEXR/IlmThread

INCLUDEPATH += $$FREEIMAGE_PATH
DEPENDPATH += $$FREEIMAGE_PATH

