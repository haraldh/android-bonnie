LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:=	\
bonnie++.cpp bon_io.cpp bon_file.cpp bon_time.cpp semaphore.cpp \
 sync.cpp thread.cpp bon_suid.cpp duration.cpp rand.cpp util.cpp ipc_sysv.c

LOCAL_MODULE:= bonnie

 LOCAL_CFLAGS +=		\
	-D__ARM__			\
	-DIS_ANDROID=1		\
	-fPIE

LOCAL_LDFLAGS += -pie

include $(BUILD_EXECUTABLE)
