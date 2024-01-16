arm-linux-gnueabihf-g++ $1 $2 -o $3 \
-I /opt/EmbedSky/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/include/ \
-Wl,-rpath-link=/opt/EmbedSky/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/lib/ \
-Wl,-rpath-link=/opt/EmbedSky/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/qt5.5/rootfs_imx6q_V3_qt5.5_env/lib/ \
-Wl,-rpath-link=/opt/EmbedSky/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/qt5.5/rootfs_imx6q_V3_qt5.5_env/qt5.5_env/lib/ \
-Wl,-rpath-link=/opt/EmbedSky/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/qt5.5/rootfs_imx6q_V3_qt5.5_env/usr/lib/ \
-lpthread \
-I /home/user/tensorflow_lite_libs_cpp/include \
/home/user/tensorflow_build/libtensorflow-lite.a \
-std=c++11 \
-ldl \
-I/usr/local/include/opencv \
-I/usr/local/include/  \
-L/home/user/opencv/platforms/linux/build_hardfp/lib \
-lopencv_dnn -lopencv_highgui -lopencv_ml -lopencv_objdetect -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_videoio -lopencv_imgcodecs -lopencv_features2d -lopencv_video -lopencv_photo -lopencv_imgproc -lopencv_flann -lopencv_core \
/home/user/app/liblibtiff.a \
/home/user/app/libabsl_time_zone.a \
/home/user/app/libabsl_strings_internal.a \
/home/user/app/libabsl_throw_delegate.a \
/home/user/app/libabsl_flags_config.a \
/home/user/app/libabsl_bad_optional_access.a \
/home/user/app/libabsl_symbolize.a \
/home/user/app/libtegra_hal.a \
/home/user/app/libabsl_base.a \
/home/user/app/libabsl_debugging_internal.a \
/home/user/app/libabsl_raw_logging_internal.a \
/home/user/app/libittnotify.a \
/home/user/app/libabsl_flags_internal.a \
/home/user/app/libabsl_flags_registry.a \
/home/user/app/libabsl_stacktrace.a \
/home/user/app/libabsl_graphcycles_internal.a \
/home/user/app/libabsl_city.a \
/home/user/app/libabsl_civil_time.a \
/home/user/app/libzlib.a \
/home/user/app/libabsl_str_format_internal.a \
/home/user/app/libabsl_cord.a \
/home/user/app/libabsl_demangle_internal.a \
/home/user/app/libabsl_status.a \
/home/user/app/libabsl_hash.a \
/home/user/app/liblibwebp.a \
/home/user/app/libabsl_malloc_internal.a \
/home/user/app/liblibjasper.a \
/home/user/app/libabsl_spinlock_wait.a \
/home/user/app/libruy.a \
/home/user/app/liblibpng.a \
/home/user/app/libabsl_log_severity.a \
/home/user/app/libabsl_time.a \
/home/user/app/libfft2d_fftsg.a \
/home/user/app/libtensorflow-lite.a \
/home/user/app/libfarmhash.a \
/home/user/app/liblibjpeg-turbo.a \
/home/user/app/libabsl_synchronization.a \
/home/user/app/liblibprotobuf.a \
/home/user/app/libabsl_bad_variant_access.a \
/home/user/app/libabsl_dynamic_annotations.a \
/home/user/app/libabsl_flags_program_name.a \
/home/user/app/libabsl_strings.a \
/home/user/app/libflatbuffers.a \
/home/user/app/libquirc.a \
/home/user/app/libabsl_flags.a \
/home/user/app/libabsl_int128.a \
/home/user/app/libfft2d_fftsg2d.a \
/home/user/app/libabsl_flags_marshalling.a \
/home/user/opencv/platforms/linux/build_hardfp/lib/libopencv_ts.a

