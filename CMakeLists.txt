cmake_minimum_required(VERSION 3.16)

project(MultiModeModem
    VERSION 0.6.6
    LANGUAGES CXX
)

# CMake project(VERSION) must stay numeric, but package/app metadata may carry
# the MultiModeModem suffix (for example 0.5.76t).  This keeps macOS crash reports,
# release filenames and in-app version strings aligned without making CMake
# parse a non-numeric project version.
set(M3_PACKAGE_VERSION "${PROJECT_VERSION}")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/M3_VERSION.txt")
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/M3_VERSION.txt" _m3_package_version_txt)
    string(STRIP "${_m3_package_version_txt}" _m3_package_version_txt)
    if(NOT _m3_package_version_txt STREQUAL "")
        set(M3_PACKAGE_VERSION "${_m3_package_version_txt}")
    endif()
endif()
message(STATUS "MultiModeModem package/app version: ${M3_PACKAGE_VERSION}")

set(M3_SOURCE_REVISION "unmarked")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/M3_SOURCE_REVISION.txt")
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/M3_SOURCE_REVISION.txt" _m3_source_revision_txt)
    string(STRIP "${_m3_source_revision_txt}" _m3_source_revision_txt)
    if(NOT _m3_source_revision_txt STREQUAL "")
        set(M3_SOURCE_REVISION "${_m3_source_revision_txt}")
    endif()
endif()
message(STATUS "MultiModeModem source revision: ${M3_SOURCE_REVISION}")

# One distribution workflow run identifies the SAME build on every platform.
# Local builds remain 0 (unknown); source-regression workflow numbers are not
# distribution identities. Configure once, rather than reading CI at runtime.
set(M3_BUILD_NUMBER "$ENV{M3_BUILD_NUMBER}" CACHE STRING "Official distribution build number")
if(NOT "$ENV{M3_BUILD_NUMBER}" STREQUAL "")
    set(M3_BUILD_NUMBER "$ENV{M3_BUILD_NUMBER}" CACHE STRING "Official distribution build number" FORCE)
endif()
if(M3_BUILD_NUMBER STREQUAL "")
    set(M3_BUILD_NUMBER "0")
endif()
if(NOT M3_BUILD_NUMBER MATCHES "^(0|[1-9][0-9]*)$" OR
   M3_BUILD_NUMBER GREATER 2147483647)
    message(FATAL_ERROR "M3_BUILD_NUMBER must be an integer from 0 to 2147483647")
endif()
set(M3_BUILD_COMMIT "$ENV{M3_BUILD_COMMIT}" CACHE STRING "Distribution source commit")
if(NOT "$ENV{M3_BUILD_COMMIT}" STREQUAL "")
    set(M3_BUILD_COMMIT "$ENV{M3_BUILD_COMMIT}" CACHE STRING "Distribution source commit" FORCE)
endif()
if(NOT M3_BUILD_COMMIT STREQUAL "" AND
   NOT M3_BUILD_COMMIT MATCHES "^[0-9a-fA-F]+$")
    message(FATAL_ERROR "M3_BUILD_COMMIT must be a hexadecimal commit id")
endif()
configure_file(cmake/M3BuildInfo.h.in generated/M3BuildInfo.h @ONLY)
message(STATUS "MultiModeModem distribution build: ${M3_BUILD_NUMBER}")

# -------------------------------------------------------------------------
# C++ standard
# -------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------------------------------------------------------
# Performance-oriented release flags
# -------------------------------------------------------------------------
# MultiModeModem is DSP-heavy, especially FT4/FT8 candidate search and LDPC decode.
# Release builds therefore request the fastest safe portable options for both
# Linux/GCC and Windows/MXE/MinGW.  We intentionally avoid -Ofast/fast-math by
# default because modem DSP and timing code should keep IEEE behaviour.

option(M3_MAX_OPTIMIZE
    "Enable high-performance portable Release flags (-O3; LTO remains a separate opt-in)."
    ON)
option(M3_NATIVE_OPTIMIZE
    "Also tune generated code for the build machine CPU (-march=native). OFF keeps packages portable."
    OFF)
option(M3_AVX2_BUILD
    "Build an AVX2/FMA/SSE4.2-only binary. OFF keeps Windows packages portable; FT8 still uses runtime AVX2 dispatch where safe."
    OFF)
option(M3_ENABLE_LTO
    "Enable CMake IPO/LTO for Release builds. OFF by default because GCC LTO has produced undefined MainWindow private-slot references on some Linux builds."
    OFF)
option(M3_MACOS_BUNDLE
    "Build MultiModeModem as a native macOS .app bundle when compiling on Apple platforms."
    ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static in-tree support libraries" FORCE)

include(CheckIPOSupported)
if(M3_MAX_OPTIMIZE)
    if(M3_ENABLE_LTO)
        check_ipo_supported(RESULT _m3_ipo_supported OUTPUT _m3_ipo_error)
        if(_m3_ipo_supported)
            set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
        endif()
    else()
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE OFF)
        message(STATUS "MultiModeModem LTO/IPO disabled by default to avoid GCC/LTO private-slot link regressions")
    endif()

    add_compile_options($<$<CONFIG:Release>:-O3>)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options($<$<CONFIG:Release>:-pipe> $<$<CONFIG:Release>:-fomit-frame-pointer> $<$<CONFIG:Release>:-ffunction-sections> $<$<CONFIG:Release>:-fdata-sections>)
        if(APPLE)
            # Apple ld64 does not support GNU ld's --gc-sections.
            # -dead_strip is the nearest safe macOS equivalent for Release bundles.
            add_link_options($<$<CONFIG:Release>:-Wl,-dead_strip>)
        else()
            add_link_options($<$<CONFIG:Release>:-Wl,--gc-sections>)
        endif()
    endif()
endif()

if(M3_NATIVE_OPTIMIZE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options($<$<CONFIG:Release>:-march=native>)
endif()

if(M3_AVX2_BUILD AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|amd64|i[3-6]86")
    message(WARNING "M3_AVX2_BUILD=ON creates an AVX2/FMA-only executable. Do not use it for generic Windows 10 packages or older CPUs.")
    if(MSVC)
        add_compile_options($<$<CONFIG:Release>:/arch:AVX2>)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options($<$<CONFIG:Release>:-mavx2> $<$<CONFIG:Release>:-mfma> $<$<CONFIG:Release>:-msse4.2>)
    endif()
else()
    message(STATUS "MultiModeModem portable CPU baseline: global AVX2/FMA disabled; runtime-dispatched kernels may still use AVX2 on capable CPUs.")
endif()

# -------------------------------------------------------------------------
# Qt automatic processing
# -------------------------------------------------------------------------

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# -------------------------------------------------------------------------
# Qt detection
# -------------------------------------------------------------------------

option(M3_REQUIRE_QT_LOCATION
    "Require Qt Quick/Location/Positioning for distribution builds instead of an offline-only map."
    OFF)
find_package(Qt6 QUIET COMPONENTS Widgets SerialPort Multimedia OpenGLWidgets PrintSupport Network)
if(Qt6_FOUND)
    set(QT Qt6)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets SerialPort Multimedia PrintSupport Network)
    set(QT Qt5)
endif()
# Optional package searches can reset Qt*_FOUND. Keep the selected Qt family
# stable, including Help and OpenGL links below.
if(M3_REQUIRE_QT_LOCATION)
    find_package(${QT} REQUIRED COMPONENTS Quick Qml Positioning Location)
else()
    find_package(${QT} QUIET COMPONENTS Quick Qml Positioning Location)
endif()

set(M3_WITH_QT_LOCATION OFF)
if(TARGET ${QT}::Quick AND TARGET ${QT}::Qml AND TARGET ${QT}::Location AND TARGET ${QT}::Positioning)
    set(M3_WITH_QT_LOCATION ON)
    message(STATUS "Qt Location QSO map backend enabled (${QT} Location/OSM with offline fallback)")
else()
    message(STATUS "Qt Location QSO map backend not available; using offline planet_map.jpg fallback")
endif()

# -------------------------------------------------------------------------
# Optional Qt Help backend
# -------------------------------------------------------------------------

find_package(${QT} QUIET COMPONENTS Help)

set(M3_WITH_QT_HELP OFF)
if(TARGET ${QT}::Help)
    set(M3_WITH_QT_HELP ON)
    message(STATUS "Qt Help backend enabled for the MM online manual")
else()
    message(STATUS "Qt Help module not available; using embedded HTML fallback help")
endif()

find_program(QHELPGENERATOR_EXECUTABLE
    NAMES qhelpgenerator qhelpgenerator-qt6 qhelpgenerator-qt5
    HINTS
        "${Qt6_DIR}/../../../bin"
        "${Qt5_DIR}/../../../bin"
        "$ENV{MXE_ROOT}/usr/bin"
        "$ENV{PATH}"
)

set(M3_HELP_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/docs/help")
set(M3_HELP_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/docs/help")
set(M3_HELP_LANGUAGES en it fr de no cs)
file(MAKE_DIRECTORY "${M3_HELP_BINARY_DIR}")
file(GLOB_RECURSE M3_HELP_SOURCE_FILES CONFIGURE_DEPENDS
    "${M3_HELP_SOURCE_DIR}/*.html"
    "${M3_HELP_SOURCE_DIR}/*.css"
    "${M3_HELP_SOURCE_DIR}/*.qhp"
    "${M3_HELP_SOURCE_DIR}/*.qhcp"
)

add_custom_target(M3HelpFiles ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${M3_HELP_SOURCE_DIR}" "${M3_HELP_BINARY_DIR}"
    DEPENDS ${M3_HELP_SOURCE_FILES}
    COMMENT "Copying localized MM HTML help files"
)

set(M3_QCH_OUTPUTS)
foreach(M3_HELP_LANG IN LISTS M3_HELP_LANGUAGES)
    list(APPEND M3_QCH_OUTPUTS "${M3_HELP_BINARY_DIR}/MM_${M3_HELP_LANG}.qch")
endforeach()

if(QHELPGENERATOR_EXECUTABLE)
    # Generate all language manuals sequentially through a CMake script instead
    # of launching six qhelpgenerator jobs in parallel.  The previous parallel
    # custom commands could race while copying docs/help into the same build
    # directory; on some Qt/MXE setups qhelpgenerator also returns a non-zero
    # status even after producing usable .qch output.  Since MM always has the
    # embedded multilingual HTML fallback, .qch generation must never break the
    # application build.
    add_custom_command(
        OUTPUT "${M3_HELP_BINARY_DIR}/M3QtHelp.stamp"
        BYPRODUCTS ${M3_QCH_OUTPUTS} "${M3_HELP_BINARY_DIR}/qhelpgenerator.log"
        COMMAND "${CMAKE_COMMAND}"
            "-DHELP_SOURCE_DIR=${M3_HELP_SOURCE_DIR}"
            "-DHELP_BINARY_DIR=${M3_HELP_BINARY_DIR}"
            "-DQHELPGENERATOR_EXECUTABLE=${QHELPGENERATOR_EXECUTABLE}"
            "-DHELP_LANGUAGES=${M3_HELP_LANGUAGES}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateM3QtHelp.cmake"
        COMMAND "${CMAKE_COMMAND}" -E touch "${M3_HELP_BINARY_DIR}/M3QtHelp.stamp"
        DEPENDS ${M3_HELP_SOURCE_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateM3QtHelp.cmake"
        COMMENT "Generating localized MM Qt Help manuals (.qch, optional)"
        VERBATIM
    )
    add_custom_target(M3QtHelp ALL DEPENDS "${M3_HELP_BINARY_DIR}/M3QtHelp.stamp")
    add_dependencies(M3QtHelp M3HelpFiles)
else()
    message(WARNING "qhelpgenerator not found; MM will still build with embedded multilingual HTML help, but .qch files will not be generated")
endif()


# -------------------------------------------------------------------------
# Native MultiModeModem CW receiver and carrier scanner
# -------------------------------------------------------------------------
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

# Live CW architecture: exact-tone carrier discrimination, robust timing-family
# estimation and bounded probabilistic Morse sequence decoding are independent
# components. No legacy Bayesian/geometric-rescue implementation is linked.
# Build number for the title bar and logs: the distribution number when set,
# otherwise a local counter that goes up with every build after a source change.
set(M3_BUILD_NUMBER_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/generated/M3BuildNumber.cpp")
find_package(Git QUIET)
add_custom_target(m3_build_counter ALL
    COMMAND ${CMAKE_COMMAND}
        "-DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
        "-DBINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}"
        "-DOUTPUT=${M3_BUILD_NUMBER_SOURCE}"
        "-DFIXED_NUMBER=${M3_BUILD_NUMBER}"
        "-DGIT_EXECUTABLE=${GIT_EXECUTABLE}"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/M3BuildCounter.cmake"
    BYPRODUCTS "${M3_BUILD_NUMBER_SOURCE}"
    COMMENT "MultiModeModem build number"
    VERBATIM)
add_library(m3_build_number STATIC "${M3_BUILD_NUMBER_SOURCE}" utils/BuildNumber.h)
target_include_directories(m3_build_number PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
add_dependencies(m3_build_number m3_build_counter)

add_library(m3_diagnostics STATIC runtime/SessionDiagnostics.cpp runtime/GzipLogWriter.h)
target_link_libraries(m3_diagnostics PUBLIC ${QT}::Core Threads::Threads ZLIB::ZLIB m3_build_number)
target_include_directories(m3_diagnostics PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")

add_library(m3_cwskimmer STATIC
    modems/cw/skimmer/CwSkimmerTypes.h
    modems/cw/skimmer/CwSkimmerEngine.h
    modems/cw/skimmer/CwSkimmerEngine.cpp
    modems/cw/skimmer/CwCarrierDiscriminator.h
    modems/cw/skimmer/CwCarrierDiscriminator.cpp
    modems/cw/skimmer/CwMorseBeamDecoder.h
    modems/cw/skimmer/CwMorseBeamDecoder.cpp
    modems/cw/skimmer/CwRelativeTimingDecoder.h
    modems/cw/skimmer/CwRelativeTimingDecoder.cpp
    modems/cw/skimmer/CwRelativeTimingTask.h
    modems/cw/skimmer/CwRelativeTimingTask.cpp
    modems/cw/skimmer/SelectedToneCwTracker.h
    modems/cw/skimmer/SelectedToneCwTracker.cpp
)
target_include_directories(m3_cwskimmer PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/modems/cw"
    "${CMAKE_CURRENT_SOURCE_DIR}/modems/cw/skimmer"
)
target_link_libraries(m3_cwskimmer PUBLIC Threads::Threads)
# This target is pure C++ and has no Q_OBJECT classes. Disable Qt autogen on
# AppleClang/Qt6 as an extra isolation layer: otherwise global AUTOMOC creates
# an unnecessary autogen TU and noisy Qt diagnostics can obscure real errors.
set_target_properties(m3_cwskimmer PROPERTIES
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(m3_cwskimmer PRIVATE -Wall -Wextra -Wno-unused-function -Wno-unused-variable)
endif()


# -------------------------------------------------------------------------
# The bundled 77-bit message/hash and QRA primitives retain process-global
# state. FT4/FT8, MSK144 and Q65 therefore use one compiled codec target and
# one lock instead of duplicating symbols or pretending per-mode locks isolate
# the common workspace.
# -------------------------------------------------------------------------
add_library(m3_weak_signal_codec STATIC
    modems/weak_signal/WeakSignalCodecLock.h
    modems/weak_signal/WeakSignalCodecLock.cpp
    third_party/mshv_gpl/nhash.cpp
    third_party/mshv_gpl/port/HvPackUnpackMsg/pack_unpack_msg77.h
    third_party/mshv_gpl/port/HvPackUnpackMsg/pack_unpack_msg77.cpp
)
target_include_directories(m3_weak_signal_codec PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/modems/weak_signal"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/mshv_gpl"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/mshv_gpl/port"
)
target_link_libraries(m3_weak_signal_codec PUBLIC ${QT}::Core Threads::Threads)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(m3_weak_signal_codec PRIVATE -Wall -Wextra)
endif()

# -------------------------------------------------------------------------
# Shared streaming resampler used by the native weak-signal engines and by
# the application text-mode decoders. Keep the implementation in one target
# so standalone regression executables link the exact same DSP code as MM.
# -------------------------------------------------------------------------
add_library(m3_linear_resampler STATIC
    dsp/text/LinearResampler.h
    dsp/text/LinearResampler.cpp
)
target_include_directories(m3_linear_resampler PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/dsp/text"
)
target_link_libraries(m3_linear_resampler PUBLIC ${QT}::Core)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(m3_linear_resampler PRIVATE -Wall -Wextra)
endif()

# Native FT8/FT4 recovery from pinned MSHV source, isolated from TX/CAT.
find_library(M3_FFTW3_LIBRARY NAMES fftw3 fftw3-3 libfftw3.so.3)
if(NOT M3_FFTW3_LIBRARY)
    message(FATAL_ERROR "FT reference recovery requires FFTW3 double precision (libfftw3-dev / fftw / brew fftw)")
endif()
add_library(m3_ft_reference STATIC
    modems/ft8/FtReferenceRecovery.h modems/ft8/FtReferenceRecovery.cpp
    third_party/mshv_gpl/ft_reference/budget.cpp
    third_party/mshv_gpl/ft_reference/decoderft8.cpp
    third_party/mshv_gpl/ft_reference/decoderft8var.cpp
    third_party/mshv_gpl/ft_reference/decoderft4.cpp
    third_party/mshv_gpl/ft_reference/decoderpom.cpp
    third_party/mshv_gpl/ft_reference/fft.cpp)
target_include_directories(m3_ft_reference PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(m3_ft_reference PUBLIC ${QT}::Core m3_weak_signal_codec m3_diagnostics Threads::Threads "${M3_FFTW3_LIBRARY}")
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-ftrivial-auto-var-init=zero" M3_FT_AUTO_INIT)
if(M3_FT_AUTO_INIT)
    target_compile_options(m3_ft_reference PRIVATE -ftrivial-auto-var-init=zero)
else()
    message(FATAL_ERROR "FT recovery requires a compiler supporting -ftrivial-auto-var-init=zero; select a current GCC or Clang toolchain")
endif()

# -------------------------------------------------------------------------
# MSK144 native RX/TX engine, including full 144-bit frames and MSK40 hashed
# short messages. Mode-specific coding is bundled here; shared 77-bit
# message/hash code belongs to m3_weak_signal_codec.
# -------------------------------------------------------------------------
add_library(m3_msk144 STATIC
    modems/msk144/Msk144Decoder.h
    modems/msk144/Msk144Decoder.cpp
    modems/msk144/tx/Msk144Transmitter.h
    modems/msk144/tx/Msk144Transmitter.cpp
    third_party/mshv_gpl/HvTxW/hvqthloc.cpp
    third_party/mshv_gpl/port/HvGenMsk/genmesage_msk.cpp
    third_party/mshv_gpl/port/HvPackUnpackMsg/pack_msg.cpp
    third_party/mshv_gpl/port/HvPackUnpackMsg/unpack_msg.cpp
)
target_include_directories(m3_msk144 PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/modems/msk144"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/mshv_gpl"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/mshv_gpl/port"
)
target_link_libraries(m3_msk144 PUBLIC m3_weak_signal_codec ${QT}::Core ${QT}::Widgets Threads::Threads)
target_link_libraries(m3_msk144 PUBLIC m3_linear_resampler)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(m3_msk144 PRIVATE -Wall -Wextra -Wno-unused-function -Wno-unused-variable -Wno-sign-compare -Wno-unused-parameter)
endif()



# -------------------------------------------------------------------------
# Q65A/B/C/D native RX/TX engine. QRA primitives are bundled here and shared
# message/hash code is linked from m3_weak_signal_codec; the demodulator
# has no optional FFT or runtime backend.
# -------------------------------------------------------------------------
add_library(m3_q65 STATIC
    modems/q65/Q65Decoder.h
    modems/q65/Q65Decoder.cpp
    modems/q65/Q65NativeEngine.h
    modems/q65/Q65NativeEngine.cpp
    modems/q65/Q65Mode.h
    modems/q65/tx/Q65Transmitter.h
    modems/q65/tx/Q65Transmitter.cpp
    third_party/mshv_gpl/port/HvGenQ65/gen_q65.cpp
    third_party/mshv_gpl/port/HvGenQ65/q65_subs.cpp
)
target_include_directories(m3_q65 PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/modems/q65"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/mshv_gpl"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/mshv_gpl/port"
)
target_link_libraries(m3_q65 PUBLIC m3_weak_signal_codec ${QT}::Core ${QT}::Widgets Threads::Threads)
target_link_libraries(m3_q65 PUBLIC m3_linear_resampler)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(m3_q65 PRIVATE -Wall -Wextra -Wno-unused-function -Wno-unused-variable -Wno-sign-compare -Wno-unused-parameter)
endif()

# -------------------------------------------------------------------------
# JS8 (JS8Call 2.2 protocol): native C++ receiver, transmitter and the
# Varicode/JSC message layer of JS8Call (third_party/js8call_gpl).
# -------------------------------------------------------------------------
add_library(m3_js8 STATIC
    modems/js8/Js8Mode.h
    modems/js8/Js8Ldpc.h
    modems/js8/Js8Ldpc.cpp
    modems/js8/Js8NativeEngine.h
    modems/js8/Js8NativeEngine.cpp
    modems/js8/Js8Message.h
    modems/js8/Js8Message.cpp
    modems/js8/Js8Decoder.h
    modems/js8/Js8Decoder.cpp
    modems/js8/Js8CommandBuffer.h
    modems/js8/Js8CommandBuffer.cpp
    modems/js8/Js8Inbox.h
    modems/js8/Js8Inbox.cpp
    modems/js8/Js8AutoReply.h
    modems/js8/Js8AutoReply.cpp
    modems/js8/tx/Js8Transmitter.h
    modems/js8/tx/Js8Transmitter.cpp
    third_party/js8call_gpl/Varicode.h
    third_party/js8call_gpl/Varicode.cpp
    third_party/js8call_gpl/Jsc.h
    third_party/js8call_gpl/Jsc.cpp
    third_party/js8call_gpl/DecodedFrame.h
    third_party/js8call_gpl/DecodedFrame.cpp
    third_party/js8call_gpl/Js8LdpcTables.h
    third_party/js8call_gpl/js8_jsc.qrc
)
set_target_properties(m3_js8 PROPERTIES AUTORCC ON AUTOMOC ON)
target_include_directories(m3_js8 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/modems/js8")
target_link_libraries(m3_js8 PUBLIC ${QT}::Core ${QT}::Gui m3_linear_resampler Threads::Threads "${M3_FFTW3_LIBRARY}")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(m3_js8 PRIVATE -Wall -Wextra)
    set_source_files_properties(third_party/js8call_gpl/Varicode.cpp third_party/js8call_gpl/Jsc.cpp
        third_party/js8call_gpl/DecodedFrame.cpp PROPERTIES COMPILE_OPTIONS "-Wno-deprecated-declarations;-Wno-sign-compare;-Wno-unused-parameter;-Wno-unused-variable;-Wno-unused-but-set-variable;-Wno-implicit-fallthrough")
endif()

# -------------------------------------------------------------------------
# Sources
# -------------------------------------------------------------------------

add_executable(MultiModeModem
    main.cpp
    FtCqWorkListUi.cpp
    Js8Ui.cpp
    ModeFrequencyUi.cpp
    mainwindow.cpp
    FtAutomationLog.cpp
    updates/UpdateController.cpp updates/UpdateController.h
    updates/Ed25519Verify.cpp updates/Ed25519Verify.h updates/UpdateSigningKeys.h
    callsign/QrzLookupPanel.cpp callsign/QrzLookupPanel.h widgets/FrequencyScaleTicks.h
    widgets/CurrentPageStack.h
    mainwindow.h
    mainwindow.ui
    resources.qrc
    audio/AudioEngine.cpp
    audio/AudioDeviceCache.cpp
    audio/AudioDeviceCache.h
    audio/BoundedAudioDispatcher.cpp
    audio/RxAudioRecorder.h
    audio/RxAudioRecorder.cpp
    audio/AudioEngine.h
    audio/BoundedAudioDispatcher.h
    audio/WavFileReader.cpp
    audio/WavFileReader.h
    audio/TxAudioEngine.cpp
    audio/TxAudioEngine.h
    audio/TxOutputDevice.h
    audio/FtTxWorker.cpp
    audio/FtTxWorker.h
    audio/AudioBlock.h
    widgets/WaterfallWidget.h
    widgets/CqSpots.h
    modems/ft8/FtCqDirective.h
    modems/ft8/FtDecodeFilter.h
    modems/ft8/FtDecodeColours.h
    widgets/SkimmerOverlay.cpp widgets/SkimmerOverlay.h
    widgets/WaterfallWidget.cpp
    widgets/RttyScopeWidget.h
    widgets/RttyScopeWidget.cpp
    widgets/QsoMapWidget.h
    widgets/QsoMapWidget.cpp
    widgets/RadioTelescopeHeatmapWidget.h
    widgets/RadioTelescopeHeatmapWidget.cpp
    widgets/CwDiagnosticWidget.h
    widgets/CwDiagnosticWidget.cpp
    utils/RuntimeI18n.h
    utils/RuntimeI18n.cpp
    utils/CockpitTheme.h
    utils/CockpitTheme.cpp utils/WindowResize.cpp
    utils/SystemResourceManager.h
    utils/SystemResourceManager.cpp
    dsp/DspEngine.h
    dsp/DspEngine.cpp
    dsp/WaterfallLeveler.cpp
    dsp/common/DspConditioner.h
    dsp/common/DspConditioner.cpp
    dsp/cpu/CpuFeatures.h
    dsp/cpu/CpuFeatures.cpp
    dsp/text/Nco.h
    dsp/text/Nco.cpp
    dsp/text/BaudAwareFilter.h
    dsp/text/BaudAwareFilter.cpp
    dsp/text/AtcDetector.h
    dsp/text/AtcDetector.cpp
    dsp/text/SymbolClockDpll.h
    dsp/text/SymbolClockDpll.cpp
    dsp/text/SoftSquelch.h
    dsp/text/SoftSquelch.cpp
    dsp/text/GoertzelToneBank.h
    dsp/text/GoertzelToneBank.cpp
    widgets/FaxImageWidget.h
    widgets/FaxImageWidget.cpp
    modems/weatherfax/WeatherFaxDecoder.h
    modems/weatherfax/WeatherFaxDecoder.cpp
    core/tx/TxModulator.h
    modems/weatherfax/tx/WeatherFaxTransmitter.h
    modems/weatherfax/tx/WeatherFaxTransmitter.cpp
    modems/sstv/tx/SstvTransmitter.h
    modems/sstv/tx/SstvTransmitter.cpp
    modems/rtty/tx/RttyTransmitter.h
    modems/rtty/tx/RttyTransmitter.cpp
    modems/bpsk31/tx/Bpsk31Transmitter.h
    modems/bpsk31/tx/Bpsk31Transmitter.cpp
    modems/cw/tx/CwTransmitter.h
    modems/cw/tx/CwMorseSequence.cpp
    modems/cw/tx/CwMorseSequence.h
    modems/cw/tx/CwSerialTransmitter.cpp
    modems/cw/tx/CwSerialTransmitter.h
    dialogs/CwTxSettingsDialog.cpp
    dialogs/CwTxSettingsDialog.h
    modems/cw/tx/CwTransmitter.cpp
    modems/mfsk/tx/MfskTransmitter.h
    modems/mfsk/tx/MfskTransmitter.cpp
    modems/hell/tx/HellschreiberTransmitter.h
    modems/hell/tx/HellschreiberTransmitter.cpp
    modems/ft8/tx/Ft8Transmitter.h
    modems/ft8/tx/Ft8Transmitter.cpp
    modems/radiotelescope/RadioTelescopeMode.h
    modems/msk144/Msk144Mode.h
    modems/msk144/tx/Msk144Transmitter.h
    dsp/FrequencyMarker.h
    modems/sstv/SstvModeDefinition.h
    modems/sstv/SstvModeDefinition.cpp
    modems/sstv/SstvDecoder.h
    modems/sstv/SstvDecoder.cpp
    third_party/mmsstv_lgpl/MmsstvRxCore.h
    third_party/mmsstv_lgpl/MmsstvRxCore.cpp
    modems/rtty/RttyDecoder.h
    modems/rtty/RttyDecoder.cpp
    modems/rtty/RttyMultiDecoder.h
    modems/rtty/RttyMultiDecoder.cpp
    modems/rtty/contest/RttyContestRules.h
    runtime/SstvImageArchive.cpp
    runtime/SstvImageArchive.h
    runtime/RxDecoderWorker.cpp
    runtime/RxDecoderWorker.h
    runtime/LogbookIndexWorker.h
    runtime/LogbookIndexWorker.cpp
    runtime/TextAssistWorker.h
    runtime/TextAssistWorker.cpp
    utils/SerialPortEndpoint.h
    runtime/CwSerialTxSession.cpp
    runtime/AsyncCatCommand.h
    modems/rtty/contest/RttyContestRules.cpp
    modems/bpsk31/Bpsk31Decoder.h
    modems/bpsk31/Bpsk31Decoder.cpp
    modems/mfsk/MfskDecoder.h
    modems/mfsk/MfskDecoder.cpp
    modems/cw/CwDecoder.h
    modems/cw/CwDecoder.cpp
    modems/cw/neural/CwNeuralDecoder.cpp
    modems/cw/neural/CwNeuralDecoder.h
    modems/cw/CwBandSkimmer.cpp modems/cw/CwBandSkimmer.h modems/cw/CwBandSkimmerWorker.h dsp/text/SkimmerSpot.h dsp/text/SkimmerCqTracker.h
    modems/hell/HellschreiberDecoder.h
    modems/hell/HellschreiberDecoder.cpp
    modems/ft8/Ft8Mode.h
    modems/ft8/Ft8Mode.cpp
    modems/ft8/Ft8RxDecoder.h
    modems/ft8/Ft8RxDecoder.cpp
    modems/ft8/FtNativeSpectrum.h
    modems/ft8/FtCoherentAlignment.h
    modems/ft8/FtSignalEnvelope.h
    modems/ft8/FtLdpcCheck.h
    dsp/FftwPlannerLock.h
    dsp/gpu/FtSyncCompute.cpp
    modems/ft8/FtDecodedText.h
    modems/ft8/FtDecodedText.cpp
    modems/ft8/FtStandardMessageSet.h
    modems/ft8/FtStandardMessageSet.cpp
    modems/ft8/FtQsoSequencer.h
    modems/ft8/FtQsoSession.h
    modems/ft8/FtQsoSession.cpp
    modems/ft8/FtTxPlan.h
    modems/ft8/FtQsoSequencer.cpp
    modems/ft8/FtSlotScheduler.h
    modems/ft8/FtSlotScheduler.cpp
    third_party/decodium_gpl/port/NtpClient.hpp
    third_party/decodium_gpl/port/NtpClient.cpp
    third_party/mshv_gpl/port/genpom.h
    third_party/mshv_gpl/port/genpom.cpp
    third_party/mshv_gpl/port/HvGenFt8/gen_ft8.h
    third_party/mshv_gpl/port/HvGenFt8/gen_ft8.cpp
    third_party/mshv_gpl/port/HvGenFt8/bpdecode_ft8_174_91.h
    third_party/mshv_gpl/port/HvGenFt4/gen_ft4.h
    third_party/mshv_gpl/port/HvGenFt4/gen_ft4.cpp
    dsp/FrequencyTracker.h
    dsp/FrequencyTracker.cpp
    settings/AppSettings.h
    settings/AppSettings.cpp
    dialogs/HelpDialog.h
    dialogs/HelpDialog.cpp
    dialogs/BandSchedulerDialog.h
    dialogs/BandSchedulerDialog.cpp
    dialogs/FrequencyListEditor.h
    dialogs/FrequencyListEditor.cpp
    dialogs/AppSettingsDialog.h
    dialogs/AppSettingsDialog.cpp
    dialogs/AutoModeNoticeDialog.cpp
    dialogs/AutoModeNoticeDialog.h
    dialogs/AudioSettingsDialog.ui
    dialogs/AudioSettingsDialog.h
    dialogs/AudioSettingsDialog.cpp
    dialogs/SoundCardCalibrationDialog.h
    dialogs/SoundCardCalibrationDialog.cpp
    dialogs/TextMacroSettingsDialog.h
    dialogs/TextMacroSettingsDialog.cpp
    dialogs/LogbookDialog.h
    dialogs/LogbookDialog.cpp
    dialogs/ContestExportWizard.cpp
    dialogs/ContestExportWizard.h
    logbook/ContestExport.h
    logbook/ContestExchange.cpp
    logbook/ContestExchange.h
    dialogs/LogbookSettingsDialog.h
    dialogs/LogbookSettingsDialog.cpp
    dialogs/RigControlSettingsDialog.h
    dialogs/RigControlSettingsDialog.cpp
    rig/HamlibController.h
    rig/HamlibController.cpp
    rotator/CatRotatorController.h
    rotator/CatRotatorController.cpp
    rotator/RotatorPeakSearch.h
    rotator/RotatorPeakSearch.cpp
    rotator/CatRotatorPanel.h
    rotator/CatRotatorPanel.cpp
    rotator/NavballWidget.h
    rotator/NavballWidget.cpp
    dialogs/SstvImageEditorDialog.h
    dialogs/SstvImageEditorDialog.cpp
    logbook/AdifLogbook.h
    logbook/AdifLogbook.cpp
    network/QsoUdpBroadcaster.h
    network/QsoUdpBroadcaster.cpp
    network/WavelogClient.h
    network/WavelogClient.cpp
    network/PskReporter.cpp
    dxcc/CtyCountryFile.h
    dxcc/CtyCountryFile.cpp
)

# -------------------------------------------------------------------------
# Windows resources
# -------------------------------------------------------------------------

if(WIN32)
    set_target_properties(MultiModeModem PROPERTIES
        WIN32_EXECUTABLE ON
    )

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/multimodemodem.rc")
        target_sources(MultiModeModem PRIVATE
            multimodemodem.rc
        )
    endif()
endif()

# -------------------------------------------------------------------------
# macOS application bundle
# -------------------------------------------------------------------------

if(APPLE AND M3_MACOS_BUNDLE)
    if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
        set(CMAKE_OSX_DEPLOYMENT_TARGET "13.0" CACHE STRING "Minimum macOS deployment target" FORCE)
    endif()
    set(M3_MACOS_BUNDLE_ID "de.migri.multimodemodem" CACHE STRING "macOS bundle identifier")
    set(M3_MACOS_ICON "${CMAKE_CURRENT_BINARY_DIR}/MultiModeModem.icns")

    add_custom_command(
        OUTPUT "${M3_MACOS_ICON}"
        COMMAND "${CMAKE_COMMAND}"
            "-DINPUT_ICON=${CMAKE_CURRENT_SOURCE_DIR}/icons/multimodemodem_1024.png"
            "-DOUTPUT_ICON=${M3_MACOS_ICON}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/macos/GenerateMacOSIcon.cmake"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/icons/multimodemodem_1024.png"
                "${CMAKE_CURRENT_SOURCE_DIR}/cmake/macos/GenerateMacOSIcon.cmake"
        COMMENT "Generating MultiModeModem.icns for macOS bundle"
        VERBATIM
    )

    target_sources(MultiModeModem PRIVATE "${M3_MACOS_ICON}")
    set_source_files_properties("${M3_MACOS_ICON}" PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )

    set_target_properties(MultiModeModem PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "MultiModeModem"
        MACOSX_BUNDLE_GUI_IDENTIFIER "${M3_MACOS_BUNDLE_ID}"
        MACOSX_BUNDLE_ICON_FILE "MultiModeModem.icns"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/cmake/macos/Info.plist.in"
    )

    # MultiModeModem expects cty.csv beside QCoreApplication::applicationDirPath().
    # In a macOS bundle that is Contents/MacOS, so copy it there for unsigned CI packages.
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cty.csv")
        add_custom_command(TARGET MultiModeModem POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${CMAKE_CURRENT_SOURCE_DIR}/cty.csv"
                "$<TARGET_BUNDLE_CONTENT_DIR:MultiModeModem>/MacOS/cty.csv"
            COMMENT "Copying cty.csv into MultiModeModem.app/Contents/MacOS"
            VERBATIM
        )
    endif()

endif()


add_dependencies(MultiModeModem M3HelpFiles)
if(TARGET M3QtHelp)
    add_dependencies(MultiModeModem M3QtHelp)
endif()

# Keep string literals and resource-generated UI text UTF-8 on Windows/MinGW/MSVC.
if(MSVC)
    target_compile_options(MultiModeModem PRIVATE /utf-8)
elseif(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Apple Clang already emits UTF-8 narrow strings by default; keep only the
    # explicit input encoding flag to avoid compiler-version-specific charset
    # option issues on GitHub macOS runners.
    target_compile_options(MultiModeModem PRIVATE -finput-charset=UTF-8)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(MultiModeModem PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

target_compile_definitions(MultiModeModem PRIVATE M3_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
# -------------------------------------------------------------------------
# Link libraries
# -------------------------------------------------------------------------


# Copy the external, data-driven RTTY/CW contest rule databases next to the executable.
# Contest rules are intentionally not compiled into MultiModeModem: replacing these files
# updates exchanges/macros/scoring without rebuilding the application.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/rtty_rules")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/rtty_rules" "${CMAKE_CURRENT_BINARY_DIR}/rtty_rules" COPYONLY)
    add_custom_command(TARGET MultiModeModem POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/rtty_rules"
            "$<TARGET_FILE_DIR:MultiModeModem>/rtty_rules"
        COMMENT "Copying external rtty_rules next to MultiModeModem"
        VERBATIM)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cw_rules")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cw_rules" "${CMAKE_CURRENT_BINARY_DIR}/cw_rules" COPYONLY)
    add_custom_command(TARGET MultiModeModem POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/cw_rules"
            "$<TARGET_FILE_DIR:MultiModeModem>/cw_rules"
        COMMENT "Copying external cw_rules next to MultiModeModem"
        VERBATIM)
endif()

# Copy optional country-files.com cty.csv next to the build executable.
# Users can replace this file with a fresh download without rebuilding.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cty.csv")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cty.csv" "${CMAKE_CURRENT_BINARY_DIR}/cty.csv" COPYONLY)
endif()

target_include_directories(MultiModeModem PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third_party/opencl_headers" "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/generated")
set_property(TARGET MultiModeModem PROPERTY AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}")
if(WIN32)
    target_link_libraries(MultiModeModem PRIVATE winmm)
endif()

target_link_libraries(MultiModeModem PRIVATE
    m3_ft_reference
    m3_diagnostics
    m3_cwskimmer
    m3_msk144
    m3_q65
    m3_weak_signal_codec
    m3_js8
    m3_linear_resampler
    ${QT}::Widgets
    ${QT}::SerialPort
    ${QT}::Multimedia
    ${QT}::PrintSupport
    ${QT}::Network
)
if(M3_WITH_QT_LOCATION)
    target_compile_definitions(MultiModeModem PRIVATE M3_WITH_QT_LOCATION=1)
    target_link_libraries(MultiModeModem PRIVATE
        ${QT}::Quick
        ${QT}::Qml
        ${QT}::Location
        ${QT}::Positioning
    )
endif()
if(M3_WITH_QT_HELP)
    target_compile_definitions(MultiModeModem PRIVATE M3_WITH_QT_HELP=1)
    target_link_libraries(MultiModeModem PRIVATE ${QT}::Help)
endif()
if(QT STREQUAL "Qt6")
    target_link_libraries(MultiModeModem PRIVATE Qt6::OpenGLWidgets)
endif()

include(GNUInstallDirs)

# -------------------------------------------------------------------------
# Required bundled/system Hamlib CAT/PTT support
# -------------------------------------------------------------------------
#
# v1.18 keeps the policy from "optional stub if missing" to "real Hamlib
# must be linked" by default.  The source package includes Hamlib under
# third_party/hamlib_lgpl/source and the build scripts compile it before
# configuring MultiModeModem.
#
# Native Linux:
#   ./third_party/hamlib_lgpl/build_hamlib.sh
#   cmake -S . -B build-linux -DHAMLIB_ROOT=$PWD/third_party/hamlib_lgpl/install-linux-x86_64
#
# Windows via MXE:
#   ./third_party/hamlib_lgpl/build_hamlib_mxe.sh
#   $MXE_ROOT/usr/bin/x86_64-w64-mingw32.static-cmake -S . -B build-win64-static \
#       -DHAMLIB_ROOT=$PWD/third_party/hamlib_lgpl/install-win64-static

option(M3_REQUIRE_HAMLIB
    "Fail configuration when Hamlib is not available. v1.40 defaults this ON for all-in-one CAT/PTT builds."
    ON)

set(HAMLIB_ROOT "" CACHE PATH "Hamlib install prefix produced by bundled build scripts or a system prefix")
set(M3_BUNDLED_HAMLIB_PREFIX_LINUX
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/hamlib_lgpl/install-linux-x86_64"
    CACHE PATH "Native Linux Hamlib prefix produced by third_party/hamlib_lgpl/build_hamlib.sh")
set(M3_BUNDLED_HAMLIB_PREFIX_WIN64_STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/hamlib_lgpl/install-win64-static"
    CACHE PATH "Windows/MXE Hamlib prefix produced by third_party/hamlib_lgpl/build_hamlib_mxe.sh")
set(M3_BUNDLED_HAMLIB_PREFIX_LEGACY
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/hamlib_lgpl/install"
    CACHE PATH "Legacy bundled Hamlib prefix")

set(_M3_HAMLIB_HINTS)
if(HAMLIB_ROOT)
    list(APPEND _M3_HAMLIB_HINTS "${HAMLIB_ROOT}")
endif()
if(WIN32 AND EXISTS "${M3_BUNDLED_HAMLIB_PREFIX_WIN64_STATIC}")
    list(APPEND _M3_HAMLIB_HINTS "${M3_BUNDLED_HAMLIB_PREFIX_WIN64_STATIC}")
endif()
if(UNIX AND NOT APPLE AND EXISTS "${M3_BUNDLED_HAMLIB_PREFIX_LINUX}")
    list(APPEND _M3_HAMLIB_HINTS "${M3_BUNDLED_HAMLIB_PREFIX_LINUX}")
endif()
if(EXISTS "${M3_BUNDLED_HAMLIB_PREFIX_LEGACY}")
    list(APPEND _M3_HAMLIB_HINTS "${M3_BUNDLED_HAMLIB_PREFIX_LEGACY}")
endif()

option(M3_AUTOBUILD_HAMLIB
    "Automatically build bundled Hamlib from source during CMake configuration when it is missing."
    OFF)

if(M3_AUTOBUILD_HAMLIB)
    set(_M3_AUTOBUILD_PREFIX "")
    set(_M3_AUTOBUILD_SCRIPT "")
    if(WIN32)
        set(_M3_AUTOBUILD_PREFIX "${M3_BUNDLED_HAMLIB_PREFIX_WIN64_STATIC}")
        set(_M3_AUTOBUILD_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/third_party/hamlib_lgpl/build_hamlib_mxe.sh")
    elseif(UNIX AND NOT APPLE)
        set(_M3_AUTOBUILD_PREFIX "${M3_BUNDLED_HAMLIB_PREFIX_LINUX}")
        set(_M3_AUTOBUILD_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/third_party/hamlib_lgpl/build_hamlib.sh")
    endif()

    if(_M3_AUTOBUILD_PREFIX AND _M3_AUTOBUILD_SCRIPT
       AND EXISTS "${_M3_AUTOBUILD_SCRIPT}"
       AND NOT EXISTS "${_M3_AUTOBUILD_PREFIX}/include/hamlib/rig.h")
        message(STATUS "Bundled Hamlib is missing; building it now: ${_M3_AUTOBUILD_PREFIX}")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E env
                    "HAMLIB_PREFIX=${_M3_AUTOBUILD_PREFIX}"
                    "HAMLIB_STATIC=on"
                    "HAMLIB_SHARED=off"
                    "${_M3_AUTOBUILD_SCRIPT}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            RESULT_VARIABLE _hamlib_build_result
        )
        if(NOT _hamlib_build_result EQUAL 0)
            message(WARNING "Bundled Hamlib auto-build failed with exit code ${_hamlib_build_result}")
        endif()
    endif()

    if(_M3_AUTOBUILD_PREFIX AND EXISTS "${_M3_AUTOBUILD_PREFIX}")
        list(PREPEND _M3_HAMLIB_HINTS "${_M3_AUTOBUILD_PREFIX}")
    endif()
endif()

# Prefer pkg-config because it carries static-link dependencies on platforms
# such as MinGW/MXE.  Fall back to explicit prefix probing, then to normal
# find_path/find_library.  The explicit prefix step is important for MXE because
# CMAKE_FIND_ROOT_PATH can otherwise re-root absolute HAMLIB_ROOT hints and miss
# third_party/hamlib_lgpl/install-win64-static even when Hamlib was built there.
list(REMOVE_DUPLICATES _M3_HAMLIB_HINTS)
if(_M3_HAMLIB_HINTS)
    message(STATUS "Hamlib search prefixes: ${_M3_HAMLIB_HINTS}")
endif()

set(_M3_ORIGINAL_PKG_CONFIG_PATH "$ENV{PKG_CONFIG_PATH}")
set(_M3_ORIGINAL_PKG_CONFIG_LIBDIR "$ENV{PKG_CONFIG_LIBDIR}")
set(_M3_HAMLIB_PKGCONFIG_DIRS)
foreach(_prefix IN LISTS _M3_HAMLIB_HINTS)
    if(EXISTS "${_prefix}/lib/pkgconfig")
        list(APPEND _M3_HAMLIB_PKGCONFIG_DIRS "${_prefix}/lib/pkgconfig")
    endif()
    if(EXISTS "${_prefix}/lib64/pkgconfig")
        list(APPEND _M3_HAMLIB_PKGCONFIG_DIRS "${_prefix}/lib64/pkgconfig")
    endif()
endforeach()
list(REMOVE_DUPLICATES _M3_HAMLIB_PKGCONFIG_DIRS)

foreach(_pcdir IN LISTS _M3_HAMLIB_PKGCONFIG_DIRS)
    if("$ENV{PKG_CONFIG_PATH}" STREQUAL "")
        set(ENV{PKG_CONFIG_PATH} "${_pcdir}")
    else()
        set(ENV{PKG_CONFIG_PATH} "${_pcdir}:$ENV{PKG_CONFIG_PATH}")
    endif()
endforeach()

# With MXE/static cross builds PKG_CONFIG_LIBDIR is often restricted to the MXE
# sysroot.  Add the private Hamlib pkgconfig directory there too so target
# pkg-config can see hamlib.pc without using any native .pc files.
if(CMAKE_CROSSCOMPILING)
    foreach(_pcdir IN LISTS _M3_HAMLIB_PKGCONFIG_DIRS)
        if("$ENV{PKG_CONFIG_LIBDIR}" STREQUAL "")
            set(ENV{PKG_CONFIG_LIBDIR} "${_pcdir}")
        else()
            set(ENV{PKG_CONFIG_LIBDIR} "${_pcdir}:$ENV{PKG_CONFIG_LIBDIR}")
        endif()
    endforeach()
endif()

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(HAMLIB_PC QUIET IMPORTED_TARGET hamlib)
endif()

# Do not leak project-specific pkg-config search paths into later configure
# checks or parent projects that include MultiModeModem as a subdirectory.
set(ENV{PKG_CONFIG_PATH} "${_M3_ORIGINAL_PKG_CONFIG_PATH}")
set(ENV{PKG_CONFIG_LIBDIR} "${_M3_ORIGINAL_PKG_CONFIG_LIBDIR}")

set(M3_HAMLIB_FOUND OFF)
set(HAMLIB_LIBRARY_DIR "")
set(HAMLIB_INCLUDE_DIR "")
set(HAMLIB_LIBRARY "")

if(TARGET PkgConfig::HAMLIB_PC)
    target_compile_definitions(MultiModeModem PRIVATE M3_WITH_HAMLIB=1)
    target_link_libraries(MultiModeModem PRIVATE PkgConfig::HAMLIB_PC)
    # MSYS2/MinGW static Hamlib can reference GetAdaptersAddresses from iphlpapi.
    # Keep this Windows-only and leave all non-Windows link lines unchanged.
    if(WIN32)
        target_link_libraries(MultiModeModem PRIVATE ws2_32 setupapi winmm iphlpapi)
    endif()
    if(HAMLIB_PC_LIBRARY_DIRS)
        list(GET HAMLIB_PC_LIBRARY_DIRS 0 HAMLIB_LIBRARY_DIR)
    endif()
    set(M3_HAMLIB_FOUND ON)
    message(STATUS "Hamlib support enabled via pkg-config: ${HAMLIB_PC_VERSION}")
else()
    # First, trust an explicit install prefix by checking the concrete files.
    # This avoids MXE root-path rewriting problems seen with find_path/find_library.
    foreach(_prefix IN LISTS _M3_HAMLIB_HINTS)
        if(NOT HAMLIB_INCLUDE_DIR AND EXISTS "${_prefix}/include/hamlib/rig.h")
            set(_M3_HAMLIB_CANDIDATE_LIBS
                "${_prefix}/lib/libhamlib.a"
                "${_prefix}/lib/libhamlib.dll.a"
                "${_prefix}/lib/libhamlib-4.a"
                "${_prefix}/lib/libhamlib-4.dll.a"
                "${_prefix}/lib/hamlib.lib"
                "${_prefix}/lib64/libhamlib.a"
                "${_prefix}/lib64/libhamlib.dll.a"
                "${_prefix}/lib64/libhamlib-4.a"
                "${_prefix}/lib64/libhamlib-4.dll.a"
                "${_prefix}/lib64/hamlib.lib"
            )
            foreach(_lib IN LISTS _M3_HAMLIB_CANDIDATE_LIBS)
                if(EXISTS "${_lib}")
                    set(HAMLIB_INCLUDE_DIR "${_prefix}/include")
                    set(HAMLIB_LIBRARY "${_lib}")
                    message(STATUS "Hamlib explicit prefix match: ${_prefix}")
                    break()
                endif()
            endforeach()
        endif()
    endforeach()

    if(NOT HAMLIB_INCLUDE_DIR OR NOT HAMLIB_LIBRARY)
        find_path(HAMLIB_INCLUDE_DIR
            NAMES hamlib/rig.h
            HINTS ${_M3_HAMLIB_HINTS}
            PATH_SUFFIXES include
            NO_DEFAULT_PATH
        )
        find_library(HAMLIB_LIBRARY
            NAMES hamlib hamlib-4 libhamlib libhamlib-4
            HINTS ${_M3_HAMLIB_HINTS}
            PATH_SUFFIXES lib lib64
            NO_DEFAULT_PATH
        )
    endif()

    if(NOT HAMLIB_INCLUDE_DIR OR NOT HAMLIB_LIBRARY)
        find_path(HAMLIB_INCLUDE_DIR
            NAMES hamlib/rig.h
            HINTS ${_M3_HAMLIB_HINTS}
            PATH_SUFFIXES include
        )
        find_library(HAMLIB_LIBRARY
            NAMES hamlib hamlib-4 libhamlib libhamlib-4
            HINTS ${_M3_HAMLIB_HINTS}
            PATH_SUFFIXES lib lib64
        )
    endif()

    if(HAMLIB_INCLUDE_DIR AND HAMLIB_LIBRARY)
        target_compile_definitions(MultiModeModem PRIVATE M3_WITH_HAMLIB=1)
        target_include_directories(MultiModeModem PRIVATE ${HAMLIB_INCLUDE_DIR})
        target_link_libraries(MultiModeModem PRIVATE ${HAMLIB_LIBRARY})
        get_filename_component(HAMLIB_LIBRARY_DIR "${HAMLIB_LIBRARY}" DIRECTORY)

        # Direct static fallback dependencies.  pkg-config is preferred because
        # it is more exact, especially for MXE; these are the common minimums.
        if(UNIX AND NOT APPLE)
            target_link_libraries(MultiModeModem PRIVATE m pthread ${CMAKE_DL_LIBS})
        endif()
        if(WIN32)
            target_link_libraries(MultiModeModem PRIVATE ws2_32 setupapi winmm iphlpapi)
        endif()

        set(M3_HAMLIB_FOUND ON)
        message(STATUS "Hamlib support enabled: ${HAMLIB_LIBRARY}")
    endif()
endif()

if(NOT M3_HAMLIB_FOUND)
    set(_msg "Hamlib was not found. v1.40 requires Hamlib for all-in-one CAT/PTT builds.
"
             "Searched prefixes: ${_M3_HAMLIB_HINTS}
"
             "Build the bundled copy first:
"
             "  Linux:   ./third_party/hamlib_lgpl/build_hamlib.sh
"
             "  Windows: ./third_party/hamlib_lgpl/build_hamlib_mxe.sh
"
             "Then re-run CMake with -DHAMLIB_ROOT=<that install prefix>.
"
             "Set -DM3_REQUIRE_HAMLIB=OFF only for developer stub builds.")
    if(M3_REQUIRE_HAMLIB)
        message(FATAL_ERROR "${_msg}")
    else()
        message(WARNING "${_msg}")
    endif()
endif()

# If a dynamic bundled Hamlib is used on Linux, make development runs work and
# install the runtime library beside the application.  Static Hamlib builds need
# no runtime copy.
if(M3_HAMLIB_FOUND AND HAMLIB_LIBRARY_DIR)
    if(HAMLIB_LIBRARY_DIR MATCHES "third_party/hamlib_lgpl")
        if(UNIX AND NOT APPLE)
            set_target_properties(MultiModeModem PROPERTIES
                BUILD_RPATH "${HAMLIB_LIBRARY_DIR}"
                INSTALL_RPATH "\$ORIGIN/../lib;\$ORIGIN")
        endif()
        file(GLOB M3_HAMLIB_RUNTIME_LIBS
            "${HAMLIB_LIBRARY_DIR}/libhamlib.so*"
            "${HAMLIB_LIBRARY_DIR}/libhamlib*.dylib"
            "${HAMLIB_LIBRARY_DIR}/hamlib*.dll"
            "${HAMLIB_LIBRARY_DIR}/libhamlib*.dll"
        )
        if(M3_HAMLIB_RUNTIME_LIBS)
            install(FILES ${M3_HAMLIB_RUNTIME_LIBS}
                DESTINATION ${CMAKE_INSTALL_LIBDIR})
        endif()
    endif()
endif()

# -------------------------------------------------------------------------
# Install
# -------------------------------------------------------------------------

# The target and the program file are both MultiModeModem.
set_target_properties(MultiModeModem PROPERTIES OUTPUT_NAME MultiModeModem)

install(TARGETS MultiModeModem
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION .
)

# Native regression tests are registered with CTest so CI can protect the
# timing-sensitive production components instead of only producing packages.
include(CTest)
if(BUILD_TESTING)
    add_executable(m3_ft_timing_policy tests/FtTimingPolicyRegression.cpp)
    set_target_properties(m3_ft_timing_policy PROPERTIES AUTOMOC OFF AUTOUIC OFF AUTORCC OFF)
    add_test(NAME m3_ft_timing_policy COMMAND m3_ft_timing_policy)
    set_tests_properties(m3_ft_timing_policy PROPERTIES TIMEOUT 10)
    add_executable(m3_ft_reference_budget
        tests/FtReferenceBudgetRegression.cpp
        tests/FtReferenceBudgetPeer.cpp
        third_party/mshv_gpl/ft_reference/budget.cpp)
    target_include_directories(m3_ft_reference_budget PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(m3_ft_reference_budget PRIVATE Threads::Threads)
    set_target_properties(m3_ft_reference_budget PROPERTIES AUTOMOC OFF AUTOUIC OFF AUTORCC OFF)
    add_test(NAME m3_ft_reference_budget COMMAND m3_ft_reference_budget)
    set_tests_properties(m3_ft_reference_budget PROPERTIES TIMEOUT 10)
    add_executable(m3_ft_ldpc_check tests/FtLdpcCheckRegression.cpp)
    target_include_directories(m3_ft_ldpc_check PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    add_test(NAME m3_ft_ldpc_check COMMAND m3_ft_ldpc_check)
    set_tests_properties(m3_ft_ldpc_check PROPERTIES TIMEOUT 30)
    add_executable(m3_ft_native_spectrum tests/FtNativeSpectrumRegression.cpp)
    target_include_directories(m3_ft_native_spectrum PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_ft_native_spectrum PRIVATE Threads::Threads "${M3_FFTW3_LIBRARY}")
    add_test(NAME m3_ft_native_spectrum COMMAND m3_ft_native_spectrum)
    set_tests_properties(m3_ft_native_spectrum PROPERTIES TIMEOUT 30)
    add_executable(m3_ft_coherent_alignment tests/FtCoherentAlignmentRegression.cpp)
    target_include_directories(m3_ft_coherent_alignment PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    add_test(NAME m3_ft_coherent_alignment COMMAND m3_ft_coherent_alignment)
    set_tests_properties(m3_ft_coherent_alignment PROPERTIES TIMEOUT 30)
    add_executable(m3_ft_signal_envelope tests/FtSignalEnvelopeRegression.cpp)
    target_include_directories(m3_ft_signal_envelope PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    add_test(NAME m3_ft_signal_envelope COMMAND m3_ft_signal_envelope)
    set_tests_properties(m3_ft_signal_envelope PROPERTIES TIMEOUT 30)
    add_executable(m3_session_diagnostics tests/SessionDiagnosticsRegression.cpp)
    target_link_libraries(m3_session_diagnostics PRIVATE m3_diagnostics)
    add_test(NAME m3_session_diagnostics COMMAND m3_session_diagnostics)
    set_tests_properties(m3_session_diagnostics PROPERTIES TIMEOUT 30)
    find_package(${QT} REQUIRED COMPONENTS Test)
    add_executable(m3_decode_clipboard tests/DecodeTableClipboardRegression.cpp)
    target_include_directories(m3_decode_clipboard PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(m3_decode_clipboard PRIVATE ${QT}::Widgets ${QT}::Test)
    add_test(NAME m3_decode_clipboard COMMAND m3_decode_clipboard)
    set_tests_properties(m3_decode_clipboard PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 10)
    add_executable(m3_wavelog_client tests/WavelogClientRegression.cpp network/WavelogClient.cpp)
    target_include_directories(m3_wavelog_client PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")
    target_link_libraries(m3_wavelog_client PRIVATE ${QT}::Core ${QT}::Network)
    add_test(NAME m3_wavelog_client COMMAND m3_wavelog_client)
    set_tests_properties(m3_wavelog_client PROPERTIES TIMEOUT 30)
    add_executable(m3_ft_cq_directive tests/FtCqDirectiveRegression.cpp)
    target_include_directories(m3_ft_cq_directive PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(m3_ft_cq_directive PRIVATE ${QT}::Core)
    set_target_properties(m3_ft_cq_directive PROPERTIES AUTOMOC OFF AUTOUIC OFF AUTORCC OFF)
    add_test(NAME m3_ft_cq_directive COMMAND m3_ft_cq_directive)
    add_executable(m3_ft_cq_worklist tests/FtCqWorkListRegression.cpp)
    target_include_directories(m3_ft_cq_worklist PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(m3_ft_cq_worklist PRIVATE ${QT}::Core)
    set_target_properties(m3_ft_cq_worklist PROPERTIES AUTOMOC OFF AUTOUIC OFF AUTORCC OFF)
    add_test(NAME m3_ft_cq_worklist COMMAND m3_ft_cq_worklist)
    add_executable(m3_ft_incremental_producer tests/FtIncrementalProducerRegression.cpp)
    target_include_directories(m3_ft_incremental_producer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(m3_ft_incremental_producer PRIVATE ${QT}::Core Threads::Threads)
    add_test(NAME m3_ft_incremental_producer COMMAND m3_ft_incremental_producer)
    set_tests_properties(m3_ft_incremental_producer PROPERTIES TIMEOUT 10)
    add_executable(m3_update_regression tests/UpdateReleaseRegression.cpp
        updates/UpdateController.cpp updates/Ed25519Verify.cpp utils/RuntimeI18n.cpp
        settings/AppSettings.cpp rotator/RotatorPeakSearch.cpp)
    target_include_directories(m3_update_regression PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/generated")
    target_link_libraries(m3_update_regression PRIVATE ${QT}::Core ${QT}::Gui ${QT}::Widgets ${QT}::Network)
    add_test(NAME m3_update_regression COMMAND m3_update_regression)
    set_tests_properties(m3_update_regression PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 20)
    add_test(NAME m3_async_architecture_regression COMMAND MultiModeModem --architecture-regression)
    set_tests_properties(m3_async_architecture_regression PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 90)
    add_test(NAME m3_window_rotator_regression COMMAND MultiModeModem --window-rotator-regression)
    set_tests_properties(m3_window_rotator_regression PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 90)
    add_test(NAME m3_runtime_workers_regression COMMAND MultiModeModem --runtime-regression)
    set_tests_properties(m3_runtime_workers_regression PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 90)
    add_test(NAME m3_settings_ui_regression COMMAND MultiModeModem --ui-regression)
    set_tests_properties(m3_settings_ui_regression PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 90)
    add_executable(m3_tx_pull_regression tests/TxPullRegression.cpp audio/TxOutputDevice.h)
    target_include_directories(m3_tx_pull_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_tx_pull_regression PRIVATE ${QT}::Core ${QT}::Gui)
    add_test(NAME m3_tx_pull_regression COMMAND m3_tx_pull_regression)
    add_executable(m3_tx_level_regression tests/TxLevelRegression.cpp audio/TxLevel.h)
    target_include_directories(m3_tx_level_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    add_test(NAME m3_tx_level_regression COMMAND m3_tx_level_regression)
    add_executable(m3_i18n_regression tests/RuntimeI18nRegression.cpp utils/RuntimeI18n.cpp tests/i18n.qrc)
    target_include_directories(m3_i18n_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_i18n_regression PRIVATE ${QT}::Core)
    add_test(NAME m3_i18n_regression COMMAND m3_i18n_regression)
    add_executable(m3_rotator_coordinates tests/RotatorCoordinatesRegression.cpp)
    target_include_directories(m3_rotator_coordinates PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(m3_rotator_coordinates PROPERTIES AUTOMOC OFF AUTOUIC OFF AUTORCC OFF)
    add_test(NAME m3_rotator_coordinates COMMAND m3_rotator_coordinates)
    add_executable(m3_antialias_regression tests/AntiAliasRegression.cpp)
    target_include_directories(m3_antialias_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(m3_antialias_regression PROPERTIES AUTOMOC OFF AUTOUIC OFF AUTORCC OFF)
    add_test(NAME m3_antialias_regression COMMAND m3_antialias_regression)
    add_executable(m3_capture_clock tests/CaptureClockRegression.cpp)
    target_include_directories(m3_capture_clock PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_capture_clock PRIVATE ${QT}::Core)
    add_test(NAME m3_capture_clock COMMAND m3_capture_clock)

    add_executable(m3_resampler_streaming tests/ResamplerStreamingRegression.cpp dsp/text/LinearResampler.cpp)
    target_include_directories(m3_resampler_streaming PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_resampler_streaming PRIVATE ${QT}::Core)
    add_test(NAME m3_resampler_streaming COMMAND m3_resampler_streaming)
    add_executable(m3_logbook_regression tests/LogbookRegression.cpp logbook/AdifLogbook.cpp)
    target_include_directories(m3_logbook_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_logbook_regression PRIVATE ${QT}::Core)
    add_test(NAME m3_logbook_regression COMMAND m3_logbook_regression)
    add_executable(m3_wav_reader_test
        tests/WavFileReaderTest.cpp
        audio/WavFileReader.cpp
        audio/WavFileReader.h
    )
    target_include_directories(m3_wav_reader_test PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    target_link_libraries(m3_wav_reader_test PRIVATE ${QT}::Core)
    add_test(NAME m3_wav_reader_test COMMAND m3_wav_reader_test)

    add_executable(m3_audio_flow_accounting tests/AudioFlowAccountingRegression.cpp audio/BoundedAudioDispatcher.cpp audio/BoundedAudioDispatcher.h)
    target_include_directories(m3_audio_flow_accounting PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_audio_flow_accounting PRIVATE ${QT}::Core)
    add_test(NAME m3_audio_flow_accounting COMMAND m3_audio_flow_accounting)

    add_executable(m3_q65_native_regression
        tests/Q65NativeRegression.cpp
    )
    target_include_directories(m3_q65_native_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_q65_native_regression PRIVATE m3_q65 ${QT}::Core)
    add_test(NAME m3_q65_native_regression COMMAND m3_q65_native_regression)
    set_tests_properties(m3_q65_native_regression PROPERTIES TIMEOUT 240)

    add_executable(m3_js8_regression
        tests/Js8Regression.cpp
    )
    target_include_directories(m3_js8_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_js8_regression PRIVATE m3_js8 ${QT}::Core)
    add_test(NAME m3_js8_regression COMMAND m3_js8_regression)
    set_tests_properties(m3_js8_regression PROPERTIES TIMEOUT 300)

    # Random over-the-air payloads through the FT8/FT4/MSK144/Q65 and JS8
    # unpackers, the JS8 command buffer and the automatic-reply rules.
    add_executable(m3_unpack_fuzz_regression
        tests/UnpackFuzzRegression.cpp
    )
    target_include_directories(m3_unpack_fuzz_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_unpack_fuzz_regression PRIVATE m3_js8 m3_weak_signal_codec ${QT}::Core)
    add_test(NAME m3_unpack_fuzz_regression COMMAND m3_unpack_fuzz_regression)
    set_tests_properties(m3_unpack_fuzz_regression PROPERTIES TIMEOUT 120)

    add_executable(m3_msk144_native_regression
        tests/Msk144NativeRegression.cpp
    )
    target_include_directories(m3_msk144_native_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_msk144_native_regression PRIVATE m3_msk144 ${QT}::Core)
    add_test(NAME m3_msk144_native_regression COMMAND m3_msk144_native_regression)
    set_tests_properties(m3_msk144_native_regression PROPERTIES TIMEOUT 180)

    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(Python3_Interpreter_FOUND AND UNIX AND CMAKE_GENERATOR STREQUAL "Unix Makefiles")
        add_test(NAME m3_ft_reference_native COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/run_ft_reference_regression.py" --build "${CMAKE_BINARY_DIR}")
        set_tests_properties(m3_ft_reference_native PROPERTIES TIMEOUT 180 RUN_SERIAL TRUE)
        add_test(NAME m3_ft_native_frontend COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/run_ft_coherent_regression.py" --build "${CMAKE_BINARY_DIR}" --probe FtNativeFrontendRegression.cpp)
        set_tests_properties(m3_ft_native_frontend PROPERTIES TIMEOUT 120 RUN_SERIAL TRUE)
        add_test(NAME m3_ft_coherent_native COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/run_ft_coherent_regression.py" --build "${CMAKE_BINARY_DIR}")
        set_tests_properties(m3_ft_coherent_native PROPERTIES TIMEOUT 120 RUN_SERIAL TRUE)
    endif()

    if(Python3_Interpreter_FOUND)
        set(M3_GUARD_UNIX_ARGS)
        if(UNIX)
            set(M3_GUARD_UNIX_ARGS --with-unix-checks)
        endif()

        if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
            add_test(NAME m3_linux_update_helpers COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_update_helpers.py")
            add_test(NAME m3_linux_installer_preflight COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_linux_installer_preflight.py")
            add_test(NAME m3_update_publication COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_update_publication.py")
            set_tests_properties(m3_linux_update_helpers m3_linux_installer_preflight m3_update_publication PROPERTIES TIMEOUT 30)
            add_test(NAME m3_instance_lock COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_instance_lock.py" $<TARGET_FILE:MultiModeModem>)
            set_tests_properties(m3_instance_lock PROPERTIES TIMEOUT 180)
        endif()

        add_test(NAME m3_architecture_guard
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_guard_suite.py"
                    architecture ${M3_GUARD_UNIX_ARGS})
        add_test(NAME m3_ui_contest_guard
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_guard_suite.py"
                    ui ${M3_GUARD_UNIX_ARGS})
        add_test(NAME m3_ft_runtime_guard
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_guard_suite.py"
                    ft ${M3_GUARD_UNIX_ARGS})
        add_test(NAME m3_release_audit
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_guard_suite.py"
                    release ${M3_GUARD_UNIX_ARGS})
    endif()

    add_executable(m3_receiver_filter_regression
        tests/ReceiverFilterRegression.cpp dsp/WaterfallLeveler.cpp dsp/DspEngine.cpp)
    target_include_directories(m3_receiver_filter_regression PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(m3_receiver_filter_regression PRIVATE Qt${QT_VERSION_MAJOR}::Core)
    add_test(NAME m3_receiver_filter_regression COMMAND m3_receiver_filter_regression)
    add_executable(m3_waterfall_cq_regression
        tests/WaterfallCqRegression.cpp dsp/WaterfallLeveler.cpp dsp/DspEngine.cpp widgets/CqSpots.h
        modems/ft8/FtDecodeFilter.h modems/ft8/FtDecodeColours.h)
    target_include_directories(m3_waterfall_cq_regression PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(m3_waterfall_cq_regression PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui)
    add_test(NAME m3_waterfall_cq_regression COMMAND m3_waterfall_cq_regression)

    if(UNIX)
        add_test(NAME m3_cw_native_regression
            COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_cw_native_regression.sh"
                         "${CMAKE_CURRENT_BINARY_DIR}/tests-cw")
        add_test(NAME m3_waterfall_native_regression
            COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_waterfall_leveler_regression.sh"
                         "${CMAKE_CURRENT_BINARY_DIR}/tests-waterfall")
        if(Python3_Interpreter_FOUND)
            add_test(NAME m3_waterfall_integrity_guard
                COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_guard_suite.py"
                        waterfall --with-unix-checks)
        endif()
        set_tests_properties(m3_cw_native_regression
                             m3_waterfall_native_regression
                             PROPERTIES TIMEOUT 180)
    endif()

endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/rtty_rules")
    if(APPLE)
        install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/rtty_rules" DESTINATION "MultiModeModem.app/Contents/MacOS")
    else()
        install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/rtty_rules" DESTINATION "${CMAKE_INSTALL_BINDIR}")
    endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cw_rules")
    if(APPLE)
        install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cw_rules" DESTINATION "MultiModeModem.app/Contents/MacOS")
    else()
        install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cw_rules" DESTINATION "${CMAKE_INSTALL_BINDIR}")
    endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cty.csv")
    if(APPLE AND M3_MACOS_BUNDLE)
        install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cty.csv"
            DESTINATION MultiModeModem.app/Contents/MacOS
        )
    else()
        install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cty.csv"
            DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
    endif()
endif()

if(UNIX AND NOT APPLE)
    foreach(ICON_SIZE 16 24 32 48 64 128 256 512 1024)
        install(FILES icons/multimodemodem_${ICON_SIZE}.png
            DESTINATION share/icons/hicolor/${ICON_SIZE}x${ICON_SIZE}/apps
            RENAME multimodemodem.png
        )
    endforeach()
    install(FILES icons/multimodemodem_256.png
        DESTINATION share/icons/hicolor/256x256/apps
        RENAME MultiModeModem.png
    )
    install(FILES MultiModeModem.desktop
        DESTINATION share/applications
    )
endif()

# -------------------------------------------------------------------------
# Optional FT parser/sequencer regression test harness
# -------------------------------------------------------------------------
option(M3_BUILD_FT_TESTS
    "Build a small QtCore-only FT parser/sequencer regression test executable."
    ${BUILD_TESTING})

if(M3_BUILD_FT_TESTS)
    add_executable(m3_ft_sequencer_selftest
        tests/FtSequencerSelfTest.cpp
        modems/ft8/FtDecodedText.h
        modems/ft8/FtDecodedText.cpp
        modems/ft8/FtQsoSequencer.h
        modems/ft8/FtQsoSequencer.cpp
        modems/ft8/FtQsoSession.h
        modems/ft8/FtQsoSession.cpp
        modems/ft8/FtTxPlan.h
    )
    target_include_directories(m3_ft_sequencer_selftest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_ft_sequencer_selftest PRIVATE ${QT}::Core)
    if(BUILD_TESTING)
        add_test(NAME m3_ft_sequencer COMMAND m3_ft_sequencer_selftest)
    endif()
endif()

# -------------------------------------------------------------------------
# Optional selected-tone CW acquisition/speed regression test
# -------------------------------------------------------------------------
option(M3_BUILD_CW_TESTS
    "Build the pure-C++ selected-tone CW regression test executable."
    OFF)

if(M3_BUILD_CW_TESTS)
    add_executable(M3CwNativeRegression
        tests/CwNativeRegression.cpp
    )
    target_include_directories(M3CwNativeRegression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(M3CwNativeRegression PRIVATE m3_cwskimmer)
endif()

if(BUILD_TESTING)
    add_executable(m3_all_contest_exports_regression
        tests/AllContestExportsRegression.cpp logbook/ContestExchange.cpp dxcc/CtyCountryFile.cpp)
    target_include_directories(m3_all_contest_exports_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_all_contest_exports_regression PRIVATE ${QT}::Core ${QT}::Gui)
    add_custom_command(TARGET m3_all_contest_exports_regression POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/cty.csv $<TARGET_FILE_DIR:m3_all_contest_exports_regression>/cty.csv
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/rtty_rules $<TARGET_FILE_DIR:m3_all_contest_exports_regression>/rtty_rules
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/cw_rules $<TARGET_FILE_DIR:m3_all_contest_exports_regression>/cw_rules)
    add_test(NAME m3_all_contest_exports_regression COMMAND m3_all_contest_exports_regression)

    add_executable(m3_logbook_workflow_regression
        tests/LogbookWorkflowRegression.cpp tests/i18n.qrc
        dialogs/LogbookDialog.cpp dialogs/LogbookDialog.h
        dialogs/ContestExportWizard.cpp dialogs/ContestExportWizard.h
        logbook/ContestExchange.cpp logbook/AdifLogbook.cpp
        widgets/QsoMapWidget.cpp widgets/QsoMapWidget.h
        settings/AppSettings.cpp rotator/RotatorPeakSearch.cpp
        dxcc/CtyCountryFile.cpp utils/RuntimeI18n.cpp utils/CockpitTheme.cpp utils/WindowResize.cpp)
    target_include_directories(m3_logbook_workflow_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_logbook_workflow_regression PRIVATE ${QT}::Core ${QT}::Gui ${QT}::Widgets ${QT}::Network ${QT}::PrintSupport)
    add_test(NAME m3_logbook_workflow_regression COMMAND m3_logbook_workflow_regression)
    set_tests_properties(m3_logbook_workflow_regression PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 30)

    add_executable(m3_contest_export_regression
        tests/ContestExportRegression.cpp tests/i18n.qrc
        dialogs/ContestExportWizard.cpp dialogs/ContestExportWizard.h
        logbook/ContestExchange.cpp logbook/ContestExchange.h
        dxcc/CtyCountryFile.cpp utils/RuntimeI18n.cpp utils/CockpitTheme.cpp utils/WindowResize.cpp)
    target_include_directories(m3_contest_export_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_contest_export_regression PRIVATE ${QT}::Core ${QT}::Gui ${QT}::Widgets)
    add_custom_command(TARGET m3_contest_export_regression POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/cty.csv $<TARGET_FILE_DIR:m3_contest_export_regression>/cty.csv)
    add_custom_command(TARGET m3_contest_export_regression POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/rtty_rules $<TARGET_FILE_DIR:m3_contest_export_regression>/rtty_rules
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/cw_rules $<TARGET_FILE_DIR:m3_contest_export_regression>/cw_rules)
    add_test(NAME m3_contest_export_regression COMMAND m3_contest_export_regression)
    set_tests_properties(m3_contest_export_regression PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 30)

    add_executable(m3_cw_integration_regression
        tests/CwIntegrationRegression.cpp tests/CwTestSignal.h
        modems/cw/CwDecoder.cpp modems/cw/CwDecoder.h
        modems/cw/neural/CwNeuralDecoder.cpp modems/cw/neural/CwNeuralDecoder.h
        modems/cw/CwBandSkimmer.cpp modems/cw/CwBandSkimmer.h modems/cw/CwBandSkimmerWorker.h
        dsp/text/LinearResampler.cpp)
    target_include_directories(m3_cw_integration_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_cw_integration_regression PRIVATE
        m3_cwskimmer ${QT}::Core ${QT}::Gui)
    add_test(NAME m3_cw_integration_regression COMMAND m3_cw_integration_regression)

    add_executable(m3_cw_ctc_prefix_regression tests/CwCtcPrefixRegression.cpp)
    target_include_directories(m3_cw_ctc_prefix_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    add_test(NAME m3_cw_ctc_prefix_regression COMMAND m3_cw_ctc_prefix_regression)

    add_executable(m3_cw_route_probe
        tests/CwRouteProbe.cpp
        modems/cw/CwDecoder.cpp modems/cw/CwDecoder.h
        modems/cw/neural/CwNeuralDecoder.cpp modems/cw/neural/CwNeuralDecoder.h
        modems/cw/CwBandSkimmer.cpp modems/cw/CwBandSkimmer.h
        dsp/text/LinearResampler.cpp)
    target_include_directories(m3_cw_route_probe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_cw_route_probe PRIVATE m3_cwskimmer ${QT}::Core ${QT}::Gui)

    add_executable(m3_cw_engine_regression
        tests/CwEngineIntegrationRegression.cpp
        modems/cw/CwDecoder.cpp modems/cw/CwDecoder.h
        modems/cw/neural/CwNeuralDecoder.cpp modems/cw/neural/CwNeuralDecoder.h
        modems/cw/CwBandSkimmer.cpp modems/cw/CwBandSkimmer.h
        dsp/text/LinearResampler.cpp)
    target_include_directories(m3_cw_engine_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_cw_engine_regression PRIVATE
        m3_cwskimmer ${QT}::Core ${QT}::Gui)
    add_test(NAME m3_cw_engine_regression COMMAND m3_cw_engine_regression)

    add_executable(m3_cw_native_probe
        tests/CwNativeProbe.cpp
        modems/cw/CwDecoder.cpp modems/cw/CwDecoder.h
        modems/cw/neural/CwNeuralDecoder.cpp modems/cw/neural/CwNeuralDecoder.h
        modems/cw/CwBandSkimmer.cpp modems/cw/CwBandSkimmer.h
        dsp/text/LinearResampler.cpp)
    target_include_directories(m3_cw_native_probe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_cw_native_probe PRIVATE
        m3_cwskimmer ${QT}::Core ${QT}::Gui)

    add_executable(m3_cw_neural_probe
        tests/CwNeuralProbe.cpp modems/cw/neural/CwNeuralDecoder.cpp)
    target_include_directories(m3_cw_neural_probe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(m3_cw_neural_probe PROPERTIES AUTOMOC OFF AUTOUIC OFF AUTORCC OFF)

    add_executable(m3_psk_reporter tests/PskReporterRegression.cpp network/PskReporter.cpp)
    target_include_directories(m3_psk_reporter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_psk_reporter PRIVATE ${QT}::Core ${QT}::Network)
    add_test(NAME m3_psk_reporter COMMAND m3_psk_reporter)
    set_tests_properties(m3_psk_reporter PROPERTIES TIMEOUT 20)

    add_executable(m3_rtty_contest_regression tests/RttyContestRegression.cpp
        modems/rtty/RttyDecoder.cpp modems/rtty/RttyMultiDecoder.cpp
        modems/rtty/tx/RttyTransmitter.cpp dsp/common/DspConditioner.cpp
        logbook/AdifLogbook.cpp dxcc/CtyCountryFile.cpp network/QsoUdpBroadcaster.cpp)
    target_include_directories(m3_rtty_contest_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_rtty_contest_regression PRIVATE m3_linear_resampler ${QT}::Core ${QT}::Gui ${QT}::Network)
    add_custom_command(TARGET m3_rtty_contest_regression POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/cty.csv $<TARGET_FILE_DIR:m3_rtty_contest_regression>/cty.csv)
    add_test(NAME m3_rtty_contest_regression COMMAND m3_rtty_contest_regression)

    add_executable(m3_rtty_synthetic_bench
        tests/RttySyntheticBench.cpp
        modems/rtty/RttyDecoder.cpp)
    target_include_directories(m3_rtty_synthetic_bench PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_rtty_synthetic_bench PRIVATE ${QT}::Core ${QT}::Gui)
    add_test(NAME m3_rtty_synthetic_bench COMMAND m3_rtty_synthetic_bench)

    add_executable(m3_text_assist_workers_regression
        tests/TextAssistWorkersRegression.cpp
        runtime/LogbookIndexWorker.h
        runtime/LogbookIndexWorker.cpp
        runtime/TextAssistWorker.h
        runtime/TextAssistWorker.cpp
        logbook/AdifLogbook.cpp
        dxcc/CtyCountryFile.cpp)
    target_include_directories(m3_text_assist_workers_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_text_assist_workers_regression PRIVATE ${QT}::Core)
    add_custom_command(TARGET m3_text_assist_workers_regression POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/cty.csv $<TARGET_FILE_DIR:m3_text_assist_workers_regression>/cty.csv)
    add_test(NAME m3_text_assist_workers_regression COMMAND m3_text_assist_workers_regression)

    add_executable(m3_installation_paths_regression tests/InstallationPathsRegression.cpp)
    target_link_libraries(m3_installation_paths_regression PRIVATE ${QT}::Core)
    add_test(NAME m3_installation_paths_regression COMMAND m3_installation_paths_regression)
    add_executable(m3_installer_cpu_regression tests/InstallerCpuRegression.cpp)
    set_target_properties(m3_installer_cpu_regression PROPERTIES AUTOMOC OFF AUTOUIC OFF AUTORCC OFF)
    add_test(NAME m3_installer_cpu_regression COMMAND m3_installer_cpu_regression)
endif()

if(BUILD_TESTING)
    add_executable(m3_gpu_sync_regression tests/FtGpuSyncRegression.cpp dsp/gpu/FtSyncCompute.cpp)
    target_include_directories(m3_gpu_sync_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/third_party/opencl_headers)
    target_link_libraries(m3_gpu_sync_regression PRIVATE ${QT}::Core)
    add_test(NAME m3_gpu_sync_fallback COMMAND m3_gpu_sync_regression)
    set_tests_properties(m3_gpu_sync_fallback PROPERTIES TIMEOUT 30)

    add_executable(m3_hell_image_regression
        tests/HellschreiberImageRegression.cpp
        modems/hell/HellschreiberDecoder.cpp)
    target_include_directories(m3_hell_image_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_hell_image_regression PRIVATE ${QT}::Core ${QT}::Gui)
    add_test(NAME m3_hell_image_regression COMMAND m3_hell_image_regression)
    add_test(NAME m3_hell_image_extended COMMAND m3_hell_image_regression --extended)
    set_tests_properties(m3_hell_image_regression m3_hell_image_extended PROPERTIES TIMEOUT 90)

    add_executable(m3_weatherfax_image_regression
        tests/WeatherFaxImageRegression.cpp
        modems/weatherfax/WeatherFaxDecoder.cpp modems/weatherfax/WeatherFaxDecoder.h
        modems/weatherfax/WeatherFaxIqFilter.h dsp/FrequencyTracker.cpp)
    target_include_directories(m3_weatherfax_image_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_weatherfax_image_regression PRIVATE ${QT}::Core ${QT}::Gui)
    add_test(NAME m3_weatherfax_image_regression COMMAND m3_weatherfax_image_regression)
    add_test(NAME m3_weatherfax_image_extended COMMAND m3_weatherfax_image_regression --extended)
    set_tests_properties(m3_weatherfax_image_regression m3_weatherfax_image_extended PROPERTIES TIMEOUT 30)
endif()

if(BUILD_TESTING)
    add_executable(m3_sstv_vis_regression tests/SstvVisRegression.cpp
        modems/sstv/SstvDecoder.cpp modems/sstv/tx/SstvTransmitter.cpp
        third_party/mmsstv_lgpl/MmsstvRxCore.cpp)
    target_include_directories(m3_sstv_vis_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_sstv_vis_regression PRIVATE ${QT}::Core ${QT}::Gui)
    add_test(NAME m3_sstv_vis_regression COMMAND m3_sstv_vis_regression)
    set_tests_properties(m3_sstv_vis_regression PROPERTIES TIMEOUT 60)
endif()

if(BUILD_TESTING)
    add_executable(m3_sstv_editor_regression tests/SstvEditorRegression.cpp
        dialogs/SstvImageEditorDialog.cpp utils/RuntimeI18n.cpp modems/sstv/tx/SstvTransmitter.cpp)
    target_include_directories(m3_sstv_editor_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_sstv_editor_regression PRIVATE ${QT}::Core ${QT}::Gui ${QT}::Widgets)
    add_test(NAME m3_sstv_editor_regression COMMAND m3_sstv_editor_regression)
    set_tests_properties(m3_sstv_editor_regression PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 30)
endif()

if(BUILD_TESTING)
    add_executable(m3_sstv_image_adjustment_regression
        tests/SstvImageAdjustmentRegression.cpp modems/sstv/SstvDecoder.cpp
        modems/sstv/tx/SstvTransmitter.cpp third_party/mmsstv_lgpl/MmsstvRxCore.cpp)
    target_include_directories(m3_sstv_image_adjustment_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_sstv_image_adjustment_regression PRIVATE ${QT}::Core ${QT}::Gui)
    add_test(NAME m3_sstv_image_adjustment_regression COMMAND m3_sstv_image_adjustment_regression)
    set_tests_properties(m3_sstv_image_adjustment_regression PROPERTIES TIMEOUT 90)
endif()

if(BUILD_TESTING)
    add_executable(m3_sstv_archive_regression tests/SstvArchiveRegression.cpp runtime/SstvImageArchive.cpp)
    target_include_directories(m3_sstv_archive_regression PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(m3_sstv_archive_regression PRIVATE ${QT}::Core ${QT}::Gui)
    add_test(NAME m3_sstv_archive_regression COMMAND m3_sstv_archive_regression)
    set_tests_properties(m3_sstv_archive_regression PROPERTIES TIMEOUT 30)
endif()

if(TARGET m3_audio_flow_accounting)
    target_link_libraries(m3_audio_flow_accounting PRIVATE m3_diagnostics)
endif()

if(BUILD_TESTING)
    # Tests read and write settings; keep them away from the operator's
    # ~/.config/MultiModeModem (see M3Installation::configDirectory).
    get_property(_m3_all_tests DIRECTORY PROPERTY TESTS)
    set_property(TEST ${_m3_all_tests} APPEND PROPERTY ENVIRONMENT "M3_CONFIG_DIR=${CMAKE_BINARY_DIR}/test-config")
endif()
