include_directories( ${PROJECT_BINARY_DIR}/src/libcalamaresui )

calamares_add_plugin( shtask
    TYPE viewmodule
    EXPORT_MACRO PLUGINDLLEXPORT_PRO
    SOURCES
        ShTaskPage.cpp
        ShTaskViewStep.cpp
        TaskCheck.cpp
        jobs/ShTaskJob.cpp
		jobs/TextWindow.cpp
    UI
        ShTaskPage.ui
        jobs/TextWindow.ui
    LINK_PRIVATE_LIBRARIES
        calamaresui
    SHARED_LIB
)
